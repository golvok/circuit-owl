
#include <circuit_graph/circuit_graph.hpp>
#include <circuit_graph/connection_finding.hpp>
#include <circuit_graph/line_finding.hpp>
#include <circuit_graph/line_clustering.hpp>
#include <elements/elements.hpp>
#include <main/common.hpp>
#include <utils/geometry_utils.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "expects exactly one param, the image path\n";
		return -1;
	}

	std::srand(time(NULL));

	cv::Mat img = cv::imread(argv[1], 0);

	cv::imshow("original", img);
	cv::waitKey();

	auto elements = get_elements(img);

	auto extraction_results = extract_nodes(img, elements, true);

	std::cout << "elements and their connections:\n";
	for (const CircuitNode& node : std::get<1>(extraction_results)) {
		std::cout << node << '\n';
	}

	std::cout.flush();

	cv::imshow("text only", std::get<0>(extraction_results));
	cv::waitKey();
}
