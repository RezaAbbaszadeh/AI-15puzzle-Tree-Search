#pragma once
#include <vector>
#include <math.h>

using namespace std;

int manhatan(vector <int> state) {
	int man = 0;
	for (int i = 0; i < 16; i++) {
		int t = state[i];
		if (t == 0) {
			continue;
		}
		man += abs(i % 4 - (t - 1) % 4);// horizental
		man += abs(i / 4 - (t - 1) / 4);// vertical
	}
	return man;
}

int goal[16][2] ={ 
					{ 0,0 } ,{ 0,1 } ,{ 0,2 } ,{ 0,3 } ,
					{ 1,0 } ,{ 1,1 } ,{ 1,2 } ,{ 1,3 } ,
					{ 2,0 } ,{ 2,1 } ,{ 2,2 } ,{ 2,3 } ,
					{ 3,0 } ,{ 3,1 } ,{ 3,2 } ,{ 3,3 } , 
};

int LinearConflict(vector <int> state) {
	int conflicts = 0;

	bool in_col[16] = {}; // set to true if this value is in the correct column
	bool in_row[16] = {}; // set to true if this value is in the correct row
	bool hasConflict[25][2] = { false }; // 0-> row conflicts       1-> col conflicts
		
	for (int i = 0; i < 16; i++)
	{
		auto bx = (state[i]-1) % 4;
		auto by = (state[i]-1) / 4;

		in_col[state[i]] = (bx == goal[i][1]);
		in_row[state[i]] = (by == goal[i][0]);
	}

	for (auto i = 0; i < 16; i++)
	{
		if (state[i] == 0) // ignore the empty space
			continue;


		if (in_col[state[i]])
		{
			for (auto j = i+4; j < 16; j+=4) // check down for conflicts
			{
				if (state[j] == 0 || hasConflict[state[j]][1]) // ignore the empty space
					continue;

				if (in_col[state[j]] && state[j] / 4 < state[i] / 4) {
					if (!hasConflict[state[i]][1])
						++conflicts;
					hasConflict[state[i]][1] = true;
					hasConflict[state[j]][1] = true;
				}
			}
		}

		if (in_row[state[i]])
		{
			for (auto j = i+1; j < i/4*4 +4 ; j++) // check right for conflicts
			{
				if (state[j] == 0 || hasConflict[state[j]][0]) // ignore the empty space
					continue;

				if (in_row[state[j]] && (state[j] - 1) % 4 < (state[i] - 1) % 4) {
					if (!hasConflict[state[i]][0])
						++conflicts;
					hasConflict[state[i]][0] = true;
					hasConflict[state[j]][0] = true;
				}
			}
		}
			
		
	}

	return 2 * conflicts + manhatan(state);
}




