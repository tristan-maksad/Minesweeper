#include "GameWindow.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <random>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

//Constructor: Initializes Minesweeper game window, assets, grid, mines, counters, and game loop
GameWindow::GameWindow(int width, int height, int mines, string name) {
    gameOverW = false;
    gameOverL = false;
    flagCount = mines;
    isPaused = false;
    isDebug = false;
    pName = name;
    clock.restart();
    window.create(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
    font.loadFromFile("files/font.ttf");
    loadFiles();
    setScreen(width / 32, (height - 100) / 32, mines);
    mineFunc(width / 32, (height - 100) / 32, mines);
    setCounter(flagCount);
    run(width / 32, (height - 100) / 32, mines);
}

//Loads all image textures (tiles, digits, icons, etc.) used in game
void GameWindow::loadFiles() {
    if (!(debug.loadFromFile("files/images/debug.png") &&
        digits.loadFromFile("files/images/digits.png") &&
        happy.loadFromFile("files/images/face_happy.png") &&
        lose.loadFromFile("files/images/face_lose.png") &&
        win.loadFromFile("files/images/face_win.png") &&
        flag.loadFromFile("files/images/flag.png") &&
        mine.loadFromFile("files/images/mine.png") &&
        one.loadFromFile("files/images/number_1.png") &&
        two.loadFromFile("files/images/number_2.png") &&
        three.loadFromFile("files/images/number_3.png") &&
        four.loadFromFile("files/images/number_4.png") &&
        five.loadFromFile("files/images/number_5.png") &&
        six.loadFromFile("files/images/number_6.png") &&
        seven.loadFromFile("files/images/number_7.png") &&
        eight.loadFromFile("files/images/number_8.png") &&
        pause.loadFromFile("files/images/pause.png") &&
        play.loadFromFile("files/images/play.png") &&
        tileHidden.loadFromFile("files/images/tile_hidden.png") &&
        tileRevealed.loadFromFile("files/images/tile_revealed.png"))) {
        cerr << "File(s) not open!\n";
    }
}

//Sets up initial board layout & UI element positions
//Initializes tile sprites & game icons
void GameWindow::setScreen(int cols, int rows, int mines) {
    tiles.resize(cols * rows);
    for (int i = 0; i < rows * cols; i++) {
        tiles[i].hiddenS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].hiddenS.setTexture(tileHidden);
        tiles[i].revealedS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].revealedS.setTexture(tileRevealed);
        tiles[i].revealedS2.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].revealedS2.setTexture(tileRevealed);
        tiles[i].mineS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].mineS.setTexture(mine);
        tiles[i].flagS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].flagS.setTexture(flag);
        tiles[i].oneS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].oneS.setTexture(one);
        tiles[i].twoS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].twoS.setTexture(two);
        tiles[i].threeS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].threeS.setTexture(three);
        tiles[i].fourS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].fourS.setTexture(four);
        tiles[i].fiveS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].fiveS.setTexture(five);
        tiles[i].sixS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].sixS.setTexture(six);
        tiles[i].sevenS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].sevenS.setTexture(seven);
        tiles[i].eightS.setPosition(i % cols * 32, i / cols * 32);
        tiles[i].eightS.setTexture(eight);
        tiles[i].isRevealed = false;
        tiles[i].isMine = false;
        tiles[i].isFlag = false;
        tiles[i].mineCount = 0;
    }
    debugS.setTexture(debug);
    debugS.setPosition((cols * 32.0f) - 304, (rows + 0.5f) * 32);
    faceS.setTexture(happy);
    faceS.setPosition((cols * 32.0f) / 2 - 32, (rows + 0.5f) * 32);
    pauseS.setTexture(pause);
    pauseS.setPosition((cols * 32.0f) - 240, (rows + 0.5f) * 32);
}

