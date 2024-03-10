#include "SimPhysics.h"
#include <thread>

float SimPhysics::distanceBetweenTwo(SimObject& obj1, SimObject& obj2)
{
    sf::Vector2f coords1 = obj1.getCoordinates();
    sf::Vector2f coords2 = obj2.getCoordinates();

    return std::sqrt(std::pow(coords2.x - coords1.x, 2) + std::pow(coords2.y - coords1.y, 2));
}

float SimPhysics::projectForceX(SimObject& obj1, SimObject& obj2, float force)
{
    float dx = obj2.getCoordinates().x - obj1.getCoordinates().x;
    return force * dx / distanceBetweenTwo(obj1, obj2);
}

float SimPhysics::projectForceY(SimObject& obj1, SimObject& obj2, float force)
{
    float dy = obj2.getCoordinates().y - obj1.getCoordinates().y;
    return force * dy / distanceBetweenTwo(obj1, obj2);
}

void SimPhysics::calculateAcceleration(std::vector<SimObject*>& objects, bool destroyObjects)
{
    float forceX = 0.0f; float forceY = 0.0f;
    float dist = 0.0f;
    for (size_t i = 0; i < objects.size(); i++)
    {
        forceX = 0; forceY = 0;
        objects[i]->setCollided(false);

        for (size_t j = 0; j < objects.size(); j++)
        {
            if (i != j)
            {
                if (!objects[i]->isCollided() && !objects[j]->isCollided())
                {
                    dist = distanceBetweenTwo(*objects[i], *objects[j]);

                    if (dist <= (objects[i]->getRadius() + objects[j]->getRadius()))
                    {
                        objects[i]->setCollided(true);
                        objects[j]->setCollided(true);

                        if (destroyObjects)
                        {
                            objects[i]->changeColor();
                            objects[i]->updateRadius(0.05f);

                            objects[i]->changeColor();

                            if (objects[i]->getLifeTime() >= 255)
                            {
                                collapseBody(*objects[i], objects);
                            }
                        }
                        continue;
                    }
                }

                if (!objects[i]->isCollided() && !objects[j]->isCollided())
                {
                    forceX += projectForceX(*objects[i], *objects[j], CalculateForce(*objects[i], *objects[j]));
                    forceY += projectForceY(*objects[i], *objects[j], CalculateForce(*objects[i], *objects[j]));
                }
            }
        }
        float aX = forceX / objects[i]->getMass();
        float aY = forceY / objects[i]->getMass();

        objects[i]->updateVelocity(aX / 10, aY / 10);
        objects[i]->updateCoordinates();
    }
}


void SimPhysics::collapseBody(SimObject& obj, std::vector<SimObject*>& objects)
{
    float forceX = 0.0f; float forceY = 0.0f;
    float accelerateX = 0.0f; float accelerateY = 0.0f;

    for (size_t i = 0; i < objects.size(); i++)
    {
        if (*objects[i] == obj)
        {
            continue;
        }
        else
        {
            if (SimPhysics::distanceBetweenTwo(obj, *objects[i]) <= 100)
            {
                forceX = projectForceX(obj, *objects[i], CalculateForce(obj, *objects[i]));
                forceY = projectForceY(obj, *objects[i], CalculateForce(obj, *objects[i]));

                accelerateX = forceX * 10 / objects[i]->getMass();
                accelerateY = forceY * 10 / objects[i]->getMass();

                objects[i]->updateVelocity(accelerateX, accelerateY);
                objects[i]->updateCoordinates();
            }
        }
    }
}

float SimPhysics::calculateRadiusByMass(float mass)
{
    return mass * 2;
}

sf::Vector2f SimPhysics::calculateMassCenter(std::vector<SimObject*>& objects)
{
    float bodyMomentsX = 0.0f;
    float bodyMomentsY = 0.0f;
    float summedMasses = 0.0f;

    for (size_t i = 0; i < objects.size(); i++)
    {
        summedMasses += objects[i]->getMass();
        bodyMomentsX += objects[i]->getCoordinates().x * objects[i]->getMass();
        bodyMomentsY += objects[i]->getCoordinates().y * objects[i]->getMass();
    }
    return sf::Vector2f(bodyMomentsX / summedMasses, bodyMomentsY / summedMasses);

}

float SimPhysics::CalculateForce(SimObject& obj1, SimObject& obj2)
{
    const float graviConst = 6.674f;
    return graviConst * ((obj1.getMass() * obj2.getMass()) / std::pow(SimPhysics::distanceBetweenTwo(obj1, obj2), 2));
}