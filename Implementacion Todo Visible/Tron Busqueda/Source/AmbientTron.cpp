#include "AmbientTron.h"
#include "AgentTron.h"

//Singleton Variables Declaration
AmbientTron* AmbientTron::m_pInstance = NULL;

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

AmbientTron::AmbientTron()
{
	for(int k=0; k < LADO_GRILLA*LADO_GRILLA; ++k)
		m_Grid[k] = '-';

	for(int i=0; i < CANTIDAD_TRON ; ++i)
	{
		m_ArrayTronStartPosition[i].x = rand()%LADO_GRILLA;
		m_ArrayTronStartPosition[i].y = rand()%LADO_GRILLA;
		m_ArrayTronPosition[i] = m_ArrayTronStartPosition[i];
		m_Grid[m_ArrayTronPosition[i].y*LADO_GRILLA+m_ArrayTronPosition[i].x] = 'A'+i;
	}

	//Prueba en grilla de 4x4
	/*
	m_Grid[1*LADO_GRILLA+3] = '*';
	m_Grid[0*LADO_GRILLA+3] = '*';
	m_Grid[0*LADO_GRILLA+2] = '*';
	m_Grid[1*LADO_GRILLA+2] = '*';
	m_Grid[2*LADO_GRILLA+2] = 'A';
	m_Tron1Position.x = 2;
	m_Tron1Position.y = 2;
	*/

	/* Ejemplo 1:
	Tamano: 4x4
	Trons: 1
	Objetivo: Que siempre siga el camino de abajo ya que es mas largo que el de arriba.
	Uno es largo 5 y el otro 4. Muestra que ande bien la selecion del mejor camino dentro de lo corto y
	no puede recorrerse toda la grilla. Nota: esta grilla tambien la debe conocer tron! sino no sirve

	a-aA
	a---
	aaa-
	a---
	
	m_Grid[0]  = 'b';
	m_Grid[1]  = '-';
	m_Grid[2]  = 'b';
	m_Grid[3]  = 'A';
	m_Grid[4]  = 'b';
	m_Grid[5]  = '-';
	m_Grid[6]  = '-';
	m_Grid[7]  = '-';
	m_Grid[8]  = 'b';
	m_Grid[9]  = 'b';
	m_Grid[10] = 'b';
	m_Grid[11] = '-';
	m_Grid[12] = 'b';
	m_Grid[13] = '-';
	m_Grid[14] = '-';
	m_Grid[15] = '-';
	m_ArrayTronPosition[0].x = 3;
	m_ArrayTronPosition[0].y = 0;
	*/
	
	

	/* Ejemplo 2:
	Tamano: 6x6
	Trons: 1
	Limite Profundidad Busqueda: 40
	Objetivo: Probar la condicion que cambia el limite de profundidad a algo un poco mas chico
	si se da que las casillas recorribles esta apenas por debajo de el limite de profundidad. Eso
	hace que se tenga que probar todos los caminos y fallar en todos hasta usar uno. La condicion
	al cambiar el limite lo hace mas rapido.
	*/

	if( USER_TRON )
	{
		m_ArrayTronStartPosition[CANTIDAD_TRON].x = rand()%LADO_GRILLA;
		m_ArrayTronStartPosition[CANTIDAD_TRON].y = rand()%LADO_GRILLA;
		m_ArrayTronPosition[CANTIDAD_TRON] = m_ArrayTronStartPosition[CANTIDAD_TRON];
		m_Grid[m_ArrayTronPosition[CANTIDAD_TRON].y*LADO_GRILLA+m_ArrayTronPosition[CANTIDAD_TRON].x] = 'Z';
	}

	m_ArrayTronColors[0] = sf::Color::Blue;
	m_ArrayTronColors[1] = sf::Color::Red;
	m_ArrayTronColors[2] = sf::Color::Yellow;
	m_ArrayTronColors[3] = sf::Color::Green;
	//...
	m_ArrayTronColors[25] = sf::Color(200,200,200,255);


	for(int i=0; i < CANTIDAD_TRON ; ++i)
		m_apAgents.push_back( new AgentTron(i) );
}

AmbientTron::~AmbientTron()
{
	for(int i=0; i < CANTIDAD_TRON ; ++i)
		delete m_apAgents[i];
	m_apAgents.clear();
}


//-------------------------------------------------------------//
//-----------------		Methods		---------------------------//
//-------------------------------------------------------------//

AmbientTron* AmbientTron::GetInstance()
{
	if( !AmbientTron::m_pInstance )
		AmbientTron::m_pInstance = new AmbientTron();
	return m_pInstance;
}

