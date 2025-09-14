#include<iostream>
#include<random>
#include<vector>

enum DirTypes {n, s, w, e, nw, ne, sw, se, dir_counts};

using namespace std;

const unsigned dirs = dir_counts;
const unsigned char mx = 100, my = 100;
//const unsigned int mass = mx * my * 0.9;
const unsigned int mass = 10;

unsigned int finmass = 0;//finished ones

float orb[mx][my] = {0};

void clear_stay(unsigned int, unsigned int);
void search_uncover(int);

class ant {
public:
//private:
	unsigned int pos[2];//moving location
	unsigned int origin[2];//origin
	char face;
	bool fined;
	bool not_moved;
	unsigned int left[2];
//public:
	static void settle(mt19937& gen);
	static void turn(mt19937& grn);
	static void moving(unsigned int MoveWhich, unsigned int destx, unsigned int desty);
};
ant p[mass];

int main() {
	random_device rd;
	mt19937 gen(rd());
	ant::settle(gen);
	long long turns_used = 0;
	while (finmass < mass) {
		ant::turn(gen);
		turns_used++;
	}
	cout << "turns used: " << turns_used << "\n";
	return 0;
}

void ant::settle(mt19937& gen) {
	uniform_int_distribution<int> x_set(0, mx - 1);
	uniform_int_distribution<int> y_set(0, my - 1);
	for (int i = 0; i < mass; i++) {
		p[i].origin[0] = static_cast<unsigned int>(x_set(gen));
		p[i].origin[1] = static_cast<unsigned int>(y_set(gen));
		p[i].pos[0] = p[i].origin[0];
		p[i].pos[1] = p[i].origin[1];
		p[i].fined = false;
		p[i].not_moved = false;

		if (orb[p[i].pos[0]][p[i].pos[1]] == 0)
			orb[p[i].pos[0]][p[i].pos[1]] = i + 0.1;
	}
}

int find_which_unmove() {
	for (int k = 0; k < mass; k++) {
		if (p[k].not_moved) {
			return k;
		}
	}
	return mass + 1;
}

unsigned int destx, desty;
bool recheck = false;

void turn_set(unsigned TurnSetWhich, mt19937& gen) {
	if (p[TurnSetWhich].fined) return;
	uniform_int_distribution<int> f_set(0, dirs - 1);
	p[TurnSetWhich].face = static_cast<unsigned int>(f_set(gen));
	desty = p[TurnSetWhich].pos[1];
	destx = p[TurnSetWhich].pos[0];
	if (p[TurnSetWhich].face == n) desty++;
	else if (p[TurnSetWhich].face == s) desty--;
	else if (p[TurnSetWhich].face == w) destx--;
	else if (p[TurnSetWhich].face == e) destx++;
	else if (p[TurnSetWhich].face == nw) {
		destx--;
		desty++;
	} else if (p[TurnSetWhich].face == ne) {
		destx++;
		desty++;
	} else if (p[TurnSetWhich].face == sw) {
		destx--;
		desty--;
	} else if (p[TurnSetWhich].face == se) {
		destx++;
		desty--;
	}

	// Check boundaries
	if (destx >= mx) destx = mx - 1;
	if (desty >= my) desty = my - 1;

	ant::moving(TurnSetWhich, destx, desty);
}

void ant::turn(mt19937& gen) {
	for (int i = 0; i < mass; i++) {
		turn_set(i, gen);
	}
	int return_find_unmove = find_which_unmove();
	if (return_find_unmove != mass + 1) search_uncover(mass);
}

bool search_for_unmoved = false;

void ant::moving(unsigned int MoveWhich, unsigned int destx, unsigned int desty) {
	if (p[MoveWhich].fined) return;

	if (destx == p[MoveWhich].origin[0] && desty == p[MoveWhich].origin[1]) {
		p[MoveWhich].fined = true;
		finmass++;
		clear_stay(p[MoveWhich].pos[0], p[MoveWhich].pos[1]);
		return;
	}

	if (orb[destx][desty] == 0) {
		p[MoveWhich].not_moved = false;
		clear_stay(p[MoveWhich].pos[0], p[MoveWhich].pos[1]);
		p[MoveWhich].pos[0] = destx;
		p[MoveWhich].pos[1] = desty;
		orb[destx][desty] = MoveWhich + 0.1;
	} else {
		p[MoveWhich].not_moved = true;
		search_for_unmoved = true;
		p[MoveWhich].left[0] = destx;
		p[MoveWhich].left[1] = desty;
	}
}

void search_uncover(int stop_place) {
	if (search_for_unmoved) {
		for (int j = 0; j < stop_place; j++) {
			if (p[j].not_moved) {
				unsigned int destx = p[j].left[0];
				unsigned int desty = p[j].left[1];
				ant::moving(j, destx, desty);
			}
		}
	}
	search_for_unmoved = false;
}

void clear_stay(unsigned int clearx, unsigned int cleary) {
	orb[clearx][cleary] = 0;
}
