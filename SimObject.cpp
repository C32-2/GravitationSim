#include "SimObject.h"

SimObject::SimObject(float mass, float radius, float x, float y, sf::Color col)
{
    Mass = mass;
    Radius = radius;

    coords.x = x;
    coords.y = y;

    color = col;

    shape.setRadius(Radius);
    shape.setPosition(coords.x, coords.y);
    shape.setFillColor(color);
}

bool SimObject::operator==(const SimObject& other) const {
    return (Mass == other.Mass &&
        Radius == other.Radius &&
        coords == other.coords &&
        velocity == other.velocity &&
        color == other.color);
}

void SimObject::updateRadius(float radiusDelta)
{
    Radius += radiusDelta;
    shape.setRadius(Radius);
}

float SimObject::getMass() const
{
    return Mass;
}

float SimObject::getRadius() const
{
    return Radius;
}

sf::Vector2f SimObject::getCoordinates() const
{
    return coords;
}

sf::Vector2f SimObject::getVelocity() const
{
    return velocity;
}

sf::CircleShape SimObject::getShape()
{
    return shape;
}

void SimObject::setCoordinates(float x, float y)
{
    coords.x = x;
    coords.y = y;
    shape.setPosition(coords.x, coords.y);
}

void SimObject::updateVelocity(float vx, float vy)
{
    velocity.x += vx;
    velocity.y += vy;
}

void SimObject::setMass(float massSeted)
{
    Mass = massSeted;
}

void SimObject::multiplyVelocity(float xMultiplier, float yMultiplier)
{
    velocity.x *= xMultiplier;
    velocity.y *= yMultiplier;
}

void SimObject::updateCoordinates()
{
    coords.x += velocity.x;
    coords.y += velocity.y;
    shape.setPosition(coords.x, coords.y);
}

bool SimObject::isCollided()
{
    return collided;
}

void SimObject::setCollided(bool flag)
{
    collided = flag;
}

void SimObject::changeColor()
{
    color = sf::Color(color.r, color.g - 1, color.b - 1);
    shape.setFillColor(color);
    Mass += 1;
    lifeTime += 1;
}

sf::Color SimObject::getColor()
{
    return color;
}

int SimObject::getLifeTime()
{
    return lifeTime;
}

