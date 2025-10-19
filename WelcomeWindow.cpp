#include "WelcomeWindow.h"
#include <cctype>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

WelcomeWindow::WelcomeWindow(int width, int height) {
    window.create(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    font.loadFromFile("files/font.ttf");
    setText(width, height);
}

void WelcomeWindow::setText(int width, int height) {
    //Welcome Text
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setStyle(sf::Text::Underlined);
    sf::FloatRect bounds = welcomeText.getLocalBounds();
    welcomeText.setOrigin(bounds.width/2, bounds.height/2);
    welcomeText.setPosition(width/2.0f, height/2.0f - 150);
    //Name text
    nameText.setString("Enter your name:");
    nameText.setFont(font);
    nameText.setCharacterSize(20);
    nameText.setFillColor(sf::Color::White);
    nameText.setStyle(sf::Text::Bold);
    bounds = nameText.getLocalBounds();
    nameText.setOrigin(bounds.width/2, bounds.height/2);
    nameText.setPosition(width/2.0f, height/2.0f - 75);
    //Input text
    inputText.setString("");
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);
    bounds = inputText.getLocalBounds();
    inputText.setOrigin(bounds.width/2, bounds.height/2);
    inputText.setPosition(width/2.0f, height/2.0f - 45);
    //Cursor Text
    cursorText.setString("|");
    cursorText.setFont(font);
    cursorText.setCharacterSize(18);
    cursorText.setFillColor(sf::Color::Yellow);
    bounds = cursorText.getLocalBounds();
    cursorText.setOrigin(bounds.width/2, bounds.height/2);
    cursorText.setPosition(width/2.0f, height/2.0f - 45);
}

//Updates the displayed input text and cursor position whenever pName changes
void WelcomeWindow::updateInputDisplay() {
    cursorText.setPosition(window.getSize().x/2.0f, window.getSize().y/2.0f - 45);
    if (!pName.empty()) {
        pName[0] = toupper(pName[0]);
        for (size_t i = 1; i < pName.size(); i++) {
            pName[i] = tolower(pName[i]);
        }
        inputText.setString(pName);
        sf::FloatRect bounds = inputText.getLocalBounds();
        inputText.setOrigin(bounds.width/2, bounds.height/2);
        inputText.setPosition(window.getSize().x/2.0f, window.getSize().y/2.0f - 45);
        cursorText.setPosition(inputText.getPosition().x + bounds.width/2 + 2, window.getSize().y/2.0f - 45);
    }
    else {
        inputText.setString("");
    }
}

//Main event loop: Handles typing input and window display
string WelcomeWindow::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return "";
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!pName.empty()) {
                        pName.pop_back();
                        updateInputDisplay();
                    }
                }
                else if (event.text.unicode == '\r') {
                    if (!pName.empty()) {
                        window.close();
                    }
                }
                else if (isalpha(static_cast<char>(event.text.unicode)) && pName.size() < 10) {
                    pName += static_cast<char>(event.text.unicode);
                    updateInputDisplay();
                }
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(welcomeText);
        window.draw(nameText);
        window.draw(inputText);
        window.draw(cursorText);
        window.display();
    }
    return pName;
}