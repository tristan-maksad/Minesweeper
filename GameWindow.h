#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <SFML/Graphics.hpp>
#include <random>
using namespace std;

class GameWindow {
    sf::RenderWindow window;
    sf::Font font;

    sf::Texture debug;
    sf::Texture digits;
    sf::Texture happy;
    sf::Texture lose;
    sf::Texture win;
    sf::Texture flag;
    //Scrapped leaderboard mechanic texture
    sf::Texture leaderboard;
    sf::Texture mine;
    sf::Texture one;
    sf::Texture two;
    sf::Texture three;
    sf::Texture four;
    sf::Texture five;
    sf::Texture six;
    sf::Texture seven;
    sf::Texture eight;
    sf::Texture pause;
    sf::Texture play;
    sf::Texture tileHidden;
    sf::Texture tileRevealed;

    sf::Sprite debugS;
    sf::Sprite faceS;
    sf::Sprite pauseS;
    //Scrapped leaderboard mechanic sprite
    sf::Sprite leaderboardS;
    sf::Sprite digitsS;

    struct Tile {
        sf::Sprite hiddenS;
        sf::Sprite revealedS;
        sf::Sprite revealedS2;
        sf::Sprite mineS;
        sf::Sprite flagS;
        sf::Sprite oneS;
        sf::Sprite twoS;
        sf::Sprite threeS;
        sf::Sprite fourS;
        sf::Sprite fiveS;
        sf::Sprite sixS;
        sf::Sprite sevenS;
        sf::Sprite eightS;
        bool isRevealed;
        bool isMine;
        bool isFlag;
        int mineCount;
    };

    vector<Tile> tiles;
    vector<sf::Sprite> digitsSV;

    bool gameOverW;
    bool gameOverL;
    int flagCount;
    bool isDebug;

    sf::Clock clock;
    sf::Time timeT;
    int timeP;
    sf::Time timeTT;
    bool isPaused;
    sf::Sprite digitsST;
    vector<sf::Sprite> digitsSVT;

    string pName;

public:
    GameWindow(int width, int height, int mines, string name);
    void loadFiles();
    void setScreen(int cols, int rows, int mines);
    void run(int cols, int rows, int mines);
    void tileLC(int x, int y, int cols, int rows, int mines);
    void tileRC(int x, int y);
    void mineFunc(int cols, int rows, int mines);
    void blanksNear(int i, int cols, int rows, bool onRight, bool onLeft, bool onUp, bool onDown);
    void setCounter(int flagCount);
    void timer();
    void gameWon(int mines);
    void gameReset(int cols, int rows, int mines);
    void gamePause();
};

#endif