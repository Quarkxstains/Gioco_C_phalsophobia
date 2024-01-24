enum Tipo_oggetto_iniziale{EMF=0, spirit_box, videocamera, calmanti, sale, nulla};//aggiunta di nulla per non mettere nulla nello zaino all'inizio

enum Tipo_oggetto_zona{adrenalina=6, cento_dollari, coltello, carte, nessun_oggetto};//aggiunta di carte, si può generare solo durante la creazione della mappa, la descrizione è scritta su usa_oggetto, sulla sezione delle carte

enum Tipo_zona{caravan=15, cucina, soggiorno, camera, bagno, garage, seminterrato};

enum Tipo_prova{prova_EMF=11, prova_spirit_box, prova_videocamera, nessuna_prova};
//la numerazione degli enum serve per la funzuone nome_enum_display_a_parole(), che invece di stampare un numero mi stampa il nome, per usare un unica funzione come unico modo ho trovato quello di numerarli in successione, stando attento nel programma a non sbagliare i range dei rand o l'assegnazione di valori specifici

//Struttura per definire dove siamo nella mappa.
struct Zona_mappa {
  enum Tipo_zona zona;
  enum Tipo_oggetto_zona oggetto_zona;
  enum Tipo_prova prova;
  bool presenza_fantasma;
  struct Zona_mappa* prossima_zona; //puntatore che punta alla prossima zona della mappa.
};

//Struttura per definire le caratteristiche di un giocatore.
struct Giocatore {
  struct Zona_mappa* posizione;//puntatore a dove si trova il giocatore
  unsigned char sanita_mentale;//sanità mentale, max 255
  unsigned char zaino[3];//zaino da 4 posizioni
  unsigned char Nome_giocatore[50];//campo nome
};


//Dichiarazione delle funzioni usate in main.c e gamelib.c.
void menu_principale(void);
void imposta_gioco(void);
void gioca(void);
void termina_gioco(void);
int isNumeric(const char *);
