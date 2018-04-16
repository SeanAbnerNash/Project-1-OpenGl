#include <Game.h>
#include <Cube.h>

#include <string>
#include <sstream>

//@Author Sean Nash de Andrade
//@Login C00217019
//Project 1 Shader,Cubes Open GL

// Helper to convert Number to String for HUD
template <typename T>
string toString(T number)
{
	ostringstream oss;
	oss << number;
	return oss.str();
}

GLuint	
vsid,		// Vertex Shader ID
fsid,		// Fragment Shader ID
progID,		// Program ID
vao = 0,	// Vertex Array ID
vbo,		// Vertex Buffer ID
vib,		// Vertex Index Buffer
to[1]; // Texture ID 1 to 32

GLint	positionID,	// Position ID
		colorID,	// Color ID
		textureID,	// Texture ID
		uvID,		// UV ID
		//mvpID[2],	// Model View Projection ID 
	x_offsetID, // X offset ID
	y_offsetID,	// Y offset ID
	z_offsetID;	// Z offset ID


GLenum	error;		// OpenGL Error Code

const string filename = ".//Assets//Textures//grid_wip.tga";

int width;			// Width of texture
int height;			// Height of texture
int comp_count;		// Component of texture

unsigned char* img_data;		// image data

mat4  projection, view;			// Model View Projection

float x_offset, y_offset, z_offset; // offset on screen (Vertex Shader)

Game::Game() :
	m_window(VideoMode(800, 600), "Introduction to OpenGL Texturing"),
	m_player(0, vec3(0, -5, 0)),//Object Initialing
	m_target(1, vec3(-10, 8, 0)),
	m_obstacleOne(2, vec3(-5, 8, 0)),
	m_obstacleTwo(3, vec3(0, 8, 0))
{

}

Game::Game(sf::ContextSettings settings) : 
	m_window(VideoMode(800, 600), 
	"Introduction to OpenGL Texturing", 
	sf::Style::Default, 
	settings)
	, m_player(0, vec3(0, -5, 0)),
	m_target(1, vec3(-10, 8, 0)),
	m_obstacleOne(2, vec3(0, 8, 0)),
	m_obstacleTwo(3, vec3(10, 8, 0))
{

}

Game::~Game(){}


void Game::run()
{

	initialize();

	Event event;

	float rotation = 0.01f;


	while (m_isRunning){

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (m_window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				m_isRunning = false;
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_player.getFired() == false && m_lives > 0)//Moves the players Block if the player is not fired or out of lives
			{

				m_player.moveBlock(vec3(-0.05, 0, 0));

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_player.getFired() == false && m_lives > 0)//Moves the players Block if the player is not fired or out of lives
			{

				m_player.moveBlock(vec3(0.05, 0, 0));
				
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_player.getFired() == false && m_lives > 0)//Rotates the players Block if the player is not fired or out of lives
			{

				m_player.rotateBlock(vec3(0, 0.05, 0));
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_player.getFired() == false && m_lives > 0)//Rotates the players Block if the player is not fired or out of lives
			{

				m_player.rotateBlock(vec3(0, -0.05, 0));
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_player.getFired() == false && m_lives > 0)//Sets the fire bool to true & Plays a sound if the player is not fired or out of lives
			{

				m_player.setBlockFired(true);
				m_fireSound.play();
			}
		}
		update();
		render();
	}

#if (DEBUG >= 2)
	DEBUG_MSG("Calling Cleanup...");
#endif
	unload();

}



void Game::collision()
{
	if (distance(m_player.getPosition(), m_target.getPosition())< 1.5)//Checks that the distance between the two objects for collision based on distance
	{
		m_player.setBlockFired(false);//Resets the Player Block
		if (m_player.getSide() == m_target.getSide())//Applies the Matching Side effects
		{
			m_point += 100;
			m_lives += 1;
		}
		else//Or Applies the Not Matching Side Penalties
		{
			m_point -= 100;
			m_lives -= 1;
			if (m_point < 0)
			{
				m_point = 0;
			}
		}

	}
	if (distance(m_player.getPosition(), m_obstacleOne.getPosition())< 1.5)//Checks that the distance between the two objects for collision based on distance
	{
		m_player.setBlockFired(false);
		if (m_player.getSide() == m_obstacleOne.getSide())//Applies the Matching Side effects
		{
			m_point += 50;

		}
		else//Or Applies the Not Matching Side Penalties
		{
			m_point -= 50;
			m_lives -= 1;
			if (m_point < 0)
			{
				m_point = 0;
			}
		}
	}
	if (distance(m_player.getPosition(), m_obstacleTwo.getPosition())< 1.5)//Checks that the distance between the two objects for collision based on distance
	{
		m_player.setBlockFired(false);
		if (m_player.getSide() == m_obstacleOne.getSide())//Applies the Matching Side effects
		{
			m_point += 50;

		}
		else//Or Applies the Not Matching Side Penalties
		{
			m_point -= 50;
			m_lives -= 1;
			if (m_point < 0)
			{
				m_point = 0;
			}
		}
	}
	if (m_player.getPosition().y > 15)//If the player shoot to far up resets the block
	{
		m_player.setBlockFired(false);
	}
}

