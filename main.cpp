#include <stdio.h>      /* printf */
#include <math.h> 
#include <GL/glut.h>
#include "RgbImage.h"

#define PI 3.141592654

using namespace std;

//cabeçalho das funções
void desenhaPernas(); //desenha as pernas 
void desenhaCorpo(); //desenha o corpo 
void drawHead(void);
void drawHand(int, int);
void drawBracos(int, int);



char* filenameTexMetal1 = (char*)"./metalTexture1.bmp";
char* filenameTexMetal2 = (char*)"./metalTexture2.bmp";
char* filenameTexMetal3 = (char*)"./metalTexture3.bmp";
char* filenameTexMetal4 = (char*)"./metalTexture4.bmp";

char* filenameTexFront = (char*)"./texturas/plano.bmp";
char* filenameTexSide = (char*)"./texturas/lateral.bmp";
char* filenameTexTop = (char*)"./texturas/plano.bmp";
char* filenameTexFerrugem = (char*)"./texturas/texturaFerrugem.bmp";

GLuint _textureIdMetal1;
GLuint _textureIdMetal2;
GLuint _textureIdMetal3;
GLuint _textureIdMetal4;
GLuint _textureIdSphere;
GLuint _textureIdCylinder;
GLuint _textureIdPlatformTop;
GLuint _textureIdPlatformFront;
GLuint _textureIdPlatformSide;
GLuint _textureIdRoboFerrugem;

bool textureOn = true;

GLUquadric* quadSphere;
GLUquadric* quadCylinder;

float diameterCylinder = 0.3;
float diameterSphere = 0.4;
float sizeArm = 4.5;
float sizeForearm = 3.0;
float sizeHand = 2.0;
float sizeClampPart = 1.0;
float lenghtPlatform = 10.0;
float heightPlatform = 0.4;
float diameterBase = 2.0;
float heightBase = 0.5;

// Parâmetros da cabeça
float widthHead = lenghtPlatform / 5;
float heightHead = lenghtPlatform / 9;
float depthHead = heightBase + 1;

// Parâmetros
float widthBody = lenghtPlatform / 2;
float heightBody = lenghtPlatform / 5;
float depthBody = heightBase + 2;


float eyeDistance = 35.0;
float eyeX;
float eyeY;
float eyeZ;
float viewAngleX = 0.0;
float viewAngleZ = 35.0;

float angleArm = 0.0;
float angleForearm = 90.0;
float angleHand = 0.0;
float angleClampZ = 0.0;
float angleClampY = 0.0;

// angulos do braço esquerda
float anguloMaoEsquerdaY = 0.0;
float anguloBracoEsquerdaY = 90.0;
float anguloAnteBracoEsquerdaY = 50.0;



// angulos do braço direito
float anguloMaoDireitoY = 0.0;
float anguloBracoDireitoY = 90.0;
float anguloAnteBracoDireitoY = 50.0;

