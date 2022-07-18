#include <SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include "tictactoe.h"
#include "button.h"
using namespace std;
sf::Texture texture;
sf::Font font;
sf::RenderWindow window;
sf::Sprite Bckgrnd, PlayerX, PlayerO, BoardLines;
TicTacToe TTT;
Button Player1, Player2;
sf::Text Title;
bool Usr = 0, GameWon=0;
bool FadeIn()
{
    static int alpha = 255;
    static sf::Sprite overlay = Bckgrnd;
    overlay.setColor(sf::Color(255, 255, 255, alpha));
    window.draw(overlay);
    alpha--;
    if (alpha == -1)
    {
        alpha = 255;
        return 1;
    }
    return 0;
}
bool FadeOut()
{
    static int alpha = 0;
    static sf::Sprite overlay = Bckgrnd;
    overlay.setColor(sf::Color(255, 255, 255, alpha));
    window.draw(overlay);
    alpha++;
    if (alpha == 256)
    {
        alpha = 0;
        return 1;
    }
    return 0;
}
short getUsrMove()
{
    static bool mouseClicked = 0;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
        mouseClicked = 1;
    else
    {
        if (mouseClicked)
        {
            mouseClicked = 0;
            if (window.hasFocus())
            {
                auto pos = sf::Mouse::getPosition(window);
                int x = pos.x / 150, y = pos.y / 150;
                if (pos.x >= 0 && x <= 2 && pos.y >= 0 && y <= 2)
                    return y * 3 + x;
            }
        }
    }
    return -1;
}
bool DrawNewMove(int mv, bool early = 0)
{
    static int alpha = 0;
    if (early)
    {
        alpha = 0;
        return 1;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (TTT.board[i][j] == 1)
            {
                if (mv == i * 3 + j)
                    PlayerX.setColor(sf::Color(255, 255, 255, alpha));
                PlayerX.setPosition(j * 150, i * 150);
                window.draw(PlayerX);
                if (mv == i * 3 + j)
                    PlayerX.setColor(sf::Color::White);
            }
            else if (TTT.board[i][j] == -1)
            {
                if (mv == i * 3 + j)
                    PlayerO.setColor(sf::Color(255, 255, 255, alpha));
                PlayerO.setPosition(j * 150, i * 150);
                window.draw(PlayerO);
                if (mv == i * 3 + j)
                    PlayerO.setColor(sf::Color::White);
            }
        }
    }
    alpha += 3;
    if (alpha >= 256)
    {
        alpha = 0;
        return 1;
    }
    return 0;
}
void DrawBoard()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (TTT.board[i][j] == 1)
            {
                PlayerX.setPosition(j * 150, i * 150);
                if (TTT.winpos[i][j])
                    PlayerX.setColor(sf::Color::White);
                window.draw(PlayerX);
                if (TTT.winpos[i][j])
                    PlayerX.setColor(sf::Color(255, 255, 255, 127));
            }
            else if (TTT.board[i][j] == -1)
            {
                PlayerO.setPosition(j * 150, i * 150);
                if (TTT.winpos[i][j])
                    PlayerO.setColor(sf::Color::White);
                window.draw(PlayerO);
                if (TTT.winpos[i][j])
                    PlayerO.setColor(sf::Color(255, 255, 255, 127));
            }
        }
    }
}
bool gameScene()
{
    static int state = 0, Move=-1;
    static bool CurrPlr = 0;
    bool GameDone = 0;
    window.draw(BoardLines);
    switch (state)
    {
    case 0:
        if (CurrPlr == Usr)
            Move = getUsrMove();
        else Move = TTT.getBestMove();
        if (Move != -1)
            state = 1;
        else
        {
            state = 4;
            break;
        }
    case 1:
        if (!TTT.makeMove(Move))
        {
            state = 4;
            break;
        }
        state = 2;
        CurrPlr = !CurrPlr;
    case 2:
        if (DrawNewMove(Move, getUsrMove() != -1)) state = 3;
        break;
    case 3:
        if (TTT.isWin(1))
            GameWon = 1;
        if (TTT.isDraw())
            GameDone = 1;
        state = 4;
    }
    if (state == 4)
    {
        DrawBoard();
        state = 0;
    }
    if (GameWon || GameDone)
    {
        state = 0, CurrPlr = 0;
        return 1;
    }
    return 0;
}
bool LoadGameScn()
{
    static int state = 0;
    switch (state)
    {
    case 0:
        window.draw(BoardLines);
        if (FadeIn())state = 1;
        break;
    case 1:
        if (gameScene()) state = 2;
        break;
    case 2:
        if (GameWon)
        {
            PlayerX.setColor(sf::Color(255, 255, 255, 127));
            PlayerO.setColor(sf::Color(255, 255, 255, 127));
        }
        state = 3;
    case 3:
        window.draw(BoardLines);
        DrawBoard();
        if (getUsrMove() != -1)state = 4;
        break;
    case 4:
        window.draw(BoardLines);
        DrawBoard();
        if (FadeOut())state = 5;
        break;
    case 5:
        if (GameWon)
        {
            PlayerX.setColor(sf::Color(255, 255, 255, 255));
            PlayerO.setColor(sf::Color(255, 255, 255, 255));
            GameWon = 0;
        }
        TTT.reset();
        state = 0;
        return 1;
    }
    return 0;
}
bool SelectPlayer()
{
    static int state = 0;
    window.draw(Title);
    switch (state)
    {
    case 0:
        window.draw(Player1.text);
        window.draw(Player2.text);
        if (FadeIn()) state = 1;
        break;
    case 1:
        if (Player1.draw(window))Usr = 0, state = 2;
        if (Player2.draw(window))Usr = 1, state = 2;
        break;
    case 2:
        window.draw(Player1.text);
        window.draw(Player2.text);
        if (FadeOut())
        {
            state = 0;
            return 1;
        }
        break;
    }
    return 0;
}
int main()
{
    //initialize
    {
        if (!texture.loadFromFile("texture.png"))
        {
            ofstream errs;
            errs.open("Error.txt");
            errs << "An error occured loading texture.png, exiting..." << endl;
            return 0;
        }
        if (!font.loadFromFile("arialbd.ttf"))
        {
            ofstream errs;
            errs.open("Error.txt");
            errs << "An error occured loading arial.ttf, exiting..." << endl;
            return 0;
        }
        window.create(sf::VideoMode(450, 450), "TicTacToe", sf::Style::Titlebar | sf::Style::Close);
        Bckgrnd.setTexture(texture);
        Bckgrnd.setTextureRect(sf::IntRect(0, 0, 450, 450));
        PlayerX.setTexture(texture);
        PlayerX.setTextureRect(sf::IntRect(450, 0, 150, 150));
        PlayerO.setTexture(texture);
        PlayerO.setTextureRect(sf::IntRect(450, 150, 150, 150));
        BoardLines.setTexture(texture);
        BoardLines.setTextureRect(sf::IntRect(0, 450, 450, 450));
        Player1.setfnt(font, 40);
        Player2.setfnt(font, 40);
        Player1.setstr("Player 1", 10);
        Player2.setstr("Player 2", 10);
        Player1.setPosition(225.0f, 225.0f - Player1.height / 2.0f);
        Player2.setPosition(225.0f, 225.0f + Player2.height / 2.0f);
        Title.setFont(font);
        Title.setString("TicTacToe");
        Title.setCharacterSize(60);
        sf::FloatRect textRect = Title.getLocalBounds();
        Title.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        Title.setFillColor(sf::Color::Black);
        Title.setPosition(225, 30);
    }
    int state = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(Bckgrnd);
        switch (state)
        {
        case 0:
            if (SelectPlayer())state = 1;
            break;
        case 1:
            if (LoadGameScn()) state = 0;
            break;
        }
        window.display();
    }

    return 0;
}