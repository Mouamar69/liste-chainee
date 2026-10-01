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
bool     liste_maximum(const Maillon *tete, int *resultat);

#endif /* LISTE_H */

/* Question A : main.c utilise Maillon, mais il ne voit que liste.h.
     Si le type etait dans liste.c, main.c ne le connaitrait pas.
   Question B : liste.c a besoin du type Maillon, et le compilateur
     verifie que les fonctions du .c sont identiques a celles du .h. */
