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

using namespace std;


class Disjoint {

	int node_counter = 0, unique_node_counter = 0;
	

	struct tuple {
		long long int hash;
		int cost;
	};

	tuple *db61, *db62, *db3;

	class Node {
	public:
		int zero;
		vector <int> state;
		int cost = 0;

		Node(vector <int> state, int zero, int cost) {
			this->state = state;
			this->zero = zero;
			this->cost = cost;
		}

		Node() {
		}
	};

private:
	long long int make_hash61(vector <int> state) {
		int x[16][2];
		for (int i = 0; i < 16; i++) {
			if (state[i] != -1) {
				x[state[i]][0] = i / 4 + 1;
				x[state[i]][1] = i % 4 + 1;
			}
		}
		return x[1][0] * 1e11 + x[1][1] * 1e10 +
			x[5][0] * 1e9 + x[5][1] * 1e8 +
			x[6][0] * 1e7 + x[6][1] * 1e6 +
			x[9][0] * 1e5 + x[9][1] * 1e4 +
			x[10][0] * 1e3 + x[10][1] * 1e2 +
			x[13][0] * 1e1 + x[13][1];
	}
	long long int make_hash62(vector <int> state) {
		int x[16][2];
		for (int i = 0; i < 16; i++) {
			if (state[i] != -1) {
				x[state[i]][0] = i / 4 + 1;
				x[state[i]][1] = i % 4 + 1;
			}
		}
		return x[7][0] * 1e11 + x[7][1] * 1e10 +
			x[8][0] * 1e9 + x[8][1] * 1e8 +
			x[11][0] * 1e7 + x[11][1] * 1e6 +
			x[12][0] * 1e5 + x[12][1] * 1e4 +
			x[14][0] * 1e3 + x[14][1] * 1e2 +
			x[15][0] * 1e1 + x[15][1];
	}
	long long int make_hash3(vector <int> state) {
		int x[16][2];
		for (int i = 0; i < 16; i++) {
			if (state[i] != -1) {
				x[state[i]][0] = i / 4 + 1;
				x[state[i]][1] = i % 4 + 1;
			}
		}
		return x[2][0] * 1e5 + x[2][1] * 1e4 +
			x[3][0] * 1e3 + x[3][1] * 1e2 +
			x[4][0] * 1e1 + x[4][1];
	}


	void swap(vector <int> *v, int i, int j) {
		int tmp = (*v)[i];
		(*v)[i] = (*v)[j];
		(*v)[j] = tmp;
	}

	vector <Node> Successor(Node node) {
		vector <Node> successors;
		vector <int> tmp = node.state;

		if (node.zero % 4 != 0) {//can go left
			swap(&tmp, node.zero, node.zero - 1);
			successors.push_back(Node(tmp, node.zero - 1, node.cost + 1 * (tmp[node.zero] != -1)));
			swap(&tmp, node.zero, node.zero - 1);
		}
		if (node.zero % 4 != 3) {//can go right
			swap(&tmp, node.zero, node.zero + 1);
			successors.push_back(Node(tmp, node.zero + 1, node.cost + 1 * (tmp[node.zero] != -1)));
			swap(&tmp, node.zero, node.zero + 1);
		}
		if (node.zero > 3) {//can go up
			swap(&tmp, node.zero, node.zero - 4);
			successors.push_back(Node(tmp, node.zero - 4, node.cost + 1 * (tmp[node.zero] != -1)));
			swap(&tmp, node.zero, node.zero - 4);
		}
		if (node.zero < 12) {//can go down
			swap(&tmp, node.zero, node.zero + 4);
			successors.push_back(Node(tmp, node.zero + 4, node.cost + 1 * (tmp[node.zero] != -1)));
			swap(&tmp, node.zero, node.zero + 4);
		}
		return successors;
	}


	struct zero_cost {
		vector <bool> z; // to save 0 was in which tiles
		int cost;

		zero_cost(vector <bool> v, int c) {
			cost = c;
			z = v;
		}
	};


	typedef map<long long int, zero_cost> DB;

