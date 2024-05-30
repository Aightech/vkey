#ifndef __FINGER_HPP__
#define __FINGER_HPP__
#include "object.hpp"

class VirtualKeyboard;

class Finger : public Object
{
public:
    Finger(VirtualKeyboard *vkey)
    {
        position = Vec3(0, 0, 0);
        dimension = Vec3(10, 10, 10);
        sf::CircleShape *circle = new sf::CircleShape();
        circle->setRadius(dimension.x());
        circle->setPosition(position.x(), position.y());
        circle->setFillColor(sf::Color::Red);
        m_shape = circle;
        m_color = sf::Color::Red;
        m_colorColliding = sf::Color::Yellow;
        name = "Finger";
        m_vkey = vkey;
    }
    ~Finger();
    void justCollide();

private:
    VirtualKeyboard *m_vkey;
};

#endif /* __FINGER_HPP__ */