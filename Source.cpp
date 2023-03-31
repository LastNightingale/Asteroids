#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include <vector>
const double M_PI = 3.14159265358979323846;
using namespace std;


struct Projectile 
{
    sf::RectangleShape projectile{};
    float dx;
    float dy;
    //sf::Time dtime;
    bool moving = false;
};

struct Meteor
{
    sf::RectangleShape asteroid{};
    float x;
    float y;
    float dx, dy;
    //sf::Time dtime;
};


int main()
{
    srand(time(NULL));
    //переменные    
    list <Projectile> projectileList;
    list <Meteor> meteorList;
    sf::Clock clock;
    sf::Time elapsedtime;
    bool moving = true;
    float timeAsSeconds;
    float x;
    float y;
    bool moving_after_stop = false;
    int k = 0;
    int meteoramount = 0;
    int integernumber = 1;

    //окно

    sf::Uint32 style = sf::Style::Default;
    sf::RenderWindow window(sf::VideoMode(1400,900), "Asteroids",style);
    window.setFramerateLimit(60); 

    //фигуры

    sf::ConvexShape notriangle;
    notriangle.setPointCount(4);
    notriangle.setPoint(0, sf::Vector2f(720, 470));
    notriangle.setPoint(1, sf::Vector2f(700, 450));
    notriangle.setPoint(2, sf::Vector2f(680, 470));    
    notriangle.setPoint(3, sf::Vector2f(700, 430));
    notriangle.setFillColor(sf::Color::Black);
    notriangle.setOutlineColor(sf::Color::White);
    notriangle.setOutlineThickness(2);
    notriangle.setOrigin(sf::Vector2f(700, 450));
    notriangle.setPosition(sf::Vector2f(700, 450));
    sf::Clock timer;
    sf::Time deltatime;  
    sf::Clock while_is_open;
    
    sf::Time elapsedInerDeltatime;
    //цикл

    while (window.isOpen())
    {
        //местные переменные
        /*sf::Time deltatime = clock.restart();
        elapsedtime += deltatime;
        timeAsSeconds = elapsedtime.asSeconds();*/

        sf::Event event;
        deltatime = timer.restart();
        float dt = deltatime.asSeconds();
        float inertdt = 0;
        sf::Clock interttimer;
        float program_time = 0;

        //действия пользователя

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::EventType::Closed) window.close(); //закрыть окно на крестик
            if (event.type == sf::Event::EventType::KeyReleased) //создать снаряд
            {
                if (event.key.code == sf::Keyboard::Key::Space)
                {
                    Projectile bullett;
                    bullett.projectile.setSize(sf::Vector2f(5, 5));
                    bullett.projectile.setPosition(notriangle.getPosition());
                    bullett.projectile.setFillColor(sf::Color::White);
                    bullett.moving = true;
                    bullett.dx = cos((notriangle.getRotation() - 90.0) * M_PI / 180);
                    bullett.dy = sin((notriangle.getRotation() - 90.0) * M_PI / 180);
                    projectileList.push_back(bullett);
                }
                if (event.key.code == sf::Keyboard::Key::W)
                {
                    moving_after_stop = true;
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) notriangle.rotate(-5.f); //повернуть игрока налево
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) notriangle.rotate(5.f); //повернуть игрока направо
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            k = 1;
            moving_after_stop = false;
            interttimer.restart();
            x = cos((notriangle.getRotation() - 90.0) * M_PI / 180);
            y = sin((notriangle.getRotation() - 90.0) * M_PI / 180);
            notriangle.move(dt * x * 200, y * dt * 200);
        }

        //действия внутри программы

        program_time = while_is_open.getElapsedTime().asSeconds(); //сколько идёт программа

        if (integernumber < program_time)
        {
            ++integernumber;
            int n;
            if (integernumber > 1 && integernumber < 5) n = 1;
            if (integernumber >= 5 && integernumber < 10) n = 2;
            if (integernumber > 10) n = 3;
            for (int i = 0; i < n; i++)
            {
                Meteor astero;
                int a = rand() % 4 + 1;
                int b = rand() % 3 + 1;
                //cout << a << " " << b << endl;
                int angle = 0;
                switch (a)
                {
                case 1:
                    astero.x = rand() % 1400 + 1;
                    astero.y = 0;
                    if (astero.x >= 700)
                    {
                        angle = rand() % 91 + 90;
                    }
                    else if (astero.x < 700)
                    {
                        angle = rand() % 91;
                    }
                    break;
                case 2:
                    astero.x = 0;
                    astero.y = rand() % 900 + 1;
                    if (astero.y >= 450)
                    {
                        angle = rand() % 91 + 270;
                    }
                    else
                    {
                        angle = rand() % 91;
                    }
                    break;
                case 3:
                    astero.x = rand() % 1400;
                    astero.y = 900;
                    if (astero.x >= 700)
                    {
                        angle = rand() % 91 + 180;
                    }
                    else
                    {
                        angle = rand() % 91 + 270;
                    }
                    break;
                case 4:
                    astero.x = 1400;
                    astero.y = rand() % 900 + 1;
                    if (astero.y >= 450)
                    {
                        angle = rand() % 91 + 90;
                    }
                    else
                    {
                        angle = rand() % 91 + 180;
                    }
                    break;
                }
                switch (b)
                {
                case 1:
                    astero.asteroid.setSize(sf::Vector2f(75, 75));
                    break;
                case 2:
                    astero.asteroid.setSize(sf::Vector2f(100, 100));
                    break;
                case 3:
                    astero.asteroid.setSize(sf::Vector2f(125, 125));
                    break;
                }
                astero.dy = sin(angle * M_PI / 180.0);
                astero.dx = cos(angle * M_PI / 180.0);
                astero.asteroid.setFillColor(sf::Color::Red);
                astero.asteroid.setPosition(astero.x, astero.y);
                meteorList.push_back(astero);
            }
        }

        if (moving_after_stop == true)
        {
            notriangle.move((35 - k / 4) * x * dt, (35 - k / 4) * y * dt);
            if (k < 50) k++;
        }

        for (auto& iter : projectileList) //движение снаряда
        {
            iter.projectile.move(iter.dx * 10, iter.dy * 10);
        }

        vector<list<Projectile>::iterator> projectileDelete;
        vector<list<Meteor>::iterator> meteorDelete;

        for (auto it = projectileList.begin(); it != projectileList.end(); ++it)//добавить в вектор
        {
            for (auto iter = meteorList.begin(); iter != meteorList.end(); ++iter)//добавить в вектор
            {
                if ((*it).projectile.getGlobalBounds().intersects((*iter).asteroid.getGlobalBounds()))
                {
                    projectileDelete.push_back(it);
                    meteorDelete.push_back(iter);
                    moving = false;
                }
            }
        }

            for (auto& iter : meteorList) //движение снаряда
            {
                iter.asteroid.move(iter.dx * 5.0, iter.dy * 5.0);
            }



            for (auto it : projectileDelete)
                projectileList.erase(it);

            for (auto iter : meteorDelete)
                meteorList.erase(iter);

            //прорисовка

            window.clear(sf::Color::Black); //фон   
            for (auto iter : projectileList) //снаряды
            {
                window.draw(iter.projectile);
            }

            for (auto iter : meteorList) //снаряды
            {
                window.draw(iter.asteroid);
            }

            window.draw(notriangle); //игрок
            window.display();//отображение        
        }
        return 0;
    }