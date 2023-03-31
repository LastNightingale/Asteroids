#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include <vector>
#include <windows.h>
#include <SFML/Window.hpp>
#include <fstream>
#include <string>
#include <sstream>

const double M_PI = 3.14159265358979323846;
using namespace std;
sf::Texture tbutStart, tbutExit, tbutRestart, tstartMenu, tbacktomenu, tbutbacktomenu;
sf::Texture tbutRecords, tContinue, tenterMenu, tRecordMenu;
sf::Texture bAsteroid, lAsteroid, mAsteroid, tUfo, tmenuStart, tmenuEnd, topening;
sf::Font font;

class Projectile
{
private:
    sf::RectangleShape projectile{};
    int x, y;   
    sf::Vector2f direction;
public:
    int angle;
    bool isDeleted;
    Projectile(sf::ConvexShape player)
    {
        projectile.setSize(sf::Vector2f(5, 5));
        projectile.setPosition(player.getPosition());
        projectile.setFillColor(sf::Color::White);
        angle = player.getRotation() - 90.0;
        direction = sf::Vector2f(cos((player.getRotation() - 90.0) * M_PI / 180), sin((player.getRotation() - 90.0) * M_PI / 180));
        isDeleted = false;
    }
    sf::FloatRect GetRect()
    {
        return projectile.getGlobalBounds();
    }
    void Move(float dt)
    {
        projectile.move(direction * dt * 500.f);
    }
    void Update(float dt)
    {
        this->Move(dt);
    }
    sf::RectangleShape GetProjectile()
    {
        return projectile;
    }
};

class Asteroid
{
private:
    sf::RectangleShape asteroid{};
    float exist;
    int x, y;
    sf::Vector2f direction;    
public:
    int angle;
    bool isDeleted;
    Asteroid(int x, int y, int angle, int size)
    {
        this->angle = angle;
        this->x = x;
        this->y = y;
        asteroid.setOrigin(size / 2.f, size / 2.f);
        asteroid.setPosition(x, y);
        asteroid.setSize(sf::Vector2f(size, size));
        this->direction = sf::Vector2f(cos(angle * M_PI / 180.0), sin(angle * M_PI / 180.0));
        if (asteroid.getSize().x == 75) asteroid.setTexture(&lAsteroid);
        if (asteroid.getSize().x == 100) asteroid.setTexture(&mAsteroid);
        if (asteroid.getSize().x == 125) asteroid.setTexture(&bAsteroid);
        int version = rand() % 4 + 1;
        for (int i = 0; i < version; ++i) asteroid.rotate(90.f);        
        isDeleted = false;
    }
    sf::FloatRect GetRect() 
    {
        return asteroid.getGlobalBounds();
    }
    void Move(float dt)
    {
        asteroid.move(direction * dt * 200.f);
    }
    void Update(float dt)
    {
        this->Move(dt); 
    }   
    sf::RectangleShape GetAsteroid()
    {
        return asteroid;
    }
};

class UFO
{
private:
    sf::RectangleShape ufo{};
    float exist;
    sf::Vector2f direction;
public:
    bool isDeleted;
    bool fire;
    sf::Clock fly;
    UFO(int x, int y, int angle)
    {    
        this->fire = false;
        exist = 0;
        ufo.setOrigin(50, 25);
        ufo.setPosition(x, y);
        ufo.setSize(sf::Vector2f(100,50));
        this->direction = sf::Vector2f(cos(angle * M_PI / 180.0), sin(angle * M_PI / 180.0));
        ufo.setTexture(&tUfo);
        isDeleted = false;
    }
    sf::Vector2f GetPosition()
    {
        return ufo.getPosition();
    }
    void Move(float dt)
    {        
        ufo.move(direction * dt * 150.f);       
    }
    sf::RectangleShape GetUFO()
    {
        return ufo;
    }
    sf::FloatRect GetRect()
    {
        return ufo.getGlobalBounds();
    }
    void Draw(sf::RenderWindow *window)
    {
        (*window).draw(ufo);
    }
};

