#include <cstdlib>
#include <iostream>

#include "Game.h"

//Singleton Variables Declaration
Game* Game::m_pInstance = NULL;

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Game::Game(): m_Window(sf::VideoMode(800,600,32), "Agente Tron Busqueda"), m_Pausa(false), m_PausaLabel("Pausa"), m_GameOver(false)
{
	m_Window.Show(true);
	m_Window.SetFramerateLimit(60);

	m_PausaLabel.SetPosition( m_Window.GetWidth()/2.0f - m_PausaLabel.GetRect().GetWidth()/2.0f, m_Window.GetHeight()/2.0f + m_PausaLabel.GetRect().GetHeight()/2.0f);
	m_PausaLabel.SetFont( sf::Font::GetDefaultFont() );

	m_GameOverLabel.SetFont( sf::Font::GetDefaultFont() );

	m_pAmbient = AmbientTron::GetInstance();

	//for(int i=0; i < CANTIDAD_TRON ; ++i)
		//m_ArraypTron[i] = new AgentTron(i);
}

Game::~Game()
{
	delete m_pAmbient;
	//for(int i=0; i < CANTIDAD_TRON ; ++i)
		//delete m_ArraypTron[i];
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

Game* Game::GetInstance()
{
	if( !Game::m_pInstance )
		Game::m_pInstance = new Game();
	return m_pInstance;
}

void Game::Iniciar()
{
	const sf::Input& input = m_Window.GetInput();
	
	float dt;
	while( m_Window.IsOpened() )
	{
		//Reset the state
		m_UpPress =  m_DownPress = m_LeftPress = m_RightPress = false;

		//Atrapamos los eventos para cerra la ventana
		while( m_Window.GetEvent(m_Evento) )
		{
			switch( m_Evento.Type )
			{
			case sf::Event::Closed:
					m_Window.Close();
				break;
				
			case sf::Event::KeyPressed:
				if( m_Evento.Key.Code == sf::Key::Escape)
					m_Window.Close();
				break;

			case sf::Event::KeyReleased:
				if( !USER_TRON )
				{
					if( m_Evento.Key.Code == sf::Key::N)//Avanzar simulacion
					{
						dt = m_Window.GetFrameTime();
						Actualizar(dt);  //Actualizamos la escena
					}
				}
				if( m_Evento.Key.Code == sf::Key::P)//Print tree
				{
					try
					{
						m_pAmbient->ExportTronDotFile(0);
						//m_ArraypTron[0]->ExportTreeDotFile();
						system("dot arbol.dot -Tpng -O");
						system("arbol.dot.png");
					}catch(std::exception &e)
					{
						MessageBoxA(NULL, e.what(), "Error", MB_OK);
					}
					#ifdef _DEBUG
						if( m_Pausa == true )
							std::cout<<"Exportando el arbol del ultimo movimiento"<<std::endl;
						else
							std::cout<<"-------------------------------"<< std::endl;
					#endif
				}
				else
				if( m_Evento.Key.Code == sf::Key::Space)//Pausar/Despausar
				{
					m_Pausa = !m_Pausa;
					#ifdef _DEBUG
						if( m_Pausa == true )
							std::cout<<"PAUSA..."<<std::endl;
						else
							std::cout<<"-------------------------------"<< std::endl;
					#endif
				}else
				if( m_Evento.Key.Code == sf::Key::Up )
				m_UpPress = true;
				else
				if( m_Evento.Key.Code == sf::Key::Down )
					m_DownPress = true;
				else
				if( m_Evento.Key.Code == sf::Key::Left )
					m_LeftPress = true;
				else
				if( m_Evento.Key.Code == sf::Key::Right	)
					m_RightPress = true;
				break;
			}
		}
		dt = m_Window.GetFrameTime();
		
		if( USER_TRON )
			Actualizar(dt);  //Actualizamos la escena

		Dibujar();       //Dibujamos todo el juego
	}
}

void Game::Actualizar(float dt)
{
	#ifdef USER_TRON_
	if( !m_Pausa && !m_GameOver)
	{
		if(  m_UpPress || m_DownPress || m_LeftPress || m_RightPress )
		{
			if( USER_TRON )
				m_UserTron.Actualizar();
			if( m_GameOver )
				return;

			m_pAmbient->Update();
			//for(int i=0; i < CANTIDAD_TRON ; ++i)
			//{
				//m_ArraypTron[i]->ExecuteSensors();
				//m_ArraypTron[i]->ExecuteActions();
			//}

			//m_pAmbient->Update();
		}
	}
	#else
	if( !m_Pausa )
	{

		m_pAmbient->Update();
		//for(int i=0; i < CANTIDAD_TRON ; ++i)
		//{
			//m_ArraypTron[i]->ExecuteSensors();
			//m_ArraypTron[i]->ExecuteActions();
		//}
	}
	#endif

}

void Game::Dibujar()
{
	m_Window.Clear();

	AmbientTron* ambient = AmbientTron::GetInstance();
	ambient->Draw(m_Window);

	if( m_Pausa )
		m_Window.Draw( m_PausaLabel );
	if( m_GameOver )
		m_Window.Draw( m_GameOverLabel );
	else
		if( USER_TRON )
			m_UserTron.ShowAskForInput(m_Window);
		
	m_Window.Display();
}

void Game::GameOver(bool win)
{
	if( win )
		m_GameOverLabel.SetText( "Game Over: Ganaste" );
	else
		m_GameOverLabel.SetText( "Game Over: Perdiste" );
	m_GameOverLabel.SetPosition( m_Window.GetWidth()/2.0f - m_GameOverLabel.GetRect().GetWidth()/2.0f, m_Window.GetHeight()/2.0f + m_GameOverLabel.GetRect().GetHeight()/2.0f);
	m_GameOver = true;
}