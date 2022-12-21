#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

import Neuron;

export module NeuralNet;

export enum Layer {
	Input, Hidden, Output
};

export struct NeuralNet
{

	void initRand() {
		init();
		for (int i = 0;i < rand()%500;i++) {
			cons.push_back(Connection((rand() % 100) / 50,
				&neurons.at(Input)[rand() % 3],
				&neurons.at(Hidden)[rand() % 5]));
			cons.push_back(Connection((rand() % 100) / 50,
				&neurons.at(Hidden)[rand() % 5],
				&neurons.at(Output)[rand() % 4]));
		}
	}
	void init() {
		// Input
		std::vector<Neuron> iv = { Neuron(),Neuron(),Neuron(),Neuron(), };
		neurons.insert({ Input,{iv} });
		// Hidden layer
		std::vector<Neuron> hv = { Neuron(),Neuron(),Neuron(),Neuron(),Neuron(),Neuron() };
		neurons.insert({ Hidden,{hv} });
		// Output layer
		std::vector<Neuron> o = { Neuron(),Neuron(), Neuron(), Neuron() };
		neurons.insert({ Output,{o} });
	}
	float randFloat() {
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
	void initBased(const NeuralNet cpy) {
		destory();
		neurons = cpy.neurons;
		cons = cpy.cons;
		// Add one random connection for random times
		if (rand() % 5 == 1) {
			for (int i = 0;i < rand() % 500;i++) {
				cons.push_back(Connection((rand() % 100) / 50,
					&neurons.at(Input)[rand() % 3],
					&neurons.at(Hidden)[rand() % 5]));
				cons.push_back(Connection((rand() % 100) / 50,
					&neurons.at(Hidden)[rand() % 5],
					&neurons.at(Output)[rand() % 4]));
			}
		}
		else {
			for (int i = 0;i < rand() % 10 - rand()%10;i++) {
				cons[rand() % cons.size()].weight += randFloat() / 10;
			}
		}
		if (rand() % 5 == 0) {
			cons.pop_back();
		}

	}

	void destory() {
		cons.clear();
		neurons.clear();
	}
	void update() {
		try {
			for (auto& n : neurons.at(Output)) {
				n.value = 0.f;
			}
			for (auto& n : neurons.at(Hidden)) {
				n.value = 0.f;
			}
			for (auto& con : cons) {
				if (con.connectedFrom != nullptr && con.connectedTo != nullptr) {
					con.connectedTo->value += con.connectedFrom->value * con.weight;
				}
			}
		}
		catch (std::exception& e) {
			std::cout << e.what() << '\n';
		}
	}
	void setInput(int index, float value) {
		neurons.at(Input)[index].value = value;
	}
	sf::Vector2f getOutputAsVelocity() {
		return sf::Vector2f(neurons.at(Output)[2].value - neurons.at(Output)[0].value, neurons.at(Output)[3].value-neurons.at(Output)[1].value);
	}
	float getOutputFrom(int index) {
		return neurons.at(Output)[index].value;
	}
	
	std::map<Layer, std::vector<Neuron>> neurons;
	std::vector<Connection> cons;
};