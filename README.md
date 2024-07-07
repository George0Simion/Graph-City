# Tema 2

## Cuprins
1. [Task 1](#task-1)
2. [Task 2](#task-2)

## Task 1
--------------------------------------------------------------------------------------------------------------------------------

Pentru realizarea acestui task, mai întâi citesc datele de la tastatură și le adaug în graf și într-o matrice de orașe pentru afișare. În funcția de adăugare a unei muchii, mai întâi adaug destinația muchiei, copiez vectorul de uzuri, setez direcția arcului și caut sursa în graf. Dacă sursa nu e în graf, atunci o adaug. Fac aceleași operații și pentru orașe invers, destinație -> sursă, deoarece este un graf neorientat. După aceea, pe parcursul a K ani, folosindu-mă de un for, actualizez uzura care este diferită de 0, uzura care este 0 și calculez maximul de pe lângă fiecare nod din graf (adică maximul dintre uzurile cele mai apropiate de acel nod).

În funcția de updateUzura, actualizez toate uzurile care sunt diferite de 0, făcându-le ori 2. Pentru uzurile care sunt 0, parcurg fiecare nod și fiecare vecin, fac o copie la vectorul de uzuri, după care verific unde este 0 și apoi împart codul în 3 etape:
1. Valorile de 0 care nu se află la extremitățile vectorului de uzură. Pentru aceste valori, calculez maximul dintre stânga și dreapta valorii curente și pun în valoarea curentă de 0 maximul împărțit la 4.
2. Uzura de 0 de la începutul vectorului, iar pentru calcularea acesteia îi dau valoarea de lângă ea din același vector și după o compar cu maximul uzurii nodului de lângă.
3. Uzura de pe ultima poziție. Pentru calcularea acesteia, inițial îi dau valoarea din "stânga" vectorului de uzuri și apoi compar această valoare cu maximul uzurii salvat în nodul de lângă.

Pentru direcția inversă, fac același lucru doar că pentru începutul vectorului. Aceste 3 etape le repet în funcția updateZero. Toate aceste 3 funcții se repetă de K ani cu ajutorul unui for, după care pentru uzurile mai mari ca 100 le transform în 100 la loc și printez graful cu uzurile și tronsoanele care încă sunt bune. În final, eliberez memoria pentru 0 memory leak-uri.

## Task 2
---------------------------------------------------------------------------------------------------------------------------------

Pentru realizarea acestui task, am avut de gestionat o hartă formată din orașe și căi ferate, implementată cu un graf neorientat care are costul minim pe muchii. În primul rând, la începutul programului citesc datele și initializez harta (graful). Cu ajutorul algoritmului lui Dijkstra, calculez inițial costul minim al grafului, după care "dezactivez" câte o muchie și recalculez costul minim, apoi o "activez" la loc, verificând astfel impactul acelei muchii asupra costului minim.

În fiecare iterație, dezactivez câte o muchie diferită și folosesc algoritmul lui Dijkstra pentru a vedea acel impact dat de suma costurilor minime și de numărul de muchii folosite de Dijkstra. De asemenea, la fiecare Dijkstra, calculez numărul de muchii folosite și calculez un maxim deoarece de fiecare dată când Dijkstra găsește un arbore de cost minim cu mai multe muchii, șterg muchia pe care o dezactivasem. Dacă numărul de muchii este egal, calculez suma de costuri minime inițială și suma de costuri minime temporară și verific dacă sunt egale. Dacă sumele sunt egale, dau break la această căutare, adică la muchia pe care eram și o șterg.

Pentru fiecare caz, salvez noul vector de costuri minime rezultat de Dijkstra pentru a-l compara cu noul vector de cost minim. În final, pentru eliminarea muchiilor care mai pot fi eliminate după ce am ajuns la K, mă folosesc de un loop infinit în care verific în același mod fiecare muchie și calculez suma costurilor minime. Când suma costurilor minime din graful care a rezultat după primul while și suma costurilor minime temporare este egală, înseamnă că putem elimina muchia pe care am dezactivat-o. În final, dacă nu mai sunt egale sumele, ieșim din loop-ul infinit, ștergem propriu-zis toate muchiile dezactivate, afișăm orașele și muchiile rămase și eliberăm memoria.