GLfloat lightposition[4] = { 0.0f, 30.0f, 0.0f, 0.0 };
GLfloat luzAmbiente[4] = { 0.19, 0.19, 0.19, 0.0 };
GLfloat luzDifusa[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat luzEspecular[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat especularidade[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat semEspecularidade[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat especMaterialTexture = 50;
GLfloat especMaterialNoTexture = 60;

//defines light source (LIGHT0)
void initLighting(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Especifica que a cor de fundo da janela será preta
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Materials will follow current color
	glEnable(GL_COLOR_MATERIAL);
}

//makes the image into a texture, and returns the id of the texture
GLuint loadTexture(char* filename) {
	GLuint textureId;

	RgbImage theTexMap(filename); //Image with texture

	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);	//Tell OpenGL which texture to edit
												//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,	//Always GL_TEXTURE_2D
		0,						//0 for now
		GL_RGB,					//Format OpenGL uses for image
		theTexMap.GetNumCols(),	//Width 
		theTexMap.GetNumRows(),	//Height
		0,						//The border of the image
		GL_RGB,					//GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,		//GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		theTexMap.ImageData());	//The actual pixel data
	return textureId; //Returns the id of the texture
}

void initRendering(void) {

	quadSphere = gluNewQuadric();
	quadCylinder = gluNewQuadric();
	_textureIdMetal1 = loadTexture(filenameTexMetal1);
	_textureIdMetal2 = loadTexture(filenameTexMetal2);
	_textureIdMetal3 = loadTexture(filenameTexMetal3);
	_textureIdMetal4 = loadTexture(filenameTexMetal4);
	_textureIdCylinder = _textureIdMetal1;
	_textureIdSphere = _textureIdMetal1;
	_textureIdPlatformTop = loadTexture(filenameTexTop);
	
	_textureIdPlatformFront = loadTexture(filenameTexFront);
	_textureIdPlatformSide = loadTexture(filenameTexSide);

	_textureIdRoboFerrugem = loadTexture(filenameTexFerrugem);
}

void handleKeypress(unsigned char key, int x, int y) {
	//GLUT_
	switch (key) {
	case 27: //Escape key
		exit(0);
	// case 'a': //Decrease view angle x axis
	// 	if (viewAngleX > 0) viewAngleX -= 3;
	// 	glutPostRedisplay();
	// 	break;
	// case 's': //Increase view angle x axis
	// 	if (viewAngleX < 180) viewAngleX += 3;
	// 	glutPostRedisplay();
	// 	break;
	case 't': //Change metal texture
		textureOn = true;
		if (_textureIdSphere == _textureIdMetal1) {
			_textureIdSphere = _textureIdMetal2;
			_textureIdCylinder = _textureIdMetal2;
		}
		else if (_textureIdSphere == _textureIdMetal2) {
			_textureIdSphere = _textureIdMetal3;
			_textureIdCylinder = _textureIdMetal3;
		}
		else if (_textureIdSphere == _textureIdMetal3) {
			_textureIdSphere = _textureIdMetal4;
			_textureIdCylinder = _textureIdMetal4;
		}
		else if (_textureIdSphere == _textureIdMetal4) {
			_textureIdSphere = _textureIdMetal1;
			_textureIdCylinder = _textureIdMetal1;
		}
		glutPostRedisplay();
		break;
	case 'n': //no texture
		textureOn = false;
		glutPostRedisplay();
		break;
	case 'q': //incrementa angula do ombro esquerdo
		if (anguloBracoEsquerdaY > 0) anguloBracoEsquerdaY -= 3;
		glutPostRedisplay();
		break;
	case 'a': //decrementa angula do ombro esquerdo
		if (anguloBracoEsquerdaY < 90) anguloBracoEsquerdaY += 3;
		glutPostRedisplay();
		break;
	case 'w': //incrementa angula do andebraco esquerdo
		if (anguloAnteBracoEsquerdaY < 120) anguloAnteBracoEsquerdaY += 3;
		glutPostRedisplay();
		break;
	case 's': //decrementa angula do andebraco esquerdo
		if (anguloAnteBracoEsquerdaY > 50) anguloAnteBracoEsquerdaY -= 3;
		glutPostRedisplay();
		break;
			
	case 'e': //incrementa angula da mão esquerdo
		if (anguloMaoEsquerdaY < 20) anguloMaoEsquerdaY += 3;
		glutPostRedisplay();
		break;
	case 'd': //decrementa angula da mão esquerdo
		if (anguloMaoEsquerdaY > 0) anguloMaoEsquerdaY -= 3;
		glutPostRedisplay();
		break;


	case 'y': //incrementa angula do braço direito
		if (anguloBracoDireitoY > 0) anguloBracoDireitoY -= 3;
		glutPostRedisplay();
		break;
	case 'h': //decrementa angula do braço direito
		if (anguloBracoDireitoY < 90) anguloBracoDireitoY += 3;
		glutPostRedisplay();
		break;		
	case 'u': //Increase hand angle
		if (anguloAnteBracoDireitoY < 120) anguloAnteBracoDireitoY += 3;
		glutPostRedisplay();
		break;
	case 'j': //Decrease hand angle
		if (anguloAnteBracoDireitoY > 50) anguloAnteBracoDireitoY -= 3;
		glutPostRedisplay();
		break;
	case 'i': //incrementa angula da mão direita
		if (anguloMaoDireitoY < 20) anguloMaoDireitoY += 3;
		glutPostRedisplay();
		break;
	case 'k': //decrementa angula da mão direita
		if (anguloMaoDireitoY > 0) anguloMaoDireitoY -= 3;
		glutPostRedisplay();
		break;	

	

	case '1': //Increase arm angle
		angleArm += 3;
		if (angleArm >= 360) angleArm = 0;
		glutPostRedisplay();
		break;
	case '2': //Decrease arm angle
		angleArm -= 3;
		if (angleArm <= 0) angleArm = 360;
		glutPostRedisplay();
		break;
	case '3': //Increase forearm angle
		if (angleForearm < 90) angleForearm += 3;
		glutPostRedisplay();
		break;
	case '4': //Decrease forearm angle
		if (angleForearm > -90) angleForearm -= 3;
		glutPostRedisplay();
		break;
	
	case '9': //Increase clamp angle z axis
		angleClampZ += 3;
		if (angleClampZ >= 360) angleClampZ = 0;
		glutPostRedisplay();
		break;
	case '0': //Decrease clamp angle z axis
		angleClampZ -= 3;
		if (angleClampZ <= 0) angleClampZ = 360;
		glutPostRedisplay();
		break;
	}
	//cout << "Arm (" << angleArm << "); Forearm (" << angleForearm << "); Hand (" << angleHand << "); ";
	//cout << "Clamp Y (" << angleClampY << "); Clamp Z (" << angleClampZ << ") \n";
	//cout << "Eye X (" << eyeX << "); Eye Y (" << eyeY << "); Eye Z (" << eyeZ << ") \n";
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (eyeDistance < 38) eyeDistance += 1;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (eyeDistance > 0) eyeDistance -= 1;
		}
	glutPostRedisplay();
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
}


//monta o a base da animação 
void drawCube(float lenghtX, float lenghtY, float height, int pIntTextura = 1)
{
	int textFront = _textureIdPlatformTop;
	int textSide = _textureIdPlatformSide;
	int textTop = _textureIdPlatformSide;

	//altera a textura do robo
	if (pIntTextura == 2){
		textFront = _textureIdRoboFerrugem;
		textSide = _textureIdRoboFerrugem;
		textTop = _textureIdRoboFerrugem;
	}

	//altera a textura da boca do robô
	if (pIntTextura == 3){
		textFront = _textureIdRoboFerrugem;
		textSide = _textureIdRoboFerrugem;
		textTop = _textureIdRoboFerrugem;
	}

	glBindTexture(GL_TEXTURE_2D, textFront);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);			// Face posterior
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, -height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, -height);
	glEnd();
	glBegin(GL_QUADS);			// Face frontal
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, height);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textSide);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);			// Face lateral esquerda
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, -height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
	glEnd();
	glBegin(GL_QUADS);			// Face lateral direita
	glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, -height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, lenghtY, height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textTop);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);			// Face superior
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, lenghtY, height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, lenghtY, height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
	glEnd();
	glBegin(GL_QUADS);			// Face inferior
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, -lenghtY, -height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, -lenghtY, -height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
	glEnd();
}

