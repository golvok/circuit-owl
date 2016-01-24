#include "connection_finding.hpp"

#include <opencv2/core/core.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

namespace cg {

ListOfConnectionList find_connections(
	const std::vector<cv::Rect>& rects,
	const ListOfListOfLineIndices& wire_cluster_lists,
	const utils::ListOfLines& lines
) {

	ListOfConnectionList connection_lists;
	for (const auto& wire_cluster_list : wire_cluster_lists) {
		connection_lists.emplace_back();
		for (size_t rect_index = 0; rect_index < rects.size(); ++rect_index) {
			for (const auto& line_index : wire_cluster_list) {
				const auto& tl = rects[rect_index].tl();
				const auto& br = rects[rect_index].br();
				const auto& tr = cv::Point2i(br.x, tl.y);
				const auto& bl = cv::Point2i(tl.x, br.y);

				const float distance_thresh = 6;
				if (false
					|| rects[rect_index].contains(lines[line_index].first)
					|| rects[rect_index].contains(lines[line_index].second)
					|| utils::line_segs_near(lines[line_index], utils::Line(tl, tr), distance_thresh)
					|| utils::line_segs_near(lines[line_index], utils::Line(tr, br), distance_thresh)
					|| utils::line_segs_near(lines[line_index], utils::Line(br, bl), distance_thresh)
					|| utils::line_segs_near(lines[line_index], utils::Line(bl, tl), distance_thresh)
				) {
					connection_lists.back().push_back(rect_index);
					std::cout << lines[line_index] << " intersects " << rects[rect_index] << ". Connects wire " << (connection_lists.size() - 1) << " to elem " << rect_index << '\n';
					
					cv::Mat dbg_img = cv::Mat::zeros(400, 640, CV_8U);
					cv::rectangle(dbg_img, rects[rect_index], 128, CV_FILLED);
					cv::line(dbg_img, lines[line_index].first, lines[line_index].second, 255, 2);

					cv::imshow("debug rect intersect", dbg_img);
					cv::waitKey();
					
					break;
				}
			}
		}
	}

	return connection_lists;
}

}
