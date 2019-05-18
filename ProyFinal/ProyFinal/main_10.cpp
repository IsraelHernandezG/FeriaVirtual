/*--------------------------------------------------------*/
/* -------------   Proyecto Final ------------------------*/
/*-----------------    2019-2   --------------------------*/
/*- Computación gráfica e interacción humano computadora -*/
/*-------------------- Grupo 04 --------------------------*/
/*-------------------- Alumnos: --------------------------*/
/*----------- Esparza Vázquez Luis Mauricio --------------*/
/*------------- Hernández García Israel ------------------*/


//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>
//bibliotecas para el manejo de archivos
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
//
#include "camera.h"
#include "cilindro.h"
#include "esfera.h"
#include "toroide.h"
#include "Model.h"
// Other Libs
#include "SOIL2/SOIL2.h"

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//primitivas
Esfera my_esfera(1.0);
Cilindro my_cilindro(1.0);
Toroide my_toroide(1.0);

//escalas
double sx_cil = 1.0f,
	sy_cil = 1.0f,
	sz_cil = 1.0f;

//variable global montaña rusa
glm::mat4 modelMR = glm::mat4(1.0f);
glm::mat4 modelEarth = glm::mat4(1.0f);
glm::mat4 modelVagon = glm::mat4(1.0f);
glm::mat4 modelHorse = glm::mat4(1.0f);
glm::mat4 modelDomo = glm::mat4(1.0f);
glm::mat4 modelLuces = glm::mat4(1.0f);

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;
bool cameraMode = false;
double xoffset1 = 0.01;
double yoffset1 = 0.01;
float mov_horizontal = 0.0f,
mov_vertical = 0.0f;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPos(0.0f, 10.0f, 10.0f);
glm::vec3 lightPosition(0.0f, 3.0f, 0.0f);
glm::vec3 lightDirection(0.0f, 0.0f, -3.0f);


void myData(void);
void display(Shader, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
void editarArchivo(string, string);
string leerArchivo(string,long);
void creaArchivo(string);
unsigned int generateTextures(char*, bool, string);
void resetElements();
void objetivosDisparo();

//for mobile models
float x1 = 0.0f,
x2 = 0.0f,
x3 = 0.0f,
x4 = 0.0f,
x5 = 0.0f;


//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f;
bool play = false,
	modelSwitch = false,
	horseOn = false,
	game = false;

//For model
float movKit_z = 0.0f,
	girollanta = 0.0f,
	direccion = 1.0f,
	rotacion = 0.0f,
	tamanioPista = 20.0f;

//for sol/lun
float giroSol = 0.0f,
colorR = 0.251f,
colorGB = 0.898f;
float bullet = 0.0f;

//textures
unsigned int	t_smile,
				t_toalla,
				t_unam,
				t_white,
				t_ladrillo,
				t_caja,
				t_caja_brillo,
				t_domo,
				t_red,
				t_bush,
				t_bush2;

//For carrusel
float	animTubos = 0.0f,
		animGiro = 0.0f,
		movModelos = 0.0f;

//Keyframes
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotX = 0.0f,
rotY = 0.0f,
rotZ = 0.0f;
bool bandera1 = false;

long contador= 0L;

#define MAX_FRAMES 250
int i_max_steps = 60; //i_max_steps = 190
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotX;
	float rotY;
	float rotZ;
	float incRotX;
	float incRotY;
	float incRotZ;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
int playIndex = 0;

void saveFrame(void)
{
	string name = "animation.txt";
	std::string s = "";
	std::ostringstream strsX,strsY,strsZ,strsRX, strsRY, strsRZ, strs;

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].posX = posX;
	strsX << posX;
	s = strsX.str();
	editarArchivo(name,s);
	KeyFrame[FrameIndex].posY = posY;
	strsY << posY;
	s = strsY.str();
	editarArchivo(name,s);
	KeyFrame[FrameIndex].posZ = posZ;
	strsZ << posZ;
	s = strsZ.str();
	editarArchivo(name,s);

	KeyFrame[FrameIndex].rotX = rotX;
	strsRX << rotX;
	s = strsRX.str();
	editarArchivo(name,s);
	KeyFrame[FrameIndex].rotY = rotY;
	strsRY << rotY;
	s = strsRY.str();
	editarArchivo(name,s);
	KeyFrame[FrameIndex].rotZ = rotZ;
	strsRZ << rotZ;
	s = strsRZ.str();
	editarArchivo(name,s);

	FrameIndex++;
	strs << FrameIndex;
	s = strs.str();
	creaArchivo("frames.txt");
	editarArchivo("frames.txt", s);
}

void cargaFrames(void) {
	//KeyFrame[MAX_FRAMES];
	FrameIndex = 0;			
	playIndex = 0;

	

	string name = "animation.txt";
	string salida = leerArchivo("frames.txt", 0L);
	std::string::size_type sz;   // alias of size_t
	long numFrames = std::stol(salida, &sz);

	long i;
	for (i = 0; i <= numFrames-1; i++) {

		salida = leerArchivo(name, 6*i);
		double l_posX = std::stod(salida, &sz);
		KeyFrame[FrameIndex].posX = l_posX;

		salida = leerArchivo(name, 6*i+1);
		double l_posY = std::stod(salida, &sz);
		KeyFrame[FrameIndex].posY = l_posY;

		salida = leerArchivo(name, 6 * i + 2);
		double l_posZ = std::stod(salida, &sz);
		KeyFrame[FrameIndex].posZ = l_posZ;

		salida = leerArchivo(name, 6 * i + 3);
		double l_rotX = std::stod(salida, &sz);
		KeyFrame[FrameIndex].rotX = l_rotX;

		salida = leerArchivo(name, 6 * i + 4);
		double l_rotY = std::stod(salida, &sz);
		KeyFrame[FrameIndex].rotY = l_rotY;

		salida = leerArchivo(name, 6 * i + 5);
		double l_rotZ = std::stod(salida, &sz);
		KeyFrame[FrameIndex].rotZ = l_rotZ;

		FrameIndex++;
	}

}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotX = KeyFrame[0].rotX;
	rotY = KeyFrame[0].rotY;
	rotZ = KeyFrame[0].rotZ;
}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].incRotX = (KeyFrame[playIndex + 1].rotX - KeyFrame[playIndex].rotX) / i_max_steps;
	KeyFrame[playIndex].incRotY = (KeyFrame[playIndex + 1].rotY - KeyFrame[playIndex].rotY) / i_max_steps;
	KeyFrame[playIndex].incRotZ = (KeyFrame[playIndex + 1].rotZ - KeyFrame[playIndex].rotZ) / i_max_steps;

}

