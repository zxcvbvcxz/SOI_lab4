#include "monitor.h"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <ctime>  

#define ROZMIAR_KOLEJKI 4

using namespace std;

class wiadomosc{
public:
	int id_kolejki;
	int id_producenta;
	int id_wiadomosci;

	wiadomosc(int id_k,int id_p,int id_w)
	{
		id_kolejki = id_k;
		id_producenta = id_p;
		id_wiadomosci = id_w;
	}
	wiadomosc()
	{
		id_kolejki = 0;
		id_producenta = 0;
		id_wiadomosci = 0;
	}
};

class k_kolejka : Monitor{
public:
	Condition full, empty, mozna_czytac, mozna_pisac;
	int czytelnicy_liczba;
	int prod_i_kons_liczba;

	int max_rozmiar;;
	int glowa;
	int licznik;
	wiadomosc kolejka[ROZMIAR_KOLEJKI];

	int pocz_czytania()
	{
		enter();
		if(prod_i_kons_liczba != 0 or !isEmpty(mozna_pisac))
		{
			wait(mozna_czytac);
		}
		czytelnicy_liczba++;
		signal(mozna_czytac);
		leave();
	}

	int kon_czytania()
	{
		enter();
		czytelnicy_liczba--;
		if(czytelnicy_liczba == 0)
		{
			signal(mozna_pisac);
		}
		leave();
	}

	int pocz_pisania()
	{
		enter();
		if(prod_i_kons_liczba != 0 or czytelnicy_liczba != 0)
		{
			wait(mozna_pisac);
		}
		prod_i_kons_liczba++;
		leave();
	}

	int kon_pisania()
	{
		enter();
		prod_i_kons_liczba--;
		if(isEmpty(mozna_czytac))
		{
			signal(mozna_pisac);
		}else
		{
			signal(mozna_czytac);
		}
		leave();
	}





	int dodaj(wiadomosc msg)
	{
//		enter();
pocz_pisania();
		if (licznik == ROZMIAR_KOLEJKI){
kon_pisania();
			wait(full);
pocz_pisania();
		}
		int koniec = (glowa + licznik) % max_rozmiar;
		kolejka[koniec] = msg;
		licznik++;
		std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
		cout << std::ctime(&end_time);
		printf("Dodalem do %d kolejki\n\n", msg.id_kolejki);
		signal(empty);
kon_pisania();
//		leave();
	}

	int wyjmij()
	{
//		enter();
pocz_pisania();
		if (licznik == 0){
kon_pisania();	
			wait(empty);
pocz_pisania();
		}
		std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
		cout << std::ctime(&end_time);
		printf("Wyjalem z %d kolejki\n\n", kolejka[glowa].id_kolejki);
		glowa = (glowa == max_rozmiar - 1) ? 0 : glowa + 1;
		licznik--;
		signal(full);
kon_pisania();
//		leave();
	}

	int czytaj()
	{
//		enter();
		pocz_czytania();
		if (licznik == 0){
		kon_czytania();
			wait(empty);
		pocz_czytania();
		}
		std::time_t end_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
		cout << std::ctime(&end_time);
		printf("id_kolejki: %d\nid_producenta: %d\nid_wiadomosci: %d\n\n",kolejka[glowa].id_kolejki, kolejka[glowa].id_producenta, kolejka[glowa].id_wiadomosci);
		kon_czytania();
//		leave();
	}

	k_kolejka() : Monitor()
	{
		max_rozmiar = ROZMIAR_KOLEJKI;
		glowa = 0;
		licznik = 0;
		czytelnicy_liczba = 0;
		prod_i_kons_liczba=0;
	}
};


