#include "UserTron.h"
#include "AmbientTron.h"

#include "Game.h"

#include <iostream>

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

UserTron::UserTron(): m_InsertInputText("Ingrese el movimiento de Tron")
	{}

//-------------------------------------------------------------//
//-----------------		Methods		---------------------------//
//-------------------------------------------------------------//

void UserTron::_MoveUp()
{
	#if _DEBUG
		std::cout<<"Usuario: Mover a Arriba"<< std::endl;
		std::cout<<"----------------------"<< std::endl;
	#endif
	
	AmbientTron* tronWorld = AmbientTron::GetInstance();

	sf::Vector2i& Pos = tronWorld->m_ArrayTronPosition[CANTIDAD_TRON];

	tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'z';
	tronWorld->m_ArrayTronPosition[CANTIDAD_TRON].y -= 1;

	if( (Pos.y < 0) || (tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] != '-') )
	{
		Game* pGame = Game::GetInstance();
		pGame->GameOver(false);
		return;
	}

	tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'Z';
}

void UserTron::_MoveDown()
{
	#if _DEBUG
		std::cout<<"Usuario: Mover a Abajo"<< std::endl;
		std::cout<<"---------------------"<< std::endl;
	#endif

	//m_pAvatar->MoveDown();
	AmbientTron* tronWorld = AmbientTron::GetInstance();

	sf::Vector2i& Pos = tronWorld->m_ArrayTronPosition[CANTIDAD_TRON];
	tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'z';
	tronWorld->m_ArrayTronPosition[CANTIDAD_TRON].y += 1;

	if( (Pos.y >= LADO_GRILLA) || (tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] != '-') )
	{
		Game* pGame = Game::GetInstance();
		pGame->GameOver(false);
		return;
	}

	tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'Z';
}

void UserTron::_MoveLeft()
{
	#if _DEBUG
		std::cout<<"Usuario: Mover a Izquierda"<< std::endl;
		std::cout<<"-------------------------"<< std::endl;
	#endif

	//m_pAvatar->MoveLeft();
	AmbientTron* tronWorld = AmbientTron::GetInstance();

	sf::Vector2i& Pos = tronWorld->m_ArrayTronPosition[CANTIDAD_TRON];
	tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'z';
	tronWorld->m_ArrayTronPosition[CANTIDAD_TRON].x -= 1;

	if( (Pos.x < 0) || (tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] != '-') )
	{
		Game* pGame = Game::GetInstance();
		pGame->GameOver(false);
		return;
	}

	tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'Z';
}

void UserTron::_MoveRight()
{
	#if _DEBUG
		std::cout<<"Usuario: Mover a Derecha"<< std::endl;
		std::cout<<"-----------------------"<< std::endl;
	#endif

	//m_pAvatar->MoveRight();
	AmbientTron* tronWorld = AmbientTron::GetInstance();

	sf::Vector2i& Pos = tronWorld->m_ArrayTronPosition[CANTIDAD_TRON];
	tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'z';
	tronWorld->m_ArrayTronPosition[CANTIDAD_TRON].x += 1;

	if( (Pos.x >= LADO_GRILLA) || (tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] != '-') )
	{
		Game* pGame = Game::GetInstance();
		pGame->GameOver(false);
		return;
	}

	tronWorld->m_Grid[Pos.y*LADO_GRILLA+Pos.x] = 'Z';
}

void UserTron::Actualizar()
{
	Game* pGame = Game::GetInstance();
	while( true )
	{
		if( pGame->m_UpPress )
		{
			_MoveUp();
			break;
		}
		if( pGame->m_DownPress )
		{
			_MoveDown();
			break;
		}
		if( pGame->m_LeftPress )
		{
			_MoveLeft();
			break;
		}
		if( pGame->m_RightPress )
		{
			_MoveRight();
			break;
		}
	}
}

void UserTron::ShowAskForInput(sf::RenderWindow& RW)
{
	RW.Draw( m_InsertInputText );
	RW.Display();
}
