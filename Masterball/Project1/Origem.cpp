#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <string.h>
#include <time.h>
#include <cmath>


// Essas variáveis são usadas para controlar a posição e ângulo da câmera, bem como o estado de exibição das bolas (Pokeball, Greatball e Masterball) e a animação de movimento da Pokeball.
float cameraX = 0.0f;
float cameraY = 2.0f;
float cameraZ = 15.0f;
int lastMouseX = -1;
int LastMouseY = -1;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
const float PI = 3.14159265359f;

bool displayMasterball = false;
bool displayPokeball = false;
bool displayGreatball = false;
bool displayBeastball = false;
bool pokeballWiggling = false;
float diglettScale = 1.0f;
float scaleIncrement = 0.1f;
float radius = 3.0f;
float ballscale = 1.0f;
float ballIncrement = 0.1f;
bool increasing = true;
bool sphereVisible = true;
bool sphereTopVisible = true;
float rotate = 0.0f;



// Essas constantes definem as cores utilizadas na renderização dos objetos, como preto, branco, vermelho, verde, etc.
const GLfloat black[] = { 0.0f,0.0f,0.0f,1.0f };
const GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat purple[] = { 0.4f, 0.0f, 0.6f, 1.0f };
const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat violeum[] = { 0.63f, 0.36f, 0.49f, 1.0f };
const GLfloat brown[] = { 0.611f, 0.352f, 0.133f, 1.0f };
const GLfloat pink[] = { 0.937f,0.733f,0.823f,1.0f };
const GLfloat shininess[] = { 100.0 };
const GLfloat blue_beastball[] = { 0.203f, 0.078f, 0.921f, 1.0f };
const GLfloat lightblue[] = { 0.078f,0.921f,0.921f,1.0f };
const GLfloat yellow[] = { 0.988f, 0.941f,0.011f,1.0f };

const GLfloat luz[] = { 2.0, 4.0, 6.0,0.5 };
const GLfloat light_diffuse[] = { 1.3, 1.3, 1.3, 1.3 };


GLuint grass,rocks;

// Função para carregar texturas
GLuint loadText(const char* filename)
{
	GLuint texture;
	GLint width, height, total_bytes;
	unsigned char* data;

	FILE* file;
	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	fseek(file, 0x0012, SEEK_SET);
	fread(&width, sizeof(int), 1, file);
	fread(&height, sizeof(int), 1, file);
	fseek(file, 54, SEEK_SET);

	data = (unsigned char*)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);


	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}

// Função para aplicar texturas às suas variáveis
void initText()
{
	grass = loadText("grass.bmp");
	rocks = loadText("rocks.bmp");
}

//Funçao que desenha estrelas
void drawStars() {
	glPointSize(1.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < 1000; ++i) {
		float x = static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f;
		float y = static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f;
		float z = static_cast<float>(rand()) / RAND_MAX * 100.0f - 50.0f;
		glVertex3f(x, y, z);
	}
	glEnd();
}

// Função que lida com eventos de teclado
void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'w':
		cameraZ -= 0.3f;
		break;
	case 's':
		cameraZ += 0.3f;
		break;
	case 'a':
		cameraX -= 0.3f;
		break;
	case 'd':
		cameraX += 0.3f;
		break;
	case 'e':
		cameraY += 0.3f;
		break;
	case 'q':
		cameraY -= 0.3f;
		break;
	case '1':
		displayPokeball = true;
		displayGreatball = false;
		displayMasterball = false;
		displayBeastball = false;
		break;
	case '2':
		displayPokeball = false;
		displayGreatball = true;
		displayMasterball = false;
		displayBeastball = false;
		break;
	case '3':
		displayPokeball = false;
		displayGreatball = false;
		displayMasterball = true;
		displayBeastball = false;
		break;
	case '4':
		displayPokeball = false;
		displayGreatball = false;
		displayMasterball = false;
		displayBeastball = true;
		break;
	case '9':
		pokeballWiggling = true;
		break;
	case 27: // tecla ESC  
		exit(0);
		break;
	}
	glutPostRedisplay();
}



// Função que lida com eventos de clique do mouse
void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		lastMouseX = x;
		LastMouseY = y;
	}
}

