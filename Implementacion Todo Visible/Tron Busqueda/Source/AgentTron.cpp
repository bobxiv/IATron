#include "AgentTron.h"
#include "AmbientTron.h"

#include "Game.h"

#include "iostream"
#include "fstream"

#include <queue>
#include <stack>

#include <cstdlib>
#include <cmath>

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

//AgentTron::AgentTron(Tron* pAvatar): m_pAvatar(pAvatar)
	//{}
AgentTron::AgentTron(int id): m_id(id), m_LimitSearchNumber(33)
	{
		m_RandomOperatorsOrder = true;
		//m_RandomOperatorsOrder = false;

		/* Ejemplo 1:
		Tamano: 4x4
		Trons: 1
		Objetivo: Que siempre siga el camino de abajo ya que es mas largo que el de arriba.
		Uno es largo 5 y el otro 4. Muestra que ande bien la selecion del mejor camino dentro de lo corto y
		no puede recorrerse toda la grilla.

		a-aA
		a---
		aaa-
		a---
		
		m_StateSensors.m_Grid[0]  = 'b';
		m_StateSensors.m_Grid[1]  = '-';
		m_StateSensors.m_Grid[2]  = 'b';
		m_StateSensors.m_Grid[3]  = 'A';
		m_StateSensors.m_Grid[4]  = 'b';
		m_StateSensors.m_Grid[5]  = '-';
		m_StateSensors.m_Grid[6]  = '-';
		m_StateSensors.m_Grid[7]  = '-';
		m_StateSensors.m_Grid[8]  = 'b';
		m_StateSensors.m_Grid[9]  = 'b';
		m_StateSensors.m_Grid[10] = 'b';
		m_StateSensors.m_Grid[11] = '-';
		m_StateSensors.m_Grid[12] = 'b';
		m_StateSensors.m_Grid[13] = '-';
		m_StateSensors.m_Grid[14] = '-';
		m_StateSensors.m_Grid[15] = '-';
		m_StateSensors.m_TronOurPosition.x = 3;
		m_StateSensors.m_TronOurPosition.y = 0;
		*/
		m_StateSensors.m_Grid[m_StateSensors.m_TronOurPosition.y*LADO_GRILLA+m_StateSensors.m_TronOurPosition.x] = 'A'+m_id;
	}

//-------------------------------------------------------------//
//-----------------		Methods		---------------------------//
//-------------------------------------------------------------//

void AgentTron::_MoveUp()
{
	#if _DEBUG
		std::cout<<"Accion: Mover a Arriba"<< std::endl;
		std::cout<<"----------------------"<< std::endl;
	#endif

	AmbientTron* tronWorld = AmbientTron::GetInstance();

	tronWorld->MoveUp(m_id);
}

void AgentTron::_MoveDown()
{
	#if _DEBUG
		std::cout<<"Accion: Mover a Abajo"<< std::endl;
		std::cout<<"---------------------"<< std::endl;
	#endif

	AmbientTron* tronWorld = AmbientTron::GetInstance();

	tronWorld->MoveDown(m_id);
}

void AgentTron::_MoveLeft()
{
	#if _DEBUG
		std::cout<<"Accion: Mover a Izquierda"<< std::endl;
		std::cout<<"-------------------------"<< std::endl;
	#endif

	AmbientTron* tronWorld = AmbientTron::GetInstance();

	tronWorld->MoveLeft(m_id);
}

void AgentTron::_MoveRight()
{
	#if _DEBUG
		std::cout<<"Accion: Mover a Derecha"<< std::endl;
		std::cout<<"-----------------------"<< std::endl;
	#endif

	AmbientTron* tronWorld = AmbientTron::GetInstance();

	tronWorld->MoveRight(m_id);
}

void AgentTron::ExecuteSensors()
{
	AmbientTron* tronWorld = AmbientTron::GetInstance();

	tronWorld->GiveMePerceptions(*this);
}

