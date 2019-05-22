#include "Game.hpp"
#include <iostream>
#include <sstream>

#define PI 3.1415926535

Game::Game(int width, int height, std::string title) :
    window(sf::VideoMode(width, height), title, sf::Style::Fullscreen),
    paddle(paddleWidth,paddleHeight),
    paddle2(paddleWidth, paddleHeight),
    ball(ballRadius)
{
    this->screen = 0;
    this->width = width = window.getSize().x;
    this->height = height = window.getSize().y;

    toInitState();

    if(!font.loadFromFile("Akronim-Regular.ttf"))
    {
        std::cout << "Couldn't load the font"<<std::endl;
    }
    if(!texture.loadFromFile("texture.png"))
    {
        std::cout << "Couldn't load the texture." << std::endl;
    }
    paddle.setTexture(&texture);
    paddle.setColor(sf::Color(127,127,255));

    paddle2.setTexture(&texture);
    paddle2.setColor(sf::Color(255,127,127));

    startText.setFont(font);
    exitText.setFont(font);
    gameOverText.setFont(font);
    gameWinText.setFont(font);

    startText.setCharacterSize(titleSize);
    startText.setFillColor(sf::Color::Green);
    startText.setString("START");
    exitText.setCharacterSize(titleSize);
    exitText.setString("EXIT");
    startText.setPosition(sf::Vector2f((width-startText.getLocalBounds().width)/2,
                                       height/2 - titleSize));
    exitText.setPosition(sf::Vector2f((width-exitText.getLocalBounds().width)/2,
                                      height/2));

    gameOverText.setCharacterSize(bigTextSize);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over");
    gameOverText.setPosition(sf::Vector2f((width-gameOverText.getLocalBounds().width)/2,
                                          height/2-bigTextSize));gameOverText.setCharacterSize(bigTextSize);

    gameWinText.setCharacterSize(titleSize);
    gameWinText.setFillColor(sf::Color::Green);
    gameWinText.setString("Err.. it seems we ran out of levels\nYou win, I guess");
    gameWinText.setPosition(sf::Vector2f((width-gameWinText.getLocalBounds().width)/2,
                                          height/2-titleSize));

    border.setPosition(sf::Vector2f(marginWidth,marginHeight));
    border.setOutlineThickness(1);
    border.setOutlineColor(sf::Color::White);
    border.setFillColor(sf::Color::Transparent);
    border.setSize(sf::Vector2f(width-2*marginWidth, height-2*marginHeight));

//            window.setKeyRepeatEnabled(false);
}

void Game::toInitState() {

    paddle.setPosition(marginWidth, (height-paddleHeight)/2);
    paddle2.setPosition(width-marginWidth-paddleWidth, (height-paddleHeight)/2);
    ball.setPosition(marginWidth+paddleWidth, (height-ballRadius*2)/2);
    ballheld = 1;

    melives = cpulives = 3;
    lvl = 1;
    score = 0;

}

void Game::drawInfo()
{


    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(marginHeight-10);
    text.setColor(sf::Color::Yellow);
    std::stringstream str;
    str << "Score: " << score;
    text.setString(str.str());
    text.setPosition(sf::Vector2f(width-300,0));
    window.draw(text);

    str.str(std::string());
    str << "LEVEL: " << lvl;
    text.setString(str.str());
    text.setPosition(sf::Vector2f(marginWidth,0));
    window.draw(text);

    sf::Sprite heartsprite;
    heartsprite.setTexture(texture);
    heartsprite.setTextureRect(sf::IntRect(0,0,60,60));
    heartsprite.setScale(sf::Vector2f(0.8f,0.8f));

    heartsprite.setColor(sf::Color::Red);

    for(int i=0; i<melives; i++)
    {
        heartsprite.setPosition(sf::Vector2f(marginWidth+i*60,height-marginHeight));
        window.draw(heartsprite);
    }

    heartsprite.setColor(sf::Color(127,127,255));

    for(int i=0; i<cpulives; i++)
    {
        heartsprite.setPosition(sf::Vector2f(width-marginWidth-60-i*60,height-marginHeight));
        window.draw(heartsprite);
    }
}

