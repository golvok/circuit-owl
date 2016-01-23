
#include <circuit_graph/line_finding.hpp>
#include <circuit_graph/line_clustering.hpp>
#include <utils/geometry_utils.hpp>

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

	std::vector<cv::Vec4i> lines_as_vecs = cg::find_lines(img, true);
	std::vector<std::pair<cv::Point2i, cv::Point2i>> lines;
	lines.reserve(lines_as_vecs.size());
	for (const auto& line_as_vec : lines_as_vecs) {
		lines.emplace_back(std::make_pair(
			cv::Point2i(line_as_vec[0], line_as_vec[1]),
			cv::Point2i(line_as_vec[2], line_as_vec[3])
		));
	}

	const float NEARNESS_THRESH = 5; // in pixels
	std::vector<std::vector<size_t>> cluster_lists = cg::cluster_lines(lines, NEARNESS_THRESH, true);

	cv::Mat with_clusters_marked = img.clone();

	for (size_t i = 0; i < cluster_lists.size(); ++i) {
		auto colour = cv::Scalar(
			(std::rand() % 15) * 16 + 16,
			(std::rand() % 15) * 16 + 16,
			(std::rand() % 15) * 16 + 16
		);

		for (const auto& line_index : cluster_lists[i]) {
			cv::line(with_clusters_marked, lines[line_index].first, lines[line_index].second, colour, 3, CV_AA);
		}
	}

	cv::imshow("with clusters", with_clusters_marked);
	cv::waitKey();
}
