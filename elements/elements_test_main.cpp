#include "elements.h"

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/nonfree/nonfree.hpp"
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

void readme();
void detectAndDisplay_harr( Mat frame );
void detectAndDisplay_homography( Mat img_scene, Mat img_object );
void maxLocs(const Mat& src, queue<Point>& dst);
void MatchingMethod( Mat img_scene, Mat templat );

String resistor_cascade_name = "resistor_model.xml";
String source_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier resistor_cascade;
CascadeClassifier source_cascade;
string window_name = "window_name";

/** @function main */
int main( int argc, char** argv )
{
	if( argc < 1)
	{ readme(); return -1; }

	Mat frame_homo = imread( argv[1] );

	Mat img_object = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_scene = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

	if( !resistor_cascade.load( resistor_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
	if( !source_cascade.load( source_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	MatchingMethod( img_scene, img_object );


	return 0;
}

void detectAndDisplay_harr( Mat frame )
{
	std::vector<Rect> resistors;
	Mat frame_gray;

	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	//-- Detect resistors
	resistor_cascade.detectMultiScale( frame_gray, resistors, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

	cout << "here1" << endl;
	for( size_t i = 0; i < resistors.size(); i++ )
	{
		Point center( resistors[i].x + resistors[i].width*0.5, resistors[i].y + resistors[i].height*0.5 );
		ellipse( frame, center, Size( resistors[i].width*0.5, resistors[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
	}
	cout << "here1" << endl;

	imwrite( "image_harr.jpg", frame );
	return;
}

void detectAndDisplay_homography( Mat img_scene, Mat img_object )
{
	Size size =img_scene.size();;//the dst image size,e.g.100x100
	resize(img_object,img_object,size);//resize image


	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	SurfFeatureDetector detector( minHessian );

	std::vector<KeyPoint> keypoints_object, keypoints_scene;

	detector.detect( img_object, keypoints_object );
	detector.detect( img_scene, keypoints_scene );

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;

	extractor.compute( img_object, keypoints_object, descriptors_object );
	extractor.compute( img_scene, keypoints_scene, descriptors_scene );

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match( descriptors_object, descriptors_scene, matches );

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ double dist = matches[i].distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist );
	printf("-- Min dist : %f \n", min_dist );

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for( int i = 0; i < descriptors_object.rows; i++ )
	{
		if( matches[i].distance < 3*min_dist )
			{ good_matches.push_back( matches[i]); }
	}

	Mat img_matches;
	drawMatches( img_object, keypoints_object, img_scene, keypoints_scene,
			good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for( uint i = 0; i < good_matches.size(); i++ )
	{
		//-- Get the keypoints from the good matches
		obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
		scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	}

	Mat H = findHomography( obj, scene, CV_RANSAC );

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
	obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform( obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line( img_matches, scene_corners[0] + Point2f( img_object.cols, 0), scene_corners[1] + Point2f( img_object.cols, 0), Scalar(0, 255, 0), 4 );
	line( img_matches, scene_corners[1] + Point2f( img_object.cols, 0), scene_corners[2] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	line( img_matches, scene_corners[2] + Point2f( img_object.cols, 0), scene_corners[3] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	line( img_matches, scene_corners[3] + Point2f( img_object.cols, 0), scene_corners[0] + Point2f( img_object.cols, 0), Scalar( 0, 255, 0), 4 );

	//-- Show detected matches
	imshow( "Good Matches & Object detection", img_matches );

	waitKey(0);	
	return;
 }

void maxLocs(const Mat& src, queue<Point>& dst)
{
    // float maxValue = -1.0f * numeric_limits<float>::max();
    float* srcData = reinterpret_cast<float*>(src.data);

    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
            if(srcData[i*src.cols + j] > 0)
            {
                // maxValue = srcData[i*src.cols + j];

                dst.push(Point(j, i));

                // if(dst.size() > size)
                // {
                //     dst.pop();
                // }
            }
        }
    }
}

float find_max(const Mat& mat)
{
    float maxValue = -1.0f * numeric_limits<float>::max();
    float* matData = reinterpret_cast<float*>(mat.data);

    for(int i = 0; i < mat.rows; i++)
    {
        for(int j = 0; j < mat.cols; j++)
        {
            if(matData[i*mat.cols + j] > maxValue)
            {
                maxValue = matData[i*mat.cols + j];
            }
        }
    }

    return maxValue;
}

void MatchingMethod( Mat img_scene, Mat templat )
{
    /// Source image to display
    Mat img_display;
    Mat result;
    Mat finds;
    img_scene.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img_display.cols - img_scene.cols + 1;
    int result_rows = img_display.rows - img_scene.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( img_display, templat, result, CV_TM_CCOEFF_NORMED );
    float max = find_max(result);
    cout << max << endl;
    threshold( result, result, max*0.9, 255,THRESH_BINARY );

    // normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    queue<Point> locations;
    maxLocs(result, locations);

    finds.create( img_scene.cols, img_scene.rows, CV_32FC1 );

    /// Show me what you got
    while(!locations.empty())
    {
        Point matchLoc = locations.front();
        rectangle( img_display, matchLoc, Point( matchLoc.x + templat.cols , matchLoc.y + templat.rows ), Scalar::all(0), 2, 8, 0 );
        locations.pop();
    }

    imshow( "image_window", img_display );
    imshow( "result_window", result );

    waitKey(0);

    return;
}


/** @function readme */
void readme()
{ std::cout << " args: <img1> " << std::endl; }
