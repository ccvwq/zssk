#include "Header.h";
#include <string>
#include <iostream>
using namespace std;

int main() {
	ProductionModel pm;
	string fname;
	cout << "Prosze podac plik: " << endl;
	cin >> fname;
	pm.getFromFile(fname);
	pm.createGraph();
	pm.bruteForce();
	pm.showResult();
	cin.get();
	cin.get();
}