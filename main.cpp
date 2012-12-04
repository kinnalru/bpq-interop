

#include <iostream>
#include <functional>

#include <boost/function.hpp>

#include <boost/python.hpp>

#include <QApplication>
#include <QTimer>
#include <QLineEdit>

#include "test_signal.h"

using namespace std;


using namespace boost;
using namespace boost::python;
using namespace boost::python::converter;
namespace py = boost::python;


qint64 unwrap(const QObject& ptr) {
    return reinterpret_cast<qint64>(&ptr);
}

QObject& wrap(qint64 pyptr) {
    return reinterpret_cast<QObject&>(pyptr);
}

/** to-python convert to QObjects */
struct QObject_to_python
{
    static PyObject* convert(const QObject& obj)
    {
        return boost::python::incref(py::object(unwrap(obj)).ptr());
    }
};

struct QWidget_from_python
{
	QWidget_from_python()
	{
		boost::python::converter::registry::push_back(
		&convertible,
		&construct,
		boost::python::type_id<QWidget*>());

		boost::python::converter::registry::push_back(
		&convertible,
		&construct,
		boost::python::type_id<QWidget>());

		boost::python::converter::registry::push_back(
		&convertible,
		&construct,
		boost::python::type_id<QWidget&>());

		boost::python::converter::registry::push_back(
		&convertible,
		&construct,
		boost::python::type_id<const QWidget&>());
	}

	static void* convertible(PyObject* obj_ptr)
	{
	//       if (!PyString_Check(obj_ptr)) return 0;
// 		return obj_ptr;

		std::cerr <<"HERE1" << std::endl;
		std::cerr << obj_ptr->ob_type->tp_doc << std::endl;

		return 0;
	}

	static void construct(
		PyObject* obj_ptr,
		boost::python::converter::rvalue_from_python_stage1_data* data)
	{
		std::cerr << "HERE" << std::endl;
// 		const char* value = PyString_AsString(obj_ptr);
// 		
// 		if (value == 0) boost::python::throw_error_already_set();
// 		void* storage = (
// 		(boost::python::converter::rvalue_from_python_storage<custom_string>*)
// 			data)->storage.bytes;
// 		new (storage) custom_string(value);
// 		data->convertible = storage;
	}
};

void* extract_pointer_1(PyObject* obj)
{
	std::cerr << "HERE" << std::endl;
	std::cerr << obj->ob_type->tp_doc << std::endl;

	try {
		object o1(borrowed(obj));
	
		object o2 = import("sip").attr("unwrapinstance")(o1);
	
		std::cerr << "here2" << std::endl;
		std::cerr << "Val:" << extract<quint64>(o2) << std::endl;
		std::cerr << "here3" << std::endl;
	}
	catch(const std::exception& e) {
		std::cerr << "Exception:" << e.what() << std::endl;
	}
	
// 		return incref(obj.ptr());
// 		
// 	if ()
	
	return 0;
}



void initializeConverters()
{
    // register the to-python converter
    boost::python::to_python_converter<QObject, QObject_to_python>();

	QWidget_from_python();
}

int main(int argc, char* argv[])
{
    initializeConverters();
    
    QApplication app(argc, argv);
    
    Py_Initialize();

	converter::registry::insert(&extract_pointer_1, type_id<QWidget>());
    
    py::object main_module = py::import("__main__");
    py::object main_namespace = main_module.attr("__dict__");
    
    main_namespace["sys"] = py::import("sys");

    
    TestSignal ts;
    ts.connect(&ts, SIGNAL(activate(QString)), SLOT(print(QString)));

// 	QLineEdit* le = new QLineEdit();

// 	QObject* o = main_namespace["w"].extract<QObject*>();

    try
    {
        main_namespace["ts_raw"] = static_cast<QObject&>(ts);
		main_namespace["www"] = 1;
		main_namespace["iww"] = 1;

	
        boost::python::exec_file("./main.py", main_namespace, main_namespace);

		std::cerr << "ok cerr" << std::endl;
		std::cout << "ok cout" << std::endl;
// 
// 		py::object o = main_namespace["w"];
		
// 		std::cerr << get_lvalue_from_python(o.ptr(), registered_pointee<QWidget>::converters) << std::endl;;

		try{
			QWidget* o = py::extract<QWidget*>(main_namespace["www"]);
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