unsigned int generateTextures(const char* filename, bool alfa, string name)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture " + name << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{

	t_smile = generateTextures("Texturas/awesomeface.png", 1, "smile");
	t_toalla = generateTextures("Texturas/toalla.tga", 0, "toalla");
	t_unam = generateTextures("Texturas/escudo_unam.png", 1, "unam");
	t_domo = generateTextures("Texturas/domo.jpg", 0, "domo");
	t_caja = generateTextures("Texturas/caja.png", 1, "caja");
	t_caja_brillo = generateTextures("Texturas/caja_specular.png", 1, "caja especular");
	t_ladrillo = generateTextures("Texturas/bricks.jpg", 0, "ladrillos");
	t_red =	 generateTextures("Texturas/red.jpg", 0, "rojo");
	t_white = generateTextures("Texturas/white.jpg", 0, "blanco");
	t_bush = generateTextures("Texturas/bush.jpg", 0, "bush");
	t_bush2 = generateTextures("Texturas/bush2.jpg", 0, "bush2");
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_smile);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, t_toalla);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, t_unam);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, t_domo);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, t_caja);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, t_caja_brillo);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, t_ladrillo);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, t_red);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, t_white);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, t_bush);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, t_bush2);
}

void myData()
{	
	float vertices[] = {
		// positions          // normals           // texture coords
		//trasera
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,//0
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  1.0f,  0.0f,//1
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,//2
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  0.0f,  1.0f,//3
		//frontal
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,//4
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,//5
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,//6
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,//7
		//izquierda
		-0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,//11
		//derecha			 
		 0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,//15
		 //inferior
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		//superior
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,//23


		//////Segunda caja

		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,//27

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,//39

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,//47




		 // positions          // normals           // texture coords
		//////////////Carrusel (Domo)
	 
		0.0f, 1.5f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//0
		-0.48f, 0.0f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.0f, 0.0f,//1
		-0.41f, 0.0f, -0.24f,		0.0f,  0.0f, 1.0f,	 1.0f, 1.0f,//2
		-0.23f, 0.0f, -0.42f,		0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//3

		0.0f, 1.5f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//4
		-0.23f, 0.0f, -0.42f,		0.0f,  0.0f, 1.0f,	 0.0f, 0.0f,//5
		0.0f, 0.0f, -0.48f,			0.0f,  0.0f, 1.0f,	 1.0f, 1.0f,//6
		0.25f, 0.0f, -0.42f,		0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//7

		0.0f, 1.5f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//8
		0.25f, 0.0f, -0.42f,		0.0f,  0.0f, 1.0f,	 0.0f, 0.0f,//9
		0.43f, 0.0f, -0.24f,		0.0f,  0.0f, 1.0f,	 1.0f, 1.0f,//10
		0.49f, 0.0f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//11

		0.0f, 1.5f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//12
		0.49f, 0.0f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.0f, 0.0f,//13
		0.43f, 0.0f, 0.24f,			0.0f,  0.0f, 1.0f,	 1.0f, 1.0f,//14
		0.25f, 0.0f, 0.42f,			0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//15
												 
		0.0f, 1.5f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//16
		0.25f, 0.0f, 0.42f,			0.0f,  0.0f, 1.0f,	 0.0f, 0.0f,//17
		0.0f, 0.0f, 0.48f,			0.0f,  0.0f, 1.0f,	 1.0f, 1.0f,//18
		-0.23f, 0.0f, 0.42f,		0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//19

		0.0f, 1.5f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//20
		-0.23f, 0.0f, 0.42f,		0.0f,  0.0f, 1.0f,	 0.0f, 0.0f,//21
		-0.41f, 0.0f, 0.24f,		0.0f,  0.0f, 1.0f,	 1.0f, 1.0f,//22
		-0.48f, 0.0f, 0.0f,			0.0f,  0.0f, 1.0f,	 0.5f, 0.5f,//23

		//superior#2
		-0.5f, 0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 30.0f,//72
		0.5f, 0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	30.0f, 30.0f,//73
		0.5f, 0.5f, 0.5f,	0.0f, 0.0f, 1.0f,	30.0f, 0.0f,//74
		-0.5f, 0.5f, 0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,//75

	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void animate(void)
{
	if (x1 <= 4.0f) {
		x1 += 0.001f;
	}
	if (x1 > 4.0f) {
		x1 = 0.0f;
	}
	
	if (x2 <= 3.0f) {
		x2 += 0.001f;
	}
	if (x2 > 3.0f) {
		x2 = -1.0f;
	}

	if (x3 <= 2.0f) {
		x3 += 0.001f;
	}
	if (x3 > 2.0f) {
		x3 = -2.0f;
	}
	if (x4 <= 1.0f) {
		x4 += 0.001f;
	}
	if (x4 > 1.0f) {
		x4 = -3.0f;
	}

	//rotacion += 0.05f;
	//giroSol += 0.5f;
	//atardecer
	if (giroSol >= 80.0f && giroSol < 110.0f) {
		colorR -= 0.004f;
		colorGB -= 0.012f;
	}
	if (giroSol >= 260.0f && giroSol < 290.0f) {
		colorR += 0.004f;
		colorGB += 0.012f;
	}
	if (giroSol >= 360.0f ) {
		giroSol = 0.0f;
	}

	movKit_z += 0.01f * direccion;
	if (movKit_z >= tamanioPista) {
		direccion = -1;
	}
	if (movKit_z <= -tamanioPista) {
		direccion = 1;
	}
	
	//Animacion por key frames
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotX += KeyFrame[playIndex].incRotX;
			rotY += KeyFrame[playIndex].incRotY;
			rotZ += KeyFrame[playIndex].incRotZ;
			

			i_curr_steps++;
		}

	}

}

