

#include <iostream>
#include <functional>

#include <boost/function.hpp>

#include <boost/python.hpp>
#include <boost/python/call.hpp>

#include <QApplication>

using namespace std;


namespace py = boost::python;



int main(int argc, char* argv[])
{

    QApplication app(argc, argv);
    
    Py_Initialize();

    py::object main_module = py::import("__main__");
    py::object main_namespace = main_module.attr("__dict__");
    
    main_namespace["sys"] = py::import("sys");


//     boost::python::exec("print(123)", main_namespace, main_namespace);


    try
    {
        boost::python::exec_file("./main.py", main_namespace, main_namespace);
    }
    catch(py::error_already_set const &)
    {
        PyErr_Print();
    }   

    
    return 0;
}
