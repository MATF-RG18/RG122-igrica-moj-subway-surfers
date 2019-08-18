#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

#include "ogradjenPut.h"
#include "kuglaIKamera.h"
#include "prepreke.h"

//svakim nivoom brzina raste za 0.05 i do nivoa 9 raste broj prepreka koje se koriste
//Nivoima su dodeljene vrednosti i kad broj poena dodje do njih dolazi se do tog nivoa
enum nivo
{
    NIVO1 = 0,
    NIVO2 = 70,
    NIVO3 = 140,
    NIVO4 = 210,
    NIVO5 = 280,
    NIVO6 = 350,
    NIVO7 = 420,
    NIVO8 = 490,
    NIVO9 = 560,
    NIVO10 = 630,
    NIVO11 = 1000,
    NIVO12 = 1200,
    NIVO13 = 1400,
    NIVO14 = 1600,
    NIVO15 = 1800,
    NIVO16 = 2000,
    NIVO17 = 2200,
    NIVO18 = 2400
};

static int pokrenutaAplikacija;
static int uSkoku;
static const int vremePozivaTajmera = 17;
static const double pi = 3.141592;
static double fi;
static const double pocetnaBrzinaPadanjaPriSkoku = 0.07;
static double brzinaPadanjaPriSkoku;
static double brzinaKuglice;
static const double pomerajKugliceUStranu = 0.45;
static double brzinaRotacijeKuglice;
static int indeksSledecePrepreke ;
static int krajAplikacije;
static int poeni;
static int pomocniPoeni;
static int skretanjeDesno, skretanjeLevo;

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
static void postaviBodove(void);
static void otpustenSpecijalniTaster(int k, int x, int y);

static void pretvoriIntUString(int x, char *s);
static void obrniString(char *s, int n);

static void obradiNivo(void);

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
    glutSpecialUpFunc(otpustenSpecijalniTaster);
    
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
    poeni = 0;
    pomocniPoeni = 0;
    skretanjeDesno = 0, skretanjeLevo = 0;
    brVrstaPrepreka = 1;
    brzinaRotacijeKuglice = 20;
    
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
    nacrtajPrepreke();
    if(krajAplikacije)
        nacrtajLoptuKadSeSudarila();
    else 
        nacrtajLoptu();

    postaviBodove();
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
                skretanjeLevo = 1;
            break;
            
            case GLUT_KEY_RIGHT:
                skretanjeDesno = 1;
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
    
    if(skretanjeLevo)
        kugla.x += pomerajKugliceUStranu;
    if(skretanjeDesno)
        kugla.x -= pomerajKugliceUStranu;
    
    pomocniPoeni += vremePozivaTajmera*(2*brzinaKuglice); //sto se brze kugla krece brze se zaradjuju poeni
    if(pomocniPoeni > 1000)
    {
        pomocniPoeni -= 1000;
        poeni++;
    }
    
    if(sudarKugleSaPreprekom() || sudarKugleSaZidom())
        krajAplikacije = 1;
        
    if(kuglaProslaPrepreku())
        indeksSledecePrepreke++;
    
    /*printf("pomeraj: %lf\nfi: %lf\n", kugla.z, fi); //TODO Dodato radi debagovanja treba izbrisati
    printf("%d\n", poeni); //TODO Dodato radi debagovanja treba izbrisati
    printf("brzinaKuglice: %lf\n", brzinaKuglice);
    ispisiSledecuPrepreku(); //TODO Dodato radi debagovanja treba izbrisati*/
    
    obradiNivo();
    
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

static void postaviBodove(void)
{
    glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);
 
    
    char poeniString[20];
    pretvoriIntUString(poeni, poeniString);
    
    int i;
    glRasterPos3f(-(sirinaPuta/3 - kugla.x), visinaZida + 3, kugla.z + 5);
    char s[] = "Score: ";
    for(i = 0; s[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    for(i = 0; poeniString[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, poeniString[i]);
    
	glEnable(GL_LIGHTING);
}

static void pretvoriIntUString(int x, char *s)
{
    if(x == 0)
    {
        s[0] = '0', s[1] = '\0';
        return;
    }
    
    int i;
    for(i = 0; x != 0; i++)
    {
        s[i] = x%10 + '0';
        x /= 10;
    }
    s[i] = '\0';
    obrniString(s, i);
    return;
    
}

static void obrniString(char *s, int n)
{
    int i, j;
    char pom;
    for(i=0, j=n-1; i<j; i++, j--)
    {
        pom = s[i];
        s[i] = s[j];
        s[j] = pom;
    }
}

static void otpustenSpecijalniTaster(int k, int x, int y)
{
    switch(k)
    {
        case GLUT_KEY_LEFT:
            skretanjeLevo = 0;
            break;
            
        case GLUT_KEY_RIGHT:
            skretanjeDesno = 0;
            break;
    }
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
    printf("sirina: %lf\n", pa.sirina);
}

static void obradiNivo(void)
{
    if(poeni >= NIVO18)
    {
        brzinaKuglice = 0.6 + 18*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO17)
    {
        brzinaKuglice = 0.6 + 17*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO16)
    {
        brzinaKuglice = 0.6 + 16*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO15)
    {
        brzinaKuglice = 0.6 + 15*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO14)
    {
        brzinaKuglice = 0.6 + 14*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO13)
    {
        brzinaKuglice = 0.6 + 13*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO12)
    {
        brzinaKuglice = 0.6 + 12*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO11)
    {
        brzinaKuglice = 0.6 + 11*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO10)
    {
        brzinaKuglice = 0.6 + 10*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO9)
    {
        brzinaKuglice = 0.6 + 9*0.05;
        brVrstaPrepreka = 9;
    }
    else if(poeni >= NIVO8)
    {
        brzinaKuglice = 0.6 + 8*0.05;
        brVrstaPrepreka = 8;
    }
    else if(poeni >= NIVO7)
    {
        brzinaKuglice = 0.6 + 7*0.05;
        brVrstaPrepreka = 7;
    }
    else if(poeni >= NIVO6)
    {
        brzinaKuglice = 0.6 + 6*0.05;
        brVrstaPrepreka = 6;
    }
    else if(poeni >= NIVO5)
    {
        brzinaKuglice = 0.6 + 5*0.05;
        brVrstaPrepreka = 5;
    }
    else if(poeni >= NIVO4)
    {
        brzinaKuglice = 0.6 + 4*0.05;
        brVrstaPrepreka = 4;
    }
    else if(poeni >= NIVO3)
    {
        brzinaKuglice = 0.6 + 3*0.05;
        brVrstaPrepreka = 3;
    }
    else if(poeni >= NIVO2)
    {
        brzinaKuglice = 0.6 + 2*0.05;
        brVrstaPrepreka = 2;
    }
    else if(poeni >= NIVO1)
    {
        brzinaKuglice = 0.6 + 1*0.05;
        brVrstaPrepreka = 1;
    }
    
    brzinaRotacijeKuglice = 34 * brzinaKuglice;
}
