#pragma once

#include <vector>

class Agent;

//General class for ambient
class Ambient
{
	protected:
		//Agent inside the ambient
		std::vector<Agent*> m_apAgents;

	public:
		Ambient();

		virtual ~Ambient() = 0;

		virtual void Update();
};

//General class for all agents
class Agent
{
	protected:
	

	public:
		Agent();

		virtual ~Agent();

		virtual void ExecuteSensors() = 0;

		virtual void ExecuteActions() = 0;
};