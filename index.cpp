#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

std::vector<sf::RectangleShape> snake;
sf::RectangleShape apple;

int direction = 0; // 0=right,1=down,2=left,3=up
bool Pause = false;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distX(0, 79);
std::uniform_int_distribution<> distY(0, 59);

sf::Clock Walkclock;
float interval = 0.1f;

void NewSegment()
{
    sf::RectangleShape seg;
    seg.setSize({10.f, 10.f});
    seg.setFillColor(sf::Color(26, 158, 0));

    if (!snake.empty()) 
    {
        seg.setPosition(snake.back().getPosition());
    }
    else 
    {
        seg.setPosition({400.f, 300.f});
        seg.setFillColor(sf::Color(37, 219, 0));
    }

    snake.push_back(seg);
}

void CreateApple()
{
    apple.setSize({10.f, 10.f});
    apple.setFillColor(sf::Color::Red);
    apple.setPosition({(float)distX(gen) * 10.f, (float)distY(gen) * 10.f});
}

void MoveSnake()
{
    if (snake.empty()) 
    {
        return;
    }

    sf::Vector2f prevPos = snake[0].getPosition();

    switch (direction) 
    {
        case 0: prevPos.x += 10.f; break;
        case 1: prevPos.y += 10.f; break;
        case 2: prevPos.x -= 10.f; break;
        case 3: prevPos.y -= 10.f; break;
    }

    if (prevPos.x < 0 && prevPos.x >= 800 && prevPos.y < 0 || prevPos.y >= 600) 
    {
        std::exit(0);
    }

    for (size_t i = snake.size() - 1; i > 0; --i) 
    {
        snake[i].setPosition(snake[i - 1].getPosition());
    }

    snake[0].setPosition(prevPos);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Snake");

    NewSegment();
    NewSegment();
    CreateApple();

    sf::Clock clock;
    float interval = 0.1f;

    while (window.isOpen()) 
    {
        while (auto eventOpt = window.pollEvent()) if (eventOpt->is<sf::Event::Closed>()) window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) 
        {
            Pause = !Pause;
        }

        if (!Pause) 
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && direction != 1) direction = 3;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && direction != 0) direction = 2;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && direction != 3) direction = 1;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && direction != 2) direction = 0;
        }

        if (clock.getElapsedTime().asSeconds() >= interval && !Pause) 
        {
            MoveSnake();

            for (size_t i = 0; i < snake.size(); ++i)
            {
                if (snake[0].getGlobalBounds().findIntersection(apple.getGlobalBounds()))
                {
                    NewSegment();
                    CreateApple();
                    break;
                }
            }

            for (size_t i = 1; i < snake.size(); ++i) 
            {
                if (snake[0].getGlobalBounds().findIntersection(snake[i].getGlobalBounds())) 
                {
                    window.close();
                }
            }

            for (size_t i = 1; i < snake.size(); ++i)
            {
                if (snake[0].getPosition().x < 0 || snake[0].getPosition().y < 0 || snake[0].getPosition().x > window.getSize().x || snake[0].getPosition().y > window.getSize().y)
                    window.close();
                
            }
            

            clock.restart();
        }

        window.clear();

        for (auto& seg : snake) 
        {
            window.draw(seg);
        }

        window.draw(apple);
        window.display();
    }
}