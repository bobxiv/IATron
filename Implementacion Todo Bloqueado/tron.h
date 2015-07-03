#ifndef ASPIRADORA_H
#define ASPIRADORA_H

#include <vector>
#include <set>
#include <list>
#include <utility>
#include "util.h"

class tron{
protected:
	typedef std::vector<std::vector<estado> > world;
	bool libre(const sensor &);
public:
	virtual action act(sensor); //the decision goes here
	virtual ~tron();
};

class ask: public tron{
public:
	virtual action act(sensor);
};

class random_bot: public tron{
public:
	virtual action act(sensor);
private:
};

class greedy: public tron{
public:
	greedy(int n);
	virtual action act(sensor);
private:
	void update(const sensor &);
	world w;

	bool searching;
	
	struct state{
	typedef std::pair<int,int> pos;
		world mundo;
		pos me;
		state(){}
		state(pos me, const world &w): mundo(w), me(me){}
		bool operator==(const state &b) const{
			return mundo==b.mundo and me==b.me;
		}
		bool operator!=(const state &b) const{
			return not ((*this) == b);
		}
		bool operator<(const state &b) const{
			if(me == b.me) 
				return mundo < b.mundo;
			return me < b.me;
		}
	};
	struct node{
		node(const state &s, int me, int p, action a):s(s), me(me), parent(p), act(a){}
		state s;
		int me, parent;
		action act;
	};

	struct edge{
		int me, parent;
		action act;
		edge(){}
		edge(int m, int p, action a):me(m), parent(p), act(a){}
	};
	
	static void print( const std::vector<edge> &, const char * );
	action busqueda(state::pos);
	std::vector<node> sucesor(const node &);
};


#endif

