#ifndef MAIN_MAIN_CPP
#define MAIN_MAIN_CPP

#include <boost/python.hpp>

//----Top-level function
//------------------------------------------------------------
char const* analyze_photo(char const* filename_in, char const* filename_out, bool debug);


BOOST_PYTHON_MODULE(circuit_analyzer)
{
    using namespace boost::python;
    def("analyze_photo", analyze_photo);
}

#endif  /* MAIN_MAIN_CPP */