std::vector<AgentTron::Node> AgentTron::_Search(SearchMethod method)
{
	m_SearchT.clear();
	std::vector<Node> path;
	m_SearchT.insert( m_SearchT.begin(), Node(m_StateSensors) );
	
	//Search strategy
	switch(method)
	{
	case SearchMethod::BroadSearch:
		_BroadSearch(path);
		break;
	case SearchMethod::DeepSearch:
		_DeepSearch(path);
		break;
	case SearchMethod::LimitedDeepSearch:
		_LimitedDeepSearch(path);
		break;
	}

	return path;
}

void AgentTron::_BroadSearch(std::vector<Node>& path)
{
	/*
	2	create a queue Q
	3      enqueue source onto Q
	4      mark source
	5      while Q is not empty:
	6          dequeue an item from Q into v
	7          for each edge e incident on v in Graph:
	8              let w be the other end of e
	9              if w is not marked:
	10                 mark w(TestSolution & Expand)
	11                 enqueue w onto Q
	*/
	
	std::queue<Dash_Structures::DTree<Node>::iterator> Q;

	if( m_RandomOperatorsOrder )//Expand the tree
		_AddLevelRandomly(m_SearchT.begin());
	else
		_AddLevel(m_SearchT.begin());

	Q.push(m_SearchT.begin());//starts form the root
	if( _TestSolution(*m_SearchT.begin()) )
		return;
	
	while( !Q.empty() )
	{
		Dash_Structures::DTree<Node>::iterator aux = Q.front();
		Q.pop();

		Dash_Structures::DTree<Node>::iterator it = aux.lchild();
		while(it != m_SearchT.end())
		{
			Q.push(it);
			if( _TestSolution(*it) )
			{
				//Desde este nodo solucion hago un transverse hasta el comienzo y asi obtengo todo
				//el recorrido, notar que esto es posible porque el arbol permite ser transverseado
				//de hoja a raiz
				Dash_Structures::DTree<Node>::iterator itAnce = it;
				while( itAnce != m_SearchT.begin() )
				{
					path.insert(path.begin(), *itAnce);
					itAnce = itAnce.onlyFather();
				}
				path.insert(path.begin(), *itAnce);

				return;
			}

			if( m_RandomOperatorsOrder )//Expand the tree
				_AddLevelRandomly(it);
			else
				_AddLevel(it);
			

			it = it.right();
		}
	}
	throw new std::exception("No existe solucion que nos lleve a la prueba de exito!");
}

void AgentTron::_DeepSearch(std::vector<Node>& path)
{
	/*
	2	create a stack Q
	3      stack source onto Q
	4      mark source
	5      while Q is not empty:
	6          pop from stack an item from Q into v
	7          for each edge e incident on v in Graph:
	8              let w be the other end of e
	9              if w is not marked:
	10                 mark w(TestSolution & Expand)
	11                 stack w onto Q
	*/
	
	std::stack<Dash_Structures::DTree<Node>::iterator> Q;

	if( m_RandomOperatorsOrder )//Expand the tree
		_AddLevelRandomly(m_SearchT.begin());
	else
		_AddLevel(m_SearchT.begin());

	Q.push(m_SearchT.begin());//starts form the root
	if( _TestSolution(*m_SearchT.begin()) )
		return;
	
	while( !Q.empty() )
	{
		Dash_Structures::DTree<Node>::iterator aux = Q.top();
		Q.pop();

		Dash_Structures::DTree<Node>::iterator it = aux.lchild();
		while(it != m_SearchT.end())
		{
			Q.push(it);
			if( _TestSolution(*it) )
			{
				//Desde este nodo solucion hago un transverse hasta el comienzo y asi obtengo todo
				//el recorrido, notar que esto es posible porque el arbol permite ser transverseado
				//de hoja a raiz
				Dash_Structures::DTree<Node>::iterator itAnce = it;
				while( itAnce != m_SearchT.begin() )
				{
					path.insert(path.begin(), *itAnce);
					itAnce = itAnce.onlyFather();
				}
				path.insert(path.begin(), *itAnce);

				return;
			}

			if( m_RandomOperatorsOrder )//Expand the tree
				_AddLevelRandomly(it);
			else
				_AddLevel(it);


			it = it.right();
		}
	}
	throw new std::exception("No existe solucion que nos lleve a la prueba de exito!");
}