void Game::draw()
{
    window.clear();

    if(screen == 0)
    {
        window.draw(startText);
        window.draw(exitText);
    }
    else if(screen == 1)
    {
        window.draw(paddle);
        window.draw(paddle2);
        window.draw(ball);
        drawInfo();
    } else if(screen == 2) {
        window.draw(gameOverText);
    } else if(screen == 3) {
        window.draw(gameWinText);
    }
    window.draw(border);

    window.display();
}

void Game::resetTextColors()
{
    exitText.setFillColor(sf::Color::Green);
    startText.setFillColor(sf::Color::Green);
}

void Game::handleEvents()
{
    sf::Event event;
    int x,y;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if(screen == 1)
            {
                if(event.key.code == sf::Keyboard::Up)
                    paddle.setVelocity(-paddleVel);
                else if(event.key.code == sf::Keyboard::Down)
                    paddle.setVelocity(paddleVel);
                else if(event.key.code == sf::Keyboard::Space)
                {
                    if(ballheld == 1)
                    {
                        ball.setVelocity(sf::Vector2f(ballInitVel,0));
                        ballheld = 0;
                    }
                }
            } else if(screen == 2 || screen == 3) {
                screen = 0;
            }
            break;
        case sf::Event::KeyReleased:
            if(screen == 1)
            {
                if((event.key.code == sf::Keyboard::Up && paddle.getVelocity()<0)||
                        (event.key.code == sf::Keyboard::Down && paddle.getVelocity()>0))
                    paddle.setVelocity(0);
            }
            break;
        case sf::Event::MouseMoved:
            x = event.mouseMove.x;
            y = event.mouseMove.y;
            if(screen == 0)
            {
                resetTextColors();
                sf::FloatRect startRect(startText.getPosition().x,
                                        startText.getPosition().y,
                                        startText.getLocalBounds().width,
                                        startText.getCharacterSize());
                sf::FloatRect exitRect(exitText.getPosition().x,
                                       exitText.getPosition().y,
                                       exitText.getLocalBounds().width,
                                       exitText.getCharacterSize());
                if(startRect.contains(x,y))
                {
                    startText.setFillColor(sf::Color::Yellow);
                }
                else if(exitRect.contains(x,y))
                {
                    exitText.setFillColor(sf::Color::Yellow);
                }
            }
            break;
        case sf::Event::MouseButtonPressed:
            x = event.mouseButton.x;
            y = event.mouseButton.y;
            if(screen == 0)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::FloatRect startRect(startText.getPosition().x,
                                            startText.getPosition().y,
                                            startText.getLocalBounds().width,
                                            startText.getCharacterSize());
                    sf::FloatRect exitRect(exitText.getPosition().x,
                                           exitText.getPosition().y,
                                           exitText.getLocalBounds().width,
                                           exitText.getCharacterSize());
                    //sf::FloatRect exitRect = exitText.getLocalBounds();
                    //std::cout << startRect.top << std::endl;
                    if(startRect.contains(x,y))
                    {
                        screen=1;
                    }
                    else if(exitRect.contains(x,y))
                    {
                        window.close();
                    }
                }
            }
        default:
            break;
        }
    }
}

void Game::perfectAI(float dt) {

    static int cnt = 0;

    if(ballheld == 2) {
        if(aitimer < 0) {
            ballheld = 0;
            ball.bounce(1);
            cnt = 0;
        } else {
            aitimer -= dt;
            cnt++;
            if(cnt %70 == 0) {
                paddle2.setVelocity( ((rand()%2)*2-1)*paddleVel );
            }
        }
    } else {

        int pcenter = paddle2.getPosition().y + paddle2.getSize().y/2;
        int bcenter = ball.getPosition().y+ballRadius;
        int diff = pcenter - bcenter;
        if(diff > 0) paddle2.setVelocity(-paddleVel);
        else if(diff < 0) paddle2.setVelocity(paddleVel);
        else paddle2.setVelocity(0);
    }
}