void displayLights(){

	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");

	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	lampShader.use();
	lampShader.setMat4("projection", projection);
	lampShader.setMat4("view", view);

	float distancia = 3.6f;
	float diametro = 0.1f;

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, distancia, 0.0f));
	model = glm::scale(model, glm::vec3(diametro));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, -distancia, 0.0f));
	model = glm::scale(model, glm::vec3(diametro));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, distancia));
	model = glm::scale(model, glm::vec3(diametro));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -distancia));
	model = glm::scale(model, glm::vec3(diametro));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, distancia*.7071f, distancia*.7071f));
	model = glm::scale(model, glm::vec3(diametro));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, -distancia *.7071f, -distancia *.7071f));
	model = glm::scale(model, glm::vec3(diametro));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, distancia*.7071f, -distancia *.7071f));
	model = glm::scale(model, glm::vec3(diametro));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, -distancia *.7071f, distancia*.7071f));
	model = glm::scale(model, glm::vec3(diametro));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	my_esfera.render();
}

void displayLightsOff() {

	Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");

	projectionShader.use();
	projectionShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("lightPos", lightPos);

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	float distancia = 3.6f;
	float diametro = 0.1f;

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, distancia, 0.0f));
	model = glm::scale(model, glm::vec3(diametro));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, -distancia, 0.0f));
	model = glm::scale(model, glm::vec3(diametro));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, distancia));
	model = glm::scale(model, glm::vec3(diametro));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -distancia));
	model = glm::scale(model, glm::vec3(diametro));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, distancia*.7071f, distancia*.7071f));
	model = glm::scale(model, glm::vec3(diametro));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, -distancia * .7071f, -distancia * .7071f));
	model = glm::scale(model, glm::vec3(diametro));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, distancia*.7071f, -distancia * .7071f));
	model = glm::scale(model, glm::vec3(diametro));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	my_esfera.render();

	model = modelLuces;

	model = glm::translate(model, glm::vec3(0.0f, -distancia * .7071f, distancia*.7071f));
	model = glm::scale(model, glm::vec3(diametro));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	my_esfera.render();
}

void diplayElemCielo() {

	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");

	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	//Sol
	lampShader.use();
	lampShader.setMat4("projection", projection);
	lampShader.setMat4("view", view);

	model = glm::mat4(1.0f);

	model = glm::rotate(model, glm::radians(giroSol), glm::vec3(0.0f, 0.0f, 1.0f)); //traslacion
	model = glm::translate(model, glm::vec3(0.0f, 99.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.75f));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	my_esfera.render();

	model = glm::mat4(1.0f);
	//Luna
	model = glm::rotate(model, glm::radians(giroSol), glm::vec3(0.0f, 0.0f, 1.0f)); //traslacion
	model = glm::translate(model, glm::vec3(0.0f, -99.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.75f));
	lampShader.setMat4("model", model);
	lampShader.setVec3("aColor", glm::vec3(0.8f, 0.8f, 0.8f));
	my_esfera.render();

}

void displayPuestoTiro() {
	Shader lightingShader("shaders/shader_texture_light_dir.vs", "shaders/shader_texture_light_dir.fs"); //Directional

	lightingShader.use();
	lightingShader.setVec3("light.direction", lightDirection);
	lightingShader.setVec3("viewPos", camera.Position);

	// light properties
	lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//For Positional and Spotlight
	lightingShader.setFloat("light.constant", 1.0f);
	lightingShader.setFloat("light.linear", 0.09f);
	lightingShader.setFloat("light.quadratic", 0.032f);

	// material properties
	lightingShader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	//lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setMat4("model", model);
	lightingShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	lightingShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	lightingShader.setVec3("ambientColor", 0.0f, 0.0f, 0.0f);
	lightingShader.setVec3("diffuseColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("specularColor", 1.0f, 0.0f, 0.0f);
	lightingShader.setInt("material_diffuse", t_caja);

	//cara cubo
	//model = modelVagon;
	//model = glm::translate(model, glm::vec3(posX, posY, posZ));

	model = glm::translate(model, glm::vec3(10.0f, -4.35f, -15.0f));
	temp = model = glm::scale(model, glm::vec3(5.0f, 3.5f, 4.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 4);
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //traslacion
	//lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 8, 4);
	glDrawArrays(GL_QUADS, 12, 4);
	glDrawArrays(GL_QUADS, 20, 4);

	model = temp;
	model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 1.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 4, 4);

	model = temp;
	model = glm::translate(model, glm::vec3(-0.4f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 1.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 4, 4);

	model = temp;
	model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 0.2f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 4, 4);

	//segunda pared
	model = temp;
	model = glm::translate(model, glm::vec3(0.0f, -0.35f, 0.05f));
	model = glm::scale(model, glm::vec3(1.0f, 0.3f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 4);

	model = temp;
	model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.05f));
	model = glm::scale(model, glm::vec3(1.0f, 0.2f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 4);

	model = temp;
	model = glm::translate(model, glm::vec3(-0.4f, 0.05f, 0.05f));
	model = glm::scale(model, glm::vec3(0.2f, 0.5f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 4);

	model = temp;
	model = glm::translate(model, glm::vec3(0.4f, 0.05f, 0.05f));
	model = glm::scale(model, glm::vec3(0.2f, 0.51f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 4);

	//mesa
	lightingShader.setInt("material_diffuse", t_white);

	model = temp;
	model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.35f));
	temp = model = glm::scale(model, glm::vec3(0.8f, 0.2f, 0.2f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//dibujar rifles

	objetivosDisparo();

}

void objetivosDisparo() {
	Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs"); //PARA PRIMITIVAS

	projectionShader.use();
	projectionShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("lightPos", lightPos);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);

	tmp = model = glm::translate(model, glm::vec3(10.0f, -5.0f, -16.85f));
	model = glm::translate(model, glm::vec3(x3, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.12f, 0.03f, 0.03));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 0.1f, 0.1f, 0.1f);
	projectionShader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();

	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
	projectionShader.setMat4("model", model);
	my_esfera.render();

	model = tmp;
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(x4, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.12f, 0.03f, 0.03));
	projectionShader.setMat4("model", model);
	my_cilindro.render();

	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
	projectionShader.setMat4("model", model);
	my_esfera.render();

	model = tmp;
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(x2, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.12f, 0.03f, 0.03));
	projectionShader.setMat4("model", model);
	my_cilindro.render();

	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
	projectionShader.setMat4("model", model);
	my_esfera.render();

	model = tmp;
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(x1, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.12f, 0.03f, 0.03));
	projectionShader.setMat4("model", model);
	my_cilindro.render();

	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
	projectionShader.setMat4("model", model);
	my_esfera.render();
}

