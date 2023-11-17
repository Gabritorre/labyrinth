# CPU algorithm

## General description

I tried to build my own algorithm to solve a given labyrinth with a decent score.
Since I didn't want to copy existing algorithms that solve mazes, I tried to write one on my own. I wasn't able to make it perfect (it may fail to solve some, and it doesn't achieve the best score), but I am still satisfied with it.

here is a brief explanation of how it works:

1. Analyze the map column by column in search of a target, which can be either a drill or a coin.
2. Once the coordinates of the target are found, the algorithm identifies whether that target is accessible horizontally (right or left) or vertically (up or down).
For example, if the target has walls on the left and right, then it is accessible vertically.
3. After doing this, the algorithm tries to align Snake with the target either vertically or horizontally, depending on the result of step 2. If, in the attempt to align, a wall is encountered, the algorithm tries to navigate around it.
	- For the circumvention, the algorithm 'launches' two ghosts in opposite directions, both attempting to see which side can bypass the wall. If both indicate that it's possible to bypass the wall, the algorithm chooses the more convenient route (determined based on the target's position). However, if both routes are not viable (leading to dead ends), then Snake backtracks by one step and places a flag '/' at that point and also on the paths taken by the ghosts, indicating that those points are not valid.
4. After aligning itself and verifying that there are no obstacles between Snake and the target, Snake moves directly toward the target.
5. Steps 1 to 4 are repeated until the map analysis is complete. Once finished, the exit of the labyrinth is set as the target, and steps 2 to 4 are executed one last time.

## algorithm limitations

Here are some notes and limitations that I am aware of in my algorithm:

1. Difficulty in handling dead ends:
With the implementation of the '/' flags to recognize dead ends, it is possible to navigate away from a dead end and explore other paths. However, in cases where the path leading to a dead end is sufficiently complex, it is possible for Snake to inadvertently trap itself in an area from which it cannot escape.
2. Self-eating by Snake:
Given the intrinsic characteristic of the algorithm to frequently backtrack, to avoid falling into infinite loops, I opted to allow Snake to eat its own tail, resulting in a loss of bonus points.
3. Panic counter:
To reduce the possibility of infinite loops, I implemented a 'panic counter', which is a step counter to reach a target. If this counter exceeds a maximum limit (determined based on the size of the map) to reach the target, the algorithm stops and is forced to reach the exit, ignoring any other remaining targets on the map.


# Spiegazione algoritmo cpu

## Descrizione generale

1. Analizza la mappa colonna per colonna alla ricerca di un target che può essere un trapano o una moneta.
2. Una volta prese le coordinate del target, l'algoritmo identifica se quel target è accessibile orizzontalmente (destra o sinistra) oppure verticalmente (alto o basso).
3. Fatto ciò l'algoritmo cerca di allineare Snake con il target verticalmente oppure orizzontalmente in base al risultato del passaggio 2. Se nel tentativo di allinearsi viene incontrato un muro l'algoritmo cerca di raggirarlo.
	- Per il raggiramento l'algoritmo "lancia" due fantasmi in direzioni opposte i quali provano a vedere da quale parte si riesce a raggirare il muro, se entrambi dicono che è possibile raggirare il muro, viene scelta la strada più conveniente (in base alla posizione del target). Se invece entrambe le strade non sono possibile (portano a vicoli ciechi) allora Il personaggio fa un passo nella direzione opposta alla quale ha incontrato il muro e posizione un flag '/' in quel punto e anche nelle strade fatte dai fantasmi, ad indicare che quei punti non sono validi.
4. Dopo essersi allineato ed aver verificato che non ci sono ostacoli tra Snake e il target, Snake si dirige dritto verso il target
5. I passi da 1 a 4 vengono ripetuti fino a che l'analisi della mappa non è terminata. Una volta terminata viene impostato come target l'uscita del labirinto e vengono eseguiti per un'ultima volta i passaggi da 2 a 4

## Limiti dell'algoritmo

Questo algoritmo presenta però alcune limitazioni:

1. Difficoltà nel gestire i vicoli ciechi: 
Con l'implementazione dei flag '/' per riconoscere i vicoli ciechi è possibile riuscire ad abbandonare il vicolo cieco e provare altre strade ma nel caso in cui il vicolo cieco sia abbastanza complesso è possibile che Snake si auto-chiuda in una zona in cui non riesce più ad uscire.
2. Snake si auto-mangia la coda: 
data la caratteristica intrinseca dell'algoritmo di tornare spesso sui propri passi, per evitare di incappare in dei loop infiniti ho preferito lasciare che Snake si mangi la coda perdendo così dei punti bonus.
3. Panic counter:
Per ridurre la possibilità di cicli infiniti è stato implementato un panic counter, cioè un contatore di passi per raggiungere un target, se questo counter va oltre un limite massimo (determinato in base alle grandezze della mappa) per raggiungere il target, l'algoritmo si ferma e viene forzato a raggiungere l'uscita (ignorando tutti gli altri eventuali target ancora presenti nella mappa).
