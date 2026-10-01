#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *t = malloc(5 * sizeof(int));
    if (t == NULL) return 1;
    t[5] = 42;                    /* une case trop loin */
    printf("%d\n", t[5]);
    free(t);
    return 0;
}

/* Exercice 8 - Ce que le compteur ne peut pas voir
   Compilation : gcc -Wall -Wextra -g -o deborde deborde.c (aucun warning)
   Sortie affichee, sans outil : 42
   Code de sortie, sans outil  : 0
   Message de valgrind :
     Invalid write of size 4
        at 0x1091B9: main (deborde.c:8)
      Address 0x4a76054 is 0 bytes after a block of size 20 alloc'd
        at 0x4846828: malloc (...)
        by 0x10919E: main (deborde.c:6)
     (puis la meme chose en "Invalid read of size 4" pour deborde.c:9)

   Question A : non, un malloc et un free : les paires sont justes.
     Le compteur aurait affiche 0, il n'aurait rien vu : il compte les blocs,
     il ne surveille pas ce qu'on ecrit dedans.
   Question B : 20 = la taille du bloc alloue, 5 * sizeof(int) = 5 * 4
     octets. 0 = la distance entre l'adresse touchee et la fin du bloc :
     t[5] commence pile a l'octet 20, juste apres la derniere case t[4]
     (octets 16 a 19). Pour t[6] : "4 bytes after a block of size 20"
     (verifie).
   Question C : un test qui passe ne prouve pas que le programme est juste.
     Le debordement est un comportement indefini : ici il ecrase de la
     memoire qui ne sert a rien, ailleurs il corromprait une autre donnee ou
     planterait. Un test montre la presence de bugs, jamais leur absence.
   Question D : le compteur, toujours actif, gratuit, sans outil : il
     verifie en permanence (meme sur Windows ou dans un test automatique)
     qu'on rend autant de blocs qu'on en prend. Valgrind (ou
     -fsanitize=address) des que le compteur n'est pas a 0, pour savoir OU
     est la fuite, et pour tout ce que le compteur ne voit pas :
     debordements, lecture apres free, variables non initialisees. On le
     passe avant chaque rendu, sur tout le programme. */
