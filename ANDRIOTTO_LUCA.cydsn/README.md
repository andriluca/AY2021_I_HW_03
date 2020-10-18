# Assignment \#3

Il controllore comunica con un programma su PC.
L'interfaccia impiegata consente, verificata la connessione del microcontrollore, l'impostazione dei tre differenti valori di intensità luminosa (rossa, verde e blu).

## Macchina a stati

È necessario suddividere in stati l'algoritmo:

- S0: _Handshake_
- S1.1: _Pre-Idle_
- S1.2: _Idle_
- S2: _Inizio lettura_
- S3: _Lettura Red_
- S4: _Lettura Green_
- S5: _Lettura Blue_
- S6: _Fine lettura_

_NB_: per ulteriori informazioni riguardanti le variabili e procedure coinvolte fare riferimento al file "states.c".

### Handshake

La connessione tra i due dispositivi viene riconosciuta nel momento in cui il PC invia un carattere al controllore e quest'ultimo risponde con una determinata stringa, una keyword.
Uno stato momentaneo e iniziale che garantisce il funzionamento dell'apparato.

### Pre-Idle

Lo stato che introduce all'idle vero e proprio.
Fornisce l'inizializzazione delle variabili in modo tale da rendere automatica l'esecuzione dello stato successivo.

### Inizializzazione packet

Il primo byte viene letto e il contatore di byte ricevuti è inizializzato a zero. Quest'ultimo è necessario per poter memorizzare opportunemente i successivi tre byte.

### Lettura RGB

I tre byte vengono letti e memorizzati.

### Termine lettura

Quando il carattere tail viene letto i valori vengono scritti nelle compare delle rispettive PWM.

## Timing

L'aspetto temporale è predominante in questo progetto. I dati vengono inviati serialmente dal computer, quindi è necessario stabilire, oltre ad un timeout massimo un minimo valore temporale al di sotto del quale i dati non possono essere registrati in memoria. Questo è il limite principale dell'hardware. Facendo vari test con coolterm ho ricavato empiricamente che il limite inferiore si aggira all'incirca su 10 ms.

## Impostazione del colore

L'ultimo stato ha come fine ultimo la scrittura dei valori di compare su due differenti PWM (la prima per rosso e verde e la seconda per il blu).
Il periodo è stato impostato nell'ordine dei kHz in modo tale da alternare rapidamente fasi di accensione e fase di spegnimento, in modo indistinguibile dall'occhio umano. In questo modo l'intensità luminosa dipende dal tempo medio di attivazione dei channel.
