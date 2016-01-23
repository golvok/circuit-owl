
#include <opencv2/core/core.hpp>

namespace cg {

std::vector<std::vector<size_t>> cluster_lines(const std::vector<std::pair<cv::Point2i, cv::Point2i>>& lines , const float distance_thresh, const bool debug = false);

} // end namespace cg