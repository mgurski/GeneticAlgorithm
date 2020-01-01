#include "GeneticAlgorithm.h"


vector<int> GeneticAlgorithm::Random_route()
{
	vector<int> tmp;
	vector<int> random_route;
	int random_number;

	random_route.push_back(0);

	for (int i = 1; i < instance_size; i++) {
		tmp.push_back(i);
	}

	for (int j = 1; j < instance_size; j++) {
		random_number = rand() % (instance_size - j);
		random_route.push_back(tmp[random_number]);
		tmp.erase(tmp.begin() + random_number);
	}

	return random_route;
}


void GeneticAlgorithm::Read(string name)
{
	int distance_reader;
	string instance_name;
	vector<int> tmp;

	ifstream file;

	file.open(name + ".txt");

	if (!file) {
		cout << "Unable to open file" << endl;
	}
	else {

		file >> instance_name;
		file >> instance_size;

		for (int i = 0; i < instance_size; i++) {
			for (int j = 0; j < instance_size; j++) {

				file >> distance_reader;
				tmp.push_back(distance_reader);
			}

			matrix.push_back(tmp);
			tmp.clear();
		}

	}

	file.close();
}

int GeneticAlgorithm::Distance(vector<int> salesmans_route, int route_size)
{
	int sum = 0;

	for (int i = 0; i < route_size - 1; i++) {
		sum += matrix[salesmans_route[i]][salesmans_route[i + 1]];
	}
	sum += matrix[salesmans_route[route_size - 1]][salesmans_route[0]];

	return sum;
}



vector<vector<int>> GeneticAlgorithm::Generate_random_population(int population_size)
{
	vector<int> new_route;
	vector<vector<int>> new_population;

	for (int i = 0; i < population_size; i++) {
		new_route = Random_route();
		new_population.push_back(new_route);
	}

	return new_population;
}

vector<int> GeneticAlgorithm::Main_algorithm(int population_size, int iterations, double mutation_probability)
{

	int parents_population_size = population_size / 2;

	//generating the starting population
	population = Generate_random_population(population_size);

	int counter = 0;
	while (counter < iterations) {

		Evaluate_the_fitness(population_size);
		
		//Best_half_selection(population_size, parents_population_size);
		Roulette_selection(population_size, parents_population_size);
		//Tournament_selection(population_size, parents_population_size);

		population = PMX_Crossover(mutation_probability);
		//population = OX_Crossover(mutation_probability);

		counter++;
	}

	Evaluate_the_fitness(population_size);

	return best_route;

}

void GeneticAlgorithm::Evaluate_the_fitness(int population_size)
{
	//Computing all the distances
	distances.clear();

	all_distance_sum = 0;
	worst_distance = 0;

	for (int i = 0; i < population_size; i++) {

		int sum = 0;
		
		for (int j = 0; j < instance_size - 1; j++) {
			sum += matrix[population[i][j]][population[i][j + 1]];
		}
		sum += matrix[population[i][instance_size - 1]][population[i][0]];

		if (sum < best_distance) {
			best_distance = sum;
			best_route = population[i];
		}

		if (sum > worst_distance) {
			worst_distance = sum;
		}

		distances.push_back(sum);
	}
}

void GeneticAlgorithm::Best_half_selection(int population_size, int parents_population_size)
{

	//Adding the n best routes to the parents population
	int best_distance, index_to_add;
	for (int i = 0; i < parents_population_size; i++) {

		best_distance = distances[0];
		index_to_add = 0;
		for (int j = 0; j < population_size; j++) {
			if (best_distance > distances[j]) {
				best_distance = distances[j];
				index_to_add = j;
			}
		}
	parents_population.push_back(population[index_to_add]);
	distances[index_to_add] = INT_MAX;

	}
}

void GeneticAlgorithm::Roulette_selection(int population_size, int parent_population_size)
{
	
	double random_number;
	double sum_of_probabilities = 0.000000001;
	double sum=0;

	for (int i = 0; i < population_size; i++) {
		all_distance_sum += (((double)worst_distance+1.0) - (double)distances[i]);
	}

	for (int i = 0; i < parent_population_size; i++) {
		random_number = (double)rand() / (double)RAND_MAX;

		for (int j = 0; j < population_size; j++) {

			sum_of_probabilities += ((double(worst_distance)+1.0) - (double)distances[j]) / all_distance_sum;

			if (sum_of_probabilities >= random_number) {
				parents_population.push_back(population[j]);
				sum_of_probabilities = 0.000000001;
				break;
			}

		}
	}
}

