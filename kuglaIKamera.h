#ifndef __kuglaIKamera__h
#define __kuglaIKamera__h

//(x, y, z) - centar sfere
typedef struct sfera 
{
    double x, y, z;
    double r;
} Sfera;

extern Sfera kugla;
extern double rotacijaKugle;

extern void nacrtajLoptu(void);
extern void postaviKameru(void);
extern void inicijalizujKuglu(void);
extern void nacrtajLoptuKadSeSudarila(void);


#endif
