#include "Header.h";
#include <fstream>
#include <string>
#include <climits>
#include <iostream>
#include <limits.h>

ProductionModel::ProductionModel() {
	warehouse_space = 2;
	demand = 2;
	tempCost = 0;
	bestCost = INT_MAX;
}

ProductionModel::ProductionModel(int warehouse_space, int demand) {
	this->warehouse_space = warehouse_space;
	this->demand = demand;
	tempCost = 0;
	bestCost = INT_MAX;
}

int ProductionModel::getNumberOfCycles() {
	return number_of_cycles;
}

int ProductionModel::getbestCost() {
	return bestCost;
}

int* ProductionModel::getBestPath() {
	return bestPath;
}

bool ProductionModel::getFromFile(std::string fname) {
	std::fstream file;
	file.open(fname, std::ios::in);
	if (file.good() == true) {
		int cycles, holding_cost, production_cost;
		file >> cycles;
		number_of_cycles = cycles;
		unit_holding_cost = new int[cycles];
		unit_proudction_cost = new int[cycles];
		for (int i = 0; i < cycles; i++) {
			file >> production_cost >> holding_cost;
			unit_holding_cost[i] = holding_cost;
			unit_proudction_cost[i] = production_cost;
		}
		file.close();
		std::cout << "Plik wczytany" << std::endl;
		std::cin.get();
		std::cin.get();
		return true;
	}
	else {
		std::cout << "Nie mozna wczytac pliku" << std::endl;
		return false;
	}
}

void ProductionModel::fillAuto(int cycles) {
	number_of_cycles = cycles;
	unit_holding_cost = new int[cycles];
	unit_proudction_cost = new int[cycles];
	for (int i = 0; i < cycles; i++) {
		unit_holding_cost[i] = ((rand() % 8) + 1);
		unit_proudction_cost[i] = ((rand() % 24) + 1);
	}
}

void ProductionModel::createGraph() {
	number_of_verticies = 2 + (warehouse_space + 1)*(number_of_cycles - 1);
	number_of_edges = (number_of_verticies - (warehouse_space + 1)) * (warehouse_space + 1);
	currentVertex = new int[number_of_cycles + 1];
	bestPath = new int[number_of_cycles + 1];
	for (int i = 0; i < number_of_cycles + 1; i++) {
		currentVertex[i] = 0;
		bestPath[i] = 0;
	}
	bestCost = INT_MAX;
	vertexes = new Node*[number_of_cycles + 1];
	vertexes[0] = new Node;
	for (int i = 1; i < number_of_cycles; i++) {
		vertexes[i] = new Node[(warehouse_space + 1)];
	}
	vertexes[number_of_cycles] = new Node;

	vertexes[number_of_cycles][0].id = 0;
	idCounter = 0;
	vertexes[0][0].id = idCounter;
	vertexes[0][0].minCost = INT_MAX;
	for (int i = 1; i < number_of_cycles; i++) {
		for (int j = 0; j < warehouse_space + 1; j++) {
			vertexes[i][j].id = ++idCounter;
			vertexes[i][j].minCost = INT_MAX;
		}
	}
	vertexes[number_of_cycles][0].id = ++idCounter;
	lastId = idCounter;
	E = new Edge[number_of_edges];
	for (int i = 0; i <= warehouse_space; i++) {
		E[i].unit_cost = unit_proudction_cost[0];
		E[i].hold_cost = unit_holding_cost[0];
		E[i].cost = (demand + i)*E[i].unit_cost;
		E[i].start = &vertexes[0][0];
		E[i].end = &vertexes[1][i];
	}
	int edge_counter = warehouse_space + 1;
	for (int i = 1; i < number_of_cycles; i++) {
		for (int j = 0; j < (warehouse_space + 1); j++) { 
			if (i != number_of_cycles - 1) {
				for (int k = 0; k <= warehouse_space; k++) {
					E[edge_counter].unit_cost = unit_proudction_cost[i];
					E[edge_counter].hold_cost = unit_holding_cost[i];
					E[edge_counter].cost = (((demand - j) + k)*E[edge_counter].unit_cost) + (j*E[edge_counter].hold_cost);
					E[edge_counter].start = &vertexes[i][j];
					E[edge_counter].end = &vertexes[i + 1][k];
					edge_counter++;
				}
			}
			else {
				E[edge_counter].unit_cost = unit_proudction_cost[i];
				E[edge_counter].hold_cost = unit_holding_cost[i];
				E[edge_counter].cost = ((demand - j)*E[edge_counter].unit_cost) + j*E[edge_counter].hold_cost;
				E[edge_counter].start = &vertexes[i][j];
				E[edge_counter].end = &vertexes[i + 1][0];
				edge_counter++;
			}
		}
	}
}

