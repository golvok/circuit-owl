
#ifndef CG_CIRCUIT_GRAPH_HPP
#define CG_CIRCUIT_GRAPH_HPP

#include <main/common.hpp>
#include <vector>
#include <tuple>

std::tuple<cv::Mat,std::vector<CircuitNode>> extract_nodes(const cv::Mat& img, std::vector<CircuitElement>& elements, bool debug=false);

#endif /* CG_CIRCUIT_GRAPH_HPP */