void GeneticAlgorithm::Tournament_selection(int population_size, int parent_population_size)
{
	int tournament_size, random_number;
	int best_distance = INT_MAX;
	vector<int> tournament_attendees;
	vector<int> best_route;

	for (int i = 0; i < parent_population_size; i++) {

		tournament_attendees.clear();
		tournament_size = rand() % (population_size / 10) + 1;
		best_distance = INT_MAX;

		for (int j = 0; j < tournament_size; j++) {
			random_number = rand() % (population_size);
			tournament_attendees.push_back(random_number);
		}

		for (int j = 0; j < tournament_size; j++) {

			if (distances[tournament_attendees[j]] < best_distance) {
				best_distance = distances[tournament_attendees[j]];
				best_route = population[tournament_attendees[j]];
			}
		}

		parents_population.push_back(best_route);
	}
}


vector<vector<int>> GeneticAlgorithm::OX_Crossover(double mutation_probability)
{
	vector<vector<int>> parents_population_copy = parents_population;
	vector<int> parent_1, parent_2, offspring_1, offspring_2;
	int random_number, first_cut_point, second_cut_point;
	bool* offspring_1_vertices_list, * offspring_2_vertices_list;

	vector<int> offspring_1_order, offspring_2_order;

	while (!parents_population.empty()) {

		offspring_1_vertices_list = new bool[instance_size];
		offspring_2_vertices_list = new bool[instance_size];


		//Selecting two random parents
		random_number = rand() % (parents_population.size());

		parent_1 = parents_population[random_number];

		parents_population.erase(parents_population.begin() + random_number);

		random_number = rand() % (parents_population.size());

		parent_2 = parents_population[random_number];

		parents_population.erase(parents_population.begin() + random_number);

		//Generating two offsprings
		
		first_cut_point = rand() % (instance_size/2 - 1) + 2;

		second_cut_point = rand() % (instance_size / 2 - 2) + (instance_size / 2 + 1);
		



		for (int i = 0; i < instance_size; i++) {
			offspring_1_vertices_list[i] = false;
			offspring_2_vertices_list[i] = false;
		}
		

		for (int i = 0; i < instance_size; i++) {
			offspring_1.push_back(-1);
			offspring_2.push_back(-1);
		}

		offspring_1_vertices_list[0] = true;
		offspring_2_vertices_list[0] = true;
		offspring_1[0] = 0;
		offspring_2[0] = 0;



		for (int i = first_cut_point; i < second_cut_point; i++) {
			offspring_1[i] = parent_1[i];
			offspring_1_vertices_list[parent_1[i]] = true;
			offspring_2[i] = parent_2[i];
			offspring_2_vertices_list[parent_2[i]] = true;
		}


		for (int i = second_cut_point; i < instance_size; i++) {
			offspring_1_order.push_back(parent_2[i]);
			offspring_2_order.push_back(parent_1[i]);
		}
		for (int i = 1; i < second_cut_point; i++) {
			offspring_1_order.push_back(parent_2[i]);
			offspring_2_order.push_back(parent_1[i]);
		}

		int iterator_1 = second_cut_point;
		int iterator_2 = second_cut_point;

		for (int i = 0; i < offspring_1_order.size(); i++) {
			
			if (!offspring_1_vertices_list[offspring_1_order[i]]) {
				offspring_1[iterator_1] = offspring_1_order[i];
				
				if (iterator_1 == instance_size - 1) {
					iterator_1 = 1;
				}
				else {
					iterator_1++;
				}

			}

			if (!offspring_2_vertices_list[offspring_2_order[i]]) {
				offspring_2[iterator_2] = offspring_2_order[i];


				if (iterator_2 == instance_size - 1) {
					iterator_2 = 1;
				}
				else {
					iterator_2++;
				}

			}


		}


		if (((double)rand() / (double)RAND_MAX) < mutation_probability) {
			offspring_1 = mutation(offspring_1);
		}
		if (((double)rand() / (double)RAND_MAX) < mutation_probability) {
			offspring_2 = mutation(offspring_2);
		}

		parents_population_copy.push_back(offspring_1);
		parents_population_copy.push_back(offspring_2);

		offspring_1.clear();
		offspring_2.clear();

		offspring_1_order.clear();
		offspring_2_order.clear();

		delete[] offspring_1_vertices_list;
		delete[] offspring_2_vertices_list;
	}


	return parents_population_copy;
}


