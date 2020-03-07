
#include "MeshFlow.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include "Utils.h"
#include "range.h"

using namespace cv;
using namespace std;
using namespace py;

namespace mf
{
	// block of size : mesh
	const float PIXELS = 16;
	;
	// motion propogation radius                                                                        ;
	const float RADIUS = 300;

	//@param { H is homography matrix of dimension(3x3)  
	//@param { pt is the(x, y) point to be transformed   
	//Return {                                           
	//returns a transformed point ptrans = H * pt)
	Vec2f point_transform(Matx33f const& H, Vec2f const& pt)
	{

		float a = H(0, 0) * pt(0) + H(0, 1) * pt(1) + H(0, 2);
		float b = H(1, 0) * pt(0) + H(1, 1) * pt(1) + H(1, 2);
		float c = H(2, 0) * pt(0) + H(2, 1) * pt(1) + H(2, 2);
		return Vec2f(a / c, b / c);
	}

	//@param { old_points are points : old_frame that are      
	//matched feature points with new_frame                    
	//@param { new_points are points : new_frame that are      
	//matched feature points with old_frame                    
	//@param { old_frame is the frame to which                 
	//motion mesh needs to be obtained                         
	//@param { H is the homography between old and new points  
	//                                                         
	//Return {                                                 
	//returns a motion mesh : x - direction                    
	//and y - direction for old_frame
	void motion_propagate(vector<Vec2f> const& old_points, vector<Vec2f> const& new_points, Mat const& old_frame, cv::Mat & out_x_Mesh, cv::Mat & out_y_Mesh)
	{
		// spreads motion over the mesh for the old_frame        
		std::map<std::pair<int, int>, float> x_motion;
		std::map<std::pair<int, int>, float> y_motion;
		int cols = int(old_frame.size().width / PIXELS);
		int rows = int(old_frame.size().height / PIXELS); ;

		// pre - warping with global homography                                                          
		Matx33f H = cv::findHomography(old_points, new_points, cv::RANSAC);
		for (int i : range(rows))
		{
			for (int j : range(cols))
			{
				Vec2f pt(PIXELS * j, PIXELS * i);
				Vec2f ptrans = point_transform(H, pt);
				x_motion[make_pair(i, j)] = pt[0] - ptrans[0];
				y_motion[make_pair(i, j)] = pt[1] - ptrans[1];
			}
		}
		// disturbute feature motion vectors                                                        
		std::map<std::pair<int, int>, vector<float>> temp_x_motion;
		std::map<std::pair<int, int>, vector<float>> temp_y_motion;
		for (int i : range(rows))
		{
			for (int j : range(cols))
			{
				Vec2f vertex(PIXELS * j, PIXELS * i);
				for (int pt_idx = 0; pt_idx < min(old_points.size(), new_points.size()); pt_idx++)
				{
					Vec2f pt = old_points[pt_idx];
					Vec2f st = new_points[pt_idx];
					// velocity = point - feature point : current frame
					float dst = (float)norm(vertex - pt);
					if (dst < RADIUS)
					{
						Vec2f ptrans = point_transform(H, pt);
						temp_x_motion[make_pair(i, j)].push_back(st[0] - ptrans[0]);
						temp_y_motion[make_pair(i, j)].push_back(st[1] - ptrans[1]);
					}
				}
			}
		}
		// apply median filter(f - 1) on obtained motion for each vertex                                
		Mat x_motion_mesh = Mat::zeros(Size(rows, cols), CV_32FC1);
		Mat y_motion_mesh = Mat::zeros(Size(rows, cols), CV_32FC1);
		for (auto kvp : x_motion)
		{
			auto key = kvp.first;
			if (temp_x_motion.count(key) && x_motion.count(key))
			{
				std::sort(temp_x_motion[key].begin(), temp_x_motion[key].end());
				PIX(x_motion_mesh, key) = x_motion[key] + temp_x_motion[key][int(temp_x_motion[key].size() / 2)];
			}
			else
			{
				PIX(x_motion_mesh, key) = x_motion[key];
			}

			if (temp_y_motion.count(key) && y_motion.count(key))
			{
				std::sort(temp_y_motion[key].begin(), temp_y_motion[key].end());
				PIX(y_motion_mesh, key) = y_motion[key] + temp_y_motion[key][int(int(temp_y_motion[key].size()) / 2)];
			}
			else {
				PIX(y_motion_mesh, key) = y_motion[key];
			}
		}

		// apply second median filter(f - 2) over the motion mesh for outliers                              ;
		int kernel_size = 3;
		Mat median_kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size * kernel_size);

		Mat x_motion_mesh_median, y_motion_mesh_median;
		filter2D(x_motion_mesh, x_motion_mesh_median, -1, median_kernel);
		filter2D(y_motion_mesh, y_motion_mesh_median, -1, median_kernel);

