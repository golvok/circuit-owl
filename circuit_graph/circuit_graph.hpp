#include <main/main.hpp>
#include <main/common.hpp>

#include <circuit_graph/line_clustering.hpp>
#include <circuit_graph/line_finding.hpp>

bool extract_nodes(const cv::Mat& img, const std::vector<CircuitElement>& elements);