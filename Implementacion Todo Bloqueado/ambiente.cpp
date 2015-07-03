#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>

#include "ambiente.h"
#include "tron.h"
#include "util.h"

ambiente::ambiente(size_t n){
	world.resize(n+2,std::vector<estado>(n+2,empty));
	for(size_t K=0; K<n+2; ++K) //centinelas
		world[0][K] = world[n+1][K] = world[K][0] = world[K][n+1] = wall;
}

ambiente::~ambiente(){
	std::for_each(
		agentes.begin(),
		agentes.end(),
		ptr::destroy()
	);
}

void ambiente::add(tron *A, estado who, bool random=false){
	if(random){
		int r,c;
		for(r=rand()%world.size(), c=rand()%world.size();
			world[r][c] != empty; 
			r=rand()%world.size(), c=rand()%world.size() )
				;
			
		world[r][c] = who;
		agentes.push_back(new agente(A, r, c, who));
			
	}
	else{
		int n = 1;
		if(world[n][n] != empty)
			n = world.size()-2;
		world[n][n] = who;
		agentes.push_back(new agente(A, n, n, who));
	}
}

void ambiente::run(){
	for(size_t K=0; K<agentes.size(); ++K){
		feel(*agentes[K]);
		update(*agentes[K], agentes[K]->act());
	}
}

void ambiente::feel(agente &ag){
	for(int K=-1; K<=1; ++K)
		for(int L=-1; L<=1; ++L)
			ag.s.vecino[K+1][L+1] = world[ag.pos.first+K][ag.pos.second+L];
		
	ag.s.pos = ag.pos;
}

bool ambiente::end(){
	for(size_t K=0; K<agentes.size(); ++K)
		if(agentes[K]->on)
			return false;
	return true;
}

void ambiente::end_game(agente &ag){
	std::cerr << "El agente " << ((ag.who==A)?'A':'B') << " acaba de morir" << std::endl;
	std::vector<agente *>::iterator it = std::find( agentes.begin(), agentes.end(), &ag );
	delete *it;
	agentes.erase(it);
}

void ambiente::update(agente &ag, action ac){
	std::cout << "The choice was to ";
	switch(ac){
	case north: 
		std::cout << "north" << std::endl;
		break;
	case east: 
		std::cout << "east" << std::endl;
		break;
	case south: 
		std::cout << "south" << std::endl;
		break;
	case west:
		std::cout << "west" << std::endl;
		break;
	default:
		std::cout << "invalid " << ac << std::endl;
		return;
	}
	std::pair<int,int> npos = ag.avance(ac);
	
	if(world[npos.first][npos.second] != empty){
		end_game(ag);
		return;
	}
	world[npos.first][npos.second] = ag.who;
	world[ag.pos.first][ag.pos.second] = estado(ag.who+1);
	ag.pos = npos;
}

static bool between(int lower, int n, int upper){
	return lower<=n and n<upper;
}

bool ambiente::inside_world(std::pair<int, int> p){
	return 
		between(0, p.first, world.size()) and
		between(0, p.second, world[0].size());
}

void ambiente::print(){
	for(size_t K=0; K<agentes.size(); ++K){
		feel(*agentes[K]);
		agentes[K]->print();
	}

	for(size_t K=0; K<world.size(); ++K){
		for(size_t L=0; L<world[K].size(); ++L)
			switch(world[K][L]){
			case empty: 
				std::cout << '.';
				break;
			case wall: 
				std::cout << '#';
				break;
			case A: 
				std::cout << 'A';
				break;
			case B: 
				std::cout << 'B';
				break;
			case trace_A: 
				std::cout << 'o';
				break;
			case trace_B: 
				std::cout << 'x';
				break;
			}
		std::cout << std::endl;
	}
}

ambiente::agente::agente(tron *a, int x, int y, estado e): me(a), who(e){
	pos.first = x;
	pos.second = y;
	on = true;
}

std::pair<int,int> ambiente::agente::avance(action dir){
	std::pair<int,int> nuevo = pos;
	switch(dir){
	case north:
		--nuevo.first;
		break;
	case south:
		++nuevo.first;
		break;
	case east:
		++nuevo.second;
		break;
	case west:
		--nuevo.second;
		break;
	}
	return nuevo;
}

action ambiente::agente::act(){
	return me->act(s);
}

#define PRINT(x) std::cout << ##x << ' ' << (x) << std::endl;

void ambiente::agente::print(){
	std::cout << "Posicion "<<pos.first<<','<<pos.second<<std::endl;
	std::cout << "Sensores (thd)\n";
	for(size_t K=0; K<3; ++K){
		for(size_t L=0; L<3; ++L)
			std::cout << s.vecino[K][L] << ' ';
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

#undef PRINT

