#pragma once
#include <opencv2/core.hpp>
namespace mf
{
    float gauss(float t, float r, float window_size);

    class RealTimeSinglePathOptimizer
    {
    public:
        int buffer_size;
        int iterations;
        int window_size;
        float beta;
        float lambda_t;
        cv::Mat W;
        int i;
        int j;
        std::vector<float> y;
        cv::Mat d;
        std::vector<float> Motion;

        RealTimeSinglePathOptimizer(int i, int j, cv::Mat& W, int buffer_size, int window_size);

        //"""
        //	@param : c is camera trajectory within the buffer
        //	Returns :
        //returns an realtime optimized smooth camera trajectory
        //	"""
        void OptimizeNext(std::vector<cv::Mat> const& c);
    };
    typedef std::shared_ptr< RealTimeSinglePathOptimizer> RealTimeSinglePathOptimizerPtr;
}
