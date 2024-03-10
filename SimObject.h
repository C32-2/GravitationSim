#pragma once
#include <SFML/Graphics.hpp>

class SimObject
{
private:
    float Mass = 0.0f;
    float Radius = 0.0f;

    sf::Vector2f coords;
    sf::Vector2f velocity;
    sf::Color color;

    int lifeTime = 0;

    sf::CircleShape shape;

    bool collided = false;
    bool destructed = false;

public:
    SimObject(const float mass, const float radius, float x, float y, sf::Color col);

    bool operator==(const SimObject& other) const;

    void updateRadius(float radiusDelta);

    float getMass() const;

    float getRadius() const;

    sf::Vector2f getCoordinates() const;

    sf::Vector2f getVelocity() const;

    sf::CircleShape getShape();

    void setCoordinates(float x, float y);

    void setMass(float massSeted);

    void updateVelocity(float vx, float vy);

    void multiplyVelocity(float xMultiplier, float yMultiplier);

    void updateCoordinates();

    bool isCollided();

    bool isCleanerObject();

    void setCollided(bool flag);

    void changeColor();

    sf::Color getColor();

    int getLifeTime();
};

