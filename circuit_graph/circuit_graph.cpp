#include <circuit_graph/circuit_graph.hpp>
#include <circuit_graph/line_clustering.hpp>
#include <circuit_graph/line_finding.hpp>

std::tuple<cv::Mat,std::vector<CircuitNode>> extract_nodes(const cv::Mat& img, const std::vector<CircuitElement>& elements)
{
    (void)img;
    (void)elements;

    std::tuple<cv::Mat,std::vector<CircuitNode>> tup;
    return tup;
}