void drawCylinder(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCone(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght * 30.0);
}

void drawDisk(float diameterInner, float diameterOuter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}

void drawSphere(float diameter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdSphere);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameter, 40.0, 40.0);
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	eyeX = eyeDistance * cos(viewAngleZ * PI / 180) * cos(viewAngleX * PI / 180);
	eyeY = eyeDistance * cos(viewAngleZ * PI / 180) * sin(viewAngleX * PI / 180);
	eyeZ = eyeDistance * sin(viewAngleZ * PI / 180);
	if (viewAngleZ < 90)
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	else
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// clears color and material parameters before drawing platform
	glColor3f(1.0f, 1.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, semEspecularidade);
	glMateriali(GL_FRONT, GL_SHININESS, 0);


	//// monta a base da animação
	drawCube(lenghtPlatform*1.2, lenghtPlatform*1.2, heightPlatform);
	//glPopMatrix();

	if (textureOn) {
		glColor3f(0.9f, 0.9f, 0.9f);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
		glMateriali(GL_FRONT, GL_SHININESS, especMaterialTexture);
	}
	else {
		glColor3f(0.8f, 0.9f, 1.0f);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
		glMateriali(GL_FRONT, GL_SHININESS, especMaterialNoTexture);
	}

	//monta as pernas	
	desenhaPernas();
	desenhaCorpo();
	drawBracos(anguloBracoEsquerdaY, anguloBracoDireitoY);
	drawHead();

	glutSwapBuffers();
}

