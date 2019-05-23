#ifndef GAMEE_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "Paddle.hpp"
#include "Ball.hpp"

class Game {
    int width, height;
    int screen;
    sf::RenderWindow window;
    sf::Texture texture;
    sf::Font font;
    sf::Text startText;
    sf::Text exitText;
    sf::Text gameWinText;
    sf::Text gameOverText;
    sf::RectangleShape border;
    Paddle paddle;
    Paddle paddle2;
    Ball ball;
    int ballheld;
    int melives;
    int cpulives;
    int lvl;
    int score; /**stores time.. too lazy to rename now**/
    int fscore; /**this is final score T_T **/

    float aitimer;

    static const int paddleWidth = 20;
    static const int paddleHeight = 200;
    static const int paddleVel = 200;
    static const int ballRadius = 10;
    static const int ballInitVel = 500;
    static const int marginWidth = 10;
    static const int marginHeight = 50;
    static const int titleSize = 50;
    static const int bigTextSize = 100;

public:
    Game(int width, int height, std::string title);
    void handleEvents();
    void update(float dt);
    void draw();
    void drawInfo();
    void resetTextColors();

    void toInitState();

    int calculateFinalScore();

    void perfectAI(float); /**not really perfect(yet)**/

    bool isRunning();
    int hasCollided(sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f);

};

#endif
