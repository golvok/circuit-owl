#include <main/common.hpp>

std::tuple<cv::Mat,std::vector<CircuitNode>> extract_nodes(const cv::Mat& img, const std::vector<CircuitElement>& elements);