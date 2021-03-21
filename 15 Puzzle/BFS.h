#pragma once

#include <string>
#include <iostream>
#include <queue>
#include <vector>
#include <iterator>
#include <stack>
#include <vector>
#include <unordered_set>


using namespace std;
class BFS {


	class Node {
	public:
		Node* parent;
		int zero;
		vector <int> state;
		long long int hash = 0;

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

	string bfsSolution(Node node) {
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



	bool bfsGoalTest(vector <int> state) {
		for (int i = 0; i < 16; i++) {
			if (state[i] != i + 1) {
				if (i == 15)
					return true;
				else
					return false;
			}
		}
	}

	typedef unordered_set<long long int> visited_hash;

	public:
	string Search(vector <int> initialState) {
		queue<Node*> frontier;
		visited_hash visitList;
		int init_zero;
		for (init_zero = 0; init_zero < 16; init_zero++) {
			if (initialState[init_zero] == 0)
				break;
		}
		Node* node = new Node(NULL, initialState, init_zero);
		if (bfsGoalTest(node->state)) return bfsSolution(*node);
		frontier.push(node);
		visitList.insert(node->hash);
		while (1) {
			if (frontier.empty()) return "failure";
			node = frontier.front();
			frontier.pop();
			vector <Node> successors = Successor(node);
			for (int i = 0; i < successors.size(); i++) {
				Node* current = new Node(successors[i].parent, successors[i].state, successors[i].zero);

				visited_hash::const_iterator repeated;
				repeated = visitList.find(current->hash);

				node_counter++;
				//cout << node_counter << endl;
				if (repeated == visitList.end()) {
					unique_node_counter++;
					if (bfsGoalTest(current->state)) {
						return bfsSolution(*current);
					}
					else {
						frontier.push(current);
						visitList.insert(current->hash);
					}
				}
				else {
					delete current;
				}
			}
		}
	}



};