#pragma once
//@Author Sean Nash de Andrade
//@Login C00217019
//Project 1 Shader,Cubes Open GL
#include <GL/glew.h>
#include <GL/wglew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Block
{
public:
	Block(int t_rowNum, vec3 t_position);//Initializes with the Position of the Block
	~Block();

	void setPosition(vec3 t_position);//Sets the position of the Block
	vec3 getPosition();
	void moveBlock(vec3 t_moveStep);//Moves the Block

	void setRotation(float t_x, float t_y, float t_z);//Sets the block rotation along the x,y and z axis
	vec3 getRotation();
	void rotateBlock(vec3 t_rotation);
	void setBlockFired(bool t_fired);//Sets the block fired bool, if set to false resets the block back to the players position (only the player can be fired)
	bool getFired();
	mat4 getmvp();

	void setID(GLint t_id);
	GLint &getID();
	void update(mat4 t_projection, mat4 t_view);
	int getSide();

private:

	vec3 m_position;
	float m_xRotation;
	float m_yRotation;
	float m_zRotation;

	int m_rowNumber;//0 = player, 1/2/3 = Targets

	bool m_fired;//If the block is fired or not. If fired it will travel upwards, collide with the targets or hit the top boundary and be reset

	int m_side;//Which side is currently facing the screen for the player to match.

	GLint m_mvpID;
	mat4 m_mvp;
	mat4 m_model;





};

