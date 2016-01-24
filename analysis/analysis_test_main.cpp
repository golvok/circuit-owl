#include <analysis/analysis.hpp>
#include <main/common.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	(void) argv;
	if (argc != 2) {
		return -1;
	}

    printf("ANALYZING...\n");

    vector<CircuitNode> nodes;
    vector<int> empty;
    empty.push_back(0);
    empty.push_back(2);
    nodes.push_back(CircuitNode(0, 9.43432, std::vector<int>(), true));
    empty.clear();
    empty.push_back(1);
    empty.push_back(2);
    nodes.push_back(CircuitNode(1, 0, std::vector<int>(), false));
    empty.clear();
    empty.push_back(2);
    empty.push_back(0);
    nodes.push_back(CircuitNode(2, 0, std::vector<int>(), true));

    vector<CircuitElement> elements;
    
    Point p1(0,0);

    elements.push_back(CircuitElement(0, p1, p1, 0, 1));
    elements[0].node_id[0] = 2;
    elements[0].node_id[1] = 0;
    elements.push_back(CircuitElement(1, p1, p1, 100, 0)); 
    elements[1].node_id[0] = 0;
    elements[1].node_id[1] = 1;
    elements.push_back(CircuitElement(2, p1, p1, 100, 0));
    elements[2].node_id[0] = 1;
    elements[2].node_id[1] = 2;

    solve_voltages(nodes, elements);

    string input(argv[1]);
    
    for(CircuitNode& n : nodes)
    {
        printf("node %d has voltage %.2f\n", n.id, n.voltage);
    }
    
	return 0;
}
