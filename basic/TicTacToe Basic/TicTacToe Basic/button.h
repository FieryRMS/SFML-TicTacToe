#include <SFML/Graphics.hpp>
using namespace std;
struct Button
{
	sf::Text text;
	sf::RectangleShape overlay;
	float height=0, width=0;
	bool wasClicked = 0;
	void reset(int padding = 0)
	{
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		text.setFillColor(sf::Color(0, 0, 0));

		overlay.setSize(sf::Vector2f(textRect.width + padding, textRect.height + padding));
		textRect = overlay.getLocalBounds();
		overlay.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		overlay.setFillColor(sf::Color(255, 255, 255, 0));
		height = textRect.height, width = textRect.width;
	}
	void setfnt(sf::Font& font, int sz=30)
	{
		text.setFont(font);
		text.setCharacterSize(sz);
	}
	void setstr(string s, int padding=0, int sz = 30)
	{
		text.setString(s);
		text.setCharacterSize(sz);
		reset(padding);
	}
	void setPosition(float x, float y)
	{
		text.setPosition(sf::Vector2f(x, y));
		overlay.setPosition(sf::Vector2f(x, y));
	}
	bool draw(sf::RenderWindow& window)
	{
		window.draw(text);
		bool isclicked=sf::Mouse::isButtonPressed(sf::Mouse::Left);
		auto mspos= window.mapPixelToCoords(sf::Mouse::getPosition(window));
		if (overlay.getGlobalBounds().contains(mspos) && window.hasFocus())
		{
			if (isclicked)
			{
				overlay.setFillColor(sf::Color(64, 64, 64, 50));
				wasClicked = 1;
			}
			else
				overlay.setFillColor(sf::Color(100, 100, 100, 40));
		}
		else
		{
			overlay.setFillColor(sf::Color(0, 0, 0, 0));
			wasClicked = 0;
		}
		window.draw(overlay);
		bool ret = wasClicked && !isclicked;
		if (ret)
			wasClicked = 0;
		return ret;
	}
};
