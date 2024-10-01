/*
 
Nella sala recruitment di una grande azienda informatica, durante una sessione di interview possono prendere posto al più N candidati,
 mentre i restanti candidati prenderanno parte alla sessione di interview successiva.
 Ogni candidato che ha trovato posto sarà chiamato al tavolo delle interview.
 L'addetto alle risorse umane attende fino che è disponibile un candidato. 
Terminata l'intervista II candidato lascia il tavolo e la sala facendo posto al prossimo candidato.
 Le sessioni di interview terminano quando non ci sono più candidati in attesa. Proporre una soluzione e discutere eventuali problemi di deadlock.

*/

#define M // candidati
#define N //posti disponibili

//M > N

candidato : semaforo = 0;
turno_interview : semaforo = 1;
fine_interview : semaforo = 0;

posti_sessione: semaforo = N;
posti_occupati : int = 0

colloqui_terminati: int = 0;

mutex_colloqui : mutex = 1;
mutex_posti_occupati : mutex = 1

candidato () {
    
    wait(posti_sessione)
    
    lock(mutex_posti_occupati)
    
    signal(candidato) //ci sono
    
    wait(turno_interview) //aspetto di poter fare il colloquio
    
    wait(fine_interview) //aspetto che finisca il colloquio
    
    signal(turno_interview) //dico al prossimo che puo entrare
    
    lock(mutex_colloqui)
    colloqui_terminati++;
    unlock(mutex_colloqui)
    
    exit(); //me ne vado, sperando di essere assunto
    
}

risorse_umane () {
        
    while(1){
        while(1) {
            wait(candidato) //aspetto il candidato
        
            sleep(5) //simulazione del colloquio
        
            signal(fine_interview)//segnala la fine dell intervista al candidato
            
            lock(mutex_colloqui)
            if(colloqui_terminati == N){
                unlock(mutex_colloqui)
                break;
            }
        }
        
        for i:0 to N {
            signal(posti_sessione);
        }
        
        lock(mutex_colloqui)
        colloqui_terminati = 0
        unlock(mutex_colloqui)
    }
}
