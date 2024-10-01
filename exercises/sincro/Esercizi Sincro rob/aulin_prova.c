#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int barca = 4;
sem_t remi_in_barca;
int num_info = 0;
int num_ing = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void imbarco() {
    // La funzione di imbarco può essere implementata come vuota,
    // poiché la tua descrizione non specifica azioni particolari al momento dell'imbarco.
}

void *informatici(void *arg) {
    while (barca > 0 && num_ing < 3) {
        sleep(2);
        pthread_mutex_lock(&mut);
        imbarco();
        num_info++;
        barca--;

        if (barca == 0) {
            sem_post(&remi_in_barca);
        }

        pthread_mutex_unlock(&mut);
    }
    pthread_exit(NULL);
}

void *ingegneri(void *arg) {
    while (barca > 0 && num_info < 3) {
        sleep(2);
        pthread_mutex_lock(&mut);
        imbarco();
        num_ing++;
        barca--;

        if (barca == 0) {
            sem_post(&remi_in_barca);
        }

        pthread_mutex_unlock(&mut);
    }
    pthread_exit(NULL);
}

void *remi(void *arg) {
    sem_wait(&remi_in_barca);

    if (barca == 0) {
        printf("Thread corrente prende i remi ing : %d inf. %d \n", num_ing, num_info);
    }

    pthread_exit(NULL);
}

int main() {
    sem_init(&remi_in_barca, 0, 0);

    pthread_t thread_informatici[5];  // Creo 5 thread informatici
    pthread_t thread_ingegneri[5];    // Creo 5 thread ingegneri
    pthread_t thread_remi;


        // Creo i thread ingegneri
    for (int i = 0; i < 5; i++) {
        pthread_create(&thread_ingegneri[i], NULL, ingegneri, NULL);
        pthread_create(&thread_informatici[i], NULL, informatici, NULL);
    }





    pthread_create(&thread_remi, NULL, remi, NULL);

    // Attendo la terminazione dei thread informatici
    for (int i = 0; i < 5; i++) {
        pthread_join(thread_informatici[i], NULL);
        pthread_join(thread_ingegneri[i], NULL);

    }

    pthread_join(thread_remi, NULL);

    sem_destroy(&remi_in_barca);

    return 0;
}