// Função que lida com eventos de movimento do mouse
void motion(int x, int y) {
	if (lastMouseX != -1 && LastMouseY != -1) {
		int deltaX = x - lastMouseX;
		int deltaY = y - LastMouseY;
		cameraAngleX += deltaY * 0.1f;
		cameraAngleY += deltaX * 0.1f;
		glutPostRedisplay();
	}
	lastMouseX = x;
	LastMouseY = y;
}


// Funçao que desenha um M na masterball
void drawM() {
	
	glPushMatrix();
	glTranslatef(0.0f,2.2f,4.4f);
	glScalef(4.0f, 4.0f, 4.0f);
	// Draw the left vertical stroke of the "M"
	glBegin(GL_POLYGON);
	glVertex3f(-0.4f, 0.0f,0.0f);
	glVertex3f(-0.3f, 0.0f,0.0f);
	glVertex3f(-0.1f, 0.4f,-0.2f);
	glVertex3f(-0.2f, 0.4f,-0.2f);
	glEnd();

	// Draw the middle diagonal stroke of the "M"
	glBegin(GL_QUADS);
	glVertex3f(-0.1f, 0.4f, -0.2f);
	glVertex3f(0.0f,0.27f,-0.15f);
	glVertex3f(0.0f, 0.15f,-0.05f);
	glVertex3f(-0.15f, 0.3f,-0.15f);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glVertex3f(0.0f, 0.27f, -0.15f);
	glVertex3f(0.0f, 0.15f, -0.05f);
	glVertex3f(0.1f,0.4f,-0.2f);
	glVertex3f(0.15f,0.3f,-0.15f);
	glEnd();

	// Draw the right vertical stroke of the "M"
	glBegin(GL_POLYGON);
	glVertex3f(0.4f, 0.0f,0.0f);
	glVertex3f(0.3f, 0.0f,0.0f);
	glVertex3f(0.1f, 0.4f,-0.2f);
	glVertex3f(0.2f, 0.4f,-0.2f);
	glEnd();

	glPopMatrix();
}


// Função para desenhar uma parte de uma esfera (capa esférica)
void drawSphericalCap(float radius, float height,float length, int slices, int stacks) {
	// Calcula o tamanho do passo angular em phi e theta
	float angleStep = 2.0f * PI / slices;
	float stackStep = PI / stacks;
	float phi, theta;

	for (int i = 0; i < stacks / length; i++) {
		phi = i * stackStep;
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= slices; j++) {
			theta = j * angleStep;
			float x = radius * sin(phi) * cos(theta);
			float y = radius * sin(phi) * sin(theta);
			float z = height + radius * cos(phi);
			glVertex3f(x, y, z);
			float xNext = radius * sin(phi + stackStep) * cos(theta);
			float yNext = radius * sin(phi + stackStep) * sin(theta);
			float zNext = height + radius * cos(phi + stackStep);
			glVertex3f(xNext, yNext, zNext);
		}
		glEnd();
	}
}


// Função para desenhar uma tampa genérica
void DrawLid(float sphereRadius, float circleRadius, int numPoints){
	float angleIncrement = (2 * PI) / numPoints;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numPoints; ++i) {
		float angle = i * angleIncrement;
		float x = circleRadius * cos(angle);
		float y = circleRadius * sin(angle);
		float z = sqrt(sphereRadius * sphereRadius - x * x - y * y);
		glVertex3f(x, y, z);
	}
	glEnd();
}

// Função para desenhar um tronco de cone sem textura
void Cone(float baseRadius, float topRadius, float height, int slices)
{
	glBegin(GL_TRIANGLE_STRIP);
	float angle = 0.0f;
	float angleIncrement = 2.0f * PI / slices;

	for (int i = 0; i <= slices; ++i)
	{
		float x1 = baseRadius * std::cos(angle);
		float y1 = baseRadius * std::sin(angle);
		float x2 = topRadius * std::cos(angle);
		float y2 = topRadius * std::sin(angle);

		glVertex3f(x1, y1, 0.0f);
		glVertex3f(x2, y2, height);

		angle += angleIncrement;
	}
	glEnd();

	// Draw the bottom base
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
	for (int i = 0; i <= slices; ++i)
	{
		float x = baseRadius * std::cos(angle);
		float y = baseRadius * std::sin(angle);
		glVertex3f(x, y, 0.0f);
		angle += angleIncrement;
	}
	glEnd();

	// Draw the top base
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, height);
	angle = 0.0f;
	for (int i = 0; i <= slices; ++i)
	{
		float x = topRadius * std::cos(angle);
		float y = topRadius * std::sin(angle);
		glVertex3f(x, y, height);
		angle += angleIncrement;
	}
	glEnd();
}