void AgentTron::_LimitedDeepSearch(std::vector<Node>& path)
{
	/*
	2	create a queue Q
	3      stack source onto Q
	4      mark source
	5      while Q is not empty:
	6          pop from stack an item from Q into v
	7          for each edge e incident on v in Graph:
	8              let w be the other end of e
	9              if w is not marked:
	10                 mark w(TestSolution & Expand)
	11                 stack w onto Q
	*/

	//Heuristica
	//Si las casillas que se pueden "recorrer"(y notar que el fillflood solo da un burdo estimativo)
	//esta muy cercano al limite m_LimitSearchNumber pero esta por debajo luego
	//todos los arboles de nivel m_LimitSearchNumber tienen que ser creados para fallar todos
	//esta condicion intenta solucionar esto(aunque como dijimos el fillflood tiene sus limites)
	int posibleMovs = _CountMoviblePositions(m_SearchT.begin()->m_State);
	int auxOldLimit = m_LimitSearchNumber;
	if( posibleMovs < m_LimitSearchNumber )
		m_LimitSearchNumber = posibleMovs;


	Dash_Structures::DTree<Node>::iterator bestNodeIt;
	int maxLevel = 0;

	
	std::stack<Dash_Structures::DTree<Node>::iterator> Q;

	if( m_RandomOperatorsOrder )//Expand the tree
		_AddLevelRandomly(m_SearchT.begin());
	else
		_AddLevel(m_SearchT.begin());

	Q.push(m_SearchT.begin());//starts form the root
	if( _TestSolutionLimitedSearch(m_SearchT.begin()) )
	{
		m_LimitSearchNumber = auxOldLimit;//Reset el limite
		return;
	}
	
	while( !Q.empty() )
	{
		Dash_Structures::DTree<Node>::iterator aux = Q.top();
		Q.pop();

		Dash_Structures::DTree<Node>::iterator it = aux.lchild();
		while(it != m_SearchT.end())
		{
			//Guardar el mejor camino en caso que no se pueda hacer un camino de longintud
			//m_LimitSearchNumber
			int aux = _GetLevel(it);
			if( aux > maxLevel )
			{
				maxLevel = aux;
				bestNodeIt = it;
			}

			Q.push(it);
			if( _TestSolutionLimitedSearch(it) )
			{
				//Desde este nodo solucion hago un transverse hasta el comienzo y asi obtengo todo
				//el recorrido, notar que esto es posible porque el arbol permite ser transverseado
				//de hoja a raiz
				Dash_Structures::DTree<Node>::iterator itAnce = it;
				while( itAnce != m_SearchT.begin() )
				{
					path.insert(path.begin(), *itAnce);
					itAnce = itAnce.onlyFather();
				}
				path.insert(path.begin(), *itAnce);


				m_LimitSearchNumber = auxOldLimit;//Reset el limite
				return;
			}

			if( m_RandomOperatorsOrder )//Expand the tree
				_AddLevelRandomly(it);
			else
				_AddLevel(it);


			it = it.right();
		}
	}

	m_LimitSearchNumber = auxOldLimit;//Reset el limite

	//Si hay mejor camino me quedo con ese
	if( maxLevel > 0 )
	{
		//Desde este nodo solucion hago un transverse hasta el comienzo y asi obtengo todo
		//el recorrido, notar que esto es posible porque el arbol permite ser transverseado
		//de hoja a raiz
		Dash_Structures::DTree<Node>::iterator itAnce = bestNodeIt;
		while( itAnce != m_SearchT.begin() )
		{
			path.insert(path.begin(), *itAnce);
			itAnce = itAnce.onlyFather();
		}
		path.insert(path.begin(), *itAnce);

		return;
	}else
		throw new std::exception("No existe solucion que nos lleve a la prueba de exito!");
}

