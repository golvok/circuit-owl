#include "elements.hpp"

using namespace cv;
using namespace std;
using namespace utils;

int main( int argc, char** argv )
{
	if( argc < 1)
	{ readme(); return -1; }

	Mat img_scene = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );

	auto var = get_elements(img_scene);

	return 0;
}