void drawHead(void) {
	glTranslatef(0.0f, 0.0f, heightBase + 11.5);
	glPushMatrix();
	drawCylinder(diameterCylinder / 2, sizeArm / 5);

	//Cabeça
	glTranslatef(0.0f, 0.0f, heightBase + 1.7);
	drawCube(widthHead, heightHead, depthHead, 2);

	//Antena
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, heightBase + 1);
	drawCylinder(diameterCylinder / 3, sizeArm / 3);
	glTranslatef(0.0f, 0.0f, heightBase + 1);
	drawSphere(diameterSphere);
	glPopMatrix();

	//Olhos bocas
	glPushMatrix();
	glTranslatef(heightBase + 0.5, heightBase + 1, heightBase + 0.3);
	glRotatef(angleForearm, 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder + 0.2, sizeArm / 10);
	drawDisk(0, diameterCylinder + 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-(heightBase + 0.5), heightBase + 1, heightBase + 0.3);
	glRotatef(angleForearm, 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder + 0.2, sizeArm / 10);
	drawDisk(0, diameterCylinder + 0.2);
	glPopMatrix();

	glTranslatef(0.0f, heightBase + 0.8, -0.5);
	glRotatef(angleForearm, 1.0f, 0.0f, 0.0f);
	glPushMatrix();
	drawCube(widthHead / 2, heightHead / 2, depthHead / 10, 2);
	glPopMatrix();

	glPopMatrix();


}
void desenhaCorpo() {
	glPushMatrix();
	glTranslatef(0, 0, 8);  // aqui eu mudo o referencial para subir a bolinha   
	drawCube(3, 2, 4, 2);
	glPopMatrix();
}

