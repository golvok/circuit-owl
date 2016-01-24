#include <main/main.hpp>
#include <circuit_graph/circuit_graph.hpp>
#include <analysis/analysis.hpp>
#include <elements/elements.hpp>

char const* analyze_photo(char const* filename)
{
    cv::Mat img = cv::imread(filename, 0);

    std::vector<CircuitElement> elements;
    //std::vector<CircuitElement> elements = get_elements(img);

    std::tuple<cv::Mat,std::vector<CircuitNode>> tup = extract_nodes(img, elements);

    cv::Mat text_img = std::get<0>(tup);
    std::vector<CircuitNode> nodes = std::get<1>(tup);

    TextFinder t;
    t.process(text_img);
    std::vector<TessResult> words = t.get_words();

    annotate(elements, words);
    solve_voltages(nodes, elements);
    
    return filename;
}