void Game::update(float dt)
{
    sf::Vector2f pPos = paddle.getPosition();
    sf::Vector2f p2Pos = paddle2.getPosition();
    sf::Vector2f pSize = paddle.getSize();
    sf::Vector2f p2Size = paddle2.getSize();
    perfectAI(dt);

    paddle.update(dt);
    paddle2.update(dt);


    if(paddle.isOutOfBounds(marginHeight, height-marginHeight))
    {
        paddle.revert(dt);
    }
    if(paddle2.isOutOfBounds(marginHeight, height-marginHeight))
    {
        paddle2.revert(dt);
    }
    if(ballheld == 1)
    {
        ball.setPosition(sf::Vector2f(pPos.x+pSize.x,
                                      pPos.y+pSize.y/2-ballRadius));
        ball.setVelocity(0.0,0);
    } else if(ballheld == 2) { /**held by cpu**/
        ball.setPosition(sf::Vector2f(p2Pos.x-2*ballRadius,
                                      p2Pos.y+p2Size.y/2-ballRadius));
        ball.setVelocity(0.0,0);
    }
    else
    {
        ball.update(dt);
        int oobstat = ball.isOutOfBounds(sf::Vector2f(marginWidth, marginHeight),
                                         sf::Vector2f(width-marginWidth, height-marginHeight));
        sf::Vector2f bPos = ball.getPosition();
        int d = 2*ball.getRadius();
        if(oobstat != 0)
        {
            if(bPos.x <= marginWidth)
            {
                melives--;
                if(melives == 0) {
                    screen = 2;
                    toInitState();
                }
                ballheld=1;
                return;
            } else if(bPos.x+d >= width-marginWidth) {
                cpulives--;
                if(cpulives == 0) {
                    /**nextLevel();**/
                }
                aitimer = 3; /**1 sec*/
                ballheld = 2;
            }
            ball.revert(dt);
            ball.bounce(oobstat);
        }


        if(hasCollided(bPos, sf::Vector2f(d,d),pPos, pSize) != 0)
            //ball collided with paddle
        {
            int ballcenter = bPos.y+d/2;
            int paddlecenter = pPos.y + pSize.y/2;
            int k = pSize.y/2;
            int diff = ballcenter - paddlecenter;
            float angle = PI*(13*diff - 18*k)/(float)(36*k)+PI/(float)2;//-PI*5/(float)12;
            ball.setVelocity(angle, ballInitVel);
        } else if(hasCollided(bPos, sf::Vector2f(d,d),p2Pos, p2Size) != 0)
            //ball collided with paddle 2
        {
            int ballcenter = bPos.y+d/2;
            int paddlecenter = p2Pos.y + p2Size.y/2;
            int k = p2Size.y/2;
            int diff = ballcenter - paddlecenter;
            float angle = -(PI*(13*diff - 18*k)/(float)(36*k))+PI/2;
            ball.setVelocity(angle, ballInitVel);
        }

    }//else part of ballheld==1
}

int Game::hasCollided(sf::Vector2f pos1, sf::Vector2f size1, sf::Vector2f pos2, sf::Vector2f size2)
{
    bool xin=false, yin = false;
    bool pos1xin = (pos1.x >= pos2.x && pos1.x <= pos2.x + size2.x);
    bool pos1endxin = (pos1.x+size1.x >= pos2.x && pos1.x+size1.x <= pos2.x + size2.x);
    bool pos1yin = (pos1.y >= pos2.y && pos1.y <= pos2.y + size2.y);
    bool pos1endyin =  (pos1.y+size1.y >= pos2.y && pos1.y+size1.y <= pos2.y + size2.y);
    if( pos1xin || pos1endxin)
    {
        xin = true;
    }
    if( pos1yin || pos1endyin)
    {
        yin = true;
    }
    if(xin &&yin)
    {
        if(pos1xin && pos1endxin )
            return 2; /**vertical bounce**/
        else if(pos1yin && pos1endyin)
            return 1; /**horizontal bounces**/
        else {
            return 3; /**diagonal bounce**/
        }
    }
    return 0;
}

bool Game::isRunning()
{
    return window.isOpen();
}
