#pragma once

#ifndef DASH_Utility

#define DASH_Utility

//_BEGIN_DASH_STRUCTURES_NAMESPACE

#include <cassert>


	template<class _first, class _second>
struct DPair{

	_first first;

	_second second;

	DPair(): first(_first()), second(_second()){}

	DPair(_first __F,_second __S):first(__F),second(__S){}

	};

//rango [from,to)
	template< class inputITERATOR1 , class inputITERATOR2 , class _Tp>
void Fill( inputITERATOR1 from,  inputITERATOR2 to, const _Tp& val){

	for(;from != to;from++){
			
			*from=val;
		}

	}

//rango [from,to)
	template< class inputITERATOR1 , class inputITERATOR2 , class _Tp >
void Copy( inputITERATOR1 from , inputITERATOR2 to , _Tp* ret ){

	for(;from != to;from++){
			*ret=*from;
			ret++;
		}

	}

class OUT_OF_RANGE:std::exception{
public:
	const char* what() const{
		return "Se intento desreferenciar una posicion fuera del alcance del vectorn\n";
		}


	};


//_END_DASH_STRUCTURES_NAMESPACE


#endif