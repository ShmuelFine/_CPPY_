#include "RT_Stab.h"


#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>

#include "MeshFlow.h"
//#include "range.h"
#include "Utils.h"
#include "Optimization.h"

#include <iostream>

using namespace cv;
using namespace std;


namespace mf
{
	class OutVideoWriter
	{
	public:
		int frame_rate;
		int frame_width;
		int frame_height;
		int frame_count;
		int fourcc;
		VideoWriter out;
		//int frame_num;

		OutVideoWriter(VideoCapture& inputCap, string const& outputPath)
		{
			// get video properties
			this->frame_rate = int(inputCap.get(CAP_PROP_FPS));
			this->frame_width = int(inputCap.get(CAP_PROP_FRAME_WIDTH));
			this->frame_height = int(inputCap.get(CAP_PROP_FRAME_HEIGHT));
			this->frame_count = int(inputCap.get(CAP_PROP_FRAME_COUNT));

			// generate stabilized video                                                                                                    ;
			this->fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D');
			this->out = VideoWriter(outputPath + ".avi", this->fourcc, this->frame_rate, Size(this->frame_width, this->frame_height));

			//this->frame_num = 0;
		}

		void WrapNext(Mat& frame, Mat& new_x_motion_mesh, Mat& new_y_motion_mesh, int HORIZONTAL_BORDER, int VERTICAL_BORDER)
		{
			// mesh warping
			Mat new_frame = mesh_warp_frame(frame, new_x_motion_mesh, new_y_motion_mesh);
			new_frame = new_frame(Rect(HORIZONTAL_BORDER, VERTICAL_BORDER, new_frame.cols - HORIZONTAL_BORDER * 2, new_frame.rows - VERTICAL_BORDER * 2));
			resize(new_frame, new_frame, frame.size());// , interpolation = INTER_CUBIC)
			this->out.write(new_frame);
		}

		void Finish()
		{
			this->out.release();
		}

	};

	struct ShiTomasi_params
	{
		int maxCorners;
		float qualityLevel;
		int minDistance;
		int blockSize;
	};
	struct LK_params
	{
		Size winSize;
		int maxLevel;
		TermCriteria criteria;
	};
	class RT_VideoStab
	{
	public:
		VideoCapture cap;
		shared_ptr<OutVideoWriter> output;
		ShiTomasi_params feature_params;
		LK_params lk_params;

		int buffer_size;
		int window_size;
		Mat old_frame, old_gray;
		Vec2i ImgSize;
		int HORIZONTAL_BORDER;
		int VERTICAL_BORDER;

		Vec2i VtxGridSize;

		vector<Mat> x_paths, y_paths;

		Mat W;

		vector<vector<RealTimeSinglePathOptimizerPtr>> PathOptimizers_x;
		vector<vector<RealTimeSinglePathOptimizerPtr>> PathOptimizers_y;

		Mat new_x_motion_mesh, new_y_motion_mesh;

		RT_VideoStab(string const& file_name)
		{
			this->cap = VideoCapture(file_name);
			this->output.reset(new OutVideoWriter(this->cap, R"(C:\Dev\Stabilization\MeshFlow4Android\outTest_cpp)"));

			// block of size : mesh
			//PIXELS = 16;
			// motion propogation radius
			//RADIUS = 300;
			// params for (auto ShiTomasi corner detection
			this->feature_params.maxCorners = 1000;
			this->feature_params.qualityLevel = 0.3f;
			this->feature_params.minDistance = 7;
			this->feature_params.blockSize = 7;

			// Parameters for (auto lucas kanade optical flow
			this->lk_params.winSize = Size(15, 15);
			this->lk_params.maxLevel = 2;
			this->lk_params.criteria = TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 20, 0.03);


			//Optimization related {
			this->buffer_size = 200;
			this->window_size = 32;

			// Take first frame
			this->cap.set(CAP_PROP_POS_FRAMES, 0);
			auto ret = this->cap.read(this->old_frame);
			cvtColor(this->old_frame, this->old_gray, COLOR_BGR2GRAY);
			this->ImgSize = Vec2i(this->old_gray.size().width, this->old_gray.size().height);
			this->HORIZONTAL_BORDER = int(30);
			this->VERTICAL_BORDER = int((this->HORIZONTAL_BORDER * this->ImgSize[1]) / this->ImgSize[0]);

			// path parameters
			this->VtxGridSize = Vec2i(int(this->ImgSize[0] / PIXELS), int(this->ImgSize[1] / PIXELS));

