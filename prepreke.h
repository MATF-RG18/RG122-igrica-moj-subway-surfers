#ifndef __prepreke__h
#define __prepreke__h

//pomerajuce su pomerajuce po x osi
enum prepreke
{
    Obicna = 0,
    Preskakajuca = 1,
    ObicnaPlusPreskakajuca = 2,
    Dvostruka = 3,
    DvostrukaPlusPreskakajuca = 4,
    PomerajucaObicna = 5,
    PomerajucaObicnaPlusPreskakajuca = 6,
    PomerajucaDvostruka = 7,
    PomerajucaDvostrukaPlusPreskakajuca = 8
};

//deo prepreke je uvek kvadar
typedef struct deoPrepreke
{
    double x, y, z; //centar kvadra
    double brzinaKretanja;
    double sirina;
    double visina;
    //debljina je uvek 1
} DeoPrepreke;

//prepreka se sastoji iz jedgog, dva ili tri dela prepreke
typedef struct prepreka
{
    DeoPrepreke a;
    DeoPrepreke b;
    DeoPrepreke c;
    
    enum prepreke vrsta;
    
} Prepreka;

extern int brVrstaPrepreka;
extern Prepreka prvaPolovina[];
extern Prepreka drugaPolovina[];
extern int brPreprekaNaPrvojPolovini;
extern int brPreprekaNaDrugojPolovini;

extern void nacrtajPrepreke(void);
extern void pomeriPrepreke(void);
extern void napraviNovePrepreke(void);

#endif
