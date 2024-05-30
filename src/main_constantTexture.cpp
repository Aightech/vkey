/**
 * @brief This progran generate a virtual keyboard. It uses :
 * - SFML for the graphical interface, event handling and sound
 * - leap motion for the hand tracking
 * - naptx for the finger hapic feedback
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "vkey.hpp"

#define W_WIDTH 800
#define W_HEIGHT 600


int main(int argc, char **argv)
{
  // Create the main window
  sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "Virtual Keyboard");

  // Create the virtual keyboard
  VirtualKeyboard vkey(3);

  // Create the leap motion controller
  OpenConnection();
  LEAP_HAND *hand;
  // fingertip shape
  std::vector<sf::CircleShape> fingertips;
  for (int i = 0; i < 5; i++)
  {
    sf::CircleShape fingertip(5);
    fingertip.setFillColor(sf::Color::Red);
    fingertips.push_back(fingertip);
  }

  vkey.open_connection("/dev/ttyACM0", 115200, O_RDWR | O_NOCTTY);

  // Create the keys
  int nbKey = 5;
  int width = 50;
  int spacing = 10;
  Vec3 origin(W_WIDTH / 2 - (nbKey * width + (nbKey - 1) * spacing) / 2, W_HEIGHT / 2, 200);
  Vec3 key_dim(width, width, width);
  Vec3 key_spacing(spacing, -width, -width);
  std::vector<std::string> sounds_paths;
  for (int i = 0; i < 5; i++)
  {
    sounds_paths.push_back("../sounds/key0" + std::to_string(i + 1) + ".ogg");
  }
  vkey.generateGrid(5, origin, key_dim, key_spacing, sounds_paths);

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

    // Get the hand position
    LEAP_TRACKING_EVENT *frame = GetFrame();
    if (frame)
    {
      hand = NULL;
      for (int i = 0; i < frame->nHands; i++)
      {
        if (frame->pHands[i].type == eLeapHandType_Right)
        {
          hand = &frame->pHands[i];
          break;
        }
      }
      if (hand)
        vkey.interactWithHand(*hand);
    }

    // Draw the virtual keyboard
    vkey.draw(window);
    // Display window
    window.display();
  }

  return 0;
}