#include "Utils.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include "range.h"


using namespace cv;
using namespace std;
using namespace py;

namespace mf
{
	float& PIX(Mat& m, std::pair<int, int> const& coords) {
		return m.at<float>(Point(coords.first, coords.second));
	}

	float& PIX(Mat& m, int i, int j) {
		return m.at<float>(Point(i, j));
	}

	float PIX(Mat const& m, int i, int j) {
		return m.at<float>(Point(i, j));
	}

	//float& PIX(Mat& m, float i, float j) {
	//	return m.at<float>(Point((int)round(i), (int)round(j)));
	//}
}