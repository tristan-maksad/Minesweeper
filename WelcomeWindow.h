#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <string>

class WelcomeWindow {
private:
    sf::RenderWindow window;
    sf::Font font;
    std::string pName;
    sf::Text welcomeText;
    sf::Text nameText;
    sf::Text inputText;
    sf::Text cursorText;
public:
    WelcomeWindow(int width, int height);
    void setText(int width, int height);
    void updateInputDisplay();
    std::string run();
};

#endif