class UFOProjectile
{
private:
    sf::RectangleShape ufoprojectile{};
    float exist;
    int x, y;
    sf::Vector2f direction;
public:
    bool isDeleted;
    UFOProjectile(sf::RectangleShape ufo, sf::Vector2f direction)
    {
        srand(time(NULL));
        int angle = rand() % 361;
        exist = 0;
        ufoprojectile.setSize(sf::Vector2f(5, 5));
        ufoprojectile.setPosition(ufo.getPosition());
        this->direction = (direction);
        isDeleted = false;
    }
    void Move(float dt)
    {
        ufoprojectile.move(sf::Vector2f(direction.x/ sqrt(pow(direction.x,2) + pow(direction.y, 2)),
            direction.y / sqrt(pow(direction.x, 2) + pow(direction.y, 2)))* 400.f *dt);
    }
    sf::RectangleShape GetUP()
    {
        return ufoprojectile;
    }
    sf::FloatRect GetRect()
    {
        return ufoprojectile.getGlobalBounds();
    }
    void Draw(sf::RenderWindow* window)
    {
        (*window).draw(ufoprojectile);
    }
};

class Player
{
private:   
    sf::ConvexShape player;    
    int angle;
    int health;
public:
    sf::Vector2f direction;
    int score;
    Player(int health, int score)
    {
        player.setPointCount(4);
        player.setPoint(0, sf::Vector2f(720, 470));
        player.setPoint(1, sf::Vector2f(700, 450));
        player.setPoint(2, sf::Vector2f(680, 470));
        player.setPoint(3, sf::Vector2f(700, 430));
        player.setFillColor(sf::Color::Black);
        player.setOutlineColor(sf::Color::White);
        player.setOutlineThickness(2);
        player.setOrigin(sf::Vector2f(700, 450));
        player.setPosition(sf::Vector2f(700, 450));
        direction = sf::Vector2f(0, -1);
        this->health = health;
        this->score = score;
    }
    int GetHealth()
    {
        return health;
    }
    sf::Vector2f GetDirection()
    {
        return direction;
    }
    int HealthDecrese()
    {
        return --health;
    }
    sf::FloatRect GetRect()
    {
        return player.getGlobalBounds();
    }
    void MoveInertion(float dt, sf::Vector2f direction)
    {
       player.move(dt * 100 * direction);
    }
    void Move(float dt)
    {
        player.move(dt * cos((player.getRotation() - 90.0) * M_PI / 180) * 200, sin((player.getRotation() - 90.0) * M_PI / 180) * dt * 200);
        direction = sf::Vector2f(cos((player.getRotation() - 90.0) * M_PI / 180), sin((player.getRotation() - 90.0) * M_PI / 180));
    }
    sf::Vector2f GetPosition()
    {
        return player.getPosition();
    }
    sf::ConvexShape GetPlayer()
    {
        return player;
    }    
    void Rotate(float direction)
    {
        player.rotate(direction);
    }
    void Update(float dt)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) Rotate(-5.f); 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) Rotate(5.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) Move(dt);
    }
};