// Função para desenhar um tronco com cone
void ConeT(float baseRadius, float topRadius, float height, int slices)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, rocks);

	glBegin(GL_TRIANGLE_STRIP);
	float angle = 0.0f;
	float angleIncrement = 2.0f * PI / slices;

	for (int i = 0; i <= slices; ++i)
	{
		float x1 = baseRadius * std::cos(angle);
		float y1 = baseRadius * std::sin(angle);
		float x2 = topRadius * std::cos(angle);
		float y2 = topRadius * std::sin(angle);

		glTexCoord2f((float)i / slices, 0.0f);
		glVertex3f(x1, y1, 0.0f);

		glTexCoord2f((float)i / slices, 1.0f);
		glVertex3f(x2, y2, height);

		angle += angleIncrement;
	}
	glEnd();

	// Draw the bottom base
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5f, 0.5f); // Center of texture
	glVertex3f(0.0f, 0.0f, 0.0f);
	angle = 0.0f;
	for (int i = 0; i <= slices; ++i)
	{
		float x = baseRadius * std::cos(angle);
		float y = baseRadius * std::sin(angle);
		glTexCoord2f(x * 0.5f / baseRadius + 0.5f, y * 0.5f / baseRadius + 0.5f);
		glVertex3f(x, y, 0.0f);
		angle += angleIncrement;
	}
	glEnd();

	// Draw the top base
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5f, 0.5f); // Center of texture
	glVertex3f(0.0f, 0.0f, height);
	angle = 0.0f;
	for (int i = 0; i <= slices; ++i)
	{
		float x = topRadius * std::cos(angle);
		float y = topRadius * std::sin(angle);
		glTexCoord2f(x * 0.5f / topRadius + 0.5f, y * 0.5f / topRadius + 0.5f);
		glVertex3f(x, y, height);
		angle += angleIncrement;
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

// Função para desenhar uma tampa de cilindro
void DrawCylinderLid(float radius, float height, int slices, int stacks) {
	float stackHeight = height / stacks;
	float stackRadius = radius / stacks;
	for (int i = 0; i < stacks; ++i) {
		float y1 = i * stackHeight - height / 2;
		float y2 = (i + 1) * stackHeight - height / 2;

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= slices; ++j) {
			float angle = 2.0f * PI * static_cast<float>(j) / slices;
			float x = radius * cos(angle);
			float z = radius * sin(angle);

			glNormal3f(cos(angle), 0.0f, sin(angle));
			glVertex3f(x, y1, z);
			glVertex3f(x, y2, z);
		}
		glEnd();
	}

	
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f, -1.0f, 0.0f); 
	glVertex3f(0.0f, -height / 2.0f, 0.0f); 
	for (int j = 0; j <= slices; ++j) {
		float theta = 2.0f * PI * static_cast<float>(j) / slices;
		float x = radius * cos(theta);
		float z = radius * sin(theta);

		glNormal3f(0.0f, -1.0f, 0.0f); 
		glVertex3f(x, -height / 2.0f, z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f, 1.0f, 0.0f); 
	glVertex3f(0.0f, height / 2.0f, 0.0f); 
	for (int j = slices; j >= 0; --j) {
		float theta = 2.0f * PI * static_cast<float>(j) / slices;
		float x = radius * cos(theta);
		float z = radius * sin(theta);

		glNormal3f(0.0f, 1.0f, 0.0f); 
		glVertex3f(x, height / 2.0f, z);
	}
	glEnd();
}

// Função que desenha um retângulo para servir como chão e vincula a textura de grama para ela
void drawRectangle(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, grass);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, -5.0f, 10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, -5.0f, -10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, -5.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, -5.0f, 10.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

