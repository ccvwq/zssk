#pragma once
#ifndef ELEMENT_LISTY_H
#define ELEMENT_LISTY_H

class ElementListy {
public:
	int in_warehouse;
	int sum_of_expanses;
	int sum_of_holding_cost;
	int sum_of_production_cost;
	ElementListy *next;
};

#endif
