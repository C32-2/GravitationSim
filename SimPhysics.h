#pragma once
#include <SFML/Graphics.hpp>
#include "SimObject.h"
#include <vector>

class SimPhysics
{
public:
    static float distanceBetweenTwo(SimObject& obj1, SimObject& obj2);

    static float projectForceX(SimObject& obj1, SimObject& obj2, float force);

    static float projectForceY(SimObject& obj1, SimObject& obj2, float force);

    static void calculateAcceleration(std::vector<SimObject*>& objects, bool destroyObjects);

    static void collapseBody(SimObject& obj, std::vector<SimObject*>& objects);

    static float calculateRadiusByMass(float mass);

    static sf::Vector2f calculateMassCenter(std::vector<SimObject*>& objects);

private:

    static float CalculateForce(SimObject& obj1, SimObject& obj2);
};
