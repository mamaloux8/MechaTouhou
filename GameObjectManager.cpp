#include <iostream>

#include "GameObjectManager.h"
#include "Game.h"



GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	std::for_each(m_gameObjects.begin(), m_gameObjects.end(), GameObjectDeallocator());
}

void GameObjectManager::add(std::string name, VisibleGameObject* gameObject)
{
	int index{1};

	while (!m_gameObjects.insert(std::pair<std::string, VisibleGameObject*>{name, gameObject}).second)
	{
		name += " " + std::to_string(index);
		gameObject->setName(name);
		++index;
	}
	
	Game::getBulletManager().searchEnnemies();
}

void GameObjectManager::remove(std::string name)
{

	std::map<std::string, VisibleGameObject*>::iterator results = m_gameObjects.find(name);
	if (results != m_gameObjects.end())
	{
		delete results->second;
		m_gameObjects.erase(results);
		Game::getBulletManager().searchEnnemies();
	}
}

std::map<std::string, VisibleGameObject*>::const_iterator GameObjectManager::remove(std::map<std::string, VisibleGameObject*>::const_iterator itr)
{

	delete itr->second;
	auto nextItr = m_gameObjects.erase(itr);

	//Il faut enlever search ennemy et faire un observer à la place pour retourner direct l'itérateur
	//Et pas faire une variable (nextItr)
	Game::getBulletManager().searchEnnemies();

	return nextItr;
}


VisibleGameObject* GameObjectManager::get(std::string name) const
{
	std::map<std::string, VisibleGameObject*>::const_iterator results = m_gameObjects.find(name);
	if (results == m_gameObjects.end())
		return NULL;
	return results->second;

}

int GameObjectManager::getObjectCount() const
{
	return m_gameObjects.size();
}

void GameObjectManager::drawAll(sf::RenderWindow& renderWindow)
{
	std::map<std::string, VisibleGameObject*>::const_iterator itr = m_gameObjects.begin();
	while (itr != m_gameObjects.end())
	{
		itr->second->draw(renderWindow);
		itr++;
	}
}

void GameObjectManager::updateAll()
{
	std::map<std::string, VisibleGameObject*>::const_iterator itr = m_gameObjects.begin();

	float deltaTime = clock.restart().asSeconds();
	deltaTime += m_pauseTime;
	m_pauseTime = 0;

	while (itr != m_gameObjects.end())
	{
		if (!itr->second->getValid())
		{
			itr = remove(itr);

			if (itr == m_gameObjects.end())
				return;
		}
		itr->second->update(deltaTime);
		itr++;
	}
}

void GameObjectManager::display()
{
	for(int i{}; i < 5; ++i)
		std::cout << '\n';

	for (const auto& [key, val] : m_gameObjects)
	{
		std::cout << key << ": " << ((val != NULL) ? "valid" : "invalid");
		std::cout << '\n';
	}
}