//Randomly places mines and updates adjacent tile mine counts
void GameWindow::mineFunc(int cols, int rows, int mines) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, cols * rows - 1);
    int count = 0;
    while (count < mines) {
        int i = dist(gen);
        int col = i % cols;
        int row = i / cols;
        bool onRight = (col == cols - 1);
        bool onLeft = (col == 0);
        bool onUp = (row == 0);
        bool onDown = (row == rows - 1);
        if (!tiles[i].isMine) {
            tiles[i].isMine = true;
            count++;
            if (!(onRight || onLeft || onUp || onDown)) {
                tiles[i-1].mineCount++;
                tiles[i+1].mineCount++;
                tiles[i+cols].mineCount++;
                tiles[i-cols].mineCount++;
                tiles[i+cols+1].mineCount++;
                tiles[i-cols+1].mineCount++;
                tiles[i+cols-1].mineCount++;
                tiles[i-cols-1].mineCount++;
            }
            else if (onRight && onUp) {
                tiles[i-1].mineCount++;
                tiles[i+cols].mineCount++;
                tiles[i+cols-1].mineCount++;
            }
            else if (onLeft && onUp) {
                tiles[i+1].mineCount++;
                tiles[i+cols].mineCount++;
                tiles[i+cols+1].mineCount++;
            }
            else if (onRight && onDown) {
                tiles[i-1].mineCount++;
                tiles[i-cols].mineCount++;
                tiles[i-cols-1].mineCount++;
            }
            else if (onLeft && onDown) {
                tiles[i+1].mineCount++;
                tiles[i-cols].mineCount++;
                tiles[i-cols+1].mineCount++;
            }
            else if (onRight) {
                tiles[i-1].mineCount++;
                tiles[i+cols].mineCount++;
                tiles[i-cols].mineCount++;
                tiles[i+cols-1].mineCount++;
                tiles[i-cols-1].mineCount++;
            }
            else if (onLeft) {
                tiles[i+1].mineCount++;
                tiles[i+cols].mineCount++;
                tiles[i-cols].mineCount++;
                tiles[i+cols+1].mineCount++;
                tiles[i-cols+1].mineCount++;
            }
            else if (onUp) {
                tiles[i-1].mineCount++;
                tiles[i+1].mineCount++;
                tiles[i+cols].mineCount++;
                tiles[i+cols-1].mineCount++;
                tiles[i+cols+1].mineCount++;
            }
            else if (onDown) {
                tiles[i-1].mineCount++;
                tiles[i+1].mineCount++;
                tiles[i-cols].mineCount++;
                tiles[i-cols-1].mineCount++;
                tiles[i-cols+1].mineCount++;
            }
        }
    }
}

//Left click handler
//Reveals clicked tile, triggers recursive blank reveals, or ends game
void GameWindow::tileLC(int x, int y, int cols, int rows , int mines) {
    if (gameOverW || gameOverL || isPaused) {
        return;
    }
    int x1 = x / 32;
    int y1 = y / 32;
    int col = window.getSize().x / 32;
    int row = (window.getSize().y - 100) / 32;
    if (x1 >= 0 && y1 >= 0 && x1 < col && y1 < row) {
        int i = x1 + y1 * col;
        if (!(tiles[i].isRevealed || tiles[i].isFlag)) {
            tiles[i].isRevealed = true;
            int col = i % cols;
            int row = i / cols;
            bool onRight = (col == cols - 1);
            bool onLeft = (col == 0);
            bool onUp = (row == 0);
            bool onDown = (row == rows - 1);
            if (!tiles[i].isMine) {
                blanksNear(i, cols, rows, onRight, onLeft, onUp, onDown);
            }
            else {
                gameOverL = true;
                return;
            }
        }
    }
}

