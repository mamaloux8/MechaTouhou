#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <vector>

class MainMenu
{

public:
	enum MenuResult
	{
		nothing,
		exit,
		play,
	};

	struct MenuItem
	{
		sf::Rect<int> rect;
		MenuResult action{};
	};

	MenuResult show(sf::RenderWindow& window);

private:
	MenuResult getMenuResponse(sf::RenderWindow& window);
	MenuResult handleClick(int x, int y);
	std::vector<MenuItem> m_menuItems;
};



#endif