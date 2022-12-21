#include <SFML/Graphics.hpp>
export module AIEntity;

import NeuralNet;

sf::Texture texture;

export void loadtexture() {
	texture.loadFromFile("img/Entity.png");
}

export struct AIEntity {
	AIEntity() {
		sprite.setTexture(texture);
		viewArea.setSize(sf::Vector2f(20.f, 150.f));
		sprite.setPosition(sf::Vector2f(300.f, 300.f));
		viewArea.setFillColor(sf::Color::Color(255, 255, 255, 100));
	}
	void createRand() {
		network.initRand();
	}
	void createBased(const NeuralNet &ne) {
		network.initBased(ne);
	}
	void update() {
		try
		{
			network.update();
		}
		catch (const std::exception&e)
		{
			printf("%c\n",e.what());
		}
		sprite.move(network.getOutputAsVelocity().x / 10, network.getOutputAsVelocity().y / 10);
		viewArea.setPosition(sprite.getPosition());
		viewArea.rotate(network.getOutputFrom(2));
		sprite.setRotation(viewArea.getRotation());
	}
	sf::Time reachedTime = sf::Time::Zero;
	int64_t hp = 10;
	sf::RectangleShape viewArea;
	sf::Sprite sprite;
	sf::Clock timer;
	NeuralNet network;
};