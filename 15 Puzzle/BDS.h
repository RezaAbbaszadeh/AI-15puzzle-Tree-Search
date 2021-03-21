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

class BDS {

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
			make_hash();
			if (parent)
				depth = parent->depth + 1;
		}

		Node() {
			parent = NULL;
		}

		void make_hash() {
			const int hashBase = 67;
			const long long int hashMode = 1e15 + 7;
			hash = 1;
			for (int i = 0; i < 16; i++) {
				hash *= hashBase;
				hash += state[i];
				hash %= hashMode;
			}
		}
	};

	string bfsSolution(Node init, Node goal) {
		stack<string> stack;

		string result = "4";
		stack.push(init);
		while (init.parent != NULL) {
			init = (*init.parent);
			stack.push(init);
		}
		while (!stack.empty()) {
			string x = stack.top();
			stack.pop();
			result += "\n";
			result += x;
		}
		while (goal.parent) {
			result += "\n";
			result += *goal.parent;
			goal = *goal.parent;
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



	Node* bdsGoalTest(long long int hash, queue <Node*> f) {
		while (!f.empty()) {
			if (hash == f.front()->hash) {
				return f.front();
			}
			f.pop();
		}
		return NULL;
	}

	typedef unordered_set<long long int> visited_hash;
public:
	string Search(vector <int> initialState) {
		queue<Node*> frontier_init;
		visited_hash visitList_init;
		visited_hash frontier_hash_init;//icrease search in frontier
		queue<Node*> frontier_goal;
		visited_hash visitList_goal;
		visited_hash frontier_hash_goal;
		int init_zero;
		for (init_zero = 0; init_zero < 16; init_zero++) {
			if (initialState[init_zero] == 0)
				break;
		}
		Node* node_init = new Node(NULL, initialState, init_zero);
		frontier_init.push(node_init);
		visitList_init.insert(node_init->hash);
		frontier_hash_init.insert(node_init->hash);

		vector <int> goal_state = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };
		Node* node_goal = new Node(NULL, goal_state, 15);
		frontier_goal.push(node_goal);
		visitList_goal.insert(node_goal->hash);

		if (node_init->hash == node_goal->hash) return bfsSolution(*node_init, *node_goal);

		int init_depth = 0, goal_depth = 0;

		while (1) {

			while (1) {
				if (frontier_init.empty()) return "failure";
				node_init = frontier_init.front();
				if (node_init->depth > init_depth)
					break;
				frontier_init.pop();
				frontier_hash_init.erase(node_init->hash);
				vector <Node> successors = Successor(node_init);
				for (int i = 0; i < successors.size(); i++) {
					Node* current = new Node(successors[i].parent, successors[i].state, successors[i].zero);

					visited_hash::const_iterator repeated;
					repeated = visitList_init.find(current->hash);

					node_counter++;
					//cout << node_counter << endl;
					if (repeated == visitList_init.end()) {
						unique_node_counter++;
						visited_hash::iterator x = frontier_hash_goal.find(current->hash);
						if (x!=frontier_hash_goal.end()) {
							Node* r = bdsGoalTest(current->hash, frontier_goal);
							return bfsSolution(*current, *r);
						}
						else {
							frontier_init.push(current);
							visitList_init.insert(current->hash);
							frontier_hash_init.insert(current->hash);
						}
					}
					else {
						delete current;
					}
				}
			}
			init_depth++;


			while (1) {
				if (frontier_goal.empty()) return "failure";
				node_goal = frontier_goal.front();
				if (node_goal->depth > goal_depth)
					break;
				frontier_goal.pop();
				frontier_hash_goal.erase(node_goal->hash);
				vector <Node> successors = Successor(node_goal);
				for (int i = 0; i < successors.size(); i++) {
					Node* current = new Node(successors[i].parent, successors[i].state, successors[i].zero);

					visited_hash::const_iterator repeated;
					repeated = visitList_goal.find(current->hash);

					node_counter++;
					//cout << node_counter << endl;
					if (repeated == visitList_goal.end()) {
						unique_node_counter++;
						visited_hash::iterator x = frontier_hash_init.find(current->hash);
						if (x!=frontier_hash_init.end()) {
							Node* r = bdsGoalTest(current->hash, frontier_init);
							return bfsSolution(*r, *current);
						}
						else {
							frontier_goal.push(current);
							visitList_goal.insert(current->hash);
							frontier_hash_goal.insert(current->hash);
						}
					}
					else {
						delete current;
					}
				}
			}
			goal_depth++;

		}
	}
};