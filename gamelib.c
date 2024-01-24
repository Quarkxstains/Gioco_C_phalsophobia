#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "gamelib.h"

//imposta_gioco()
static int Numero_giocatori(void);
static void Nomi_giocatori(void);
static int Inizializza_giocatori(void);
static void Dealloca_giocatori(void);
static void Nomi_giocatori_stampa(void);
static void Sanita_mentale(void);
static int Difficolta(void);
static void Assegnazione_oggetti(void);
static int seleziona_giocatore_casuale(bool giocatori_da_scegliere[]);
static int inserisci_singola_zona(void);
static int inserisci_tutte_le_zone(void);//funzione extra che aggiunge subito 6 zone, non aggiunge mai 2 zone uguali, non aggiuge mai caravan
static int random_zona(bool zona_creata[]);
static void modifica_zona_casualmente(void);//funzione extra che modifica una zona, se presente, casualmente, non crea nessuna nuova zona
static void cancella_ultima_zona(void);
static void cancella_tutte_le_zone(void);//funzione extra per cancellare tutte le zone inserite
static void stampa_mappa(void);
static int chiudi_mappa(void);
static const char* nome_enum_display_a_parole(int);

//gioca()
static int* ordine_giocatori_casuale(void);
static void gioca_turno(int, int array_avanzamento_giocatori[], int);
static bool tutti_giocatori_senza_sanita(void);
static void controllo_zaino_giocatori(int);//funzione extra per controllare l'effettiva possibilità a proseguire nel gioco
static int torna_caravan(int);
static int oggetto_essenziale_prova_false(void);
static void stampa_giocatore(int);
static void stampa_zona(int);
static void avanza(int, int array_avanzamento_giocatori[], int);
static void raccogli_prova(int);
static void Apparizzione_fantasma(int, int);
static int Numero_prove_trovate(void);
static void probabilita_decremento_sanita(int);
static void raccogli_oggetto(int);
static void usa_oggetto(int, int array_avanzamento_giocatori[]);
static int rimozione_oggetto_zaino(int);//funzione extra per rimuovere un oggetto scelto dall'utente tra quelli disponibili nello zaino
static void Vittoria(void);
static void Sconfitta(void);
static int numero_giocatori_vivi(void);

//termina_gioco()
static void menu_titolo(void);//puro menù grafico

//variabili globali
bool mappa_creata = false;
bool mappa_implementata=false;
int numero_giocatori=0;
struct Giocatore** giocatori = NULL;//array di puntatori alla struct Giocatore
int difficolta=0;
int turno=0;
struct Zona_mappa* pFirst = NULL;
struct Zona_mappa* pLast = NULL;
struct Zona_mappa* zona=NULL;
struct Zona_mappa* zona_caravan=NULL;
bool presenza_fantasma=false;
bool creazione_zone_mappa[7];
bool trova_prova_EMF=false;
bool trova_prova_spirit_box=false;
bool trova_prova_videocamera=false;
bool protezione_fantasma[4];


//stampa solo la scrittta phasmophobia, senza il menù di scelta
void menu_titolo(void){
printf(" ██▓███   ██░ ██  ▄▄▄       ██▓      ██████  ▒█████   ██▓███   ██░ ██  ▒█████   ▄▄▄▄    ██▓ ▄▄▄      \n");
printf("▓██░  ██▒▓██░ ██▒▒████▄    ▓██▒    ▒██    ▒ ▒██▒  ██▒▓██░  ██▒▓██░ ██▒▒██▒  ██▒▓█████▄ ▓██▒▒████▄    \n");
printf("▓██░ ██▓▒▒██▀▀██░▒██  ▀█▄  ▒██░    ░ ▓██▄   ▒██░  ██▒▓██░ ██▓▒▒██▀▀██░▒██░  ██▒▒██▒ ▄██▒██▒▒██  ▀█▄  \n");
printf("▒██▄█▓▒ ▒░▓█ ░██ ░██▄▄▄▄██ ▒██░      ▒   ██▒▒██   ██░▒██▄█▓▒ ▒░▓█ ░██ ▒██   ██░▒██░█▀  ░██░░██▄▄▄▄██ \n");
printf("▒██▒ ░  ░░▓█▒░██▓ ▓█   ▓██▒░██████▒▒██████▒▒░ ████▓▒░▒██▒ ░  ░░▓█▒░██▓░ ████▓▒░░▓█  ▀█▓░██░ ▓█   ▓██▒\n");
printf("▒▓▒░ ░  ░ ▒ ░░▒░▒ ▒▒   ▓▒█░░ ▒░▓  ░▒ ▒▓▒ ▒ ░░ ▒░▒░▒░ ▒▓▒░ ░  ░ ▒ ░░▒░▒░ ▒░▒░▒░ ░▒▓███▀▒░▓   ▒▒   ▓▒█░\n");
printf("░▒ ░      ▒ ░▒░ ░  ▒   ▒▒ ░░ ░ ▒  ░░ ░▒  ░ ░  ░ ▒ ▒░ ░▒ ░      ▒ ░▒░ ░  ░ ▒ ▒░ ▒░▒   ░  ▒ ░  ▒   ▒▒ ░\n");
printf("░░        ░  ░░ ░  ░   ▒     ░ ░   ░  ░  ░  ░ ░ ░ ▒  ░░        ░  ░░ ░░ ░ ░ ▒   ░    ░  ▒ ░  ░   ▒   \n");
printf("          ░  ░  ░      ░  ░    ░  ░      ░      ░ ░            ░  ░  ░    ░ ░   ░       ░        ░  ░\n");
printf("                                                                                     ░               \n");

}


//funzione che stampa il menù principale
void menu_principale(void){
  menu_titolo();
  printf("Si prega di inserire un numero da tastiera:\n\t[1] per impostare il gioco;\n\t[2] per giocare;\n\t[3] per terminare il programma\n");
}


/////////////////////////////
void imposta_gioco(void){
  if(giocatori!=NULL)//se precedentemente sono stati inseriti(e quindi anche allocati) dei giocatori, si procede a deallocarli
  Dealloca_giocatori();

  if(mappa_creata)//se precedentemente è stata creata una mappa occorre prima rimuovere i dati creati
  cancella_tutte_le_zone();

  if(zona_caravan!=NULL){//se precedentemente è stata creata una zona caravan, essa va cancellata
    free(zona_caravan);
    zona_caravan=NULL;
  }

  /*numero_giocatori è una variabile globale, in quanto se precedentemente
  sono stati allocati dei giocatori voglio deallocarne tanti quanti sono stati allocati in precedenza*/
  numero_giocatori=Numero_giocatori();/*diamo alla variabile numero_giocatori
  il valore della funzione che calcola e restituisce il numero di giocatori che l'utente inserisce da tastiera*/
  system("clear");
  printf("il numero di giocatori che prenderanno parte alla partita è [%d]\n", numero_giocatori);

  int return_inizializzazione_giocatori =Inizializza_giocatori();/*funzione che crea in memoria dinamica l'array di puntatori e l'array che tiene traccia
  dei singoli giocatori*/

  if(return_inizializzazione_giocatori==-1){//restituisce -1 solo se abbiamo problemi ad allocare memoria
    return;
  }

  Nomi_giocatori();/*funzione che riempie i campi Nome_giocatore di ogni giocatore,
  sono stati effettuati numerosi controlli in vista di qualche piccolo problemino di spaziatura
  e di stampa, ora tutti risolti*/
  system("clear");

  Nomi_giocatori_stampa();//funzione che stampa i nomi dei giocatori precedentemente inseriti.

  Sanita_mentale();//funzione che assegna a tutti i giocatori una quantità base di sanità mentale paria 100.

  difficolta=Difficolta();/*diamo alla variabile difficolta il valore della funzione
  che mi restituisce la difficoltà selezionata dall'utente*/

  //funzione che assegna gli oggetti ai giocatori facendoli scegliere tra quelli che escono casualmente
  //(basandosi sul numero dei giocatori)
  Assegnazione_oggetti();


  //menù di scelta per il 'game master'
  int scelta;
  do {

    printf("\nMenu Creazione Mappa:\n");

    printf("\t[1] Creazione di una singola zona per la mappa\n");
    printf("\t[2] Creazione di 6 zone mappa(crea tutte le zone tranne il caravan, così che il comando [3] modifichi sempre 1 zona)\n");

    printf("\t[3] Modifica zona casuale(verrà scelta una zona casuale e verrè modificato il suo campo \"oggetto\"\n(se la zona estratta casualmente non è presente allora non avverrà nessuna modifica)\n");
    printf("\t[4] Cancella l'ultima zona inserita\n");

    printf("\t[5] Cancella tutte le zone inserite fino ad ora\n");
    printf("\t[6] Stampa mappa(fa vedere tutte le zone create sino ad ora)\n");

    printf("\t[7] Chiudi mappa(possibile solo se è stata creata almeno una zona)\n");

    printf("La tua scelta è: ");

    char temp[100];

    fgets(temp, sizeof(temp), stdin);

    scelta = atoi(temp);

    switch (scelta) {

      case 1:

      system("clear");
      int return_inserisci_singola_zona=inserisci_singola_zona();

      if(return_inserisci_singola_zona==-1){
        return;
      }

      break;


      case 2:
      system("clear");
      int return_inserisci_tutte_le_zone=inserisci_tutte_le_zone();

      if(return_inserisci_tutte_le_zone==-1){
        return;
      }

      break;


      case 3:

      system("clear");
      modifica_zona_casualmente();

      break;


      case 4:
      system("clear");
      cancella_ultima_zona();
      break;


      case 5:
      cancella_tutte_le_zone();
      break;


      case 6:
      stampa_mappa();
      break;


      case 7:
      int return_chiudi_mappa=chiudi_mappa();

      if(return_chiudi_mappa==-1){
        return;
      }

      break;


      default:
      system("clear");
      printf("Scelta non valida, riprovare.\n");
      break;

    }
  } while(!mappa_creata);
}//
/////////////////////////////


//funzione che delloca la memoria dei giocatori
static void Dealloca_giocatori(void) {

  printf("deallocazione giocatori in corso...\n");
  sleep(1);

  for (int i = 0; i < numero_giocatori; i++) {

    printf("rimozione dalla memoria del giocatore [%d]\n", i+1);

    free(giocatori[i]);//deallocazione dei singoli campi dell'array
  }

  free(giocatori);//deallocazione della memoria per l'array di puntatori a struct Giocatore
  giocatori=NULL;
  printf("memoria deallocata in maniera corretta.\n");
}


