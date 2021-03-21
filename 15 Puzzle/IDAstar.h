#pragma once

#include <string>
#include <iostream>
#include <queue>
#include <vector>
#include <iterator>
#include <stack>
#include <vector>
#include <unordered_set>
#include <set>
#include "Heuristic.h"
#include "Disjoint.h"

using namespace std;

	//int heuristic;
	//Disjoint disjoint;
class IDAstar {

	class Node {
		friend class IDAstar;
		Node* parent;
		int zero;
		vector <int> state;
		long long int hash = 0;
		int depth = 0;
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
				this->depth = parent->depth + 1;
			}
			if (parent) {
				g = parent->g + 1;
			}
			f = g + Heuristic(state);
			make_hash();
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

		Node() {
			parent = NULL;
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



	string dfsSolution(Node node) {
		stack<string> stack;

		string result = "4";
		stack.push(node);
		while (node.parent != NULL) {
			node = (*node.parent);
			stack.push(node);
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
		if (node->zero % 4 != 3) {//can go right
			swap(&tmp, node->zero, node->zero + 1);
			successors.push_back(Node(node, tmp, node->zero + 1));
			swap(&tmp, node->zero, node->zero + 1);
		}
		if (node->zero % 4 != 0) {//can go left
			swap(&tmp, node->zero, node->zero - 1);
			successors.push_back(Node(node, tmp, node->zero - 1));
			swap(&tmp, node->zero, node->zero - 1);
		}
		return successors;
	}



	bool dfsGoalTest(vector <int> state) {
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
			else if (a->depth == b->depth) {
				return (a->hash < b->hash);
			}
			else
				return (a->depth < b->depth);
		}
	};

	typedef unordered_set<long long int> parents_hash;
	typedef set<Node*, NodeComparator> parents_set;

	string dfs(Node initialNode, int &min_leaf_f) {
		stack<Node*> frontier;
		parents_hash parents_h; //only to check if a node already created in parents
		parents_set parents; //set of parents ordered by depth
		int depth = 0;
		int cutoff = min_leaf_f;
		min_leaf_f = INT_MAX;

		Node* node = new Node(NULL, initialNode.state, initialNode.zero);
		if (dfsGoalTest(node->state)) return dfsSolution(*node);
		frontier.push(node);
		while (1) {
			if (frontier.empty()) return "failure";
			node = frontier.top();
			frontier.pop();
			if (node->depth < depth) {//returned from leaf so update parents set
				parents_set::iterator x = --parents.end();
				while ((*x)->depth >= node->depth) {
					parents_h.erase((*x)->hash);
					delete *x;
					parents.erase(x);
					x = --parents.end();
				}
			}
			depth = node->depth;
			parents_hash::const_iterator repeated;
			repeated = parents_h.find(node->hash);
			if (repeated != parents_h.end()) {
				delete node;
				continue;
			}
			else {
				unique_node_counter++;
			}
			if (dfsGoalTest(node->state)) {
				return dfsSolution(*node);
			}
			if (node->f > cutoff) {
				min_leaf_f = min(min_leaf_f, node->f);
				delete node;
				continue;
			}
			vector <Node> successors = Successor(node);
			parents_h.insert(node->hash);
			parents.insert(node);
			for (int i = 0; i < successors.size(); i++) {
				Node* current = new Node(successors[i].parent, successors[i].state, successors[i].zero);
				frontier.push(current);

				node_counter++;
				//cout << node_counter << "    " << depth << "      " << cutoff << endl;
			}
		}
	}
public:
	string Search(vector <int> initialState) {
		if (heuristic == 3)
			disjoint.LoadDatabase();

		int init_zero;
		for (init_zero = 0; init_zero < 16; init_zero++) {
			if (initialState[init_zero] == 0)
				break;
		}
		Node* node = new Node(NULL, initialState, init_zero);
		string result = "failure";
		int min = node->f;
		int counter = 0;
		string x = "\n";
		while (result == "failure") {
			counter++;
			x += to_string(min) + ", ";
			cout << x;
			result = dfs(*node, min);
		}
		return result + x;
	}

};