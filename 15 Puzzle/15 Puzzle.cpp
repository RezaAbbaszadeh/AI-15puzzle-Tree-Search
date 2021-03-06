// 15 Puzzle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

#include "Disjoint.h"
int heuristic;
Disjoint disjoint;
int node_counter = 0, unique_node_counter = 0;

#include "BFS.h"
#include "UCS.h"
#include "DFS.h"
#include "Astar.h"
#include "IDAstar.h"
#include "BDS.h"
#include "24Puzzle.h"


void writeToFile(string x) {
	ofstream myfile;
	myfile.open("solution.txt");
	myfile << x;
	myfile.close();
}

int main()
{
	BFS bfs; DFS dfs; UCS ucs; Astar astar; IDAstar idastar; BDS bds; puzzle24 puzzle24;
	int size , size2;
	int search;

	cout << "Size: ";
	cin >> size;
	if (size == 4) {
		cout << "Search? BFS->1 DFS->2 UCS->3 A*->4 IDA*->5 BDS->6\n";
		cin >> search;
	}

	if (search == 4 || search == 5) {
		cout << "Heuristic? Manhatan->1 Linear->2 Disjoint->3\n";
		cin >> heuristic;
	}
	size2 = size*size;
	vector <int> initialState(size2);

	cout << "initial state: \n";
	for (int i = 0; i < size2; i++) {
		cin >> initialState[i];
	}

	clock_t start, end;
	double cpu_time_used;

	start = clock();

	string result;
	if (search == 1) {
		result = bfs.Search(initialState);
	}
	else if (search == 2) {
		result = dfs.Search(initialState);
	}
	else if (search == 3) {
		result = ucs.Search(initialState);
	}
	else if (search == 4) {
		result = astar.Search(initialState);
	}
	else if (search == 5) {
		result = idastar.Search(initialState);
	}
	else if (search == 6) {
		result = bds.Search(initialState);
	}
	else {
		result = puzzle24.Search(initialState);
	}

	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	result += "\ntime: " + to_string(cpu_time_used) + "\nnodes created: " + to_string(node_counter) + "\nunique nodes created: " + to_string(unique_node_counter);
	writeToFile(result);


	//Disjoint dis;
	//dis.CreateDisjoint();

    return 0;
}


