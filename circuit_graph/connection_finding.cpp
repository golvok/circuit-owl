#include "connection_finding.hpp"

#include <opencv2/core/core.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <limits>

namespace cg {

std::pair<
	ListOfConnectionList,
	std::vector<std::pair<size_t,size_t>>
> find_connections(
	const std::vector<cv::Rect>& rects,
	const ListOfListOfLineIndices& wire_cluster_lists,
	const utils::ListOfLines& lines,
	bool debug
) {
	ListOfConnectionList connection_lists;
	std::vector<std::pair<size_t,size_t>> element_connections(rects.size(), {-1,-1});

	for (const auto& wire_cluster_list : wire_cluster_lists) {
		connection_lists.emplace_back();
		for (size_t rect_index = 0; rect_index < rects.size(); ++rect_index) {
			const auto& rect = rects[rect_index];
			for (const auto& line_index : wire_cluster_list) {
				const auto& line = lines[line_index];
				const auto& tl = rect.tl();
				const auto& br = rect.br();
				const auto& tr = cv::Point2i(br.x, tl.y);
				const auto& bl = cv::Point2i(tl.x, br.y);

				const float distance_thresh = 6;
				if (false
					|| rect.contains(line.first)
					|| rect.contains(line.second)
					|| utils::line_segs_near(line, utils::Line(tl, tr), distance_thresh)
					|| utils::line_segs_near(line, utils::Line(tr, br), distance_thresh)
					|| utils::line_segs_near(line, utils::Line(br, bl), distance_thresh)
					|| utils::line_segs_near(line, utils::Line(bl, tl), distance_thresh)
				) {
					if (debug) {
						std::cout << line << " intersects " << rect << ". Connects wire " << (connection_lists.size() - 1) << " to elem " << rect_index << '\n';

						cv::Mat dbg_img = cv::Mat::zeros(400, 640, CV_8U);
						cv::rectangle(dbg_img, rect, 128, CV_FILLED);
						cv::line(dbg_img, line.first, line.second, 255, 2);

						cv::imshow("debug rect intersect", dbg_img);
						cv::waitKey();
					}

					cv::Point2i centroid = rect.tl() + (rect.br() - rect.tl()) * 0.5;

					auto closest_point = [&](){
						if (cv::norm(centroid - line.first) < cv::norm(centroid - line.second)) {
							return line.first;
						} else {
							return line.second;
						}
					}();

					int best_side_index = -1;
					float min_dist = std::numeric_limits<float>::max();

					int current_side_index = 0;
					for (const auto& side : {
						utils::Line(tl, tr),
						utils::Line(tr, br),
						utils::Line(br, bl),
						utils::Line(bl, tl),
					}) {
						float test_dist = utils::minimum_distance(side, closest_point);
						if (test_dist < min_dist) {
							best_side_index = current_side_index;
							min_dist = test_dist;
						}
						++current_side_index;
					}

					switch (best_side_index) {
						default:
						case 0:
						case 3:
							// case right & top
							element_connections[rect_index].first = (connection_lists.size() - 1);
						break;
						case 1:
						case 2:
							// case left & bottom
							element_connections[rect_index].second = (connection_lists.size() - 1);
						break;
					}

					connection_lists.back().push_back(rect_index);
					break;
				}
			}
		}
	}

	return std::make_pair(connection_lists, element_connections);

}

}
