#include "elements.h"

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

void readme();
void detectAndDisplay( Mat frame );

String resistor_cascade_name = "haarcascade_frontalface_alt.xml";
String source_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier resistor_cascade;
CascadeClassifier source_cascade;
string window_name = "window_name";

/** @function main */
int main( int argc, char** argv )
{
	if( argc != 2 )
	{ readme(); return -1; }

	// Mat img_object_temp = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_scene = imread( argv[1] );

	if( !resistor_cascade.load( resistor_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
	if( !source_cascade.load( source_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	// Size size =img_scene.size();;//the dst image size,e.g.100x100
	// Mat img_object;//dst image
	// resize(img_object_temp,img_object,size);//resize image

	// size = Size(11,11);
	// GaussianBlur( img_scene, img_scene, size, 11, 11);
	// size = Size(51,51);
	// GaussianBlur( img_object, img_object, size, 11, 11);

	if( !img_scene.data)
	{ std::cout<< " --(!) Error reading images " << std::endl; return -1; }

	detectAndDisplay( img_scene );

	return 0;

}

void detectAndDisplay( Mat frame )
{
	std::vector<Rect> resistors;
	Mat frame_gray;

	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	//-- Detect resistors
	resistor_cascade.detectMultiScale( frame_gray, resistors, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

	for( size_t i = 0; i < resistors.size(); i++ )
	{
		Point center( resistors[i].x + resistors[i].width*0.5, resistors[i].y + resistors[i].height*0.5 );
		ellipse( frame, center, Size( resistors[i].width*0.5, resistors[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

		Mat resistorROI = frame_gray( resistors[i] );
		std::vector<Rect> source;

		//-- In each resistor, detect source
		source_cascade.detectMultiScale( resistorROI, source, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

		for( size_t j = 0; j < source.size(); j++ )
		 {
			 Point center( resistors[i].x + source[j].x + source[j].width*0.5, resistors[i].y + source[j].y + source[j].height*0.5 );
			 int radius = cvRound( (source[j].width + source[j].height)*0.25 );
			 circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
		 }
	}

	//-- Show what you got
	imshow( window_name, frame );
	imwrite( "image.jpg", frame );
	char a;
	cin >> a;
	getchar();
	cout << "done" << endl;
 }

/** @function readme */
void readme()
{ std::cout << " args: <img1> " << std::endl; }