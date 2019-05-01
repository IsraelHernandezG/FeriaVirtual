#include "cilindro.h"

//glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
//GLuint VBO, VAO, lightVAO;
Cilindro my_cylinder1(1.0);
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//int SCR_WIDTH = 3800;
//int SCR_HEIGHT = 7600;

Cilindro::Cilindro(GLfloat radio) :

	radio(radio) {
	cilindro_VAO[4] = (-1, -1, -1, -1);
	cilindro_VBO[4] = (-1, -1, -1, -1);

}


Cilindro::~Cilindro()
{
	glDeleteVertexArrays(4, cilindro_VAO);
	glDeleteBuffers(4, cilindro_VBO);
}


void Cilindro::init()
{
	const int nn = PARALELOS * MERIDIANOS * 3;
	GLfloat cylinder_nor[nn]; // normal
	// generate the sphere data
	GLfloat x, y, z, a, b, da, db, r = 1.0;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(MERIDIANOS);
	db = (GLfloat)M_PI / GLfloat(PARALELOS - 1);

	// [Generate sphere point data]

	// spherical angles a,b covering whole sphere surface
	for (ix = 0, b = (GLfloat)-0.5*M_PI, ib = 0; ib < PARALELOS; ib++, b += db)
		for (a = 0.0, ia = 0; ia < MERIDIANOS; ia++, a += da, ix += 3)
		{
			x = b;
			y = sin(a);
			z = cos(a);
			cilindro_pos[ix + 0] = x * r;
			cilindro_pos[ix + 1] = y * r;
			cilindro_pos[ix + 2] = z * r;
			cylinder_nor[ix + 0] = x;
			cylinder_nor[ix + 1] = y;
			cylinder_nor[ix + 2] = z;
		}
	// [Generate GL_TRIANGLE indices]
	for (ix = 0, iy = 0, ib = 1; ib < PARALELOS; ib++)
	{
		for (ia = 1; ia < MERIDIANOS; ia++, iy++)
		{
			// first half of QUAD
			cilindro_index[ix] = iy;      ix++;
			cilindro_index[ix] = iy + 1;    ix++;
			cilindro_index[ix] = iy + MERIDIANOS;   ix++;
			// second half of QUAD
			cilindro_index[ix] = iy + MERIDIANOS;   ix++;
			cilindro_index[ix] = iy + 1;    ix++;
			cilindro_index[ix] = iy + MERIDIANOS + 1; ix++;
		}
		// first half of QUAD
		cilindro_index[ix] = iy;       ix++;
		cilindro_index[ix] = iy + 1 - MERIDIANOS;  ix++;
		cilindro_index[ix] = iy + MERIDIANOS;    ix++;
		// second half of QUAD
		cilindro_index[ix] = iy + MERIDIANOS;    ix++;
		cilindro_index[ix] = iy - MERIDIANOS + 1;  ix++;
		cilindro_index[ix] = iy + 1;     ix++;
		iy++;
	}
	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, cilindro_VAO);
	glGenBuffers(4, cilindro_VBO);
	glBindVertexArray(cilindro_VAO[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cilindro_pos), cilindro_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // normal
	glBindBuffer(GL_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_nor), cylinder_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cilindro_index), cilindro_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Cilindro::render()
{
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(cilindro_VAO[0]);
	//  glDrawArrays(GL_POINTS,0,sizeof(esfera_pos)/sizeof(GLfloat));                   // POINTS ... no indices for debug
	glDrawElements(GL_TRIANGLES, sizeof(cilindro_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)
	glBindVertexArray(0);
}