void AgentTron::_AddLevel(Dash_Structures::DTree<Node>::iterator it)
{
	State    curState = (*it).m_State;
	sf::Vector2i& Pos = curState.m_TronOurPosition;
	char aux1 = curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x];
	char aux2;

	//MoveUp Operator				isInside(i) ^ isInsideGrid(i+1) -> MoveUp -> ~isInside(i) ^ isInside(i+1)
	if( Pos.y > 0 && curState.m_Grid[(Pos.y-1)*LADO_GRILLA+Pos.x]=='-' )
	{
		curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+m_id;
		sf::Vector2i newPos = Pos;
		newPos.y -= 1;
		aux2 = curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x];
		curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = 'A'+m_id;
		m_SearchT.insert( it.lchild(),Node("Up", State( curState.m_Grid,
													         newPos)) );

		//Reset the state for the other posible operations
		curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = aux2;
		curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = aux1;
	}
	//MoveDown Operator				isInside(i) ^ isInsideGrid(i-1) -> MoveDown -> ~isInside(i) ^ isInside(i-1)
	if( Pos.y < (LADO_GRILLA-1) && curState.m_Grid[(Pos.y+1)*LADO_GRILLA+Pos.x]=='-' )
	{
		curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+m_id;
		sf::Vector2i newPos = Pos;
		newPos.y += 1;
		aux2 = curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x];
		curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = 'A'+m_id;
		m_SearchT.insert( it.lchild(),Node("Down", State( curState.m_Grid,
													           newPos)) );

		//Reset the state for the other posible operations
		curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = aux2;
		curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = aux1;
	}

	//MoveRight Operator			isInside(i) ^ isInsideGrid(i+1) -> MoveRight -> ~isInside(i) ^ isInside(i+1)
	if( Pos.x < (LADO_GRILLA-1) && curState.m_Grid[Pos.y*LADO_GRILLA+(Pos.x+1)]=='-' )
	{
		curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+m_id;
		sf::Vector2i newPos = Pos;
		newPos.x += 1;
		aux2 = curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x];
		curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = 'A'+m_id;
		m_SearchT.insert( it.lchild(),Node("Right", State( curState.m_Grid,
													            newPos)) );

		//Reset the state for the other posible operations
		curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = aux2;
		curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = aux1;
	}
	//MoveLeft Operator				isInside(i) ^ isInsideGrid(i-1) -> MoveLeft -> ~isInside(i) ^ isInside(i-1)
	if( Pos.x > 0 && curState.m_Grid[Pos.y*LADO_GRILLA+(Pos.x-1)]=='-')
	{
		curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+m_id;
		sf::Vector2i newPos = Pos;
		newPos.x -= 1;
		char aux2 = curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x];
		curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = 'A'+m_id;
		m_SearchT.insert( it.lchild(),Node("Left", State( curState.m_Grid,
													           newPos)) );

		//Reset the state for the other posible operations
		curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = aux2;
		curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = aux1;
	}
}

