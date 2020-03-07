#include "Optimization.h"

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
//#include "range.h"
#include "Utils.h"

using namespace cv;
using namespace std;


namespace mf
{
	//    """
//    @param : window_size is the size of window over which gaussian to be applied
//    @param : t is the index of current point
//    @param : r is the index of point : window

//    Return :
//returns spacial guassian weights over a window size
//    """

	float gauss(float t, float r, float window_size)
	{
		if (abs(r - t) > window_size)
			return 0;
		else
			return exp((pow(-9 * (r - t), 2)) / pow(window_size, 2));
	}



	RealTimeSinglePathOptimizer::RealTimeSinglePathOptimizer(int i, int j, Mat& W, int buffer_size, int window_size)
	{
		this->buffer_size = buffer_size; //200
		this->iterations = 10;
		this->window_size = window_size; //32
		this->beta = 1;
		this->lambda_t = 100;
		this->W = W;
		this->i = i;
		this->j = j;
		//this->y = Mat();
		this->d = Mat();
		//this->Motion = [];
	}

	void RealTimeSinglePathOptimizer::OptimizeNext(std::vector<cv::Mat> const& c)
	{
		int t = c.size();
		int currBufSize = min(t, this->buffer_size);
		cv::Mat C_Win(currBufSize, 1, CV_32FC1);
		float* C_Win_ptr = C_Win.ptr<float>();
		for (int time = max(t - this->buffer_size, 0); time < t; time++)
		{
			*C_Win_ptr++ = PIX(c[time], i, j);
		}

		cv::Mat W_Win = (t >= W.cols) ? W : W(Rect(0, 0, t, t));
		Mat P = C_Win;
		Mat Ones = Mat::ones(Size(1, W_Win.cols), W_Win.type());
		Mat W_RowSums = W_Win * Ones;

		if (t < this->buffer_size + 1)
		{
			if (!this->d.empty())
			{
				for (int _ = 0; _ < this->iterations; _++)
				{
					Mat alpha = C_Win + this->lambda_t * W_Win * P;
					alpha(Rect(0, 0, 1, alpha.rows - 1)) = alpha(Rect(0, 0, 1, alpha.rows - 1)) + this->beta * this->d;
					Mat gamma = 1 + this->lambda_t * W_RowSums;
					gamma(Rect(0, 0, 1, gamma.rows - 1)) = gamma(Rect(0, 0, 1, gamma.rows - 1)) + this->beta;
					divide(alpha, gamma, P);
				}
			}
		}
		else
		{
			for (int _ = 0; _ < this->iterations; _++)
			{
				Mat alpha = C_Win + this->lambda_t * this->W * P;
				alpha(Rect(0, 0, 1, alpha.rows - 1)) = alpha(Rect(0, 0, 1, alpha.rows - 1)) + this->beta * this->d(Rect(0, 1, 1, d.rows - 1));
				//alpha[:-1] = alpha[:-1] + this->beta * this->d[1:];
				Mat gamma = 1 + this->lambda_t * W_RowSums;
				gamma(Rect(0, 0, 1, gamma.rows - 1)) = gamma(Rect(0, 0, 1, gamma.rows - 1)) + this->beta;
				divide(alpha, gamma, P);
			}
		}

		this->d = P;
		this->y.push_back(P.ptr<float>()[P.size().area() - 1]);
		this->Motion.push_back(P.ptr<float>()[P.size().area() - 1] - C_Win.ptr<float>()[C_Win.size().area() - 1]);

	}
}