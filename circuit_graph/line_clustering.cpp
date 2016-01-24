
#include "line_clustering.hpp"

#include <iostream>

namespace cg {

ListOfListOfLineIndices cluster_lines(const std::vector<std::pair<cv::Point2i, cv::Point2i>>& lines , const float distance_thresh, const bool debug) {
	ListOfListOfLineIndices cluster_lists;

	// iterate over the lines
	for (size_t i = 0; i < lines.size(); ++i) {
		if (debug) { std::cout << "looking at line " << i << " : " << lines[i] << " : "; }
		std::vector<size_t> clusters_this_line_is_in;

		// iterate over the clusters
		for (size_t cluster_list_index = 0; cluster_list_index < cluster_lists.size(); ++cluster_list_index) {
			auto& cluster_list = cluster_lists[cluster_list_index];
			// std::cout << "next cluster\n";
			// iterate over the lines in a cluster
			for (const auto& clustered_line_index : cluster_list) {
				const auto& clustered_line = lines[clustered_line_index];
				// std::cout << "checking against " << clustered_line << '\n';

				// check if that line should be clustered in this cluster
				if (utils::line_segs_near(lines[i], clustered_line, distance_thresh)) {
					// std::cout << "clustering " << i << " in " << cluster_list.size() << '\n';
					clusters_this_line_is_in.push_back(cluster_list_index);
					break;
				}
				if (!clusters_this_line_is_in.empty() && clusters_this_line_is_in.empty() == cluster_list_index) { break; }
			}
		}

		if (clusters_this_line_is_in.empty()) {
			// not in any cluster, so make a new one
			cluster_lists.emplace_back();
			cluster_lists.back().push_back(i);
			if (debug) { std::cout << "not clustered! making cluster " << cluster_lists.size() << '\n'; }
		} else if (clusters_this_line_is_in.size() == 1) {
			// only in one cluster
			cluster_lists[clusters_this_line_is_in.front()].push_back(i);
			if (debug) { std::cout << "only in cluster " << clusters_this_line_is_in.front() << '\n'; }
		} else {
			// in multiple clusters - it is a bridge - combine them
			auto& cluster_to_combine_into = cluster_lists[clusters_this_line_is_in.front()];

			if (debug) { std::cout << "was clustered into "; }
			for (const auto& cluster_list_index : clusters_this_line_is_in) {
				if (debug) { std::cout << cluster_list_index << ", "; }
				// skip the first one
				if (cluster_list_index == clusters_this_line_is_in.front()) { continue; }

				// copy over other ones
				std::copy(
					cluster_lists[cluster_list_index].begin(),
					cluster_lists[cluster_list_index].end(),
					std::back_inserter(cluster_to_combine_into)
				);

				// put it in too
				cluster_to_combine_into.push_back(i);

				// empty the old one
				cluster_lists[cluster_list_index].resize(0);
			}
			if (debug) { std::cout << '\n'; }
		}
	}

	{
		// remove empty oness
		ListOfListOfLineIndices new_cluster_lists;
		for (auto& cluster_list : cluster_lists) {
			if (cluster_list.empty() == false) {
				new_cluster_lists.emplace_back(std::move(cluster_list));
			}
		}
		cluster_lists = std::move(new_cluster_lists);
	}

	if (debug) {
		std::cout << "there were " << cluster_lists.size() << " clusters:\n";
		for (const auto& cluster_list : cluster_lists) {
			std::cout << cluster_list.size() << ',';
		}
		std::cout << '\n';
	}

	return cluster_lists;

}

ListOfListOfLineIndices::const_iterator cluster_most_likely_to_be_circuit(const ListOfListOfLineIndices& cluster_lists, const utils::ListOfLines& lines) {
	float best_area = -1;
	ListOfListOfLineIndices::const_iterator best_cluster_iter = cluster_lists.end();

	for (auto cl_iter = cluster_lists.begin(); cl_iter != cluster_lists.end(); ++cl_iter) {
		const auto& cluster_list = *cl_iter;
		const auto& first_line = lines[cluster_list.front()];

		// init to the first line
		cv::Rect bound(first_line.first, first_line.second);

		for (const auto& line_index : cluster_list) {
			const auto& line = lines[line_index];
			// expand by the bound of the line
			bound |= cv::Rect(line.first, line.second);
		}

		if (best_area < bound.area()) {
			best_area = bound.area();
			best_cluster_iter = cl_iter;
		}
	}

	return best_cluster_iter;
}

} // end namespace cg