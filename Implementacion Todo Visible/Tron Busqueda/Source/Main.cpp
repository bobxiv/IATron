#include <cstdlib>
#include <ctime>

#include "Game.h"

using namespace std;

int main(int argc, char* argv[])
{
	int io = 'Z'-'A';
	Game* pMyGame = Game::GetInstance();//El objeto de juego principal

	srand(time(NULL));//ponemos una semilla pseudo-aleatoria

	pMyGame->Iniciar();

	return 0;
}

