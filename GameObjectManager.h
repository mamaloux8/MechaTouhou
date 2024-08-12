#ifndef GAMEOBJECT_MANAGER_H
#define GAMEOBJECT_MANAGER_H

#include "VisibleGameObject.h"

#include <iostream>
#include <typeinfo>

class GameObjectManager
{
public:

	//Return the keys of object of a certain type
	template <typename T>
	std::vector<std::pair<std::string, T*>> getObjectsOfType() const
	{
		std::vector<std::pair<std::string, T*>> vec;
		vec.reserve(30);
		for (const auto& [key, val] : m_gameObjects)
		{
			T* castVal{ dynamic_cast<T*>(val) };
			if (castVal != NULL)
			{
				vec.push_back({key, castVal});
			}
		}

		return vec;
	}

	GameObjectManager();
	~GameObjectManager();

	void add(std::string name, VisibleGameObject* gameObject);

	void remove(std::string name);
	std::map<std::string, VisibleGameObject*>::const_iterator remove(std::map<std::string, VisibleGameObject*>::const_iterator itr);

	int getObjectCount() const;

	void display();

	VisibleGameObject* get(std::string name) const;

	void drawAll(sf::RenderWindow& renderWindow);

	void updateAll();

	void restartDelta() { clock.restart();  };
	void pauseDelta() { m_pauseTime = clock.restart().asSeconds(); };

private:

	//Ce serait bien de faire un système de layer, pour afficher des objets par dessus d'autres objets
	std::map<std::string, VisibleGameObject*> m_gameObjects;

	sf::Clock clock;
	float m_pauseTime{0};

	struct GameObjectDeallocator
	{
		void operator()(const std::pair<std::string, VisibleGameObject*>& p) const
		{
			delete p.second;
		}

	};


};

#endif

