/*
   In una pizzeria, due pizzaioli sfornano continuamente pizze che
 pongono di volta in volta in uno di N piatti
   disponibili su un bancone. I piatti sono prelevati da M (M<N)
 camerieri che provvedono a servirle ai tavoli.
   Ciascun cameriere può portare 2 pizze per volta, se disponibili, ma, 
in caso di affollamento, i camerieri
   preleveranno 3 pizze per volta. Fornire una soluzione con semafori e
 discutere eventuali problemi di
   starvation/deadlock.

*/


#define N;
#define M;
#define K;

piatti[N]: int = 0;
pieno: semaforo = 0;
vuoto: semaforo = N;
affollato: bool = false;
tavoli_disponibili: int = K;

pizzaiolo(){
    produci_pizza();
    wait(vuoto);
 lock(mut)
    bancone[i]=elemento;
    i=(i+1)%N;
unlock(mut)
signal(pieno);
}

cameriere(){
    wait(pieno);
lock(mut)
    if(affollato==true)
    porta_pizze(3);
    else
    porta_pizze(2);
unlock(mut)
   signal(vuoto);
}


cliente(){
    lock(mut)
if(tavoli_disponibili>0){
    tavoli_disponibili--;
}else{
    exit();
}
 if(tavoli_disponibili<3){
        affollato=true;
 }
    unlock(mut)
}

