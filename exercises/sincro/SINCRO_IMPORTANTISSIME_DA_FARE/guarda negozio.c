//guardia : Si prende cura di far entrare un numero predefinito di persone per volta (i.e.: 1,2,N) all’interno del negozio, e all’uscita di queste ripete il suo ciclo
 
//persone : Aspettano il loro turno per l’entrata nel negozio, fanno compere all’interno di esso e poi escono, segnalando l’uscita

#define N

persona: semaforo = 0;
posti_disponibili: semaforo = N;
turno: semaforo = 0;
guardia: semaforo = 0;

guardia () {
    while(1){ // si mette while(1) perché la guardia esegue queste cose in loop
        wait(persona);//aspetta arrivino persone
        
        wait(posti_disponibili);//aspetta che ci siano posti disponibili
        
        signal(turno); //segnala che ci sono posti disponibili
    }
}

persona () {
    signal(persona);//segnala la presenza
    
    wait(turno); //aspetta educatamente il turno
    
    doshopping(); //compra delle air force one
    
    signal(posti_disponibili); //segnala l'uscita
    
    exit(); //se ne va
}

int main () {
    crea_processo(guardia);
    
    for i = 0 to K:
        crea_processo(persona);
}

