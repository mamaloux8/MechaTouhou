#ifndef BOSS_H
#define BOSS_H

#include "Ennemy.h"
#include "DialogueSubject.h"

struct BossState
{
    int health;
    int score;

    std::vector<std::pair<std::pair<MathVector, MathVector>, sf::Time>> moveTimeline;
    std::vector<std::pair<BulletPattern*, sf::Time>> patternQueue;
};

class Boss :
    public Ennemy, public DialogueObserver
{
public:
    Boss(std::vector<BossState>, std::pair<std::string, std::vector<sf::IntRect>>*, float);
    ~Boss();
    std::vector<BossState>& getBossStates() { return m_bossStates; };

    virtual void onNotify(DialogueObserver::State state);
    void getOutOfBounds();
    
    void getHit();
    void die();

    void nextState();
    int getCurrentState() const { return m_currentState; };
    int getNbStates() const { return m_nbStates; };

    static Subject<Boss>& getBossSubject() { return m_bossSubject; };

    static void setDialogueSubject(DialogueSubject* sub) { m_dialogueSubject = sub; };
    static DialogueSubject* getDialogueSubject() { return m_dialogueSubject; };

private:
    std::vector<BossState> m_bossStates{};
    int m_currentState{0};
    int m_nbStates{};

    //L'objet sera deux sujets en même temps
    static Subject<Boss> m_bossSubject;
    static DialogueSubject* m_dialogueSubject;
};

#endif
