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

using namespace std;

class DFS {

	class Node {
	public:
		Node* parent;
		int zero;
		vector <int> state;
		long long int hash = 0;
		int depth = 0;

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
			make_hash();
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
		if (node->zero % 4 != 3) {//can go right
			swap(&tmp, node->zero, node->zero + 1);
			successors.push_back(Node(node, tmp, node->zero + 1));
			swap(&tmp, node->zero, node->zero + 1);
		}
		if (node->zero < 12) {//can go down
			swap(&tmp, node->zero, node->zero + 4);
			successors.push_back(Node(node, tmp, node->zero + 4));
			swap(&tmp, node->zero, node->zero + 4);
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
public:
	string Search(vector <int> initialState) {
		stack<Node*> frontier;
		parents_hash parents_h; //only to check if a node already created in parents
		parents_set parents;
		int depth = 0;
		int init_zero;
		for (init_zero = 0; init_zero < 16; init_zero++) {
			if (initialState[init_zero] == 0)
				break;
		}
		Node* node = new Node(NULL, initialState, init_zero);
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
			if (repeated != parents_h.end()) {//already in parents (Visited)
				delete node;
				continue;
			}
			else {
				unique_node_counter++;
			}
			if (dfsGoalTest(node->state)) {
				return dfsSolution(*node);
			}
			if (node->depth > 79) {
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
				//cout << node_counter << "    " << depth << endl;
			}
		}
	}
};