#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <algorithm>
#include "Observer.h"

template <typename T>
class Subject
{
public:
	Subject() = default;
	~Subject() = default;

	void addObserver(Observer<T>* observer)
	{
		m_observers.push_back(observer);
	}

	void removeObserver(Observer<T>* observer)
	{
		typename std::vector<Observer<T>*>::iterator itr{ std::find(m_observers.begin(), m_observers.end(), observer) };
		m_observers.erase(itr);
	}

	void notify(const T* entity, Observer<T>::State state)
	{
		for (const auto& obs : m_observers)
		{
			obs->onNotify(entity, state);
		}
	}

private:
	std::vector<Observer<T>*> m_observers{};
	int m_numObservers;
};

#endif

