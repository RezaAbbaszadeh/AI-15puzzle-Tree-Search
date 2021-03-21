#pragma once

#include <string>
#include <iostream>
#include <queue>
#include <vector>
#include <iterator>
#include <stack>
#include <vector>
#include <unordered_set>
#include <map>
#include <set>

using namespace std;

class puzzle24 {

	static int manhatan(vector <int> state) {
		int man = 0;
		for (int i = 0; i < 25; i++) {
			int t = state[i];
			if (t == 0) {
				continue;
			}
			man += abs(i % 5 - (t - 1) % 5);// horizental
			man += abs(i / 5 - (t - 1) / 5);// vertical
		}
		return man;
	}


	static int LinearConflict(vector <int> state) {
		static int goal[25][2] = {
			{ 0,0 } ,{ 0,1 } ,{ 0,2 } ,{ 0,3 } , { 0,4 } ,
			{ 1,0 } ,{ 1,1 } ,{ 1,2 } ,{ 1,3 } , { 1,4 } ,
			{ 2,0 } ,{ 2,1 } ,{ 2,2 } ,{ 2,3 } , { 2,4 } ,
			{ 3,0 } ,{ 3,1 } ,{ 3,2 } ,{ 3,3 } , { 3,4 } ,
			{ 4,0 } ,{ 4,1 } ,{ 4,2 } ,{ 4,3 } , { 4,4 }
		};
		int conflicts = 0;
		int current_state[25][2];
		bool hasConflict[25][2] = { false }; // 0-> row conflicts       1-> col conflicts

		bool in_col[25] = {}; // set to true if this value is in the correct column
		bool in_row[25] = {}; // set to true if this value is in the correct row


		for (int i = 0; i < 25; i++)
		{
			int bx = (state[i] - 1) % 5;
			int by = (state[i] - 1) / 5;

			in_col[state[i]] = (bx == goal[i][1]);
			in_row[state[i]] = (by == goal[i][0]);

			current_state[state[i]][0] = i / 5;
			current_state[state[i]][1] = i % 5;
		}

		for (auto i = 0; i < 25; i++)
		{
			if (state[i] == 0) // ignore the empty space
				continue;

			if (in_col[state[i]])
			{
				for (auto j = i + 5; j < 25; j += 5) // check down for conflicts
				{
					if (state[j] == 0 || hasConflict[state[j]][1]) // ignore the empty space
						continue;

					if (in_col[state[j]] && state[j] / 5 < state[i] / 5) {
						if (!hasConflict[state[i]][1])
							++conflicts;
						hasConflict[state[i]][1] = true;
						hasConflict[state[j]][1] = true;
						//break;
					}
				}
			}

			if (in_row[state[i]])
			{
				for (auto j = i + 1; j < i / 5 * 5 + 5; j++) // check right for conflicts
				{
					if (state[j] == 0 || hasConflict[state[j]][0]) // ignore the empty space
						continue;

					if (in_row[state[j]] && (state[j] - 1) % 5 < (state[i] - 1) % 5) {
						if (!hasConflict[state[i]][0])
							++conflicts;
						hasConflict[state[i]][0] = true;
						hasConflict[state[j]][0] = true;
						//break;
					}
				}
			}
		}
		int man = manhatan(state);

		//last move
		if (!hasConflict[24][1] && !hasConflict[20][0] && current_state[24][1] != 4 && current_state[20][0] != 4 && man > 0) {
			conflicts++;
		}
		//last two move
		if (!hasConflict[23][1] && !hasConflict[15][0] && !hasConflict[19][0] && !hasConflict[19][1] &&
			current_state[23][1] < 3 && current_state[15][0] < 3 &&
			current_state[19][1] != 4 && current_state[19][0] != 4 && man>1) {
			conflicts++;
		}
		//corners
		if (in_col[2] && in_row[2] &&
			in_col[6] && in_row[6] &&
			state[0] != 1 &&
			!hasConflict[2][0] && !hasConflict[6][1]
			) {
			conflicts++;
		}
		if (in_col[4] && in_row[4] &&
			in_col[10] && in_row[10] &&
			state[4] != 5 &&
			!hasConflict[4][0] && !hasConflict[10][1]
			) {
			conflicts++;
		}
		if (in_col[16] && in_row[16] &&
			in_col[22] && in_row[22] &&
			state[20] != 21 &&
			!hasConflict[16][1] && !hasConflict[22][0]
			) {
			conflicts++;
		}



		return 2 * conflicts + man;
	}

	class Node {
	public:
		Node* parent;
		int zero;
		vector <int> state;
		long long int hash = 0;
		int depth = 0;
		int g = 0;
		int f = 0;

		operator std::string() const {
			string str = "";
			for (int i = 0; i < 25; i++) {
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
				g = parent->g + 1;
			}
			f = g + LinearConflict(state);
			make_hash();
		}

		Node() {
			parent = NULL;
		}

		void make_hash() {
			const int hashBase = 67;
			const long long int hashMode = 1e15 + 7;
			hash = 1;
			for (int i = 0; i < 25; i++) {
				hash *= hashBase;
				hash += state[i];
				hash %= hashMode;
			}
		}
	};

	string dfsSolution(Node node) {
		stack<string> stack;

		string result = "5";
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


		if (node->zero > 4) {//can go up
			swap(&tmp, node->zero, node->zero - 5);
			successors.push_back(Node(node, tmp, node->zero - 5));
			swap(&tmp, node->zero, node->zero - 5);
		}
		if (node->zero < 20) {//can go down
			swap(&tmp, node->zero, node->zero + 5);
			successors.push_back(Node(node, tmp, node->zero + 5));
			swap(&tmp, node->zero, node->zero + 5);
		}
		if (node->zero % 5 != 4) {//can go right
			swap(&tmp, node->zero, node->zero + 1);
			successors.push_back(Node(node, tmp, node->zero + 1));
			swap(&tmp, node->zero, node->zero + 1);
		}
		if (node->zero % 5 != 0) {//can go left
			swap(&tmp, node->zero, node->zero - 1);
			successors.push_back(Node(node, tmp, node->zero - 1));
			swap(&tmp, node->zero, node->zero - 1);
		}
		return successors;
	}



	bool dfsGoalTest(vector <int> state) {
		for (int i = 0; i < 25; i++) {
			if (state[i] != i + 1) {
				if (i == 24)
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
		//node_counter = 0;
		//unique_node_counter = 0;

		Node* node = new Node(NULL, initialNode.state, initialNode.zero);
		if (dfsGoalTest(node->state)) return dfsSolution(*node);
		frontier.push(node);
		while (1) {
			if (frontier.empty())
				return "failure";
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

		int init_zero;
		for (init_zero = 0; init_zero < 25; init_zero++) {
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