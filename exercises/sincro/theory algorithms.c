// Produttore e consumatore
const n; 		// numero di buffer
type item;
buffer[0 … n - 1] di item;
full: semaforo = 0;
empty: semaforo = n;
int prod = 0, cons = 0; 		/* produttore e consumatore */

produttore() 
{
  wait(empty);
  buffer[prod] = { produce };
  prod = prod + 1 % n;
  signal(full);
  { resto del ciclo }
}

consumatore()
{
  wait(full);
  x = buffer[cons] { consuma };
  cons = cons + 1 % n;
  signal(empty);
  { resto del ciclo }
}

// Lettore e scrittore con priorità ai lettori
int num_lettori;
mutex: semaforo = 1;
dati: semaforo = 1;

lettore()
{
  wait(mutex);
  num_lettori++;
  if (num_lettori == 1)
  {
    wait(dati); 	/* se è il primo lettore, ferma gli scrittori */
  }
  signal(mutex);
  { legge }
  wait(mutex);
  num_lettori—-;
  if (num_lettori == 0)
  {
    signal(dati);
  }
  signal(mutex);
}

scrittore()
{
  begin
    wait(dati);
    { scrive }
    signal(dati);
  end

  /**
   * questo blocco risveglia un lettore o uno scrittore e vengono eseguiti concorrentemente
   */
  begin
    num_lettori = 0;
    lettore();
    scrittore();
  end
}

// Lettore e scrittore con priorità agli scrittori
int num_lettori, num_scrittori;
&mutex1: semaforo = 1;
&mutex2: semaforo = 1;
&unoallavolta: semaforo = 1;
&lettura: semaforo =  1;
&scrittura: semaforo = 1;

lettore()
{
  wait(unoallavolta);
  wait(lettura);
  wait(mutex2)
  num_lettori++;
  if (num_lettori == 1)
  {
    wait(scrittura);
  }
  signal(mutex2);
  signal(lettura);
  signal(unoallavolta)
  { legge }
  wait(mutex2)
  num_lettori—;
  if (num_lettore == 0)
  {
    signal(&scrittura);
  }
  signal(mutex2);
}

scrittore()
{
  begin
    wait(mutex1);
    num_scrittori++;
    if (num_scrittori == 1)
    {
      wait(lettura);
    }
    signal(mutex1);
    wait(scrittura);
    { scrive }
    signal(scrittura);
    wait(mutex1);
    num_scrittori—;
    if(num_scrittori == 0)
    {
      signal(lettura);
    }
    signal(mutex1);
  end

  begin
    num_lettori = num_scrittori = 0;
    lettore();
    scrittore();
  end
}

// Barbiere addormentato
int sedie = n;
int coda;
mutex: semaforo = 1;
cliente: semaforo = 0;
barbiere: semaforo = 0;

barbiere()
{
  wait(cliente);
  wait(mutex);
  coda—-;
  signal(barbiere);
  signal(mutex);
  { esegue taglio }
}

cliente()
{
  wait(mutex);
  if (coda < sedie) /* se c’è posto */
  {
    coda++;
    signal(mutex);
    signal(cliente);
    wait(barbiere);
    { esegue taglio }
  } 
  else 
  {
    signal(mutex); 	/* va via */
  }
}