	void CreateDisjoint61() {
		queue<Node> frontier;
		DB db;
		Node current;
		vector <bool> zeros(16);
		int cost = 0;
		Node node = Node({ 1,-1,-1,-1,5,6,-1,-1,9,10,-1,-1,13,-1,-1,0 }, 15, 0);
		frontier.push(node);
		db.insert(DB::value_type(make_hash61(node.state), zero_cost(zeros, cost)));
		while (1) {
			if (frontier.empty()) {
				cout << "FINISHED";
				FILE* pFile;
				fopen_s(&pFile, "15691013.bin", "wb");
				long long int hash;
				int cost;
				for (DB::iterator r = db.begin(); r != db.end(); r++) {
					hash = r->first;
					cost = r->second.cost;
					fwrite(&hash, 1, sizeof(long long int), pFile);
					fwrite(&cost, 1, sizeof(int), pFile);
				}
				fclose(pFile);
				db.clear();
				return ;
			}

			node = frontier.front();
			frontier.pop();
			vector <Node> successors = Successor(node);
			for (int i = 0; i < successors.size(); i++) {
				current = Node(successors[i].state, successors[i].zero, successors[i].cost);

				DB::const_iterator repeated;
				long long int hash = make_hash61(current.state);
				repeated = db.find(hash);

				node_counter++;

				for (int i = 0; i < 16; i++) {
					zeros[i] = false;
				}

				if (repeated == db.end()) {//not visited 
					frontier.push(current);
					db.insert(DB::value_type(hash, zero_cost(zeros, current.cost)));
				}
				else { //visited 
					zeros = repeated->second.z;
					cost = repeated->second.cost;

					if (!zeros[current.zero]) {// not visited (ignoring 0)
						unique_node_counter++;
						zeros[current.zero] = true;
						frontier.push(current);
						if (repeated->second.cost > current.cost) {
							db.erase(repeated);
							db.insert(DB::value_type(hash, zero_cost(zeros, current.cost)));
						}
						else {
							db.erase(repeated);
							db.insert(DB::value_type(hash, zero_cost(zeros, cost)));
						}
					}

				}
			}
		}

		return;
	}


	void CreateDisjoint62() {
		queue<Node> frontier;
		DB db;
		Node current;
		vector <bool> zeros(16);
		int cost = 0;
		Node node = Node({ -1,-1,-1,-1,-1,-1,7,8,-1,-1,11,12,-1,14,15,0 }, 15, 0);
		frontier.push(node);
		db.insert(DB::value_type(make_hash62(node.state), zero_cost(zeros, cost)));
		while (1) {
			if (frontier.empty()) {
				cout << "FINISHED";
				FILE* pFile;
				fopen_s(&pFile, "7811121415.bin", "wb");
				long long int hash;
				int cost;
				for (DB::iterator r = db.begin(); r != db.end(); r++) {
					hash = r->first;
					cost = r->second.cost;
					fwrite(&hash, 1, sizeof(long long int), pFile);
					fwrite(&cost, 1, sizeof(int), pFile);
				}
				fclose(pFile);
				db.clear();
				return;
			}

			node = frontier.front();
			frontier.pop();
			vector <Node> successors = Successor(node);
			for (int i = 0; i < successors.size(); i++) {
				current = Node(successors[i].state, successors[i].zero, successors[i].cost);

				DB::const_iterator repeated;
				long long int hash = make_hash62(current.state);
				repeated = db.find(hash);

				node_counter++;

				for (int i = 0; i < 16; i++) {
					zeros[i] = false;
				}

				if (repeated == db.end()) {//not visited 
					frontier.push(current);
					db.insert(DB::value_type(hash, zero_cost(zeros, current.cost)));
				}
				else { //visited 
					zeros = repeated->second.z;
					cost = repeated->second.cost;

					if (!zeros[current.zero]) {// not visited (ignoring 0)
						unique_node_counter++;
						zeros[current.zero] = true;
						frontier.push(current);
						if (repeated->second.cost > current.cost) {
							db.erase(repeated);
							db.insert(DB::value_type(hash, zero_cost(zeros, current.cost)));
						}
						else {
							db.erase(repeated);
							db.insert(DB::value_type(hash, zero_cost(zeros, cost)));
						}
					}

				}
			}
		}

		return;
	}


	void CreateDisjoint3() {
		queue<Node> frontier;
		DB db;
		Node current;
		vector <bool> zeros(16);
		int cost = 0;
		Node node = Node({ -1,2,3,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 }, 15, 0);
		frontier.push(node);
		db.insert(DB::value_type(make_hash3(node.state), zero_cost(zeros, cost)));
		while (1) {
			if (frontier.empty()) {
				cout << "FINISHED";
				FILE* pFile;
				fopen_s(&pFile, "234.bin", "wb");
				long long int hash;
				int cost;
				for (DB::iterator r = db.begin(); r != db.end(); r++) {
					hash = r->first;
					cost = r->second.cost;
					fwrite(&hash, 1, sizeof(long long int), pFile);
					fwrite(&cost, 1, sizeof(int), pFile);
				}
				fclose(pFile);
				db.clear();
				return;
			}

			node = frontier.front();
			frontier.pop();
			vector <Node> successors = Successor(node);
			for (int i = 0; i < successors.size(); i++) {
				current = Node(successors[i].state, successors[i].zero, successors[i].cost);

				DB::const_iterator repeated;
				long long int hash = make_hash3(current.state);
				repeated = db.find(hash);

				node_counter++;

				for (int i = 0; i < 16; i++) {
					zeros[i] = false;
				}

				if (repeated == db.end()) {//not visited 
					frontier.push(current);
					db.insert(DB::value_type(hash, zero_cost(zeros, current.cost)));
				}
				else { //visited 
					zeros = repeated->second.z;
					cost = repeated->second.cost;

					if (!zeros[current.zero]) {// not visited (ignoring 0)
						unique_node_counter++;
						zeros[current.zero] = true;
						frontier.push(current);
						if (repeated->second.cost > current.cost) {
							db.erase(repeated);
							db.insert(DB::value_type(hash, zero_cost(zeros, current.cost)));
						}
						else {
							db.erase(repeated);
							db.insert(DB::value_type(hash, zero_cost(zeros, cost)));
						}
					}

				}
			}
		}

		return;
	}

public:
	void CreateDisjoint() {

		CreateDisjoint61();
		CreateDisjoint62();
		CreateDisjoint3();
	}