//funzione per inizializzare giocatori e allocare memoria
static int Inizializza_giocatori(void){

  giocatori = (struct Giocatore**)malloc(numero_giocatori * sizeof(struct Giocatore*));/*allocazione in memoria dinamica di un array di giocatori
  di grandezza numero_giocatori*/

  if (giocatori == NULL) {

    printf("Errore nell'allocazione della memoria.\n");/*stampiamo un messaggio di errore nel caso in cui l'allocazione in
    memoria non dovesse riuscire*/

    return -1;
  }

  for (int i = 0; i < numero_giocatori; i++) {

    giocatori[i] = (struct Giocatore*)malloc(sizeof(struct Giocatore));//inizializziamo ogni giocatore con una nuova allocazione di memoria

    if (giocatori[i] == NULL) {

      printf("Errore nell'allocazione della memoria per il giocatore [%d].\n", i+1);/*messaggio di errore in caso l'deallocazione
      fallisse anche per l'array dei giocatori*/

      return -1;
    }

  }

return 0;
}


//funzione che chiede di inserire da 1 a 4 giocatori e controlla che il numero inserito sia in questo range
static int Numero_giocatori(void) {

  int N_players;//variabile temporanea che prende il numero di giocatori

  do {

    printf("inserire il numero di giocatori che prenderanno parte alla partita, il massimo consentito è di 4 giocatori\n");

    char temp[100]; //buffer temporaneo per l'input

    fgets(temp, sizeof(temp), stdin); //leggiamo l'input come stringa

    N_players = atoi(temp); //conversione della stringa in un intero


    if (N_players<1 || N_players>4) {

      system("clear");

      printf("il numero di giocatori inserito non è valido.\n");
      sleep(1);

      continue;

    }
    //se arriviamo a questo punto siamo sicuri che il numero inserito sia nel range che desideriamo
    printf("Il numero di giocatori inseriti è di: [%d]\n", N_players);/*il numero inserito sarà compreso tra 1 e 4,
    in quanto se il numero è un intero che non rientra nel range viene fatto il continue,
    se inseriamo anche solo un carattere non numerico viene restituito 0, che non rientra nel range*/

  } while(N_players<1 || N_players>4);

  return (N_players);
}


//funzione che fa inserire da tastiera i nomi dei giocatori, basandosi sul numero di giocatori precedentemente scelti
static void Nomi_giocatori(void) {

  for (int i = 0; i < numero_giocatori; i++) {

    printf("Inserire il nome del giocatore numero [%d]: ", i + 1);

    char temp[300];//buffer temporaneo

    fgets(temp, sizeof(temp), stdin);

    if (temp[strlen(temp) - 1] == '\n') {/*if usato per rimuovere il carattere invio,
      lo sostituisce con il carattere di terminazione della stringa \0,
      in quanto [strlen] restituisce la lunghezza della stringa,
      e andando alla posizione precedente abbiamo sicuramente il carattere invio (\n)*/
      temp[strlen(temp) - 1] = '\0';
    }

    if (strspn(temp, " \t\n") == strlen(temp)) {/*if usato per assicuarsi della validità del nome,
      in quanto se il nome è composto solo da spazi, invio o tab ripete l'iterazione,
      ciò è possibile in quanto [strspn] restituisce la lunghezza tenendo conto solo dei caratteri
      che fanno parte dell'insieme, e in questo caso mettendo " ", "\t" e "\n" andiamo a considerare
      l'insieme fatto dallo spazio, dal tab orizzontale e dall'invio*/
      printf("Nome non valido. Riprova.\n");

      i--; //ripetiamo l'iterazione per chiedere un nuovo nome, facendo sempre il controllo sulla sua validità

      continue;
    }

    memmove(temp + 1, temp, strlen(temp) + 1);/*[memmove] serve per spostare la sequenza di caratteri
    che gli vengono passati(temp, quindi quelli inseriti da tastiera dall'utente),
    di una posizione in questo caso(temp + 1), gli passiamo la posizione dove vogliamo spostare qualcosa,
    gli passiamo ciò che vogliamo spostare e la lunghezza della cosa da spostare,
    mi è servita in quanto andando a stampare il nome del giocatore su stampa_giocatore()
    mi rimuoveva il primo spazio, il nome ABC diventava BC*/

    temp[0] = ' ';/*aggiungo uno spazio in prima posizione, perché per il problema precedentemente scritto esso verrà cancellato,
    e lo sapzio è comodo, in quanto per la stampa non da problemi,
    basta togliere uno spazio dal printf e si ottiene lo stesso risultato*/

    strncpy((char *)giocatori[i]->Nome_giocatore, temp, sizeof(giocatori[i]->Nome_giocatore));/*[strncpy] serve per copiare una stringa
    in un altra stringa, gli dobbiamo passare dove dobbiamo copiare la stringa di riferimento,
    la stringa di riferimento e il numero massimo di caratteri che ci vogliamo copiare,
    se la lunghezza di temp è minore allora riempie gli spazi rimanenti con i caratteri di terminazione della stringa*/
  }
}


//funzione che stampa il nome dei giocatori(nomi presi in output grazie a Nomi_giocatori)
static void Nomi_giocatori_stampa(void){

  printf("I nomi inseriti sono:\n");

  for (int i = 0; i < numero_giocatori; i++) {

    printf("Giocatore [%d]:%s\n", i + 1, giocatori[i]->Nome_giocatore);/*non metto lo spazio dopo [%d]: in quanto
    ho messo come primo spazio del nome uno spazio, che successivamente mi verrà cancellato
    (problema che non ho saputo come affrontare in altri modi)*/
  }
}


//funzione per asseganre la sanità mentale ai giocatori
static void Sanita_mentale(void){

  for (int i = 0; i < numero_giocatori; i++) {

    giocatori[i]->sanita_mentale=100;/*andiamo a mettere dentro sanità_mentale il valore 100, lo facciamo per ogni giocatore
    essendo un Unsigned char il massimo valore che accetta è 255, quindi più avanti verranno fatti dei controlli*/
  }

}


//funzione usata per scegliere la difficoltà
static int Difficolta(void){

  int difficulty=0;//variabile temporanea per tener traccia del livello di difficoltà inserito dall'utente

  printf("Selezionare il livello di difficoltà:\n");

  do{

    printf("\t[1] livello dilettante(la sanità scende molto lentamente)\n\t[2] livello intermedio(la sanità scende più velocemente e gli avvistamenti sono più frequenti)\n\t[3] livello incubo(buona fortuna)\n");

    char temp[100];//buffer temporaneo

    fgets(temp, sizeof(temp), stdin);

    difficulty = atoi(temp);

    if(difficulty<1 || difficulty>3){//se non rientra nel range previsto si richiede all'utente di scegliere la difficoltà

      system("clear");
      printf("Il numero/carattere selezionato non corrisponde a nessuna difficoltà, si prega di scegliere tra le seguenti:\n");

    }

  }while(difficulty<1 || difficulty>3);

  switch (difficulty) {

    case 1:
    printf("La difficoltà selezionata è [1], livello [dilettante]\n");
    break;

    case 2:
    printf("La difficoltà selezionata è [2], livello [intermedio]\n");
    break;

    case 3:
    printf("La difficoltà selezionata è [3], livello [incubo]\n");
    break;

  }

  return difficulty;
}

//funzione che genera casualmente un oggetto tra gli enum Tipo_oggetto_iniziale{EMF, spirit_box, videocamera, calmanti, sale},
//controllando sempre la presenza di almeno uno tra: EMF, spirit_box o videocamera
static void Assegnazione_oggetti(void) {
  srand(time(NULL));//randomizziamo in base al tempo

  for(int i=0; i < numero_giocatori; i++){/*ciclo per impostare tutti i campi dello zaino come vuoti,
    dandogli nulla, un oggetto extra che ho aggiunto che non fa nulla*/
    for(int j=0; j<4; j++)

    giocatori[i]->zaino[j] = nulla;//5 e nulla sono la stessa cosa, ma è meglio scrivere nulla per capire meglio la logica del programma
  }

  //si generano casualmente tanti oggetti quanti sono i giocatori e li memorizza in un array di oggetti iniziali
  enum Tipo_oggetto_iniziale oggetto_iniziale[numero_giocatori];

  for (int i = 0; i < numero_giocatori; i++) {

    oggetto_iniziale[i] = rand() % 5;//gli oggetti casuali generati variano tra 0 e 4, quindi EMF, spirit_box, videocamera, calmanti, sale
  }

  //verifichiamo se almeno uno tra EMF, spirit_box o videocamera è presente nell'array degli oggetti iniziali
  bool contiene_essenziali = false;
  for (int i = 0; i < numero_giocatori; i++) {

    if (oggetto_iniziale[i] == EMF || oggetto_iniziale[i] == spirit_box || oggetto_iniziale[i] == videocamera) {

      contiene_essenziali = true;//variabile aggiornata solo se trova almeno un oggetto essenziale tra gli oggetti precedentemente generati

      break;
    }
  }

  //se nessuno degli oggetti iniziali è EMF, spirit_box o videocamera, sostituisco casualmente uno degli oggetti con uno di essi
  if (!contiene_essenziali) {

    int oggetto_sostituito = rand() % numero_giocatori;//scelgo casualmente quale oggetto sostituire con un oggetto essenziale

    oggetto_iniziale[oggetto_sostituito] = rand() % 3; /*genera solo EMF (0), spirit_box (1) o videocamera (2),
    e sostituisce un oggetto non essenziale casuale precedentemente generato con l'oggetto essenziale generato*/
  }

  //array per tenere traccia dei giocatori che devono ancora scegliere
  bool giocatori_da_scegliere[numero_giocatori];

  for (int i = 0; i < numero_giocatori; i++) {

    giocatori_da_scegliere[i] = true;//impostiamo tutte le posizioni dell'array di booleani come true
  }

  //ciclo che permette a ogni giocatore di scegliere un oggetto tra quelli disponibili e rimuove tale oggetto (scelto) dalla lista
  for(int turno = 0; turno < numero_giocatori; turno++){

    int num_giocatori_da_scegliere = numero_giocatori - turno; /*alcoliamo il numero di giocatori che devono
    ancora scegliere, se abbiamo 4 giocatori al primo turno [0], il primo giocatore deve essere scelto tra 4,
    poi al secondo turno [1] saranno in 3, e così via*/
    int giocatore_casuale = seleziona_giocatore_casuale(giocatori_da_scegliere);

    int scelta=0;
    do{
      //menù di scelta che mostra gli oggetti disponibili tra cui scegliere
      printf("Oggetti disponibili tra cui scegliere:\n");

      for (int i = 0; i < num_giocatori_da_scegliere; i++)

      printf("\t[%d] [%s].\n", i + 1, nome_enum_display_a_parole(oggetto_iniziale[i]));/*si poteva fare anche con uno switch,
      ma avendo implementato questa funzione che mi converte i numeri degli enum nei corrispettivi oggetti,
      è più conveniente fare così*/

        printf("Giocatore [%d], scegli l'oggetto che preferisci inserendo il numero corrispondente: ", giocatore_casuale + 1);

        char temp[100];

        fgets(temp, sizeof(temp), stdin);

        scelta = atoi(temp);

        if(scelta < 1 || scelta > num_giocatori_da_scegliere){
          system("clear");
          printf("si prega di inserire valore valido, qualunque altro numero non può essere associato ad un oggetto\n");
        }


    } while (scelta < 1 || scelta > num_giocatori_da_scegliere);

    giocatori[giocatore_casuale]->zaino[0] = oggetto_iniziale[scelta - 1];/*nel primo posto dello zaino viene messo
    l'oggetto precedentemente scelto*/

    //rimuoviamo l'oggetto scelto dalla lista degli oggetti disponibili
    for (int j = scelta - 1; j < num_giocatori_da_scegliere - 1; j++) {

      oggetto_iniziale[j] = oggetto_iniziale[j + 1];

    }

    giocatori_da_scegliere[giocatore_casuale] = false; /*poniamo il giocatore che ha preso l'oggetto come false,
    così non può più essere scelto da seleziona_giocatore_casuale, quindi non può più scegliere nessun oggetto*/
}
  system("clear");

  //ciclo che stampa quale oggetto ha scelto ogni giocatore
  for (int i = 0; i < numero_giocatori; i++)

  printf("Il giocatore [%d] ha scelto l'oggetto [%s].\n", i + 1, nome_enum_display_a_parole(giocatori[i]->zaino[0]));

}


