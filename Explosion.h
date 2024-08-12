#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <array>
#include "VisibleGameObject.h"

class Explosion :
    public VisibleGameObject
{
public:
    Explosion();

    ~Explosion() = default;

    void update(float elapsedTime);
    void draw(sf::RenderWindow& rw);

    const std::string& getName() const { return m_name; };
    void setName(std::string& str) { m_name = str; };

private:
    //à rajouter à tous les gameobjects
    std::string m_name{"Explosion"};

    static constexpr int EXPLOSION_ANIM_NB_STEPS{ 4 };
    const std::array<sf::IntRect, EXPLOSION_ANIM_NB_STEPS> m_explosionRects{sf::IntRect{ 0,0,180,165 }, { 180,0,180,165 }, { 0,165,180,165 }, { 180,165,180,165 } };
    int m_explosionAnimStep{ 0 };

    sf::Clock m_animClock{};
};

#endif

