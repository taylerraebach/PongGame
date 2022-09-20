/* --------------------------------------------------------
 *    File: pong_game.cpp
 *  Author: Tayler Rae Bachmann
 *   Class: COP 2006 CRN 13969
 * Purpose: incorporate an AI paddle to complete the overall game
 * -------------------------------------------------------- */

#include "pong_defs.h"


// Function declarations (prototypes)
// --------------------------------------------------------
void setup(Ball &theBall, Borders &theWalls, MovingBlock &playerPaddle, MovingBlock &AIPaddle);
direction processInput() ;
bool update(direction &input, Ball &ball, float delta, Borders &wall, MovingBlock &playerPaddle, bool &started,
            bool gameIsOver, MovingBlock &AIPaddle);
void render(sf::RenderWindow &window, Ball &ball, float delta, Borders &wall, MovingBlock &playerPaddle, MovingBlock &AIPaddle);
bool checkCollision(Ball &ball, Block &wall);
bool checkBlockCollision (Block &moving, Block &stationary);
bool overallCollisionCheck (Ball &ball, MovingBlock &playerPaddle, Borders &wall, MovingBlock &AIPaddle);

/**
 * The main application
 * @return OS status message (0=Success)
 */
int main() {

    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong Game");
    window.clear(WINDOW_COLOR);

    Ball theBall;
    Borders theWalls;
    MovingBlock playerPaddle;
    MovingBlock AIPaddle;
    setup(theBall, theWalls, playerPaddle, AIPaddle);

    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;

    bool started = false;
    bool gameOver = false;
    while (!gameOver){

        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;

        // process events
        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }

        // Process user input
        // ------------------------------------------------
        direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;

        // Process Updates
        // ------------------------------------------------
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time

            gameOver = update(userInput, theBall, delta, theWalls, playerPaddle, started, gameOver, AIPaddle);

            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }

        // Render the window
        // ------------------------------------------------
        render(window, theBall, delta, theWalls, playerPaddle, AIPaddle);

    } // end main game loop

    // game ending, close the graphics window
    window.close();

    return 0;   // return success to the OS
} // end main


/**
 * initialization code for game objects, i.e. ball, wall, and paddles
 * @param theBall - the properties of the ball
 * @param theWall - the properties of the walls
 * @param playerPaddle - the properties of the player's paddle
 * @param AIPaddle - the properties of the AI paddle
 */
