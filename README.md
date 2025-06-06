# Game of Life
Cod scris in C. Include simulare, compresie de stare, arbori de evolutie si analiza de graf.  
Inspirat de Conway's Game of Life
## Descriere
Acest proiect implementeaza mai multe moduri de lucru pe baza unei grile initiale in Game of Life.  
In functie de tipul de task ales (T ? {1, 2, 3, 4}), programul poate:
- T = 1: simula K generatii Game of Life clasic
- T = 2: stoca in mod eficient doar modificarile dintre generatii folosind o stiva
- T = 3: construi un arbore binar cu K niveluri de evolutie (reguli diferite in stanga/dreapta)
- T = 4: determina, pentru fiecare configuratie din arbore, cel mai lung lant Hamiltonian de celule vii adiacente
## Utilizare
### Compilare
Se foloseste un compilator standard C. Exemplu cu gcc:
    gcc -o game_of_life main.c
### Rulare
    ./game_of_life input.txt output.txt
## Format fisier de intrare (input.txt)
    T
    N M
    K
    +++++
    +X+++
    +XX++
    +++X+
    +++++
- T – numarul taskului
- N si M – dimensiuni grila
- K – numar generatii (sau adancime arbore)
- matrice cu + (celula moarta) si X (vie)
## Structura proiectului
Tot codul este scris intr-un singur fisier: main.c, dar este modularizat folosind functii si structuri dedicate.
### Structuri de date
- Coord – coordonatele unei celule
- List, Node – lista ordonata pentru modificari pe generatii
- Stack, StackNode – stiva pentru taskul 2
- ArboreNode – nod in arborele binar de evolutie
- Graf – structura pentru reprezentarea celulelor vii ca graf (task 4)
## Functii principale
| Functie                | Descriere                                                              |
|------------------------|------------------------------------------------------------------------|
| generatie_urmatoare()  | calculeaza urmatoarea generatie (regula Game of Life)                 |
| generatieb()           | generatie alternativa: celule cu exact 2 vecini devin vii             |
| compara_matrici()      | detecteaza ce celule s-au modificat intre doua matrici                |
| push()                 | adauga modificari in stiva (task 2)                                   |
| construieste()         | construieste arborele de generatii (task 3/4)                         |
| matrice_in_graf()      | transforma o matrice intr-un graf pe celule vii                       |
| cel_mai_lung_lant()    | cauta cel mai lung lant Hamiltonian (task 4)                          |
## Exemple de output
### Task 2 – Stiva cu modificari
    1 1 2 3 2 3 3
    2 2 1 2 2 2 3 3 1
### Task 4 – Lant Hamiltonian
    4
    (1,1) (2,2) (3,2) (3,3) (2,3)
## Observatii pentru dezvoltatori
Matricile sunt alocate dinamic, deci codul poate lucra cu orice dimensiune (limitat doar de RAM).  
Codul e organizat modular, deci se poate separa cu usurinta in fisiere:
    stack.c, arbore.c, grafuri.c, etc.
Nu s-au folosit librarii externe – codul e pur C standard.
## Extensii posibile
- Interfata vizuala (cu SDL sau ncurses)
- Detectarea automata a ciclurilor in evolutie
- Introducerea de alte reguli de vecinatate
## Autor
David Denis Emanoil
