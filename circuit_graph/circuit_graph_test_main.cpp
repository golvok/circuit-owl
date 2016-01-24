
#include <circuit_graph/connection_finding.hpp>
#include <circuit_graph/line_finding.hpp>
#include <circuit_graph/line_clustering.hpp>
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

	std::vector<cv::Vec4i> lines_as_vecs = cg::find_lines(img, true);
	std::vector<std::pair<cv::Point2i, cv::Point2i>> lines;
	lines.reserve(lines_as_vecs.size());
	for (const auto& line_as_vec : lines_as_vecs) {
		lines.emplace_back(std::make_pair(
			cv::Point2i(line_as_vec[0], line_as_vec[1]),
			cv::Point2i(line_as_vec[2], line_as_vec[3])
		));
	}

	const float NEARNESS_THRESH = 7; // in pixels
	cg::ListOfListOfLineIndices cluster_lists = cg::cluster_lines(lines, NEARNESS_THRESH, true);

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

	auto circuit_cluster_iter = cg::cluster_most_likely_to_be_circuit(cluster_lists, lines);
	const auto& circuit_cluster = *circuit_cluster_iter;

	cv::Mat with_circuit_removed = img.clone();

	auto overwrite_colour = cv::Scalar(255, 255, 255);
	const float overwrite_width = 10;
	for (const auto& line_index : circuit_cluster) {
		cv::line(with_circuit_removed, lines[line_index].first, lines[line_index].second, overwrite_colour, overwrite_width, CV_AA);
	}

	cv::imshow("with circuit removed", with_circuit_removed);
	cv::waitKey();

	std::vector<cv::Rect> hardcoded_elements { // for img/circuit.png
		cv::Rect(177,  72, 90, 45),
		cv::Rect(160, 260, 90, 45),
		cv::Rect(308, 179, 55, 50),
		cv::Rect(426,  76, 90, 40),
		cv::Rect(506, 159, 60, 60),
	};
	const auto& circuit_element_rects = hardcoded_elements;

	cv::Mat only_wires = img.clone();

	for (const auto& rect : circuit_element_rects) {
		cv::rectangle(only_wires, rect, overwrite_colour, CV_FILLED);
	}

	for (auto cl_iter = cluster_lists.begin(); cl_iter != cluster_lists.end(); ++cl_iter) {
		if (cl_iter != circuit_cluster_iter) {
			for (const auto& line_index : *cl_iter) {
				cv::line(only_wires, lines[line_index].first, lines[line_index].second, overwrite_colour, overwrite_width, CV_AA);
			}
		}
	}

	cv::imshow("only wires", only_wires);
	cv::waitKey();

	std::vector<cv::Vec4i> only_wire_lines_as_vecs = cg::find_lines(only_wires, true);
	std::vector<std::pair<cv::Point2i, cv::Point2i>> only_wire_lines;
	only_wire_lines.reserve(only_wire_lines_as_vecs.size());
	for (const auto& line_as_vec : only_wire_lines_as_vecs) {
		only_wire_lines.emplace_back(std::make_pair(
			cv::Point2i(line_as_vec[0], line_as_vec[1]),
			cv::Point2i(line_as_vec[2], line_as_vec[3])
		));
	}

	const cg::ListOfListOfLineIndices wire_cluster_lists = cg::cluster_lines(only_wire_lines, NEARNESS_THRESH, true);

	cv::Mat with_wire_clusters_marked = only_wires.clone();

	for (size_t i = 0; i < wire_cluster_lists.size(); ++i) {
		auto colour = cv::Scalar(
			(std::rand() % 15) * 16 + 16,
			(std::rand() % 15) * 16 + 16,
			(std::rand() % 15) * 16 + 16
		);

		for (const auto& line_index : wire_cluster_lists[i]) {
			cv::line(with_wire_clusters_marked, only_wire_lines[line_index].first, only_wire_lines[line_index].second, colour, 3, CV_AA);
		}
	}

	cv::imshow("with wire clusters", with_wire_clusters_marked);
	cv::waitKey();

	cg::ListOfConnectionList connection_lists = cg::find_connections(circuit_element_rects, wire_cluster_lists, only_wire_lines);

	size_t index = 0;
	for (const auto& connection_list : connection_lists) {
		std::cout << index << " -> ";
		for (const auto& elem_index : connection_list) {
			std::cout << elem_index << ", ";
		}
		std::cout << '\n';
		++index;
	}

}
