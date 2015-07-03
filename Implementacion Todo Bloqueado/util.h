#ifndef UTIL_H
#define UTIL_H

enum estado{empty, wall, A, trace_A, B, trace_B};
enum action{north, east, south, west};

struct sensor{
	//estado de las cuatro casillas vecinas
	estado vecino[3][3];
	std::pair<int, int> pos;
};

namespace ptr{
	class destroy{
	public:
		template<class T>
		void operator()(T *p) const{
			delete p;
		}
	};
}

#endif