		out_x_Mesh = x_motion_mesh_median;
		out_y_Mesh = y_motion_mesh_median;
	}

	//@param { x_paths is vertex profiles along x - direction                    
		//@param { y_paths is vertex profiles along y_direction                      
		//@param { x_motion_mesh is obtained motion mesh along                       
		//x - direction from motion_propogate()                                      
		//@param { y_motion_mesh is obtained motion mesh along                       
		//y - direction from motion_propogate()                                      
		//                                                                           
		//Returns {                                                                  
		//returns updated x_paths, y_paths with new                                  
		//x_motion_mesh, y_motion_mesh added to the                                  
		//last x_paths, y_paths                                                      
	void generate_vertex_profiles(std::vector<cv::Mat>& x_paths, std::vector<cv::Mat>& y_paths,
		cv::Mat const& x_motion_mesh, cv::Mat const& y_motion_mesh)
	{

		Mat new_x_path = x_paths.back() + x_motion_mesh;
		Mat new_y_path = y_paths.back() + y_motion_mesh;

		x_paths.push_back(new_x_path);
		y_paths.push_back(new_y_path);
	}

	//                                                     
		//    @param { frame is the current frame              
		//    @param { x_motion_mesh is the motion_mesh to     
		//    be warped on frame along x - direction           
		//    @param { y_motion_mesh is the motion mesh to     
		//    be warped on frame along y - direction           
		//                                                     
		//    Returns {                                        
		//returns a mesh warped frame according                
		//to given motion meshes x_motion_mesh,                
		//y_motion_mesh                                        
	cv::Mat mesh_warp_frame(Mat const& frame, Mat const& x_motion_mesh, Mat const& y_motion_mesh)
	{

		// define handles on mesh : x-direction
		Mat map_x = Mat::zeros(frame.size(), CV_32FC1);

		// define handles on mesh : y-direction
		Mat map_y = Mat::zeros(frame.size(), CV_32FC1);

		for (int i : range(x_motion_mesh.cols - 1))
		{
			for (int j : range(x_motion_mesh.rows - 1))
			{
				vector<Vec2f> src = {
					Vec2f(j * PIXELS, i * PIXELS),
					Vec2f(j * PIXELS, (i + 1) * PIXELS),
					Vec2f((j + 1) * PIXELS, i * PIXELS),
					Vec2f((j + 1) * PIXELS, (i + 1) * PIXELS)
				};

				vector<Vec2f> dst = {
					Vec2f(j * PIXELS + PIX(x_motion_mesh, i, j), i * PIXELS + PIX(y_motion_mesh, i,  j)),
					Vec2f(j * PIXELS + PIX(x_motion_mesh, i + 1, j), (i + 1) * PIXELS + PIX(y_motion_mesh, i + 1,  j)),
					Vec2f((j + 1) * PIXELS + PIX(x_motion_mesh, i,  j + 1), i * PIXELS + PIX(y_motion_mesh, i,  j + 1)),
					Vec2f((j + 1) * PIXELS + PIX(x_motion_mesh, i + 1,  j + 1), (i + 1) * PIXELS + PIX(y_motion_mesh, i + 1,  j + 1))
				};


				Matx33f H = cv::findHomography(src, dst, cv::RANSAC);

				for (float k : range(PIXELS* i, PIXELS* (i + 1)))
				{
					for (float l : range(PIXELS* j, PIXELS* (j + 1)))
					{
						float x = H(0, 0) * l + H(0, 1) * k + H(0, 2);
						float y = H(1, 0) * l + H(1, 1) * k + H(1, 2);
						float w = H(2, 0) * l + H(2, 1) * k + H(2, 2);
						if (!w == 0)
						{
							x = x / (w * 1.0); y = y / (w * 1.0);
						}
						else
						{
							x = l; y = k;
							PIX(map_x, k, l) = x;
							PIX(map_y, k, l) = y;
						}
					}
				}
			}
		}
		// repeat motion vectors for remaining frame : y - direction                                       ;
		for (float i : range(PIXELS* x_motion_mesh.cols, map_x.cols))
		{
			map_x.row(PIXELS * x_motion_mesh.cols - 1).copyTo(map_x.row(i));
			map_y.row(PIXELS * x_motion_mesh.cols - 1).copyTo(map_y.row(i));
		}
		// repeat motion vectors for remaining frame : x - direction                                   ;
		for (float j : range(PIXELS* x_motion_mesh.rows, map_x.rows))
		{
			map_x.col(PIXELS * x_motion_mesh.cols - 1).copyTo(map_x.col(j));
			map_y.col(PIXELS * x_motion_mesh.cols - 1).copyTo(map_y.col(j));
		}
		// deforms mesh                                                                             ;
		Mat new_frame;
		cv::remap(frame, new_frame, map_x, map_y, /*interpolation =*/ cv::INTER_LINEAR, /*borderMode = */cv::BORDER_CONSTANT);
		return new_frame;
	}
}