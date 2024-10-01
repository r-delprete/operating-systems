/*Nella sala recuitment di DeepMind, durante una sessione di interview, possono prendere posto N candidati, 
mentre i restanti prenderanno parte alla sessione di interview successiva. Ogni candidato che ha trovato posto può 
richiedere ad un addetto dell'azienda un computer portatile tra gli M modelli a disposizione, considerando che per ogni 
modello ci sono K portatili. Quando tutti gli N candidati hanno ciascuno il poprio PC, hanno inizio le interview. 
Le sessioni terminano quando finiscono i candidati in attesa.*/

#define N
#define M
#define K

posto: semaforo = 10;
richiesta: semaforo = 0;
mut: mutex;
int portaili[M]=K;
start_interview: semaforo =0;
pc_ricevuto: semaforo=0;

candidato(){
    wait(posto);
    lock(mut)
    do{
    modello=rand()%M;
    }while(portatili[modello]!=0);
    signal(richiesta);
    unlock(mut);
    wait(pc_ricevuto)
    wait(start_interview);
    candidato-esegue_interviw();
    exit();S

}

addetto(){
    wait(richiesta);
    lock(mut)
     portatili[modello]-=1;
     candidati_pronti+=1;
     signal(scelta);
     unlock(mut);
    if(candidati_pronti==N){
        lock(mut)
        for(i=0 to N)
        signal(start_interviw);
        unlock(mut)
    }

}