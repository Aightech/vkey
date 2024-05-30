#ifndef __VKEY_HPP__
#define __VKEY_HPP__

#include "object.hpp"
#include "key.hpp"
#include "finger.hpp"
#include <LeapC.h>
#include <leapmotion.hpp>
#include <naptx.hpp>
#include <eigen3/Eigen/Dense>
#include <vector>
#include <strANSIseq.hpp>

class VirtualKeyboard : virtual public NaptX
{
public:
    VirtualKeyboard(int verbose = -1) : ESC::CLI(verbose, "Vkey"){};
    ~VirtualKeyboard(){};

    void generateGrid(int nbKey, Vec3 origin, Vec3 key_dim, Vec3 key_spacing, std::vector<std::string> sounds_paths)
    {
        Vec3 position = origin;
        for (int i = 0; i < nbKey; i++)
        {
            m_keys.push_back(new Key(position, key_dim, std::to_string(i), sounds_paths[i]));
            position += key_dim + key_spacing;
            m_keys[i]->name = "Key" + std::to_string(i);
        }
        for (int i = 0; i < 5; i++)
        {
            m_fingers.push_back(new Finger(this));
            m_fingers[i]->id = i;
            m_fingers[i]->name = "Finger" + std::to_string(i);
        }
    }

    void interactWithHand(LEAP_HAND &hand)
    {
        // loop over the fingers
        for (int f_i = 0; f_i < 5; f_i++)
        {
            LEAP_DIGIT *finger = &hand.digits[f_i];
            m_fingers[f_i]->position = leapToSFML(finger->distal.next_joint);
        }
        // loop over the keys
        for (int k_i = 0; k_i < m_keys.size(); k_i++)
        {
            Key *key = m_keys[k_i];
            // loop over the fingers
            for (int f_i = 0; f_i < 5; f_i++)
                key->interact(*m_fingers[f_i]);
        }

        // resolve the collisions
        for (int k_i = 0; k_i < m_keys.size(); k_i++)
            m_keys[k_i]->resolveCollisions();
        for (int f_i = 0; f_i < 5; f_i++)
            m_fingers[f_i]->resolveCollisions();
    };

    void draw(sf::RenderWindow &window)
    {
        w_width = window.getSize().x;
        w_height = window.getSize().y;
        for (int i = 0; i < m_keys.size(); i++)
            m_keys[i]->draw(window);
        for (int i = 0; i < 5; i++)
            m_fingers[i]->draw(window);
    }

    Vec3 leapToSFML(LEAP_VECTOR leapVector)
    {
        return Vec3(leapVector.x + w_width / 2, leapVector.z + w_height / 2, leapVector.y);
    };


private:
    std::vector<Key *> m_keys;
    std::vector<Finger *> m_fingers;
    int w_width = 800;
    int w_height = 600;
};

void Finger::justCollide()
{
    m_vkey->pulse(collisionIntensity, this->id);
    std::cout << "clock: " << std::to_string(clock()) <<  name << ": Finger collided f: " << collisionIntensity << " with {";
    for (int i = 0; i < collidingObjects.size(); i++)
    {
        std::cout << collidingObjects[i]->name;
        if (i < collidingObjects.size() - 1)
            std::cout << ", ";
    }
    std::cout << "}" << std::endl;
}

#endif /* __VKEY_HPP__ */
