#include <SFML/Graphics.hpp>
#include<iostream>
#include<fstream>
#include "tictactoe.h"
#include "button.h"
using namespace std;
sf::RenderWindow window;
sf::Sprite Bckgrnd, PlayerX, PlayerO, Board;
TicTacToe TTT;
Button Player1, Player2, Menu1, Menu2,Empty;
sf::Font font;
bool Usr = 1,won=0, prenk=0, unfprenk=0, unwtdprenk=0, emptyprenk=0;
struct Message
{
    sf::Text text;
    void setmsg(string s,int sz=30)
    {
        text.setString(s);
        text.setCharacterSize(sz);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setFillColor(sf::Color(0, 0, 0));
    }
    void setPosition(float x, float y)
    {
        text.setPosition(sf::Vector2f(x, y));
    }
}Msg;
struct M
{
    string s;
    int sz;
} msgs[] = {
    {"Hah!\n Don't make me laugh,\n\t\tyou don't have friends",30},
    {"Oh look, it's retarded,\n\t it's clicking on empty spaces",30},
    {"Why are you even trying...", 30},
    {"What's the point,\n\t\tYou know you'll never win", 30},
    {"Might as well\n\t\tjust play with myself\n\t\t\t\t\t\tat this point", 27},
    {"Look at you,\n\n\t\ta 3 dimensional being,\nplaying a 2 dimensional game,\n\twith a 1 dimensional brain", 27},
    {"Sometimes,\n\t\t life isn't fair", 30},
    {"Sometimes,\n\tyou don't get what you want", 30},
    {"Ever installed a virus?\nit's the most exhilarating thing you can do on a pc,\n\t\t\twanna try one?", 17}
};
string your_mine ="YOU'RE MINE ";
int unf = 7, unwtd = 8;
const int MsgSize = (sizeof(msgs) / sizeof(*msgs));
bool done[MsgSize] = {};
int Event = 2, minimum=3, maximum=8;
string vrsmsgs[] = {
        "MINE",
        "YOU'RE DEAD",
        "ALL YOU DATA DELETED",
        "ALL YOU DATA GONE",
        "ALL YOUR DATA CORRUPTED",
        "GONE",
        "DUMBASS",
        "SAY YOUR GOOD BYES",
        "YOU IDIOT"
};
int vrsmsgsize = 9, vrscnt = 0;
const int viruses = 20;
sf::RenderWindow vrs[viruses];
sf::Text vrsmsg[viruses];
sf::Color vrsclrs[] = {
    sf::Color(143, 0, 0),
    sf::Color(0, 9, 110),
    sf::Color(40, 40, 40),
    sf::Color(0, 94, 9),
    sf::Color(28, 0, 74)
};
int aclrs[viruses] = {};
bool parityclr[viruses] = {};
bool FadeIn()
{
    static int alpha = 255;
    Bckgrnd.setColor(sf::Color(255, 255, 255, alpha));
    window.draw(Bckgrnd);
    Bckgrnd.setColor(sf::Color(255, 255, 255, 255));
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
    Bckgrnd.setColor(sf::Color(255, 255, 255, alpha));
    window.draw(Bckgrnd);
    Bckgrnd.setColor(sf::Color(255, 255, 255, 255));
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
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
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
void SetColor(sf::Sprite& Player, int mv)
{
    for (int k = 0; k < 3; k++)
    {
        if (TTT.win[k] == mv)
        {
            Player.setColor(sf::Color(255, 255, 255, 255));
            return;
        }
    }
    Player.setColor(sf::Color(255, 255, 255, 127));
}
bool DrawNwMv(short mv,bool early=0)
{
    static short alpha = 0;
    if (early)
    {
        alpha = 0;
        return 1;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (TTT.board[i][j] == 1 || (unfprenk && TTT.board[i][j] !=0))
            {
                PlayerO.setPosition(j * 150, i * 150);
                if (mv == i * 3 + j)
                {
                    PlayerO.setColor(sf::Color(255, 255, 255, alpha));
                    window.draw(PlayerO);
                    PlayerO.setColor(sf::Color(255, 255, 255, 255));
                }
                else
                {
                    window.draw(PlayerO);
                }
                
            }
            else if (TTT.board[i][j] == -1)
            {
                PlayerX.setPosition(j * 150, i * 150);
                if (mv == i * 3 + j)
                {
                    PlayerX.setColor(sf::Color(255, 255, 255, alpha));
                    window.draw(PlayerX);
                    PlayerX.setColor(sf::Color(255, 255, 255, 255));
                }
                else
                {
                    window.draw(PlayerX);
                }
            }
        }
    }
    alpha += 3;
    if (alpha >255)
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
            if (TTT.board[i][j] == 1 || (unfprenk && TTT.board[i][j]!=0))
            {
                PlayerO.setPosition(j * 150, i * 150);
                if (won)
                    SetColor(PlayerO, i * 3 + j);
                window.draw(PlayerO);
            }
            else if (TTT.board[i][j] == -1)
            {
                PlayerX.setPosition(j * 150, i * 150);
                if (won)
                    SetColor(PlayerX, i * 3 + j);
                window.draw(PlayerX);
            }
        }
    }
}
bool GameScene()
{
    static bool  drawingDone=1,turn=0;
    static short mvToDraw;
    bool gamedone = 0;
    short UsrMove = getUsrMove();
    window.draw(Board);
    if (!won && !TTT.isDraw() && drawingDone)
    {
        if (UsrMove!=-1 && TTT.getMove(UsrMove)==0 && turn == Usr)
        {
            if (!unwtdprenk)
                TTT.makeMove(UsrMove);
            else
                TTT.randMove(UsrMove);
            drawingDone = 0, mvToDraw = UsrMove, won = TTT.isWin(), turn = !turn;
        }
        else if (turn != Usr)
        {
            int best = TTT.getBestMove();
            TTT.makeMove(best);
            drawingDone = 0, mvToDraw = best, won = TTT.isWin(), turn = !turn;
        }
    }
    else if (UsrMove != -1)
    {
        if (!drawingDone) drawingDone = DrawNwMv(mvToDraw,1);
        else
        {
            gamedone = 1, turn = 0;
        }
    }
    if (drawingDone)
        DrawBoard();
    else
        drawingDone=DrawNwMv(mvToDraw);
    return gamedone;
}
bool gotoGameScene()
{
    static int state = 0;
    if(state!=3)
        window.draw(Board);
    switch (state)
    {
    case 0:
        state += FadeIn();
        break;
    case 1:
        state += GameScene();
        break;
    case 2:
        DrawBoard();
        state += FadeOut();
        break;
    case 3:
        state = 0;
        TTT.reset();
        won = 0;
        PlayerO.setColor(sf::Color::White);
        PlayerX.setColor(sf::Color::White);
        return 1;
    }
    return 0;
}
bool SelectPlayer()
{
    static short state = 0;
    static int ticks = 0;
    sf::Vector2i pos;
    bool isclicked;
    switch (state)
    {
    case 0:
        Msg.setmsg("TicTacToe", 60);
        Msg.text.setPosition(225, 30);
        state = 1;
    case 1:
        window.draw(Msg.text);
        window.draw(Player1.text);
        window.draw(Player2.text);
        state += FadeIn();
        break;
    case 2:
        window.draw(Msg.text);
        pos = sf::Mouse::getPosition(window);
        isclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (Player1.draw(window, pos, isclicked))Usr = 0, state++;
        else if (Player2.draw(window, pos, isclicked))Usr = 1, state++;
        else if (Empty.ifClick(window, pos, isclicked) && !emptyprenk) state = 5;
        break;
    case 3:
        window.draw(Msg.text);
        window.draw(Player1.text);
        window.draw(Player2.text);
        state += FadeOut();
        break;
    case 4:
        state = 0;
        return 1;
    case 5:
        Msg.setmsg(msgs[1].s, msgs[1].sz);
        Msg.setPosition(225, 225);
        emptyprenk = 1;
        state = 6;
    case 6:
        window.draw(Msg.text);
        if (FadeIn())state = 7;
        break;
    case 7:
        window.draw(Msg.text);
        if (++ticks>600 && getUsrMove() != -1) state = 0,ticks=0;
        
        break;
    }
    return 0;
}
bool menu()
{
    static bool first = 1;
    static int state = 0;
    sf::Vector2i pos;
    bool isclicked;
    if (first)
    {
        Msg.setmsg("TicTacToe", 60);
        Msg.text.setPosition(225, 30);
        first = 0;
    }

    switch (state)
    {
    case 0:
        window.draw(Msg.text);
        if (!prenk)
            window.draw(Menu2.text);
        window.draw(Menu1.text);
        state += FadeIn();
        break;
    case 1:
        window.draw(Msg.text);
        pos = sf::Mouse::getPosition(window);
        isclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (!prenk)
        {
            if (Menu2.draw(window, pos, isclicked))
                state = 3;
        }
        if (Menu1.draw(window, pos, isclicked))
            state = 7;
        break;
    case 3:
        window.draw(Msg.text);
        window.draw(Menu1.text);
        window.draw(Menu2.text);
        if (FadeOut())
        {
            state = 4;
            Msg.setmsg(msgs[0].s, msgs[0].sz);
            Msg.setPosition(225, 225);
        }
        break;
    case 4:
        window.draw(Msg.text);
        if (FadeIn())state = 5;
        break;
    case 5:
        window.draw(Msg.text);
        if (getUsrMove() != -1)state = 6;
        break;
    case 6:
        window.draw(Msg.text);
        if (FadeOut())
        {
            first = 1, prenk = 1, state = 0;
            Menu1.location(225, 225);
        }
        break;
    case 7:
        window.draw(Msg.text);
        if (!prenk)
            window.draw(Menu2.text);
        window.draw(Menu1.text);
        if (FadeOut())state = 8;
        break;
    case 8:
        first = 1, state = 0;
        Menu1.location(225.0f, 225.0f - Menu1.height / 2.0f);
        return 1;
    }
    return 0;

}
void fileout()
{
    ofstream file;
    file.open("UwU");
    file << minimum << " " << Event<<" " << maximum;
    file.close();
}
string repeater(string v, unsigned long long exp)
{
    string ans = "";
    while (exp > 0)
    {
        if (exp & 1)ans += v;
        exp >>= 1;
        v += v;
    }
    return ans;
}
bool block = 0;
sf::Vector2i blkerpos;
void blocker()
{
    while (block)
        sf::Mouse::setPosition(blkerpos);
}
void Virus()
{
    if (Event == -2 || Event<-3) return;
    if (Event == -1)
    {
        int state = 0;
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
            }
            window.clear();
            window.draw(Bckgrnd);
            switch (state)
            {
            case 0:
                Msg.setmsg("wow...\n\t\t  you acutally came back...", 25);
                Msg.setPosition(225, 225);
                state = 1;
            case 1:
                window.draw(Msg.text);
                if (getUsrMove() != -1) state = 2;
                break;
            case 2:
                Msg.setmsg("Well, it was a joke,\n\tthere is no virus\n\t\t if you were wondering", 25);
                Msg.setPosition(225, 225);
                state = 3;
            case 3:
                window.draw(Msg.text);
                if (getUsrMove() != -1) state = 4;
                break;
            case 4:
                Msg.setmsg("From now on plain old TicTacToe\n\t\tNo more gags", 25);
                Msg.setPosition(225, 225);
                state = 5;
            case 5:
                window.draw(Msg.text);
                if (getUsrMove() != -1) state = 6;
                break;
            case 6:
                Msg.setmsg("Unless you delete the UwU file,\n\t well then u start over", 25);
                Msg.setPosition(225, 225);
                state = 7;
            case 7:
                window.draw(Msg.text);
                if (getUsrMove() != -1)
                {
                    window.display();
                    Event = -2;
                    fileout();
                    return;
                }
            }
            window.display();
        }
    }
    window.display();
    int state = 0, tick = 0, annoy = 3;
    if (Event == -3)state = 5;
    sf::Text Cls("Oh you think you control the close button??", font, 20),
        Clk("DUMBASS....\n\t\tYOU BELONG TO ME NOW\n\t\t\t\t\tYOU'RE MINE", font, 25);
    bool close = 0, reg = 0, clicked = 0, wrt = 0;
    Cls.setFillColor(sf::Color::Black);
    Clk.setFillColor(sf::Color(138, 3, 3));
    Clk.setPosition(50, 300);
    Msg.setmsg(msgs[MsgSize - 1].s, msgs[MsgSize - 1].sz);
    Msg.setPosition(225, 225);
    sf::Thread blker(&blocker);
    while (window.isOpen())
    {
        sf::Event event;
        close = 0;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                close = 1;
        }
        window.clear();
        switch (state)
        {
        case 0:
            window.draw(Bckgrnd);
            window.draw(Msg.text);
            if (FadeIn()) state = 1,Event=-3,fileout();
            break;
        case 1:
            window.draw(Bckgrnd);
            window.draw(Msg.text);
            if (getUsrMove() != -1 && !clicked)
            {
                clicked = 1;
                block = 1, blkerpos = sf::Mouse::getPosition();
                blker.launch();
            }
            if (close)
            {
                //138, 3, 3
                if (reg)
                {
                    sf::Color tmp = Bckgrnd.getColor();
                    tmp.r -= (tmp.r - 138.0f) / 4.5f;
                    tmp.g -= (tmp.g - 3.0f) / 4.5f;
                    tmp.b -= (tmp.b - 3.0f) / 4.5f;
                    Bckgrnd.setColor(tmp);
                }
                reg = 1;
            }
            if (clicked)
                window.draw(Clk), tick++;
            if (reg) window.draw(Cls);
            if (tick >= 2000)state = 2, tick = 0;
            break;
        case 2:
            if (!wrt)
            {
                Msg.setmsg("installing virus...", 10);
                Msg.text.setFillColor(sf::Color::White);
                Msg.setPosition(225, 225);
                window.close();
                window.create(sf::VideoMode(450, 450), "", sf::Style::None);
                window.requestFocus();
                reg = 0;
                wrt = 1;
                Event = -1;
                fileout();
            }
            window.draw(Msg.text);
            if (tick <= 0)
            {
                if (vrscnt < viruses)
                {
                    if (vrscnt != 0)
                    {
                        vrs[vrscnt].create(sf::VideoMode(450, 100), "", sf::Style::None);
                        sf::Vector2i temp;
                        temp.x = RAND(0, sf::VideoMode::getDesktopMode().width - 450);
                        temp.y = RAND(0, sf::VideoMode::getDesktopMode().height - 100);
                        vrs[vrscnt].setPosition(temp);
                        vrs[vrscnt].requestFocus();
                        vrsmsg[vrscnt].setFont(font);
                        vrsmsg[vrscnt].setString(vrsmsgs[RAND(0, vrsmsgsize - 1)]);
                        aclrs[vrscnt] = RAND(0, 4);
                        parityclr[vrscnt] = RAND(0, 1);
                    }
                    else
                    {
                        sf::VideoMode temp = sf::VideoMode::getDesktopMode();
                        vrs[vrscnt].create(temp, "", sf::Style::Fullscreen);
                        vrsmsg[vrscnt].setFont(font);
                        your_mine = repeater(your_mine, ceil(temp.width / 408.0f));
                        your_mine += '\n';
                        your_mine = repeater(your_mine, ceil(temp.height / 69.0f));
                        vrsmsg[vrscnt].setString(your_mine);
                        vrsmsg[vrscnt].setFillColor(sf::Color(138, 3, 3));
                        vrsmsg[vrscnt].setCharacterSize(60);
                        window.requestFocus();
                    }
                    vrscnt++;
                }
                else
                {
                    state = 3;
                    for (int i = 0; i < vrscnt; i++)
                        vrs[i].close();
                    vrscnt = 0, tick = 1500;
                    break;
                }
                if (vrscnt == viruses)
                    tick = 500;
                else
                    tick = 200;
            }
            for (int i = 1; i < vrscnt; i++)
            {
                if (parityclr[i])
                    vrs[i].clear(vrsclrs[aclrs[i]]);
                else
                    vrs[i].clear();
                parityclr[i] = !parityclr[i];
                vrs[i].draw(vrsmsg[i]);
                vrs[i].display();
                vrs[i].requestFocus();
                if (i % 4 == 0)tick -= 3;
            }
            vrs[0].clear();
            vrs[0].draw(vrsmsg[0]);
            vrs[0].display();
            if (vrscnt > 1)block = 0;
            tick--;
            break;
        case 3:
            Msg.setmsg("Virus installed!!!!", 10);
            Msg.text.setFillColor(sf::Color(255, 255, 255));
            Msg.setPosition(225, 225);
            state = 4;
        case 4:
            window.draw(Msg.text);
            tick--;
            if (tick <= 0) window.close();
            break;
        case 5:
            Msg.setmsg("DUMBASS", 50);
            Msg.text.setFillColor(sf::Color::Black);
            Msg.setPosition(225, 225);
            Bckgrnd.setColor(sf::Color(138, 3, 3));
            blkerpos.x = window.getPosition().x + 255;
            blkerpos.y = window.getPosition().y + 325;
            block = 1;
            blker.launch();
            state = 6, tick = 700;
        case 6:
            if (tick <= 0)state = 2;
            tick--;
            window.draw(Bckgrnd);
            window.draw(Msg.text);
            break;
        }
        window.display();
    }
}
bool randomEvnts()
{
    static bool onGoing = 0;
    static int state = 0;
    
    if (!onGoing && Event>-1 && Event<MsgSize && !unfprenk && !unwtdprenk)
    {
        maximum--;
        int rndm = RAND(0, 100000);
        if (minimum > 0)
        {
            minimum--;
            fileout();
            return 1;
        }
        
        if (rndm <= 30000 || maximum<1)
        {
            onGoing = 1;
            Msg.setmsg(msgs[Event].s, msgs[Event].sz);
            Msg.setPosition(225, 225);
            Event++;
        }
    }
    if (onGoing)
    {
        if (Event == MsgSize)
        {
            Virus();
            onGoing = 0;
            return 1;
        }
        window.draw(Msg.text);
        switch (state)
        {
        case 0:
            if (FadeIn()) state = 1;
            break;
        case 1:
            if (getUsrMove() != -1) state = 2;
            break;
        case 2:
            if (FadeOut()) state = 3;
            break;
        case 3:
            onGoing = 0,state=0;
            minimum = 2,maximum=8;
            if (Event == unf)unfprenk = 1;
            else if (Event == unwtd)unwtdprenk = 1;
            else fileout();
        }
    }
    return !onGoing;
}
int main()
{
    sf::Texture texture;
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
        ifstream file;
        file.open("UwU");
        if (file.is_open())
        {
            if (!(file >> minimum) || !(file >> Event) || !(file >> maximum))
            {
                ofstream errs;
                errs.open("Error.txt");
                errs << "An error occured reading from persistant data, please delete UwU from current file directory" << endl;
                return 0;
            }
        }
        window.create(sf::VideoMode(450, 450), "TicTacToe", sf::Style::Titlebar | sf::Style::Close);
        Bckgrnd.setTexture(texture);
        Bckgrnd.setTextureRect(sf::IntRect(0, 0, 450, 450));
        PlayerX.setTexture(texture);
        PlayerX.setTextureRect(sf::IntRect(450, 0, 150, 150));
        PlayerO.setTexture(texture);
        PlayerO.setTextureRect(sf::IntRect(450, 150, 150, 150));
        Board.setTexture(texture);
        Board.setTextureRect(sf::IntRect(0, 450, 450, 450));
        Player1.setfnt(font, 40);
        Player2.setfnt(font, 40);
        Menu1.setfnt(font, 40);
        Menu2.setfnt(font, 40);
        Player1.setstr("Player 1", 10);
        Player2.setstr("Player 2", 10);
        Menu1.setstr("Against AI", 10,25);
        Menu2.setstr("Against Human", 10,25);
        Empty.overlay.setSize(sf::Vector2f(10, 430));
        Empty.overlay.setPosition(sf::Vector2f(40, 10));
        Empty.overlay.setFillColor(sf::Color::Black);
        Msg.text.setFont(font);
        Player1.location(225.0f, 225.0f - Player1.height / 2.0f);
        Player2.location(225.0f, 225.0f + Player2.height / 2.0f);
        Menu1.location(225.0f, 225.0f - Menu1.height / 2.0f);
        Menu2.location(225.0f, 225.0f + Menu2.height / 2.0f);
    }
    if (Event <=0)Virus();
    int state = 0, suffer = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear();
        window.draw(Bckgrnd);
        switch (state)
        {
        case 0:
            if (menu())state = 1;
            break;
        case 1:
            if (SelectPlayer()) state = 2;
            break;
        case 2:
            if (randomEvnts()) state = 3;
            break;
        case 3:
            if (gotoGameScene())
            {
                if (prenk) state = 1;
                else state = 0;
                if (unwtdprenk || unfprenk)
                {
                    suffer++;
                    if (suffer == 3)
                        unwtdprenk = 0, unfprenk = 0, suffer = 0;
                }
                if (!unwtdprenk && Event == unwtd) fileout();
                if (!unfprenk && Event == unf) fileout();
            }
            break;
        }
        window.display();
    }
    return 0;
}
