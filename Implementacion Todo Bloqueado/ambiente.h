#ifndef AMBIENTE_H
#define AMBIENTE_H

#include <vector>
#include "util.h"

class tron;

class ambiente{
public:
	ambiente(size_t n);
	virtual void run();
	virtual ~ambiente();
	virtual void add(tron *, estado who, bool random);
	virtual void print();
	bool end();
protected:
	struct agente{
		agente(tron *, int, int, estado);

		std::pair<int, int> avance(action);
		virtual action act();

		tron *me;
		sensor s;
		std::pair<int, int> pos;
		estado who;
		bool on;
		void print();
	};
private:
	void feel(agente &);
	virtual void update(agente &, action);
	virtual bool inside_world(std::pair<int, int> );
	void end_game(agente &);
protected:
	std::vector<agente*> agentes;
	std::vector<std::vector<estado> > world;
};

#endif

