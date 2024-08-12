#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <array>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "Observer.h"
#include "Player.h"
#include "Ennemy.h"

class UiManager
	: public Observer<Player>, public Observer<Ennemy>
{
public:
	UiManager(std::string, float);
	~UiManager() = default;

	virtual void onNotify(const Player*, Observer<Player>::State);
	virtual void onNotify(const Ennemy*, Observer<Ennemy>::State);

	void draw(sf::RenderTexture&);
	void update();

	bool getUpdateUi() { return m_updateUI; };
	void setUpdateUi(bool update) { m_updateUI = update; };

private:
	//Faire un buffer plutôt que d'utiliser un bool ? C'est-à-dire mettre les sprites qu'on doit dessiner
	//dans un buffer et les envoyer à la window
	bool m_updateUI{ true };

	int m_score{0};
	int m_scoreMultiplier{ 1 };

	int m_ennemyStreak{ 0 };

	sf::Texture m_healthTexture;
	//Ce serait mieux d'utiliser une constante MAX_LIVES
	std::array<sf::Sprite, 8> m_healthSprites;
	int m_playerLives{8};

	sf::Font m_font{};
	std::string m_scoreString{"0"};
	sf::Text m_scoreText{};

	bool m_displayHealthBar{ false };
	sf::Sprite m_emptyHealthBar{};
	sf::Sprite m_healthBar{};

	std::string m_statesString{ "0" };
	sf::Text m_statesText{};
};

#endif