// Função para desenhar um cilindro
void DrawCylinder(float radius, float height, int slices, int stacks) {
	float stackHeight = height / stacks;
	float stackRadius = radius / stacks;

	for (int i = 0; i < stacks; ++i) {
		float y1 = i * stackHeight - height / 2;
		float y2 = (i + 1) * stackHeight - height / 2;

		// Inicia o desenho de um conjunto de quadriláteros
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= slices; ++j) {
			// Calcula o ângulo correspondente ao segmento atual
			float angle = 2.0f * PI * static_cast<float>(j) / slices;

			// Calcula as coordenadas x e z usando trigonometria
			float x = radius * cos(angle);
			float z = radius * sin(angle);

			// Define a normal para a superfície do cilindro
			glNormal3f(cos(angle), 0.0f, sin(angle));

			// Define os vértices para formar um quadrilátero no cilindro
			glVertex3f(x, y1, z);
			glVertex3f(x, y2, z);
		}
		// Finaliza o desenho do conjunto de quadriláteros
		glEnd();
	}
}


// Função para desenhar metade de uma esfera (parte superior)
void DrawHalfSphereTop(float radius, int slices, int stacks) {
	// Loop para percorrer as stacks (metade superior da esfera)
	for (int i = 0; i < stacks / 2; ++i) {
		// Calcula os valores das latitudes atual e seguinte
		float lat0 = PI * ( 0.5f + static_cast<float>(i) / stacks);
		float lat1 = PI * ( 0.5f + static_cast<float>(i + 1) / stacks);
		// Calcula os valores dos senos e cossenos das latitudes
		float y0 = sin(lat0);
		float y1 = sin(lat1);
		float r0 = cos(lat0);
		float r1 = cos(lat1);

		// Inicia um novo conjunto de triângulos com triângulos em forma de faixa
		glBegin(GL_TRIANGLE_STRIP);
		// Loop para percorrer os slices (longitude)
		for (int j = 0; j <= slices; ++j) {
			// Calcula o valor da longitude atual
			float lng = 2 * PI * static_cast<float>(j) / slices;
			// Calcula as coordenadas x, y, z do ponto na superfície da esfera
			float x = cos(lng);
			float z = sin(lng);
			// Define as normais e vértices para os dois pontos da faixa
			glNormal3f(x * r0, y0, z * r0);
			glVertex3f(x * r0 * radius, y0 * radius, z * r0 * radius);

			glNormal3f(x * r1, y1, z * r1);
			glVertex3f(x * r1 * radius, y1 * radius, z * r1 * radius);
		}
		glEnd();
	}
}

// Função para desenhar metade de uma esfera (parte inferior)
void DrawHalfSphereBottom(float radius, int slices, int stacks) {
	// Loop para percorrer as stacks (metade inferior da esfera)
	for (int i = 0; i < stacks / 2 ; ++i) {
		// Calcula os valores das latitudes atual e seguinte
		float lat0 = PI * (-0.5f + static_cast<float>(i) / stacks);
		float lat1 = PI * (-0.5f + static_cast<float>(i + 1) / stacks);
		// Calcula os valores dos senos e cossenos das latitudes
		float y0 = sin(lat0);
		float y1 = sin(lat1);
		float r0 = cos(lat0);
		float r1 = cos(lat1);

		// Inicia um novo conjunto de triângulos com triângulos em forma de faixa
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= slices; ++j) {
			float lng = 2 * PI * static_cast<float>(j) / slices;
			float x = cos(lng);
			float z = sin(lng);

			glNormal3f(x * r0, y0, z * r0);
			glVertex3f(x * r0 * radius, y0 * radius, z * r0 * radius);

			glNormal3f(x * r1, y1, z * r1);
			glVertex3f(x * r1 * radius, y1 * radius, z * r1 * radius);
		}
		glEnd();
	}
}

