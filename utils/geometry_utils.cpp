
#include "geometry_utils.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const std::pair<cv::Point2i, cv::Point2i>& line) {
	os << '{' << line.first.x << ',' << line.first.y << "} - {" << line.second.x << ',' << line.second.y << '}';
	return os;
}

namespace utils {

float minimum_distance(const std::pair<cv::Point2i, cv::Point2i>& line, cv::Point2f p) {
	const cv::Point2f v = line.first;
	const cv::Point2f w = line.second;

	// Return minimum distance between line segment vw and point p
	const float l2 = [&](const cv::Point2f& d){ return d.x*d.x + d.y*d.y; }(v - w);  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0) return norm(p - v);   // v == w case
	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line.
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	const float t = (p - v).dot(w - v) / l2;
	if (t < 0.0) return norm(p - v);       // Beyond the 'v' end of the segment
	else if (t > 1.0) return norm(p - w);  // Beyond the 'w' end of the segment
	const cv::Point2f projection = v + t * (w - v);  // Projection falls on the segment
	auto min_dist = norm(p - projection);
	// std::cout << "min dist=" << min_dist << '\n';
	return min_dist;
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool onSegment(cv::Point2i p, cv::Point2i q, cv::Point2i r) {
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
	   return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(cv::Point2i p, cv::Point2i q, cv::Point2i r) {
	// See http://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.y - p.y) * (r.x - q.x) -
			  (q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear

	return (val > 0)? 1: 2; // clock or counterclock wise
}

bool intersects(const std::pair<cv::Point2i, cv::Point2i>& line1, const std::pair<cv::Point2i, cv::Point2i>& line2) {
	const auto& p1 = line1.first;
	const auto& q1 = line1.second;
	const auto& p2 = line2.first;
	const auto& q2 = line2.second;

	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and p2 are colinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	 // p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

bool line_segs_near(const Line& l1, const Line& l2, float distance_thresh) {
	return false
		|| utils::intersects(l1, l2)
		|| utils::minimum_distance(l1, l2.first ) < distance_thresh
		|| utils::minimum_distance(l1, l2.second) < distance_thresh
		|| utils::minimum_distance(l2, l1.first ) < distance_thresh
		|| utils::minimum_distance(l2, l1.second) < distance_thresh
	;
}


} // end namespace utils
