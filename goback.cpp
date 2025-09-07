#include<iostream>
#include<random>

enum DirTypes {n, s, w, e, nw, ne, sw, se, dir_counts};

using namespace std;

const unsigned dirs = dir_counts;
const unsigned char mx = 100, my = 100;
const unsigned int mass = mx * my * 0.9;
unsigned int finmass;//finished ones


float orb[mx][my];

void clear_stay(unsigned int, unsigned int);
void ant::settle(mt19937& gen);
void ant::turn(mt19937& grn);
void moving();

class ant {
private:
	unsigned int pos[2];//moving location
	unsigned int origin[2];//origin
	char face;
	bool fined;
	bool not_moved;
	unsigned int left[2];
public:
	static void settle(mt19937& gen);
	static void turn(mt19937& grn);
	static void moving();
};
ant p[mass];

int main() {
	random_device rd;
	mt19937 gen(rd());
	ant::settle(gen);
	while (finmass < mass) {
		ant::turn(gen);
		moving();
	}
	return 0;
}
void ant::settle(mt19937& gen) {
	uniform_int_distribution<int> x_set(1, mx);
	uniform_int_distribution<int> y_set(1, my);
	for (int i = 0; i < mass; i++) {
		p[i].origin[0] = static_cast<unsigned int>(x_set(gen));
		p[i].origin[1] = static_cast<unsigned int>(y_set(gen));
		p[i].pos[0] = p[i].origin[0];
		p[i].pos[1] = p[i].origin[1];

		if (orb[p[i].pos[0]][p[i].pos[1]] != 0) orb[p[i].pos[0]][p[i].pos[1]] = i + 0.1;
	}
}

void find_which_unmove() {
	for (int k = 0; k < mass; k++) {
		if (!p[k].not_moved) {
			return k;
			return;
        } else if(k==mass-1){
            return mass+1;
        }
	}
}

unsigned int destx, desty;
bool recheck;
void turn_set(unsigned TurnSetWhich) {
	if (p[TurnSetWhich].fined)break;
	p[TurnSetWhich].face = static_cast<unsigned int>(f_set(gen));
	desty = p[TurnSetWhich].pos[1];
	destx = p[TurnSetWhich].pos[0];
	if (p[TurnSetWhich].face == n) desty++;
	else if (p[TurnSetWhich].face == s)desty--;
	else if (p[TurnSetWhich].face == w) destx--;
	else if (p[TurnSetWhich].face == s) destx++;
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
	moving();
}

void ant::turn(mt19937& gen) {
	uniform_int_distribution<int> f_set(1, dirs);
	for (int i = 0; i < mass; i++) {
		turn_set(i);
	}
    if(find_which_unmove()!=mass+1);
}

void moving(unsigned int MoveWhich) {
	if (destx == p[MoveWhich].origin[0] || desty == p[MoveWhich].origin[1]) {
		p[MoveWhich].fined = true;
		return;
	}
	if (orb[destx][desty] == 0) {
		p[MoveWhich].not_moved = false;
		clear_stay(p[MoveWhich].pos[0], p[MoveWhich].pos[1]);
		orb[destx][desty] = MoveWhich + 0.1;
	} else {
		p[MoveWhich].not_moved = true;
		p[MoveWhich].left[0] = destx;
		p[MoveWhich].left[1] = desty;
	}
}
void clear_stay(unsigned int clearx, unsigned int cleary) {
	orb[clearx][cleary] = 0;
}