//funzione usata all'interno di Assegnazione_oggetti
static int seleziona_giocatore_casuale(bool giocatori_da_scegliere[]) {

  int giocatore_casuale;

  do {

    giocatore_casuale = rand() % numero_giocatori;//si sceglie un giocatore casuale tra i giocatori che prendono parte al gioco

  } while (!giocatori_da_scegliere[giocatore_casuale]);/*se tale giocatore è false(quindi è già stato scelto),
  allora si ripete il loop finché non trova un giocatore che non ha ancora scelto nessun oggetto tra i disponibili*/

  return giocatore_casuale;//quando lo trova ritorna tale valore
}


//funzione inserisci_zona(), aggiunge in coda alla mappa una nuova zona
static int inserisci_singola_zona(void){
  zona=(struct Zona_mappa*) (malloc(sizeof(struct Zona_mappa)));//creazione di una mappa in memoria dinamica, zona è globale

  if(zona == NULL) {//controllo in caso di errore nell'allocazione in memoria
    printf("Errore: memoria esaurita.\n");
    return -1;
  }

  int zona_casuale=(rand() % 6) + 16;/*ci restituisce un valore che varia tra 16 e 21, ovvero
  tra cucina e seminterrato(cucina, soggiorno, camera, bagno, garage, seminterrato), esclude sempre il caravan*/

  zona->zona = zona_casuale;//inseriamo il valore estratto nel campo zona della nuova zona creata

  //generiamo l'oggetto zona casuale
  int probabilita_oggetto = rand() % 100;

  if (probabilita_oggetto < 40) {
    zona->oggetto_zona = nessun_oggetto;//40% probabilità per nessun_oggetto

  } else if(probabilita_oggetto > 40 && probabilita_oggetto < 50){
    zona->oggetto_zona = carte;/*10% probabilità di carte(9), oggetto aggiunto da me,
    i suoi effetti sono tutti spiegati nella funzione usa_oggetto, nel case dello switch [carte]*/

  }else {
    zona->oggetto_zona = (enum Tipo_oggetto_zona)((rand() % 3) +6);/*rimanente 50% di probabilità per gli oggetti:
    adrenalina(6), cento_dollari(7) e coltello(8)*/
  }

  //le prove vengono generate con avanza(), quindi non serve a nulla metterle durante la creazione delle zone
  zona->prova = nessuna_prova;

  if(pFirst==NULL){/*caso lista vuota, il puntatore al primo e all'ultimo elemento puntano alla stessa zona,
  in quanto ce ne è solo una, il puntatore prossima_zona di zona punta a se stesso, in quanto è una lista circolare,
  e se andiamo avanti torniamo nella stessa zona*/
    pFirst=zona;
    pLast=zona;
    zona->prossima_zona=zona;

    printf("La zona [%s] è la prima zona della tua mappa, inserisci più zone per avere un maggiore divertimento\n", nome_enum_display_a_parole(zona->zona));
  } else {/*caso lista non vuota, andiamo a cambiare il puntatore prissima_zona di pLast,
    che siccome punta all'ultimo elemento deve puntare alla nuova zona inserita, infinte la colleghiamo con pFirt poiché circolare,
    tutto questo in quanto stiamo inserendo una zona in coda alla lista, altrimenti avremo cambiato il campo prossima_zona di zona,
    mettendoci pFirst, il campo prossima_zona di pLast avrebbe puntato alla nuova zona
    e infine pFirst lo avremo fatto puntare alla nuova zona*/

    pLast->prossima_zona=zona;
    zona->prossima_zona=pFirst;
    pLast=zona;

    printf("La zona [%s] è stata inserita con successo alla fine della mappa\n", nome_enum_display_a_parole(zona->zona));
  }

  mappa_implementata=true;//se almento una zona è stata inserita si può procedere a giocare
return 0;
}


//funzione per inserire tutte le zone(in ordine casuale) tranne il caravan
static int inserisci_tutte_le_zone(void){

bool zona_creata[6];

for(int j=0; j<6; j++){
  //array di booleani per tener traccia delle zone create
zona_creata[j]=false;
}

  for(int i=0; i<6; i++){
    zona = (struct Zona_mappa*)(malloc(sizeof(struct Zona_mappa)));

    if(zona == NULL) {//controllo di eventuali problemi nell'allocazione della memoria dinamica
      printf("Errore: memoria esaurita.\n");
      return -1;
    }

    zona->zona = random_zona(zona_creata);

    //generiamo l'oggetto zona casuale
    int probabilita_oggetto = rand() % 100;

    if (probabilita_oggetto < 40) {
      zona->oggetto_zona = nessun_oggetto;//40% probabilità per nessun_oggetto

    } else if(probabilita_oggetto > 40 && probabilita_oggetto < 50){
      zona->oggetto_zona = carte;//10% probabilità di carte

    }else {
      zona->oggetto_zona = (enum Tipo_oggetto_zona)((rand() % 3) +6);//rimanente 50% di probabilità per gli oggetti:
      //adrenalina(6), cento_dollari(7) e coltello(8)
    }

    //le prove vengono generate con avanza()
    zona->prova = nessuna_prova;

//poniamo la nuova zona o come primo elemento della lista(caso lista vuota) oppure lo inseriamo in coda alla lista
    if (pFirst == NULL) {
      pFirst = zona;
      pLast = zona;
      zona->prossima_zona = zona;

    } else {
      pLast->prossima_zona = zona;
      zona->prossima_zona = pFirst;
      pLast = zona;

    }
  }

  printf("Nuova mappa implementata con successo!\n");
  mappa_implementata=true;
  return 0;
}


//funzione che torna il numero della zona non ancora inserito nella lista
static int random_zona(bool zona_creata[]){
int numero_zona_casuale;
int indice;
do{
  indice=rand() % 6;//indice dell'array(0-5)
numero_zona_casuale = indice+16;//numero della zona(16-21)
}while(zona_creata[indice]);
zona_creata[indice] = true;
return numero_zona_casuale;
}


//funzione per modificare una zona se già presente nella lista
static void modifica_zona_casualmente(void) {

  if(!mappa_implementata){//se non c'è nessuna mappa possiamo uscire subito
    printf("Impossibile modificare i campi della mappa in quanto non sono ancora stati creati\n");
    return;
  }

  int zona_modificata=0;

  struct Zona_mappa* current = pFirst;
  struct Zona_mappa* next = NULL;


  int random =  16 + rand() % 6; //generiamo una zona casuale escludendo il caravan

  do {
    next = current->prossima_zona; //salvo il puntatore alla prossima zona

    if (current->zona == (enum Tipo_zona)random) {/*scorriamo la mappa, e ogni zona che troviamo
      corrispondente a quella estratta casualmente si ritroverà modificata del suo campo oggetto*/
      zona_modificata++;//serve per vedere se e quante volte è avvenuta la modifica
      printf("Zona [%s] già esistente, ", nome_enum_display_a_parole(current->zona));

      int probabilita_oggetto = rand() % 100;

      if (probabilita_oggetto < 40) {
        current->oggetto_zona = nessun_oggetto;//40% probabilità di nessun oggetto

      } else {
        current->oggetto_zona = (enum Tipo_oggetto_zona)((rand() % 4) +6);/*restante 60% di probabilità per
        adrenalina, cento_collari, coltello o carte*/
      }

      printf("sostituzione in corso...\n");
    }

    current = next;//avanziamo alla prossima zona

  } while (current != pFirst);

  if(zona_modificata==1)
  printf("[%d] Zona modifiata con successo!\n", zona_modificata);

  else if(zona_modificata>1){
    printf("[%d] Zone modificate con successo\n", zona_modificata);//ritorniamo quante zone sono state eventualemnte modificate

  } else printf("La zona [%s] non è presente nella tua lista, pertanto non può essere modificata\n", nome_enum_display_a_parole(random));
}


//funzione per cancellare tutte le zone inserite
static void cancella_tutte_le_zone(void) {

  if (pFirst == NULL) {//se la mappa è vuota non abbiamo nulla da cancellare
    printf("La lista è vuota, non c'è nulla da cancellare\n");
    mappa_creata = 0;
    return;
  }

  struct Zona_mappa* current = pFirst;
  struct Zona_mappa* next = NULL;

  system("clear");

  do {
    next = current->prossima_zona; //salviamo il puntatore nella prossima zona
      printf("cancellazione della zona [%s] precedentemente creata...\n", nome_enum_display_a_parole(current->zona));
    free(current); //deallochiamo la zona corrente
    current = next; //passiamo alla prossima zona
  } while (current != pFirst);//lo facciamo per tutta la durata della lista circolare

  pFirst = NULL;//reset dei puntatori
  pLast = NULL;
  mappa_implementata = false;//la mappa non è quindi più implemenetata
  mappa_creata=false;//poniamo a falso la variabile globale mappa_creata

  printf("cancellazione delle zone precedentemente create effettuata con successo!\n");
}


//fuzione per cancellare l'ultima zona inserita
static void cancella_ultima_zona(void) {

    if (pFirst == NULL) {

        printf("La lista è vuota, non ci sono zone da cancellare.\n");

        return;

    } else if (pFirst == pLast) {//caso di un solo elemento nella lista
      printf("la zona [%s] ", nome_enum_display_a_parole(pFirst->zona));
        free(pFirst);
        pFirst = NULL;
        pLast = NULL;
        printf("è stata cancellata, la lista è ora vuota\n");
        //reset delle variabili
        mappa_implementata = false;
      mappa_creata=false;

    } else {
        struct Zona_mappa *elementoPrecedente = pFirst;
        //cerchiamo il penultimo elemento per far restare la licta circolare
        while (elementoPrecedente->prossima_zona != pLast) {
            elementoPrecedente = elementoPrecedente->prossima_zona;
        }

        elementoPrecedente->prossima_zona = pFirst;
        printf("la zona [%s] ", nome_enum_display_a_parole(pLast->zona));
        free(pLast);

        pLast = elementoPrecedente;

        printf("è stata cancellata con successo\n");
    }
}


