#ifndef GAME_H
#define GAME_H
//@Author Sean Nash de Andrade
//@Login C00217019
//Project 1 Shader,Cubes Open GL


#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>


#include <Debug.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Block.h"

using namespace std;
using namespace sf;
using namespace glm;

class Game
{
public:
	Game();
	Game(sf::ContextSettings t_settings);
	~Game();
	void run();

private:
	RenderWindow m_window;
	Font m_font;	// Game font
	Clock m_clock;
	Time m_time;
	Block m_player;
	Block m_target;
	Block m_obstacleOne;
	Block m_obstacleTwo;


	string m_hud;
	sf::SoundBuffer m_fireBuffer;
	sf::Sound m_fireSound;

	bool m_animate = false;
	vec3 m_animation = vec3(0.0f);
	float m_rotation = 0.0f;
	bool m_isRunning = false;
	float m_point{ 0 };
	int m_lives{ 3 };

	void collision();
	void initialize();
	void update();
	void render();
	void unload();
	void drawScreen(mat4 t_mvp, GLint t_ID, const GLfloat t_colour[]);
};

#endif