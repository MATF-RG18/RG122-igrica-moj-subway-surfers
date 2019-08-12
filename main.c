#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

#include "ogradjenPut.h"
#include "kuglaIKamera.h"
#include "prepreke.h"

static int pokrenutaAplikacija = 0;
static int uSkoku = 0;
static const int vremePozivaTajmera = 17;
static const double pi = 3.141592;
static double fi = 0;
static const double pocetnaBrzinaPadanjaPriSkoku = 0.07;
static double brzinaPadanjaPriSkoku = 0.07;
static double brzinaKuglice = 0.6;
static double pomerajKugliceUStranu = 0.5;
static double brzinaRotacijeKuglice = 20;
static int indeksSledecePrepreke = 0;
static int krajAplikacije = 0;

static void inicijalizacijaGlut(int argc, char **argv);
static void postaviSvetlo(void);
static void promenaVelicineProzora(int sirina, int visina);
static void iscrtajNaEkran(void);
static void tastatura(unsigned char pritisnutKarakter, int x, int y);
static void tastaturaSpecijalniTasteri(int pritisnutSpecijalniKarakter, int x, int y);
static void tajmer(int idTajmera);
static void obradiSkok(void);
static void kretanjePoOsiZ(void);
static void rotirajKuglicu(void);
static int sudarKugleSaPreprekom(void);
static int sudarKugleSaDelomPrepreke(DeoPrepreke px);
static int tackaSeNalaziUKvadru(double x, double y, double z, DeoPrepreke px);
static int kuglaProslaPrepreku(void);
static int sudarKugleSaZidom(void);
static void inicijalizacijaAplikacije(void);

void ispisiSledecuPrepreku(void); //TODO Dodato radi debagovanja treba izbrisati
void ispisi(DeoPrepreke pa); //TODO Dodato radi debagovanja treba izbrisati

int main(int argc, char **argv)
{
    inicijalizacijaAplikacije();
    inicijalizacijaGlut(argc, argv);
    
    glutMainLoop();

    return 0;
}

static void inicijalizacijaGlut(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(tastatura);
    glutSpecialFunc(tastaturaSpecijalniTasteri);
    glutReshapeFunc(promenaVelicineProzora);
    glutDisplayFunc(iscrtajNaEkran);
    
    glClearColor((double)135/255, (double)206/255, (double)235/255, 0); //preuzeo sa neta boju neba
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

static void inicijalizacijaAplikacije(void)
{
    inicijalizujKuglu();
    inicijalizujPrepreke();
    
    pokrenutaAplikacija = 0;
    uSkoku = 0;
    fi = 0;
    brzinaPadanjaPriSkoku = 0.07;
    brzinaKuglice = 0.6;
    indeksSledecePrepreke = 0;
    krajAplikacije = 0;
    
    srand(time(NULL));
    napraviNovePrepreke();
    pomeriPrepreke();
    napraviNovePrepreke();
}

static void postaviSvetlo()
{
    GLfloat pozicijaSvetla[] = { 0, 1, -0.3, 0 };
    GLfloat ambijentalnoSvetlo[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat difuznoSvetlo[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat spekularnoSvetlo[] = { 1, 1, 1, 1 };
    
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, pozicijaSvetla);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambijentalnoSvetlo);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuznoSvetlo);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spekularnoSvetlo);
}

static void promenaVelicineProzora(int sirina, int visina)
{
    glViewport(0, 0, sirina, visina);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (double)sirina / visina, 1, duzinaPuta/2);
}

static void iscrtajNaEkran(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    postaviKameru();    
    postaviSvetlo();

    nacrtajOgradjenPut();
    nacrtajLoptu();
    nacrtajPrepreke();
    
    glutSwapBuffers();
}

static void tastatura(unsigned char pritisnutKarakter, int x, int y)
{
    switch(pritisnutKarakter) 
    {
        case 27:
            exit(0);
            break;
            
        case 'g':
        case 'G':
            if(!pokrenutaAplikacija && !krajAplikacije)
            {
                pokrenutaAplikacija = 1;
                glutTimerFunc(vremePozivaTajmera, tajmer, 0);
            }
            break;
            
        case 'p':
        case 'P':
            pokrenutaAplikacija = 0;
            break;
            
        case 'r':
        case 'R':
            if(krajAplikacije)
            {
                inicijalizacijaAplikacije();
                glutPostRedisplay();
            }
    }
}

