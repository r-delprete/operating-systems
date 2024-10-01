/*
   Nella sala prove del giovane talentuoso chitarrista
 Matteo Mancuso possono entrare e trovare posto seduto 
al più N appassionati chitarristi per una Masterclass con Matteo.
 I partecipanti rimasti in piedi seguiranno il seminario successivo. 
Ogni allievo che ha trovato posto può richiedere ad un collaboratore 
di Matteo Mancuso un modello di chitarra degli M modelli messi a 
disposizione dalla casa produttrice, di cui Matteo Mancuso è 
testimonial, considerando che per ogni modello ci sono K chitarre.
 Quando tutti gli N allievi hanno la propria chitarra, Matteo suona
 un pezzo semplice chiedendo a tutti gli  allievi di suonarlo insieme
 a lui. La masterclass termina quando non ci sono più chitarristi in
 attesa
*/

#define N // posti masterclass

#define M //modelli chitarra
#define K //numero chitarre per modello

chitarre[M] : semaforo = {K, K, K . . .}

appassionato : semaforo = 0
posti_disponibili : semaforo = N
inizio_lezione : semaforo = 0
fine_lezione : semaforo = 0
turno: semaforo = 0
ready: semaforo = 0
prossima_lezione: semaforo = 0
accesso_chitarre : mutex = 1
accesso_posti_occupati : mutex = 1

posti_occupati: int = 0

//M*K == N

processo_Matteo_Mancuso () {
    while(1){
        
        wait(ready) //aspetta che i posti siano occupati
        
        for i:0 to N
            signal(inizio_lezione) //segnala a tutti che è disponibile a iniziare
            
        sleep(10) //lezione
            
            for i:0 to N
                signal(fine_lezione) //segnala la fine della lezione
                
        signal(prossima_lezione)
    }
    
}

processo_appassionato () {
    signal(appassionato) //segnala la presenza
    wait(turno) //aspetta di poter entrare
    
    signal(richiesta) //richiede al collaboratore di aiutarlo con una chitarra
    wait(risposta) //aspetta di essere aiutato
    lock(accesso_chitarre) //locka l'accesso alle chitarre
    modello: int = select_modello() //la function restituisce l'indice del primo semaforo > 0 che trova, in modo                              //da non causare starvation
    wait(chitarre[modello]) //esegue la wait su quella chitarra, dato che sicuramente il semaforo è maggiore di 0
                            //e ci sono chitarre per tutti il programma non andrà in deadlock
    unlock(accesso_chitarre) //unlocka l'accesso alle chitarre
    
    wait(inizio_lezione) //aspetta che inizi la lezione . . .
    wait(fine_lezione)//. . . e che finisca
    
    signal(chitarre[modello]) //dato che ogni processo opera su una specifica porzione di array non è necessario
                              //lockare il mutex

    exit()//se ne va a caso contento
}

processo_collaboratore_accesso (){
    
    while(1){
        wait(appassionato) //aspetta che ci siano appassionati
        wait(posti_disponibili) //verifica ci siano posti
        signal(turno) //segnala che si puo entrare
        
        posti_occupati++
        
        if(posti_occupati == N){ //se la sala è piena . . .
            signal(ready)//. . . segnala che si puo iniziare
            wait(prossima_lezione) // aspetta che finisca la lezione
        }
        
        if(semGetValue(appassionato) == 0){ //se non ci sono appassionati in fila. . .
            if(posti_occupati > 0){ //. . . inizia una nuova lezione se ci sono persone a sedere
                signal(ready)
                wait(prossima_lezione)//aspetta che la lezione finisca . . .
                exit(0)//. . . e poi manda tutti a casa . . .
            }
            
            else
                exit(0)//. . . oppure chiude la baracca se non c'è nessuno in fila
        }
    }
}

processo_collaboratore_chitarre (){
    
    while(1){
        wait(richiesta)
        signal(risposta)
    }
}

