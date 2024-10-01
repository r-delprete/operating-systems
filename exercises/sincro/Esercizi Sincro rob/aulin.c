/*Da qualche parte vicino Napoli, c'è una barca a remi che viene usata sia dagli Informatici che dagli
Ingegneri Informatici per attraversare un fiume. La barca può contenere esattamente quattro persone e
non lascerà la riva con più o meno persone. Per garantire la sicurezza dei passeggeri, non è consentito
mettere un Informatico nella barca con tre Ingegneri, né un Ingegnere con tre Informatici. Qualsiasi
altra combinazione è sicura.
• Quando ogni thread sale sulla barca, deve invocare una funzione chiamata imbarco. È necessario
garantire che tutti e quattro i thread di ogni barca invochino imbarco prima che lo facciano i thread
della barca successiva.
• Dopo che tutti e quattro i thread hanno invocato imbarco, esattamente uno di loro deve chiamare una
funzione chiamata remiBarca, indicando che quel thread prenderà i remi. Non importa quale thread
chiami la funzione, purché uno lo faccia.
• Non ci si deve preoccupare della direzione di marcia. Supponiamo di essere interessati solo al traffico
che va in una delle direzioni*/


var
barca: int = 4;
remi_in_barca: semaforo = 0;
num_info: int = 0;
num_ing: int = 0;
mut: mutex;

informatici(){
    while(barca>0 && num_ing<3){
        lock(mut)
            imbarco();
            num_info++;
            barca--;
                if(barca==0){
            signal(remi_in_barca);
                }
        unlock(mut)
    }
}

ingegneri(){
    while(barca>0 && num_info<3){
        lock(mut)
            imbarco();
            num_ing++;
            barca--;
            if(barca==0){
            signal(remi_in_barca);
                }
        unlock(mut)
    }

}

remi(){
    wait(remi_in_barca);
    if(barca==0){
        printf(thread_corrente_prende_remi());
    }
}
