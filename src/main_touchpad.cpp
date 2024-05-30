

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "touchscreen.hpp"
#include "naptx.hpp"

#include "vkey.hpp"

#define W_WIDTH 800
#define W_HEIGHT 600

int main(int argc, char **argv)
{
  // // Create the main window
  std::string path = (argc > 1) ? argv[1] : "/dev/input/event7";
  cTouchScreen ts(1);
  ts.connect(path.c_str());
  int nb_mt = ts.max_mt();
  std::string clear_string = "\xd" + std::string(nb_mt * 15, ' ') + "\xd";

  NaptX naptx;
  naptx.open_connection("/dev/ttyACM0", 115200, O_RDWR | O_NOCTTY);

  sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "Virtual Keyboard");
  // for (;;)
  // {
  //   for (int i = 0; i < nb_mt; i++)
  //     std::cout << "(" << ts.pos_rel(i)[0] << " " << ts.pos_rel(i)[1] << ") ";
  //   std::cout << std::flush << clear_string;
  // }

  // Create bumps
  std::vector<sf::CircleShape> bumps;
  int nb_bumps = 20;
  for (int j = 0; j < nb_bumps; j++)
  {
    for (int i = 0; i < nb_bumps; i++)
    {
      sf::CircleShape bump(8);
      bump.setPosition(100 + i * 20, 100 + j * 20);
      bump.setFillColor(sf::Color::Blue);
      bumps.push_back(bump);
    }
  }

  // fingertip shape
  std::vector<sf::CircleShape> fingertips;
  std::vector<bool> is_touching;
  std::vector<bool> just_touched;
  std::vector<int> nb_collision;
  for (int i = 0; i < 5; i++)
  {
    sf::CircleShape fingertip(5);
    fingertip.setFillColor(sf::Color::Red);
    fingertips.push_back(fingertip);
    is_touching.push_back(false);
    just_touched.push_back(false);
    nb_collision.push_back(0);
  }

  // Start the game loop
  while (window.isOpen())
  {
    // Process events
    sf::Event event;
    while (window.pollEvent(event))
    {
      // Close window: exit
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // Clear screen
    window.clear();

    // set the position of the fingertips
    for (int i = 0; i < nb_mt; i++)
    {
      fingertips[i].setPosition(ts.pos_rel(i)[0] * W_WIDTH, ts.pos_rel(i)[1] * W_HEIGHT);
      window.draw(fingertips[i]);

      // check if the finger is touching a bump
      for (int j = 0; j < bumps.size(); j++)
      {
        if (bumps[j].getGlobalBounds().contains(fingertips[i].getPosition()))
        {
          nb_collision[i]++;
        }
      }
    }

    for (int i = 0; i < nb_mt; i++)
    {
      if (nb_collision[i] > 0)
      {
        if (!is_touching[i])
        {
          just_touched[i] = true;
        }
        is_touching[i] = true;

        if (just_touched[i])
        {
          naptx.pulse(500, i);
          std::cout << "Pulse " << i << std::endl;
          just_touched[i] = false;
        }
      }
      else
      {
        is_touching[i] = false;
        just_touched[i] = false;
      }
      nb_collision[i] = 0;
    }

    for (int i = 0; i < bumps.size(); i++)
    {
      window.draw(bumps[i]);
    }

    // Display window
    window.display();
  }

  return 0;
}