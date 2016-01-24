#include <analysis/analysis.hpp>

#define MIN_ITERATION_CHANGE 0.0001

using namespace cv;

int calc_dist(Point& p1, Point& p2)
{
    return sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));
}   

void annotate(std::vector<CircuitElement>& elements, std::vector<TessResult>& texts)
{
    for(CircuitElement& element : elements)
    {
        int best_distance = INT_MAX;
        Point symbol_center((element.NE.y + element.SW.y)/2, (element.NE.x + element.SW.x)/2);
        
        for(TessResult& text : texts)
        {
            Point text_center((text.NE.y+text.SW.y)/2, (text.NE.x + text.SW.x)/2);            
            int distance = calc_dist(symbol_center, text_center);

            if(distance < best_distance)
            {
                element.val = text.val;
                best_distance = distance;
            }
        }
    }
}

void solve_voltages(std::vector<CircuitNode>& nodes, std::vector<CircuitElement>& elements)
{
    auto v_src_iter = std::find_if(elements.begin(), elements.end(), [&](CircuitElement& elem) {
        return elem.type == 1;
    });

    if (v_src_iter != elements.end()) {
        nodes[v_src_iter->node_id[0]].voltage = v_src_iter->val;
        nodes[v_src_iter->node_id[0]].changeable = false;
        nodes[v_src_iter->node_id[1]].voltage = 0;
        nodes[v_src_iter->node_id[1]].changeable = false;
    }

    bool done_iteration = false;
    while (!done_iteration)
    {
        bool small_change = true;
        for(CircuitNode& current_node : nodes)
        {
                printf("\n\n looking at node %d\n", current_node.id);
                double term1 = 0; //first part of voltage equation
                double term2 = 0; //second part of voltage equation
                double old_voltage = current_node.voltage;
                for(int& el_index : current_node.element_ids)
                {
                    CircuitElement& current_element = elements[el_index];
                    printf("looking at element %d\n", current_element.id);
                    if(current_element.type == 0)
                    {
                        printf("is resistor and resistance is %d\n", current_element.val);
                        term1 += 1.0/current_element.val; 
                        int needed_node; 
                        if (current_element.node_id[0] == current_node.id) 
                            needed_node = current_element.node_id[1];
                        else //vice versa of above
                            needed_node = current_element.node_id[0];
                        term2 += nodes[needed_node].voltage/current_element.val;
                        printf("term 2 is %.2f\n", term2);
                    }
                }
                printf("term1: %.2f term2: %.2f\n", term1, term2);
                if(current_node.changeable)
                {
                    printf("setting %d to %.2f\n", current_node.id, term2/term1);
                    current_node.voltage = term2/term1; //changes node voltage according to equation
                }
                if (abs(old_voltage-current_node.voltage) >= MIN_ITERATION_CHANGE) 
                {
                    small_change = false;
                }
        }
        if (small_change)
        {   
            break;
        }
    }
}
