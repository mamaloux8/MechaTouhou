#ifndef ENNEMY_TIMELINE_H
#define ENNEMY_TIMELINE_H	

#include <string>
#include "Boss.h"
#include "GameObjectManager.h"

#include "Character.h"

using Characters = std::vector<std::pair<Character, CharacterPosition>>;
using Dialogues = std::vector<std::pair<Character, std::wstring>>;

class EnnemyTimeline :
	public Observer<Boss>
{
public:
	EnnemyTimeline();
	EnnemyTimeline(std::vector<std::pair<Characters, Dialogues>>);
	~EnnemyTimeline();

	virtual void onNotify(const Boss*, Observer<Boss>::State);

	void update(GameObjectManager& gm);

	void addEnnemy(Ennemy&, sf::Time, MathVector, MathVector, MathVector);
	void addEnnemy(Ennemy&, sf::Time, MathVector, std::vector<std::pair<std::pair<MathVector, MathVector>, sf::Time>>&);
	void addEnnemy(Boss&, sf::Time, MathVector, DialogueSubject&);

	int getSize() { return m_timeline.size(); };

	const std::vector<std::pair<Characters, Dialogues>>& getDialogues() const { return m_dialogues; };
	size_t getDialogueIndex() const { return m_dialogueIndex; };

	Subject<EnnemyTimeline>& getSubject() { return m_subject; };

private:
	std::vector<std::pair<Ennemy*, sf::Time>> m_timeline{};

	std::vector<std::pair<Characters, Dialogues>> m_dialogues;
	size_t m_dialogueIndex;

	sf::Clock m_timer{};

	bool m_isEmpty{ false };

	int m_ennemyID{};

	Subject<EnnemyTimeline> m_subject{};
};

#endif

