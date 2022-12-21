/*
	Example of artificial intelliegence
*/

#include <SFML/Graphics.hpp>
#include <thread>
#include <iostream>

import AIEntity;

sf::Texture walls_texture;

std::vector<sf::Sprite> walls;
std::vector<sf::RectangleShape> wayPoints;
std::vector<AIEntity*> entities;

sf::Font regFont;
sf::Text text;


unsigned long long generation = 0;
unsigned long long absBestTime = 999999;
unsigned int generation_entity_count = 100;

bool renderEntities = true;


float distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

sf::RenderWindow window(sf::VideoMode(800, 800), "Neural networks");
sf::View view;

int entitiesReachedGoal = 0;

void checkGen() {
	while (window.isOpen()) {

		for (auto& e : entities) {
			e->update();
			for (auto& w : wayPoints) {
				if (e->sprite.getGlobalBounds().intersects(w.getGlobalBounds())) {
					if (e->reachedTime == sf::Time::Zero) {
						e->reachedTime = e->timer.getElapsedTime();
						entitiesReachedGoal++;
					}
				}
			}
		}


		text.setString("Generaatio : " + std::to_string(generation)
		+"\nTekoälyjen määrä : " + std::to_string(entities.size())+
		"\nParas Aika : " + std::to_string(absBestTime) + "ms");
			if (entitiesReachedGoal >= 1) {
				AIEntity* bestEntity = nullptr;
				sf::Int64 best = 100000000;
				for (auto e : entities) {
					if (best > e->reachedTime.asMilliseconds()) {
						best = e->reachedTime.asMilliseconds();
						bestEntity = e;
					}
				}
				if (bestEntity != nullptr) {
					for (auto e : entities) {
						if (e != bestEntity) {
							e->hp = 0;
						}
					}
					for (int i = 0;i < generation_entity_count;i++) {
						AIEntity* ai = new AIEntity();
						ai->createBased(bestEntity->network);
						entities.push_back(ai);
						for (auto& c : ai->network.cons) {
							//std::cout << "Connection weight : "<< c.weight<<'\t' << ", From *"<<c.connectedFrom << '\t' << ", To *" << c.connectedTo<< '\n';
						}
					}
					if (bestEntity->reachedTime != sf::Time::Zero) {
						if (bestEntity->reachedTime.asMilliseconds() < absBestTime) {
							absBestTime = bestEntity->reachedTime.asMilliseconds();
						}
					}
					// To reset position
					bestEntity->sprite.setPosition(300, 300);

				}
				entitiesReachedGoal = 0;
				std::cout << "New generation\n";
				generation++;
			}
	}
}


int main(){
	regFont.loadFromFile("Font.ttf");
	text.setFont(regFont);
	int _map = 0;
	walls_texture.loadFromFile("img/wall.png");

	sf::Image map;


	{
		using std::cout;
		int seed = 0;

		system("CLS");


		cout << "***************************************************\n";
		cout << "				HL Research team\n";
		cout << "Code by Lauri Etelamaki\n";
		cout << "W,A,S,D to move neural networks\n";
		cout << "SPACE to activate all input neurons\n";
		cout << "***************************************************\n";



		std::wcout << "Syötä tason numero : ";
		std::cin >> _map;
		if (_map > 3) {
			std::cout << "Maailma ei olemassa ; palautetaan arvo 0";
			_map = 0;
		}

		std::wcout << "Kuinka monta tekoälyä joka generaatiolla luodaan? : ";
		std::cin >> generation_entity_count;
		std::wcout << "Syötä satunnaisgeneraattorin siemen : ";
		std::cin >> seed;
		srand(seed);
	}

	map.loadFromFile(("img/"+std::to_string(_map)+".png"));
	// Load map
	for (int y = 0;y < map.getSize().y;y++) {
		for (int x = 0;x < map.getSize().x;x++) {
			if (map.getPixel(x, y).r >= 250) {
				sf::Sprite sprite;
				sprite.setTexture(walls_texture);
				sprite.setScale(4.f, 4.f);
				sprite.setPosition(x * 16, y * 16);
				walls.push_back(sprite);
			}
			if (map.getPixel(x, y).b >= 250) {
				sf::RectangleShape shape;
				shape.setFillColor(sf::Color::Blue);
				shape.setPosition(x * 16, y * 16);
				shape.setSize(sf::Vector2f(16.f, 16.f));
				wayPoints.push_back(shape);
			}
		}
	}
	sf::Event ev {};

	loadtexture();
	// Generate AIs
	for (int i = 0;i < 100;i++) {
		entities.push_back(new AIEntity());
		entities[i]->createRand();
	}

	window.setFramerateLimit(240);
	// Game loop
	std::thread thread(checkGen);


	while (window.isOpen()) {
		window.clear();
		window.setView(view);
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
			entities[0]->network.setInput(0, 0.f);
			entities[0]->network.setInput(1, 0.f);


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				for (auto& n : entities) {
					n->sprite.move(0,1);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				for (auto& n : entities) {
					n->sprite.move(0, -1);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				for (auto& n : entities) {
					n->sprite.move(-1, 0);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				for (auto& n : entities) {
					n->sprite.move(1, 0);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				for (auto& e : entities) {
					e->network.setInput(0, 1.f);
					e->network.setInput(1, 1.f);
				}
			}
			if (ev.type == sf::Event::MouseWheelMoved)
			{
				if (ev.mouseWheel.delta >= 1.f) {
					view.zoom(1.005f);
				} else if (ev.mouseWheel.delta <= -1.f) {
					view.zoom(.995f);
				}
			}
		}

		if (renderEntities) {
			for (auto& e : entities) {
				window.draw(e->viewArea);
				window.draw(e->sprite);
			}
		}
		for (auto& wall : walls) {
			window.draw(wall);
			for (auto& e : entities) {
				if (wall.getGlobalBounds().intersects(e->viewArea.getGlobalBounds())) {
					// Set neural networks inout neuron value to distance between player and the wall
					e->network.setInput(0, distance(wall.getPosition().x, wall.getPosition().y, e->sprite.getPosition().x, e->sprite.getPosition().x) / 100);
				}
			}
		}
		entities.erase(
			std::remove_if(entities.begin(), entities.end(),
				[](const AIEntity* o) { return o->hp<=0; }),
			entities.end());
		for (auto& w : wayPoints) {
			window.draw(w);
		}

		window.draw(text);
		window.display();
	}
	return 0;
}