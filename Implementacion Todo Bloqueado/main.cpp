#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include "tron.h"
#include "ambiente.h"
#include "util.h"

template<class T>
void convert(const std::string &s, T &value){
	std::stringstream ss(s);
	ss >> value;
}

int main(int argc, char **argv){
	int n;
	bool random;
	srand( time(NULL) );
	if(argc>1)
		convert(argv[1], n);
	else
		n=10;
	if( argc==3 and std::string(argv[2])=="random" )
		random = true;
	else
		random = false;

	greedy Tron(n);
	random_bot machine;
	//ask machine;
	ambiente simple(n);
	simple.add(&machine, B, random);
	simple.add(&Tron, A, random);

	std::cout << "Start " << std::endl;

	while( not simple.end() ){
		simple.print();
		simple.run();
		if( std::cin.get() == 'q' ) break;
	}
	return 0;
}

