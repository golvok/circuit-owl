#include <iostream>
#include "elements.hpp"

using namespace cv;
using namespace std;
using namespace utils;

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
				dst.push(Point(j, i));
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

void MatchingMethod( Mat img_scene, Mat templat, vector<Rect>& rects, double thresh )
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
	if (max < 0.7) {

		// cout << max << " returning" << endl;

		// imshow( "image_window", img_display );
		// imshow( "result_window", result );

		waitKey(0);
		return;
	}
	cout << max << endl;
	threshold( result, result, max*thresh, 255,THRESH_BINARY);

	queue<Point> locations;
	maxLocs(result, locations);

	finds.create( img_scene.cols, img_scene.rows, CV_32FC1 );

	/// Show me what you got
	while(!locations.empty())
	{
		Point matchLoc = locations.front();
		rectangle( img_display, matchLoc, Point( matchLoc.x + templat.cols , matchLoc.y + templat.rows ), Scalar::all(0), 2, 8, 0 );

		rects.push_back(Rect(matchLoc.x, matchLoc.y, templat.cols, templat.rows));

		locations.pop();
	}

	// imshow( "image_window", img_display );
	// imshow( "result_window", result );

	// waitKey(0);

	return;
}

vector<CircuitElement>  get_elements(Mat img_scene){
	vector<Rect> restistors;
	vector<Rect> sources;

	Mat img_res_v = imread("circuit_images/resistor_v.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_res_h = imread("circuit_images/resistor_h.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_src_v = imread("circuit_images/source_v.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_src_h = imread("circuit_images/source_h.png", CV_LOAD_IMAGE_GRAYSCALE);

	MatchingMethod( img_scene, img_res_v, restistors, 0.9 );
	MatchingMethod( img_scene, img_res_h, restistors, 0.9 );
	MatchingMethod( img_scene, img_src_v, sources, 0.9);
	MatchingMethod( img_scene, img_src_h, sources, 0.9);

	int id = 0;
	vector<CircuitElement> v;
	while (!restistors.empty()){
		Rect r = restistors.front();
		v.push_back(CircuitElement( id, Point( r.x, r.y ), Point( r.x + r.width , r.y + r.height ), 0, 0));
		restistors.pop_back();
		id++;
	}

	while (!sources.empty()){
		Rect r = sources.front();
		v.push_back(CircuitElement( id, Point( r.x, r.y ), Point( r.x + r.width , r.y + r.height ), 0, 0));
		sources.pop_back();
		id++;
	}

	// return pair<restistors,sources>;
	return v;
}

/** @function readme */
void readme()
{ std::cout << " args: <img1> " << std::endl; }

