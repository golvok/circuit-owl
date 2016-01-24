#include <algorithm>
#include <iostream>
#include <memory>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "text/text_finder.hpp"

using namespace cv;

class element
{
    public:
    element(int _id, Point& _NE, Point& _SW, int _val=0, bool _is_resistor=true) 
            : id(_id), NE(_NE), SW(_SW), val(_val), is_resistor(_is_resistor) {}

    int get_id() { return id; }
    int get_val() { return val; }
    
    Point& get_SW() { return SW; }
    Point& get_NE() { return NE; }

    int get_node_id1() { return node_id[0]; }
    int get_node_id2() { return node_id[1]; }

    bool is_el_resistor() { return is_resistor; }
    bool is_el_voltage() { return !is_resistor; }

    void set_val(int _val) { val = _val; }

    void set_node_id1(int id1) { node_id[0] = id1; }
    void set_node_id2(int id2) { node_id[1] = id2; }

    private:
    int id;
    Point NE;
    Point SW;
    int val;
    bool is_resistor;
    int node_id[2];
};

class node
{
    public:
    node(int _id, double _voltage, vector<int>& _element_ids, bool _changeable=true) 
        : id(_id), voltage(_voltage), element_ids(_element_ids), changeable(_changeable) {}

    vector<int>& get_element_ids() { return element_ids; }
    double get_voltage() { return voltage; }
    int get_id() { return id; }
    bool is_changeable() { return changeable; }    
    
    void set_voltage(double _voltage) { voltage = _voltage; }

    private:
    int id;
    double voltage;
    vector<int> element_ids;
    bool changeable;
};

void solve_voltages(vector<node>& nodes, vector<element>& elements);
void annotate(vector<element>& elements, vector<tess_result>& texts);
