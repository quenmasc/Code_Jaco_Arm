#include <python2.7/Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

class PythonBridge {
	public :
		PythonBridge();
		void finalize_python();
		void Running_python();
};
