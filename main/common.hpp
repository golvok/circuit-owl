#ifndef MAIN_COMMON_CPP
#define MAIN_COMMON_CPP

#include <boost/python.hpp>

//----Shared elements
//------------------------------------------------------------

class CircuitElement
{
    public:
        // Accessible
        int id;
        int val;
        int type;
        int node_id[2];

        // Some assembly required
        Point NE;
        Point SW;
};

class CircuitNode
{
    public:
        // Accessible
        int id;
        double voltage;        
        bool changeable;

        // Some assembly required
        std::vector<int> element_ids;
        std::vector<std::string>::iterator elem_id_begin() {return element_ids.begin();}        
        std::vector<std::string>::iterator elem_id_end() {return element_ids.end();}
};

BOOST_PYTHON_MODULE(circuit_analyzer)
{
    using namespace boost::python;
    def("analyze_photo", analyze_photo);

    class_<CircuitElement>("CircuitElement")
        .def_readwrite("id", & CircuitElement::id)
        .def_readwrite("val", & CircuitElement::val)
        .def_readwrite("is_resistor", & CircuitElement::type)
        .def_readwrite("node_id", & CircuitElement::node_id)

    class_<CircuitNode>("CircuitNode")
        .def_readwrite("id", &CircuitNode::id)
        .def_readwrite("voltage", &CircuitNode::voltage)
        .def_readwrite("changeable", &CircuitNode::changeable)
        .def_readwrite("element_ids", range(&CircuitNode::elem_id_begin, &CircuitNode::elem_id_end))
}

#endif  /* MAIN_COMMON_CPP */