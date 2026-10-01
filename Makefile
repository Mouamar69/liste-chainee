demo: main.o liste.o
	gcc -Wall -Wextra -std=c11 -g -o demo main.o liste.o

main.o: main.c liste.h
	gcc -Wall -Wextra -std=c11 -g -c main.c

liste.o: liste.c liste.h
	gcc -Wall -Wextra -std=c11 -g -c liste.c

clean:
	rm -f main.o liste.o demo

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