//funzione per stampare i campi di tutte le zone create fino a quel momento
static void stampa_mappa(void) {

  system("clear");

  if (pFirst == NULL) {//se la mappa è vuota non abbiamo nulla da stampare
    printf("La mappa è vuota.\n");
    return;
  }

  struct Zona_mappa* zona_corrente = pFirst;

  do {
    printf("\tZona: [%s]; \n\tOggetto: [%s]; \n\tProva: [%s]\n\n", nome_enum_display_a_parole(zona_corrente->zona), nome_enum_display_a_parole(zona_corrente->oggetto_zona), nome_enum_display_a_parole(zona_corrente->prova));

    zona_corrente = zona_corrente->prossima_zona;//stampiamo i dati della zona partendo dalla prima,
    //dalla testa della lista, continuando fino alla fine
  } while (zona_corrente != pFirst);
}


//funzione per chiudere il menu di creazione della mappa
static int chiudi_mappa(void) {
  system("clear");

  if(!mappa_implementata){
    printf("Nessuna mappa implemenetata oppure troppe poche zone inserite, si prega di implemenetare la mappa più volte ([1] o [2]) prima di chiudere il menù [6]\n");

  } else{

    zona_caravan = (struct Zona_mappa*)malloc(sizeof(struct Zona_mappa));//creazione caravan

    if (zona_caravan == NULL) {
      printf("Errore: memoria esaurita.\n");
      return-1;
    }

    zona_caravan->zona = caravan;//il caravan non deve essere collegato alla lista, inoltre non deve contenere ne oggetti ne prove
    zona_caravan->oggetto_zona=nessun_oggetto;
    zona_caravan->prova=nessuna_prova;
    zona_caravan->prossima_zona=NULL;


    mappa_creata = true; //solo se si crea la mappa questo valore viene posto a 1, così da permettere l'inizio del vero gioco
    printf("Creazione della mappa terminata.\n\n");
  }
  return 0;
}


//funzione che invece di stampare un numero stampa l'oggetto / zona_mappa / prova associata
static const char* nome_enum_display_a_parole(int i) {
  static const char* nomi[] = {
    "EMF", "spirit_box", "videocamera", "calmanti", "sale", "nulla",//enum Tipo_oggetto_iniziale
    "adrenalina", "cento_dollari", "coltello", "carte", "nessun_oggetto",//enum Tipo_oggetto_zona
    "prova_EMF", "prova_spirit_box", "prova_videocamera", "nessuna_prova",//enum Tipo_prova
    "caravan", "cucina", "soggiorno", "camera", "bagno", "garage", "seminterrato",//enum Tipo_zona
  };

  return nomi[i];
}


/////////////////////
void gioca(void){
if(mappa_creata==0){/*controllo se la mappa è stata creata, quindi se è stato impostato il gioco
  in caso affermativo si procede con gioca, altrimenti si deve o impostare il gioco o si termina*/
  system("clear");
printf("prima di giocare occorre impostare mappa, giocatori e prendere degli oggetti [1], solo in seguito è possibile giocare [2]\n\n");
sleep(1);
return;
}

int array_avanzamento_giocatori[numero_giocatori];
int refresh=4;//variabile usata per rimettere oggetti nella zona quando un giocatore va in una nuova zona
//(avviene solo una volta ogni 4 turni)
struct Zona_mappa* zona_controllo_fantasma = NULL;

for(int i=0; i<numero_giocatori; i++){//all'inizio del gioco tutti i giocatori vengono messi nella prima zona
  giocatori[i]->posizione=pFirst;
}


do{

  system("clear");
  printf("Turno: [%d]\n", ++turno);

zona_controllo_fantasma=pFirst;


  do {

    if(zona_controllo_fantasma->presenza_fantasma==true)
    zona_controllo_fantasma->presenza_fantasma=false;/*alla fine di ogni turno, la presenza del fantasma si toglie,
    permettendo ai giocatori di tornare sul caravan*/

    zona_controllo_fantasma = zona_controllo_fantasma->prossima_zona;
  } while (zona_controllo_fantasma != pFirst);//continuiamo per tutta la mappa

for(int j=0; j<numero_giocatori; j++){//chi usa sale viene protetto dalla diminuzione della sanità provocata dal fantasma, ma dura solo per quel turno
  protezione_fantasma[j]=false;
}

  refresh--;

  if(refresh==0)
  refresh=4;

  int* ordine_corrente = ordine_giocatori_casuale();//ogni turno avremo un ordine diverso


  printf("L'ordine corrente dei giocatori è: ");
    for (int i = 0; i < numero_giocatori; i++) {//stampiamo i numeri

      if(giocatori[ordine_corrente[i]]->sanita_mentale==0){/*se il giocatore non ha più sanità è considerato morto,
        quindi non può fare nulla e non viene considerato*/
      continue;
      }

        printf("-> [%d] ", ordine_corrente[i]+1);
    }
    printf("\nOvvero: ");
    for (int i = 0; i < numero_giocatori; i++) {//stampiamo anche i nomi

      if(giocatori[ordine_corrente[i]]->sanita_mentale==0){//stesso controllo fatto in precedenza
      continue;
      }

        printf("-> [%s] ", giocatori[ordine_corrente[i]]->Nome_giocatore);
    }
    printf("\n");


    for(int i=0; i<numero_giocatori; i++)
    array_avanzamento_giocatori[i]=1;//tutti i giocatori possono avanzare una volta per turno, eccezzion fatta per l'uso di adrenalina

  for (int i = 0; i < numero_giocatori; i++) {

            int giocatore_corrente = ordine_corrente[i];//l'ordine segue l'array ordine_giocatori

            if(giocatori[giocatore_corrente]->sanita_mentale==0)
            //se il giocatore è morto non potrà giocare
            continue;

          if(trova_prova_EMF && trova_prova_spirit_box && trova_prova_videocamera)/*se tutte le prove sono state raccolte
          si finisce il turno senza fare ulteriori azioni e c'è il messaggio di vittoria*/
          continue;


            gioca_turno(giocatore_corrente, array_avanzamento_giocatori, refresh);

        }

        free(ordine_corrente);

        /*controlliamo sia la sanità mentale dei giocatori che le prove raccolte, continuiamo finché almeno una delle due
        non diventa vera, poi la neghiamo, quindi diventa falsa e usciamo*/
      }while(!tutti_giocatori_senza_sanita() && !(trova_prova_EMF && trova_prova_spirit_box && trova_prova_videocamera));

if(trova_prova_EMF && trova_prova_spirit_box && trova_prova_videocamera){//se si esce dal ciclo con le prove trovate, si vince

  Vittoria();
} else Sconfitta(); //altrimenti si perde
system("clear");

Dealloca_giocatori();//togliamo tutto ciò che abbiamo creato
cancella_tutte_le_zone();
free(zona_caravan);
zona_caravan=NULL;

system("clear");
//resettiamo tutte le variabili globali
mappa_creata=0;
turno=0;
trova_prova_EMF=false;
trova_prova_spirit_box=false;
trova_prova_videocamera=false;

}//
/////////////////////


//funzione per ottenere casualmente l'ordine in cui giocheranno i giocatori alla fine di ogni turno
static int* ordine_giocatori_casuale(void) {

    int* ordine_giocatori = (int*)malloc(numero_giocatori * sizeof(int));
    bool array_ordine[numero_giocatori];

    for (int i = 0; i < numero_giocatori; i++) {//impostiamo a true tutti gli spazi dell'array che ci servono
        ordine_giocatori[i] = i;//ordine 0-1-2-3(caso 4 giocatori)
        array_ordine[i] = true;//impostiamo tutti i campi a true
    }

    srand(time(NULL));

    int* nuovo_ordine = (int*)malloc(numero_giocatori * sizeof(int));
    /*free fatta su gioca(), passando int* ordine_corrente = ordine_giocatori_casuale();
    poi possiamo fare la free tramite free(ordine_corrente);*/

    for (int i = 0; i < numero_giocatori; i++) {
        int ordine;

        do {
            ordine = rand() % numero_giocatori;
        } while (!array_ordine[ordine]);//otteniamo così un ordine casuale

        array_ordine[ordine] = false;//mettiamo a false la posizione uscita, così che non possa uscire nuovamente
        nuovo_ordine[i] = ordine_giocatori[ordine];
    }

    free(ordine_giocatori);

    return nuovo_ordine;
}

/*funzione che controlla la sanità dei giocatori, se il giocatore non ha più sanità deve essere successivamente tolto,
se nessun giocatore ha più sanità il gioco deve finire*/
static bool tutti_giocatori_senza_sanita(void) {

        for (int i = 0; i < numero_giocatori; i++) {

            if (giocatori[i]->sanita_mentale != 0) {

                return false; //almeno un giocatore ha sanità mentale maggiore di 0
            }
        }

        return true; //tutti i giocatori hanno sanità mentale uguale a 0, quindi si perde
    }


