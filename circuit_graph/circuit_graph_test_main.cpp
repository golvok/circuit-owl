
#include <circuit_graph/line_finding.hpp>

#include <iostream>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "expects exactly one param, the image path\n";
		return -1;
	}

	cv::Mat img = cv::imread(argv[1], 0);

	cv::imshow("original", img);
	cv::waitKey();

	std::vector<cv::Vec4i> lines = cg::find_lines(img, true);

}
