#ifndef DIALOGUE_MANAGER_H
#define DIALOGUE_MANAGER_H


#include <vector>
#include <array>

#include "Observer.h"
#include "DialogueSubject.h"

#include "Character.h"

#include "EnnemyTimeline.h"
#include "Boss.h"


class DialogueManager 
	: public Observer<EnnemyTimeline>
{

public:

	DialogueManager();

	virtual void onNotify(const EnnemyTimeline*, Observer<EnnemyTimeline>::State);

	void beginDialogue(std::vector<std::pair<Character, std::wstring>>, std::vector<std::pair<Character, CharacterPosition>>);

	void draw(sf::RenderTexture&);

	bool getUpdateUi() const { return m_updateUI; };
	void setUpdateUi(bool update) { m_updateUI = update; };
	bool getHasBegun() const { return m_hasBegun; };

	void nextDialogue() { ++m_dialogueIndex; };

	DialogueSubject& getSubject() { return m_subject; };

private:
	const std::array<std::string, character_size> m_characterImages{ "res/img/KManProfile.png", "res/img/ShalomiumProfile.png" };

	//Faire un buffer plutôt que d'utiliser un bool ? C'est-à-dire mettre les sprites qu'on doit dessiner
	//dans un buffer et les envoyer à la window
	bool m_updateUI{ false };
	bool m_hasBegun{ false };

	std::vector<std::pair<Character, CharacterPosition>> m_characters{};
	std::vector<std::pair<Character, std::wstring>> m_dialogues{};
	int m_dialogueIndex{};

	sf::RectangleShape m_dialogueBox{};
	sf::Text m_dialogueText{};
	sf::Font m_font{};

	std::vector<sf::Texture*> m_characterTextures{};

	Character m_leftCharacter{};
	sf::Sprite m_leftCharacterSprite{};

	sf::Sprite m_rightCharacterSprite{};
	Character m_rightCharacter{};

	DialogueSubject m_subject{};
};

#endif