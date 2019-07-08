#include <stdio.h>      /* printf */
#include <math.h> 
#include <GL/glut.h>
#include "RgbImage.h"

#define PI 3.141592654

using namespace std;


//cabeçalho das funções
void desenhaPernas(); //desenha as pernas 
void desenhaCorpo(); //desenha o corpo 
void drawGarra();



char* filenameTexMetal1 = (char*)"./metalTexture1.bmp";
char* filenameTexMetal2 = (char*)"./metalTexture2.bmp";
char* filenameTexMetal3 = (char*)"./metalTexture3.bmp";
char* filenameTexMetal4 = (char*)"./metalTexture4.bmp";
char* filenameTexTop = (char*)"./woodTextureTop.bmp";
char* filenameTexFront = (char*)"./woodTextureFront.bmp";
char* filenameTexSide = (char*)"./woodTextureSide.bmp";

GLuint _textureIdMetal1;
GLuint _textureIdMetal2;
GLuint _textureIdMetal3;
GLuint _textureIdMetal4;
GLuint _textureIdSphere;
GLuint _textureIdCylinder;
GLuint _textureIdPlatformTop;
GLuint _textureIdPlatformFront;
GLuint _textureIdPlatformSide;

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

float eyeDistance = 30.0;
float eyeX;
float eyeY;
float eyeZ;
float viewAngleX = 0.0;
float viewAngleZ = 50.0;

float angleArm = 0.0;
float angleForearm = 90.0;
float angleHand = 0.0;
float angleClampZ = 0.0;
float angleClampY = 0.0;

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
}

