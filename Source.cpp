#include <SFML/Graphics.hpp>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace sf;

int size = 64;
bool game_over = false;
bool immortality_available = true;
bool immortality_enaibled = false;

class snake
{
private:
	std::vector<std::pair<int, int>> body;
	Texture texture;
	std::pair<int, int> fruit;
	int fruits_collected;

public:
	snake()
	{
		srand(time(NULL));

		fruit.first = 11;
		fruit.second = 11;

		body.push_back({ 5, 5 });
		body.push_back({ 6, 5 });
		body.push_back({ 7, 5 });

		fruits_collected = 0;

		texture.loadFromFile("image.png");
	}

	void draw(RenderWindow &window)
	{
		Sprite sprite;
		sprite.setTexture(texture);

		int n = body.size();

		// Рисуем хвост
		if (body[0].first == body[1].first)
		{
			if (body[0].second < body[1].second)
			{
				sprite.setTextureRect(IntRect(4 * size, 3 * size, size, size));
			}
			else
			{
				sprite.setTextureRect(IntRect(3 * size, 2 * size, size, size));
			}
		}
		else
		{
			if (body[0].first < body[1].first)
			{
				sprite.setTextureRect(IntRect(4 * size, 2 * size, size, size));
			}
			else
			{
				sprite.setTextureRect(IntRect(3 * size, 3 * size, size, size));
			}
		}
		sprite.setPosition(body[0].first * size, body[0].second * size);
		window.draw(sprite);


		// Рисуем тело
		for (int i = 1; i < n - 1; ++i)
		{
			if (body[i - 1].first == body[i].first && body[i].first == body[i+1].first)
			{
					sprite.setTextureRect(IntRect(2 * size, 1 * size, size, size));	
			}
			else if (body[i - 1].second == body[i].second && body[i].second == body[i + 1].second)
			{
					sprite.setTextureRect(IntRect(1 * size, 0 * size, size, size));
			}
			else if (body[i - 1].second > body[i].second && body[i].first < body[i + 1].first || 
					 body[i - 1].first > body[i].first && body[i].second < body[i + 1].second)
			{
				sprite.setTextureRect(IntRect(0 * size, 0 * size, size, size));
			}
			else if (body[i - 1].second > body[i].second && body[i].first > body[i + 1].first ||
					 body[i - 1].first < body[i].first && body[i].second < body[i + 1].second)
			{
				sprite.setTextureRect(IntRect(2 * size, 0 * size, size, size));
			}
			else if (body[i - 1].first > body[i].first && body[i].second > body[i + 1].second ||
					 body[i - 1].second < body[i].second && body[i].first < body[i + 1].first)
			{
				sprite.setTextureRect(IntRect(0 * size, 1 * size, size, size));
			}
			else if (body[i - 1].first < body[i].first && body[i].second > body[i + 1].second ||
					 body[i - 1].second < body[i].second && body[i].first > body[i + 1].first)
			{
				sprite.setTextureRect(IntRect(2 * size, 2 * size, size, size));
			}
			sprite.setPosition(body[i].first * size, body[i].second * size);
			window.draw(sprite);
		}

		// Рисуем голову
		if (body[n-2].first == body[n-1].first)
		{
			if (body[n-2].second > body[n-1].second)
			{
				sprite.setTextureRect(IntRect(3 * size, 0 * size, size, size));
			}
			else
			{
				sprite.setTextureRect(IntRect(4 * size, 1 * size, size, size));
			}
		}
		else
		{
			if (body[n-2].first < body[n-1].first)
			{
				sprite.setTextureRect(IntRect(4 * size, 0 * size, size, size));
			}
			else
			{
				sprite.setTextureRect(IntRect(3 * size, 1 * size, size, size));
			}
		}
		sprite.setPosition(body[n-1].first * size, body[n-1].second * size);
		window.draw(sprite);


		sprite.setTextureRect(IntRect(0 * size, 3 * size, size, size));
		sprite.setPosition(fruit.first * size, fruit.second * size);
		window.draw(sprite);

		if (game_over)
		{
			Font font;
			font.loadFromFile("font.ttf");

			Text text;
			text.setFont(font);

			std::string s1 = "\t  Game over\nYou ate ";
			std::string s2 = " fruits\n";

			text.setString(s1 + std::to_string(fruits_collected) + s2);
			text.setCharacterSize(40);
			text.setPosition(640-90, 480-40);
			window.draw(text);
		}
	}

	void update(int direction)
	{
		if (direction == -1 || game_over)
		{
			return;
		}

		int n = body.size();

		if (direction == 1)
		{
			body.push_back({ body[n - 1].first + 1, body[n - 1].second });
		}
		else if (direction == 2)
		{
			body.push_back({ body[n - 1].first - 1, body[n - 1].second });
		}
		else if (direction == 3)
		{
			body.push_back({ body[n - 1].first, body[n - 1].second + 1 });
		}
		else if (direction == 4)
		{
			body.push_back({ body[n - 1].first, body[n - 1].second - 1 });
		}

		if (body[n].first < 0 || body[n].first == 20 || body[n].second < 0 || body[n].second == 15)
		{
			game_over = true;
			return;
		}
		

		if (body[n].first == fruit.first && body[n].second == fruit.second)
		{
			fruits_collected += 1;
			bool on_snake = true;
			while (on_snake)
			{ 
				fruit.first = rand() % 20;
				fruit.second = rand() % 15;
				on_snake = false;
				for (int i = 0; i < body.size(); ++i)
				{
					if (fruit.first == body[i].first && fruit.second == body[i].second)
					{
						on_snake = true;
						break;
					}
				}
			}
		}
		else
		{
			body.erase(body.begin(), body.begin() + 1);
		}

		if (immortality_enaibled)
		{
			return;
		}

		n = body.size() - 1;
		for (int i = 0; i < n; ++i)
		{
			if (body[i].first == body[n].first && body[i].second == body[n].second)
			{
				game_over = true;
				return;
			}
		}
	}
};

int main()
{
	RenderWindow window(sf::VideoMode(1280, 960), "Snake");
	window.setFramerateLimit(7);

	int direction = -1;

	snake s;

	Clock clock;
	
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		if (time > 3 && immortality_enaibled)
		{
			immortality_enaibled = false;
			clock.restart();
		}

		if (time > 10 && immortality_enaibled == false)
		{
			immortality_available = true;
		}

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}



		if (Keyboard::isKeyPressed(Keyboard::Space) && immortality_available)
		{
			clock.restart();
			immortality_enaibled = true;
			immortality_available = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (direction != 2)
			{
				direction = 1;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (direction != 1 && direction != -1)
			{
				direction = 2;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (direction != 4)
			{
				direction = 3;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (direction != 3)
			{
				direction = 4;
			}
		}

		window.clear(Color(0,0,255));

		if (immortality_available)
		{
			window.clear(Color(211, 191, 0));
		}
		else if (immortality_enaibled)
		{
			window.clear(Color(255, 119, 251));
		}
		else
		{
			window.clear();
		}

		s.update(direction);
		s.draw(window);
		window.display();
	}

	return 0;
}