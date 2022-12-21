export module Neuron;

export struct Neuron {
	Neuron() {

	}
	Neuron(float v) : value(v) {
	}
	float value = 0.f;
};

export struct Connection {
	Connection(float w, Neuron* from, Neuron * to) {
		weight = w;
		connectedFrom = from;
		connectedTo = to;
	}
	float weight = 0.f;
	Neuron* connectedFrom = nullptr;
	Neuron* connectedTo = nullptr;
};