void displayGun() {

	Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs"); //PARA PRIMITIVAS

	projectionShader.use();
	projectionShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("lightPos", lightPos);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);

	model = glm::translate(model, camera.Position);
	model = glm::translate(model, glm::vec3(0.0f, -0.05f, -0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mov_horizontal), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp = model = glm::rotate(model, glm::radians(mov_vertical), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, camera.Up.);
	model = glm::scale(model, glm::vec3(0.12f, 0.003f, 0.003));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 0.1f, 0.1f, 0.1f);
	projectionShader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();


}

void fireBullet(){

	Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs"); //PARA PRIMITIVAS

	projectionShader.use();
	projectionShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("lightPos", lightPos);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	glBindVertexArray(VAO);

	model = glm::translate(model, camera.Position);
	model = glm::translate(model, glm::vec3(0.0f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(mov_horizontal), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mov_vertical), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.19f));
	model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 0.0f, 0.0f, 0.0f);
	projectionShader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_esfera.render();

}


void displayCubes()
{
	Shader lightingShader("shaders/shader_texture_light_dir.vs", "shaders/shader_texture_light_dir.fs"); //Directional

	lightingShader.use();
	lightingShader.setVec3("light.direction", lightDirection);
	lightingShader.setVec3("viewPos", camera.Position);

	// light properties
	lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//For Positional and Spotlight
	lightingShader.setFloat("light.constant", 1.0f);
	lightingShader.setFloat("light.linear", 0.09f);
	lightingShader.setFloat("light.quadratic", 0.032f);

	// material properties
	lightingShader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	//lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setMat4("model", model);
	lightingShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	lightingShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	lightingShader.setVec3("ambientColor", 0.0f, 0.0f, 0.0f);
	lightingShader.setVec3("diffuseColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("specularColor", 1.0f, 0.0f, 0.0f);
	lightingShader.setInt("material_diffuse", t_caja);

	//cara cubo
	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.45f));
	model = glm::scale(model, glm::vec3(0.8f, 0.6f, 0.05f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 4);

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.8f, -0.45f));
	model = glm::scale(model, glm::vec3(0.8f, 0.6f, 0.05f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 4, 4);

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.0f));
	model = glm::scale(model, glm::vec3(0.8f, 0.6f, 0.86f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 8, 4);

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.95f, 0.0f));
	model = glm::scale(model, glm::vec3(0.8f, 0.9f, 0.86f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 12, 4);

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.8f, 1.0f, 0.86f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 16, 4);

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.19f, 0.66f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.3f, 0.84f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);


	glBindVertexArray(0);

}

void display(Shader shader, Model ground)
{
	shader.use();

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	model = glm::mat4(1.0f);
	//Ground
	model = glm::scale(model, glm::vec3(tamanioPista / 10, tamanioPista / 10, tamanioPista / 10));
	shader.setMat4("model", model);
	ground.Draw(shader);

}

//Primitiva de un riel para la montaña rusa
void drawSegment(Shader projectionShader) {

	projectionShader.use();
	projectionShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("lightPos", lightPos);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	model = modelMR;

	glBindVertexArray(VAO);
	//Segmento riel
	//
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 4, sy_cil / 8, sz_cil / 8));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 1.0f, 0.0f, 0.0f);
	projectionShader.setVec3("specularColor", 0.0f, 0.0f, 1.0f);
	my_cilindro.render();

	tmp = model;

	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 4.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.6f, 0.6f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	projectionShader.setVec3("specularColor", 0.0f, 0.0f, 1.0f);
	my_cilindro.render();

	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 2.0f, -4.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.6f, 0.6f));
	projectionShader.setMat4("model", model);
	my_cilindro.render();

	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 1.0f, 2.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-28.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.2f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 1.0f, 0.0f, 0.0f);
	projectionShader.setVec3("specularColor", 0.0f, 0.0f, 1.0f);
	my_cilindro.render();

	model = tmp;

	model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(28.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.2f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	my_cilindro.render();

	
}

//Función de trazo para el domo del carrusel
void drawDomoCarrusel(Shader projectionShader) {
	Shader lightingShader("shaders/shader_texture_light_dir.vs", "shaders/shader_texture_light_dir.fs"); //Directional

	lightingShader.use();
	
	lightingShader.setVec3("light.direction", lightDirection);
	
	lightingShader.setVec3("viewPos", camera.Position);
		// light properties
	lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//For Positional and Spotlight
	lightingShader.setFloat("light.constant", 1.0f);
	lightingShader.setFloat("light.linear", 0.09f);
	lightingShader.setFloat("light.quadratic", 0.032f);
		// material properties
	lightingShader.setFloat("material_shininess", 32.0f);
		// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
		//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();
		// pass them to the shaders
	//lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setMat4("model", model);
	lightingShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	lightingShader.setMat4("projection", projection);

	glBindVertexArray(VAO);
	//Colocar código aquí
	lightingShader.setVec3("ambientColor", 0.0f, 0.0f, 0.0f);
	lightingShader.setVec3("diffuseColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("specularColor", 1.0f, 0.0f, 0.0f);
	lightingShader.setInt("material_diffuse", t_domo);


	model = modelDomo;
	model = glm::translate(model, glm::vec3(16.0f, -3.0f, -20.0f)); //Centro del carrusel
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.2f, 1.4f, 7.2f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 48, 24);

}