void AgentTron::_AddLevelRandomly(Dash_Structures::DTree<Node>::iterator it)
{
	State    curState = (*it).m_State;
	sf::Vector2i& Pos = curState.m_TronOurPosition;
	char aux1 = curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x];
	char aux2;

	//Disorder
	srand(std::time(NULL));
	int ord[4];
	ord[0] = 0; ord[1] = 1; ord[2] = 2; ord[3] = 3;
	int shuf[4];
	for(int i=0; i < 4 ; ++i)
	{
		int index = -1;
		int tmp   = -1;
		while( tmp == -1 )
		{
			index = rand()%4;
			tmp   = ord[index];
		}
		ord[index] = -1;
		shuf[i] = tmp;
	}
	
	for(int i=0; i < 4 ; ++i)
	{
		switch(shuf[i])
		{
		case 0:
			//MoveUp Operator				isInside(i) ^ isInsideGrid(i+1) -> MoveUp -> ~isInside(i) ^ isInside(i+1)
			if( Pos.y > 0 && curState.m_Grid[(Pos.y-1)*LADO_GRILLA+Pos.x]=='-' )
			{
				curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+m_id;
				sf::Vector2i newPos = Pos;
				newPos.y -= 1;
				aux2 = curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x];
				curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = 'A'+m_id;
				m_SearchT.insert( it.lchild(),Node("Up", State( curState.m_Grid,
																	 newPos)) );

				//Reset the state for the other posible operations
				curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = aux2;
				curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = aux1;
			}
			break;
		case 1:
			//MoveDown Operator				isInside(i) ^ isInsideGrid(i-1) -> MoveDown -> ~isInside(i) ^ isInside(i-1)
			if( Pos.y < (LADO_GRILLA-1) && curState.m_Grid[(Pos.y+1)*LADO_GRILLA+Pos.x]=='-' )
			{
				curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+m_id;
				sf::Vector2i newPos = Pos;
				newPos.y += 1;
				aux2 = curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x];
				curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = 'A'+m_id;
				m_SearchT.insert( it.lchild(),Node("Down", State( curState.m_Grid,
																	   newPos)) );

				//Reset the state for the other posible operations
				curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = aux2;
				curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = aux1;
			}
			break;
		case 2:
			//MoveRight Operator			isInside(i) ^ isInsideGrid(i+1) -> MoveRight -> ~isInside(i) ^ isInside(i+1)
			if( Pos.x < (LADO_GRILLA-1) && curState.m_Grid[Pos.y*LADO_GRILLA+(Pos.x+1)]=='-' )
			{
				curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+m_id;
				sf::Vector2i newPos = Pos;
				newPos.x += 1;
				aux2 = curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x];
				curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = 'A'+m_id;
				m_SearchT.insert( it.lchild(),Node("Right", State( curState.m_Grid,
																		newPos)) );

				//Reset the state for the other posible operations
				curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = aux2;
				curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = aux1;
			}
			break;
		case 3:
			//MoveLeft Operator				isInside(i) ^ isInsideGrid(i-1) -> MoveLeft -> ~isInside(i) ^ isInside(i-1)
			if( Pos.x > 0 && curState.m_Grid[Pos.y*LADO_GRILLA+(Pos.x-1)]=='-')
			{
				curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+m_id;
				sf::Vector2i newPos = Pos;
				newPos.x -= 1;
				char aux2 = curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x];
				curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = 'A'+m_id;
				m_SearchT.insert( it.lchild(),Node("Left", State( curState.m_Grid,
																	   newPos)) );

				//Reset the state for the other posible operations
				curState.m_Grid[newPos.y*LADO_GRILLA+newPos.x] = aux2;
				curState.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = aux1;
			}
			break;
		}
	}
}

bool AgentTron::_TestSolution(const AgentTron::Node& state)
{
	//If all the grids were explored then finish
	for(int k=0; k < LADO_GRILLA*LADO_GRILLA ; ++k)
		if( state.m_State.m_Grid[k] == '-' )
			return false;
	return true;
}

bool AgentTron::_TestSolutionLimitedSearch(Dash_Structures::DTree<Node>::iterator it)
{
	int levels = _GetLevel(it);

	//If the deep of the action is the maximum deep then we succeeded
	if( levels == m_LimitSearchNumber )
		return true;

	return false;
}

int AgentTron::_GetLevel(Dash_Structures::DTree<Node>::iterator it)
{
	int levels = 0;
	while( it != m_SearchT.begin() )
	{
		++levels;
		it = it.onlyFather();
	}
	return levels;
}

int AgentTron::_CountMoviblePositions(const State& curState)
{
	char Grid[LADO_GRILLA*LADO_GRILLA];
	for(int k=0; k < LADO_GRILLA*LADO_GRILLA ; ++k)
		Grid[k] = curState.m_Grid[k];
	Grid[curState.m_TronOurPosition.y*LADO_GRILLA+curState.m_TronOurPosition.x] = '-';

	_FillFlood(curState.m_TronOurPosition, Grid);

	float count = 0;
	for(int k=0; k < LADO_GRILLA*LADO_GRILLA ; ++k)
		if( Grid[k] == '@' )
			++count;
	return (count-1);//El -1 es porque ahi estabamos contanto la posicion actual del tron con un @ y ese
					 //no es un movimiento sino que es nuestra posicion actual
}

