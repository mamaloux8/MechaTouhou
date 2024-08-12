#include "DialogueManager.h"
#include "Game.h"

DialogueManager::DialogueManager()
	: m_dialogueBox{ {Game::SCREEN_WIDTH * 0.75f, Game::SCREEN_HEIGHT / 8.f} }
{
	m_dialogueBox.setPosition({ Game::SCREEN_WIDTH / 2.f - m_dialogueBox.getLocalBounds().width/2.f, Game::SCREEN_HEIGHT - m_dialogueBox.getLocalBounds().height});
	m_dialogueBox.setFillColor({ 50,168,82 });

	
	m_font.loadFromFile("res/fonts/arial.ttf");

	m_dialogueText.setPosition(m_dialogueBox.getPosition().x, m_dialogueBox.getPosition().y);
	m_dialogueText.setFillColor(sf::Color::White);
	m_dialogueText.setFont(m_font);
}

void DialogueManager::onNotify(const EnnemyTimeline* timeline, Observer<EnnemyTimeline>::State state)
{
	if (state == Observer<EnnemyTimeline>::isEmpty)
		beginDialogue(timeline->getDialogues()[timeline->getDialogueIndex()].second, timeline->getDialogues()[timeline->getDialogueIndex()].first);
}

void DialogueManager::beginDialogue(std::vector<std::pair<Character, std::wstring>> dialogues, std::vector<std::pair<Character, CharacterPosition>> characters)
{
	m_dialogues = dialogues;
	m_characters = characters;

	for (auto character : m_characters)
	{
		sf::Texture* texture = Game::getTextureCache().getImage(m_characterImages[character.first]);

		//Utiliser m_characterTextures pour quand il y aura plus de deux personnages
		//m_characterTextures.push_back(texture);

		if (character.second == left)
		{
			m_leftCharacter = character.first;
			m_leftCharacterSprite.setTexture(*texture, true);
		}
		else if (character.second == right)
		{
			m_rightCharacter = character.first;
			m_rightCharacterSprite.setTexture(*texture, true);
		}
	}

	m_leftCharacterSprite.setScale(0.5f, 0.5f);
	m_leftCharacterSprite.setPosition(0.f, m_dialogueBox.getPosition().y - m_leftCharacterSprite.getLocalBounds().height * 0.5f);

	m_rightCharacterSprite.setScale(0.5f, 0.5f);
	m_rightCharacterSprite.setPosition(Game::SCREEN_WIDTH - m_rightCharacterSprite.getLocalBounds().width * 0.5f, m_dialogueBox.getPosition().y - m_rightCharacterSprite.getLocalBounds().height * 0.5f);

	m_hasBegun = true;
	m_updateUI = true;

	m_subject.notify(DialogueObserver::hasBegun);
}

void DialogueManager::draw(sf::RenderTexture& rw)
{
	if (m_dialogueIndex < m_dialogues.size() && m_hasBegun)
	{
		//Draw characters
		if (m_dialogues[m_dialogueIndex].first == m_leftCharacter)
			rw.draw(m_leftCharacterSprite);
		else if (m_dialogues[m_dialogueIndex].first == m_rightCharacter)
			rw.draw(m_rightCharacterSprite);

		//Draw dialogue box
		rw.draw(m_dialogueBox);

		//Draw dialogue
		m_dialogueText.setString(m_dialogues[m_dialogueIndex].second);
		rw.draw(m_dialogueText);

		//Display
		rw.display();

		m_updateUI = false;
		return;
	}
	//The dialogue is finished
	else if (m_hasBegun)
	{
		m_subject.notify(DialogueObserver::hasEnded);

		m_dialogueIndex = 0;
		m_characterTextures = {};
		m_leftCharacterSprite = {};
		m_rightCharacterSprite = {};

		m_hasBegun = false;
	}

	m_updateUI = false;
}