#include <main/main.hpp>
#include <main/common.hpp>
#include <line_clustering.hpp>
#include <line_finding.hpp>

bool extract_nodes(const cv::Mat& img, const std::vector<CircuitElement>& elements);