#include "EnnemyTimeline.h"

EnnemyTimeline::EnnemyTimeline(std::vector<std::pair<Characters, Dialogues>> dialogues)
	: m_ennemyID{ 1 }
	, m_dialogues{dialogues}
{
}

EnnemyTimeline::EnnemyTimeline()
	: EnnemyTimeline({ {{},{}} })
{
}

EnnemyTimeline::~EnnemyTimeline()
{
}

void EnnemyTimeline::onNotify(const Boss* ennemy, Observer<Boss>::State state)
{
	if (state == Observer<Boss>::isDead)
	{
		m_subject.notify(this, Observer<EnnemyTimeline>::isEmpty);
		++m_dialogueIndex;
	}
}

void EnnemyTimeline::addEnnemy(Ennemy& ennemy, sf::Time time, MathVector position, MathVector velocity, MathVector acceleration)
{
	Ennemy* e{ new Ennemy{ennemy} };
	e->setPosition(position.getX(), position.getY());

	std::vector<std::pair<std::pair<MathVector, MathVector>, sf::Time>> moveTimeline{};
	std::pair<std::pair<MathVector, MathVector>, sf::Time> value = std::pair{ std::pair{position, velocity}, sf::seconds(999.f) };
	moveTimeline.push_back(value);

	e->setMoveTimeline(moveTimeline);
	e->setVelocity(velocity);
	e->setAcceleration(acceleration);

	m_timeline.push_back({ e, time });
}

void EnnemyTimeline::addEnnemy(Boss& ennemy, sf::Time time, MathVector position, DialogueSubject& subject)
{
	Boss* e{ new Boss{ennemy} };
	e->setPosition(position.getX(), position.getY());

	e->setMoveTimeline(e->getBossStates()[0].moveTimeline);
	e->setVelocity(e->getBossStates()[0].moveTimeline.back().first.first);
	e->setAcceleration(e->getBossStates()[0].moveTimeline.back().first.second);

	//Register the boss as an observer of DialogueManager
	//subject.addObserver(e);
	ennemy.getDialogueSubject()->addObserver(e);

	m_timeline.push_back({ e, time });
}

void EnnemyTimeline::addEnnemy(Ennemy& ennemy, sf::Time time, MathVector position, std::vector<std::pair<std::pair<MathVector, MathVector>, sf::Time>>& timeline)
{
	Ennemy* e{ new Ennemy{ennemy} };
	e->setPosition(position.getX(), position.getY());
	e->setMoveTimeline(timeline);
	e->setVelocity(timeline.back().first.first);
	e->setAcceleration(timeline.back().first.second);

	m_timeline.push_back({ e, time });
}

void EnnemyTimeline::update(GameObjectManager& gm)
{
	if (m_isEmpty && m_timeline.size() <= 0)
		return;
	else if (m_timeline.size() <= 0)
	{
		//Le state sert à rien, je devrais faire un observer sans state
		m_subject.notify(this, Observer<EnnemyTimeline>::isEmpty);
		++m_dialogueIndex;
		m_isEmpty = true;
		return;
	}
	else
		m_isEmpty = false;

	if (m_timer.getElapsedTime() > m_timeline.back().second)
	{
		m_timer.restart();
		m_timeline.back().first->resetTimers();
		gm.add("Ennemy " + std::to_string(m_ennemyID), m_timeline.back().first);
		++m_ennemyID;
		m_timeline.pop_back();
	}
}