	void LoadDatabase() {
		long long int hash;
		int cost;

		db61 = new tuple[5765760];
		db62 = new tuple[5765760];
		db3 = new tuple[3360];

		FILE* pFile;
		fopen_s(&pFile, "15691013.bin", "rb");
		for (int i = 0; i < 5765760; i++) {
			fread(&hash, sizeof(long long int), 1, pFile);
			fread(&cost, sizeof(int), 1, pFile);
			db61[i].hash = hash;
			db61[i].cost = cost;
		}
		fclose(pFile); 

		fopen_s(&pFile, "7811121415.bin", "rb");
		for (int i = 0; i < 5765760; i++) {
			fread(&hash, sizeof(long long int), 1, pFile);
			fread(&cost, sizeof(int), 1, pFile);
			db62[i].hash = hash;
			db62[i].cost = cost;
		}
		fclose(pFile);

		fopen_s(&pFile, "234.bin", "rb");
		for (int i = 0; i < 3360; i++) {
			fread(&hash, sizeof(long long int), 1, pFile);
			fread(&cost, sizeof(int), 1, pFile);
			db3[i].hash = hash;
			db3[i].cost = cost;
		}
		fclose(pFile);

	}

	int binarySearch(tuple arr[], int l, int r, long long int x)
	{
		
		int mid = l + (r - l) / 2;

		if (arr[mid].hash == x)
			return arr[mid].cost;

		if (arr[mid].hash > x)
			return binarySearch(arr, l, mid - 1, x);

		return binarySearch(arr, mid + 1, r, x);
		
	}

	int getH(vector <int> state) {
		vector <int> state61(16), state62(16), state3(16);
		for (int i = 0; i < 16; i++) {
			if (state[i] == 0) {
				state61[i] = 0;
				state62[i] = 0;
				state3[i] = 0;
			}
			else if (state[i] == 1) {
				state61[i] = 1;
				state62[i] = -1;
				state3[i] = -1;
			}
			else if (state[i] == 2) {
				state61[i] = -1;
				state62[i] = -1;
				state3[i] = 2;
			}
			else if (state[i] == 3) {
				state61[i] = -1;
				state62[i] = -1;
				state3[i] = 3;
			}
			else if (state[i] == 4) {
				state61[i] = -1;
				state62[i] = -1;
				state3[i] = 4;
			}
			else if (state[i] == 5) {
				state61[i] = 5;
				state62[i] = -1;
				state3[i] = -1;
			}
			else if (state[i] == 6) {
				state61[i] = 6;
				state62[i] = -1;
				state3[i] = -1;
			}
			else if (state[i] == 7) {
				state61[i] = -1;
				state62[i] = 7;
				state3[i] = -1;
			}
			else if (state[i] == 8) {
				state61[i] = -1;
				state62[i] = 8;
				state3[i] = -1;
			}
			else if (state[i] == 9) {
				state61[i] = 9;
				state62[i] = -1;
				state3[i] = -1;
			}
			else if (state[i] == 10) {
				state61[i] = 10;
				state62[i] = -1;
				state3[i] = -1;
			}
			else if (state[i] == 11) {
				state61[i] = -1;
				state62[i] = 11;
				state3[i] = -1;
			}
			else if (state[i] == 12) {
				state61[i] = -1;
				state62[i] = 12;
				state3[i] = -1;
			}
			else if (state[i] == 13) {
				state61[i] = 13;
				state62[i] = -1;
				state3[i] = -1;
			}
			else if (state[i] == 14) {
				state61[i] = -1;
				state62[i] = 14;
				state3[i] = -1;
			}
			else if (state[i] == 15) {
				state61[i] = -1;
				state62[i] = 15;
				state3[i] = -1;
			}

		}

		long long int hash61, hash62, hash3;
		hash61 = make_hash61(state61);
		hash62 = make_hash62(state62);
		hash3 = make_hash3(state3);

		int cost61 = binarySearch(db61, 0, 5765759, hash61);
		int cost62 = binarySearch(db62, 0, 5765759, hash62);
		int cost3 = binarySearch(db3, 0, 3359, hash3);
		return  cost61 + cost62 + cost3;;
	}

};