void Game::initialize()
{
	m_isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	glewInit();

	//Copy UV's to all faces
	for (int i = 1; i < 6; i++)
	memcpy(&uvs[i * 4 * 2], &uvs[0], 2 * 4 * sizeof(GLfloat));

	DEBUG_MSG(glGetString(GL_VENDOR));
	DEBUG_MSG(glGetString(GL_RENDERER));
	DEBUG_MSG(glGetString(GL_VERSION));


	glGenBuffers(1, &vbo);		//Gen Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Vertices (3) x,y,z , Colors (4) RGBA, UV/ST (2)
	glBufferData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS) + (2 * UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS) + (2 * UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &vib); //Gen Vertex Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);

	//Indices to be drawn
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);

	const char* vs_src = 
		"#version 400\n\r"
		""
		"in vec3 sv_position;"
		"in vec4 sv_color;"
		"in vec2 sv_uv;"
		""
		"out vec4 color;"
		"out vec2 uv;"
		""
		"uniform mat4 sv_mvp;"
		"uniform float sv_x_offset;"
		"uniform float sv_y_offset;"
		"uniform float sv_z_offset;"
		""
		"void main() {"
		"	color = sv_color;"
		"	uv = sv_uv;"
		"	gl_Position = sv_mvp * vec4(sv_position.x  + sv_x_offset, sv_position.y + sv_y_offset, sv_position.z + sv_z_offset, 1 );"
		"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL);
	glCompileShader(vsid);

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}


	const char* fs_src =
		"#version 400\n\r"
		""
		"uniform sampler2D f_texture;"
		""
		"in vec4 color;"
		"in vec2 uv;"
		""
		"out vec4 fColor;"
		""
		"void main() {"
		"	fColor = color - texture2D(f_texture, uv);"
		""
		"}"; //Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);

	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();
	glAttachShader(progID, vsid);
	glAttachShader(progID, fsid);
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}


	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to[0]);
	glBindTexture(GL_TEXTURE_2D, to[0]);


	//Filtering
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 


	//Bind to OpenGL
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(
		GL_TEXTURE_2D,			//2D Texture Image
		0,						//Mipmapping Level 
		GL_RGBA,				//GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width,					//Width
		height,					//Height
		0,						//Border
		GL_RGBA,				//Bitmap
		GL_UNSIGNED_BYTE,		//Specifies Data type of image data
		img_data				//Image Data
		);

	// Find variables in the shader
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
	uvID = glGetAttribLocation(progID, "sv_uv");
	textureID = glGetUniformLocation(progID, "f_texture");

	// Projection Matrix 
	projection = perspective(
		45.0f,					// Field of View 45 degrees
		4.0f / 3.0f,			// Aspect ratio
		5.0f,					// Display Range Min : 0.1f unit
		100.0f					// Display Range Max : 100.0f unit
		);

	// Camera Matrix
	view = lookAt(
		vec3(0.0f, 0.0f, 20.0f),	// Camera (x,y,z), in World Space
		vec3(0.0f, 0.0f, 0.0f),		// Camera looking at origin
		vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
	);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);


	m_font.loadFromFile("./Assets/Fonts/BBrick.ttf");
	m_fireBuffer.loadFromFile("./Assets/Audio/fireAudio.wav");//Asset loading
	m_fireSound.setBuffer(m_fireBuffer);
}

