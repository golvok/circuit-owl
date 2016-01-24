
#ifndef CG_CIRCUIT_GRAPH_HPP
#define CG_CIRCUIT_GRAPH_HPP

#include <main/common.hpp>

std::tuple<cv::Mat,std::vector<CircuitNode>> extract_nodes(const cv::Mat& img, const std::vector<CircuitElement>& elements, bool debug=false);

#endif /* CG_CIRCUIT_GRAPH_HPP */