void AgentTron::_FillFlood(sf::Vector2i Position, char Grid[LADO_GRILLA*LADO_GRILLA])
{
	if( Grid[Position.y*LADO_GRILLA+Position.x] == '-' )
	{
		Grid[Position.y*LADO_GRILLA+Position.x] = '@';
		if( Position.x < (LADO_GRILLA-1) && Grid[Position.y*LADO_GRILLA+Position.x+1] == '-' )
			_FillFlood(sf::Vector2i(Position.x+1, Position.y), Grid);
		if( Position.x > 0 && Grid[Position.y*LADO_GRILLA+Position.x-1] == '-' )
			_FillFlood(sf::Vector2i(Position.x-1, Position.y), Grid);
		if( Position.y < (LADO_GRILLA-1) && Grid[(Position.y+1)*LADO_GRILLA+Position.x] == '-' )
			_FillFlood(sf::Vector2i(Position.x  , Position.y+1), Grid);
		if( Position.y > 0 && Grid[(Position.y-1)*LADO_GRILLA+Position.x] == '-' )
			_FillFlood(sf::Vector2i(Position.x  , Position.y-1), Grid);
	}
}

void AgentTron::_MainSaveDotFile(std::string FileName, const std::vector<Node>& path)
{
	std::ofstream file(FileName.c_str());

	if( !file.is_open() )
		throw std::exception("No se pudo guardar el archivo dot");

	if( m_SearchT.begin() == m_SearchT.end() )
		throw std::exception("No hay nada en el arbol para exportar");

	file<<"graph graphname"<<std::endl<<'{'<<std::endl;

	_SaveDotFile(file, m_SearchT.begin(), 0, path);

	file<<'}';

	file.close();
}

void AgentTron::_SaveDotFile(std::ostream& output, Dash_Structures::DTree<Node>::iterator it, int deep, const std::vector<Node>& path)
{
	int id = reinterpret_cast<int>(&(*it));
	
	//The name id of a node will be the concatenation of the action with a id

	if( it->m_Action == "" )
		it->m_Action = "root";

	output<< it->m_Action << id <<" [label="+it->m_Action;


	if( deep<path.size() && path[deep].m_State == it->m_State )
		output<<", color=blue]"<< std::endl;
	else
		output<<']'<< std::endl;

	//Draw it node relations
	Dash_Structures::DTree<Node>::iterator Sonit = it.lchild();
	while(Sonit != m_SearchT.end())
	{
		int Sonid = reinterpret_cast<int>(&(*Sonit));

		output<< it->m_Action << id << " -- " << (*Sonit).m_Action << Sonid<< std::endl;

		Sonit = Sonit.right();
	}

	//Expand to the childs
	Sonit = it.lchild();
	while(Sonit != m_SearchT.end())
	{
		_SaveDotFile(output, Sonit, deep+1, path);

		Sonit = Sonit.right();
	}
}

void AgentTron::ExecuteActions()
{
	m_Path.clear();
	try
	{
		//sec = _Search(SearchMethod::BroadSearch);
		m_Path = _Search(SearchMethod::LimitedDeepSearch);
	}catch(std::exception* e)
	{
		MessageBoxA(NULL, e->what(), "Error", MB_OK);
		exit(1);
	}

	if( m_Path.size() == 0 )//Ya estamos en estado de exito... osea no tenemos donde ir
	{						//osea que este tron perdio
		Game* pGame = Game::GetInstance();
		pGame->GameOver(true);
		return;
	}
	

	//_MainSaveDotFile("arbol.dot", sec);

	if(m_Path.size() == 1)//the initial state always is in the path
		return;

	//the first element of sec if the action to apply first
	if(m_Path[1].m_Action == "Up")
		_MoveUp();
	else
		if(m_Path[1].m_Action == "Down")
			_MoveDown();
		else
			if(m_Path[1].m_Action == "Left")
				_MoveLeft();
			else
				if(m_Path[1].m_Action == "Right")
					_MoveRight();
}

void AgentTron::ExportTreeDotFile()
{
	_MainSaveDotFile("arbol.dot", m_Path);
}