//Recursively reveals adjacent blank tiles and corresponding neighbors.
void GameWindow::blanksNear(int i, int cols, int rows, bool onRight, bool onLeft, bool onUp, bool onDown) {
    vector<int> nears;
    if (!(onRight || onLeft || onUp || onDown)) {
        nears.push_back(i-1);
        nears.push_back(i+1);
        nears.push_back(i+cols);
        nears.push_back(i-cols);
        nears.push_back(i+cols+1);
        nears.push_back(i-cols+1);
        nears.push_back(i+cols-1);
        nears.push_back(i-cols-1);
    }
    else if (onRight && onUp) {
        nears.push_back(i-1);
        nears.push_back(i+cols);
        nears.push_back(i+cols-1);
    }
    else if (onLeft && onUp) {
        nears.push_back(i+1);
        nears.push_back(i+cols);
        nears.push_back(i+cols+1);
    }
    else if (onRight && onDown) {
        nears.push_back(i-1);
        nears.push_back(i-cols);
        nears.push_back(i-cols-1);
    }
    else if (onLeft && onDown) {
        nears.push_back(i+1);
        nears.push_back(i-cols);
        nears.push_back(i-cols+1);
    }
    else if (onRight) {
        nears.push_back(i-1);
        nears.push_back(i+cols);
        nears.push_back(i-cols);
        nears.push_back(i+cols-1);
        nears.push_back(i-cols-1);
    }
    else if (onLeft) {
        nears.push_back(i+1);
        nears.push_back(i+cols);
        nears.push_back(i-cols);
        nears.push_back(i+cols+1);
        nears.push_back(i-cols+1);
    }
    else if (onUp) {
        nears.push_back(i-1);
        nears.push_back(i+1);
        nears.push_back(i+cols);
        nears.push_back(i+cols-1);
        nears.push_back(i+cols+1);
    }
    else if (onDown) {
        nears.push_back(i-1);
        nears.push_back(i+1);
        nears.push_back(i-cols);
        nears.push_back(i-cols-1);
        nears.push_back(i-cols+1);
    }
    for (auto j : nears) {
        if (tiles[j].isMine || tiles[j].isRevealed || tiles[j].isFlag) {
            continue;
        }
        else {
            tiles[j].isRevealed = true;
            if (tiles[j].mineCount == 0) {
                bool onRight = ((j + 1) % cols == 0);
                bool onLeft = (j == 0 || j % cols == 0);
                bool onUp = (j / cols == 0);
                bool onDown = ((j + cols) / (cols * rows) == 1);
                blanksNear(j, cols, rows, onRight, onLeft, onUp, onDown);
            }
        }
    }
}

//Right click handler
//Places/removes flags
void GameWindow::tileRC(int x, int y) {
    if (gameOverW || gameOverL || isPaused) {
        return;
    }
    int x1 = x / 32;
    int y1 = y / 32;
    int col = window.getSize().x / 32;
    int row = (window.getSize().y - 100) / 32;
    if (x1 >= 0 && y1 >= 0 && x1 < col && y1 < row) {
        int i = x1 + y1 * col;
        if (!tiles[i].isFlag) {
            tiles[i].isFlag = true;
            flagCount--;
        }
        else {
            tiles[i].isFlag = false;
            flagCount++;
        }
    }
}

//Updates flag counter display at bottom left of screen (includes negative values)
void GameWindow::setCounter(int flagCount) {
    digitsSV.clear();
    float x = 33;
    float y = 32 * ((window.getSize().y - 100) / 32 + 0.5f) + 16;
    string flagStr = to_string(abs(flagCount));
    int flagInt = 0;
    while (flagStr.size() < 3) {
        flagStr = "0" + flagStr;
    }
    if (flagCount < 0) {
        flagStr = '-' + flagStr;
    }
    for (int i = flagStr.size() - 1; i >= 0; i--) {
        if (flagStr[i] == '-') {
            flagInt = 10;
        }
        else {
            flagInt = flagStr[i] - '0';
        }
        digitsS.setTexture(digits);
        digitsS.setTextureRect(sf::IntRect(flagInt * 21, 0, 21, 32));
        digitsS.setPosition(x + (i * 21), y);
        digitsSV.push_back(digitsS);
    }
}

//Updates/displays in-game clock
void GameWindow::timer() {
    digitsSVT.clear();
    if (!isPaused) {
        timeT = clock.getElapsedTime() - sf::seconds(static_cast<float>(timeP));
    }
    int sec = static_cast<int>(timeT.asSeconds()) % 60;
    int min = static_cast<int>(timeT.asSeconds()) / 60;
    string secStr = to_string(sec);
    if (secStr.size() < 2) {
        secStr = "0" + secStr;
    }
    string minStr = to_string(min);
    if (minStr.size() < 2) {
        minStr = "0" + minStr;
    }
    float x = (window.getSize().x) - 54;
    float y = 32 * ((window.getSize().y - 100) / 32 + 0.5f) + 16;
    for (int i = secStr.size() - 1; i >= 0; i--) {
        int secInt = secStr[i] - '0';
        digitsST.setTexture(digits);
        digitsST.setTextureRect(sf::IntRect(secInt * 21, 0, 21, 32));
        digitsST.setPosition(x + (i * 21), y);
        digitsSVT.push_back(digitsST);
    }
    x = (window.getSize().x) - 97;
    for (int i = minStr.size() - 1; i >= 0; i--) {
        int minInt = minStr[i] - '0';
        digitsST.setTexture(digits);
        digitsST.setTextureRect(sf::IntRect(minInt * 21, 0, 21, 32));
        digitsST.setPosition(x + (i * 21), y);
        digitsSVT.push_back(digitsST);
    }
}