//Función para trazar el carrito de la montaña rusa
void drawVagon(Shader projectionShader) {
	projectionShader.use();
	projectionShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("lightPos", lightPos);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	projectionShader.setVec3("viewPos", camera.Position);
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	//vagon
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.5f));
	model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.05f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 0.0f, 0.0f, 0.0f);
	projectionShader.setVec3("specularColor", 0.0f, 0.0f, 1.0f);
	my_esfera.render();

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, -0.5f));
	model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.05f));
	projectionShader.setMat4("model", model);
	my_esfera.render();

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.05f, 0.05f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 0.8f, 0.8f, 0.8f);
	projectionShader.setVec3("specularColor", 0.0f, 0.0f, 1.0f);
	my_cilindro.render();

	//
	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.2f, 1.2f, -0.35f));
	model = glm::rotate(model, glm::radians(28.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.15f, 0.02f, 0.02f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 0.1f, 0.1f, 0.1f);
	projectionShader.setVec3("specularColor", 0.0f, 0.0f, 1.0f);
	my_cilindro.render();

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.2f, 1.2f, +0.35f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.15f, 0.02f, 0.02f));
	projectionShader.setMat4("model", model);
	my_cilindro.render();

	model = modelVagon;
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::rotate(model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 1.09f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.22f, 0.02f, 0.02f));
	projectionShader.setMat4("model", model);
	my_cilindro.render();


	displayCubes();

}

//Funcion para trazar el suelo donde yace la feria
void displayEnvironment() {
	Shader lightingShader("shaders/shader_texture_light_dir.vs", "shaders/shader_texture_light_dir.fs"); //Directional
	lightingShader.use();
	lightingShader.setVec3("light.direction", lightDirection);
	lightingShader.setVec3("viewPos", camera.Position);

	// light properties
	lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//For Positional and Spotlight
	lightingShader.setFloat("light.constant", 1.0f);
	lightingShader.setFloat("light.linear", 0.09f);
	lightingShader.setFloat("light.quadratic", 0.032f);

	// material properties
	lightingShader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	//lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setMat4("model", model);
	lightingShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	lightingShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	lightingShader.setVec3("ambientColor", 1.0f, 0.0f, 0.0f);
	lightingShader.setVec3("diffuseColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setInt("material_diffuse", t_ladrillo);

	model = glm::translate(model, glm::vec3(0.0f, -6.6f, -25.0f));
	model = glm::scale(model, glm::vec3(50.0f, 1.0f, 40.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 72, 4); //dibujamos unicamente la parte superior de una caja texturizada con ladrillos para el piso

}

//Funcion para dibujar los arbustos
void displayBushes(){
	Shader lightingShader("shaders/shader_texture_light_dir.vs", "shaders/shader_texture_light_dir.fs"); //Directional
	lightingShader.use();
	lightingShader.setVec3("light.direction", lightDirection);
	lightingShader.setVec3("viewPos", camera.Position);

	// light properties
	lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//For Positional and Spotlight
	lightingShader.setFloat("light.constant", 1.0f);
	lightingShader.setFloat("light.linear", 0.09f);
	lightingShader.setFloat("light.quadratic", 0.032f);

	// material properties
	lightingShader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	//lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setMat4("model", model);
	lightingShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	lightingShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	lightingShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("diffuseColor", 0.0f, 1.0f, 0.0f);
	lightingShader.setVec3("specularColor", 0.0f, 1.0f, 0.0f);
	lightingShader.setInt("material_diffuse", t_bush);

	
	model = glm::translate(model, glm::vec3(5.0f, -5.65f, -5.5f));
	model = glm::scale(model, glm::vec3(1.0f, 0.8f, 0.5f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 
	temp = model;

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 


	//Arbustos del lado derecho del parque
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, -0.5f));
	model = glm::scale(model, glm::vec3(0.5f, 1.0f, 2.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); 

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
	
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Arbustos de atrás
	model = glm::translate(model, glm::vec3(-1.2f, 0.0f, -0.2f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 0.5f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Arbusto de la izquierda
	model = glm::translate(model, glm::vec3(-0.3f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 1.0f, 2.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	//Arbustos que faltan enfrente
	model = temp;
	model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);

	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24);
}

//Función de trazo de la montaña rusa
void displayRoallingCoaster(Shader shader) {

	Shader temp = shader;

	shader.use();
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("lightPos", lightPos);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setVec3("viewPos", camera.Position);
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	modelMR = glm::mat4(1.0f);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, -4.5f, -20.0f)); //Pegué tu montaña rusa un poco más al suelo y la desplazé más al fondo para que se vea completa al ejecutar.
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	modelVagon = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp); //Completo giro de 90 grados
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp); //completo giro de 180 grados
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	//empieza giro tonel
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	//inicio giro
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);//fin giro 180 grados
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	//inicio Loop
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	//fin loop
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, 0.05f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	//inicia giro
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-9.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	//
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);

	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
	model = glm::rotate(model, glm::radians(6.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	//
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, -0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);

	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.0f, 0.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.5f, 0.05f, 0.0f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
}

//Variables que delimitan la velocidad de animacion del carrusel
void startCarrouselSpin() {
	
	animTubos += 0.5;
	animGiro += 0.125;
	movModelos += 0.2; //Velocidad con la que suben y bajan los modelos del carrusel en los tubos con ajuste para estar más pegados a la base.

}