void AmbientTron::Draw(sf::RenderWindow& RW) const
{
	float width = RW.GetWidth();
	float height = RW.GetHeight();

	float widthGrid  = width/LADO_GRILLA;
	float heightGrid = height/LADO_GRILLA;

	sf::Shape GridWire  = sf::Shape::Rectangle(sf::Vector2<float>(0.0f,0.0f), sf::Vector2<float>(widthGrid,heightGrid), sf::Color(255,255,255,0), 2.0f, sf::Color::Yellow);
	sf::Shape TronMarkGeneric = sf::Shape::Rectangle(sf::Vector2<float>(0.0f,0.0f), sf::Vector2<float>(widthGrid,heightGrid), sf::Color::White);

	float radius = (widthGrid<heightGrid)? widthGrid/2.0f: heightGrid/2.0f;
	sf::Shape TronGeneric = sf::Shape::Circle(sf::Vector2<float>(widthGrid/2.0f,heightGrid/2.0f), radius, sf::Color::White);

	for(int i=0; i < LADO_GRILLA; ++i)
		for(int j=0; j < LADO_GRILLA; ++j)
		{
			//The Retraces
			if( m_Grid[i*LADO_GRILLA+j] >= 'a' && m_Grid[i*LADO_GRILLA+j] <= 'z')
			{				
				TronMarkGeneric.SetPosition(j*widthGrid, i*heightGrid);
				TronMarkGeneric.SetColor( m_ArrayTronColors[m_Grid[i*LADO_GRILLA+j]-'a'] );
				RW.Draw(TronMarkGeneric);
			}

			//The Trons
			if( m_Grid[i*LADO_GRILLA+j] >= 'A' && m_Grid[i*LADO_GRILLA+j] <= 'Z')
			{				
				TronGeneric.SetPosition(j*widthGrid, i*heightGrid);
				TronGeneric.SetColor( m_ArrayTronColors[m_Grid[i*LADO_GRILLA+j]-'A'] );
				RW.Draw(TronGeneric);
			}
		}

	for(int i=0; i < LADO_GRILLA; ++i)
		for(int j=0; j < LADO_GRILLA; ++j)
		{
			GridWire.SetPosition(j*widthGrid, i*heightGrid);
			RW.Draw(GridWire);
		}
}

void AmbientTron::GiveMePerceptions(AgentTron& agent) const
{
	//Sensor de Posicion absoluta
	agent.m_StateSensors.m_TronOurPosition = m_ArrayTronPosition[agent.m_id];

	//Sensor de Grillas circundantes
		
	sf::Vector2i& Pos = agent.m_StateSensors.m_TronOurPosition;
	agent.m_StateSensors.m_Grid[Pos.y*LADO_GRILLA+Pos.x] = m_Grid[Pos.y*LADO_GRILLA+Pos.x];

	if( (Pos.y+1) < LADO_GRILLA )
	{
	agent.m_StateSensors.m_Grid[(Pos.y+1)*LADO_GRILLA+Pos.x] = m_Grid[(Pos.y+1)*LADO_GRILLA+Pos.x];
	if( (Pos.x+1) < LADO_GRILLA )
		agent.m_StateSensors.m_Grid[(Pos.y+1)*LADO_GRILLA+(Pos.x+1)] = m_Grid[(Pos.y+1)*LADO_GRILLA+(Pos.x+1)];
	if( (Pos.x-1) >= 0 )
		agent.m_StateSensors.m_Grid[(Pos.y+1)*LADO_GRILLA+(Pos.x-1)] = m_Grid[(Pos.y+1)*LADO_GRILLA+(Pos.x-1)];
	}

	if( (Pos.y-1) >= 0 )
	{
		agent.m_StateSensors.m_Grid[(Pos.y-1)*LADO_GRILLA+Pos.x] = m_Grid[(Pos.y-1)*LADO_GRILLA+Pos.x];
		if( (Pos.x-1) >= 0 )
			agent.m_StateSensors.m_Grid[(Pos.y-1)*LADO_GRILLA+(Pos.x-1)] = m_Grid[(Pos.y-1)*LADO_GRILLA+(Pos.x-1)];
		if( (Pos.x+1) < LADO_GRILLA )
			agent.m_StateSensors.m_Grid[(Pos.y-1)*LADO_GRILLA+(Pos.x+1)] = m_Grid[(Pos.y-1)*LADO_GRILLA+(Pos.x+1)];
	}

	if( (Pos.x+1) < LADO_GRILLA )
		agent.m_StateSensors.m_Grid[Pos.y*LADO_GRILLA+(Pos.x+1)] = m_Grid[Pos.y*LADO_GRILLA+(Pos.x+1)];
	if( (Pos.x-1) >= 0 )
		agent.m_StateSensors.m_Grid[Pos.y*LADO_GRILLA+(Pos.x-1)] = m_Grid[Pos.y*LADO_GRILLA+(Pos.x-1)];
		
}

void AmbientTron::MoveUp(int id)
{
	sf::Vector2i& Pos = m_ArrayTronPosition[id];
	m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+id;
	m_ArrayTronPosition[id].y -= 1;
	m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'A'+id;
}

void AmbientTron::MoveDown(int id)
{
	sf::Vector2i& Pos = m_ArrayTronPosition[id];
	m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+id;
	m_ArrayTronPosition[id].y += 1;
	m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'A'+id;
}

void AmbientTron::MoveLeft(int id)
{
	sf::Vector2i& Pos = m_ArrayTronPosition[id];
	m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+id;
	m_ArrayTronPosition[id].x -= 1;
	m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'A'+id;
}

void AmbientTron::MoveRight(int id)
{
	sf::Vector2i& Pos = m_ArrayTronPosition[id];
	m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'a'+id;
	m_ArrayTronPosition[id].x += 1;
	m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'A'+id;
}

void AmbientTron::ExportTronDotFile(int id)
{
	dynamic_cast<AgentTron*>(m_apAgents[id])->ExportTreeDotFile();
}