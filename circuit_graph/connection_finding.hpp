
#ifndef CG_CONNECTION_FINDING_HPP
#define CG_CONNECTION_FINDING_HPP

#include <circuit_graph/line_clustering.hpp>

namespace cg {

using ConnectionList = std::vector<size_t>;
using ListOfConnectionList = std::vector<ConnectionList>;

std::pair<
	ListOfConnectionList,
	std::vector<std::pair<size_t,size_t>>
> find_connections(
	const std::vector<cv::Rect>& rects,
	const ListOfListOfLineIndices& wire_cluster_lists,
	const utils::ListOfLines& lines,
	bool debug
);

} // end namespace cg

#endif /* CG_CONNECTION_FINDING_HPP */
