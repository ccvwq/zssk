#include "Header.h";
#include <fstream>
#include <string>
#include <climits>
#include <iostream>

ProductionModel::ProductionModel() {
	warehouse_space = 2;
	demand = 2;
}

void ProductionModel::getFromFile(std::string fname) {
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
	}
}

void ProductionModel::createGraph() {
	number_of_verticies = 2 + (warehouse_space + 1)*(number_of_cycles - 1);
	number_of_edges = (number_of_verticies - (warehouse_space + 1)) * (warehouse_space + 1);
	vertexes = new Node*[number_of_cycles + 1];
	for (int i = 0; i < number_of_cycles + 1; i++) {
		vertexes[i] = new Node[(warehouse_space + 1)];
	}
	idCounter = 0;
	for (int i = 0; i < number_of_cycles + 1; i++) {
		for (int j = 0; j < warehouse_space + 1; j++) {
			vertexes[i][j].id = idCounter;
			vertexes[i][j].minCost = 9999999;
			idCounter++;
		}
	}
	E = new Edge[number_of_edges];
	for (int i = 0; i <= warehouse_space; i++) {
		E[i].unit_cost = unit_proudction_cost[0];
		E[i].hold_cost = unit_holding_cost[0];
		E[i].cost = (demand + i)*E->unit_cost;
		E[i].start = vertexes[0][0];
		E[i].end = vertexes[1][i];
	}
	int edge_counter = warehouse_space + 1;
	for (int i = 1; i < number_of_cycles; i++) {
		for (int j = 0; j < (warehouse_space + 1); j++) { //vertexes[i][j];
			if (i != number_of_cycles - 1) {
				for (int k = 0; k <= warehouse_space; k++) {
					E[edge_counter].unit_cost = unit_proudction_cost[i];
					E[edge_counter].hold_cost = unit_holding_cost[i];
					E[edge_counter].cost = (((demand-j) + k)*E->unit_cost) + (j*E->hold_cost);
					E[edge_counter].start = vertexes[i][j];
					E[edge_counter].end = vertexes[i + 1][k];
					edge_counter++;
				}
			}
			else {
				E[edge_counter].unit_cost = unit_proudction_cost[i];
				E[edge_counter].hold_cost = unit_holding_cost[i];
				E[edge_counter].cost = (((demand - j)*E->hold_cost) + j*E->hold_cost);
				E[edge_counter].start = vertexes[i][j];
				E[edge_counter].end = vertexes[i + 1][0];
			}
		}
	}
}

void ProductionModel::bruteForce() {
	vertexes[number_of_cycles][0].cost_of_path = 0;
	Node v = vertexes[number_of_cycles][0];
	
	for (int j = 0; j <= warehouse_space; j++) {
		for (int i = 0; i < number_of_edges; i++) {
			if (E[i].end.id == v.id && E[i].start.id == vertexes[number_of_cycles - 1][j].id) {
				if (vertexes[number_of_cycles - 1][j].minCost > E[i].cost) {
					vertexes[number_of_cycles - 1][j].cost_of_path = E[i].cost;
					vertexes[number_of_cycles - 1][j].minCost = E[i].cost;
					vertexes[number_of_cycles - 1][j].parentId = v.id; // ????
				}
			}
		}
	}
	int c = 0;
	for (int i = number_of_cycles - 1; i >= 2; i--) {
		for(int j = 0; j <= warehouse_space; j++) {
			for (int k = 0; k < number_of_edges; k++) {
				if (E[i].end.id == vertexes[i][c].id && E[k].start.id == vertexes[i - 1][j].id) {
					if (vertexes[i - 1][j].minCost > E[k].cost + E[k].end.cost_of_path) {
						vertexes[i - 1][j].cost_of_path = E[k].cost + E[k].end.cost_of_path;
						vertexes[i - 1][j].minCost = vertexes[i - 1][j].cost_of_path;
						vertexes[i - 1][j].parentId = E[k].end.id;
					}
				}
			} c++;
		}
		c = 0;
	}
	for (int i = 0; i <= warehouse_space; i++) {
		for(int j = 0; j < number_of_edges; j++) {
			if (E[i].end.id == vertexes[1][i].id && E[i].end.id == vertexes[0][0].id) {
				if (vertexes[0][0].minCost > E[i].cost + E[i].end.cost_of_path) {
					vertexes[0][0].cost_of_path = E[i].cost + E[i].end.cost_of_path;
					vertexes[0][0].minCost = vertexes[0][0].cost_of_path;
					vertexes[0][0].parentId = E[i].end.id;
				}
			}
		}
	}

}

void ProductionModel::showResult() {
	std::cout << "Calkowity koszt: " << vertexes[0][0].cost_of_path << std::endl;
	std::cout << "Droga:" << std::endl;
	std::cout << "-------------------" << std::endl;
	for (int i = 0; i < number_of_edges; i++) {
		std::cout << i << ": " << E[i].cost << std::endl;
	}
	std::cout << "start: " << E[21].start.id << std::endl;
	std::cout << "end: " << E[21].end.id << std::endl;
	std::cout << "start: " << E[22].start.id << std::endl;
	std::cout << "end: " << E[22].end.id << std::endl;
	std::cout << "start: " << E[23].start.id << std::endl;
	std::cout << "end: " << E[23].end.id << std::endl;

}

