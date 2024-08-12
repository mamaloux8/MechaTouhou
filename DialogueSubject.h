#ifndef DIALOGUE_SUBJECT_H
#define DIALOGUE_SUBJECT_H

#include <vector>
#include <algorithm>
#include "DialogueObserver.h"

class DialogueSubject
{
public:
	DialogueSubject() = default;
	~DialogueSubject() = default;

	void addObserver(DialogueObserver* observer)
	{
		m_observers.push_back(observer);
	}

	void removeObserver(DialogueObserver* observer)
	{
		std::vector<DialogueObserver*>::iterator itr{ std::find(m_observers.begin(), m_observers.end(), observer) };
		if(itr != m_observers.end())
			m_observers.erase(itr);
	}

	void notify(DialogueObserver::State state)
	{
		for (const auto& obs : m_observers)
		{
			obs->onNotify(state);
		}
	}

private:
	std::vector<DialogueObserver*> m_observers{};
	int m_numObservers;
};

#endif

