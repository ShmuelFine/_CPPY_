#pragma once

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

namespace mf
{
	float& PIX(cv::Mat& m, std::pair<int, int> const& coords);
	float& PIX(cv::Mat& m, int i, int j);
	//float& PIX(cv::Mat& m, float i, float j);
	float PIX(cv::Mat const& m, int i, int j);

}

#ifdef _DEBUG
	#pragma comment(lib, "opencv_world412d.lib")
#else
	#pragma comment(lib, "opencv_world412.lib")
#endif