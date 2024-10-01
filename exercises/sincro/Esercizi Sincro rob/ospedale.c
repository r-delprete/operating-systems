/* In un ambulatorio medico lavorano due medici ed un infermiere. Inizialmente i due medici sono in attesa
dell'arrivo dei pazienti. Un paziente entra nell'ambulatorio e si reca dal primo medico libero che esegue la visita medica. 
Al termine della visita, il medico redige un referto che inserisce in un portadocumenti con M posizioni.
L'infermiere preleva un referto alla volta e lo inserisce nel database dell'ambulatorio. Se entrambi i medici sono impegnati, il paziente
 si accomoda nella sala d'aspetto che dispone di N sedie. Se le N sedie sono tutte occupate, il paziente lascia l'ambulatorio.
Discutere la soluzione proposta in termini di possibili situazioni di starvation/deadlock e, nel caso, si propongano soluzioni ammissibili. */


#define N

var:s
attesa_paziente: semaforo = 0;
sedie: int = N;
medico_libero: semaforo = 0;
med_flag: int = 2;
portadocumenti: semaforo_contatore=M;
mut: mutex;
mut_sedie: mutex;
mut_med: mutex;

medico(){
    signal(medico_libero);
    wait(attesa_paziente);
    esegue_visita();
    lock(mut_med);
    med_flag++;
    unlock(mut_med);
    wait(portadocumenti);
}

paziente(){
    if(med_flag==0 && sedie>0){
        lock(mut_sedie);
        sedie--;
        unlock(mut_sedie);
    wait(medico_libero);
    lock(mut_sedie);
    sedie++;
    unlock(mute_sedie);
    }else if(sedie == 0){
        exit();
    }
    lock(mut);
    med_flag--;
    unlock(mut);
    signal(attesa_paziente);
    
}

inferimere(){
    signal(portadocumenti);
    inserimento_referto();
}