static void tastaturaSpecijalniTasteri(int pritisnutSpecijalniKarakter, int x, int y)
{
    if(!krajAplikacije && pokrenutaAplikacija)
    {
        switch(pritisnutSpecijalniKarakter)
        {
            case GLUT_KEY_LEFT:
                kugla.x += pomerajKugliceUStranu;
                break;
            
            case GLUT_KEY_RIGHT:
                kugla.x += -pomerajKugliceUStranu;
                break;
                
            case GLUT_KEY_UP:
                if(!uSkoku)
                {
                    uSkoku = 1;
                }
                break;
            
            //Omogucava kad smo u skoku da brze padnemo na zemlju
            case GLUT_KEY_DOWN:
            if(uSkoku)
            {                
                brzinaPadanjaPriSkoku += pocetnaBrzinaPadanjaPriSkoku;
            }
            break;
        }
    }
}

static void tajmer(int idTajmera)
{
    kretanjePoOsiZ();
    rotirajKuglicu();
    obradiSkok();
    kretanjePomerajucihPrepreka();
    
    if(sudarKugleSaPreprekom() || sudarKugleSaZidom())
        krajAplikacije = 1;
        
    if(kuglaProslaPrepreku())
        indeksSledecePrepreke++;
    
    printf("pomeraj: %lf\nfi: %lf\n", kugla.z, fi); //TODO Dodato radi debagovanja treba izbrisati
    ispisiSledecuPrepreku(); //TODO Dodato radi debagovanja treba izbrisati
    
    glutPostRedisplay();
    if(pokrenutaAplikacija && !krajAplikacije)
        glutTimerFunc(vremePozivaTajmera, tajmer, 0);
}

static void kretanjePoOsiZ(void)
{
    kugla.z += brzinaKuglice;
    
    if(kugla.z >= duzinaPuta/2)
    {
        kugla.z -= duzinaPuta/2;
        pomeriPrepreke();
        indeksSledecePrepreke = 0;
        napraviNovePrepreke();
    }
}

static void obradiSkok(void)
{
    if(uSkoku)
    {
        fi += brzinaPadanjaPriSkoku;
        if(fi >= pi)
        {
            fi = 0;
            uSkoku = 0;
            brzinaPadanjaPriSkoku = pocetnaBrzinaPadanjaPriSkoku; //opet postavljamo jer se mozda menjalo u GLUT_KEY_DOWN
        }
        kugla.y = kugla.r + 2.5*sin(fi);
    }
}

static void rotirajKuglicu(void)
{
    rotacijaKugle += brzinaRotacijeKuglice;
    
    if(rotacijaKugle >= 360)
        rotacijaKugle += -360;
}

static int sudarKugleSaPreprekom(void)
{
    Prepreka p;
    
    if(indeksSledecePrepreke < brPreprekaNaPrvojPolovini)
        p = prvaPolovina[indeksSledecePrepreke];
    else
        p = drugaPolovina[0];
    
    int sudar;
    switch(p.vrsta)
    {
        case Obicna:
            sudar = sudarKugleSaDelomPrepreke(p.a);
            break;
        case Preskakajuca:
            sudar = sudarKugleSaDelomPrepreke(p.a);
            break;
        case ObicnaPlusPreskakajuca:
            sudar = sudarKugleSaDelomPrepreke(p.a) || sudarKugleSaDelomPrepreke(p.b);
            break;
        case Dvostruka:
            sudar = sudarKugleSaDelomPrepreke(p.a) || sudarKugleSaDelomPrepreke(p.b);
            break;
        case DvostrukaPlusPreskakajuca:
            sudar = sudarKugleSaDelomPrepreke(p.a) || sudarKugleSaDelomPrepreke(p.b) || sudarKugleSaDelomPrepreke(p.c);
            break;
        case PomerajucaObicna:
            sudar = sudarKugleSaDelomPrepreke(p.a);
            break;
        case PomerajucaObicnaPlusPreskakajuca:
            sudar = sudarKugleSaDelomPrepreke(p.a) || sudarKugleSaDelomPrepreke(p.b);
            break;
        case PomerajucaDvostruka:
            sudar = sudarKugleSaDelomPrepreke(p.a) || sudarKugleSaDelomPrepreke(p.b);
            break;
        case PomerajucaDvostrukaPlusPreskakajuca:
            sudar = sudarKugleSaDelomPrepreke(p.a) || sudarKugleSaDelomPrepreke(p.b) || sudarKugleSaDelomPrepreke(p.c);
            break;
    }
    
    return sudar;
}