//Función de dibujo de .obj
void drawModel(Shader shader, Model modelo1, int id) //con int id, agregamos un case al switch y así modificamos ahí individualmente los parámetros de dibujo
{
	shader.use();

	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	switch (id) {
	case 1: //El primer caballo del carrusel
		model = glm::translate(model, glm::vec3(16.0f, -4.5f, -20.0f)); //Centro del carrusel
		model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro al rededor del centro
		model = glm::translate(model, glm::vec3(2.5f, (1.5 * sin(movModelos) / 4) - 0.5f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Enderezar el modelo del caballo para que quede de pie
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Ajuste de posicion para que vea hacia enfrente
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f)); //Ajustar el tamaño del modelo caballo
		shader.setMat4("model", model);
		modelo1.Draw(shader);
		break;
	case 2: //La primer lámpara cuádruple que va a un lado del carrusel
		model = glm::translate(model, glm::vec3(11.0f, -5.5f, -15.0f));
		model = glm::scale(model, glm::vec3(0.0035f, 0.0035f, 0.0035f));
		shader.setMat4("model", model);
		modelo1.Draw(shader);
		break;
	case 3:
		model = glm::translate(model, glm::vec3(16.0f, -4.5f, -20.0f)); //Centro del carrusel
		model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro al rededor del centro
		model = glm::translate(model, glm::vec3(-2.5f, (1.5 * sin(movModelos + 90) / 4) - 0.7f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Enderezar el modelo del caballo para que quede de pie
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f)); //Ajustar el tamaño del modelo caballo
		shader.setMat4("model", model);
		modelo1.Draw(shader);
		break;
	case 4:
		model = glm::translate(model, glm::vec3(16.0f, -4.5f, -20.0f)); //Centro del carrusel
		model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro al rededor del centro
		model = glm::translate(model, glm::vec3(0.0f, (1.5 * sin(movModelos + 45) / 4) - 0.7f, 2.5f)); //Movimiento de subir y bajar y colocar en tubo
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Enderezar el modelo del caballo para que quede de pie
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Ajuste de posicion para que vea hacia enfrente
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f)); //Ajustar el tamaño del modelo caballo
		shader.setMat4("model", model);
		modelo1.Draw(shader);
		break;
	case 5:
		model = glm::translate(model, glm::vec3(16.0f, -4.5f, -20.0f)); //Centro del carrusel
		model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro al rededor del centro
		model = glm::translate(model, glm::vec3(0.0f, (1.5 * sin(movModelos + 135) / 4) - 0.7f, -2.5f)); //Movimiento de subir y bajar y colocar en tubo
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Enderezar el modelo del caballo para que quede de pie
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Ajuste de posicion para que vea hacia enfrente
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f)); //Ajustar el tamaño del modelo caballo
		shader.setMat4("model", model); 
		modelo1.Draw(shader);
		break;
	case 6: //La primer lámpara cuádruple que va a un lado del carrusel
		model = glm::translate(model, glm::vec3(11.0f, -5.5f, -20.0f));
		model = glm::scale(model, glm::vec3(0.0035f, 0.0035f, 0.0035f));
		shader.setMat4("model", model);
		modelo1.Draw(shader);
		break;
	}

}

//Función vieja para dibujo de .obj
/*
void drawModelosCarrusel(Shader shader, Model modelo1) //Creo una funcion duplicado "drawModel" con identificador, para dibujar objetos con una sola funcion.
{
	shader.use();

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	model = modelHorse;
	//
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro al rededor del centro
	model = glm::translate(model, glm::vec3(2.5f, (sin(movModelos) / 4) - 0.5f, 0.0f)); //Movimiento de subir y bajar
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //Enderezar el modelo del caballo para que quede de pie
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Ajuste de posicion para que vea hacia enfrente
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f)); //Ajustar el tamaño del modelo caballo
	shader.setMat4("model", model);

	modelo1.Draw(shader);
}*/

