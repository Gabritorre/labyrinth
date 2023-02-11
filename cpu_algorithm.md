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