void Game::update()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Updating...");
#endif
	// Update Model View Projection

	m_target.rotateBlock(vec3(0, 0.001, 0));
	m_obstacleOne.rotateBlock(vec3(0, 0.001, 0));
	m_obstacleTwo.rotateBlock(vec3(0, 0.001, 0));//Rotates the target blocks

	m_player.update(projection, view);
	m_target.update(projection, view);
	m_obstacleOne.update(projection, view);
	m_obstacleTwo.update(projection, view);//Updates the objects

	collision();//Checks for collisions

}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Render Loop...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	// Save current OpenGL render states
	// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7
	m_window.pushGLStates();


	if (m_lives > 0)//If the player still has lives updates the HUD
	{
		 m_hud = "Points -> ["
			+ string(toString(m_point))
			+ "] Side Number -> ["
			+ string(toString(m_player.getSide()))
			+ "] Lives -> ["
			+ string(toString(m_lives))
			+ "]";
	}
	else if (m_lives <= 0)//Otherwise gameover
	{
		 m_hud = "Game Over";
	}

	Text text(m_hud, m_font);

	text.setColor(sf::Color(255, 255, 255, 170));
	text.setPosition(5.0f, 490.f);

	m_window.draw(text);

	

	// Restore OpenGL render states
	// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7

	m_window.popGLStates();

	// Rebind Buffers and then set SubData
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);

	// Use Progam on GPU
	glUseProgram(progID);

	// Find variables within the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	if (positionID < 0) { DEBUG_MSG("positionID not found"); }

	colorID = glGetAttribLocation(progID, "sv_color");
	if (colorID < 0) { DEBUG_MSG("colorID not found"); }

	uvID = glGetAttribLocation(progID, "sv_uv");
	if (uvID < 0) { DEBUG_MSG("uvID not found"); }

	textureID = glGetUniformLocation(progID, "f_texture");
	if (textureID < 0) { DEBUG_MSG("textureID not found"); }

	m_player.setID(glGetUniformLocation(progID, "sv_mvp"));
	m_target.setID(glGetUniformLocation(progID, "sv_mvp"));
	m_obstacleOne.setID(glGetUniformLocation(progID, "sv_mvp"));
	m_obstacleTwo.setID(glGetUniformLocation(progID, "sv_mvp"));//Sets the mvpID


	x_offsetID = glGetUniformLocation(progID, "sv_x_offset");
	if (x_offsetID < 0) { DEBUG_MSG("x_offsetID not found"); }

	y_offsetID = glGetUniformLocation(progID, "sv_y_offset");
	if (y_offsetID < 0) { DEBUG_MSG("y_offsetID not found"); }

	z_offsetID = glGetUniformLocation(progID, "sv_z_offset");
	if (z_offsetID < 0) { DEBUG_MSG("z_offsetID not found"); };

	drawScreen(m_player.getmvp(), m_player.getID(), colors);
	drawScreen(m_target.getmvp(), m_target.getID(), colors);
	drawScreen(m_obstacleOne.getmvp(), m_obstacleOne.getID(), colors);
	drawScreen(m_obstacleTwo.getmvp(), m_obstacleTwo.getID(), colors);

	m_window.display();

	//Disable Arrays
	glDisableVertexAttribArray(positionID);
	glDisableVertexAttribArray(colorID);
	glDisableVertexAttribArray(uvID);

	// Unbind Buffers with 0 (Resets OpenGL States...important step)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Reset the Shader Program to Use
	glUseProgram(0);

	// Check for OpenGL Error code
	error = glGetError();
	if (error != GL_NO_ERROR) {
		//DEBUG_MSG(error);
	}
}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);	//Delete Shader
	glDeleteBuffers(1, &vbo);	//Delete Vertex Buffer
	glDeleteBuffers(1, &vib);	//Delete Vertex Index Buffer
	stbi_image_free(img_data);		//Free image
}

void Game::drawScreen(mat4 t_mvp, GLint t_ID, const GLfloat t_colour[])//Drawing function to make reading the program simpler.
{
	glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * VERTICES * sizeof(GLfloat), 4 * COLORS * sizeof(GLfloat), t_colour);
	glBufferSubData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat), 2 * UVS * sizeof(GLfloat), uvs);

	// Send transformation to shader mvp uniform
	glUniformMatrix4fv(t_ID, 1, GL_FALSE, &t_mvp[0][0]);

	//Set Active Texture .... 32
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);

	// Set the X, Y and Z offset (this allows for multiple cubes via different shaders)
	// Experiment with these values to change screen positions
	glUniform1f(x_offsetID, 0.00f);
	glUniform1f(y_offsetID, 0.00f);
	glUniform1f(z_offsetID, 0.00f);

	//Set pointers for each parameter (with appropriate starting positions)
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (VOID*)(3 * VERTICES * sizeof(GLfloat)));
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (VOID*)(((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat)));

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(uvID);

	//Draw Element Arrays
	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);

}

