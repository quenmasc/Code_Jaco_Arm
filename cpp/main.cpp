#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "PythonBridge.h"
#include <thread>
PythonBridge Bridge ;

void foo(){
	PyGILState_STATE gstate=PyGILState_Ensure();
	Bridge.Running_python();
	PyGILState_Release(gstate);
}
void fol(){
	Bridge.ClassValue2();
}
void mess (){
	while(1){
			sleep(2);
			printf("oui");
	}
}
int main(){
	std:: thread first(foo);
	std:: thread second(fol);
	std:: thread third(mess);
	first.join();
	second.join();
	third.join();
	return 0 ;
	
}