class Game
{
private:
    sf::RectangleShape endMenu, butRestart, butEnd, startMenu, butStart, butExit, butRecords, recordMenu, beforestart;
    sf::RectangleShape top1, top2, top3, backtomenu, enterMenu, enterName, toEnd, butToMenu, forlives, forcurrentcore;
    Player player{ 3, 0 };
    sf::Clock while_is_open;
    float program_time, dt, opening;
    int integernumber, side, meteor_angle, meteor_size;
    sf::RenderWindow window{ sf::VideoMode(1400,900), "Asteroids", sf::Style::Default };    
    list<UFO> UFOs;
    list<UFOProjectile> UFOProjectiles;
    list<Asteroid> Asteroids;
    list<Projectile> Projectiles;   
    ofstream fout;
    ifstream fin;
    void (Game::* update)(float dt);
    void (Game::* draw)();  
    sf::Text text, name, record1, record2, record3, lives, current_score;
    string enterText, path;
    bool toWrite, isinEnd, isStarted, isinStart, isopening;
public:
    bool inertion;
    Game()
    {   
        opening = 0;
        isopening = true;
        isinStart = false;
        path = "AsteroidsRecords.txt";
        isinEnd = false;
        toWrite = false;
        inertion = false;
        isStarted = false;
        update = &Game::UpdateOpening;
        draw = &Game::DrawOpening;
        beforestart.setSize(sf::Vector2f(1400, 900));
        forlives.setSize(sf::Vector2f(150, 50));
        forlives.setPosition(3, 3);
        forlives.setFillColor(sf::Color::Black);
        forlives.setOutlineThickness(3);
        forcurrentcore.setSize(sf::Vector2f(200, 50));
        forcurrentcore.setPosition(1197, 3);
        forcurrentcore.setFillColor(sf::Color::Black);
        forcurrentcore.setOutlineThickness(3);
        startMenu.setPosition(300, 0);
        startMenu.setSize(sf::Vector2f(800, 900));
        recordMenu.setPosition(300, 0);
        recordMenu.setSize(sf::Vector2f(800, 900));
        butStart.setPosition(sf::Vector2f(475, 400));
        butStart.setSize(sf::Vector2f(450, 75));
        butRecords.setPosition(sf::Vector2f(475, 500));
        butRecords.setSize(sf::Vector2f(450, 75));
        butExit.setPosition(sf::Vector2f(475, 600));
        butExit.setSize(sf::Vector2f(450, 75));
        enterMenu.setPosition(350, 250);
        enterMenu.setSize(sf::Vector2f(700, 400));
        enterName.setSize(sf::Vector2f(600, 75));
        enterName.setPosition(400, 400);
        enterName.setFillColor(sf::Color::Black);
        enterName.setOutlineThickness(3);
        enterName.setOutlineColor(sf::Color(127, 127, 127));
        toEnd.setSize(sf::Vector2f(150, 75));
        toEnd.setPosition(sf::Vector2f(625, 525));

        backtomenu.setPosition(sf::Vector2f(650, 700));
        backtomenu.setSize(sf::Vector2f(300, 75));
        top1.setPosition(sf::Vector2f(350, 300));
        top1.setSize(sf::Vector2f(700, 75));
        top2.setPosition(sf::Vector2f(350, 400));
        top2.setSize(sf::Vector2f(700, 75));
        top3.setPosition(sf::Vector2f(350, 500));
        top3.setSize(sf::Vector2f(700, 75));
        top1.setFillColor(sf::Color::Black);
        top2.setFillColor(sf::Color::Black);
        top3.setFillColor(sf::Color::Black);
        top1.setOutlineColor(sf::Color::White);
        top1.setOutlineThickness(3);
        top2.setOutlineColor(sf::Color::White);
        top2.setOutlineThickness(3);
        top3.setOutlineColor(sf::Color::White);
        top3.setOutlineThickness(3);
        toEnd.setTexture(&tContinue);

        endMenu.setPosition(300, 0);
        endMenu.setFillColor(sf::Color::White);
        endMenu.setSize(sf::Vector2f(800, 900));
        butRestart.setSize(sf::Vector2f(450, 75));
        butToMenu.setSize(sf::Vector2f(450, 75));
        butEnd.setSize(sf::Vector2f(450, 75));
        butRestart.setPosition(sf::Vector2f(475, 400));
        butToMenu.setPosition(475, 500);
        butEnd.setPosition(sf::Vector2f(475, 600));
        meteor_angle = 0;
        integernumber = 1;
        window.setFramerateLimit(60);
        side = 0;        
        endMenu.setTexture(&tmenuEnd);
        startMenu.setTexture(&tmenuStart);
        butEnd.setTexture(&tbutExit);
        butRestart.setTexture(&tbutRestart);
        butExit.setTexture(&tbutExit);
        butStart.setTexture(&tbutStart);
        butRecords.setTexture(&tbutRecords);
        enterMenu.setTexture(&tenterMenu);
        backtomenu.setTexture(&tbacktomenu);
        recordMenu.setTexture(&tRecordMenu);
        butToMenu.setTexture(&tbutbacktomenu);
        beforestart.setTexture(&topening);
        text = sf::Text("", font, 48);
        text.setStyle(sf::Text::Bold);
        text.setPosition(760, 305);
        SetRecordsText();
        SetName();
        enterText = "";
    }     
    void SetDataOnScreen()
    {
       lives = current_score = sf::Text("", font, 32);
       lives.setPosition(8, 3);
       current_score.setPosition(1202, 3);
    }
    void SetRecordsText()
    {
        record2 = record3 = record1 = sf::Text("", font, 48);
        record1.setStyle(sf::Text::Bold);
        record2.setStyle(sf::Text::Bold);
        record3.setStyle(sf::Text::Bold);
        record1.setPosition(355, 305);
        record2.setPosition(355, 405);
        record3.setPosition(355, 505);
    }
    void SetName()
    {
        name = sf::Text("", font, 48);
        name.setStyle(sf::Text::Bold);
        name.setPosition(410, 410);
    }
    void Ressurect()
    {
        player = Player(player.HealthDecrese(), player.score);
        Projectiles.clear();
        Asteroids.clear();
        UFOs.clear();
        UFOProjectiles.clear();
        while_is_open.restart();
        integernumber = 1;
        inertion = false;
    }
    void SetUFOProjectile(UFO &ufo)
    {        
            if (ufo.fly.getElapsedTime().asSeconds() > 3 && !ufo.fire)
            {               
                ufo.fire = true;
                sf::Vector2f newdirection = sf::Vector2f(player.GetPosition() - ufo.GetPosition());
                UFOProjectile bullet(ufo.GetUFO(), player.GetPosition() - ufo.GetPosition());
                UFOProjectiles.push_back(bullet);
            }
    }
    void SetUFO()
    {
        int x = 0, y = 0, ufo_angle = 0;
        side = rand() % 4 + 1;
        switch (side)
        {
        case 1:
            x = rand() % 1400 + 1;
            y = 0;
            if (x >= 700) ufo_angle = rand() % 91 + 90;
            else ufo_angle = rand() % 91;
            break;
        case 2:
            x = 0;
            y = rand() % 900 + 1;
            if (y >= 450) ufo_angle = rand() % 91 + 270;
            else ufo_angle = rand() % 91;
            break;
        case 3:
            x = rand() % 1400;
            y = 900;
            if (x >= 700)  ufo_angle = rand() % 91 + 180;
            else ufo_angle = rand() % 91 + 270;
            break;
        case 4:
            x = 1400;
            y = rand() % 900 + 1;
            if (y >= 450) ufo_angle =  rand() % 91 + 270;
            else  ufo_angle = rand() % 91 + 90;
            break;
        }
        UFO ufo(x, y, ufo_angle);
        UFOs.push_back(ufo);
    }
    void SetAsteroid()
    {
        int x = 0, y = 0, size;        
            side = rand() % 4 + 1;
            switch (side)
            {
            case 1:
                x = rand() % 1400 + 1;
                y = 0;
                if (x >= 700) meteor_angle = rand() % 91 + 90;
                else meteor_angle = rand() % 91;
                break;
            case 2:
                x = 0;
                y = rand() % 900 + 1;
                if (y >= 450) meteor_angle = rand() % 91 + 270;
                else meteor_angle = rand() % 91;
                break;
            case 3:
                x = rand() % 1400;
                y = 900;
                if (x >= 700)  meteor_angle = rand() % 91 + 180;
                else meteor_angle = rand() % 91 + 270;
                break;
            case 4:
                x = 1400;
                y = rand() % 900 + 1;
                if (y >= 450) meteor_angle = rand() % 91 + 180;
                else  meteor_angle = rand() % 91 + 90;
                break;
            }
            size = (rand() % 3 + 1) * 25 + 50;
            Asteroid asteroid(x, y, meteor_angle, size);
            Asteroids.push_back(asteroid);
    }
    void Readrecords()
    {
        SetRecordsText();
        string scores[3]; int i = 0; string names[3];
        fin.open(path);
        while (fin >> scores[i] >> names[i])
        {
            for (int j = 0; j < names[i].size(); ++j) if (names[i][j] == '_') names[i][j] = ' ';
            ++i;
        }
        for (int j = 0; j < 3; ++j) cout << scores[j] << " " << names[j] << endl;
        fin.close();
        record1.setString(names[0] + " : " + scores[0]);
        record2.setString(names[1] + " : " + scores[1]);
        record3.setString(names[2] + " : " + scores[2]);
    }
    void EnterRecord()
    {
        int scores[3]{ 0 }, i = 0; string names[3]{"No_name","No_name","No_name"};
        if (enterText.size() == 0) enterText = "No_name";
        for (int j = 0; j < enterText.size(); ++j)
        {
            if (enterText[j] == ' ') enterText[j] = '_';
        }
        fin.open(path);
        while (fin >> scores[i] >> names[i]) ++i;
        fin.close();
        for (int j = 0; j < 3; ++j)
        {
            if (player.score > scores[j])
            {
                for (int k = j + 1; k < 2; ++k)
                {
                    scores[k] = scores[k + 1];
                    names[k] = names[k + 1];
                }
                scores[j] = player.score;
                names[j] = enterText;
                break;
            }
        }  
        for (int j=0;j<3;++j) cout<< scores[j] << " " << names[j] << endl;
        fout.open(path);
        for (int j = 0; j < 3; j++)
        {
            fout << scores[j] << " " << names[j] << endl;
        }
        fout.close();
    }
    void UpdateOpening(float dt)
    {
        if (opening >= 3)
        {
            update = &Game::UpdateStart;
            draw = &Game::DrawStart;
            isinStart = true;
            isopening = false;
        }
    }
    void DrawOpening()
    {
        window.clear(sf::Color::Black);
        window.draw(beforestart);
        window.display();
    }
    void UpdateStart(float dt)
    {
        if (isinStart)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                if (butStart.getGlobalBounds().contains(cursor.x, cursor.y))
                {
                    isStarted = true;
                    update = &Game::UpdateRun;
                    draw = &Game::DrawRun;
                    player = Player{ 3 , 0 };
                    player.score = 0;
                    SetName();
                    SetDataOnScreen();
                    isinStart = false;
                }
                if (butRecords.getGlobalBounds().contains(cursor.x, cursor.y))
                {
                    Readrecords();
                    update = &Game::UpdateRecords;
                    draw = &Game::DrawRecords;
                }
                if (butExit.getGlobalBounds().contains(cursor.x, cursor.y))
                {
                    window.close();
                }
            }
        }        
    }
    void DrawStart()
    {
        window.clear(sf::Color::Black);
        window.draw(startMenu);
        window.draw(butStart);
        window.draw(butRecords);
        window.draw(butExit);
        window.display();
    }
    void DrawRun()
    {
        window.clear(sf::Color::Black); 
        for (auto &iter : Projectiles)
        {
            window.draw(iter.GetProjectile());
        }

        for (auto &iter : Asteroids) 
        {
            window.draw(iter.GetAsteroid());
        }

        for (auto& iter : UFOs) 
        {
            iter.Draw(&window);
        }

        for (auto& iter : UFOProjectiles)
        {
            iter.Draw(&window);
        }

        window.draw(player.GetPlayer());
        window.draw(forlives);
        window.draw(forcurrentcore);
        window.draw(current_score);
        window.draw(lives);
        window.display();
    }
    void Collision()
    {
        vector<list<UFO>::iterator> ufoDelete;
        vector<list<Projectile>::iterator> projectileDelete;
        vector<list<Asteroid>::iterator> asteroidDelete;
        vector<list<UFOProjectile>::iterator> ufopDelete;
        for (auto it = Projectiles.begin(); it != Projectiles.end(); ++it)
        {
            for (auto iter = Asteroids.begin(); iter != Asteroids.end(); ++iter)
            {
                if ((*it).GetRect().intersects((*iter).GetRect()))
                {
                    if (!it->isDeleted)
                    {
                        projectileDelete.push_back(it);
                        it->isDeleted = true;
                    }
                    if (!iter->isDeleted)
                    {
                        asteroidDelete.push_back(iter);
                        if ((*iter).GetAsteroid().getSize().x == 125) player.score += 10;
                        if ((*iter).GetAsteroid().getSize().x == 100) player.score += 25;
                        if ((*iter).GetAsteroid().getSize().x == 75) player.score += 50;
                        iter->isDeleted = true;
                        if (iter->GetAsteroid().getSize().x == 125 || iter->GetAsteroid().getSize().x == 100)
                        {
                            sf::Vector2f centre = sf::Vector2f(iter->GetAsteroid().getPosition().x, iter->GetAsteroid().getPosition().y);
                            //double forvector1 = iter->GetAsteroid().getSize().x / (sqrt(pow(cos((iter->angle + 90) * M_PI / 180.f), 2.f))*4.f);
                            //double forvector2 = iter->GetAsteroid().getSize().x / (sqrt(pow(cos((iter->angle - 90) * M_PI / 180.f), 2.f))*4.f);
                            //sf::Vector2f vector1 = sf::Vector2f(cos((iter->angle + 90) * M_PI / 180.f) * forvector1, sin((iter->angle + 90) * M_PI / 180.f)*forvector1);
                            //sf::Vector2f vector2 = sf::Vector2f(cos((iter->angle - 90) * M_PI / 180.f) * forvector2, sin((iter->angle - 90) * M_PI / 180.f) * forvector2);
                            Asteroid asteroid1(centre.x, centre.y, iter->angle - 90, iter->GetAsteroid().getSize().x - 25);
                            Asteroids.push_back(asteroid1);
                            Asteroid asteroid2(centre.x, centre.y, iter->angle + 90, iter->GetAsteroid().getSize().x - 25);
                            Asteroids.push_back(asteroid2);
                        }
                    } 
                }                
            }
        }

        for (auto itery = Projectiles.begin(); itery != Projectiles.end(); ++itery)
        {
            for (auto iter = UFOs.begin(); iter != UFOs.end(); ++iter)
            {
                if ((*itery).GetRect().intersects((*iter).GetRect()))
                {  
                    if (!iter->isDeleted)
                    {
                        ufoDelete.push_back(iter);
                        iter->isDeleted = true;
                    }
                    if (!itery->isDeleted)
                    {
                        projectileDelete.push_back(itery);
                        player.score += 75;
                        itery->isDeleted = true;
                    }
                }
            }
        }

        for (auto itery = Asteroids.begin(); itery != Asteroids.end(); ++itery)
        {
            for (auto iter = UFOs.begin(); iter != UFOs.end(); ++iter)
            {
                if ((*itery).GetRect().intersects((*iter).GetRect()))
                {
                    if (!itery->isDeleted)
                    {
                        asteroidDelete.push_back(itery);
                        itery->isDeleted = true;
                    }
                    if (!iter->isDeleted)
                    {
                        ufoDelete.push_back(iter);
                        iter->isDeleted = true;
                    }
                    if (itery->GetAsteroid().getSize().x == 125 || itery->GetAsteroid().getSize().x == 100)
                    {
                        sf::Vector2f centre = sf::Vector2f(itery->GetAsteroid().getPosition().x, itery->GetAsteroid().getPosition().y);
                        Asteroid asteroid1(centre.x, centre.y, itery->angle - 90, itery->GetAsteroid().getSize().x - 25);
                        Asteroids.push_back(asteroid1);
                        Asteroid asteroid2(centre.x, centre.y, itery->angle + 90, itery->GetAsteroid().getSize().x - 25);
                        Asteroids.push_back(asteroid2);
                    }
                }
            }
        }              

        for (auto it = Asteroids.begin(); it != Asteroids.end(); ++it)
            if ((*it).GetAsteroid().getPosition().x < -50 || (*it).GetAsteroid().getPosition().y < -50 ||
                (*it).GetAsteroid().getPosition().x > 1450 || (*it).GetAsteroid().getPosition().y > 950)
            {
                if (!it->isDeleted)
                {
                    asteroidDelete.push_back(it);
                    it->isDeleted = true;
                }
            }

        for (auto it = Projectiles.begin(); it != Projectiles.end(); ++it)
            if ((*it).GetProjectile().getPosition().x < -50 || (*it).GetProjectile().getPosition().y < -50 ||
                (*it).GetProjectile().getPosition().x > 1450 || (*it).GetProjectile().getPosition().y > 950)
            {                
                if (!it->isDeleted)
                {
                    projectileDelete.push_back(it);
                    it->isDeleted = true;
                }
            }

        for (auto it = UFOs.begin(); it != UFOs.end(); ++it)
            if ((*it).GetUFO().getPosition().x < -50 || (*it).GetUFO().getPosition().y < -50 ||
                (*it).GetUFO().getPosition().x > 1450 || (*it).GetUFO().getPosition().y > 950)
            {
                if (!it->isDeleted)
                {
                    ufoDelete.push_back(it);
                    it->isDeleted = true;
                }
            }

        for (auto it = UFOProjectiles.begin(); it != UFOProjectiles.end(); ++it)
            if ((*it).GetUP().getPosition().x < -50 || (*it).GetUP().getPosition().y < -50 ||
                (*it).GetUP().getPosition().x > 1450 || (*it).GetUP().getPosition().y > 950)
            {
                if (!it->isDeleted)
                {
                    ufopDelete.push_back(it);
                    it->isDeleted = true;
                }
            }

        for (auto& it : projectileDelete)
            Projectiles.erase(it);
            

        for (auto& iter : asteroidDelete)
        {            
                Asteroids.erase(iter);
        }
            

        for (auto& itery : ufoDelete)
            UFOs.erase(itery);

        for (auto& itery : ufopDelete)
            UFOProjectiles.erase(itery);


        bool isDead = false;
        if (player.GetPlayer().getPosition().x < 0 || player.GetPlayer().getPosition().y < 0 ||
            player.GetPlayer().getPosition().x > 1400 || player.GetPlayer().getPosition().y > 900)
            isDead = true;
        for (auto iter = UFOProjectiles.begin(); iter != UFOProjectiles.end(); ++iter)
            if (player.GetRect().intersects((*iter).GetRect()))
            {
                isDead = true;
                break;
            }
        for (auto iter = Asteroids.begin(); iter != Asteroids.end(); ++iter)
            if (player.GetRect().intersects((*iter).GetRect()))
            {
                isDead = true;
                break;
            }
        for (auto iter = UFOs.begin(); iter != UFOs.end(); ++iter)
            if (player.GetRect().intersects((*iter).GetRect()))
            {
                isDead = true;
                break;
            }
        if (isDead) Ressurect();
    }
    void UpdateEnter(float dt)
    {   
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2i cursor = sf::Mouse::getPosition(window);
            if (toEnd.getGlobalBounds().contains(cursor.x, cursor.y))
            {
                cout << enterText << endl;
                update = &Game::UpdateEnd;
                draw = &Game::DrawEnd;
                EnterRecord();
            }
            if (enterName.getGlobalBounds().contains(cursor.x, cursor.y))
            {
                toWrite = true;
                enterName.setOutlineColor(sf::Color::White);
            }
        }
    }
    void DrawEnter()
    {
        window.clear(sf::Color::Black);
        window.draw(enterMenu);
        window.draw(enterName);
        window.draw(toEnd);
        window.draw(name);
        window.display();
    }
    void UpdateRecords(float dt)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2i cursor = sf::Mouse::getPosition(window);
            if (backtomenu.getGlobalBounds().contains(cursor.x, cursor.y))
            {
                update = &Game::UpdateStart;
                draw = &Game::DrawStart;
            }
        }
    }
    void DrawRecords()
    {
        window.clear(sf::Color::Black);
        window.draw(recordMenu);
        window.draw(top1);
        window.draw(top2);
        window.draw(top3);
        window.draw(record1);
        window.draw(record2);
        window.draw(record3);
        window.draw(backtomenu);
        window.display();
    }
    void UpdateRun(float dt)
    {
        if (!player.GetHealth())
        {
            isinEnd = false;
            isStarted = false;
            ostringstream playerScoreString;
            playerScoreString << player.score;
            text.setString(playerScoreString.str());
            //cout << player.score << endl;
            update = &Game::UpdateEnter;
            draw = &Game::DrawEnter;
            toWrite = false;
            enterName.setOutlineColor(sf::Color(127, 127, 127));
            SetName();
            enterText = "";
        }
        ostringstream scoreString, livesString;
        livesString << player.GetHealth();
        scoreString << player.score;
        lives.setString("Lives: " + livesString.str());
        current_score.setString("Score: " + scoreString.str());
        player.Update(dt);
        if (inertion) player.MoveInertion(dt, player.GetDirection());
        for (auto& iter : Projectiles)
        {
            iter.Move(dt);
        }
        for (auto& iter : Asteroids)
        {
            iter.Move(dt);
        }
        for (auto& iter : UFOs)
        {
            iter.Move(dt);           
            SetUFOProjectile(iter);                
        }
        for (auto& iter : UFOProjectiles)
        {
            iter.Move(dt);
        }
        Collision();
    }
    void DrawEnd()
    {
        window.clear(sf::Color::Black);
        window.draw(endMenu);
        window.draw(butRestart);
        window.draw(butToMenu);
        window.draw(butEnd);
        window.draw(text);
        window.display();
    }
    void UpdateEnd(float dt)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2i cursor = sf::Mouse::getPosition(window);
            if (isinEnd)
            {
                if (butRestart.getGlobalBounds().contains(cursor.x, cursor.y))
                {
                    update = &Game::UpdateRun;
                    draw = &Game::DrawRun;
                    player = Player{ 3, 0 };
                    isStarted = true;
                    inertion = false;
                    SetName();
                    SetDataOnScreen();
                }
                if (butToMenu.getGlobalBounds().contains(cursor.x, cursor.y))
                {
                    update = &Game::UpdateStart;
                    draw = &Game::DrawStart;                    
                }
                if (butEnd.getGlobalBounds().contains(cursor.x, cursor.y))
                {
                    window.close();
                }
            }
            
        }        
    }
    void Run()
    {
        srand(time(NULL));
        sf::Clock timer;
        while (window.isOpen())
        {            
            sf::Event event;
            dt = timer.restart().asSeconds();
            if (!isStarted)
            {
                if (isopening) opening += while_is_open.restart().asSeconds();
                else while_is_open.restart();
            }
                
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::EventType::Closed) window.close();
                    if (event.type == sf::Event::EventType::KeyPressed)
                    {
                        if (event.key.code == sf::Keyboard::Key::W) inertion = false;                                              
                    }
                    if (event.type == sf::Event::EventType::KeyReleased)
                        if (event.key.code == sf::Keyboard::Key::Space)
                        {
                            if (isStarted)
                            {
                                Projectile projectile(player.GetPlayer());
                                Projectiles.push_back(projectile);
                            }
                        }
                    if (event.key.code == sf::Keyboard::Key::W) inertion = true;
                    if (update == &Game::UpdateEnd)
                    {
                        if (event.key.code == sf::Mouse::Button::Left)
                        {
                            isinEnd = true;
                        }
                    }
                    if (update == &Game::UpdateStart)
                    {
                        if (event.key.code == sf::Mouse::Button::Left)
                        {
                            isinStart = true;
                        }
                    }
                    if (update == &Game::UpdateEnter)
                    {
                        if (toWrite)
                        {
                            if (event.type == sf::Event::EventType::TextEntered)
                            {
                                if (event.text.unicode == 8)
                                { 
                                    enterText = enterText.substr(0, enterText.size() - 1);
                                }
                                else if (enterText.size() < 16) 
                                {
                                    enterText += event.text.unicode;
                                }
                            }
                            else name.setString(enterText);
                        }
                    }
                }        

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) inertion = false;

                program_time = while_is_open.getElapsedTime().asSeconds();                

                if (integernumber < program_time)
                {                    
                    //++integernumber;
                    integernumber += 2;                   
                    int n = 0;
                    if (integernumber > 1 && integernumber < 7) n = 1;
                    if (integernumber >= 7 && integernumber < 15) n = 2;
                    if (integernumber > 15) n = 3;
                    if (integernumber > 0 && fmod(integernumber - 10, 3) == 0)
                    {
                        SetUFO();                       
                    }
                    for (int i = 0; i < n; i++) SetAsteroid();
                }                
            (this->*update)(dt);
            (this->*draw)();
        }
    }
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    tbutRestart.loadFromFile("textures/ButtonRestart.png");
    tbutExit.loadFromFile("textures/ButtonExit.png");
    tbutStart.loadFromFile("textures/ButtonStart.png");
    tmenuStart.loadFromFile("textures/Asteroids.png");
    tbutRecords.loadFromFile("textures/ButtonRecords.png");
    lAsteroid.loadFromFile("textures/mMeteor.png");
    bAsteroid.loadFromFile("textures/bMeteor.png");
    mAsteroid.loadFromFile("textures/medMeteor.png");
    tUfo.loadFromFile("textures/modUfo.png");
    tmenuEnd.loadFromFile("textures/Defeat.png");
    tContinue.loadFromFile("textures/ButtonContinue.png");
    tenterMenu.loadFromFile("textures/EnterName.png");
    tbacktomenu.loadFromFile("textures/BackToMenu.png");
    tRecordMenu.loadFromFile("textures/TopScores.png");
    tbutbacktomenu.loadFromFile("textures/ButtonBackToMenu.png");
    topening.loadFromFile("textures/Opening.png");
    font.loadFromFile("textures/RhodiumLibre-Regular.ttf");
    Game game;
    game.Run();
}