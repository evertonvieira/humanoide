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

    //Aqui que acontece a mágica!!!
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Primeiro Programa");
    glutDisplayFunc(Desenha);

    Inicializa();

    glutMainLoop();
}

