#pragma once

#include "Agent.h"

#include "AmbientTron.h"//haves define LADO_GRILLA

#include "Dash_TreeMod.h"

//Inludes de SFML
#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

#include <vector>

//class Tron;

class AgentTron: public Agent
{
	private:
		//The implemented search methods
		enum SearchMethod{BroadSearch,DeepSearch,LimitedDeepSearch};

		//If the operators should be applied in random order vs predefine order
		bool m_RandomOperatorsOrder;

		//The id of the agent
		int  m_id;

		//The maximum deep to make the limited deep search
		int  m_LimitSearchNumber;

		#pragma region Sensors States

			struct State
			{
				State()
				{
					for(int k=0 ; k < LADO_GRILLA*LADO_GRILLA ; ++k)
						m_Grid[k] = '-';
				}
				State(const State& B)
				{
					for(int k=0 ; k < LADO_GRILLA*LADO_GRILLA ; ++k)
						m_Grid[k] = B.m_Grid[k];
					m_TronOurPosition = B.m_TronOurPosition;
				}
				State(char Grid[LADO_GRILLA*LADO_GRILLA], sf::Vector2i TronPosition)
				{
					for(int k=0 ; k < LADO_GRILLA*LADO_GRILLA ; ++k)
						m_Grid[k] = Grid[k];
					m_TronOurPosition = TronPosition;
				}
				State(sf::Vector2i TronPosition)
				{
					for(int k=0 ; k < LADO_GRILLA*LADO_GRILLA ; ++k)
						m_Grid[k] = '-';
					m_TronOurPosition = TronPosition;
				}

				State& operator=(const State& B)
				{
					for(int k=0 ; k < LADO_GRILLA*LADO_GRILLA ; ++k)
						m_Grid[k] = B.m_Grid[k];
					m_TronOurPosition = B.m_TronOurPosition;
					return *this;
				}
				bool operator==(const State& B) const
				{
					if( m_TronOurPosition != B.m_TronOurPosition )
						return false;
					for(int k=0 ; k < LADO_GRILLA*LADO_GRILLA ; ++k)
						if( m_Grid[k] != B.m_Grid[k] )
							return false;
					return true;
				}

				//Memory
				char		 m_Grid[LADO_GRILLA*LADO_GRILLA];
				//Absolute Position
				sf::Vector2i m_TronOurPosition;
			};
			
			//This agent is omnisient so he knows everything that happens in the ambient... basically
			//they have the save information
			State m_StateSensors;

		#pragma endregion

		#pragma region Actions/Operators

			//Moves up
			void _MoveUp();

			//Moves down
			void _MoveDown();

			//Moves to the right
			void _MoveRight();

			//Moves to the left
			void _MoveLeft();

		#pragma endregion

		//Node in the Search tree
		//It have information of the state of the ambient from the tron perspetive inside that state
		//And the last action apply to get to that state
		class Node
		{
			public:
				Node(std::string action, AgentTron::State state): m_Action(action), m_State(state)
					{}
				Node(AgentTron::State state): m_State(state)
					{}
				Node(const Node& B): m_Action(B.m_Action), m_State(B.m_State)
					{}
				Node()
					{}

				bool operator==(const Node& B) const
				{
					if( m_Action.size() != B.m_Action.size() )
						return false;
					for(int i=0; i<m_Action.size() ;++i)
						if( m_Action[i] != B.m_Action[i] )
							return false;

					return (m_State==B.m_State);
				}

				//Identifier of the operation of the node
				std::string		 m_Action;
				//State of the ambient from the agent perspective
				AgentTron::State m_State;
		};

		std::vector<Node> m_Path;
		
		//Search Tree
		Dash_Structures::DTree<Node> m_SearchT;

		//Main search call excecute the search for the action path to follow
		std::vector<Node> _Search(SearchMethod method);

		//Uses Broad search to find the node that satisfice the function _TestSolution
		void _BroadSearch(std::vector<Node>& path);

		void _DeepSearch(std::vector<Node>& path);

		void _LimitedDeepSearch(std::vector<Node>& path);

		//Help class used to expand the tree, the only thing that limit the movement is:
		//																				* The limit of the grid
		//																				* A Wall or the enemy
		//Puts the operators in a predefine order.
		//The posibly adding operators are:
		// 1) MoveUp
		// 2) MoveDown
		// 3) MoveRight
		// 4) MoveLeft
		void _AddLevel(Dash_Structures::DTree<Node>::iterator it);

		//Help class used to expand the tree, the only thing that limit the movement is:
		//																				* The limit of the grid
		//																				* A Wall or the enemy
		//Puts the operators in a random order.
		//The posibly adding operators are:
		// 1) MoveUp
		// 2) MoveDown
		// 3) MoveRight
		// 4) MoveLeft
		void _AddLevelRandomly(Dash_Structures::DTree<Node>::iterator it);

		//Test the node to see if that state satisfice the problem objetive
		bool _TestSolution(const AgentTron::Node& state);

		bool _TestSolutionLimitedSearch(Dash_Structures::DTree<Node>::iterator it);

		//Use Food fill to get an estimation of the grids that we can transverse
		int  _CountMoviblePositions(const State& curState);
		void _FillFlood(sf::Vector2i Position, char Grid[LADO_GRILLA*LADO_GRILLA]);

		//Gives the level of the node pointed by the iterator it
		int _GetLevel(Dash_Structures::DTree<Node>::iterator it);

		void _MainSaveDotFile(std::string FileName, const std::vector<Node>& path);

		void _SaveDotFile(std::ostream& output, Dash_Structures::DTree<Node>::iterator it, int deep, const std::vector<Node>& path);

		//Tron* m_pAvatar;

	public:
		friend class AmbientTron;

		AgentTron(int id);

		void ExportTreeDotFile();

		virtual void ExecuteSensors();

		virtual void ExecuteActions();
};