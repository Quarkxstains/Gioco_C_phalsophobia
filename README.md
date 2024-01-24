# PhalsoPhobia: Un gioco in C
Template progetto finale Programmazione Procedurale con Laboratorio a.a. 2022/2023.

Aggiungere a questo README eventuali estensioni al progetto assegnato, e scelte progettuali da comunicare.


main.c: nessuna aggiunta particolare


gamelib.h: aggiunta di [nulla] tra gli [enum Tipo_oggetto_iniziale], aggiunta di [carte] tra gli [enum Tipo_oggetto_zona],
           aggiunta del booleano [bool presenza_fantasma] tra gli [struct Zona_mappa]

           CARTE: Le carte, ispirandomi al gioco originale(apportando varie modifiche) hanno:
                    [14%] di probabilità di uccidere il giocatore che le usa;
                    [1%] di probabilità di uccidere tutti i gicatori;
                    [15%] di probabilità di far tornare tutti i giocatori in vita con 1(5%), 20(5%) o 70(5%) di sanità;
                    [35%] di ottenere 20 di sanità mentale;
                    [35%] di probabilità di perdere 20 di sanità mentale;


gamelib.c: IMPOSTA_GIOCO: nel menù di creazione della mappa di gioco ho inserito 3 ulteriori funzioni:
           [inserisci_tutte_le_zone], che permette di inserire tutte e 6 le zone in maniera casuale;
           [modifica_zona_casualmente], che permette di modificare il campo oggetto_zona di una zona estratta casualmente, se essa fa parte della mappa;
           [cancella_tutte_le_zone], che permette di cancellare tutte le zone inserite e di riavere una mappa vuota.
           aggiunta della funzione [nome_enum_display_a_parole], che stampa i nomi degli enum invece di lasciare i numeri.

           GIOCA: aggiunta della funzione [rimozione_oggetto_zaino], che permette di rimuovere un oggetto a scelta.
           aggiunta della funzione [controllo_zaino_giocatori], che controlla lo zaino di tutti i giocatori capacitati a giocare, in modo da stabilire
           se il gioco è completabile o meno, in caso negativo da ad un giocatore casuale(capace di giocare) un oggetto essenziale casuale che manca.
           aggiunta della funzione [numero_giocatori_vivi], serve per stampare, in caso di vittoria, quanti giocatori sul totale iniziale sono riusciti
           a completare la partita.


Nome: Davide

Cognome: Bistocchi

Matricola: 356014
