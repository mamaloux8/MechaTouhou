#ifndef CHANGE_FLASH_H
#define CHANGE_FLASH_H

#include <array>
#include "VisibleGameObject.h"

class ChangeFlash:
    public VisibleGameObject
{
public:
    ChangeFlash();

    ~ChangeFlash() = default;

    void update(float elapsedTime);
    void draw(sf::RenderWindow& rw);

    const std::string& getName() const { return m_name; };
    void setName(std::string& str) { m_name = str; };

    void play() { m_animClock.restart(); m_play = true; };

private:
    //à rajouter à tous les gameobjects
    std::string m_name{ "ZChangeFlash" };
    bool m_play{ false };

    static constexpr int ANIM_NB_STEPS{ 2 };
    const std::array<sf::IntRect, ANIM_NB_STEPS> m_flashRects{ sf::IntRect{ 0,0,225,314 }, { 225,0,225,314} };
    int m_animStep{ 0 };

    sf::Clock m_animClock{};
};

#endif

