
#ifndef CG_CONNECTION_FINDING_HPP
#define CG_CONNECTION_FINDING_HPP

#include <circuit_graph/line_clustering.hpp>

namespace cg {

using ConnectionList = std::vector<size_t>;
using ListOfConnectionList = std::vector<ConnectionList>;

ListOfConnectionList find_connections(
	const std::vector<cv::Rect>& rects,
	const ListOfListOfLineIndices& wire_cluster_lists,
	const utils::ListOfLines& lines
);

} // end namespace cg

#endif /* CG_CONNECTION_FINDING_HPP */
