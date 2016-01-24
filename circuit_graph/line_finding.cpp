
#include "line_finding.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

namespace cg {

std::vector<cv::Vec4i> find_lines(const cv::Mat& img_src, bool debug) {

	cv::Mat inverted = img_src.clone();
	cv::bitwise_not(inverted,inverted);
	cv::threshold(inverted, inverted, 127, 255, cv::THRESH_BINARY);
	cv::Mat skel(inverted.size(), CV_8UC1, cv::Scalar(0));
	cv::Mat temp;
	cv::Mat eroded;

	cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));

	while (true) {
		cv::erode(inverted, eroded, element);
		cv::dilate(eroded, temp, element); // temp = open(inverted)
		cv::subtract(inverted, temp, temp);
		cv::bitwise_or(skel, temp, skel);
		eroded.copyTo(inverted);

		if (cv::countNonZero(inverted) == 0) {
			break;
		}
	}

	if (debug) {
		cv::imshow("Skeleton", skel);
		cv::waitKey();
	}

	cv::Mat dst = skel.clone();
	cv::Mat cdst;
	cv::cvtColor(dst, cdst, cv::COLOR_GRAY2BGR);

	// cv::imshow("Canny", dst);
	// cv::waitKey();

	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(skel, lines, 1, CV_PI/180, 4, 1, 10);
	
	if (debug) {
		std::cout << "found " << lines.size() << " lines\n";
	}

	for( size_t i = 0; i < lines.size(); i++ )
	{
		cv::Vec4i l = lines[i];
		auto colour = cv::Scalar((i%3==0)*255,(i%3==1)*255,(i%3==2)*255);
		cv::line( cdst, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), colour, 1, CV_AA);
	}

	if (debug) {
		cv::imshow("Lines", cdst);
		cv::waitKey();
	}

	return lines;
}

}
