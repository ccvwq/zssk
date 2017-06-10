#pragma once
#ifndef HEADER_H
#define HEADER_H
#include <string>

class ProductionModel  { 
private:
	struct Edge;
	struct Node {
		int warehouse_state;
		int cost_of_path;
		int id;
		int minCost;
		int parentId;
	};

	struct Edge{
		int cost, unit_cost, hold_cost;
		Node* start;
		Node* end;
	};
	
	int warehouse_space;
	int demand;
	int *unit_proudction_cost;
	int *unit_holding_cost;
	int number_of_cycles;
	Node **vertexes;
	Edge *E;
	int number_of_verticies;
	int number_of_edges;
	int idCounter;
	int tempCost;
	int *currentVertex;
	int bestCost;
	int *bestPath;
	int lastId;

public:
	ProductionModel();
	ProductionModel(int warehouse_space, int demand);
	bool getFromFile(std::string);
	void createGraph();
	void dynamic();
	void showResult();
	void bruteForceRecursive(int x, int y);
	void permutationCore(int pos);
	int getNumberOfCycles();
	int getbestCost();
	int* getBestPath();
	void fillAuto(int cycles);
};

#endif