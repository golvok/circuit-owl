
#include <opencv2/core/core.hpp>

#include <cstdlib>

std::ostream& operator<<(std::ostream& os, const std::pair<cv::Point2i, cv::Point2i>& line);

namespace utils {

using Line = std::pair<cv::Point2i, cv::Point2i>;
using ListOfLines = std::vector<Line>;

float minimum_distance(const std::pair<cv::Point2i, cv::Point2i>& line, cv::Point2f p);

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(cv::Point2i p, cv::Point2i q, cv::Point2i r);

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(cv::Point2i p, cv::Point2i q, cv::Point2i r);

bool intersects(const std::pair<cv::Point2i, cv::Point2i>& line1, const std::pair<cv::Point2i, cv::Point2i>& line2);

bool line_segs_near(const Line& l1, const Line& l2, float distance_thresh);

} // end namespace utils
