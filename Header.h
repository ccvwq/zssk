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
		Node start, end;
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

public:
	ProductionModel();
	void getFromFile(std::string);
	void simulateCycle(int index);
	void createGraph();
	void bruteForce();
	void showResult();

};

#endif