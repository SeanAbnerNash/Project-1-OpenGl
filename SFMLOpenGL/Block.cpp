#include "Block.h"

//@Author Sean Nash de Andrade
//@Login C00217019
//Project 1 Shader,Cubes Open GL

Block::Block(int t_rowNum, vec3 t_position)
{
	m_rowNumber = t_rowNum;
	m_position = t_position;
	m_side = 1;
	m_xRotation = 0;
	m_yRotation = 0;
	m_zRotation = 0;

	m_fired = false;
}


Block::~Block()
{
}

void Block::setPosition(vec3 t_position)
{
	m_position = t_position;
}

vec3 Block::getPosition()
{
	return m_position;
}

void Block::moveBlock(vec3 t_moveStep)
{
	m_position += t_moveStep;
}

void Block::setRotation(float t_x, float t_y, float t_z)
{
	m_xRotation = t_x;
	m_yRotation = t_y;
	m_zRotation = t_z;
	if (m_xRotation > 360)
	{
		m_xRotation -= 360;
	}
	if (m_yRotation > 360)
	{
		m_yRotation -= 360;
	}
	if (m_zRotation > 360)
	{
		m_zRotation -= 360;
	}

}

vec3 Block::getRotation()
{
	return vec3(m_xRotation, m_yRotation, m_zRotation);
}

void Block::rotateBlock(vec3 t_rotation)
{
	m_xRotation += t_rotation.x;
	m_yRotation += t_rotation.y;
	m_zRotation += t_rotation.z;
	if (m_yRotation > 6.5)
	{
		m_yRotation = 0;
	}
	if (m_yRotation < 0)
	{
		m_yRotation = 6.5;
	}

}

void Block::setBlockFired(bool t_fired)
{
	m_fired = t_fired;
	if (t_fired == false)
	{
		setPosition(vec3(0, -5, 0));
	}
}

bool Block::getFired()
{
	return m_fired;
}

mat4 Block::getmvp()
{
	return m_mvp;
}

void Block::setID(GLint t_ID)
{
	m_mvpID = t_ID;
}

GLint & Block::getID()
{
	return m_mvpID;
}

void Block::update(mat4 t_projection, mat4 t_view)
{

	if (m_yRotation < 0.8125 && m_yRotation > 0 || m_yRotation > 5.6875)
	{
		m_side = 1;
	}
	if (m_yRotation < 2.4375 && m_yRotation > 0.8125)
	{
		m_side = 2;
	}
	if (m_yRotation < 4.0625 && m_yRotation > 2.4375)
	{
		m_side = 3;
	}
	if (m_yRotation < 5.6875 && m_yRotation > 4.0625)
	{
		m_side = 4;
	}
	if (m_fired)
	{
		moveBlock(vec3(0, 0.025, 0));
	}
		
	m_model = mat4(1.0f);

	m_model = translate(m_model, m_position);
	m_model = glm::rotate(m_model, m_xRotation, glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, m_yRotation, glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, m_zRotation, glm::vec3(0, 0, 1));

	m_mvp = t_projection * t_view * m_model;
}

int Block::getSide()
{
	return m_side;
}
