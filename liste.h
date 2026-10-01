#ifndef LISTE_H
#define LISTE_H

#include <stdbool.h>

typedef struct Maillon {
    int valeur;
    struct Maillon *suivant;
} Maillon;

Maillon *liste_inserer(Maillon *tete, int valeur);
int      liste_longueur(const Maillon *tete);
bool     liste_contient(const Maillon *tete, int valeur);
void     liste_afficher(const Maillon *tete);
void     liste_liberer(Maillon *tete);
int      liste_blocs_en_circulation(void);

#endif /* LISTE_H */

/* Question A : main.c manipule des Maillon (Maillon *liste = NULL) et le
     compilateur doit connaitre le type pour compiler les prototypes. Or
     main.c n'inclut que liste.h, jamais liste.c : un type defini dans
     liste.c serait invisible depuis main.c, d'ou "unknown type name".
   Question B : pour que le compilateur verifie que les definitions de
     liste.c correspondent exactement aux declarations promises dans
     liste.h (sinon une difference de parametres passerait inapercue), et
     parce que liste.c a lui aussi besoin du type Maillon. */