void ProductionModel::dynamic() {
	vertexes[number_of_cycles][0].cost_of_path = 0;
	Node v = vertexes[number_of_cycles][0];

	for (int j = 0; j <= warehouse_space; j++) {
		for (int i = 0; i < number_of_edges; i++) {
			if (E[i].end->id == v.id && E[i].start->id == vertexes[number_of_cycles - 1][j].id) {
				if (vertexes[number_of_cycles - 1][j].minCost > E[i].cost) {
					vertexes[number_of_cycles - 1][j].cost_of_path = E[i].cost;
					vertexes[number_of_cycles - 1][j].minCost = E[i].cost;
					vertexes[number_of_cycles - 1][j].parentId = v.id; 				
				}break;
			}
		}
	}
	int c = 0;
	for (int i = number_of_cycles - 1; i >= 2; i--) {
		for (int j = 0; j <= warehouse_space; j++) {
			for (int c = 0; c <= warehouse_space; c++) {
				for (int k = 0; k < number_of_edges; k++) {
					if (E[k].end->id == vertexes[i][c].id && E[k].start->id == vertexes[i - 1][j].id) {
						if (vertexes[i - 1][j].minCost > E[k].cost + E[k].end->cost_of_path) {
							vertexes[i - 1][j].cost_of_path = E[k].cost + E[k].end->cost_of_path;
							vertexes[i - 1][j].minCost = vertexes[i - 1][j].cost_of_path;
							vertexes[i - 1][j].parentId = E[k].end->id;
						}break;
					}
				}
			}
		}
	}
	for (int i = 0; i <= warehouse_space; i++) {
		for (int j = 0; j < number_of_edges; j++) {
			if (E[j].end->id == vertexes[1][i].id && E[j].start->id == vertexes[0][0].id) {
				if (vertexes[0][0].minCost > E[j].cost + E[j].end->cost_of_path) {
					vertexes[0][0].cost_of_path = E[j].cost + E[j].end->cost_of_path;
					vertexes[0][0].minCost = vertexes[0][0].cost_of_path;
					vertexes[0][0].parentId = E[j].end->id;
				}break;
			}
		}
	}
}

void ProductionModel::permutationCore(int n) { 
	int state;
	if (n == 0) {
		bruteForceRecursive(0, 0);
	}
	else {
		for (state = 0; state <= warehouse_space; ++state) {
			currentVertex[n - 1] = state;
			permutationCore(n - 1);
		}
	}
}

void ProductionModel::bruteForceRecursive(int x, int y) {
	for (int k = 0; k < number_of_edges; k++) {
		if (E[k].start->id == vertexes[x][y].id && E[k].end->id == vertexes[x + 1][currentVertex[x+1]].id) {
			tempCost += E[k].cost; 
			break;
		}
	}
	if (x < (number_of_cycles - 1)) {
		bruteForceRecursive(x + 1, currentVertex[x + 1]);
	}
	else {
		if (bestCost > tempCost) {
			bestCost = tempCost;
			tempCost = 0;
			for (int i = 0; i < number_of_cycles + 1; i++) {
				bestPath[i] = currentVertex[i];
			}
		}
		tempCost = 0;
	}
}

void ProductionModel::showResult() {
	std::cout << "Calkowity koszt: " << vertexes[0][0].cost_of_path << std::endl;
	std::cout << "Droga:" << std::endl;
	std::cout << "-------------------" << std::endl;
	Node *v = &vertexes[0][0];
	int index;
	do {
		for (int i = 0; i < number_of_edges; i++) {
			if (E[i].start->id == v->id && E[i].end->id == v->parentId) {
				std::cout << E[i].start->id << " -> " << E[i].end->id << " koszt przejscia stanow: " << E[i].cost << std::endl;
				index = i;
				break;
			}
		} v = E[index].end; 
	} while (v->parentId >= 0 && v->parentId <= lastId); }

