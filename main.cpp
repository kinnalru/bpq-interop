
#include <iostream>
#include <functional>

#include <boost/function.hpp>
#include <boost/python.hpp>

#include <QApplication>
#include <QTimer>
#include <QLineEdit>

#include "test_signal.h"

using namespace std;
namespace py = boost::python;

/** Class to create PyQt and C++ Qt pointer conversion */
template <typename Object>
struct QObject_and_PyQt
{
	QObject_and_PyQt(const std::string& classname, py::object& main_namespace)
	{
		name_ = classname;
		main_namespace_ = &main_namespace;
	}

	static PyObject* to_python(void const* obj)
	{
		//raw C++ pointer from any QObject
		qint64 qptr = reinterpret_cast<qint64>(obj);
		
		try {
			//evaluate PyQt4 class class from classname
			py::object qtclass = py::eval(name_.c_str(), *main_namespace_, *main_namespace_);
			//wrap raw C++ pointer with sip-class
			py::object wrapped = py::import("sip").attr("wrapinstance")(qptr, qtclass);
			
			return py::incref(wrapped.ptr());
		}
		catch(py::error_already_set const &)
		{
			std::cerr << name_ << " to_python Exception:" << std::endl;
			PyErr_Print();
		}
		catch(const std::exception& e) {
			std::cerr << name_ << " to_python Exception:" << e.what() << std::endl;
		}
		catch(...){
			std::cerr << name_ << " to_python Exception: Unknown" << std::endl;
		}

		return 0;
	};

	static void* from_python(PyObject* obj)
	{
		try {
			//get raw C++ pointer from PyQt4 object
			py::object unwrapped = py::import("sip").attr("unwrapinstance")(py::borrowed(obj));
			quint64 qptr = py::extract<quint64>(unwrapped);
			
			return qobject_cast<Object*>(reinterpret_cast<QObject*>(qptr));
		}
		catch(py::error_already_set const &)
		{
			std::cerr << name_ << " from_python Exception:" << std::endl;
			PyErr_Print();
		}
		catch(const std::exception& e) {
			std::cerr << name_ << " from_python Exception:" << e.what() << std::endl;
		}
		catch(...){
			std::cerr << name_ << " from_python Exception: Unknown" << std::endl;
		}

		return 0;
	}

private:
	static py::object* main_namespace_;
	static std::string name_;
};

template <typename Object>
std::string QObject_and_PyQt<Object>::name_;

template <typename Object>
py::object* QObject_and_PyQt<Object>::main_namespace_;



template <typename Object>
void initialize_converter(const std::string& classname, py::object& main_namespace)
{
	// Configure convertert static members
	QObject_and_PyQt<Object> converter(classname, main_namespace);
	
	// register the to-python converter
	py::converter::registry::insert(converter.to_python, py::type_id<Object>());

	// register the from-python converter
	py::converter::registry::insert(converter.from_python, py::type_id<Object>());
}

/// help macros
#define INITIALIZE_CONVERTER(classname, main_namespace)\
	initialize_converter<classname>(#classname, main_namespace)

void initializeConverters(py::object& main_namespace)
{
	INITIALIZE_CONVERTER(QObject, main_namespace);
	INITIALIZE_CONVERTER(QWidget, main_namespace);
// 	py::class_<TestSignal, py::bases<QObject>, boost::noncopyable >("TestSignal");
	INITIALIZE_CONVERTER(TestSignal, main_namespace);	
    
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    Py_Initialize();

    try
    {
	
		py::object main_module = py::import("__main__");
		py::object main_namespace = main_module.attr("__dict__");

		main_namespace["sys"] = py::import("sys");
		main_namespace["sip"] = py::import("sip");
		py::exec("from PyQt4.Qt import *", main_namespace, main_namespace);

		initializeConverters(main_namespace);

		TestSignal ts;
		ts.connect(&ts, SIGNAL(activate(QString)), SLOT(print(QString)));

        main_namespace["ts_raw"] = ts;
		main_namespace["www"] = 1;
		main_namespace["iww"] = 1;

	    boost::python::exec_file("./main.py", main_namespace, main_namespace);

		try{
			QWidget* o = py::extract<QWidget*>(main_namespace["www"]);
			std::cerr << "Win class name:" << o << std::endl;
		}
		catch(...) {
			PyErr_Print();
			std::cerr <<"can't convert" << std::endl;
		}
		
        return app.exec();
    }
    catch(py::error_already_set const &)
    {
        PyErr_Print();
		return -1;
    }   

    
    return 0;
}
