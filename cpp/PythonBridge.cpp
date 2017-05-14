#include "PythonBridge.h"
/* The code below is from Ulysse Project */
PyObject *pNameSVM , *pModuleSVM , *pDictSVM , *pClassSVM , *pInstanceSVM;


PythonBridge::PythonBridge(){
	const char* python_information[]={"PythonFile","SpeechRecognition","Speech_Recognition"};
	// build project name
	pNameSVM=PyString_FromString(python_information[1]);
	// load the module project
	pModuleSVM=PyImport_Import(pNameSVM);
	// pdict  is q borrewd reference
	pDictSVM=PyModule_GetDict(pModuleSVM);
	// Build the name of the callable class 
	pClassSVM=PyDict_GetItemString(pDictSVM,python_information[2]);
	
	pInstanceSVM=NULL;
	// create an instance of the class
	if(PyCallable_Check(pClassSVM)){
		pInstanceSVM=PyObject_CallObject(pClassSVM,NULL);
	}
	
}
void PythonBridge::finalize_python(){
	Py_DECREF(pInstanceSVM);
	Py_DECREF(pDictSVM);
	Py_DECREF(pModuleSVM);
	Py_DECREF(pNameSVM);
	Py_Finalize();
}

void PythonBridge::Running_python(){
	PyObject_CallMethod(pInstanceSVM,"run","");
}



