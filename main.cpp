#include "Header.h";
#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;

int startMenu();
LARGE_INTEGER startTimer();
LARGE_INTEGER endTimer();

int main() {
	ProductionModel pm;
	ProductionModel *pmTest;
	LARGE_INTEGER performanceCountStart, performanceCountEnd;
	string fname;
	int number_of_cycles;
	int *endPathBF;
	double timeBF = 0;
	double timeDyn = 0;
	bool fileLoaded = false;
	
	int choice = startMenu();
	while (choice >= 1 && choice <= 4) {
		switch (choice) {
		case 1:
			system("cls");
			cout << "Prosze podac plik: " << endl;
			cin >> fname;
			fileLoaded = pm.getFromFile(fname);
			pm.createGraph();
			number_of_cycles = pm.getNumberOfCycles();
			choice = startMenu();
			break;
		case 2:
			system("cls");
			if (fileLoaded == false) {
				cout << "Nie wczytano pliku" << endl;
				cin.get();
				cin.get();
				choice = startMenu();
				break;
			}
			performanceCountStart = startTimer();
			pm.permutationCore(number_of_cycles);
			performanceCountEnd = endTimer();
			endPathBF = pm.getBestPath();
			cout << "Calkowity koszt: " << pm.getbestCost() << endl;
			cout << "Droga: ";
			for (int i = 0; i < number_of_cycles; i++) {
				cout << endPathBF[i] << " -> ";
			}
			cout << "0" << endl;
			timeBF = performanceCountEnd.QuadPart - performanceCountStart.QuadPart;
			timeBF /= 1000.0;
			cout << "Czas: " << timeBF << " ms" << endl;
			cin.get();
			cin.get();
			choice = startMenu();
			break;
		case 3:
			system("cls");
			if (fileLoaded == false) {
				cout << "Nie wczytano pliku" << endl;
				cin.get();
				cin.get();
				choice = startMenu();
				break;
			}
			performanceCountStart = startTimer();
			pm.dynamic();
			performanceCountEnd = endTimer();
			timeDyn = performanceCountEnd.QuadPart - performanceCountStart.QuadPart;
			timeDyn /= 1000.0;
			cout << "Czas: " << timeDyn << " ms" << endl;
			pm.showResult();
			cin.get();
			cin.get();
			choice = startMenu();
			break;
		case 4:
			system("cls");
			int warehouse_space;
			int demand;
			int repeat = 20;
			cout << "Podaj ilosc cykli: ";
			cin >> number_of_cycles;
			cout << "Podaj pojemnosc magazynu: ";
			cin >> warehouse_space;
			cout << "Podaj popyt: ";
			cin >> demand;

			for (int i = 0; i < repeat; i++) {
				pmTest = new ProductionModel(warehouse_space - 1, demand);
				pm.fillAuto(number_of_cycles);
				pm.createGraph();
				performanceCountStart = startTimer();
				pm.permutationCore(number_of_cycles);
				performanceCountEnd = endTimer();
				timeBF += performanceCountEnd.QuadPart - performanceCountStart.QuadPart;
				delete pmTest;
			}

			for (int i = 0; i < repeat; i++) {
				pmTest = new ProductionModel(warehouse_space-1, demand);
				pm.fillAuto(number_of_cycles);
				pm.createGraph();
				performanceCountStart = startTimer();
				pm.dynamic();
				performanceCountEnd = endTimer();
				timeDyn += performanceCountEnd.QuadPart - performanceCountStart.QuadPart;
				delete pmTest;
			}
			timeBF /= (double)repeat;
			timeBF /= 1000.0;
			cout << "Sredni czas " << repeat << " powtorzen dla przegladu zupelnego: " << timeBF << " ms" << endl;


			timeDyn /= (double)repeat;
			timeDyn /= 1000.0;
			cout << "Sredni czas " << repeat << " powtorzen dla programowanie dynamicznego to: " << timeDyn << " ms" << endl;
			cin.get();
			cin.get();
			choice = startMenu();
			break;
		}
	}
	return 0;
}

int startMenu() {
	int choice;
	system("cls");
	cout << "Problem inwentaryzacji. Viktor Rys i Dawid Dabrowski" << endl;
	cout << "\n1. Wczytaj z pliku\n2. Przeglad zupelny (Bruteforce)\n3. Programowanie dynamiczne\n4. Testy\nInny znak - Wyjscie";
	cout << "\nWybor: ";
	cin >> choice;
	return choice;
}

LARGE_INTEGER startTimer() {
	LARGE_INTEGER start;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return start;
}

LARGE_INTEGER endTimer() {
	LARGE_INTEGER end;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&end);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return end;
}
