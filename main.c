#include <stdio.h>
#include "liste.h"

int main(void)
{
    Maillon *liste = NULL;
    for (int i = 1; i <= 5; i++) liste = liste_inserer(liste, i * 10);

    printf("liste     : ");
    liste_afficher(liste);
    printf("longueur  : %d\n", liste_longueur(liste));
    printf("contient 30 : %s\n", liste_contient(liste, 30) ? "oui" : "non");
    printf("blocs apres construction : %d\n", liste_blocs_en_circulation());

    liste_liberer(liste);
    printf("liberee\n");
    printf("blocs apres liberation   : %d\n", liste_blocs_en_circulation());
    return 0;
}

/* Exercice 2 - Compiler en deux temps
   Sortie du programme :
     liste     : 50 -> 40 -> 30 -> 20 -> 10 -> NULL
     longueur  : 5
     contient 30 : oui
     liberee

   Question A : deux fichiers .o, main.o et liste.o, un par fichier .c. Il
     n'y a pas de liste.h.o : un .h ne se compile pas seul, il est recopie
     par le preprocesseur dans chaque .c qui l'inclut (#include = copier-
     coller). Son contenu est donc compile a l'interieur de main.o et liste.o.
   Question B : elle recompile tous les .c a chaque fois, meme ceux qui n'ont
     pas change, et ne garde aucun .o. En deux temps, si seul main.c change,
     on recompile main.c seul et on relie avec l'ancien liste.o : sur un gros
     projet, on gagne beaucoup de temps (c'est ce qu'automatise make). */

/* Exercice 3 - Les trois erreurs classiques
   Cas 1 (liste.o oublie) - LIEN :
     /usr/bin/ld: main.c:(.text+0x35): undefined reference to `liste_inserer'
     ... puis collect2: error: ld returned 1 exit status
   Cas 2 (#include "liste.h" retire) - COMPILATION :
     main.c:5:5: error: unknown type name 'Maillon'
   Cas 3 (garde retiree + double inclusion) - COMPILATION :
     liste.h:4:16: error: redefinition of 'struct Maillon'

   Question 1 : le cas 1. On le voit au prefixe /usr/bin/ld (ld = l'editeur
     de liens), a "collect2: error: ld returned 1 exit status", et au fait
     qu'il n'y a pas de numero de ligne mais une adresse (.text+0x35). Le
     code compile : c'est l'assemblage qui ne trouve pas les fonctions.
   Question 2 : le premier, "unknown type name 'Maillon'". Les autres
     (implicit declaration of function..., int * from int...) n'en sont que
     des consequences : une fois l'include remis, ils disparaissent tous.
     Regle : toujours corriger la PREMIERE erreur d'abord.
   Question 3 : des qu'un .h en inclut un autre. Par exemple un pile.h qui
     fait #include "liste.h" : un main.c qui inclut pile.h et liste.h recoit
     liste.h deux fois, sans l'avoir ecrit deux fois. La garde le protege. */

/* Exercice 6 - Trouver une fuite sans aucun outil
   Fuite introduite : une seconde liste de 3 elements, jamais liberee
     Maillon *fuite = NULL;
     for (int i = 1; i <= 3; i++) fuite = liste_inserer(fuite, i);
                                  Sans la fuite   Avec la fuite
     Compteur apres construction        5               8
     Compteur apres liberation          0               3

   Question A : ce sont des details internes du module. Static les rend
     invisibles hors de liste.c : personne d'autre ne peut modifier blocs
     (blocs = 0 depuis main.c fausserait le compte) ni appeler suivi_free sur
     un bloc qui ne vient pas de suivi_malloc. On n'expose que la lecture,
     liste_blocs_en_circulation. Et static evite les conflits de noms avec
     une autre variable blocs ailleurs dans le projet.
   Question B : sans le test dans suivi_malloc, un malloc rate (NULL)
     compterait un bloc qui n'existe pas : le compteur annoncerait une fuite
     inexistante. Sans le test dans suivi_free, free(NULL) (legal, ne fait
     rien) decrementerait quand meme : le compteur pourrait masquer une vraie
     fuite, voire devenir negatif. Dans les deux cas il ment.
   Question C : non, il dit combien, pas ou. Avec ce seul outil, on affiche
     le compteur a plusieurs endroits du programme (avant/apres chaque
     construction et chaque liberation) et on cherche l'etape ou il monte
     sans jamais redescendre : la fuite est la, on resserre jusqu'a la
     trouver (recherche par dichotomie). */
