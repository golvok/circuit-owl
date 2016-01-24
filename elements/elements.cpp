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

std::vector<std::pair<Rect,float>> anneal(std::vector<std::pair<Rect,float>> matches)
{
	vector<pair<Rect, float>> clusters;
	for(auto res : matches)
	{
		bool is_cluster = true;
		for (auto& cluster : clusters)
		{
			if((res.first & cluster.first).area() != 0)
			{
				cluster.first = res.first | cluster.first;
				cluster.second = std::max(cluster.second, res.second);

				is_cluster = false;
				break;
			}
		}

		if (is_cluster)
		{
			clusters.push_back(res);
		}
	}
	return clusters;
}

void MatchingMethod( Mat img_scene, Mat templat, vector<pair<Rect,float>>& matches, double thresh )
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
	
	/*
	if (max < 0.7) {

		// cout << max << " returning" << endl;

		// imshow( "image_window", img_display );
		// imshow( "result_window", result );

		waitKey(0);
		return;
	}
	*/

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

		matches.emplace_back(Rect(matchLoc.x, matchLoc.y, templat.cols, templat.rows), result.at<float>(matchLoc.y, matchLoc.x));

		locations.pop();
	}

	// imshow( "image_window", img_display );
	// imshow( "result_window", result );

	// waitKey(0);

	return;
}

vector<CircuitElement>  get_elements(Mat img_scene){

	vector<pair<Rect, float>> restistors;
	vector<pair<Rect, float>> sources;

	Mat img_res_v = imread("./circuit_images/resistor_v.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_res_h = imread("./circuit_images/resistor_h.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_src_v = imread("./circuit_images/source_v.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_src_h = imread("./circuit_images/source_h.png", CV_LOAD_IMAGE_GRAYSCALE);

	MatchingMethod( img_scene, img_res_v, restistors, 0.9 );
	MatchingMethod( img_scene, img_res_h, restistors, 0.9 );
	MatchingMethod( img_scene, img_src_v, sources, 0.9);
	MatchingMethod( img_scene, img_src_h, sources, 0.9);

	
	vector<CircuitElement> v;
	
	std::vector<std::pair<Rect,float>> res_c = anneal(restistors);
	std::vector<std::pair<Rect,float>> src_c = anneal(sources);

	int id = 0;	

	std::cout << "un-annealed elements: " << restistors.size() + sources.size() << std::endl;
	std::cout << "Annealed elements: " << res_c.size() + src_c.size() << std::endl;


	for(auto res : res_c)
	{	
		bool biggest = true;
		for(auto src : src_c)
		{
			if((src.first & res.first).area() != 0 && src.second > res.second)
			{
				biggest = false;
				break;
			}
		}
		
		if(biggest)
		{
			v.push_back(CircuitElement( 
				id, 
				Point( res.first.x, res.first.y ), 
				Point( res.first.x + res.first.width , res.first.y + res.first.height ), 
				0, 
				0)
			);
			id++;
		}
	}		
	
	for(auto src : src_c)
	{	
		bool biggest = true;
		for(auto res : res_c)
		{
			if((src.first & res.first).area() != 0 && res.second >= src.second)
			{
				biggest = false;
				break;
			}
		}
		
		if(biggest)
		{
			v.push_back(CircuitElement( 
				id,
				Point( src.first.x, src.first.y ), 
				Point( src.first.x + src.first.width , src.first.y + src.first.height ), 
				0, 
				1)
			);
			id++;
		}
	}

	std::cout << "Number of elements detected: " << v.size() << std::endl;
	
	return v;
}

/** @function readme */
void readme()
{ std::cout << " args: <img1> " << std::endl; }

