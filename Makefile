CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJ    = main.o liste.o

demo: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c liste.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) demo

.PHONY: clean

# Exercice 4 - Premier Makefile
# make (1re fois) execute les trois commandes :
#   gcc -Wall -Wextra -std=c11 -g -c main.c
#   gcc -Wall -Wextra -std=c11 -g -c liste.c
#   gcc -Wall -Wextra -std=c11 -g -o demo main.o liste.o
# make (2e fois, sans rien modifier) affiche :
#   make: 'demo' is up to date.
#
# Question A : make compare les dates de modification. Une cible n'est
#   refaite que si elle n'existe pas ou si l'une de ses dependances est plus
#   recente qu'elle. Ici demo, main.o et liste.o sont tous plus recents que
#   leurs sources : rien n'a change, donc rien a refaire.
# Question B : message exact avec quatre espaces au lieu de la tabulation :
#   Makefile:2: *** missing separator.  Stop.
#   make exige une tabulation en debut de ligne de commande ; avec des
#   espaces, il ne reconnait pas la ligne comme une commande.

# Exercice 5 - La dependance au fichier d'en-tete
# Etape 2, avec la dependance (main.o: main.c liste.h), touch liste.h :
#   main.c ET liste.c sont recompiles, puis demo est relie.
# Etape 4, sans la dependance (main.o: main.c), touch liste.h :
#   seul liste.c est recompile, puis demo est relie. main.o n'est PAS refait.
#
# Question A : sans la dependance, make ne sait plus que main.o depend de
#   liste.h : quand liste.h change, il garde l'ancien main.o. Seul liste.o
#   est mis a jour.
# Question B : un executable incoherent. liste.o est compile avec la nouvelle
#   structure Maillon (plus grande, champs decales) mais main.o avec
#   l'ancienne : les deux moities ne sont pas d'accord sur la taille et la
#   disposition d'un Maillon. Aucune erreur a la compilation ni au lien, mais
#   un comportement indefini a l'execution (valeurs fausses, plantage).
#   Seul un make clean && make le corrige.
#
# Makefile final : $@ = la cible, $^ = toutes les dependances, $< = la
#   premiere dependance. La regle %.o: %.c liste.h vaut pour tous les .o et
#   garde la dependance a liste.h.
