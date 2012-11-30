

#include <iostream>
#include <functional>

#include <boost/function.hpp>

#include <boost/python.hpp>

#include <QApplication>
#include <QTimer>

#include "test_signal.h"

using namespace std;


namespace py = boost::python;


qint64 unwrap(const QObject& ptr) {
    return reinterpret_cast<qint64>(&ptr);
}

/** to-python convert to QObjects */
struct QObject_to_python
{
    static PyObject* convert(const QObject& obj)
    {
        return boost::python::incref(py::object(unwrap(obj)).ptr());
    }
};

void initializeConverters()
{
    // register the to-python converter
    boost::python::to_python_converter<QObject, QObject_to_python>();
}

int main(int argc, char* argv[])
{
    initializeConverters();
    
    QApplication app(argc, argv);
    
    Py_Initialize();
    
    py::object main_module = py::import("__main__");
    py::object main_namespace = main_module.attr("__dict__");
    
    main_namespace["sys"] = py::import("sys");

    
    TestSignal ts;
    ts.connect(&ts, SIGNAL(activate(QString)), SLOT(print(QString)));
    
    try
    {
        py::object ptr(static_cast<QObject&>(ts));
        main_namespace["ts_raw"] = ptr;
        boost::python::exec_file("./main.py", main_namespace, main_namespace);
//         return app.exec();
    }
    catch(py::error_already_set const &)
    {
        PyErr_Print();
    }   

    
    return 0;
}
