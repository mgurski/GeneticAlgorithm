#include "GeneticAlgorithm.h"
#include <iostream>
#include <ctime>

using namespace std;


int main() {

	srand(time(NULL));

	GeneticAlgorithm * geneticAlgorithm;
	string file_name;
	

	geneticAlgorithm = new GeneticAlgorithm();
	cout << "Enter the file name: " << endl;
	cin >> file_name;
	geneticAlgorithm->Read(file_name);

	vector<int> best_route = geneticAlgorithm->Main_algorithm(200, 100, 0.05);

	for (int i = 0; i < best_route.size(); i++) {
		cout << best_route[i] << " ";
	}

	cout << endl;

	cout << geneticAlgorithm->Distance(best_route, best_route.size());

	cout << endl;

}