			this->x_paths.push_back(Mat::zeros(Size(this->VtxGridSize[0], this->VtxGridSize[1]), CV_32FC1));
			this->y_paths.push_back(Mat::zeros(Size(this->VtxGridSize[0], this->VtxGridSize[1]), CV_32FC1));

			this->W = Mat::zeros(Size(this->buffer_size, this->buffer_size), CV_32FC1);
			for (int i = 0; i < this->buffer_size; i++)
			{
				for (int j = 0; j < this->buffer_size; j++)
				{
					PIX(W, i, j) = gauss(i, j, this->window_size);
				}
			}
			for (int i = 0; i < this->VtxGridSize[0]; i++)
			{
				vector<RealTimeSinglePathOptimizerPtr> currRow_X, currRow_Y;

				for (int j = 0; j < this->VtxGridSize[1]; j++)
				{
					currRow_X.push_back(make_shared<RealTimeSinglePathOptimizer>(i, j, W, this->buffer_size, this->window_size));
					currRow_Y.push_back(make_shared<RealTimeSinglePathOptimizer>(i, j, W, this->buffer_size, this->window_size));
				}
				this->PathOptimizers_x.push_back(currRow_X);
				this->PathOptimizers_y.push_back(currRow_Y);
			}

			this->OptimizePaths();
			this->WarpLastFrame();
		}

		void WarpLastFrame()
		{
			this->output->WrapNext(this->old_frame, this->new_x_motion_mesh, this->new_y_motion_mesh, this->HORIZONTAL_BORDER, this->VERTICAL_BORDER);
		}

		void OptimizePaths()
		{
			for (int i = 0; i < this->VtxGridSize[0]; i++)
			{
				for (int j = 0; j < this->VtxGridSize[1]; j++)
				{
					this->PathOptimizers_x[i][j]->OptimizeNext(this->x_paths);
					this->PathOptimizers_y[i][j]->OptimizeNext(this->y_paths);
				}
			}

			//  sx_paths, sy_paths  are the Y vectors of the optimizers.
			if (new_x_motion_mesh.empty()) new_x_motion_mesh = Mat(Size(this->VtxGridSize[0], this->VtxGridSize[1]), CV_32FC1);
			if (new_y_motion_mesh.empty()) new_y_motion_mesh = Mat(Size(this->VtxGridSize[0], this->VtxGridSize[1]), CV_32FC1);

			for (int i = 0; i < this->VtxGridSize[0]; i++)
			{
				for (int j = 0; j < this->VtxGridSize[1]; j++)
				{
					PIX(this->new_x_motion_mesh, i, j) = this->PathOptimizers_x[i][j]->Motion.back();
					PIX(this->new_y_motion_mesh, i, j) = this->PathOptimizers_y[i][j]->Motion.back();
				}
			}

		}

		bool ProcessNextFrame()
		{
			Mat frame;
			auto ret = this->cap.read(frame);
			if (!ret)
			{
				return false;
			}

			Mat frame_gray;
			cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

			// find corners : it
			vector<Point2f> p0, p1;
			goodFeaturesToTrack(this->old_gray, p0, this->feature_params.maxCorners, this->feature_params.qualityLevel, this->feature_params.minDistance, /*mask = */Mat(), this->feature_params.blockSize);

			vector<uchar> st;
			vector<float> err;
			calcOpticalFlowPyrLK(this->old_gray, frame_gray, p0, p1, st, err, this->lk_params.winSize, this->lk_params.maxLevel, this->lk_params.criteria);
			// calculate optical flow

			// Select good points
			vector<Vec2f> good_new, good_old;
			for (int ptIdx = 0; ptIdx < p0.size(); ptIdx++)
			{
				if (st[ptIdx])
				{
					good_new.push_back(p1[ptIdx]);
					good_old.push_back(p0[ptIdx]);
				}
			}

			// estimate motion mesh for old_frame
			Mat x_motion_mesh, y_motion_mesh;
			motion_propagate(good_old, good_new, frame, x_motion_mesh, y_motion_mesh);


			// generate vertex profiles
			// TODO{ Shouldn't accamulate all the path, just the recent buffer_size.
			generate_vertex_profiles(this->x_paths, this->y_paths, x_motion_mesh, y_motion_mesh);

			// updates frames
			this->old_frame = frame;
			this->old_gray = frame_gray;

			this->OptimizePaths();
			this->WarpLastFrame();

			return true;
		}

		void Finish()
		{
			this->output->Finish();
			this->cap.release();
		}

	};
	
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "ERR";
	}

	// get video properties
	string file_name = argv[1];

	mf::RT_VideoStab s(file_name);

	while (s.ProcessNextFrame())
	{
		// empty;
	}

	s.Finish();

	return 0;
}