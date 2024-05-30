#ifndef __BUMP_HPP__
#define __KEY_HPP__

#include "object.hpp"

class VirtualKeyboard;
class Bump : public Object
{
public:
  Bump(VirtualKeyboard *vkey, Vec3 position, Vec3 dimension)
  {
    this->position = position;
    this->dimension = dimension;
    sf::CircleShape *circle = new sf::CircleShape();
    circle->setRadius(dimension.x());
    circle->setPosition(position.x(), position.y());
    circle->setFillColor(sf::Color::Red);
    m_shape = circle;
    m_color = sf::Color::Red;
    m_colorColliding = sf::Color::Yellow;
    name = "Bump" + std::to_string(id++);
    m_vkey = vkey;
  }
  ~Bump();

  void justCollide(){
    std::cout << name << ": Bump just collided" << std::endl;

  };

private:
VirtualKeyboard *m_vkey;
static int id;
};
int Bump::id = 0;

#endif /* __KEY_HPP__ */
