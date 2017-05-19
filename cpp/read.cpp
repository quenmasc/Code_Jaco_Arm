#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

//define FIFO_NAME "\home\pi\Code_Jaco_Arm\cpp\fifo"
int main (){
	const char *fifo_name="/home/pi/Code_Jaco_Arm/cpp/fifo";
	mknod(fifo_name,S_IFIFO | 0666,0);
	std::ifstream f(fifo_name);
	std::string(line);
	getline(f,line);
	auto data_size=std::stoi(line);
	std::cout << "Size" << data_size << std::endl;
	std::string data;
	{
		std::vector<char> buf(data_size);
		f.read(buf.data(),data_size);
		data.assign(buf.data(),buf.size());
	}
	if (!f.good()){
		std::cerr <<"read failed" << std::endl ;
	}
	std::cout << "Data size" << data.size() << "content" << data << std::endl ;
}
