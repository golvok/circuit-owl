#include "analysis/analysis.hpp"
#define MIN_ITERATION_CHANGE 0.0001

using namespace cv;

int calc_dist(Point& p1, Point& p2)
{
    return sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));
}   

void annotate(vector<element>& elements, vector<tess_result>& texts)
{
    for(element& element : elements)
    {
        int best_distance = INT_MAX;
        Point symbol_center((element.get_NE().y + element.get_SW().y)/2, (element.get_NE().x + element.get_SW().x)/2);
        
        for(tess_result& text : texts)
        {
            Point text_center((text.NE.y+text.SW.y)/2, (text.NE.x + text.SW.x)/2);            
            int distance = calc_dist(symbol_center, text_center);

            if(distance < best_distance)
            {
                element.set_val(text.val);
                best_distance = distance;
            }
        }
    }
}

void solve_voltages(vector<node>& nodes, vector<element>& elements)
{
    bool done_iteration = false;
    while (!done_iteration)
    {
                    bool small_change = true;
                    for(node& current_node : nodes)
                    {
                            printf("\n\n looking at node %d\n", current_node.get_id());
                            double term1 = 0; //first part of voltage equation
                            double term2 = 0; //second part of voltage equation
                            double old_voltage = current_node.get_voltage();
                            for(int& el_index : current_node.get_element_ids())
                            {
                                element& current_element = elements[el_index];
                                printf("looking at element %d\n", current_element.get_id());
                                if(current_element.is_el_resistor())
                                {
                                    printf("is resistor and resistance is %d\n", current_element.get_val());
                                    term1 += 1.0/current_element.get_val(); 
                                    int needed_node; 
                                    if (current_element.get_node_id1() == current_node.get_id()) 
                                        needed_node = current_element.get_node_id2();
                                    else //vice versa of above
                                        needed_node = current_element.get_node_id1();
                                    term2 += nodes[needed_node].get_voltage()/current_element.get_val();
                                    printf("term 2 is %.2f\n", term2);
                                }
                            }
                            printf("term1: %.2f term2: %.2f\n", term1, term2);
                            if(current_node.is_changeable())
                            {
                                printf("setting %d to %.2f\n", current_node.get_id(), term2/term1);
                                current_node.set_voltage(term2/term1); //changes node voltage according to equation
                            }
                            if (abs(old_voltage-current_node.get_voltage()) >= MIN_ITERATION_CHANGE) 
                                small_change = false;
                    }
                    if (small_change)
                    {   
                        break;
                    }
    }
}