void setup(Ball &theBall, Borders &theWalls, MovingBlock &playerPaddle, MovingBlock &AIPaddle) {

    // wall properties
    theWalls.leftWall.left = 0.0;
    theWalls.leftWall.top = 0.0;
    theWalls.leftWall.width = WALL_THICKNEESS;
    theWalls.leftWall.height = WINDOW_HEIGHT;
    theWalls.leftWall.rectangle.setSize(sf::Vector2(theWalls.leftWall.width, theWalls.leftWall.height));
    theWalls.leftWall.rectangle.setPosition(theWalls.leftWall.left, theWalls.leftWall.top);
    theWalls.leftWall.rectangle.setFillColor(WALL_COLOR);

    theWalls.topWall.left = 0.0;
    theWalls.topWall.top = 0.0;
    theWalls.topWall.width = WINDOW_WIDTH;
    theWalls.topWall.height = WALL_THICKNEESS;
    theWalls.topWall.rectangle.setSize(sf::Vector2(theWalls.topWall.width, theWalls.topWall.height));
    theWalls.topWall.rectangle.setPosition(theWalls.topWall.left, theWalls.topWall.top);
    theWalls.topWall.rectangle.setFillColor(WALL_COLOR);

    theWalls.rightWall.left = WINDOW_WIDTH - WALL_THICKNEESS;
    theWalls.rightWall.top = 0.0;
    theWalls.rightWall.width = WALL_THICKNEESS;
    theWalls.rightWall.height = WINDOW_HEIGHT;
    theWalls.rightWall.rectangle.setSize(sf::Vector2(theWalls.rightWall.width, theWalls.rightWall.height));
    theWalls.rightWall.rectangle.setPosition(theWalls.rightWall.left, theWalls.rightWall.top);
    theWalls.rightWall.rectangle.setFillColor(WALL_COLOR);

    theWalls.bottomWall.left = 0.0;
    theWalls.bottomWall.top = WINDOW_HEIGHT - WALL_THICKNEESS;
    theWalls.bottomWall.width = WINDOW_WIDTH;
    theWalls.bottomWall.height = WALL_THICKNEESS;
    theWalls.bottomWall.rectangle.setSize(sf::Vector2(theWalls.bottomWall.width, theWalls.bottomWall.height));
    theWalls.bottomWall.rectangle.setPosition(theWalls.bottomWall.left, theWalls.bottomWall.top);
    theWalls.bottomWall.rectangle.setFillColor(WALL_COLOR);

    // player paddle properties
    playerPaddle.paddleProperties.left = WALL_THICKNEESS;
    playerPaddle.paddleProperties.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0;
    playerPaddle.paddleProperties.width = PADDLE_THICKNESS;
    playerPaddle.paddleProperties.height = PADDLE_HEIGHT;
    playerPaddle.paddleProperties.color = PADDLE_COLOR;
    playerPaddle.paddleProperties.rectangle.setSize(sf::Vector2f(playerPaddle.paddleProperties.width, playerPaddle.paddleProperties.height));
    playerPaddle.paddleProperties.rectangle.setFillColor(PADDLE_COLOR);
    playerPaddle.paddleXVelocity = 0.0;
    playerPaddle.paddleYVelocity = 0.0;

    // AI paddle properties
    AIPaddle.paddleProperties.left = theWalls.rightWall.left - PADDLE_THICKNESS;
    AIPaddle.paddleProperties.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0;
    AIPaddle.paddleProperties.width = PADDLE_THICKNESS;
    AIPaddle.paddleProperties.height = PADDLE_HEIGHT;
    AIPaddle.paddleProperties.color = PADDLE_COLOR;
    AIPaddle.paddleProperties.rectangle.setSize(sf::Vector2f(playerPaddle.paddleProperties.width, playerPaddle.paddleProperties.height));
    AIPaddle.paddleProperties.rectangle.setFillColor(PADDLE_COLOR);
    AIPaddle.paddleXVelocity = 0.0;
    AIPaddle.paddleYVelocity = 0.0;

    // ball properties
    theBall.radius = BALL_RADIUS;
    theBall.coordinateX = playerPaddle.paddleProperties.left + playerPaddle.paddleProperties.width + theBall.radius + 1;
    theBall.coordinateY = playerPaddle.paddleProperties.top + (playerPaddle.paddleProperties.height / 2.0);
    theBall.velocityX = 0.0;
    theBall.velocityY = 0.0;
    theBall.color = BALL_COLOR;

} // end setup


/**
 * convert user keyboard input into recognized integer values
 * for left=1/up=2/right=3/down=4
 * @return int - user input (default no-input=0, quit=-1)
 */
direction processInput() {
    direction input = None;  // no input

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        input = Exit;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        input = Start;
    }

    return input;
} // end getUserInput


/**
 * update the state of game objects
 * @param input - user keyboard input
 * @param ball  - update ball position and speed
 * @param delta - current frame time
 * @param wall - properties of the border walls
 * @param playerPaddle - update player's paddle position and speed
 * @param started - if the game has started or not
 * @param gameIsOver - if the ball has gone past the player's paddle and hit the left wall, ending the game
 * @param AIPaddle - update AI paddle position and speed
 */