//funzione centrale in gioca(), permette all'utente di scegliere quale azione fare richiamando l'apposita funzione
static void gioca_turno(int i, int *array_avanzamento_giocatori, int refresh){

    int scelta=0;

do{

  if(giocatori[i]->sanita_mentale==0){//se il giocatore non ha sanità mentale non può fare nulla
    //è un controllo extra per far finire il turno corrente di un giocatore che muore
    return;
  }

controllo_zaino_giocatori(i);/*controllo per vedere se il gioco è completabile,
se quando si rimuove un oggetto non sono presenti oggeti essenziali nello zaino di nesusn giocatore,
allora il gioco non risulta completabile, quindi viene dato un oggetto essenziale*/

  printf("Giocatore [%d], scegli quale comando eseguire durante questo turno(i numeri con ° non provocano mai decremento sanità), le operazioni disponibili sono:\n\t[1] torna al caravan\n\t[2]° stampa i tuoi dati\n\t[3]° stampa le caratteristiche della zona in cui risiedi\n\t[4] avanza\n\t[5] raccogli prova\n\t[6] raccogli oggetto\n\t[7] usa oggetto\n\t[8]° finisci il turno\n\t[9]° suicidio\n\t[10]° per rimuovere un oggetto a tua scelta dallo zaino\n", i+1);
/* stampa: Giocatore [%d], scegli quale comando eseguire durante questo turno
(i numeri con ° non provocano mai decremento sanità), le operazioni disponibili sono:
[1] torna al caravan
[2]° stampa i tuoi dati
[3]° stampa le caratteristiche della zona in cui risiedi
[4] avanza
[5] raccogli prova
[6] raccogli oggetto
[7] usa oggetto
[8]° finisci il turno
[9]° suicidio
[10]° per rimuovere un oggetto a tua scelta dallo zaino*/

  char temp[100];

        fgets(temp, sizeof(temp), stdin);

        scelta = atoi(temp);

    switch(scelta){

      case 1:
      int return_torna_caravan = torna_caravan(i);
      if(return_torna_caravan==0){//se il giocatore non è effettivamente entrato nel caravan il turno può continuare
        break;
      }
      sleep(3);
      return;//altriemnti il turno finisce e passa al prossi giocatore

      case 2:
      system("clear");
      stampa_giocatore(i);
      break;

      case 3:
      system("clear");
      stampa_zona(i);
      break;

      case 4:
      system("clear");
      probabilita_decremento_sanita(i);//funzione che può decrementare la sanità mentale del giocatore che fa l'azione
      avanza(i, array_avanzamento_giocatori, refresh);
      break;

      case 5:
      system("clear");
      probabilita_decremento_sanita(i);
      raccogli_prova(i);
      break;

      case 6:
      system("clear");
      probabilita_decremento_sanita(i);
      raccogli_oggetto(i);
      break;

      case 7:
      system("clear");
      probabilita_decremento_sanita(i);
      usa_oggetto(i, array_avanzamento_giocatori);
      break;

      case 8:
      system("clear");
      //passa()
      return;

      case 9:
      system("clear");
      giocatori[i]->sanita_mentale=0;
      printf("Il giocatore [%d] non ha resistio alla pressione e si è tolto la vita\n", i+1);
      return;

      case 10:
      system("clear");
      rimozione_oggetto_zaino(i);
      break;

      default:
      system("clear");
      printf("Il comando selezionato non è valido, si prega di scegliere un comando tra quelli proposti.\n");
}
}while(scelta!=8 || (trova_prova_EMF && trova_prova_spirit_box && trova_prova_videocamera));
  }


