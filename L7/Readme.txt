A tenir en compte:
*Quan es selecciona una opció, la flecha continuarà parpadejant però una vegada es canvii de linea es veurà com ha quedat
seleccionada.
*Una vegada estan seleccionades les opcions de les tres files, apareix automaticament un missatge de confirmació, 
si es confirma la maquina comença a preparar el cafe i, si no, la maquina torna a la finestra de selecció.
*Si el nivell de l'aigua no és suficient, surt un misssatge i es bloqueja el programa fins un reset.


Funcionament del codi:
El nostre codi funciona amb una serie de maquines d'estat. Tenim una principal que depen del moment on ens trobem: pantalla de 
selecció, pantalla de confirmació, pantalla de preparació, pantalla final i pantalla d'error (nivell d'aigua).
Dins de cada un d'aquests estats tenim una serie de maquines d'estat per saber en quin moment i lloc ens trobem, per exemple, per la 
pantalla de selecció tenim un estat que indica en que columna ens trobem, és a dir, si la flecha de selecció está en la columna esquerra
(mínim), central(normal) o dreta(màxim). En el mateix estat de selecció tenim un altre estat per saber en quina fila ens trobem 
(tipus, dosi i temperatura).
A la pantalla de preparació tenim una maquina d'estats que ens serveix per indicar si estem en el temps de preparació del molinet, 
de l'aigua o si ja hem acabat.

Hem implementat dos tipus d'interrupcions, una d'alta prioritat a la cual accedim cada 0,1s per saber quant temps
falta de preparació, per implementar el parpadeig de la flecha i per implementar un sistema d'anti-rebots. 
L'altre interrupció, de baixa prioritat, s'encarrega de la comunicació linea-serie, tracta tant la rebuda com l'enviament 
d'informació, per enviar informació hem implementat un buffer circular de punters per guardar i llegir les lletres que s'han d'enviar.
Per calcular el duty del ventilador hem calculat la recta que pasa pels punts (-5,100) i (5,0), és a dir: y = -10x + 50;
on x és l'error calculat i y el valor del duty.



*Un error que ens hem trobat era sobre el tractament de l'error OERR (moltes entrades en poc temps), resetejant el bit CREN feia 
que el bit de l'error baixés però el flag de la rebuda d'informació estaba sempre a 1 perque el registre RCREG quedaba ple i 
entraba tota l'estona a la interrupció. Per solucionar aquest error, al tractament hem fet que s'executi RCREG 3 vegades per 
buidar el buffer i després ressetejar el bit CREN. A més, per fer que s'executi aquest tractament, a la RSI, si s'ha detectat 
aquest error, es deshabilita la RSI, d'altre manera tornaria a entrar a la RSI i no s'arribaria a executar mai el tractament.

