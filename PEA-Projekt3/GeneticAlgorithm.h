#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>


using namespace std;

class GeneticAlgorithm {

private:

	vector<vector<int>> matrix, population, parents_population;
	vector<int> best_route, distances;
	int instance_size, best_distance=INT_MAX;
	int worst_distance = 0;
	double all_distance_sum;
public:
	void Read(string);
	int Distance(vector<int>, int);
	vector<int> Random_route();


	vector<vector<int>> Generate_random_population(int population_size);
	vector<int> Main_algorithm(int population_size, int iterations, double mutation_probability);

	void Evaluate_the_fitness(int population_size);

	void Best_half_selection(int population_size, int parents_population_size);
	void Roulette_selection(int population_size, int parent_population_size);
	void Tournament_selection(int population_size, int parent_population_size);

	vector<vector<int>> OX_Crossover(double);
	vector<vector<int>> PMX_Crossover(double);

	vector<int> mutation(vector<int>);


};


#endif 