// Função para fazer a animação de abrir a pokebola
void PokeballanimationOpening(float radius, int slices, int stacks) {

	for (int i = 0; i < stacks / 2; ++i) {
		float lat0 = PI * (0.5f + static_cast<float>(i) / stacks);
		float lat1 = PI * (0.5f + static_cast<float>(i + 1) / stacks);
		float y0 = sin(lat0);
		float y1 = sin(lat1);
		float r0 = cos(lat0);
		float r1 = cos(lat1);

		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= slices; ++j) {
			float lng = 2 * PI * static_cast<float>(j) / slices;
			float x = cos(lng);
			float z = sin(lng);

			glNormal3f(x * r0, y0, z * r0);
			glVertex3f(x * r0 * radius, y0 * radius, z * r0 * radius + 5.0f);

			glNormal3f(x * r1, y1, z * r1);
			glVertex3f(x * r1 * radius, y1 * radius, z * r1 * radius + 5.0f);
		}
		glEnd();
	}
	glTranslatef(0.0f, 0.0f, 5.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	DrawCylinder(5.01f, 1.0f, 50, 40);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	drawSphericalCap(5.0f, 0.01f, 8.35f, 50, 50);


	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glTranslatef(0.0f, 0.0f, 0.05f);
	for (float i = 0; i <= 1.4f; i += 0.002) {
		DrawLid(5.0f, 1.4f - i, 100);
	}

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glTranslatef(0.0f, 0.0f, 0.01f);
	for (float i = 0; i <= 1.0f; i += 0.002) {
		DrawLid(5.0f, 1.0f - i, 100);
	}

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glRotatef(90.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 4.90f, 0.0f);
	DrawCylinderLid(0.7f, 0.3f, 100, 100);
	glPopMatrix();
}

