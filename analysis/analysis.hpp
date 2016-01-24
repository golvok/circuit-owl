#include <algorithm>
#include <iostream>
#include <memory>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <main/common.hpp>
#include <text/text_finder.hpp>


void solve_voltages(std::vector<CircuitNode>& nodes, std::vector<CircuitElement>& elements);
void annotate(std::vector<CircuitElement>& elements, std::vector<TessResult>& texts);
