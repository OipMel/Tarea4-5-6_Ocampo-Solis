#include <SFML/Graphics.hpp>
#include <string>
#include "menu.h"
#include "MenuPrincipal.h"
#include <iostream>
using namespace std;

void text()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Text Input");

    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) // Replace with your font file
    {
        // Handle font loading error
    }

    sf::Text inputText("", font, 30);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(100, 100);

    std::string currentString;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128) // Only process ASCII characters for simplicity
                {
                    if (event.text.unicode == '\b') // Backspace
                    {
                        if (!currentString.empty())
                        {
                            currentString.pop_back();
                        }
                    }
                    else if (event.text.unicode == '\r') // Enter key
                    {
                        // Handle enter key, e.g., submit text
                        cout << "Enter pressed. String: " << currentString << endl;
						currentString.clear(); // Clear the string after submission
                    }
                    else
                    {
                        currentString += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(currentString);

                }
            }
        }

        window.clear();
        window.draw(inputText);
        window.display();
    }
}

int main() {
    ContextSettings settings;
    settings.antialiasingLevel = 8;

    RenderWindow window(VideoMode(1920, 1080), "SFML shapes", Style::Fullscreen);
    window.setFramerateLimit(60);
    bool menuPrincipal = true;

    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) return -1;

    menuInicio(window, menuPrincipal, font);
}