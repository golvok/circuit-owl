// #include <opencv2/text/ocr.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <memory>
#include <algorithm>

#include <text/text_finder.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	(void) argv;
	if (argc != 2) {
		return -1;
	}

    string input(argv[1]);

    cv::Mat img = cv::imread(input, 0);

    TextFinder t;
    t.process(img);
    t.save("./img/output_img.png");

	return 0;
}

