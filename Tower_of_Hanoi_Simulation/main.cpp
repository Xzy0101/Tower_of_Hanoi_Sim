/*
* A Tower of Hanoi setup consists of N disks, weighing 1,…,N kilos. Each disk sits in one of MM stacks, 
* at positions 0,…,M−1. All disks start stacked at position zero, ordered by weight with the lightest on top. 
* At each point in time, a valid move can be made. A valid move consists of taking the top disk off of one of the 
* stacks and moving it to the top of the stack to the immediate left or right (that is, at position one less or 
* one greater than its current position), provided that the moved disk weighs less than all other disks at its 
* destination stack. For eample, in a N=M=3 problem with disks 1,2,3 at positions 0,1,2, respecitvely, 
* the only valid moves are for disk 1 to move to position 1 and disk 2 to move to position 2. We choose moves 
* uniformly at random amongst all valid moves at a given time. We are interested in the position of the center of 
* mass after T moves, that is
* ∑(d)(d * p(d)) / ∑(d)d
* where d is the disk weight and p(d) is the position of a disk weighing d.
* 
* For M=3,N=3, what is the mean of the center of mass after T=16?
* For M=3,N=3, what is the standard deviation of the center of mass after T=16?
* 
* For M=6,N=6, what is the mean of the center of mass after T=256?
* For M=6,N=6, what is the standard deviation of the center of mass after T=256?
*/

/*
* This is a simulation program of the problem above.
*/

#include "HanoiDisk.h"
#include <string>
#include <stack>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
using std::cin;
using std::cout;

double test(int m, int t);

int main() {
	constexpr int max_result_storage = 13107200;//double count of 100MB
	//constexpr int max_result_storage = 100;//test

	/* set parameter */
	int m, t, repeat;	
	cout << "M=N=";
	cin >> m;
	cout << "T=";
	cin >> t;
	cout << "repeat:";
	cin >> repeat;

	/* run tests */
	cout << "running...\n";
	auto start = chrono::steady_clock::now();
	double res;
	double mean = 0;
	double s = 0;
	if (repeat < max_result_storage) {
		vector<double> result;
		result.reserve(repeat);//reserve enough space to store results, improve speed.

		for (int i = 0; i < repeat; ++i) {
			res = test(m, t);
			result.push_back(res);
			mean += res;
		}

		mean /= repeat;
		for (double x : result)
			s += (x - mean)*(x - mean);
		s = sqrt(s / repeat);
	}
	else {
		string file_name;
		cout << "data file name: ";
		cin >> file_name;
		ofstream data{ file_name , fstream::binary };
		for (int i = 0; i < repeat; ++i) {
			res = test(m, t);
			data.write((char*)&res, sizeof(double));
			mean += res;
		}
		data.close();

		ifstream dt{ file_name , fstream::binary };
		mean /= repeat;
		while (dt.read((char*)&res, sizeof(double)))
			s += (res - mean)*(res - mean);
		s = sqrt(s / repeat);
	}
	auto end = chrono::steady_clock::now();

	/* print result */
	cout << fixed << setprecision(10)
		<< "mean: " << mean
		<< "\nstandard deviation: " << s << "\n";
	cout << "running time: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s\n";
	cout << "Test complete, enter any character to exit:";
	char x;
	cin >> x;
	return 0;
}

int randint(int min, int max) {
	static random_device rd;   // non-deterministic generator  
	static mt19937 gen(rd());  // to seed mersenne twister.  
	uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

vector<pair<int, int>> getAllPossibleMoves(const vector<stack<HanoiDisk>> &v) {
	vector<pair<int, int>> moves;
	for (int i = 0; i < v.size(); ++i) {
		if (!v[i].empty()) {
			if (i - 1 >= 0)
				if (v[i - 1].empty())
					moves.push_back(pair<int, int>(i, i - 1));
				else
					if (v[i].top().movable(v[i - 1].top()))
						moves.push_back(pair<int, int>(i, i - 1));
			if (i + 1 < v.size())
				if (v[i + 1].empty())
					moves.push_back(pair<int, int>(i, i + 1));
				else
					if (v[i].top().movable(v[i + 1].top()))
						moves.push_back(pair<int, int>(i, i + 1));
		}
	}
	return moves;
}

double test(int m, int t) {
	/* initialize Hanoi Tower */
	vector<stack<HanoiDisk>> v(m);
	for (int i = m; i > 0; --i)
		v[0].push(HanoiDisk(i));

	/* run test */
	vector<pair<int, int>> moves;
	pair<int, int> move;
	for (int i = 0; i < t; ++i) {
		moves = getAllPossibleMoves(v);
		//cout << "possible moves: ";
		//for (pair<int, int> x : moves)
			//cout << x.first << "-" << x.second << " ";
		//cout << endl;
		move = moves[randint(0, moves.size() - 1)];
		v[move.second].push(v[move.first].top());
		v[move.first].pop();
		//cout << move.first << " to " << move.second << endl;
	}

	/* calculate center of mass */
	double center = 0;
	int totalWeight = 0;
	for (int i = 0; i < v.size(); ++i) {
		while (!v[i].empty()) {
			center += i * v[i].top().getWeight();
			totalWeight += v[i].top().getWeight();
			v[i].pop();
		}
	}
	center /= totalWeight;
	//cout << "center: " << center << endl;
	return center;
}