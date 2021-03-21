#pragma once
#include <string>
#include <iostream>
#include <iterator>
#include <stack>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <math.h>
#include "Heuristic.h"
#include "Disjoint.h"
using namespace std;

class Astar {
	
	class Node {
	public:
		long long int hash = 0;
		Node* parent;
		int zero;
		vector <int> state;
		int g = 0;
		int f = 0;

		operator std::string() const {
			string str = "";
			for (int i = 0; i < 16; i++) {
				str += to_string(state[i]);
				str += ", ";
			}
			return str.substr(0, str.length() - 2);
		}

		Node(Node* parent, vector <int> state, int zero) {
			this->parent = parent;
			this->state = state;
			this->zero = zero;
			if (parent) {
				g = parent->g + 1;
			}
			f = g + Heuristic(state);
			make_hash();
		}

		Node() {
			parent = NULL;
		}

		int Heuristic(vector <int> state) {
			if (heuristic == 1) {
				return manhatan(state);
			}
			if (heuristic == 2) {
				return LinearConflict(state);
			}
			if (heuristic == 3) {
				return disjoint.getH(state);
			}
		}

		void make_hash() {
			const int hashBase = 67;
			const long long int hashMode = 1e12 + 7;
			hash = 1;
			for (int i = 0; i < 16; i++) {
				hash *= hashBase;
				hash += state[i];
				hash %= hashMode;
			}
		}

	};


	string AstarSolution(Node *node) {
		stack<string> stack;

		string result = "4";
		stack.push(*node);
		while (node->parent != NULL) {
			node = node->parent;
			stack.push(*node);
		}
		while (!stack.empty()) {
			string x = stack.top();
			stack.pop();
			result += "\n";
			result += x;
		}
		return result;
	}


	void swap(vector <int> *v, int i, int j) {
		int tmp = (*v)[i];
		(*v)[i] = (*v)[j];
		(*v)[j] = tmp;
	}

	vector <Node> Successor(Node* node) {
		vector <Node> successors;
		vector <int> tmp = node->state;

		if (node->zero % 4 != 0) {//can go left
			swap(&tmp, node->zero, node->zero - 1);
			successors.push_back(Node(node, tmp, node->zero - 1));
			swap(&tmp, node->zero, node->zero - 1);
		}
		if (node->zero % 4 != 3) {//can go right
			swap(&tmp, node->zero, node->zero + 1);
			successors.push_back(Node(node, tmp, node->zero + 1));
			swap(&tmp, node->zero, node->zero + 1);
		}
		if (node->zero > 3) {//can go up
			swap(&tmp, node->zero, node->zero - 4);
			successors.push_back(Node(node, tmp, node->zero - 4));
			swap(&tmp, node->zero, node->zero - 4);
		}
		if (node->zero < 12) {//can go down
			swap(&tmp, node->zero, node->zero + 4);
			successors.push_back(Node(node, tmp, node->zero + 4));
			swap(&tmp, node->zero, node->zero + 4);
		}
		return successors;
	}



	bool AstarGoalTest(vector <int> state) {
		for (int i = 0; i < 16; i++) {
			if (state[i] != i + 1) {
				if (i == 15)
					return true;
				else
					return false;
			}
		}
	}


	struct NodeComparator {
		bool operator()(const Node* a, const Node*b) const {
			if (a->hash == b->hash) {
				return false;
			}
			else if (a->f == b->f) {
				return (a->hash < b->hash);
			}
			else
				return (a->f < b->f);
		}
	};


	typedef unordered_map<long long int, Node*> frontier_hashmap;// this finds a node in frontier in O(1)
	typedef set<Node*, NodeComparator> frontier_set;// this is ordered by g()
	typedef unordered_set<long long int> visited_hashmap;


	public:
	string Search(vector <int> initialState) {
		if (heuristic == 3)
			disjoint.LoadDatabase();
		frontier_set frontier;
		frontier_hashmap frontier_hash;
		visited_hashmap visitList;

		int init_zero;
		for (init_zero = 0; init_zero < 16; init_zero++) {
			if (initialState[init_zero] == 0)
				break;
		}
		Node* node = new Node(NULL, initialState, init_zero);
		frontier.insert(node);
		frontier_hash.insert(frontier_hashmap::value_type(node->hash, node));

		while (1) {
			if (frontier.empty()) return "failure";
			node = *(frontier.begin());
			frontier.erase(frontier.begin());
			frontier_hash.erase(node->hash);
			if (AstarGoalTest(node->state)) {
				return AstarSolution(node);
			}
			visitList.insert(node->hash);
			vector <Node> successors = Successor(node);
			for (int i = 0; i < successors.size(); i++) {
				Node* current = new Node(successors[i].parent, successors[i].state, successors[i].zero);

				visited_hashmap::iterator visited_before = visitList.find(current->hash);

				node_counter++;
				//cout << node_counter << endl;

				if (visited_before == visitList.end()) {// not found in visitList
					frontier_hashmap::iterator in_frontier = frontier_hash.find(current->hash);
					if (in_frontier == frontier_hash.end()) {// not found in frontier
						frontier.insert(current);
						frontier_hash.insert(frontier_hashmap::value_type(current->hash, current));
						unique_node_counter++;
					}
					else {//founded in frontier
						Node* t = in_frontier->second;
						frontier_set::iterator c = frontier.find(t);
						if (current->g < t->g) {// found in frontier with higher cost
							delete *c;
							frontier.erase(c);
							frontier.insert(current);
							frontier_hash.erase(in_frontier);
							frontier_hash.insert(frontier_hashmap::value_type(current->hash, current));
						}
					}

				}
				else {
					delete current;
				}
			}
		}
	}

};