vector<vector<int>> GeneticAlgorithm::PMX_Crossover(double mutation_probability)
{
	vector<vector<int>> parents_population_copy = parents_population;
	vector<int> parent_1, parent_2, offspring_1, offspring_2;
	int random_number, first_cut_point, second_cut_point;
	bool* offspring_1_vertices_list, * offspring_2_vertices_list;

	while (!parents_population.empty()) {

		offspring_1_vertices_list = new bool[instance_size];
		offspring_2_vertices_list = new bool[instance_size];


		//Selecting two random parents
		random_number = rand() % (parents_population.size());

		parent_1 = parents_population[random_number];

		parents_population.erase(parents_population.begin() + random_number);

		random_number = rand() % (parents_population.size());

		parent_2 = parents_population[random_number];

		parents_population.erase(parents_population.begin() + random_number);

		//Generating two offsprings

	
		do
		{

			first_cut_point = rand() % (instance_size);

		} while ((instance_size) - first_cut_point <= 2);

		second_cut_point = rand() % (instance_size - first_cut_point) + first_cut_point;
		



		for (int i = 0; i < instance_size; i++) {
			offspring_1_vertices_list[i] = false;
			offspring_2_vertices_list[i] = false;
		}
		

		for (int i = 0; i < instance_size; i++) {
			offspring_1.push_back(-1);
			offspring_2.push_back(-1);
		}

		offspring_1_vertices_list[0] = true;
		offspring_2_vertices_list[0] = true;
		offspring_1[0] = 0;
		offspring_2[0] = 0;
		
		

		for (int i = first_cut_point; i < second_cut_point; i++) {
			offspring_1[i] = parent_2[i];
			offspring_1_vertices_list[parent_2[i]] = true;
			offspring_2[i] = parent_1[i];
			offspring_2_vertices_list[parent_1[i]] = true;
		}
		


		for (int i = first_cut_point; i < second_cut_point; i++) {

			if (!offspring_1_vertices_list[parent_1[i]]) {

				int check = parent_2[i];
					
				for (int j = 0; j < instance_size; j++) {

					if (check == parent_1[j]) {
						if (!offspring_1_vertices_list[parent_2[j]]) {
							offspring_1[j] = parent_1[i];
							break;
						}
						else {
							check = parent_2[j];
							j = 0;
						}
					}
				}
			
			}

			if (!offspring_2_vertices_list[parent_2[i]]) {

				if (!offspring_2_vertices_list[parent_2[i]]) {

					int check = parent_1[i];

					for (int j = 0; j < instance_size; j++) {

						if (check == parent_2[j]) {
							if (!offspring_2_vertices_list[parent_1[j]]) {
								offspring_2[j] = parent_2[i];
								break;
							}
							else {
								check = parent_1[j];
								j = 0;
							}
						}
					}

				}
			}

		}


		for (int i = 0; i < instance_size; i++) {

			if (offspring_1[i] == -1) {
				offspring_1[i] = parent_1[i];			
			}

			if (offspring_2[i] == -1) {
				offspring_2[i] = parent_2[i];			
			}
		}


		if (((double)rand() / (double)RAND_MAX) < mutation_probability){
			offspring_1 = mutation(offspring_1);
		}
		if (((double)rand() / (double)RAND_MAX) < mutation_probability) {
			offspring_2 = mutation(offspring_2);
		}

		parents_population_copy.push_back(offspring_1);
		parents_population_copy.push_back(offspring_2);

		offspring_1.clear();
		offspring_2.clear();
		delete[] offspring_1_vertices_list;
		delete[] offspring_2_vertices_list;
	}


	return parents_population_copy;
}



vector<int> GeneticAlgorithm::mutation(vector<int> before_mutation)
{
	vector<int> after_mutation = before_mutation;

	int a = rand() % (instance_size - 1) + 1;
	int b = rand() % (instance_size - 1) + 1;
	while (a == b) b = rand() % (instance_size - 1) + 1;

	int temp = before_mutation[a];
	after_mutation[a] = before_mutation[b];
	after_mutation[b] = temp;

	return after_mutation;
}

