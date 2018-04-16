#include <Game.h>
//@Author Sean Nash de Andrade
//@Login C00217019
//Project 1 Shader,Cubes Open GL

int main(void)
{
	sf::ContextSettings settings;
	settings.depthBits = 24u;
	settings.antialiasingLevel = 4u;
	Game& game = Game(settings);
	game.run();
}