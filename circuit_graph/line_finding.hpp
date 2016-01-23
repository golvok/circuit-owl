
#ifndef CG_LINE_FINDING_CPP
#define CG_LINE_FINDING_CPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace cg {

std::vector<cv::Vec4i> find_lines(const cv::Mat& img_src, bool debug = false);

} // end namespace cg

#endif /* CG_LINE_FINDING_CPP */
