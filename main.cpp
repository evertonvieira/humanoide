#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>


void Desenha(void) {

    //Limpa a janela de visualização com a cor de fundo especificada
    glClear(GL_COLOR_BUFFER_BIT);

    //Executa os comandos OpenGL
    glFlush();

}

// Inicializa parâmetros de rendering
void Inicializa(void) {

    // Define a cor de fundo da janela de visualização como preta
    glClearColor(1, 1, 1, 1.0f);

}
// Programa Principal
int main(int argc, char ** argv) {

    glutInit(&argc, argv);

    int width = 900; //define a largura da janela
    int height = 600; //define a altura da janela

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Robô Humanoide!");

    //recupera as dimenções da tela
    int widthTela = glutGet(GLUT_SCREEN_WIDTH);
    int heightTela = glutGet(GLUT_SCREEN_HEIGHT); 

    //centraliza a janela no centro da tela
    glutPositionWindow((widthTela-width)/2, (heightTela-height)/2);
    glutReshapeWindow(width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    
    glutDisplayFunc(Desenha);

    Inicializa();

    glutMainLoop();

}

