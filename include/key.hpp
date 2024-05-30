#ifndef __KEY_HPP__
#define __KEY_HPP__

#include "object.hpp"
#include "bump.hpp"

class VirtualKeyboard;

class Key : public Object
{
public:
  Key(Vec3 position, Vec3 dimension, std::string text, std::string soundPath)
  {
    this->position = position;
    this->dimension = dimension;
    sf::RectangleShape *rectangle = new sf::RectangleShape();
    rectangle->setSize(sf::Vector2f(dimension.x(), dimension.y()));
    rectangle->setPosition(position.x(), position.y());
    rectangle->setFillColor(sf::Color::White);
    m_shape = rectangle;

    std::cout << "Key " << text << " position: " << position.transpose() << std::endl;
    std::cout << "Key " << text << " dimension: " << dimension.transpose() << std::endl;

    this->m_text = new sf::Text();
    this->m_text->setString(text);
    this->m_text->setPosition(position.x() + dimension.x() / 2, position.y() + dimension.y() / 2);

    this->m_sound = new sf::Sound();
    this->m_buffer = new sf::SoundBuffer();
    m_buffer->loadFromFile(soundPath);
    this->m_sound->setBuffer(*m_buffer);
    m_sound->setVolume(100.f);

    m_color = sf::Color::White;
    m_colorColliding = sf::Color::Green;
    name = "Key";

    // type of density
    typedef enum _DensityType
    {
      HOMOGENOUS,
      LINEAR,
      GRAVITY,
    } DensityType;

    for (int i = 0; i < 8; i++)
      m_bumps.push_back(std::vector<Bump *>());

    DensityType densityType = HOMOGENOUS;
    float density = 5;
    int n[3];
    switch (densityType)
    {
    case HOMOGENOUS:
      for (int dim_i = 0; dim_i < 3; dim_i++)
      {
        n[dim_i] = this->dimension[dim_i] / density;
        std::cout << "n[" << dim_i << "] = " << n[dim_i] << std::endl;
      }

      int blockDelta[3];
      for (int x_i = 0; x_i < n[0]; x_i++)
      {
        blockDelta[0] = (x_i > n[0] / 2) ? 0 : 4;
        for (int y_i = 0; y_i < n[1]; y_i++)
        {
          blockDelta[1] = (y_i > n[1] / 2) ? 0 : 2;
          for (int z_i = 0; z_i < n[2]; z_i++)
          {
            blockDelta[2] = (z_i > n[2] / 2) ? 0 : 1;
            int block_i = blockDelta[0] + blockDelta[1] + blockDelta[2];
            Vec3 p = {this->position.x() + x_i * density, this->position.y() + y_i * density, this->position.z() + z_i * density};
            Vec3 d = {5, 5, 5};
            m_bumps[block_i].push_back(new Bump(m_vkey, p, d));

            // std::cout << "Bump " << m_bumps[block_i].back()->name << " position: " << p.transpose() << " in block " << block_i << std::endl;
          }
        }
      }
      break;
    case LINEAR:
      // dn+1 = dn+d;
      // dn = n*d + d0;
      // dn = D;
      // n = (D-d0)/d;
      for (int dim_i = 0; dim_i < 3; dim_i++)
      {
        n[dim_i] = this->dimension[dim_i] / 2 / density;
      }
      int *spacings[3];
      // for(int dim_i=0;dim_i<3;dim_i++)
      // {
      // for(int i =n[dim_i]/2; i<n[dim_i];i++)
      // {
      //   px[i]=this->position[]
      // }
      // }
      break;

    default:
      break;
    }

    for (int block_i = 0; block_i < 8; block_i++)
    {
    }
  }
  ~Key();

  void collide()
  {
    // when the finger is inside the key
    // check collision with inside bumps
    // for each colliding object, check in wich of the 8 sub cube it is
    for (auto obj : collidingObjects)
    {
      // std::cout << "Key " << name << " colliding with " << obj->name << " at " << obj->position.transpose() << std::endl;
      Vec3 pos = obj->position;
      int block_i = 0;
      if (pos.x() - this->position.x() > this->dimension.x() / 2)
        block_i += 4;
      if (pos.y() - this->position.y() > this->dimension.y() / 2)
        block_i += 2;
      if (pos.z() - this->position.z() > this->dimension.z() / 2)
        block_i += 1;

      // std::cout << "Key " << name << " colliding with " << obj->name << " in block " << block_i << std::endl;
      // std::cout << "There is " << m_bumps[block_i].size() << " bumps in this block" << std::endl;
      for (int bi = 0; bi < 8; bi++)
      {
        for (auto bump : m_bumps[bi])
        {
          //std::cout << "Checking bump " << bump->name << " at " << bump->position.transpose() << " dimension " << bump->dimension.transpose() << std::endl;
          bump->interact(*obj,100);
            // std::cout << "Key " << name << " colliding with " << obj->name << " in block " << bi << std::endl;
        }
      }
    }
  }

  void justCollide()
  {
    m_sound->play();
    // std::cout << name << ": Key collided with {";
    // for (int i = 0; i < collidingObjects.size(); i++)
    // {
    //   std::cout << collidingObjects[i]->name;
    //   if (i < collidingObjects.size() - 1)
    //     std::cout << ", ";
    // }
    // std::cout << "}" << std::endl;
  };

private:
  sf::Text *m_text;
  sf::SoundBuffer *m_buffer;
  sf::Sound *m_sound;
  std::vector<std::vector<Bump *>> m_bumps;
  VirtualKeyboard *m_vkey;
  static int id;
};
int Key::id = 0;

#endif /* __KEY_HPP__ */
