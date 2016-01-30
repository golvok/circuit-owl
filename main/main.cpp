#include <main/main.hpp>
#include <circuit_graph/circuit_graph.hpp>
#include <analysis/analysis.hpp>
#include <elements/elements.hpp>


char const* analyze_photo(char const* filename_in, char const* filename_out, bool debug)
{
    cv::Mat img = cv::imread(filename_in, 0);

    std::vector<CircuitElement> elements = get_elements(img);

    std::tuple<cv::Mat,std::vector<CircuitNode>> tup = extract_nodes(img, elements, debug);

    cv::Mat text_img = std::get<0>(tup);
    std::vector<CircuitNode>& nodes = std::get<1>(tup);

    TextFinder t;
    t.process(text_img);
    std::vector<TessResult> words = t.get_words();

    annotate(elements, words);
    solve_voltages(nodes, elements);

    cvtColor(img, img, CV_GRAY2RGB);

    // Render the anotations
    for(const CircuitNode& node: nodes)
    {
        cv::putText(img, std::to_string(node.voltage) + "V", node.centroid,  cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,255), 1, CV_AA);
    }

    if (debug) {
        cv::imshow("final result", img);
        cv::waitKey();
    }

    if (strcmp(filename_out,"/dev/null") != 0) {
        imwrite(filename_out, img);
    }

    return filename_out;
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cout << "expects exactly two params, the image input path and image output path\n";
        return -1;
    }

    char const* out_file = analyze_photo(argv[1], argv[2], false);
    (void)out_file;
}