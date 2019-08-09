#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "ogradjenPut.h"
#include "kuglaIKamera.h"

static void inicijalizacija(int argc, char **argv);
static void promenaVelicineProzora(int sirina, int visina);
static void iscrtajNaEkran(void);
static void postaviSvetlo(void);

int main(int argc, char **argv)
{
    inicijalizacija(argc, argv);

    glutMainLoop();

    return 0;
}

static void inicijalizacija(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    //TODO rad sa tastaturom
    glutReshapeFunc(promenaVelicineProzora);
    glutDisplayFunc(iscrtajNaEkran);
    
    glClearColor((double)135/255, (double)206/255, (double)235/255, 0); //preuzeo sa neta boju neba
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

static void promenaVelicineProzora(int sirina, int visina)
{
    glViewport(0, 0, sirina, visina);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (double)sirina / visina, 1, duzinaPuta/2.5);
}

static void iscrtajNaEkran(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    postaviKameru();    
    postaviSvetlo();

    nacrtajOgradjenPut();
    nacrtajLoptu();
    //TODO nacrtajPrepreke();
    
    glutSwapBuffers();
}

static void postaviSvetlo()
{
    GLfloat pozicijaSvetla[] = { 0, 1, 0.3, 0 };
    GLfloat ambijentalnoSvetlo[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat difuznoSvetlo[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat spekularnoSvetlo[] = { 1, 1, 1, 1 };
    
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pozicijaSvetla);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambijentalnoSvetlo);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuznoSvetlo);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spekularnoSvetlo);
}
