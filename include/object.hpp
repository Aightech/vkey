#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <eigen3/Eigen/Dense>
#include <vector>

// define Vec3 as Eigen::Vector3f
typedef Eigen::Vector3f Vec3;

class Object
{
public:
    Object()
    {
        position = Vec3(0, 0, 0);
        dimension = Vec3(10, 10, 10);
    }
    ~Object(){};

    virtual void justCollide()
    {
        std::cout << name << ": Object just collided with " << collidingObjects.size() << " objects: {";
        for (int i = 0; i < collidingObjects.size(); i++)
        {
            std::cout << collidingObjects[i]->name;
            if (i < collidingObjects.size() - 1)
                std::cout << ", ";
        }
        std::cout << "}" << std::endl;
    }

    virtual void collide()
    {
        // std::cout << name << ": Object colliding with " << collidingObjects.size() << " objects: {";
        // for (int i = 0; i < collidingObjects.size(); i++)
        // {
        //     std::cout << collidingObjects[i]->name;
        //     if (i < collidingObjects.size() - 1)
        //         std::cout << ", ";
        // }
        // std::cout << "}" << std::endl;
    }

    void draw(sf::RenderWindow &window)
    {
        if (isColliding)
            m_shape->setFillColor(m_colorColliding);
        else
            m_shape->setFillColor(m_color);
        m_shape->setPosition(position.x(), position.y());
        window.draw(*m_shape);
    }

    bool interact(Object &object, float intensity = 500.)
    {
        // check if the object is in the 3D dimension
        if (object.position.x() > position.x() && object.position.x() < position.x() + dimension.x() &&
            object.position.y() > position.y() && object.position.y() < position.y() + dimension.y() &&
            object.position.z() > position.z() && object.position.z() < position.z() + dimension.z())
        {
            isColliding = true;
            object.isColliding = true;
            // check if the object is not already in the colliding objects
            if (std::find(collidingObjects.begin(), collidingObjects.end(), &object) == collidingObjects.end())
            { // add object to the colliding objects
                collidingObjects.push_back(&object);
                object.collidingObjects.push_back(this);
                justCollided = true;
                object.justCollided = true;
                collisionIntensity = intensity;
                object.collisionIntensity = intensity;
                
            }
            return true;
        }
        else
        {
            // ensure the object is not in the colliding objects
            collidingObjects.erase(std::remove(collidingObjects.begin(), collidingObjects.end(), &object), collidingObjects.end());
            object.collidingObjects.erase(std::remove(object.collidingObjects.begin(), object.collidingObjects.end(), this), object.collidingObjects.end());
        }
        return false;
    }

    void resolveCollisions()
    {
        if (collidingObjects.size() == 0)
        {
            isColliding = false;
        }
        else if (justCollided)
        {
            justCollide();
            justCollided = false;
        }
        else
            collide();
    }

    Vec3 position;
    Vec3 dimension;
    bool justCollided = false;
    bool isColliding = false;
    float collisionIntensity = 500.;
    int id = 0;

    std::vector<Object *> collidingObjects;
    std::string name;

protected:
    sf::Shape *m_shape;
    sf::Color m_color;
    sf::Color m_colorColliding;
};

#endif /* __OBJECT_HPP__ */
