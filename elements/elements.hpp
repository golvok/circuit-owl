#ifndef INCLUDE_ELEMENTS
#define INCLUDE_ELEMENTS

#include <utils/geometry_utils.hpp>

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <iostream>
#include <stdio.h>


using namespace cv;
using namespace std;
using namespace utils;
void readme();
void detectAndDisplay_harr( Mat frame );
void detectAndDisplay_homography( Mat img_scene, Mat img_object );
void maxLocs(const Mat& src, queue<Point>& dst);
void MatchingMethod( Mat img_scene, Mat templat, vector<Rect>& rects, double thresh );
void get_elements(Mat img_scene);

#endif