bool update(direction &input, Ball &ball, float delta, Borders &wall, MovingBlock &playerPaddle, bool &started,
            bool gameIsOver, MovingBlock &AIPaddle) {

    gameIsOver = false;

    // adjust velocity directions for user input
    // ------------------------------------------------
    if (input) {
        switch (input) {
            case Up: // Up
                playerPaddle.paddleYVelocity -= PADDLE_SPEED;
                break;
            case Down: // Down
                playerPaddle.paddleYVelocity += PADDLE_SPEED;
                break;
            case Start: // Start
                if (!started){
                    ball.velocityX = BALL_SPEED_X;
                    ball.velocityY = BALL_SPEED_Y;
                    if ((int(delta * 10) & 1) % 2){
                        ball.velocityY *= 1; // ball goes down if odd
                    }
                    started = true;
                }
        }
        // clear input
        input = None;
    }

    //adjust the location of the player's paddle for speed * time
    playerPaddle.paddleProperties.top += playerPaddle.paddleYVelocity * delta;
    AIPaddle.paddleProperties.top += AIPaddle.paddleYVelocity * delta;

    if (started){
        // adjust the location of the ball for speed * time
        ball.coordinateX += ball.velocityX * delta;
        ball.coordinateY += ball.velocityY * delta;
        if (ball.coordinateY < (AIPaddle.paddleProperties.top + AIPaddle.paddleProperties.height / 2)){
            AIPaddle.paddleYVelocity -= PADDLE_SPEED;
        } else if (ball.coordinateY > (AIPaddle.paddleProperties.top + AIPaddle.paddleProperties.height / 2)){
            AIPaddle.paddleYVelocity += PADDLE_SPEED;
        }
    } else{
        ball.coordinateX = playerPaddle.paddleProperties.left + playerPaddle.paddleProperties.width + ball.radius + 1;
        ball.coordinateY = playerPaddle.paddleProperties.top + (playerPaddle.paddleProperties.height / 2.0);
    }

    gameIsOver = overallCollisionCheck(ball, playerPaddle, wall, AIPaddle);

    return gameIsOver;

} // end update


/**
 * draw the ball on the graphics window
 * @param window - handle to open graphics window
 * @param ball   - structure variable with properties for the ball
 * @param delta  - amount of frame time plus lag (in ms)
 * @param wall - structure variable with properties for the walls
 * @param playerPaddle - structure variable with properties for the player's paddle
 * @param AIPaddle - structure variable with properties for the AI paddle
 */
void render(sf::RenderWindow &window, Ball &ball, float delta, Borders &wall, MovingBlock &playerPaddle, MovingBlock &AIPaddle) {
    // Render drawing objects
    // ------------------------------------------------
    // clear the window with the background color
    window.clear(WINDOW_COLOR);

    // draw the ball
    // ------------------------------------------------
    sf::CircleShape circle;
    circle.setFillColor(BALL_COLOR);
    circle.setRadius(30.0);
    // set screen coordinates relative to the center of the circle
    circle.setOrigin(30.0, 30.0);
    // calculate current drawing location relative to speed and frame-time
    float xPosition;
    float yPosition;
    xPosition = ball.coordinateX + ball.velocityX * delta;
    yPosition = ball.coordinateY + ball.velocityY * delta;
    circle.setPosition(xPosition, yPosition);
    window.draw(circle);

    //draw top border wall
    // ------------------------------------------------
    sf::RectangleShape topRectangle;
    topRectangle.setFillColor(WALL_COLOR);
    topRectangle.setSize(sf::Vector2f(WINDOW_WIDTH, WALL_THICKNEESS));
    topRectangle.setPosition(0.0, 0.0);
    window.draw(topRectangle);

    //draw right border wall
    // ------------------------------------------------
    sf::RectangleShape rightRectangle;
    rightRectangle.setFillColor(WALL_COLOR);
    rightRectangle.setSize(sf::Vector2f(WALL_THICKNEESS, WINDOW_HEIGHT));
    rightRectangle.setPosition(WINDOW_WIDTH - WALL_THICKNEESS, 0.0);
    window.draw(rightRectangle);

    //draw bottom border wall
    // ------------------------------------------------
    sf::RectangleShape bottomRectangle;
    bottomRectangle.setFillColor(WALL_COLOR);
    bottomRectangle.setSize(sf::Vector2f(WINDOW_WIDTH, WALL_THICKNEESS));
    bottomRectangle.setPosition(0.0, WINDOW_HEIGHT - WALL_THICKNEESS);
    window.draw(bottomRectangle);

    //draw left border wall
    // ------------------------------------------------
    sf::RectangleShape leftRectangle;
    leftRectangle.setFillColor(WALL_COLOR);
    leftRectangle.setSize(sf::Vector2f(WALL_THICKNEESS, WINDOW_HEIGHT));
    leftRectangle.setPosition(0.0, 0.0);
    window.draw(leftRectangle);

    //draw player's paddle
    // ------------------------------------------------
    sf::RectangleShape playersPaddle;
    float playerPaddleXPosition;
    float playerPaddleYPosition;
    playerPaddleXPosition = playerPaddle.paddleProperties.left + playerPaddle.paddleXVelocity * delta;
    playerPaddleYPosition = playerPaddle.paddleProperties.top + playerPaddle.paddleYVelocity * delta;
    playersPaddle.setFillColor(PADDLE_COLOR);
    playersPaddle.setSize(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT));
    playersPaddle.setPosition(playerPaddleXPosition, playerPaddleYPosition);
    window.draw(playersPaddle);

    //draw AI paddle
    // ------------------------------------------------
    sf::RectangleShape AIsPaddle;
    float AIPaddleXPosition;
    float AIPaddleYPosition;
    AIPaddleXPosition = AIPaddle.paddleProperties.left + AIPaddle.paddleXVelocity * delta;
    AIPaddleYPosition = AIPaddle.paddleProperties.top + AIPaddle.paddleYVelocity * delta;
    AIsPaddle.setFillColor(PADDLE_COLOR);
    AIsPaddle.setSize(sf::Vector2f(PADDLE_THICKNESS, PADDLE_HEIGHT));
    AIsPaddle.setPosition(AIPaddleXPosition, AIPaddleYPosition);
    window.draw(AIsPaddle);


    // show the new window
    // ------------------------------------------------
    window.display();

} // end render



