
#pragma once
#ifndef TOROIDE_H_
#define TOROIDE_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>
#define M_PI 3.14159265358979323846264338327950288
#define MERIDIANOS 50
#define PARALELOS 10
class Toroide
{
public:
	Toroide(GLfloat radio);
	void init();
	//void load();
	void render();
	//void riel(glm::mat4 model);
	virtual ~Toroide();

private:

	float radio;

	GLuint toroide_VAO[4], toroide_VBO[4], toroide_index[MERIDIANOS*(PARALELOS - 1) * 6];
	GLfloat toroide_pos[PARALELOS * MERIDIANOS * 3];

};

#endif // !CILINDRO_H_