/*
  In un negozio di scarpe c’è un addetto che aiuta i clienti 
  a provare   i modelli di scarpe scelti. L’addetto può solo
  aiutare un cliente per volta di K clienti che possono attendere
  in negozio, seduti su M sedie (K > M). Quando entra
  un nuovo cliente, attende su una sedia libera, altrimenti lascia 
  il   negozio. L’addetto, in mancanza di clienti,
  provvede ad aggiornare il database dei nuovi modelli di 
  scarpe arrivati il giorno prima.   
*/


#define K
#define M

sem_cliente: semaforo=1;
num_clienti: int=0;



addetto(){
    wait(cliente_disp);
    wait(sem_cliente);
    num_clienti--;
    signal(servi_cliente);
        if(num_clienti==0){
            aggiorna_database();
        }
        signal(sem_cliente);
}






cliente(){
        wait(sem_cliente);

    if(num_clienti<M){
        num_clienti++;
        signal(cliente_disp);
        signal(sem_cliente);
        wait(servi_cliente);
        misura_scarpe();
    }else{
        signal(sem_cliente);
    }
}