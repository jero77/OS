main: aufgabe1 bc aufgabe2 aufgabe3

aufgabe1: aufgabe1_1.c
		gcc -o aufgabe1_1.out aufgabe1_1.c

bc: bc.c
		gcc -o bc.out bc.c

aufgabe2: client.c server.c
		gcc -o client.out client.c
		gcc -o server.out server.c

aufgabe3: racecondition.c solvedrace.c
		gcc -pthread -o racecondition.out racecondition.c
		gcc -pthread -o solvedracec.out solvedrace.c
