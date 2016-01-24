#ifndef MAIN_COMMON_CPP
#define MAIN_COMMON_CPP

#include <opencv2/core/core.hpp>

#include <iostream>
#include <iterator>

//----Shared elements
//------------------------------------------------------------

class CircuitElement
{

    public:
        CircuitElement()
            : id(0), NE(), SW(), val(0), type(0) {};

        CircuitElement(int _id, cv::Point _NE, cv::Point _SW, int _v, int _t)
            : id(_id), NE(_NE), SW(_SW), val(_v), type(_t) {};
         
        int id;
        cv::Point NE;
        cv::Point SW;
        int val;
        int type; // 0 is resistor
        int node_id[2];
};

class CircuitNode
{
    public:
        CircuitNode()
            : id(0), voltage(0), element_ids(), changeable(true), centroid() {};

        CircuitNode(int _id, double _v, std::vector<int> _elem, bool _c)
            : id(_id), voltage(_v), element_ids(_elem), changeable(_c), centroid() {};

        CircuitNode(int _id, std::vector<int> _elem, cv::Point _centroid)
            : id(_id), voltage(0), element_ids(_elem), changeable(true), centroid(_centroid) {};

        int id;
        double voltage;
        std::vector<int> element_ids;
        bool changeable;

        cv::Point centroid;

        // Some assembly required        
        std::vector<int>::iterator elem_id_begin() {return element_ids.begin();}        
        std::vector<int>::iterator elem_id_end() {return element_ids.end();}
};

inline std::ostream& operator<<(std::ostream& os, const CircuitNode& node) {
    os << "CircuitNode: id=" << node.id << " -> ";
    std::copy(node.element_ids.begin(), node.element_ids.end(), std::ostream_iterator<int>(os, ", "));
    os << ", V=" << node.voltage << ", changeable=" << node.changeable << ", centroid=" << node.centroid;
    return os;
}

/*
BOOST_PYTHON_MODULE(circuit_common)
{
    using namespace boost::python;

    class_<CircuitElement>("CircuitElement")
        .def_readwrite("id", &CircuitElement::id)
        .def_readwrite("val", &CircuitElement::val)
        .def_readwrite("is_resistor", &CircuitElement::type)
        .def_readwrite("node_id", &CircuitElement::node_id);

    class_<CircuitNode>("CircuitNode")
        .def_readwrite("id", &CircuitNode::id)
        .def_readwrite("voltage", &CircuitNode::voltage)
        .def_readwrite("changeable", &CircuitNode::changeable)
        .def_readwrite("element_ids", range(&CircuitNode::elem_id_begin, &CircuitNode::elem_id_end));
}
*/

#endif  /* MAIN_COMMON_CPP */