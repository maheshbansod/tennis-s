
#include "Paddle.hpp"
#include <iostream>

Paddle::Paddle(int width, int height) :
    rectshape(sf::Vector2f(width, height)) {
    vel = 0;
}

void Paddle::setTexture(sf::Texture *texture) {
    rectshape.setTexture(texture);
    rectshape.setTextureRect(sf::IntRect(0,60,20,200));
}

void Paddle::setColor(sf::Color color) {
    rectshape.setFillColor(color);
}

void Paddle::update(float dt) {
    move(0,vel*dt);
}

void Paddle::revert(float dt) {
    move(0,-vel*dt);
}

bool Paddle::isOutOfBounds(int zero, int height) {
    sf::Vector2f pos = getPosition();
    //std::cout << pos.x <<","<<width<<std::endl;
    if(pos.y < zero || pos.y > height- rectshape.getSize().y) return true;
    return false;
}

void Paddle::setVelocity(int v) {
    vel = v;
}

int Paddle::getVelocity() {
    return vel;
}

sf::Vector2f Paddle::getSize() {
    return rectshape.getSize();
}