//sudar kugle sa kvadrom jer su sve prepreke kvadar
//parametarska jednacina kruga i sad za te neke tacke na krugu proverimo da li su usle u kvadar
//ako jesu onda je do sudara doslo
static int sudarKugleSaDelomPrepreke(DeoPrepreke px)
{
    double fi, teta;
	double korak = pi/10;
    double x, y, z;
	for(fi = 0; fi <= 2*pi; fi += korak)
    {
        for(teta = 0; teta <= pi; teta += korak)
		{
			x = kugla.x + kugla.r*sin(teta)*cos(fi);
            y = kugla.y + kugla.r*sin(teta)*sin(fi);
   			z = kugla.z + kugla.r*cos(teta);
            
            if(tackaSeNalaziUKvadru(x, y, z, px))
                return 1;
		}
    }
    
    return 0;
}

static int tackaSeNalaziUKvadru(double x, double y, double z, DeoPrepreke px)
{
    if
    (
        (x >= px.x - px.sirina/2 && x <= px.x + px.sirina/2) &&
        (y >= px.y - px.visina/2 && y <= px.y + px.visina/2) &&
        (z >= px.z - 0.5 && z <= px.z + 0.5) //0.5 jer je debljina uvek 1 pa debljina/2
    )
        return 1;
    else return 0;
}

static int kuglaProslaPrepreku(void)
{
    Prepreka p;
    
    if(indeksSledecePrepreke < brPreprekaNaPrvojPolovini)
        p = prvaPolovina[indeksSledecePrepreke];
    else
        p = drugaPolovina[0];
    
    return kugla.z-kugla.r > p.a.z+0.5; //zadnja strana kugle prosla zadnju stranu prepreke
}

static int sudarKugleSaZidom(void)
{
    if(kugla.x+kugla.r <= sirinaPuta/2 && kugla.x-kugla.r >= -sirinaPuta/2)
        return 0;
    else
        return 1;
}

void ispisiSledecuPrepreku(void) //TODO Dodato radi debagovanja treba izbrisati
{
    Prepreka p;
    
    if(indeksSledecePrepreke < brPreprekaNaPrvojPolovini)
        p = prvaPolovina[indeksSledecePrepreke];
    else
        p = drugaPolovina[0];
    
    switch(p.vrsta)
    {
        case Obicna:
            printf("Obicna:\n");
            ispisi(p.a);
            break;
        case Preskakajuca:
            printf("Preskakajuca:\n");
            ispisi(p.a);
            break;
        case ObicnaPlusPreskakajuca:
            printf("ObicnaPlusPreskakajuca:\n");
            ispisi(p.a);
            ispisi(p.b);
            break;
        case Dvostruka:
            printf("Dvostruka:\n");
            ispisi(p.a);
            ispisi(p.b);
            break;
        case DvostrukaPlusPreskakajuca:
            printf("DvostrukaPlusPreskakajuca:\n");
            ispisi(p.a);
            ispisi(p.b);
            ispisi(p.c);
            break;
        case PomerajucaObicna:
            printf("PomerajucaObicna:\n");
            ispisi(p.a);
            break;
        case PomerajucaObicnaPlusPreskakajuca:
            printf("PomerajucaObicnaPlusPreskakajuca:\n");
            ispisi(p.a);
            ispisi(p.b);
            break;
        case PomerajucaDvostruka:
            printf("PomerajucaDvostruka:\n");
            ispisi(p.a);
            ispisi(p.b);
            break;
        case PomerajucaDvostrukaPlusPreskakajuca:
            printf("PomerajucaDvostrukaPlusPreskakajuca:\n");
            ispisi(p.a);
            ispisi(p.b);
            ispisi(p.c);
            break;
    }
}

void ispisi(DeoPrepreke pa) //TODO Dodato radi debagovanja treba izbrisati
{
    printf("Centar: %lf, %lf %lf\n", pa.x, pa.y, pa.z);
    printf("brzinaKretanja: %lf\n", pa.brzinaKretanja);
}