/**
 * check for collisions between the ball and the border walls
 * @param ball   - structure variable with properties for the ball
 * @param wall  - structure variable with properties for the wall
 * @param collision  - whether the ball collided with the wall or not
 */
bool checkCollision(Ball &ball, Block &wall){

    bool collision;
    collision = false;

    // variables for the ball's sides
    // ------------------------------------------------
    float ballLeft;
    float ballTop;
    float ballRight;
    float ballBottom;

    ballLeft = ball.coordinateX - ball.radius;
    ballTop = ball.coordinateY - ball.radius;
    ballRight = ball.coordinateX + ball.radius;
    ballBottom = ball.coordinateY + ball.radius;

    // variables for the wall's sides
    // ------------------------------------------------
    float wallLeft;
    float wallTop;
    float wallRight;
    float wallBottom;

    wallLeft = wall.left;
    wallTop = wall.top;
    wallRight = wall.left + wall.width;
    wallBottom = wall.top + wall.height;

    // if statement to detect if there is a collision
    // ------------------------------------------------
    if((ballLeft < wallRight) && (ballRight > wallLeft)
    && (ballTop < wallBottom) && (ballBottom > wallTop)){
        collision = true;
    }

    return collision;

} // end checkCollision



/**
 * check for collisions between the paddle and borders
 * @param moving   - structure variable with properties for the paddles
 * @param stationary  - structure variable with properties for the border walls
 */
bool checkBlockCollision(Block &moving, Block &stationary){

    bool collision;
    collision = false;

    // variables for the paddle's sides
    // ------------------------------------------------
    float movingLeft;
    float movingTop;
    float movingRight;
    float movingBottom;

    movingLeft = moving.left;
    movingTop = moving.top;
    movingRight = moving.left + moving.width;
    movingBottom = moving.top + moving.height;

    // variables for the wall's sides
    // ------------------------------------------------
    float stationaryLeft;
    float stationaryTop;
    float stationaryRight;
    float stationaryBottom;

    stationaryLeft = stationary.left;
    stationaryTop = stationary.top;
    stationaryRight = stationary.left + stationary.width;
    stationaryBottom = stationary.top + stationary.height;

    // if statement to detect if there is a collision
    // ------------------------------------------------
    if ((movingLeft < stationaryRight) && (movingRight > stationaryLeft)
    && (movingTop < stationaryBottom) && (movingBottom > stationaryTop)) {
        collision = true;
    }

    return collision;

} // end checkBlockCollision

/**
 * check for collisions between the ball, the border walls, and the paddles
 * @param ball   - structure variable with properties for the ball
 * @param moving   - structure variable with properties for the paddles
 * @param stationary  - structure variable with properties for the border walls
 * @param collision  - whether the ball collided with the wall or not
 */
