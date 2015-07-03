#pragma once

#include "Agent.h"

class AgentTron;

//Inludes de SFML
#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"


#define USER_TRON_
//#define USER_TRON 1
//Si es 1 existe el tron controlado por el usuario, si es 0 no existe el tron controlado por el usuario
#define USER_TRON 1
//Lado de la grilla, es decir la grilla es LADO_GRILLA*LADO_GRILLA de casillas
#define LADO_GRILLA 10
//Cantidad de tron controlados por agentes inteligentes de busqueda
#define CANTIDAD_TRON 1

//La grilla es en 2D y tiene LADO_GRILLA*LADO_GRILLA de casillas.
//Es cuadrada.
class AmbientTron: public Ambient
{
	private:

		//Posiciones en las que comienzan los trons
		//Se guardan por las dudas se las llegase a necesitar
		sf::Vector2i m_ArrayTronStartPosition[CANTIDAD_TRON+USER_TRON];

		//Posiciones en las que estan los trons
		//Si hay tron de usuario entonces va al final
		sf::Vector2i m_ArrayTronPosition[CANTIDAD_TRON+USER_TRON];

		//Matriz del ambiente
		//Leyenda:
		// - Free
		// * Tron 1 Estela
		// % Tron 2 Estela
		// Tron i Posicion rango:[A-Y] : entonces Tron 1 -> A
		//								 entonces Tron 2 -> B
		// La Z esta reservada para el UserTron y la z para su estela
		//Corre por filas la matriz(osea de esa forma esta linealizada)
		char		 m_Grid[LADO_GRILLA*LADO_GRILLA];

		//Colores que le corresponden a cada agente tron
		//EL ultimo color es el del usuario si es que este esta
		sf::Color	 m_ArrayTronColors[26];

		static AmbientTron* m_pInstance;

		AmbientTron();

	public:
		//Este es el usuario y no un agente, por eso no hace falta limitar su accesibilidad... no mas que
		//la razon de ser respetuoso del paragidma de orientacion a objetos
		friend class UserTron;

		//Recibe un agente Tron y lo llena con las percepciones que le corresponden
		void GiveMePerceptions(AgentTron& agent) const;

		//Recibe el id del agente y realiza la accion mover para arriba en el ambiente
		void MoveUp(int id);

		//Recibe el id del agente y realiza la accion mover para abajo en el ambiente
		void MoveDown(int id);

		//Recibe el id del agente y realiza la accion mover para la izquierda en el ambiente
		void MoveLeft(int id);

		//Recibe el id del agente y realiza la accion mover para la derecha en el ambiente
		void MoveRight(int id);

		//Exporta el archivo dot del agente con el id que se ingresa para luego graficarse con
		//Graphviz
		void ExportTronDotFile(int id);

		//Esta clase es snigleton
		static AmbientTron* GetInstance();

		virtual ~AmbientTron();

		//Dibuja el ambiente
		void Draw(sf::RenderWindow& RW) const;
};