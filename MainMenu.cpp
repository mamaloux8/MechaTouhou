#include "MainMenu.h"

MainMenu::MenuResult MainMenu::show(sf::RenderWindow& window)
{

	//Load menu image from file
	sf::Texture image;
	image.loadFromFile("res/img/MainMenu.png");

	sf::Sprite sprite(image);

	//Setup clickable regions
	MenuItem playButton;
	playButton.rect.top = 45;
	playButton.rect.height = 380;
	playButton.rect.left = 0;
	playButton.rect.width = 1024;
	playButton.action = MenuResult::play;

	//Exit menu item coordinates
	MenuItem exitButton;
	exitButton.rect.left = 0;
	exitButton.rect.width = 1023;
	exitButton.rect.top = 383;
	exitButton.rect.height = 560;
	exitButton.action = MenuResult::exit;

	m_menuItems.push_back(playButton);
	m_menuItems.push_back(exitButton);

	window.draw(sprite);
	window.display();

	return getMenuResponse(window);


}

MainMenu::MenuResult MainMenu::handleClick(int x, int y)
{
	for (size_t i{}; i < m_menuItems.size(); ++i)
	{
		sf::Rect<int> menuItemRect{ m_menuItems[i].rect };
		if (menuItemRect.height > y
			&& menuItemRect.top < y
			&& menuItemRect.left < x
			&& menuItemRect.width > x)
		{
			return m_menuItems[i].action;
		}

	}

	return MenuResult::nothing;
}

MainMenu::MenuResult MainMenu::getMenuResponse(sf::RenderWindow& window)
{
	sf::Event menuEvent;

	while (true)
	{

		while (window.pollEvent(menuEvent))
		{
			if (menuEvent.type == sf::Event::MouseButtonPressed)
			{
				return handleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y);
			}

			if (menuEvent.type == sf::Event::Closed)
			{
				return MenuResult::exit;
			}
		}
	}
}