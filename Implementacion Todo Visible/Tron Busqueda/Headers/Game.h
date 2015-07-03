#pragma once

//Agente y ambiente del problema
#include "UserTron.h"
#include "AgentTron.h"
#include "AmbientTron.h"

//Inludes de SFML
#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

//Clase principal del juego, contiene toda la estructura de juego
//Este objeto sera el responsable de:
//	-Crear la escena
//	-Dibujar la pantalla
//	-Actualizar el juego
class Game
{
private:
	
	//Ventana principal
	sf::RenderWindow m_Window;

	sf::Event        m_Evento;

	//Elementos de la escena:
	//-----------------------

	AmbientTron*	 m_pAmbient;

	//AgentTron*       m_ArraypTron[CANTIDAD_TRON];
	UserTron		 m_UserTron;

	sf::String       m_PausaLabel;

	bool             m_Pausa;
	bool			 m_GameOver;
	sf::String       m_GameOverLabel;

	static Game*	 m_pInstance;

	Game();

public:

	static Game* GetInstance();

	~Game();

	bool			 m_UpPress;
	bool			 m_DownPress;
	bool			 m_LeftPress;
	bool			 m_RightPress;

	void GameOver(bool win);

	//Inicializa la escena, se llama luego de creada la instacia de Game
	void Iniciar();

	//Actualiza la escena
	void Actualizar(float dt);

	//Dibuja la escena
	void Dibujar();

};