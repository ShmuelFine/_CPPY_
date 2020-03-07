#pragma once

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

namespace mf
{
	// block of size : mesh
	static const float PIXELS = 16;
	;
	// motion propogation radius                                                                        ;
	static const float RADIUS = 300;

	void motion_propagate(
		std::vector<cv::Vec2f> const& old_points,
		std::vector<cv::Vec2f> const& new_points,
		cv::Mat const& old_frame,
		cv::Mat& out_x_Mesh, cv::Mat& out_y_Mesh);

	void generate_vertex_profiles(std::vector<cv::Mat>& x_paths, std::vector<cv::Mat>& y_paths,
		cv::Mat const& x_motion_mesh, cv::Mat const& y_motion_mesh);

	cv::Mat mesh_warp_frame(cv::Mat const& frame, cv::Mat const& x_motion_mesh, cv::Mat const& y_motion_mesh);


}