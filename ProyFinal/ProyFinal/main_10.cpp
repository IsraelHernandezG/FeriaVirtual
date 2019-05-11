/*--------------------------------------------------------*/
/* -------------   Proyecto Final ------------------------*/
/*-----------------    2019-2   --------------------------*/
/*- Computación gráfica e interacción humano computadora -*/
/*-------------------- Grupo 04 --------------------------*/
/*-------------------- Alumnos: --------------------------*/
/*-------------- Esparza Luis Mauricio -------------------*/
/*------------- Hernández García Israel ------------------*/


//#define STB_IMAGE_IMPLEMENTATION
#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>

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
glm::mat4 modelHorse = glm::mat4(1.0f);
glm::mat4 modelSingleLamp = glm::mat4(1.0f);
glm::mat4 modelDoubleLamp = glm::mat4(1.0f);
glm::mat4 modelQuadLamp = glm::mat4(1.0f);
glm::mat4 modelFence = glm::mat4(1.0f);
glm::mat4 modelBush = glm::mat4(1.0f);
glm::mat4 modelArch = glm::mat4(1.0f);
glm::mat4 modelGarbage = glm::mat4(1.0f);

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;
double xoffset1 = 0.01;
double yoffset1 = 0.01;

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
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;
bool play = false;

//For model
float movKit_z = 0.0f,
	girollanta = 0.0f,
	direccion = 1.0f,
	rotacion = 0.0f,
	tamanioPista = 20.0f;

//For carrusel
float	animTubos = 0.0f,
		animGiro = 0.0f,
		movModelos = 0.0f;

unsigned int generateTextures(const char* filename, bool alfa)
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
		std::cout << "Failed to load texture" << std::endl;
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

	
}

void myData()
{	
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

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
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,


		//////Segunda caja

		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

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
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

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
	
	//rotacion += 0.05f;

	movKit_z += 0.01f * direccion;
	if (movKit_z >= tamanioPista) {
		direccion = -1;
	}
	if (movKit_z <= -tamanioPista) {
		direccion = 1;
	}
		

}

void display(Shader shader, Model modelo1, Model ground)
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

	model = glm::mat4(1.0f);

	//
	tmp = model = glm::translate(model, glm::vec3(movKit_z-4.0f, 0.0f, 2.0f));
	model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); //earth
	shader.setMat4("model", model);

	//modelo1.Draw(shader);
}

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
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	projectionShader.setVec3("specularColor", 0.0f, 0.0f, 1.0f);
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
	projectionShader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	projectionShader.setVec3("diffuseColor", 1.0f, 0.0f, 0.0f);
	projectionShader.setVec3("specularColor", 0.0f, 0.0f, 1.0f);
	my_cilindro.render();

	
}

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
	model = glm::translate(model, glm::vec3(0.78f, -4.5f, 0.0f));
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
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = modelMR;
	model = glm::translate(model, glm::vec3(0.78f, 0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMR = model;
	drawSegment(temp);
	model = glm::translate(model, glm::vec3(0.74f, -0.05f, 0.0f));
	//model = glm::rotate(model, glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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


GLfloat domeVertices[] =
{
	12,56,84,10,29,854,45,11,586,
	0.0f, 0.0f, 10.0f, //pico
	0.0f, 0.0f, -10.0f,
	3.0f, 0.0f, -9.0f,
	6.0f, 0.0f, -6.0f,
	9.0f, 0.0f, -3.0f,
	10.0f, 0.0f, 0.0f,
	9.0f, 0.0f, 3.0f,
	6.0f, 0.0f, 6.0f,
	3.0f, 0.0f, 9.0f,
	0.0f, 0.0f, 10.0f,
	-3.0f, 0.0f, 9.0f,
	-6.0f, 0.0f, 6.0f,
	-9.0f, 0.0f, 3.0f,
	-10.0f, 0.0f, 0.0f,
	-9.0f, 0.0f, -3.0f,
	-6.0f, 0.0f, -6.0f,
	-3.0f, 0.0f, -9.0f,
};

void startCarrouselSpin() {
	
	animTubos += 0.5;
	animGiro += 0.125;
	movModelos += 0.2; //Velocidad con la que suben y bajan los modelos del carrusel en los tubos con ajuste para estar más pegados a la base.

}

void drawModelosCarrusel(Shader shader, Model modelo1)
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
}

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
	model = glm::translate(model, glm::vec3(16.0f, -4.5f, 10.0f));
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
	model = tmp;
	modelHorse = model;

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

	//	model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.3f));
	//	view = glm::translate(view, glm::vec3(10.0f, -5.0f, 0.0f));
	//	model = glm::rotate(model, glm::radians(0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
	//	shader.setMat4("model", model);
	//	shader.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	//	shader.setVec3("diffuseColor", 0.5f, 0.8f, 0.2f);
	//	shader.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	//	my_cilindro.render();	//Base Carrusel

	model = glm::mat4(1.0f);
	drawModelosCarrusel(Modelshader, modelo1);
}

void displayScene(Shader shader, Model modelo1, Model modelo2, Model modelo3) {
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


	model = glm::translate(model, glm::vec3(-2.0f, -4.5f, 1.0f));
	model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
	shader.setMat4("model", model);

	modelo1.Draw(shader);
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
	Model Model1 = ((char *)"Models/tierra/Earth.obj");
	Model Model2 = ((char *)"Models/pista.obj");
	Model Model3 = ((char *)"Models/arbol/arbol.obj");
	Model Model4 = ((char *)"Models/banca/banca.obj");
	Model Model5 = ((char *)"Models/basura/basura.obj");
	Model Model6 = ((char *)"Models/barda/barda.obj");
	Model Model7 = ((char *)"Models/caballo/caballo.obj");
	Model Model8 = ((char *)"Models/luz_dual/luz_dual.obj");
	Model Model9 = ((char *)"Models/luz_quad/luz_quad.obj");
	Model Model10 = ((char *)"Models/luz_simple/luz_simple.obj");
	Model Model11 = ((char *)"Models/arco/arco.obj");
	//Model Model = ((char *)"Models/.obj");

    
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
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
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display(modelShader, Model1, Model2);
		displayRoallingCoaster(projectionShader);
		displayCarrousell(projectionShader, modelShader, Model7);
		//displayScene(modelShader, Model12, Model10, Model11);

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
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS){
		xoffset1 = 2.0;
		yoffset1 = 2.0;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
		camera.ProcessMouseMovement(-xoffset1/(float)deltaTime, 0);
	}

	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
		camera.ProcessMouseMovement(xoffset1/(float)deltaTime, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS) {
		camera.ProcessMouseMovement(0, yoffset1 / (float)deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
		camera.ProcessMouseMovement(0, -yoffset1 / (float)deltaTime);
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}