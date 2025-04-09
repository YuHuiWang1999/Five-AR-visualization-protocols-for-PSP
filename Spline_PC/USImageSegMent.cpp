#include "USImageSegMent.h"
USImageSegMent::USImageSegMent(const char* pth)
{
	Py_SetPythonHome(L"C:\\Users\\Administrator\\miniconda3\\envs\\cnn\\");
	Py_Initialize();

	const char* dir1 = pth;

	PyObject* pModule = NULL;
	PyObject* pFunc = NULL;

	pModule = PyImport_ImportModule("api");         //注意文件名字大小写
	pFunc = PyObject_GetAttrString(pModule, "preparation"); //获取函数名称
	
	PyObject* pArg = PyTuple_New(1);


	PyTuple_SetItem(pArg, 0, Py_BuildValue("s", dir1));

	PyEval_CallObject(pFunc, pArg);
}
USImageSegMent::~USImageSegMent()
{
	Py_Finalize();                                  //调用Py_Finalize,和Py_Initialize相对应的.
}

void USImageSegMent::Do_Segment(const char * img, int target)
{
	const char* dir2 = img;

	PyObject* pModule = NULL;
	PyObject* pFunc = NULL;

	pModule = PyImport_ImportModule("api");         //注意文件名字大小写
	if (target == 1)
	{
		pFunc = PyObject_GetAttrString(pModule, "seg_single"); //获取函数名称
	}
	else
	{
		pFunc = PyObject_GetAttrString(pModule, "seg_group"); //获取函数名称
	}


	double CArray[] = { 0, 0, 448, 448 };
	//double CArray[] = { 323, 765, 95, 527 };
	PyObject* PyList = PyList_New(4);
	for (int i = 0; i < PyList_Size(PyList); i++)
		PyList_SetItem(PyList, i, PyFloat_FromDouble(CArray[i]));//给PyList对象的每个元素赋值

	PyObject* pArg = PyTuple_New(2);
	PyTuple_SetItem(pArg, 1, PyList);

	PyTuple_SetItem(pArg, 0, Py_BuildValue("s", dir2));


	PyEval_CallObject(pFunc, pArg);
}
