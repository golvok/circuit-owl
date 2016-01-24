#include <main/main.hpp>
#include <circuit_graph/circuit_graph.hpp>
#include <analysis/analysis.hpp>
#include <elements/elements.hpp>


char const* analyze_photo(char const* filename)
{
    cv::Mat img = cv::imread(filename, 0);

    std::vector<CircuitElement> elements = get_elements(img);

    std::tuple<cv::Mat,std::vector<CircuitNode>> tup = extract_nodes(img, elements);

    cv::Mat text_img = std::get<0>(tup);
    std::vector<CircuitNode>& nodes = std::get<1>(tup);

    TextFinder t;
    t.process(text_img);
    std::vector<TessResult> words = t.get_words();

    annotate(elements, words);
    solve_voltages(nodes, elements);

    // Render the anotations
    for(const CircuitNode& node: nodes)
    {
        cv::putText(img, std::to_string(node.voltage) + "V", node.centroid,  cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
    }

    std::string out_file = std::string(filename) + ".out";
    imwrite(out_file, img);

    return out_file.c_str();
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "expects exactly one param, the image path\n";
        return -1;
    }

    char const* out_file = analyze_photo(argv[1]);
    (void)out_file;
}