#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <stack>
#include <queue>
#include <set>

#include "tron.h"
#include "util.h"

tron::~tron(){}

action tron::act(sensor s){
	return east;
}

action ask::act(sensor s){
	int choice;
	std::cout << "You are mister B, what action will you take?" << std::endl;
	std::cout << north << "north" << std::endl;
	std::cout << east << "east" << std::endl;
	std::cout << south << "south" << std::endl;
	std::cout << west << "west" << std::endl;
	std::cin >> choice;
	return action(choice);
}

action random_bot::act(sensor s){
	//verificar que haya un espacio libre
	if(libre(s)){
		int K;
		//elegir el movimiento al azar
		for(K = rand()%4; ; K = rand()%4){
			if(K==north and s.vecino[0][1]==empty) break;
			if(K==east and s.vecino[1][2]==empty) break;
			if(K==south and s.vecino[2][1]==empty) break;
			if(K==west and s.vecino[1][0]==empty) break;
		}
		return action(K);
	}
	else
		return south;
}

bool tron::libre(const sensor &s){
	for(size_t K=0; K<3; ++K)
		for(size_t L=0; L<3; ++L){
			if(K!=1 and L!=1) continue;
			if(s.vecino[K][L] == empty)
				return true;
		}
	return false;
}

greedy::greedy(int n): searching(true){
	//considara que lo que no conoce está bloqueado
	w.resize( n+2, std::vector<estado>(n+2, wall) );
}

static bool other(estado, estado);

action greedy::act(sensor s){
	//se tiene un cambio de estrategia cuando se encuentra rastro del rival
	if( searching ){
		for(size_t K=0; K<3; ++K)
			for(size_t L=0; L<3; ++L)
				if( other(s.vecino[1][1], s.vecino[K][L]) ){ //enemigo a la vista
					searching = false;
					break;
				}		
	}
	std::cerr << "Searching " << ((searching)?"si":"no") << std::endl;
	update(s);
	if( not libre(s) ) return south;
//	return busqueda(s.pos);
	action a = busqueda(s.pos);
//rellenar las esquinas	
	for(int K=-1; K<=1; ++K)
		for(int L=-1; L<=1; ++L)
			w[s.pos.first+K][s.pos.second+L] = s.vecino[K+1][L+1];
	
	return a;
}

void greedy::update(const sensor &s){
	//si no se encontro al rival, se ignoran las celdas de las esquinas
	w[s.pos.first][s.pos.second] = A;
	for(int K=-1; K<=1; ++K)
		for(int L=-1; L<=1; ++L){
			if(searching and K!=0 and L!=0) continue;
			w[s.pos.first+K][s.pos.second+L] = s.vecino[K+1][L+1];
		}
}

action greedy::busqueda(greedy::state::pos pos){ //busqueda a lo ancho
	state beg(pos, w);
	std::queue<node> son;
	std::vector<edge> arbol;
	
	std::set<state> visited;

	//inicializacion
	son.push( node(beg, arbol.size(), 0, south) );
	visited.insert(beg);
	arbol.push_back( edge(0, 0, south) );

	//busqueda a lo ancho
	while( not son.empty() ){
		node actual = son.front(); son.pop();
		std::vector<node> heredero = sucesor(actual);

		for(size_t K=0; K<heredero.size(); ++K){
			if( visited.find(heredero[K].s)==visited.end() ){ //posible camino
				//creación del enlace
				visited.insert( heredero[K].s );
				heredero[K].me = arbol.size();
				arbol.push_back( edge(heredero[K].me, actual.me, heredero[K].act) );
				son.push( heredero[K] );
			}
		}
	}
	
	print( arbol, "arbol" );
	//devolucion de la accion
	int K = arbol.size()-1;
	if( K<=0 ) throw "an exception";
	//la prueba de meta se verifica en el ultimo nodo. Por construccion
	while( arbol[K].parent!=0 ) K = arbol[K].parent; //buscar la accion que llevo a ese estado
	return arbol[K].act;
}

std::vector<greedy::node> greedy::sucesor(const greedy::node &n){
	//expansion del nodo
	std::vector<node> herederos;
	state::pos me = n.s.me;
	for(int K=-1; K<=1; ++K)
		for(int L=-1; L<=1; ++L){
			if( K!=0 and L!=0 ) continue;
			if( n.s.mundo[me.first+K][me.second+L] == empty ){ //opcion valida
				herederos.push_back( n );
				herederos.back().parent = n.me;
				herederos.back().s.mundo[me.first+K][me.second+L] = trace_A;

				//registrar la accion
				if     (K==-1) herederos.back().act = north; 
				else if(K== 1) herederos.back().act = south;
				else if(L==-1) herederos.back().act = west;
				else if(L== 1) herederos.back().act = east;
				else std::cerr << "Something went terrible wrong "<< K << ' ' << L , std::cin.get();
	
				herederos.back().s.me.first+=K;
				herederos.back().s.me.second+=L;
			}
		}
	//orden aleatorio para evitar previsibilidad
	std::random_shuffle(herederos.begin(), herederos.end());
	return herederos;
}

void greedy::print( const std::vector<greedy::edge> &arbol, const char *filename ){
	static std::string Action[] = {"north", "east", "south", "west"};
	std::ofstream out(filename, std::ios::app | std::ios::ate);
	if(not out)
		out.open(filename, std::ios::out);

	for(int K=0; K<20; ++K) out << '-';
	out << "\nArbol de busqueda\n";
	out << "id_nodo   id_padre   action_de_cambio\n";

	for( size_t K=0; K<arbol.size(); ++K)
		out << K << ' ' << arbol[K].parent << ' ' << Action[arbol[K].act] <<'\n';
	out << std::endl << std::endl;
}

static bool other(estado a, estado b){
	//ver al enemigo
	if(     (a==A or a==trace_A) and (b==B or b==trace_B)
		or  (a==B or a==trace_B) and (b==A or b==trace_A) )
		return true;
	return false;
}

