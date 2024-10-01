/* In un ambulatorio medico lavorano due medici ed un infermiere. Inizialmente i due medici sono in attesa
dell'arrivo dei pazienti. Un paziente entra nell'ambulatorio e si reca dal primo medico libero che esegue la visita medica. Al termine della visita, il medico
 redige un referto che inserisce in un portadocumenti con M posizioni.
L'infermiere preleva un referto alla volta e lo inserisce nel database dell'ambulatorio. Se entrambi i medici sono impegnati, il paziente
 si accomoda nella sala d'aspetto che dispone di N sedie. Se le N sedie sono tutte occupate, il paziente lascia l'ambulatorio.
Discutere la soluzione proposta in termini di possibili situazioni di starvation/deadlock e, nel caso, si propongano soluzioni ammissibili. */

#define N //sedie
#define M
coda: int =0
array[M]: Portadocumenti 
indice: int = 0
ind_med: int = 0

cons: semaforo =0
prod: semaforo =1
paz: semaforo = 0
med: semaforo = 0
ambulatorio: semaforo = 0



mutex_referto: mutex 
mutex_occupato: mutex

infermiere(){
	while(1){
		wait(cons) //0
		lock(mutex_referto)
		ins_database(array,indice) //l'infermiere consuma il referto
		indice=(indice+1)mod M //indice portadocumenti per l'array circolare 
		unlock(mutex_referto)
		signal(prod)
	}
}

medico(){
	
	while(1){
		
		wait(paz)
		
		
		//inizia visita
		signal(med)
		
		wait(prod) //1 all'inizio. Se l'infermiere non ha finito di sistemare il referto non continuo
		
		lock(mutex_referto)
		crea_referto(array,indice) //funzione che crea il referto e lo inserisce nel portadocumenti
		unlock(mutex_referto)
		
		signal(cons) //segnalo all'infermiere (che è il consumatore) che il medico ha prodotto un referto
		
		lock(mutex_occupato)
		
		if(coda>0){ //se c'è qualcuno seduto lo sveglio con una signal e continuo a risultare occupato per gli altri pazienti tranne per quello che ho sbloccato
			signal(ambulatorio) 
		}else //se non ci sono pazienti seduti il medico può risultare come non occupato
			ind_med--
		unlock(mutex_occupato)
		
	}
	
}

paziente(){
	
	lock(mutex_occupato)
	if(coda<N && ind_med == 2){ //se c'è posto per mettersi seduti e i medici sono occupati
		coda++
		unlock(mutex_occupato)
		wait(ambulatorio) //aspetto il tempo che un medico si liberi
	    lock(mutex_occupato)
		//non faccio ind_med++ perché il medico già risulta occupato visto che ha sbloccato un paziente seduto, così non rischio che entri un altro
		//cliente che occupi il mutex prima del cliente sbloccato dalla sedia
		coda--
		unlock(mutex_occupato)	
		signal(paz)		//segnalo al medico che ci sono
		wait(med)		//aspetto l'inizio della visita
		sleep(1) //fa la visita
		exit()//esce il paziente
	}
	else if(ind_med<2){ //se è libero il medico me lo prendo io 
		ind_med++		//occupo il medico			
		unlock(mutex_occupato)
		signal(paz)		//segnalo al medico che ci sono
		wait(med)		//aspetto il medico per l'inizio della visita
		sleep(1)//fa la visita
		exit() //esce il paziente perché ha finito la visita
	}
	else {
		//VAI VIA 
		unlock(mutex_occupato)
		exit() //esce il paziente
	}
		
}
