#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "gamelib.h"

//Funzione principale del gioco
int main(void){
  int numero_menu=0;
do    {

        menu_principale();

        char temp[100]; //buffer temporaneo per l'input

        fgets(temp, sizeof(temp), stdin); /*leggiamo l'input come stringa dall'array di caratteri temp,
        successivamnete gli passiamo la dimensione di tale array per evitare overflow
        e infine andiamo a leggere l'input da tastiera tramite stdin per poi memorizzarla dentro temp*/

        numero_menu = atoi(temp); /*conversione della stringa in un intero, restituisce 0 se trova un carattere non numerico.
        Con atoi non è possibile distinguere il caso in cui l'utente mette 0 rispetto ad un qualunque valore non numerico,
        ecco perché è necessario non includere lo 0 tra le scelte valide*/


        switch(numero_menu)   {

            case 1:
              imposta_gioco();
              break;
            case 2:
              gioca();
              break;
              case 3:
              //termina_gioco()
              break;
            default:
              //system("clear");
              printf("Il comando inserito non rientra tra i comandi validi, si prega di inserire 1, 2 oppure 3\n\n");
              sleep(1);

      }

  }  while(numero_menu != 3);
  termina_gioco();
  system("clear");
    return 0;

  }
