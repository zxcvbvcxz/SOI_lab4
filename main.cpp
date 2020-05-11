#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>  
#include "monitor.h"
#include "Kolejka.cpp"

using namespace std;

k_kolejka kolejka_1, kolejka_2;

void *czytelnik_1(void *){
	double sekundy = 1 + (rand()%4) / (rand()%10 + 1); 
	while(true){
		kolejka_1.czytaj();
		usleep(sekundy  * 1000000);
		sekundy = 1 + (rand()%4) / (rand()%10 + 1);// od 1 do 5
	}
}

void *czytelnik_2(void *){
	double sekundy = 1 + (rand()%4) / (rand()%10 + 1); 
	while(true){
		kolejka_2.czytaj();
		usleep(sekundy  * 1000000);
		sekundy = 1 + (rand()%4) / (rand()%10 + 1);// od 1 do 5
	}
}

void *producent_1(void *){
	int id_w = 1;
	wiadomosc msg(1, 1111111111,id_w);
	double sekundy = 1 + (rand()%4) / (rand()%10 + 1); 
	while(true){
		kolejka_1.dodaj(msg);
		id_w++;
		msg.id_wiadomosci = id_w;
		usleep(sekundy  * 1000000);
		sekundy = 1 + (rand()%4) / (rand()%10 + 1);
	}
}

void *producent_2(void *){
	int id_w = 1;
	wiadomosc msg(2, 222222222,id_w);
	double sekundy = 1 + (rand()%4) / (rand()%10 + 1); 
    while(true){
		kolejka_2.dodaj(msg);
		id_w++;
		msg.id_wiadomosci = id_w;
		usleep(sekundy  * 1000000);
		sekundy = 1 + (rand()%4) / (rand()%10 + 1);
	}
}

void *konsument(void *){
	double sekundy = 1 + (rand()%4) / (rand()%10 + 1);
	while(true){
		kolejka_1.wyjmij();
		kolejka_2.wyjmij();
		usleep(sekundy  * 1000000);
		sekundy = 1 + (rand()%4) / (rand()%10 + 1); 
	}
}

int main() {
    pthread_t c1, c2, p1, p2, k;

    srand((unsigned) time(NULL));

    pthread_create(&c1, NULL, czytelnik_1, NULL);
    pthread_create(&c2, NULL, czytelnik_2, NULL);
    pthread_create(&p1, NULL, producent_1, NULL);
    pthread_create(&p2, NULL, producent_2, NULL);
    pthread_create(&k, NULL, konsument, NULL);

    pthread_join(c1, NULL);
    pthread_join(c2, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(k, NULL);

    return 0;
}

