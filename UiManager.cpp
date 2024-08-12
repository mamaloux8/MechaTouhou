#include "UiManager.h"
#include "Game.h"


UiManager::UiManager(std::string healthStr, float healthScale)
{
	m_font.loadFromFile("res/fonts/arial.ttf");

	m_scoreText.setPosition(Game::SCREEN_WIDTH - 300.f, 100.f);
	m_scoreText.setScale({ 2.f, 2.f });
	m_scoreText.setFillColor(sf::Color::Green);
	m_scoreText.setFont(m_font);
	m_scoreText.setString(m_scoreString);

	m_healthTexture = *Game::getTextureCache().getImage(healthStr);

	sf::Vector2f healthSpriteSize{ m_healthTexture.getSize().x * healthScale,  m_healthTexture.getSize().y * healthScale};
	for (size_t i{}; i < m_healthSprites.size() ; ++i)
	{
		m_healthSprites[i].setTexture(m_healthTexture);
		m_healthSprites[i].setScale({ healthScale, healthScale });
		m_healthSprites[i].setPosition(healthSpriteSize.x * i, Game::SCREEN_HEIGHT - healthSpriteSize.y);
	}

	//Boss Healthbar

	m_emptyHealthBar.setTexture(*Game::getTextureCache().getImage("res/img/HealthbarEmpty.png"));
	m_emptyHealthBar.setScale(0.45f, 0.45f);

	m_healthBar.setTexture(*Game::getTextureCache().getImage("res/img/Healthbar.png"));
	m_healthBar.setScale(0.45f, 0.45f);

	m_statesText.setPosition(37.f, 10.f);
	m_statesText.setScale({ 1.5f, 1.5f });
	m_statesText.setFillColor(sf::Color::Green);
	m_statesText.setFont(m_font);
	m_statesText.setString(m_statesString);


}

void UiManager::onNotify(const Player* player, Observer<Player>::State state)
{
	if (state == Observer<Player>::State::isHit)
	{
		m_playerLives = player->getLives();
		m_ennemyStreak = 0;
		m_updateUI = true;
	}
}

void UiManager::onNotify(const Ennemy* ennemy, Observer<Ennemy>::State state)
{
	if (state == Observer<Ennemy>::State::isDead)
	{
		m_score += ennemy->getScore() * m_scoreMultiplier;
		m_scoreString = std::to_string(m_score);
		m_scoreText.setString(m_scoreString);
		++m_ennemyStreak;

		//Peut-être faire une condition pour que ce soit uniquement lors de la mort de boss
		m_displayHealthBar = false;

		//Mettre dans une fonction
		if (m_ennemyStreak >= 10 && m_ennemyStreak < 20)
			m_scoreMultiplier = 2;
		else if (m_ennemyStreak >= 20 && m_ennemyStreak < 30)
			m_scoreMultiplier = 3;
		else if (m_ennemyStreak >= 30 && m_ennemyStreak < 50)
			m_scoreMultiplier = 4;
		else if (m_ennemyStreak >= 50)
			m_scoreMultiplier = 8;
		else
			m_scoreMultiplier = 1;
	}
	else if (state == Observer<Ennemy>::State::isHit)
	{
		m_healthBar.setTextureRect(sf::IntRect{ 0, 0,  static_cast<int>(m_emptyHealthBar.getTextureRect().width * (static_cast<float>(ennemy->getHealth()) / static_cast<float>(ennemy->getBaseHealth())) ),
		static_cast<int>(m_healthBar.getTextureRect().height) });
	}
	else if (state == Observer<Ennemy>::State::isSpawn)
	{
		m_displayHealthBar = true;

		const Boss* boss{ dynamic_cast<const Boss*>(ennemy) };
		if (boss != NULL)
		{
			m_statesString = std::to_string(boss->getNbStates() - boss->getCurrentState() - 1);
			m_statesText.setString(m_statesString);
		}
	}

	m_updateUI = true;
}

void UiManager::draw(sf::RenderTexture& rw)
{
	rw.clear(sf::Color(0,0,0,0));
	for (size_t i{}; i < m_playerLives; ++i)
	{
		rw.draw(m_healthSprites[i]);
	}

	//Draw boss healthbar
	if (m_displayHealthBar)
	{
		rw.draw(m_emptyHealthBar);
		rw.draw(m_healthBar);
		rw.draw(m_statesText);
	}
	
	//Draw score
	rw.draw(m_scoreText);

	rw.display();
	m_updateUI = false;
}

void UiManager::update()
{

}