//Toggles play/pause state and updates pause button icon
void GameWindow::gamePause() {
    if (isPaused) {
        timeP = static_cast<int>(timeTT.asSeconds()) - static_cast<int>(timeT.asSeconds());
        pauseS.setTexture(play);
    } else {
        clock.restart();
        pauseS.setTexture(pause);
    }
}

//Checks win condition and updates game state
void GameWindow::gameWon(int mines) {
    int count1 = 0;
    int count2 = 0;
    for (auto& tile : tiles) {
        count1++;
        if (tile.isRevealed) {
            count2++;
        }
    }
    if (count1 - mines == count2) {
        faceS.setTexture(win);
        flagCount = 0;
        setCounter(flagCount);
        gameOverW = true;
        for (auto& tile : tiles) {
            if (tile.isMine) {
                tile.isFlag = true;
            }
        }
    }
}

//Resets game state for new game
void GameWindow::gameReset(int cols, int rows, int mines) {
    clock.restart();
    gameOverW = false;
    gameOverL = false;
    isPaused = false;
    isDebug = false;
    flagCount = mines;
    digitsSV.clear();
    faceS.setTexture(happy);
    pauseS.setTexture(pause);
    tiles.clear();
    setScreen(cols, rows, mines);
    mineFunc(cols, rows, mines);
    setCounter(flagCount);
    timeP = 0;
    timeT = sf::Time::Zero;
    timeTT = sf::Time::Zero;
}

//Main game loop: Handles events, updates, & drawing
void GameWindow::run(int cols, int rows, int mines) {
    while (window.isOpen()) {
        timeTT = clock.getElapsedTime();
        if (!isPaused && !gameOverL && !gameOverW) {
            timer();
            pauseS.setTexture(pause);
        }
        else {
            gamePause();
        }
        gameWon(mines);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (debugS.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !gameOverL && !gameOverW && !isPaused) {
                        if (!gameOverW || !gameOverL) {
                            isDebug = !isDebug;
                        }
                    }
                    else if (faceS.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        gameReset(cols, rows, mines);
                    }
                    else if (pauseS.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !gameOverL && !gameOverW) {
                        isPaused = !isPaused;
                    }
                    else {
                        tileLC(event.mouseButton.x, event.mouseButton.y, cols, rows, mines);
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    tileRC(event.mouseButton.x, event.mouseButton.y);
                    setCounter(flagCount);
                }
            }
        }
        window.clear(sf::Color::White);
        for (auto& tile : tiles) {
            if (isPaused) {
                window.draw(tile.revealedS2);
            }
            else if (tile.isRevealed && !tile.isMine) {
                window.draw(tile.revealedS);
                if (tile.mineCount == 1) {
                    window.draw(tile.oneS);
                }
                else if (tile.mineCount == 2) {
                    window.draw(tile.twoS);
                }
                else if (tile.mineCount == 3) {
                    window.draw(tile.threeS);
                }
                else if (tile.mineCount == 4) {
                    window.draw(tile.fourS);
                }
                else if (tile.mineCount == 5) {
                    window.draw(tile.fiveS);
                }
                else if (tile.mineCount == 6) {
                    window.draw(tile.sixS);
                }
                else if (tile.mineCount == 7) {
                    window.draw(tile.sevenS);
                }
                else if (tile.mineCount == 8) {
                    window.draw(tile.eightS);
                }
            }
            else {
                window.draw(tile.hiddenS);
                if (tile.isRevealed && tile.isMine) {
                    for (auto& t : tiles) {
                        if (t.isMine) {
                            t.isRevealed = true;
                            window.draw(t.mineS);
                        }
                    }
                    faceS.setTexture(lose);
                }
                else if (isDebug && tile.isMine && !tile.isFlag) {
                    window.draw(tile.mineS);
                }
                if (tile.isFlag) {
                    window.draw(tile.flagS);
                }
            }
        }
        window.draw(debugS);
        window.draw(faceS);
        window.draw(pauseS);
        for (auto& digit : digitsSV) {
            window.draw(digit);
        }
        for (auto& digit : digitsSVT) {
            window.draw(digit);
        }
        window.display();
    }
}