// Função para desenhar o Diglett utilizando os mesmos codigos da pokebola
void drawDiglett() {
	// Draw Diglett

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, brown);
	DrawHalfSphereTop(2.0f, 50, 50);
	glPushMatrix();
	glTranslatef(0.0f, -1.5f, 0.0f);
	DrawCylinder(2.0f, 3.0f, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pink);
	glTranslatef(0.0f, -0.5f, 2.0f);
	glRotatef(90.0, 0.0f, 0.0f, 1.0f);
	DrawCylinder(0.5f, 0.5f, 50, 50);
	glTranslatef(0.0f, 0.25f, 0.0f);
	DrawHalfSphereTop(0.5f, 50, 50);
	glRotatef(180.0f, 0.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.5f, 0.0f);
	DrawHalfSphereTop(0.5f, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glTranslatef(0.4f, 0.8f, 1.9f);
	DrawCylinder(0.1f, 0.4f, 50, 50);
	glTranslatef(0.0f, 0.2f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	DrawHalfSphereTop(0.1f, 50, 50);
	glTranslatef(0.0f, -0.4f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	DrawHalfSphereBottom(0.1f, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glTranslatef(-0.4f, 0.8f, 1.9f);
	DrawCylinder(0.1f, 0.4f, 50, 50);
	glTranslatef(0.0f, 0.2f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	DrawHalfSphereTop(0.1f, 50, 50);
	glTranslatef(0.0f, -0.4f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	DrawHalfSphereBottom(0.1f, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -3.0f, 0.0f);
	glRotatef(90, -1.0f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	ConeT(2.5f, 2.3f, 0.5, 50);
	glPopMatrix();
	
}

// Função que cria a parte inferior da pokebola, já que ela se repete nas maiorias
void CreateSphereBottom() {

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	DrawHalfSphereBottom(5, 50, 50);


	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	DrawCylinder(5.01f, 1.0f, 50, 40);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	drawSphericalCap(5.0f, 0.01f, 8.35f, 50, 50);


	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glTranslatef(0.0f, 0.0f, 0.05f);
	for (float i = 0;i <= 1.4f;i += 0.002) {
		DrawLid(5.0f, 1.4f - i, 100);
	}

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glTranslatef(0.0f, 0.0f, 0.01f);
	for (float i = 0;i <= 1.0f;i += 0.002) {
		DrawLid(5.0f, 1.0f - i, 100);
	}

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glRotatef(90.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 4.90f, 0.0f);
	DrawCylinderLid(0.7f, 0.3f, 100, 100);
	glPopMatrix();
}

// Função que faz a animação da bola
void pokeballAnimation() {
	if (pokeballWiggling) {
		glTranslatef(0.0f, 0.0f, -10.0f);
		if (sphereTopVisible) {
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glMaterialfv(GL_FRONT, GL_SPECULAR, red);
			glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
			glPushMatrix();
			glRotatef(rotate, -1.0f, 0.0f, 0.0f);
			PokeballanimationOpening(5.0f, 50, 50);
			glPopMatrix();
		}
		glTranslatef(0.0f, 0.0f, 5.0f);
		if (sphereVisible) {
			DrawHalfSphereBottom(5.0f,50,50);
		}
		glPushMatrix();
		glScalef(ballscale, ballscale, ballscale);
		glutSolidSphere(radius, 50, 25);
		glPopMatrix();

		glPushMatrix();
		glScalef(diglettScale, diglettScale, diglettScale);

		drawDiglett();
		glPopMatrix();

		rotate += 5.0f;
		if (rotate >= 40.0f) {
			if (rotate == 90.0f) {
				sphereTopVisible = false;
			}
			if (increasing) {
				ballscale += ballIncrement;
				if (ballscale >= 3.0f) {
					increasing = false;
				}
			}
			else {
				if (ballscale > 1.0f) {
					ballscale -= ballIncrement;
				}
				if (ballscale <= 2.8f && diglettScale <= 2.0f) {
					sphereVisible = false;
					diglettScale += scaleIncrement;
				}
			}
		}

		glTranslatef(0.0f, -1.0f, 0.0f);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
		drawRectangle();
	}
}

void display() {
	// Limpa o buffer de cores e o buffer de profundidade
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Define a matriz atual como a matriz de visualização/modelo e a redefine para a identidade
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Aplica rotação à cena com base nos ângulos de rotação da câmera
	glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
	glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
	
	// Define a posição e a orientação da câmera usando gluLookAt
	gluLookAt(cameraX, cameraY, cameraZ, cameraX, cameraY, cameraZ - 0.5f, 0.0f, 0.5f, 0.0f);


	// Configuração da iluminação
	glEnable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	// Ativa a iluminação, define a posição da luz e suas propriedades difusas
	glLightfv(GL_LIGHT0,GL_POSITION, luz);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glEnable(GL_LIGHT0);

	// Define o material e desenha as estrelas
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	drawStars();

	// Desenha a Pokeball
	if (displayPokeball) {
		if (pokeballWiggling) {
			pokeballAnimation();
		}
		else {
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glMaterialfv(GL_FRONT, GL_SPECULAR, red);
			glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
			DrawHalfSphereTop(5, 50, 50);

			CreateSphereBottom();
			
			glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
			drawRectangle();
		}
	}

	//Desenha a Greatball
	if (displayGreatball) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
		glMaterialfv(GL_FRONT, GL_SPECULAR, blue);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		DrawHalfSphereTop(5, 50, 50);

		CreateSphereBottom();

		// Desenha os cilindros vermelhos adicionais
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
		glRotatef(45.0, 0.0f, 0.0f, 1.0f);
		glTranslatef(2.5f, 0.0f, 1.0f);
		DrawCylinderLid(3.0f, 1.0f, 100, 100);
		glPopMatrix();

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
		glRotatef(45.0, 0.0f, 0.0f, -1.0f);
		glTranslatef(-2.5f, 0.0f, 1.0f);
		DrawCylinderLid(3.0f, 1.0f, 100, 100);
		glPopMatrix();

		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		drawRectangle();
	}

	// Desenha a Masterball
	if (displayMasterball) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purple);
		glMaterialfv(GL_FRONT, GL_SPECULAR, purple);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		DrawHalfSphereTop(5, 50, 50);


		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, violeum);
		glMaterialfv(GL_FRONT, GL_SPECULAR, violeum);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glPushMatrix();
		glRotatef(90.0, -1.0f, 0.8f, -0.1f);
		glTranslatef(0.0f, 0.0f, 0.03f);
		drawSphericalCap(5.0f, 0.01f, 5.7f, 50, 50);

		glRotatef(90.0, -1.0f, -0.8f, 0.5f);
		glTranslatef(0.0f, 0.0f, 0.03f);
		drawSphericalCap(5.0f, 0.01f, 5.7f, 50, 50);
		glPopMatrix();

		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);

		drawM();

		CreateSphereBottom();
		
		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		drawRectangle();
	}

	// Desenha a Beastball
	if (displayBeastball) {
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue_beastball);
		glMaterialfv(GL_FRONT, GL_SPECULAR, blue_beastball);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		DrawHalfSphereTop(5.0f, 50, 50);
		DrawHalfSphereBottom(5.0f, 50, 50);

		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightblue);
		glMaterialfv(GL_FRONT, GL_SPECULAR, lightblue);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		DrawCylinder(5.01f, 0.6f, 50, 40);
		glPushMatrix();
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		DrawCylinder(5.01f, 0.6f, 50, 40);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.05f);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightblue);
		drawSphericalCap(5.0f, 0.01f, 8.35f, 50, 50);


		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
		glTranslatef(0.0f, 0.0f, 0.05f);
		for (float i = 0;i <= 1.6f;i += 0.002) {
			DrawLid(5.0f, 1.6f - i, 100);
		}

		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightblue);
		glTranslatef(0.0f, 0.0f, 0.01f);
		for (float i = 0;i <= 1.2f;i += 0.002) {
			DrawLid(5.0f, 1.2f - i, 100);
		}

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glRotatef(90.0, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 4.90f, 0.0f);
		DrawCylinderLid(0.8f, 0.5f, 100, 100);
		glPopMatrix();
		glPopMatrix();

		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);

		glPushMatrix();
		glRotatef(45.0, 0.0f, 0.0f, 1.0f);
		glTranslatef(2.5f, 0.0f, 1.0f);
		DrawCylinderLid(3.3f, 1.0f, 100, 100);
		glPopMatrix();

		glPushMatrix();
		glRotatef(45.0, 0.0f, 0.0f, -1.0f);
		glTranslatef(-2.5f, 0.0f, 1.0f);
		DrawCylinderLid(3.3f, 1.0f, 100, 100);
		glPopMatrix();

		glPushMatrix();
		glRotatef(45.0, 0.0f, 0.0f, 1.0f);
		glTranslatef(-2.5f, 0.0f, 1.0f);
		DrawCylinderLid(3.3f, 1.0f, 100, 100);
		glPopMatrix();

		glPushMatrix();
		glRotatef(45.0, 0.0f, 0.0f, -1.0f);
		glTranslatef(2.5f, 0.0f, 1.0f);
		DrawCylinderLid(3.3f, 1.0f, 100, 100);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.9f);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightblue);
		Cone(5.0f, 4.9f, 0.4f, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 4.05f);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightblue);
		Cone(2.6f, 2.5f, 0.35f, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 3.0f);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightblue);
		Cone(3.9f, 3.8f, 0.35f, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -4.05f);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightblue);
		Cone(2.6f, 2.5f, -0.35f, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -3.0f);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightblue);
		Cone(3.9f, 3.8f, -0.35f, 50);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -1.4f);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightblue);
		Cone(4.9f, 4.8f, -0.35f, 50);
		glPopMatrix();

		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
		drawRectangle();
	}

	
	// Limpa o pipeline gráfico, troca o buffer de leitura e escrita para exibir a cena
	glFlush();
	glutSwapBuffers();
}

// Função reshape ajusta a janela de exibição (viewport) para se adequar às dimensões da janela atual.
void reshape(int width, int height) {
	// Define a janela de exibição (viewport) como o tamanho da janela atual
	glViewport(0, 0, width, height);
	// Define a matriz atual como a matriz de projeção e redefine para a identidade
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Define a perspectiva da cena usando gluPerspective
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

int main(int argc, char** argv)
{
	int Window,i = 0;
	printf("Reinitializing FreeGLUT\n");
	// Inicializa o GLUT e passa os argumentos de linha de comando
	glutInit(&argc, argv);
	// Define o modo de exibição do GLUT com buffers duplos, RGBA, profundidade e canal alfa
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	// Define o tamanho inicial da janela e a posição inicial da janela
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	Window = glutCreateWindow("Masteball");
	if (!Window) {
		printf("Failed to create window\n");
		return -1;
	}
	// Define a cor de fundo da janela como preto
	glClearColor(0.0, 0.0, 0.0, 1.0);
	initText();
	// Habilita o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	// Define as funções de callback para os eventos do GLUT
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	// Entra no loop principal do GLUT
	glutMainLoop();
	return 0;
}