void handleKeypress(unsigned char key, int x, int y) {
	//GLUT_
	switch (key) {
	case 27: //Escape key
		exit(0);
	case GLUT_KEY_UP: //Increase view angle z axis
		if (viewAngleZ < 180) viewAngleZ += 3;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN: //Decrease view angle z axis
		if (viewAngleZ > 0) viewAngleZ -= 3;
		glutPostRedisplay();
		break;
	case 'a': //Decrease view angle x axis
		if (viewAngleX > 0) viewAngleX -= 3;
		glutPostRedisplay();
		break;
	case 's': //Increase view angle x axis
		if (viewAngleX < 180) viewAngleX += 3;
		glutPostRedisplay();
		break;
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
	case '5': //Increase hand angle
		if (angleHand < 90) angleHand += 3;
		glutPostRedisplay();
		break;
	case '6': //Decrease hand angle
		if (angleHand > -90) angleHand -= 3;
		glutPostRedisplay();
		break;
	case '7': //Increase clamp angle y axis
		if (angleClampY < 60) angleClampY += 3;
		glutPostRedisplay();
		break;
	case '8': //Decrease clamp angle y axis
		if (angleClampY > 0) angleClampY -= 3;
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
			if (eyeDistance < 30) eyeDistance += 1;
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
void drawCube(float lenghtX, float lenghtY, float height)
{
	glBindTexture(GL_TEXTURE_2D, _textureIdPlatformFront);
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
	glBindTexture(GL_TEXTURE_2D, _textureIdPlatformSide);
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
	glBindTexture(GL_TEXTURE_2D, _textureIdPlatformTop);
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
	drawCube(lenghtPlatform, lenghtPlatform, heightPlatform);
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
	drawGarra();

	glutSwapBuffers();
}

void desenhaCorpo(){
	glPushMatrix();
	glTranslatef(0, 0, 8);  // aqui eu mudo o referencial para subir a bolinha   
	drawCube(3,2,4);
	glPopMatrix();
}

void desenhaPernas(){

	//monta perna esquerda
    glPushMatrix();
	glTranslatef(2, 0, heightBase*2.5);  // aqui eu mudo o referencial para subir a bolinha   
	drawSphere(diameterSphere);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5); // aqui o referencial é a bolinha
	glPushMatrix();
    glRotatef(angleClampY + 95, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço 
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15); // aqui eu mudo o referencial para o andebraço
	drawSphere(diameterSphere / 3); //aqui eu crio a articulação
    glRotatef(angleClampY + 40, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
    glTranslatef(0.0f, 0.0f, diameterSphere / 5); // mudo o referencial para a articulação
    
    drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)
    glPopMatrix(); // aqui eu termino e vou começar o outro braço ou garra
    glPushMatrix();

    glRotatef(-angleClampY - 95, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
    drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15); // aqui eu mudo o referencial para o andebraço

    drawSphere(diameterSphere / 3); //aqui eu crio a articulação
    glRotatef(-angleClampY - 40, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço

    glTranslatef(0.0f, 0.0f, diameterSphere / 5);
    drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)
	glPopMatrix(); 
	glPushMatrix();

	glRotatef(angleClampY + 95, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço
   	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3); //aqui eu crio a articulação
	glRotatef(angleClampY + 40, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço

	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)
	glPopMatrix(); // aqui eu termino e vou começar o outro braço ou garra
	
	glPushMatrix();
	glRotatef(-angleClampY - 95, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço
	drawCylinder(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3); //aqui eu crio a articulação
	glRotatef(-angleClampY - 40, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço

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
	glRotatef(angleClampY + 95, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço

	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15); // aqui o referencial é a bolinha
	drawSphere(diameterSphere / 3);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5); // aqui o referencial é a bolinha
	glRotatef(angleClampY + 40, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)

	glPopMatrix();
	glPushMatrix();
	glRotatef(-angleClampY - 95, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço

	drawCylinder(diameterCylinder / 3, sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15); // aqui o referencial é a bolinha
	drawSphere(diameterSphere / 3);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glRotatef(-angleClampY - 40, 1.0f, 0.0f, 0.0f); ///rotaciono antebraço
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)

	glPopMatrix();
	glPushMatrix();
	glRotatef(angleClampY + 95, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	drawCylinder(diameterCylinder / 3 , sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glRotatef(angleClampY + 40, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)

	glPopMatrix();
	
	glPushMatrix();
	glRotatef(-angleClampY - 95, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço

	drawCylinder(diameterCylinder / 3 , sizeClampPart); // aqui eu crio o antebraço
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);

	drawSphere(diameterSphere / 3);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glRotatef(-angleClampY - 40, 0.0f, 1.0f, 0.0f); ///rotaciono antebraço
	drawCone(diameterCylinder / 3, sizeClampPart);// crio a mão (garra)
	
	glPopMatrix();
	drawCylinder(diameterCylinder, sizeForearm);
	glPopMatrix();
	glutSwapBuffers();
}

void changeView(int key, int x, int y){
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
		if (viewAngleX < 180) viewAngleX += 3;
		glutPostRedisplay();
		break;
	}
}
void drawGarra (){
    //draws the base
	//drawCylinder(diameterBase, heightBase);
	// glTranslatef(0.0f, 0.0f, heightBase);
	// drawDisk(diameterCylinder, diameterBase);

	// // move to arm referential
	// //glRotatef(angleArm, 0.0f, 0.0f, 1.0f);

	//  //draws the arm
	// drawCylinder(diameterCylinder, sizeArm);
    

	// // move to forearm referential
	// glTranslatef(0.0f, 0.0f, sizeArm + diameterSphere / 5);
	// glRotatef(angleForearm, 0.0f, 1.0f, 0.0f);

	// // //draws the forearm
	// drawSphere(diameterSphere);
	// glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	// drawCylinder(diameterCylinder, sizeForearm);

	// //move to hand referential
	// glTranslatef(0.0f, 0.0f, sizeForearm + diameterSphere / 5);
	// glRotatef(angleHand, 0.0f, 1.0f, 0.0f);

	// //draws the hand
	// drawSphere(diameterSphere);
	// glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	// drawCylinder(diameterCylinder, sizeHand);

	// // move to clamp referential
	// glTranslatef(0.0f, 0.0f, sizeHand + diameterSphere / 5);
	// glRotatef(angleClampZ, 0.0f, 0.0f, 1.0f);

	// //draws the clamp sphere
	// drawSphere(diameterSphere);
	// glTranslatef(0.0f, 0.0f, diameterSphere / 2);

	// glPushMatrix();


    // /////GARRASSSSSS
	// //draws top part of clamp
	// glRotatef(angleClampY + 60, 0.0f, 1.0f, 0.0f);

	// drawCylinder(diameterCylinder / 3, sizeClampPart);
	// glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// drawSphere(diameterSphere / 3);

	// glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// glRotatef(-60, 0.0f, 1.0f, 0.0f);

	// drawCylinder(diameterCylinder / 3, sizeClampPart);
	// glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// drawSphere(diameterSphere / 3);

	// glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// glRotatef(-60, 0.0f, 1.0f, 0.0f);
	// drawCone(diameterCylinder / 3, sizeClampPart);

	// glPopMatrix();
	// glPushMatrix();

	// //draws bottom part of clamp
	// glRotatef(-angleClampY - 60, 0.0f, 1.0f, 0.0f);

	// drawCylinder(diameterCylinder / 3, sizeClampPart);
	// glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// drawSphere(diameterSphere / 3);

	// glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// glRotatef(60, 0.0f, 1.0f, 0.0f);

	// drawCylinder(diameterCylinder / 3, sizeClampPart);
	// glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// drawSphere(diameterSphere / 3);

	// glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// glRotatef(60, 0.0f, 1.0f, 0.0f);
	// drawCone(diameterCylinder / 3, sizeClampPart);

	// glPopMatrix();

	// // rotate to draw other two parts
	// glRotatef(90, 0.0f, 0.0f, 1.0f);

	// glPushMatrix();

	// //draws top part of clamp
	// glRotatef(angleClampY + 60, 0.0f, 1.0f, 0.0f);

	// drawCylinder(diameterCylinder / 3, sizeClampPart);
	// glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// drawSphere(diameterSphere / 3);

	// glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// glRotatef(-60, 0.0f, 1.0f, 0.0f);

	// drawCylinder(diameterCylinder / 3, sizeClampPart);
	// glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// drawSphere(diameterSphere / 3);

	// glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// glRotatef(-60, 0.0f, 1.0f, 0.0f);
	// drawCone(diameterCylinder / 3, sizeClampPart);

	// glPopMatrix();

	// //draws bottom part of clamp
	// glRotatef(-angleClampY - 60, 0.0f, 1.0f, 0.0f);

	// drawCylinder(diameterCylinder / 3, sizeClampPart);
	// glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// drawSphere(diameterSphere / 3);

	// glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// glRotatef(60, 0.0f, 1.0f, 0.0f);

	// drawCylinder(diameterCylinder / 3, sizeClampPart);
	// glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	// drawSphere(diameterSphere / 3);

	// glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	// glRotatef(60, 0.0f, 1.0f, 0.0f);
	// drawCone(diameterCylinder / 3, sizeClampPart);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Clamp");

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