//funzione che sposta la posizione del giocatore sul caravan,
//appena il giocatore finisce le azioni che può fare sul caravan viene riportato sulla prima zona dela lista circolare
static int torna_caravan(int i){

    if(giocatori[i]->posizione->presenza_fantasma==true){
      printf("Il fantasma è presente in questa zona, non è quindi possibile tornare al caravan per questo turno.\n");
      return 0;
    }

    system("clear");
    probabilita_decremento_sanita(i);//solo se il fantasma non è presente nella zona c'è il 20% di probabilità di decrementeare la sanità del giocatore

if(giocatori[i]->sanita_mentale==0){
printf("il giocatore è morto nel tentativo di tornare nel [caravan]\n");
  sleep(1);
  return 1;
}

    int scelta=0;

    giocatori[i]->posizione=zona_caravan;//spostiamo la posizione del giocatore fuori dalla lista circolare
    printf("zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));

    do{
      printf("Dal caravan è possibile depositare le prove raccolte, premere [1] per depositare le prove raccolte, [2] per uscire dalla zona carvan e tornare nel seminterrato\n");
      char temp[100];

      fgets(temp, sizeof(temp), stdin);

      scelta = atoi(temp);

      if(scelta<1 || scelta>2){
        system("clear");
        printf("Opzione non disponibile, si prega di premere [1] o [2]\n");
        sleep(1);
      }



      if(scelta==1){

        for(int j=0; j<4; j++){
          if(giocatori[i]->zaino[j]==prova_EMF){

            if(trova_prova_EMF){//controllo ulteriore per vedere che non sia già stata presa la prova da un altro giocatore
            printf("La [prova EMF] è già stata presa da un altro giocatore\n");

            giocatori[i]->posizione = pFirst;//rimettiamo il giocatore nella lista circolare, nella prima posizione

            printf("Il giocatore esce dal [caravan] e torna nella zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));
            sleep(1);

            return 1;
          }

          printf("La [prova EMF] è stata raccolta con successo, ora un altro oggetto essenziale ti verrà dato al suo posto\n");
          trova_prova_EMF=true;
          giocatori[i]->zaino[j]=oggetto_essenziale_prova_false();

          giocatori[i]->posizione = pFirst;

          printf("Il giocatore esce dal [caravan] e torna nella zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));
          sleep(1);

          return 1;
        }

        if(giocatori[i]->zaino[j]==prova_spirit_box){
          if(trova_prova_spirit_box){
            printf("La [prova spirit_box] è già stata presa da un altro giocatore\n");

            giocatori[i]->posizione = pFirst;

            printf("Il giocatore esce dal [caravan] e torna nella zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));
            sleep(1);

            return 1;
          }
          printf("La [prova spirit_box] è stata raccolta con successo, ora un altro oggetto essenziale ti verrà dato al suo posto\n");
          trova_prova_spirit_box=true;
          giocatori[i]->zaino[j]=oggetto_essenziale_prova_false();

          giocatori[i]->posizione = pFirst;

          printf("Il giocatore esce dal [caravan] e torna nella zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));
          sleep(1);

          return 1;
        }

        if(giocatori[i]->zaino[j]==prova_videocamera){
          if(trova_prova_videocamera){
            printf("La [prova videocamera] è già stata presa da un altro giocatore\n");

            giocatori[i]->posizione = pFirst;

            printf("Il giocatore esce dal [caravan] e torna nella zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));
            sleep(1);

            return 1;
          }

          printf("La [prova videocamera] è stata raccolta con successo, ora un altro oggetto essenziale ti verrà dato al suo posto\n");
          trova_prova_videocamera=true;
          giocatori[i]->zaino[j]=oggetto_essenziale_prova_false();

          giocatori[i]->posizione = pFirst;

          printf("Il giocatore esce dal [caravan] e torna nella zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));
          sleep(1);

          return 1;
        }
      }
//se arriviamo a questo punto vuol dire che il giocatore non ha nessuna prova, quindi può solo uscire dal caravan
        printf("Il giocatore [%d] non ha nessuna prova da depositare nel caravan\n", i+1);

        giocatori[i]->posizione = pFirst;

        printf("Il giocatore esce dal [caravan] e torna nella zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));
        sleep(1);
        return 1;


    } else if(scelta==2) {
      giocatori[i]->posizione = pFirst;
      printf("Il giocatore esce dal [caravan] e torna nella zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));
    }
  } while(scelta!=2);
return 1;
}


//funzione che stampa tutti i dati del giocatore che sta eseguendo il turno
static void stampa_giocatore(int i){

    printf("Stampa dei dati del giocatore [%d]:\n", i+1);

    printf("\tNome: [%s] \n\n", giocatori[i]->Nome_giocatore);

    printf("\tSanità mentale: [%hhu]\n\n", giocatori[i]->sanita_mentale);

    printf("\tZaino:\n\t[%s]\n\t[%s]\n\t[%s]\n\t[%s]\n\n", nome_enum_display_a_parole(giocatori[i]->zaino[0]), nome_enum_display_a_parole(giocatori[i]->zaino[1]), nome_enum_display_a_parole(giocatori[i]->zaino[2]), nome_enum_display_a_parole(giocatori[i]->zaino[3]));

    printf("\tPosizione: [%s]\n\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));

  }


//funzione che stampa i dati relativi alla zona in cui sta il giocatore, fa quindi vedere il nome della zona, l'oggetto e la prova
static void stampa_zona(int i){

      printf("\tZona: [%s]; \n\n\tOggetto: [%s]; \n\n\tProva: [%s];\n\n\tProssima zona: [%s].\n\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona), nome_enum_display_a_parole(giocatori[i]->posizione->oggetto_zona), nome_enum_display_a_parole(giocatori[i]->posizione->prova), nome_enum_display_a_parole(giocatori[i]->posizione->prossima_zona->zona));

}


//funzione che fa avanzare il giocatore nella successiva zona della lista circolare(max 1 volta per turno, senza ausilio di adrenalina)
static void avanza(int i, int* array_avanzamento_giocatori, int refresh) {

  if(array_avanzamento_giocatori[i]==0){
    printf("Non è più possibile avanzare in questo turno per il gioratore [%d] in quanto è già stato effettuato un avanzamento (si consiglia di usare l'adrenalina per avanzare una volta in più)\n", i+1);
    return;
  }

  //teniamo conto della zona in cui si trova correntemente il giocatore
  struct Zona_mappa* zona_corrente = giocatori[i]->posizione;


  //aggiorniamo casualmente il campo prova della nuova zona
  int probabilita_prova = rand() % 100;

  if (probabilita_prova < 40) {
    zona_corrente->prossima_zona->prova = nessuna_prova; // 40% di probabilità di nessuna prova

    if(refresh==1)
    zona_corrente->prossima_zona->oggetto_zona=nessun_oggetto; // 40% probabilità di nessun oggetto ogni 4 turni

  } else if (probabilita_prova < 60) {
    zona_corrente->prossima_zona->prova = prova_EMF; // 20% di probabilità per prova_EMF

    if(refresh==1)
    zona_corrente->prossima_zona->oggetto_zona=adrenalina; // 20% probabilità di adrenalina ogni 4 turni

  } else if (probabilita_prova < 80) {
    zona_corrente->prossima_zona->prova = prova_spirit_box; // 20% di probabilità per prova_spirit_box

    if(refresh==1)
    zona_corrente->prossima_zona->oggetto_zona=cento_dollari; // 20% probabilità di cento_dollari ogni 4 turni

  } else {
    zona_corrente->prossima_zona->prova = prova_videocamera; // 20% di probabilità per prova_videocamera

    if(refresh==1)
    zona_corrente->prossima_zona->oggetto_zona=coltello; // 20% probabilità di nessun coltello ogni 4 turni
  }//non vengono mai generate le carte, le carte si possono ottenere solo nella creazione iniziale della mappa, in quanto sono un oggetto molto potente che può cambiare il corso della partita, quindi ho deciso di renderle molto rare

  //avanziamo sulla zona successiva
  giocatori[i]->posizione = zona_corrente->prossima_zona;

  printf("Sei avanzato nella zona: [%s]\n", nome_enum_display_a_parole(giocatori[i]->posizione->zona));
  array_avanzamento_giocatori[i]--;//senza adrenalina non si avanza due volte nello stesso turno
}


//funzione che permette l'utilizzo degli oggetti contenuti in qualsiasi posizione dello zaino del giocatore che sta eseguendo il turno
static void usa_oggetto(int i, int* array_avanzamento_giocatori){
  if (giocatori[i]->zaino[0] == 5 && giocatori[i]->zaino[1] == 5 && giocatori[i]->zaino[2] == 5 && giocatori[i]->zaino[3] == 5) {
        printf("Lo [zaino] è vuoto, non puoi utilizzare nessun oggetto.\n");
        return;
    }
            int scelta;
do{
        printf("Lo zaino contiene i seguenti oggetti: \n [1]%s\n [2]%s\n [3]%s\n [4]%s\nSi selezioni quello che si preferisce per eseguire una delle seguenti azioni:\n\t[sale], il fantasma non provocherà decremento di sanità mentale\n\t[calmanti], la tua sanità mentale aumenta di 40 punti(max 255)\n\t[100 dollari], al posto dei 100 dollari compare nello zaino un oggetto a caso tra calmante e sale\n\t[coltello], uccide uno degli altri membri della zona se si ha sanità mentale inferiore a 30\n\t[adrenalina], si può avanzare più di una volta lo stesso turno\n\t[carte]: [14%%] di probabilità di ucciderti\n\t         [1%%] di probabilità di uccidere tutti i gicatori\n\t         [15%%] di probabilità di far tornare tutti i giocatori in vita con 1(5%%), 20(5%%) o 70(5%%) di sanità\n\t         [35%%] di ottenere 20 di sanità mentale\n\t         [35%%] di probabilità di perdere 20 di sanità mentale\nPremere [5] se non si vogliono effettuare altre azioni\n", nome_enum_display_a_parole(giocatori[i]->zaino[0]), nome_enum_display_a_parole(giocatori[i]->zaino[1]), nome_enum_display_a_parole(giocatori[i]->zaino[2]), nome_enum_display_a_parole(giocatori[i]->zaino[3]));
        /* stampa: Lo zaino contiene i seguenti oggetti:
        [1]%s [2]%s [3]%s [4]%s
        Si selezioni quello che si preferisce per eseguire una delle seguenti azioni:
        [sale], il fantasma non provocherà decremento di sanità mentale
        [calmanti], la tua sanità mentale aumenta di 40 punti(max 255)
        [100 dollari], al posto dei 100 dollari compare nello zaino un oggetto a caso tra calmante e sale
        [coltello], uccide uno degli altri membri della zona se si ha sanità mentale inferiore a 30
        [adrenalina], si può avanzare più di una volta lo stesso turno
        [carte]: [14%%] di probabilità di ucciderti
                [1%%] di probabilità di uccidere tutti i gicatori
                [15%%] di probabilità di far tornare tutti i giocatori in vita con 1(5%%), 20(5%%) o 70(5%%) di sanità
                [35%%] di ottenere 20 di sanità mentale
                [35%%] di probabilità di perdere 20 di sanità mentale
                Premere [5] se non si vogliono effettuare altre azioni*/
        char temp[100];

            fgets(temp, sizeof(temp), stdin);

            scelta = atoi(temp);

    if (scelta < 1 || scelta > 5) {
      system("clear");
        printf("Scelta non valida. Riprova.\n");
        continue;
    }

    //selezione dell'oggetto in base alla scelta dell'utente
    int oggetto_da_usare = giocatori[i]->zaino[scelta - 1];

    //applicazione dell'effetto dell'oggetto selezionato
        switch (oggetto_da_usare) {

            case sale:
            protezione_fantasma[i]=true;//il giocatore non subirà il decremento di sanità da parte del fantasma in quel turno
            system("clear");
                printf("Hai usato il sale. Il fantasma non provocherà nessun decremento di sanità mentale per questo turno\n");
                giocatori[i]->zaino[scelta - 1]=nulla;
                printf("[sale] rimosso dallo zaino\n");
                break;


            case calmanti:

            if(giocatori[i]->sanita_mentale > 215){//controllo per non sforare oltre 255 di sanità
              giocatori[i]->sanita_mentale = 255;
              printf("Hai usato i calmanti. La tua sanità mentale è arrivata al massimo(255).\n");
              giocatori[i]->zaino[scelta - 1]=nulla;
              printf("[calmanti] rimossi dallo zaino\n");
              break;
            }

                giocatori[i]->sanita_mentale += 40;
                system("clear");
                printf("Hai usato i calmanti. La tua sanità mentale è aumentata di 40 punti.\n");
                giocatori[i]->zaino[scelta - 1]=nulla;
                printf("[calmanti] rimossi dallo zaino\n");
                break;


            case cento_dollari:
            system("clear");
            printf("Hai usato i 100 dollari. Un oggetto casuale (tra [sale] e [calmanti]) verrà aggiunto nel tuo zaino.\n");

                int probabilita = rand() % 100;
                if (probabilita < 50) {//50% di probabilità di avere sale
                  giocatori[i]->zaino[scelta - 1]=sale;
                      printf("[sale] aggiunto nello zaino al posto dei [cento_dollari]\n");
                      break;
                    } else {//altro 50% di probabilità di avere calmanti
                  giocatori[i]->zaino[scelta - 1]=calmanti;
                      printf("[calmanti] aggiunti nello zaino al posto dei [cento_dollari]\n");
                      break;
              }


            case coltello:
            system("clear");
                if (giocatori[i]->sanita_mentale <= 30) {
                    printf("Utilizzo del cortello in corso...\n");

                    for(int j=0; j<numero_giocatori; j++){

                      if(j==i)//un giocatore è sicuramente nella stessa posizione di se stesso, quindi lui non può essere ucciso
                      continue;

                      if(giocatori[j]->posizione->zona == giocatori[i]->posizione->zona){
                        giocatori[j]->sanita_mentale=0;
                        printf("Il giocatore [%d] è morto\n", j+1);
                      }//se un altro giocatore si trova nella stessa posizione di questo giocatore mentre esso usa coltello con meno di 30 sanità mentale, il giocatore viene ucciso

                    }

                    giocatori[i]->zaino[scelta - 1]=nulla;
                    printf("[coltello] rimosso dallo zaino\n");
                } else {
                    printf("La tua sanità mentale è troppo alta per usare il coltello.\n");
                }
                break;


            case adrenalina:
            system("clear");
            array_avanzamento_giocatori[i]++;
            if(array_avanzamento_giocatori[i]==1)
            printf("è nuovamente possibile avanzare in questo turno\n");

            if(array_avanzamento_giocatori[i]==2)
            printf("è possibile avanzare [due] volte in questo turno\n");

            if(array_avanzamento_giocatori[i]==3)
            printf("è possibile avanzare [tre] volte in questo turno\n");

            if(array_avanzamento_giocatori[i]==4)
            printf("è possibile avanzare [quattro] volte in questo turno\n");

            if(array_avanzamento_giocatori[i]==5)
            printf("è possibile avanzare [cinque] volte in questo turno\n");

            if(array_avanzamento_giocatori[i]>5)
            printf("è possibile avanzare moltissime volte in questo turno\n");

            giocatori[i]->zaino[scelta - 1]=nulla;
            printf("[adrenalina] rimossa dallo zaino\n");
                break;


            case carte:

            int continuo_carte=0;//variabile usata per scegliere se continuare o meno ad usare le carte
            int usa_carte=0;//variabile che limita l'uso delle carte a 3 utilizzi

            do{

            int carte_della_s_fortuna=rand() % 100;

            if(carte_della_s_fortuna<14){//14% probabilità di uccidere il giocatore che le usa
              giocatori[i]->sanita_mentale=0;
              printf("Il giocatore [%d] è morto\n", i+1);
              sleep(2);
              return;
            }
             else if(carte_della_s_fortuna==14){//1% probabilità di uccidere tutti i giocatori
              for(int j=0; j<numero_giocatori; j++){
                giocatori[j]->sanita_mentale=0;
                printf("Il giocatore [%d] è morto\n", j+1);
              }
              sleep(3);
              return;
            } else if(carte_della_s_fortuna>14 && carte_della_s_fortuna<=19){//5% probabilità di far risorgere tutti i giocatori morti con 1 di sanità
              for(int j=0; j<numero_giocatori; j++){
                if(giocatori[j]->sanita_mentale!=0)
                printf("Il giocatore [%d] non può tornare in vita con 1 di sanità in quanto non è morto\n", j+1);

                if(giocatori[j]->sanita_mentale==0){
                  giocatori[j]->sanita_mentale=1;
                  printf("Il giocatore [%d] è tornato in vita con 1 di sanità\n", j+1);
                }
              }
            } else if(carte_della_s_fortuna>19 && carte_della_s_fortuna<=24){//5% probabilità di far risorgere tutti i giocatori morti con 20 di sanità
              for(int j=0; j<numero_giocatori; j++){
                if(giocatori[j]->sanita_mentale!=0)
                printf("Il giocatore [%d] non può tornare in vita con 20 di sanità in quanto non è morto\n", j+1);

                if(giocatori[j]->sanita_mentale==0){
                  giocatori[j]->sanita_mentale=20;
                  printf("Il giocatore [%d] è tornato in vita con 1 di sanità\n", j+1);
                }
              }
            } else  if(carte_della_s_fortuna>24 && carte_della_s_fortuna<=29){//5% probabilità di far risorgere tutti i giocatori morti con 70 di sanità
              for(int j=0; j<numero_giocatori; j++){
                if(giocatori[j]->sanita_mentale!=0)
                printf("Il giocatore [%d] non può tornare in vita con 70 di sanità in quanto non è morto\n", j+1);

                if(giocatori[j]->sanita_mentale==0){
                  giocatori[j]->sanita_mentale=70;
                  printf("Il giocatore [%d] è tornato in vita con 1 di sanità\n", j+1);
                }
              }
            } else  if(carte_della_s_fortuna>29 && carte_della_s_fortuna<=64){//35% probabilità di aggiungere 20 di sanità al giocatore che usa le carte
                if(giocatori[i]->sanita_mentale>235){
                giocatori[i]->sanita_mentale=255;
                printf("Il giocatore [%d] ha ottenuto [20] di [sanità mentale] ed ha raggiunto il massimo di sanità ottenibile\n", i+1);
              }else giocatori[i]->sanita_mentale += 20;
                  printf("Il giocatore [%d] ha ottenuto [20] di [sanità mentale]\n", i+1);
                } else if(carte_della_s_fortuna>64 && carte_della_s_fortuna<=99){//35% probabilità di togliere 20 di sanità al giocatore che usa le carte
              if(giocatori[i]->sanita_mentale<20){
              giocatori[i]->sanita_mentale=0;
              printf("il giocatore [%d] è morto (sanità troppo bassa)\n", i+1);
              sleep(2);
              return;
            } else giocatori[i]->sanita_mentale -= 20;
                printf("Il giocatore [%d] ha perso [20] di [sanità mentale]\n", i+1);
              }

              usa_carte++;
              if(usa_carte==3)
              break;

            do{ printf("Le carte possono essere usate fino ad un massimo di 3 volte, premere [1] per usarle un altra volta, premere [2] per terminare l'uso delle carte\n");
              char temp[100]; // Buffer temporaneo per l'input
              fgets(temp, sizeof(temp), stdin); // Legge l'input come stringa
              continuo_carte = atoi(temp); // Converte la stringa in un intero
              if(continuo_carte<1 || continuo_carte>2){
                system("clear");
                printf("Il numero selezionato non rientra tra quelli proposti, selezionare un numero adeguato\n");
              }

            }while(continuo_carte<1 || continuo_carte>2);
          }while(continuo_carte!=2);
             giocatori[i]->zaino[scelta - 1]=nulla;
             printf("\n[carte] rimosse dallo zaino\n\n");

            break;


            case nulla:
            system("clear");
            printf("nessun oggetto presente nello slot selezionato\n\n");
            break;

            default:
            system("clear");
                break;
        }
}while(scelta!=5);
}


//funzione che raccoglie l'oggetto, se presente nella zona, se lo zaino del giocatore è pieno si da all'utente la possibilità di scegliere se svuotare uno spazio oppure se non prendere l'oggetto
static void raccogli_oggetto(int i){

    struct Zona_mappa* zona_attuale = giocatori[i]->posizione;
      //verifichiamo se nella zona è presente o meno un oggetto
      if (zona_attuale->oggetto_zona == nessun_oggetto) {
          printf("Non c'è alcun oggetto in questa zona.\n");
          return;
      }

      int scelta=0;
      //verifichiamo se c'è spazio nello zaino per raccogliere l'oggetto
      int spazio_zaino = 0;
      while (spazio_zaino < 4 && giocatori[i]->zaino[spazio_zaino] != nulla) {
          spazio_zaino++;
      }

      if (spazio_zaino >= 4) {
          printf("Lo zaino è pieno, se si desidera togliere un oggetto premere [1], altrimenti premere qualunque altro tasto per tornare al menù.\n");

          char temp[100];

          fgets(temp, sizeof(temp), stdin);

          scelta = atoi(temp);

          switch (scelta) {

            case 1:
            spazio_zaino=rimozione_oggetto_zaino(i);
            break;


            default:
            return;
          }
      }

      //se c'è l'oggetto e se lo zaino non è pieno, è possibile aggiungerlo allo zaino
      giocatori[i]->zaino[spazio_zaino] = zona_attuale->oggetto_zona;
      zona_attuale->oggetto_zona = nessun_oggetto;
      printf("L'oggetto [%s] è stato raccolto con successo\n", nome_enum_display_a_parole(giocatori[i]->zaino[spazio_zaino]));
  }


//funzione che fa scegliere all'utente quale oggetto rimuovere dallo zaino
static int rimozione_oggetto_zaino(int i){
    system("clear");
int vuoto=0;

  for(int j=0; j<4; j++){
    if(giocatori[i]->zaino[j]==nulla)
    vuoto++;
  }

  if(vuoto==4){//se ci sono 4 spazi vuoi, non c'è nulla da togliere
    printf("Lo zaino del giocatore corrente è vuoto, non è dunque necessario svuotarlo\n");
    return 0;
  }

  int scelta=0;
do{
printf("Si selezioni l'oggetto che si desidera rimuovere dallo zaino: \n");
printf("\tZaino[1] -> %s\n", nome_enum_display_a_parole(giocatori[i]->zaino[0]));
printf("\tZaino[2] -> %s\n", nome_enum_display_a_parole(giocatori[i]->zaino[1]));
printf("\tZaino[3] -> %s\n", nome_enum_display_a_parole(giocatori[i]->zaino[2]));
printf("\tZaino[4] -> %s\n", nome_enum_display_a_parole(giocatori[i]->zaino[3]));

  char temp[100];

  fgets(temp, sizeof(temp), stdin);

  scelta = atoi(temp);

      if (scelta < 1 || scelta > 4) {
          printf("Posizione zaino selezionata non valida, si prega di inserire una posizione valida\n");
      } else {
        if(giocatori[i]->zaino[scelta - 1] == nulla){
          printf("Operazione non valida in quanto la posizione selezionata non contiene nessun oggetto, verrete dunque riportati al menù di scelta\n");
          continue;
        }
         printf("\nLa posizione [%d] dello zaino è stata svuotata con successo\n\n", scelta);
  }
} while (scelta < 1 || scelta > 4);


giocatori[i]->zaino[scelta-1]=5;//svuotiamo lo zaino mettendoci nulla

return (scelta-1);
}


//funzione che ha il 20% di possibilità di togliere 15 di sanità ad un giocatore, avviene quando viene fatta un azione che non è segnata con °(vedi funzione gioca_turno)
static void probabilita_decremento_sanita(int i){

  if(giocatori[i]->sanita_mentale<=15){//se il giocatore ha sanità troppo bassa muore
    if((rand() % 100) < 20){
      giocatori[i]->sanita_mentale =0;
      system("clear");
      printf("La sanità mentale del giocatore [%d] è scesa a 0\n", i+1);
      sleep(2);
    }
  } else {if((rand() % 100) < 20){//altrimenti gli vengono tolti 15 di sanità

    giocatori[i]->sanita_mentale -= 15;
    printf("La sanità mentale del giocatore [%d] è scesa di 15 punti\n", i+1);
    sleep(1);
          }
        }
  }


//funzione usata per raccogliere la prova, se è presente e se il giocatore ha l'oggetto giusto, c'è una certa probabilità che appaia il fantasma
static void raccogli_prova(int i) {
  int probabilita_apparizione_fantasma=0;

      struct Zona_mappa* zona_corrente = giocatori[i]->posizione;

      //verifichiamo se nella zona è presente una prova
      if (zona_corrente->prova == nessuna_prova) {
          printf("In questa zona non c'è alcuna prova da raccogliere.\n");
          return;
      }

      //verifichiamo se il giocatore ha l'oggetto corrispondente alla prova nello zaino
      int presenza_oggetto_per_racogliere_la_prova=-1;

      if (zona_corrente->prova == prova_EMF)//se nella zona c'è prova_EMF
      for (int j = 0; j < 4; j++){
        if (giocatori[i]->zaino[j] == EMF){//e il giocatore ha l'oggetto essenziale giusto, allora sarà possibile procedere con la raccolta della prova
          presenza_oggetto_per_racogliere_la_prova = j;
          break;
        }

      }

        if(zona_corrente->prova == prova_spirit_box)//stessa cosa con le altre due prove
        for (int j = 0; j < 4; j++){
          if (giocatori[i]->zaino[j] == spirit_box){
          presenza_oggetto_per_racogliere_la_prova = j;
          break;
        }

      }

        if (zona_corrente->prova == prova_videocamera)
          for (int j = 0; j < 4; j++){
              if (giocatori[i]->zaino[j] == videocamera){
                  presenza_oggetto_per_racogliere_la_prova = j;
                  break;
              }

            }

      if (presenza_oggetto_per_racogliere_la_prova == -1) {//se il giocatore non ha la prova giusta allora non la può raccogliere
          printf("Per raccogliere questa prova è necessario avere l'oggetto relativo alla prova.\n");
          return;
      }

      //si raccoglie la prova e la si sostituisce con l'oggetto corrispondente nello zaino
      giocatori[i]->zaino[presenza_oggetto_per_racogliere_la_prova] = zona_corrente->prova;
      zona_corrente->prova = nessuna_prova;//viene rimossa tale prova dalla zona
      printf("[%s] raccolta con successo\n", nome_enum_display_a_parole(giocatori[i]->zaino[presenza_oggetto_per_racogliere_la_prova]));

      //arrivati a questo punto una prova è sicuramente stata presa, quindi si può verificare l'apparizzione del fantasma
      //gestione della probabilità di apparizione del fantasma e di decremento della sanità mentale
      if(difficolta==1)
        probabilita_apparizione_fantasma = 10;//probabilità iniziale di apparizione del fantasma(caso difficoltà 1)

      if(difficolta==2)
        probabilita_apparizione_fantasma = 15;//probabilità iniziale di apparizione del fantasma(caso difficoltà 2)

      if(difficolta==3)
        probabilita_apparizione_fantasma = 25;//probabilità iniziale di apparizione del fantasma(caso difficoltà 3)

      Apparizzione_fantasma(probabilita_apparizione_fantasma, i);

}


//funzione che blocca la possibilità di andare nel caravan per tutto il turno a tutti i giocatori
static void Apparizzione_fantasma(int probabilità_apparizione_fantasma, int i){

  int numero_prove_raccolte=Numero_prove_trovate();

  //10% base, 20% con una prova, 30% con 2 prove(difficoltà 1)
  //15% base, 30% con una prova, 45% con 2 prove(difficoltà 2)
  //25% base, 50% con una prova, 75% con 2 prove(difficoltà 3)
  int random_phantom=probabilità_apparizione_fantasma*(1+numero_prove_raccolte);

  int numero_casuale = rand() % 100;
  if (random_phantom<numero_casuale) {//probabilità di apparizzione del fantasma
    printf("Il fantasma è apparso!\n");
    giocatori[i]->posizione->presenza_fantasma=true;

    for(int j=0; j<numero_giocatori; j++) {
      if(protezione_fantasma[j]){
        printf("il giocatore [%d] non subirà nessun decremento di sanità, in quanto ha precedentemente usato il sale\n", j+1);
        protezione_fantasma[j]=false;
        continue;
      }

if(giocatori[j]->sanita_mentale <= probabilità_apparizione_fantasma * (1+numero_prove_raccolte)){
  giocatori[j]->sanita_mentale=0;
  printf("il giocatore [%d] è morto\n", j+1);
continue;
}

printf("la sanità del giocatore [%d] è scesa da [%d] ", j+1, giocatori[j]->sanita_mentale);
      //togliamo alla sanità del giocatore un valore che si basa sulla difficoltà selezionata([1], [2], [3])
      //sanita_mentale= sanita_mentale - (10 *numero di prove raccolte) [1]
      //sanita_mentale= sanita_mentale - (15 *numero di prove raccolte) [2]
      //sanita_mentale= sanita_mentale - (25 *numero di prove raccolte) [3]
      giocatori[j]->sanita_mentale -= probabilità_apparizione_fantasma * (1+numero_prove_raccolte);

      printf("a [%d]\n", giocatori[j]->sanita_mentale);
    }


  } else {

    printf("Il fantasma non è apparso\n");
  }
}


//funzione che tiene il conto delle prove raccolte
static int Numero_prove_trovate(void){
  int numero_prove_raccolte=0;

  if(trova_prova_EMF)
  numero_prove_raccolte++;

  if(trova_prova_spirit_box)
  numero_prove_raccolte++;

  if(trova_prova_videocamera)
  numero_prove_raccolte++;

  return numero_prove_raccolte;
}


/*funzione che controlla la possibilità di continuare il gioco,
se nessun giocatore ha in nessuna posizione un oggetto essenziale, allora viene dato un oggetto essenziale,
se c'è una posizione libera viene messo li, altrimenti un oggetto casuale viene sostituito con un essenziale*/
static void controllo_zaino_giocatori(int i){

  for(int j=0; j<numero_giocatori; j++){
    for(int k=0; k<4; k++){

      if(giocatori[j]->sanita_mentale==0)/*se un giocatore morto ha un oggetto essenziale non lo può usare, quindi il gioco si deve
      comuneue considerare non completabile*/
      continue;

      if(giocatori[j]->zaino[k]==EMF || giocatori[j]->zaino[k]==spirit_box || giocatori[j]->zaino[k]==videocamera || giocatori[j]->zaino[k]==prova_EMF || giocatori[j]->zaino[k]==prova_spirit_box || giocatori[j]->zaino[k]==prova_videocamera){
        return;
      }
    }
  }

  //se non viene trovato nessun oggetto fondamentale, o perché il giocatore che lo possedeva è morto o perché tale oggetto è stato rimosso dallo zaino,
  //il gioco deve poter continuare, e quindi viene dato un oggetto casuale tra i fondamentali ad un giocatore casuale
for(int j=0; j<numero_giocatori; j++){
  for(int k=0; k<4; k++){
if(giocatori[j]->zaino[k]==5){
  printf("L'oggetto [%s] è stato scambiato al giocatore [%d] con ", nome_enum_display_a_parole(giocatori[j]->zaino[k]), j+1);
  giocatori[j]->zaino[k]=oggetto_essenziale_prova_false();//se c'è uno slot libero allora in tale slot viene messo un oggetto essenziale
  printf("[%s], altrimenti non sarebbe stato possibile completare il gioco\n", nome_enum_display_a_parole(giocatori[j]->zaino[k]));
  return;
}
}
}

//se non è stato trovato nessun posto libero nello zaino allora viene scambiato un oggetto casuale
int scambio=rand() % 4;//posizione casuale dello zaino
int giocatore_scambio=rand() %numero_giocatori;
printf("L'oggetto [%s] è stato scambiato al giocatore [%d] con ", nome_enum_display_a_parole(giocatori[giocatore_scambio]->zaino[scambio]), giocatore_scambio+1);
giocatori[giocatore_scambio]->zaino[scambio]=oggetto_essenziale_prova_false();
printf("[%s], altrimenti non sarebbe stato possibile completare il gioco\n", nome_enum_display_a_parole(giocatori[giocatore_scambio]->zaino[scambio]));
}


//funzione che restituisce il numero dell'oggetto relativo alla prova ancora non completata, funzione usata su torna_caravan
static int oggetto_essenziale_prova_false(void){

if(!trova_prova_EMF)
return 0;

if(!trova_prova_spirit_box)
return 1;

if(!trova_prova_videocamera)
return 2;

return 5;
}


//menù grafico in caso di sconfitta
static void Sconfitta(void) {
  system("clear");
printf(" SS:::::::::::::::S     CCC::::::::::::C   OO:::::::::OO   N:::::::N       N::::::NF::::::::::::::::::::FI::::::::IT:::::::::::::::::::::TT:::::::::::::::::::::T        A:::A              \n");
printf("S:::::SSSSSS::::::S   CC:::::::::::::::C OO:::::::::::::OO N::::::::N      N::::::NF::::::::::::::::::::FI::::::::IT:::::::::::::::::::::TT:::::::::::::::::::::T       A:::::A             \n");
printf("S:::::S     SSSSSSS  C:::::CCCCCCCC::::CO:::::::OOO:::::::ON:::::::::N     N::::::NFF::::::FFFFFFFFF::::FII::::::IIT:::::TT:::::::TT:::::TT:::::TT:::::::TT:::::T      A:::::::A            \n");
printf("S:::::S             C:::::C       CCCCCCO::::::O   O::::::ON::::::::::N    N::::::N  F:::::F       FFFFFF  I::::I  TTTTTT  T:::::T  TTTTTTTTTTTT  T:::::T  TTTTTT     A:::::::::A           \n");
printf("S:::::S            C:::::C              O:::::O     O:::::ON:::::::::::N   N::::::N  F:::::F               I::::I          T:::::T                T:::::T            A:::::A:::::A          \n");
printf(" S::::SSSS         C:::::C              O:::::O     O:::::ON:::::::N::::N  N::::::N  F::::::FFFFFFFFFF     I::::I          T:::::T                T:::::T           A:::::A A:::::A         \n");
printf("  SS::::::SSSSS    C:::::C              O:::::O     O:::::ON::::::N N::::N N::::::N  F:::::::::::::::F     I::::I          T:::::T                T:::::T          A:::::A   A:::::A        \n");
printf("    SSS::::::::SS  C:::::C              O:::::O     O:::::ON::::::N  N::::N:::::::N  F:::::::::::::::F     I::::I          T:::::T                T:::::T         A:::::A     A:::::A       \n");
printf("       SSSSSS::::S C:::::C              O:::::O     O:::::ON::::::N   N:::::::::::N  F::::::FFFFFFFFFF     I::::I          T:::::T                T:::::T        A:::::AAAAAAAAA:::::A      \n");
printf("            S:::::SC:::::C              O:::::O     O:::::ON::::::N    N::::::::::N  F:::::F               I::::I          T:::::T                T:::::T       A:::::::::::::::::::::A     \n");
printf("            S:::::S C:::::C       CCCCCCO::::::O   O::::::ON::::::N     N:::::::::N  F:::::F               I::::I          T:::::T                T:::::T      A:::::AAAAAAAAAAAAA:::::A    \n");
printf("SSSSSSS     S:::::S  C:::::CCCCCCCC::::CO:::::::OOO:::::::ON::::::N      N::::::::NFF:::::::FF           II::::::II      TT:::::::TT            TT:::::::TT   A:::::A             A:::::A   \n");
printf("S::::::SSSSSS:::::S   CC:::::::::::::::C OO:::::::::::::OO N::::::N       N:::::::NF::::::::FF           I::::::::I      T:::::::::T            T:::::::::T  A:::::A               A:::::A  \n");
printf("S:::::::::::::::SS      CCC::::::::::::C   OO:::::::::OO   N::::::N        N::::::NF::::::::FF           I::::::::I      T:::::::::T            T:::::::::T A:::::A                 A:::::A \n");
printf(" SSSSSSSSSSSSSSS           CCCCCCCCCCCCC     OOOOOOOOO     NNNNNNNN         NNNNNNNFFFFFFFFFFF           IIIIIIIIII      TTTTTTTTTTT            TTTTTTTTTTTAAAAAAA                   AAAAAAA\n");
sleep(3);
}


//menù grafico in caso di vittoria
static void Vittoria(void) {
  system("clear");

  printf("complimenti, hai vinto il gioco con un livello di difficoltà pari a [%d], con [%d] giocatore/i e in [%d] turni, il numero di giocatori sopravvissuti fino alla fine del gioco è di [%d]\n\n", difficolta, numero_giocatori, turno, numero_giocatori_vivi());


printf("VVVVVVVV           VVVVVVVVIIIIIIIIIITTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT     OOOOOOOOO     RRRRRRRRRRRRRRRRR   IIIIIIIIII               AAA               \n");
printf("V::::::V           V::::::VI::::::::IT:::::::::::::::::::::TT:::::::::::::::::::::T   OO:::::::::OO   R::::::::::::::::R  I::::::::I              A:::A              \n");
printf("V::::::V           V::::::VI::::::::IT:::::::::::::::::::::TT:::::::::::::::::::::T OO:::::::::::::OO R::::::RRRRRR:::::R I::::::::I             A:::::A             \n");
printf("V::::::V           V::::::VII::::::IIT:::::TT:::::::TT:::::TT:::::TT:::::::TT:::::TO:::::::OOO:::::::ORR:::::R     R:::::RII::::::II            A:::::::A            \n");
printf(" V:::::V           V:::::V   I::::I  TTTTTT  T:::::T  TTTTTTTTTTTT  T:::::T  TTTTTTO::::::O   O::::::O  R::::R     R:::::R  I::::I             A:::::::::A           \n");
printf("  V:::::V         V:::::V    I::::I          T:::::T                T:::::T        O:::::O     O:::::O  R::::R     R:::::R  I::::I            A:::::A:::::A          \n");
printf("   V:::::V       V:::::V     I::::I          T:::::T                T:::::T        O:::::O     O:::::O  R::::RRRRRR:::::R   I::::I           A:::::A A:::::A         \n");
printf("    V:::::V     V:::::V      I::::I          T:::::T                T:::::T        O:::::O     O:::::O  R:::::::::::::RR    I::::I          A:::::A   A:::::A        \n");
printf("     V:::::V   V:::::V       I::::I          T:::::T                T:::::T        O:::::O     O:::::O  R::::RRRRRR:::::R   I::::I         A:::::A     A:::::A       \n");
printf("      V:::::V V:::::V        I::::I          T:::::T                T:::::T        O:::::O     O:::::O  R::::R     R:::::R  I::::I        A:::::AAAAAAAAA:::::A      \n");
printf("       V:::::V:::::V         I::::I          T:::::T                T:::::T        O:::::O     O:::::O  R::::R     R:::::R  I::::I       A:::::::::::::::::::::A     \n");
printf("        V:::::::::V          I::::I          T:::::T                T:::::T        O::::::O   O::::::O  R::::R     R:::::R  I::::I      A:::::AAAAAAAAAAAAA:::::A    \n");
printf("         V:::::::V         II::::::II      TT:::::::TT            TT:::::::TT      O:::::::OOO:::::::ORR:::::R     R:::::RII::::::II   A:::::A             A:::::A   \n");
printf("          V:::::V          I::::::::I      T:::::::::T            T:::::::::T       OO:::::::::::::OO R::::::R     R:::::RI::::::::I  A:::::A               A:::::A  \n");
printf("           V:::V           I::::::::I      T:::::::::T            T:::::::::T         OO:::::::::OO   R::::::R     R:::::RI::::::::I A:::::A                 A:::::A \n");
printf("            VVV            IIIIIIIIII      TTTTTTTTTTT            TTTTTTTTTTT           OOOOOOOOO     RRRRRRRR     RRRRRRRIIIIIIIIIIAAAAAAA                   AAAAAAA\n");
printf("Tornerai automaticamente al menù iniziale fra 12 secondi\n");
sleep(12);
  }


//funzione che ritorna il numero di giocatori sopravvissuti
static int numero_giocatori_vivi(void){
  int giocatori_vivi=0;

  for(int i=0; i<numero_giocatori; i++){
    if(giocatori[i]->sanita_mentale!=0)
    giocatori_vivi++;

  }
  return giocatori_vivi;
}


/////////////////////////////
void termina_gioco(void){

  if(giocatori!=NULL)//se precedentemente sono stati inseriti(e quindi anche allocati) dei giocatori, si procede a deallocarli
  Dealloca_giocatori();

  if(mappa_creata)//se precedentemente è stata creata una mappa occorre prima rimuovere i dati creati
  cancella_tutte_le_zone();

  if(zona_caravan!=NULL)
  free(zona_caravan);


  system("clear");
        printf("\rGrazie per aver giocato\n");
        sleep(1);

        printf("\r\tPhalsophobia, un gioco in C\n");
        sleep(1);

        menu_titolo();
        sleep(4);

    }//
/////////////////////////////
