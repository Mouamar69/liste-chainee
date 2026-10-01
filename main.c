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

    int max;
    if (liste_maximum(liste, &max))
        printf("maximum   : %d\n", max);
    else
        printf("maximum   : liste vide\n");
    if (liste_maximum(NULL, &max))
        printf("maximum de NULL : %d\n", max);
    else
        printf("maximum de NULL : liste vide\n");

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

/* Exercice 7 - valgrind sur la meme fuite (valgrind --leak-check=full ./demo)
   Sans la fuite, deux dernieres lignes du rapport :
     For lists of detected and suppressed errors, rerun with: -s
     ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
   Avec la fuite :
     48 (16 direct, 32 indirect) bytes in 1 blocks are definitely lost
        at 0x4846828: malloc (in .../vgpreload_memcheck-amd64-linux.so)
        by 0x109351: suivi_malloc (liste.c:11)
        by 0x1093D3: liste_inserer (liste.c:28)
        by 0x109251: main (main.c:9)
                          Sans la fuite              Avec la fuite
     definitely lost      0                          16 octets en 1 bloc
     indirectly lost      0                          32 octets en 2 blocs
     total heap usage     6 allocs, 6 frees          9 allocs, 6 frees
     Votre compteur       0                          3

   Question A : main.c:9, la ligne qui construit la seconde liste
     (fuite = liste_inserer(fuite, i)). Ce n'est pas la ligne du free oublie :
     valgrind montre ou le bloc a ete ALLOUE, pas ou il aurait du etre
     libere. Le free manquant est liste_liberer(fuite), qui devait figurer a
     cote de liste_liberer(liste), avant le return.
   Question B : 1 bloc definitely lost et 2 indirectly lost. Plus aucun
     pointeur ne mene a la tete (3) : elle est perdue directement. Les
     maillons 2 et 1 ne sont accessibles que par le champ suivant de la tete
     perdue : ils sont perdus indirectement. Liberer la tete correctement
     (avec liste_liberer) aurait tout rendu.
   Question C : N = 9 et M = 6 (sans fuite : 6 et 6). On a cree 8 maillons
     (5 + 3), pas 9 : l'allocation en plus est le tampon de 4096 octets que
     printf reserve pour stdout (d'ou 4,224 = 8*16 + 4096 octets). Les
     6 free = 5 maillons de la premiere liste + ce tampon. */

/* Exercice 9 - Ajouter une fonction au module
   Sortie du programme :
     liste     : 50 -> 40 -> 30 -> 20 -> 10 -> NULL
     longueur  : 5
     contient 30 : oui
     blocs apres construction : 5
     maximum   : 50
     maximum de NULL : liste vide
     liberee
     blocs apres liberation   : 0
   valgrind --leak-check=full ./demo : aucune erreur, aucune fuite.

   Question A : trois fichiers modifies : liste.h (declaration), liste.c
     (definition) et main.c (appel). make a recompile main.c ET liste.c,
     puis relie demo : les deux .o dependent de liste.h, qui a change.
   Question B : -1 peut etre une vraie valeur de la liste : une liste qui
     contient -5, -1 et -3 a pour maximum -1, et l'appelant ne pourrait pas
     distinguer "le maximum vaut -1" de "la liste est vide". Aucune valeur
     d'int n'est libre pour servir de signal. On separe donc les deux
     informations : le bool dit s'il y a un resultat, *resultat le contient. */
