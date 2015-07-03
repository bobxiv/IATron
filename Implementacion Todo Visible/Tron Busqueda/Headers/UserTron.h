#pragma once

#include "AmbientTron.h"//haves define LADO_GRILLA

//Inludes de SFML
#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

class UserTron
{
	private:
		sf::Vector2i m_Position;

		//Moves up
		void _MoveUp();

		//Moves down
		void _MoveDown();

		//Moves to the right
		void _MoveRight();

		//Moves to the left
		void _MoveLeft();

		sf::String m_InsertInputText;

	public:
		UserTron();

		void Actualizar();

		void ShowAskForInput(sf::RenderWindow& RW);

};