#ifndef INCLUDE_ELEMENTS
#define INCLUDE_ELEMENTS

#include <utils/geometry_utils.hpp>
#include <main/common.hpp>

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <iostream>
#include <stdio.h>

void readme();
void detectAndDisplay_harr( cv::Mat frame );
void detectAndDisplay_homography( cv::Mat img_scene, cv::Mat img_object );
void maxLocs(const cv::Mat& src, std::queue<cv::Point>& dst);
void MatchingMethod( cv::Mat img_scene, cv::Mat templat, std::vector<std::pair<cv::Rect,float>>& matches, double thresh );
std::vector<CircuitElement> get_elements(cv::Mat img_scene);

#endif