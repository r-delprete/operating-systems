/* In una fabbrica, N operai preparano piastrelle 
da far cuocere in un
forno, capace di cuocerne M contemporaneamente
• All'uscita dal forno K operai visionano le 
piastrelle per decorarle
secondo tale sequenza di passi: se trova 
una piastrella difettata ne
prende dal forno 2 alla volta, altrimenti 
ne prende 1 alla volta.
• Individuare i processi/thread da modellare
 e scriverne lo pseudocodice*/


var
vuoto: varcond;
pieno: varcond;
mut: mutex;
buffer[M]: int=0
count: int

main(){
    for(i=0;i<N;i++){
        operai_produttori();
    }

    for(i=0;i<K;i++){
        operai_consumatori();
    }
}

 operai_produttori(){
    lock(mut)

        while(count<M){
            cond_wait(pieno,mut);
        }
        preparazione();
        cond_signal(vuoto);
    unlock(mut)
 }

 operai_consumatori(){
    lock(mut)

        if(count==M)[
            cond_wait(vuoto,mut);
        ]
        decorazione();
        cond_signal(pieno);

    unlock(mut)
 }


 preparazione(){
    buffer[i]=rand()%2;
    i=(i+1)%M;
    count=count+1;
 }

 decorazione(){
    if(buffer[i]==1){  //se è uguale a 1 la piastrella è difettata
    i=(i+1)%M;
    count=count-2;
    }else{
    i=(i+1)%M;
    count=count-1;
    }

 }