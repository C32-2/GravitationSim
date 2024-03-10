#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include "SimObject.h"
#include "SimPhysics.h"

int main()
{

    int width = 1920 * 0.8; int height = 1080 * 0.8;
    int mouseX = 0.0f; int mouseY = 0.0f;
    float mass =  3.0f;
    float radius = SimPhysics::calculateRadiusByMass(mass);
    bool isDestroyable = false;

    sf::RenderWindow window(sf::VideoMode(width, height), "Space Simulation");
    std::vector<SimObject*> objects;

    sf::CircleShape massCenter(5.0f);
    sf::CircleShape destroyableButton(15.0f);
    sf::CircleShape prototype;

    massCenter.setFillColor(sf::Color(100, 100, 100));
    destroyableButton.setPosition(50, 50);
    destroyableButton.setFillColor(sf::Color(150, 0, 0));
    prototype.setFillColor(sf::Color(255, 255, 255, 128));

    window.setFramerateLimit(120);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }
            else if (event.type == sf::Event::MouseWheelScrolled) 
            {
                if (event.mouseWheelScroll.delta > 0) 
                {
                    mass += 1.0f;
                }
                else if (event.mouseWheelScroll.delta < 0) 
                {
                    mass -= 1.0f;
                }

                if (mass <= 1.0f)
                {
                    mass = 1.0f;
                }
                else if (mass >= 12.0f)
                {
                    mass = 8.0f;
                }
                radius = SimPhysics::calculateRadiusByMass(mass);
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                {
                    mouseX = event.mouseMove.x;
                    mouseY = event.mouseMove.y;

                    std::cout << mass << "\n";

                    if (objects.size() < 300)
                    {
                        objects.emplace_back(new SimObject(mass, radius, mouseX - radius, mouseY - radius, sf::Color(255, 255, 255)));
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    mouseX = event.mouseButton.x;
                    mouseY = event.mouseButton.y;

                    if (destroyableButton.getGlobalBounds().contains(sf::Vector2f(mouseX, mouseY)))
                    {
                        if (isDestroyable) 
                        {
                            isDestroyable = false;
                            destroyableButton.setFillColor(sf::Color(150, 0, 0));
                        }
                        else
                        {
                            isDestroyable = true;
                            destroyableButton.setFillColor(sf::Color(0, 150, 0));
                        }
                    }

                    else
                    {
                        objects.emplace_back(new SimObject(mass, radius, mouseX - radius, mouseY - radius, sf::Color(255, 255, 255)));
                    }
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            {
                for (auto it = objects.begin(); it != objects.end(); ++it) 
                {
                    delete* it;
                }
                objects.clear();
            }
        }

        prototype.setRadius(radius);
        prototype.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x - radius, sf::Mouse::getPosition(window).y - radius));

        window.clear(sf::Color(30, 30, 30));

        if (!objects.empty())
        {
            std::cout << objects.size() << "\n";
            massCenter.setPosition(SimPhysics::calculateMassCenter(objects));
            for (auto object : objects)
            {
                if(object->getLifeTime() >= 255)
                {
                    auto it = std::find(objects.begin(), objects.end(), object);
                    if (it != objects.end())
                    {
                        delete* it;
                        objects.erase(it);
                        
                    }
                }
                if ((object->getCoordinates().x - object->getRadius()) < 0)
                {
                    object->setCoordinates(object->getRadius(), object->getCoordinates().y);
                    object->multiplyVelocity(-0.2f, 1);
                }
                else if ((object->getCoordinates().x + object->getRadius()) > width)
                {
                    object->setCoordinates(width - object->getRadius(), object->getCoordinates().y);
                    object->multiplyVelocity(-0.2f, 1);
                }
                else if ((object->getCoordinates().y - object->getRadius()) < 0)
                {
                    object->setCoordinates(object->getCoordinates().x, object->getRadius());
                    object->multiplyVelocity(1, -0.2f);
                }
                else if ((object->getCoordinates().y + object->getRadius()) > height)
                {
                    object->setCoordinates(object->getCoordinates().x, height - object->getRadius());
                    object->multiplyVelocity(1, -0.2f);
                }

                window.draw(massCenter);
                window.draw(object->getShape());
            }

        }

        SimPhysics::calculateAcceleration(objects, isDestroyable);

        if (!destroyableButton.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)))
        {
            window.draw(prototype);
        }
        window.draw(destroyableButton);
        window.display();
    }

    for (auto object : objects)
    {
        delete object;
    }

    return 0;
}