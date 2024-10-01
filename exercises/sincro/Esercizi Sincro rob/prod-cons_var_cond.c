/*• Risolvere il problema produttori-consumatori 
con N buffer usando
le variabili di condizione
• Scrivere separatamente le funzioni inserisci() 
e preleva() invocate dai
produttori e i consumatori, rispettivamente*/

var 
vuoto: varcond
pieno: varcond
mut: mutex
buffer[N]: int = 0
count: int //numer odi elementi attualmente nel buffer



produttore(){
    lock(mut);

        while (count<N){
            cond_wait(pieno,mut);
        }
        inserisci();
        cond_signal(vuoto);

    unlock(mut);
}

consumatore(){
    lock(mut);

        if (count==N){
            cond_wait(vuoto);
        }
        consuma();
        cond_signal(pieno);

    unlock(mut);
}


inserisci(){
   
    buffer[i]=elemento;
    i=(i+1)%N;
    count=count+1;    
}

consuma(){
    
    i=(i+1)%N;
    count=count-1;    
    
}

