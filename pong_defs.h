/* --------------------------------------------------------
 *    File: pong_defs.h
 *  Author: Tayler Rae Bachmann
 *   Class: COP 2006 CRN 13969
 * Purpose: definitions for pong game
 * -------------------------------------------------------- */
#include <SFML/Graphics.hpp>

#ifndef PONG_PADDLE_PONG_DEFS_H
#define PONG_PADDLE_PONG_DEFS_H

// Global Constants
// --------------------------------------------------------

// window properties
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const sf::Color WINDOW_COLOR = sf::Color::White;

// drawing properties
const float FRAME_RATE = (1.0/30.0) * 1000.0;       // FPS in ms
const sf::Color BALL_COLOR = sf::Color::Blue;
const float BALL_RADIUS = 10.0;

// wall constants
const float WALL_THICKNEESS = 20.0;
const sf::Color WALL_COLOR = sf::Color::Black;

// paddle constants
const float PADDLE_HEIGHT = 80.0;
const float PADDLE_THICKNESS = 10.0;
const sf::Color PADDLE_COLOR = sf::Color::Magenta;
const float PADDLE_SPEED = PADDLE_HEIGHT / 10.0 / 1000.0;


// These are just for fun
// speed that the can accelerate at to span window in
// n-seconds (in ms) broken up for each frame
const float SPEED_TIME = (3.0 * 1000.0) * 30.0;     //
const float BALL_SPEED_X = BALL_RADIUS * 10.0 / 1000.0;    // speed horizontally
const float BALL_SPEED_Y = BALL_RADIUS * 8.5 / 1000.0;   // speed  vertically


// Type definitions
// --------------------------------------------------------

// <direction enum here>
enum direction {Exit = -1, None = 0, Left = 1, Up = 2, Right = 3, Down = 4, Start};

// ball properties
struct Ball {
    float radius;
    float coordinateX;
    float coordinateY;
    float velocityX;
    float velocityY;
    sf::Color color;
};

// wall properties
struct Block {
    float left;
    float top;
    float width;
    float height;
    sf::Color color;
    sf::RectangleShape rectangle;
};

// paddle properties
struct MovingBlock {
    Block paddleProperties;
    float paddleXVelocity;
    float paddleYVelocity;
};

// border properties
struct Borders {
    Block leftWall;
    Block topWall;
    Block rightWall;
    Block bottomWall;
};

#endif //PONG_PADDLE_PONG_DEFS_H