bool overallCollisionCheck (Ball &ball, MovingBlock &playerPaddle, Borders &wall, MovingBlock &AIPaddle){

    bool gameIsOver;
    gameIsOver = false;

    // check collision between player paddle and ball
    // ------------------------------------------------
    if (checkCollision(ball, playerPaddle.paddleProperties) == true){
        ball.velocityX *= -1;
        ball.coordinateX = playerPaddle.paddleProperties.left + playerPaddle.paddleProperties.width + ball.radius + 1;
        if ((ball.velocityY > 0) && (playerPaddle.paddleYVelocity > 0)){
            ball.velocityY *= 2.0;
        } else if ((ball.velocityY < 0) && (playerPaddle.paddleYVelocity < 0)) {
            ball.velocityY *= 2.0;
        } else if ((ball.velocityY > 0) && (playerPaddle.paddleYVelocity < 0)){
            ball.velocityY *= 0.5;
        } else if ((ball.velocityY < 0) && (playerPaddle.paddleYVelocity > 0)){
            ball.velocityY *= 0.5;
        }
    } else if (checkCollision(ball, AIPaddle.paddleProperties) == true){
        ball.velocityX *= -1;
        ball.coordinateX = AIPaddle.paddleProperties.left - ball.radius - 1;
        if ((ball.velocityY > 0) && (AIPaddle.paddleYVelocity > 0)){
            ball.velocityY *= 2.0;
        } else if ((ball.velocityY < 0) && (AIPaddle.paddleYVelocity < 0)) {
            ball.velocityY *= 2.0;
        } else if ((ball.velocityY > 0) && (AIPaddle.paddleYVelocity < 0)){
            ball.velocityY *= 0.5;
        } else if ((ball.velocityY < 0) && (AIPaddle.paddleYVelocity > 0)){
            ball.velocityY *= 0.5;
        }
    } else if(checkCollision(ball, wall.leftWall) == true){
        ball.velocityX *= -1;
        ball.coordinateX = wall.leftWall.left + wall.leftWall.width + ball.radius + 1;
        gameIsOver = true;
    } else if (checkCollision(ball, wall.rightWall) == true){
        ball.velocityX *= -1;
        ball.coordinateX = wall.rightWall.left - ball.radius - 1;
        gameIsOver = true;
    }

    // check collision between ball and top and bottom walls
    // ------------------------------------------------
    if (checkCollision(ball, wall.topWall) == true){
        ball.velocityY *= -1;
        ball.coordinateY = wall.topWall.top + wall.topWall.height + ball.radius + 1;
    } else if (checkCollision(ball, wall.bottomWall) == true){
        ball.velocityY *= -1;
        ball.coordinateY = wall.bottomWall.top - ball.radius - 1;
    }

    //check collision between player paddle and border walls
    // ------------------------------------------------
    if (checkBlockCollision(playerPaddle.paddleProperties, wall.topWall) == true){
        playerPaddle.paddleYVelocity = 0.0;
        playerPaddle.paddleProperties.top = wall.topWall.top + wall.topWall.height + 1;
    } else if (checkBlockCollision(playerPaddle.paddleProperties, wall.bottomWall) == true){
        playerPaddle.paddleYVelocity = 0.0;
        playerPaddle.paddleProperties.top = wall.bottomWall.top - playerPaddle.paddleProperties.height - 1;
    }

    //check collision between ai paddle and border walls
    // ------------------------------------------------
    if (checkBlockCollision(AIPaddle.paddleProperties, wall.topWall) == true){
        AIPaddle.paddleYVelocity = 0.0;
        AIPaddle.paddleProperties.top = wall.topWall.top + wall.topWall.height + 1;
    } else if (checkBlockCollision(AIPaddle.paddleProperties, wall.bottomWall) == true){
        AIPaddle.paddleYVelocity = 0.0;
        AIPaddle.paddleProperties.top = wall.bottomWall.top - AIPaddle.paddleProperties.height - 1;
    }



    return gameIsOver;
} // end overallCollisionCheck
