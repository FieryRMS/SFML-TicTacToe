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
	void setstr(string s,int padding=0, int sz = 30)
	{
		text.setString(s);
		text.setCharacterSize(sz);
		reset(padding);
	}
	void location(float x, float y)
	{
		text.setPosition(sf::Vector2f(x, y));
		overlay.setPosition(sf::Vector2f(x, y));
	}
	bool draw(sf::RenderWindow& window, sf::Vector2i pos, bool isclicked)
	{
		window.draw(text);
		bool contained = 0;
		auto mspos= window.mapPixelToCoords(pos);
		if (overlay.getGlobalBounds().contains(mspos))
		{
			contained = 1;
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
		bool ret = wasClicked && contained && !isclicked && window.hasFocus();
		if (ret)
			wasClicked = 0;
		return ret;
	}
	bool ifClick(sf::RenderWindow& window, sf::Vector2i pos, bool isclicked)
	{
		bool contained = 0;
		auto mspos = window.mapPixelToCoords(pos);
		if (overlay.getGlobalBounds().contains(mspos))
		{
			contained = 1;
			if (isclicked)
				wasClicked = 1;
		}
		else
			wasClicked = 0;
		bool ret = wasClicked && contained && !isclicked;
		if (ret)
			wasClicked = 0;
		return ret;
	}
};
