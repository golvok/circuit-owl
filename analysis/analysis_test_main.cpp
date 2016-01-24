#include <analysis/analysis.hpp>

using namespace cv;

int main(int argc, char** argv) {
	(void) argv;
	if (argc != 2) {
		return -1;
	}

    printf("ANALYZING...\n");

    vector<node> nodes;
    vector<int> empty;
    empty.push_back(0);
    empty.push_back(2);
    nodes.push_back(node(0, 9.43432, empty, false));
    empty.clear();
    empty.push_back(1);
    empty.push_back(2);
    nodes.push_back(node(1, 0, empty));
    empty.clear();
    empty.push_back(2);
    empty.push_back(0);
    nodes.push_back(node(2, 0, empty, false));

    vector<element> elements;
    
    Point p1(0,0);

    elements.push_back(element(0, p1, p1, 0, false));
    elements[0].set_node_id1(2);
    elements[0].set_node_id2(0);
    elements.push_back(element(1, p1, p1, 100)); 
    elements[1].set_node_id1(0);
    elements[1].set_node_id2(1);
    elements.push_back(element(2, p1, p1, 100));
    elements[2].set_node_id1(1);
    elements[2].set_node_id2(2);

    solve_voltages(nodes, elements);

    string input(argv[1]);
    
    for(node& n : nodes)
    {
        printf("node %d has voltage %.2f\n", n.get_id(), n.get_voltage());
    }    
    
	return 0;
}
