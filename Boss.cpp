#include "Boss.h"
#include "Game.h"

Boss::Boss(std::vector<BossState> states, std::pair<std::string, std::vector<sf::IntRect>>* animator, float scale)
	: Ennemy(states[0].patternQueue, states[0].health, states[0].score, animator, scale)
{
	m_bossStates = states;
	m_nbStates = states.size();
}

Boss::~Boss()
{
	m_dialogueSubject->removeObserver(this);
}

void Boss::getHit()
{
	m_ennemySubject.notify(this, Observer<Ennemy>::isHit);
	Ennemy::getHit();
}

void Boss::getOutOfBounds()
{
	//Peut-être pas le moyen le plus optimisé puisque aller dans la fonction ne sert à rien
	return;
}

void Boss::die()
{
	Ennemy::die();
	if(m_currentState >= m_nbStates)
		m_bossSubject.notify(this, Observer<Boss>::isDead);
	nextState();
}

void Boss::nextState()
{
	//Delete all bullets
	Game::getBulletManager().clear();

	++m_currentState;

	if (m_currentState >= m_nbStates)
	{
		return;
	}

	//To display healthbar and the number of states left
	m_ennemySubject.notify(this, Observer<Ennemy>::isSpawn);

	m_health = m_bossStates[m_currentState].health;
	m_baseHealth = m_bossStates[m_currentState].health;
	m_score = m_bossStates[m_currentState].score;
	m_patternQueue = m_bossStates[m_currentState].patternQueue;
	m_moveTimeline = m_bossStates[m_currentState].moveTimeline;
}

void Boss::onNotify(DialogueObserver::State state) {
	if (state == DialogueObserver::hasEnded)
	{
		nextState();
		ServiceLocator::getAudio()->playSong("res/bgm/bossTheme.wav", true);
	}
};

Subject<Boss> Boss::m_bossSubject{};
DialogueSubject* Boss::m_dialogueSubject{};