//Función para trazar el carrusel
void displayCarrousell(Shader shader, Shader Modelshader, Model modelo1) {

	shader.use();
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("lightPos", lightPos);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setVec3("viewPos", camera.Position);
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	glBindVertexArray(VAO);

	//Intento fallido, mis triangulitos no se mueven xd :(
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glVertexPointer(3, GL_FLOAT, 0, domeVertices);
//	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
//	glDisableClientState(GL_VERTEX_ARRAY);

	//Ejemplo
//	drawSegment(temp);
//	model = modelMR;
//	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.05f));
//	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelMR = model;

//Centro del carrusel
	model = glm::translate(model, glm::vec3(16.0f, -4.5f, -20.0f));
	tmp = model;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 1.3, sy_cil / 1.5, sz_cil / 1.5));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 1.0f, 0.84f, 0.05f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();

	//Color 1 de la base
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(0.0f, -1.3f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 8, sy_cil * 3.19, sz_cil * 3.19));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.51f, 0.22f, 0.09f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();

	//Color 2 de la base
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 8, sy_cil * 3.2, sz_cil * 3.2));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 1.0f, 0.57f, 0.40f);
	shader.setVec3("diffuseColor", 1.0f, 0.57f, 0.40f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();

	//Base
	//#MARK poner una textura de madera
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 8, sy_cil * 3.185, sz_cil * 3.185));
	modelHorse = model;
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.51f, 0.22f, 0.09f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_esfera.render();

	//Cabecera del carrusel
	//#MARK A este modelo le ponemos la textura "CabeceraCarrusel.png para que se vea bonito :v
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(0.0f, 1.4f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 4, sy_cil * 3.5, sz_cil * 3.5));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 1.0f, 0.57f, 0.40f);
	shader.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();

	//Techo del carrusel
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(0.0f, 1.2f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelLuces = model;
	model = glm::scale(model, glm::vec3(sx_cil / 4, sy_cil * 3.49, sz_cil * 3.49));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 1.0f, 0.57f, 0.40f);
	shader.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_esfera.render();

	//Esfera de adorno que va arriba de la cúpula/domo sobre el carrusel
	//#MARK Aún no encuentro cómo declarar la primitiva para la cúpula; pensaba hacer triángulos como dibujamos las cajas en prácticas pasadas, pero el shader no me deja xdddd
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(0.0f, 3.5f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 4, sy_cil / 4, sz_cil / 4));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 1.0f, 0.57f, 0.40f);
	shader.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_esfera.render();

	//Tubos para modelo de caballo
	//#1
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil, sy_cil / 15, sz_cil / 15));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.84f, 0.84f, 0.84f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();
	if (!horseOn) {
		//Sillita
		model = tmp;
		model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(sx_cil / 2, sy_cil / 8, sz_cil / 2));
		model = glm::translate(model, glm::vec3(0.0f, (sin(movModelos + 135)) - 0.5f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
		shader.setMat4("model", model);
		shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("diffuseColor", 1.0f, 0.0f, 0.0f);
		shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
		my_esfera.render();
	}
	

	//#2
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(-2.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil, sy_cil / 15, sz_cil / 15));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.84f, 0.84f, 0.84f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();
	if (!horseOn) {
		//Sillita
		model = tmp;
		model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
		model = glm::translate(model, glm::vec3(-2.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(sx_cil / 2, sy_cil / 8, sz_cil / 2));
		model = glm::translate(model, glm::vec3(0.0f, (sin(movModelos + 135)) - 0.5f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
		shader.setMat4("model", model);
		shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("diffuseColor", 1.0f, 0.0f, 0.0f);
		shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
		my_esfera.render();
	}

	//#3
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil, sy_cil / 15, sz_cil / 15));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.84f, 0.84f, 0.84f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();
	if (!horseOn) {
		//Sillita
		model = tmp;
		model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.5f));
		model = glm::scale(model, glm::vec3(sx_cil / 2, sy_cil / 8, sz_cil / 2));
		model = glm::translate(model, glm::vec3(0.0f, (sin(movModelos + 135)) - 0.5f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
		shader.setMat4("model", model);
		shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("diffuseColor", 1.0f, 0.0f, 0.0f);
		shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
		my_esfera.render();
	}

	//#4
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil, sy_cil / 15, sz_cil / 15));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.84f, 0.84f, 0.84f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();
	if (!horseOn) {
		//Sillita
		model = tmp;
		model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(sx_cil / 2, sy_cil / 8, sz_cil / 2));
		model = glm::translate(model, glm::vec3(0.0f, (sin(movModelos + 135)) - 0.5f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
		shader.setMat4("model", model);
		shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("diffuseColor", 1.0f, 0.0f, 0.0f);
		shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
		my_esfera.render();
	}

	//#5
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil, sy_cil / 18, sz_cil / 18));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.84f, 0.84f, 0.84f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();
	//Sillita
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 2, sy_cil / 8 , sz_cil / 2 ));
	model = glm::translate(model, glm::vec3(0.0f, (sin(movModelos)) - 0.5f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_esfera.render();


	//#6
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil, sy_cil / 18, sz_cil / 18));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.84f, 0.84f, 0.84f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();
	//Sillita
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 2, sy_cil / 8, sz_cil / 2));
	model = glm::translate(model, glm::vec3(0.0f, (sin(movModelos + 90)) - 0.5f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_esfera.render();

	//#7
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil, sy_cil / 18, sz_cil / 18));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.84f, 0.84f, 0.84f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();
	//Sillita
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 2, sy_cil / 8, sz_cil / 2));
	model = glm::translate(model, glm::vec3(0.0f, (sin(movModelos + 45)) - 0.5f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_esfera.render();

	//#8
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(sx_cil, sy_cil / 18, sz_cil / 18));
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 0.83f, 0.68f, 0.21f);
	shader.setVec3("diffuseColor", 0.84f, 0.84f, 0.84f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_cilindro.render();
	//Sillita
	model = tmp;
	model = glm::rotate(model, glm::radians(animTubos), glm::vec3(0.0f, 1.0f, 0.0f)); //Animacion de giro
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(sx_cil / 2, sy_cil / 8, sz_cil / 2));
	model = glm::translate(model, glm::vec3(0.0f, (sin(movModelos + 135)) - 0.5f, 0.0f)); //Movimiento de subir y bajar y colocar en tubo
	shader.setMat4("model", model);
	shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	shader.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_esfera.render();

	model = glm::mat4(1.0f);
	if (horseOn) {
		drawModel(Modelshader, modelo1, 1); //Función equivalente utilizando la nueva función para evitar el duplicado de código
		drawModel(Modelshader, modelo1, 3);
		drawModel(Modelshader, modelo1, 4);
		drawModel(Modelshader, modelo1, 5);
	}
	drawDomoCarrusel(shader);

	//luces carrusel
	if (giroSol >= 90.0f && giroSol < 270.0f)
		displayLights();
	if (giroSol < 90.0f || giroSol >= 270.0f)
		displayLightsOff();

}

//Dentro de ésta funcion mandamos a llamar a drawModelos con el modelo que queramos y un id creciente; dentro de esa función modificamos los parámetros de dibujo.
//Funcion que recibe todos nuestros modelos y los manda a dibujar tantas veces como queramos
void displayObjects(Shader shader, Model modelTierra, Model modelPista, /*Model modelArbol, Model modelBanca, Model modelBasura, Model modelBarda,*/ Model modelCaballo, /*Model modelLuz2,*/ Model modelLuz4/*, Model modelLuz1, Model modelEntrada*/) {
	shader.use();

	if (modelSwitch) {
		drawModel(shader, modelLuz4, 2);
		drawModel(shader, modelLuz4, 6);
	}
}

void creaArchivo(string name) {
	ofstream fs(name.c_str());
	fs.close();
}

string leerArchivo(string name,long numlinea) {
	ifstream fs(name.c_str(), ios::in);
	char linea[128];
	long contador = 0L;
	if (fs.fail())
		printf("el fichero no existe\n");
	else
		while (!fs.eof()) {
			fs.getline(linea, sizeof(linea));

			if (numlinea == contador) {
				//cout << linea << endl;
				string str(linea);
				return str;
				break;
			}
			contador++;
		}
	fs.close();
	
}

void readFile(string name) {
	ifstream fs(name.c_str(), ios::in);
	char linea[128];
	long contador = 0L;
	if (fs.fail())
		printf("el fichero no existe\n");
	else
		while (!fs.eof()) {
			fs.getline(linea, sizeof(linea));
			cout << linea << endl;
		}
	fs.close();
}

