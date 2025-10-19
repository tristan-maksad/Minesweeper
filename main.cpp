#include <SFML/Graphics.hpp>
#include <iostream>
#include "WelcomeWindow.h"
#include "GameWindow.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    string widthS;
    string heightS;
    string minesS;
    ifstream fileIn("files/config.cfg");
    if (!fileIn.is_open()) {
        cerr << "File not open!\n";
        return 0;
    }
    getline(fileIn, widthS);
    int width = 32 * stoi(widthS);
    getline(fileIn, heightS);
    int height = 100 + 32 * stoi(heightS);
    getline(fileIn, minesS);
    int mines = stoi(minesS);
    int tiles = stoi(widthS) * stoi(heightS);
    WelcomeWindow welcomeW(width, height);
    std::string pName = welcomeW.run();
    pName[0] = toupper(pName[0]);
    if (pName != "") {
        GameWindow gameW(width, height, mines, pName);
    }
    return 0;
}