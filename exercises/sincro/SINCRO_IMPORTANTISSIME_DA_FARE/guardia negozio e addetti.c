//guardia: Si prende cura di far entrare un numero predefinito di persone per volta (i.e.: 1,2,N) all’interno del negozio, e all’uscita di queste ripete il suo ciclo

//persone: Aspettano il loro turno per l’entrata nel negozio, fanno compere all’internodi esso e poi escono, segnalando l’uscita . . . e una volta entrati, aspettano che un commesso li serva; appena serviti, escono dal negozio e segnalano l’uscita.

//addetti : Appena liberi, servono i clienti ed accedono ad una mensola di scarpe per trovare il paio di scarpe che richiedono.

//assumiamo che arrivino K persone, i posti disponibili siano 2 e che inizialmente ci sono due addetti disponibili


#define M //scarpe sullo scaffale

guardia: semaforo = 0;
addetto: semaforo = 2; //inizializzato a 2 perché inizialmente ci sono due addetti a disposizione
persona: semaforo = 0;

posti_disponibili: semaforo = 2;
turno: semaforo = 0;
richiesta: semaforo = 0; //semaforo per la richiesta di un commesso

mutex_scaffale: mutex;

scaffale : array[M];

guardia () { //identico
    while(1){
        wait(persone);
        
        wait(posti_disponibili);
        
        signal(turno);
    }
}

persone() {
    signal(persona);
    
    wait(turno);
    
    signal(richiesta); //una volta entrato segnala la sua presenza agli addetti
    
    wait(addetto); //aspetta l'addetto
    
    doshopping(scaffale,rand()); //compra le air force one
    
    signal(posti_disponibili); //uscendo segnala che ci sono nuovi posti liberi
    
    exit(); //esce
}

addetto () {
    while(1){
        wait(richiesta); //aspetta la richiesta di un cliente
        
        lock(mutex_scaffale);
        prendi_scarpe(scaffale); //esegue l'accesso in MutEx allo scaffale, seleziona le scarpe desiderate, e le elimina dall'array. Successivamente le sblocca il MutEx
        unlock(mutex_scaffale);
        
        signal(addetto); //segnala che è di nuovo disponibile
    }
}