void drawHand(int pIntAnguloMao, int pIntAnteBraco) {
	//draws the hand
	drawSphere(diameterSphere);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	drawCylinder(diameterCylinder, sizeHand);

	// move to clamp referential
	glTranslatef(0.0f, 0.0f, sizeHand + diameterSphere / 5);
	glRotatef(angleClampZ, 0.0f, 0.0f, 1.0f);

	//draws the clamp sphere
	drawSphere(diameterSphere);
	glTranslatef(0.0f, 0.0f, diameterSphere / 2);

	glPushMatrix();

	//draws top part of clamp
	glRotatef(pIntAnguloMao + 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	drawCone(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();
	glPushMatrix();

	//draws bottom part of clamp
	glRotatef(-pIntAnguloMao - 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);
	drawCone(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();

	// rotate to draw other two parts
	glRotatef(90, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	//draws top part of clamp
	glRotatef(pIntAnguloMao + 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	drawCone(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();

	//draws bottom part of clamp
	glRotatef(-pIntAnguloMao - 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);
	drawCone(diameterCylinder / 3, sizeClampPart);
}
void drawBracos(int pIntAngBracoEsquerdo, int pIntAngBracoDireito) {
	
	//Braço direito
	glPushMatrix();
	//draws the forearm
	glTranslatef(3.6, 0.0f, heightBase + 10);
	drawSphere(diameterSphere + 0.5);
	glTranslatef(0.0f, 0.0f, (diameterSphere) / 5);
	//glPushMatrix();
	glRotatef(-(pIntAngBracoDireito * 2), 1.0f, 0.0f, 0.0f);
	drawCylinder(diameterCylinder, sizeForearm);
	glTranslatef(0.0f, 0.0f, sizeForearm + diameterSphere / 15);
	glRotatef(anguloAnteBracoDireitoY, 1.0f, 0.0f, 0.0f);
	drawSphere(diameterSphere + 0.2);
	
	drawHand(anguloMaoDireitoY, anguloAnteBracoDireitoY);
	
	glPopMatrix();

	//Braço esquerdo
	glPushMatrix();
	//draws the forearm
	glTranslatef(-3.6, 0.0f, heightBase + 10);
	drawSphere(diameterSphere + 0.5);
	glTranslatef(0.0f, 0.0f, (diameterSphere) / 5);
	//glPushMatrix();
	//glRotatef(-(pIntAngBracoDireito * 2), 1.0f, 0.0f, 0.0f);
	glRotatef(-(pIntAngBracoEsquerdo * 2), 1.0f, 0.0f, 0.0f);

	drawCylinder(diameterCylinder, sizeForearm);
	glTranslatef(0.0f, 0.0f, sizeForearm + diameterSphere / 15);
	glRotatef(anguloAnteBracoEsquerdaY, 1.0f, 0.0f, 0.0f);
	drawSphere(diameterSphere + 0.2);
	
	drawHand(anguloMaoEsquerdaY, anguloAnteBracoEsquerdaY);
	
	glPopMatrix();

}


void desenhaPernas() {

	//monta perna esquerda
	glPushMatrix();
	glTranslatef(2, 0, heightBase*2.5);  // aqui eu mudo o referencial para subir a bolinha   
	drawSphere(diameterSphere);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5); // aqui o referencial é a bolinha
	glPushMatrix();
	glRotatef(95, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço 
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15); // aqui eu mudo o referencial para o andebraço
	drawSphere(diameterSphere / 3); //aqui eu crio a articulação
	glRotatef(40, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	glTranslatef(0.0f, 0.0f, diameterSphere / 5); // mudo o referencial para a articulação

	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)
	glPopMatrix(); // aqui eu termino e vou começar o outro braço ou garra
	glPushMatrix();

	glRotatef(-95, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15); // aqui eu mudo o referencial para o andebraço

	drawSphere(diameterSphere / 3); //aqui eu crio a articulação
	glRotatef(-40, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço

	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)
	glPopMatrix();
	glPushMatrix();

	glRotatef(95, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço
	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3); //aqui eu crio a articulação
	glRotatef(40, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço

	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)
	glPopMatrix(); // aqui eu termino e vou começar o outro braço ou garra

	glPushMatrix();
	glRotatef(-95, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço
	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3); //aqui eu crio a articulação
	glRotatef(-40, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço

	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)

	glPopMatrix();
	drawCylinder(diameterCylinder, sizeForearm);


	//monta perna direita
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2, 0, heightBase*2.5);  // aqui eu mudo o referencial para subir a bolinha   	
	drawSphere(diameterSphere);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5); // aqui o referencial é a bolinha
	glPushMatrix();
	glRotatef(95, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço

	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15); // aqui o referencial é a bolinha
	drawSphere(diameterSphere / 3);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5); // aqui o referencial é a bolinha
	glRotatef(40, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)

	glPopMatrix();
	glPushMatrix();
	glRotatef(-95, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço

	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15); // aqui o referencial é a bolinha
	drawSphere(diameterSphere / 3);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glRotatef(-40, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)

	glPopMatrix();
	glPushMatrix();
	glRotatef(95, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glRotatef(40, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)

	glPopMatrix();

	glPushMatrix();
	glRotatef(-95, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço

	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);

	drawSphere(diameterSphere / 3);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glRotatef(-40, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)

	glPopMatrix();
	drawCylinder(diameterCylinder, sizeForearm);
	glPopMatrix();	
}

void changeView(int key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	case GLUT_KEY_DOWN: //Increase view angle z axis
		if (viewAngleZ < 180) viewAngleZ += 3;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP: //Decrease view angle z axis
		if (viewAngleZ > 0) viewAngleZ -= 3;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT: //Decrease view angle x axis
		if (viewAngleX > 0) viewAngleX -= 3;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT: //Increase view angle x axis
		if (viewAngleX < 360) viewAngleX += 3;
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Robô Futurama");

	initLighting();
	initRendering();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(GerenciaMouse);
	glutReshapeFunc(handleResize);
	glutSpecialFunc(changeView);

	glutMainLoop();
	return 0;
}
