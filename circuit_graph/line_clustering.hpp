
#ifndef CG_LINE_CLUSTERING_HPP
#define CG_LINE_CLUSTERING_HPP

#include <utils/geometry_utils.hpp>

#include <opencv2/core/core.hpp>

namespace cg {

using ListOfLineIndices = std::vector<size_t>;
using ListOfListOfLineIndices = std::vector<ListOfLineIndices>;

ListOfListOfLineIndices cluster_lines(const std::vector<std::pair<cv::Point2i, cv::Point2i>>& lines , const float distance_thresh, const bool debug = false);

ListOfListOfLineIndices::const_iterator cluster_most_likely_to_be_circuit(const ListOfListOfLineIndices& cluster_lists, const utils::ListOfLines& lines);

} // end namespace cg

#endif /* CG_LINE_CLUSTERING_HPP */