void editarArchivo(string name, string contenido) {
	ofstream fs(name.c_str(), ios::app);
	fs << contenido << endl;
	fs.close();
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	//


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 10", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();

	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	my_esfera.init();
	my_cilindro.init();
	my_toroide.init();
	glEnable(GL_DEPTH_TEST);
	
	Shader modelShader("Shaders/modelLoading.vs", "Shaders/modelLoading.fs");
	Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs"); //PARA PRIMITIVAS

	// Load models
	Model modelTierra	= ((char *)"Models/tierra/Earth.obj");
	Model modelPista	= ((char *)"Models/pista.obj");
	//Model modelArbol	= ((char *)"Models/arbol/arbol.obj");
	//Model modelBanca	= ((char *)"Models/banca/banca.obj");
	//Model modelBasura	= ((char *)"Models/basura/basura.obj");
	//Model modelBarda	= ((char *)"Models/barda/barda.obj");
	//Model modelCaballo = ((char *)"Models/caballo/caballo.obj");
	//Model modelLuz2		= ((char *)"Models/luz_dual/luz_dual.obj");
	//Model modelLuz4		= ((char *)"Models/luz_quad/luz_quad.obj");
	//Model modelLuz1		= ((char *)"Models/luz_simple/luz_simple.obj");
	//Model modelEntrada	= ((char *)"Models/arco/arco.obj");
	//Model Model = ((char *)"Models/.obj");

	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	
	camera.reposition();
	//render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        my_input(window);
		animate();
		startCarrouselSpin(); //Animación de giro de los tubos #MARK Los modelos de caballos deben girar a ésta velocidad y subir bajar de una forma senoidal

        // render
        // Backgound color
		glClearColor(colorR, colorGB, colorGB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display(modelShader, modelPista);
		//displayRoallingCoaster(projectionShader);
		//drawVagon(projectionShader);
		diplayElemCielo();
		//displayCarrousell(projectionShader, modelShader, modelCaballo);
		//displayObjects(modelShader, modelTierra, modelPista, /*modelArbol, modelBanca, modelBasura, modelBarda,*/ modelCaballo, /*modelLuz2,*/ modelLuz4/*, modelLuz1, modelEntrada*/);
		displayEnvironment();
		//displayBushes();
		displayPuestoTiro();
		if (game==true) {
			displayGun();
			fireBullet();
		}
		

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		xoffset1 += 0.1;
		yoffset1 += 0.1;
		glfwWaitEventsTimeout(1.7);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		xoffset1 -= 0.1;
		yoffset1 -= 0.1;
		glfwWaitEventsTimeout(1.7);
	}

	if (glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS) {
		if (cameraMode == true) {
			camera.reposition();
		}
		cameraMode = !cameraMode;
		glfwWaitEventsTimeout(1.7);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (cameraMode == false) {
			camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		}
		else{
			camera.ProcessKeyboardFree(FORWARD, (float)deltaTime);
		}
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (cameraMode == false) {
			camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		}
		else {
			camera.ProcessKeyboardFree(BACKWARD, (float)deltaTime);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (cameraMode == false) {
			camera.ProcessKeyboard(LEFT, (float)deltaTime);
		}
		else {
			camera.ProcessKeyboardFree(LEFT, (float)deltaTime);
		}
	}
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (cameraMode == false) {
			camera.ProcessKeyboard(RIGHT, (float)deltaTime);
		}
		else {
			camera.ProcessKeyboardFree(RIGHT, (float)deltaTime);
		}
	}
		


	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
		camera.ProcessMouseMovement(-xoffset1 / (float)deltaTime, 0);
		mov_horizontal += 0.1f*xoffset1 / (float)deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		camera.ProcessMouseMovement(xoffset1 / (float)deltaTime, 0);
		mov_horizontal -= 0.1f*xoffset1 / (float)deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
		camera.ProcessMouseMovement(0, yoffset1 / (float)deltaTime);
		mov_vertical += 0.1f*yoffset1 / (float)deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
		camera.ProcessMouseMovement(0, -yoffset1 / (float)deltaTime);
		mov_vertical -= 0.1f*yoffset1 / (float)deltaTime;
	}

	//Movimiento del modelo Vagon
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && bandera1 == true)
		posY += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && bandera1 == true)
		posY -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && bandera1 == true)
		posX -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && bandera1 == true)
		posX += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS && bandera1 == true)
		posZ += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS && bandera1 == true)
		posZ -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_X) && bandera1 == true)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			//if (movCodo < 0)
			rotX -= 0.1f;
		}
		else {
			//if (movCodo > -140)
			rotX += 0.1f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Z) && bandera1 == true)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			//if (movCodo < 0)
			rotY -= 0.1f;
		}
		else {
			//if (movCodo > -140)
			rotY += 0.1f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_C) && bandera1 == true)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			//if (movCodo < 0)
			rotZ -= 0.1f;
		}
		else {
			//if (movCodo > -140)
			rotZ += 0.1f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS) {
		bandera1 = !bandera1;
		glfwWaitEventsTimeout(1.7);
	}

	//bullet
	if (glfwGetKey(window, GLFW_KEY_UP)) {
		bullet -= 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		bullet += 0.001f;
	}

	//To play KeyFrame animation 
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
		glfwWaitEventsTimeout(1.7);
	}

	//To Save a KeyFrame
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
			glfwWaitEventsTimeout(1.7); //delay para evitar lecturas erroneas del teclado
		}
	}

	//Para vaciar el fichero
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		creaArchivo("animation.txt");
		creaArchivo("frames.txt");
		glfwWaitEventsTimeout(1.7);
	}
		
	//Para leer el fichero
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		readFile("animation.txt");
		//cargaFrames();
		glfwWaitEventsTimeout(1.7);
	}

	//Para cargar las posiciones guardadas en el arreglo de estructuras
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		cargaFrames();
		glfwWaitEventsTimeout(1.7);
	}

	//Para activar/desactivar modelos en el espacio. Desactivados por defecto para acelerar la carga
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		modelSwitch = !modelSwitch;
		horseOn = !horseOn;
		glfwWaitEventsTimeout(1.7);
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		game = !game;
		glfwWaitEventsTimeout(1.7);
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;


	//if (game == false) {
		camera.ProcessMouseMovement(xoffset, yoffset);
		mov_horizontal -= 0.001f*xoffset / (float)deltaTime;
		//mov_vertical += 0.001f*yoffset / (float)deltaTime;
	//}
	
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}