#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>

class Ship
{
private:
    sf::Vector2f position;
    float ShipSpeed = 10.f;
    float xVelocity = 2.f;
    float yVelocity = 2.f;
public:
    int life;
    sf::Texture shipTex;
    sf::Sprite ship;
    int points = 0;
    Ship(float startX, float startY, std::string texture, int l);
    void Update(sf::Time elapsed, sf::Sprite& ship, sf::RenderWindow& window);
    bool bullet_shot = false;
};
Ship::Ship(float startX, float startY, std::string texture, int l)
{
    position.x = startX;
    position.y = startY;
    life = l;
    ship.setPosition(position);
    std::cout << texture << std::endl;
    if (!shipTex.loadFromFile(texture))
        std::cout << "Brak tekstury" << std::endl;

    std::cout << texture << std::endl;
    ship.setTexture(shipTex);
    sf::Vector2f scale;
    scale.x = 0.05;
    scale.y = 0.05;
    ship.setScale(scale);
}

void Ship::Update(sf::Time elapsed, sf::Sprite& ship, sf::RenderWindow& window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && ship.getPosition().x > 0)
    {
        ship.move(-0.2f, 0.f);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && ship.getPosition().x < window.getSize().x)
    {
        ship.move(0.2f, 0.f);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && ship.getPosition().y > 0)
    {
        ship.move(0.f, -0.2f);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && ship.getPosition().y < window.getSize().y)
    {
        ship.move(0.f, 0.2f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        bullet_shot = true;
    }

};

class Enemy
{
protected:
    float xVelocity = .2f;
    float yVelocity = .4f;
public:
    Enemy(std::string texture);
    Enemy();
    sf::Texture enemyTex;
    sf::Vector2f position;
    virtual void Update(sf::Time elapsed, sf::RenderWindow& window, sf::Sprite& enemy, Ship& ship, bool randomize_x);
    sf::Sprite enemy;
    bool is_present = true;
};
Enemy::Enemy(){}
Enemy::Enemy(std::string texture)
{
    std::cout << texture << std::endl;
    if (!enemyTex.loadFromFile(texture))
        std::cout << "Brak tekstury" << std::endl;

    enemy.setTexture(enemyTex);
    sf::Vector2f scale;
    scale.x = 0.05;
    scale.y = 0.05;
    enemy.setScale(scale);
}
void Enemy::Update(sf::Time elapsed, sf::RenderWindow& window, sf::Sprite& enemy, Ship& ship, bool randomize_x = false)
{
    if (enemy.getPosition().y > window.getSize().y)
    {
        is_present = false;
        ship.points -= 50;
    }
    float speedx = 0;
    int ran = rand() % 50 - 25;
    if (randomize_x)
        speedx = ran * elapsed.asSeconds() * xVelocity * 100;
    float speedy = elapsed.asSeconds() * yVelocity * 100;
    enemy.move(speedx, speedy);
};

class Bullet
{
public:
    float xVelocity = 3.2f;
    virtual void Update(sf::Time elapsed, sf::RenderWindow& window, int mod);
    Bullet(std::string texture);
    sf::Texture bulletTex;
    sf::Sprite bullet;
    bool is_present = false;
private:
};

Bullet::Bullet(std::string texture)
{
    std::cout << texture << std::endl;
    if (!bulletTex.loadFromFile(texture))
        std::cout << "Brak tekstury" << std::endl;
    bullet.setTexture(bulletTex);
    sf::Vector2f scale;
    scale.x = 0.02;
    scale.y = 0.02;
    bullet.setScale(scale);
    bool is_present = true;
}
void Bullet::Update(sf::Time elapsed, sf::RenderWindow& window, int mod=-1)
{
    float speedy = mod * elapsed.asSeconds() * xVelocity * 100;
    bullet.move(0, speedy);
}

class Boss : public Enemy
{
public:
    Boss(std::string texture, int l);
    void Attack();
    int life;
    bool bullet_shot = false;
};

Boss::Boss(std::string texture, int l)
{
    std::cout << texture << std::endl;
    if (!enemyTex.loadFromFile(texture))
        std::cout << "Brak tekstury" << std::endl;

    enemy.setTexture(enemyTex);
    sf::Vector2f scale;
    scale.x = 0.2;
    scale.y = 0.2;
    enemy.setScale(scale);
    life = l;
    yVelocity = .2f;
}

void Boss::Attack()
{
    bullet_shot = true;
}

void CollisionBullet(Bullet& bullet, Enemy& enemy, Ship& ship)
{
    if (enemy.enemy.getGlobalBounds().intersects(bullet.bullet.getGlobalBounds()))
    {
        enemy.is_present = false;
        bullet.is_present = false;
        ship.points += 100;
    }
}
void CollisionShip(Ship& ship, Enemy& enemy)
{
    if (enemy.enemy.getGlobalBounds().intersects(ship.ship.getGlobalBounds()))
    {
        enemy.is_present = false;
        ship.life--;
    }
}
void CollisionShipBullet(Ship& ship, Bullet& bullet)
{
    if (bullet.bullet.getGlobalBounds().intersects(ship.ship.getGlobalBounds()))
    {
        bullet.is_present = false;
        ship.life--;
    }
}
void CollisionBossBullet(Boss& boss, Bullet& bullet)
{
    if (bullet.bullet.getGlobalBounds().intersects(boss.enemy.getGlobalBounds()))
    {
        bullet.is_present = false;
        boss.life--;
    }
}
int main()
{
    srand(time(NULL));
    int Window_width = 600;
    int Window_height = 600;
    sf::RenderWindow window(sf::VideoMode(Window_width, Window_height), "SFML works!");
    //system("dir");
    Ship ship(window.getSize().x / 2, window.getSize().y / 2, "ship.png", 4);
    sf::Clock clock;

    Enemy enemy("enemy.png");
    int enemy_count = 8;
    std::vector<Enemy> enemy_vector;
    for (int i = 0; i < enemy_count; i++)
    {
        sf::Vector2f v1(rand() % window.getSize().x, 20);
        enemy.enemy.setPosition(v1);
        enemy_vector.push_back(enemy);
    }

    std::vector<Enemy> enemy_vector_second;
    for (int i = 0; i < 16; i++)
    {
        sf::Vector2f v1(rand() % window.getSize().x, 20);
        enemy.enemy.setPosition(v1);
        enemy_vector_second.push_back(enemy);
    }

    Bullet bullet("bullet.png");
    Bullet boss_bullet("bullet.png");
    Boss boss("enemy.png", 10);
    sf::Vector2f v2(window.getSize().x / 2, 20);
    boss.enemy.setPosition(v2);
    int counter = 0;
    while (window.isOpen())
    {
        sf::Event event;
        sf::Time elapsed = clock.restart();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Red);
        ship.Update(elapsed, ship.ship, window);
        if (ship.bullet_shot)
        {
            ship.bullet_shot = false;
            bullet.bullet.setPosition(ship.ship.getPosition().x, ship.ship.getPosition().y);
            bullet.is_present = true;
        }
        bullet.Update(elapsed, window);
        if (bullet.is_present)
            window.draw(bullet.bullet);
        window.draw(ship.ship);
        window.draw(bullet.bullet);
        for (size_t i = 0; i < enemy_vector.size(); i++)
        {
            enemy_vector[i].Update(elapsed, window, enemy_vector[i].enemy, ship);
            CollisionBullet(bullet, enemy_vector[i], ship);
            CollisionShip(ship, enemy_vector[i]);
            if (enemy_vector[i].is_present)
                window.draw(enemy_vector[i].enemy);
            else
                enemy_vector.erase(enemy_vector.begin() + i);
        }
        if (enemy_vector.size() == 0)
        {
            for (size_t i = 0; i < enemy_vector_second.size(); i++)
            {
                enemy_vector_second[i].Update(elapsed, window, enemy_vector_second[i].enemy, ship);
                CollisionBullet(bullet, enemy_vector_second[i], ship);
                CollisionShip(ship, enemy_vector_second[i]);
                if (enemy_vector_second[i].is_present)
                    window.draw(enemy_vector_second[i].enemy);
                else
                    enemy_vector_second.erase(enemy_vector_second.begin() + i);
            }
        }
        if (enemy_vector_second.size() == 0)
        {
            boss.Update(elapsed, window, boss.enemy, ship, true);
            if (counter % 1000 == 0)
                boss.Attack();
            if (boss.bullet_shot)
            {
                boss.bullet_shot = false;
                boss_bullet.bullet.setPosition(boss.enemy.getPosition().x, boss.enemy.getPosition().y);
                boss_bullet.is_present = true;
            }
            boss_bullet.Update(elapsed, window, 1);
            if (boss_bullet.is_present)
                CollisionShipBullet(ship, boss_bullet);
            if (bullet.is_present)
                CollisionBossBullet(boss, bullet);
            if (boss_bullet.is_present)
                window.draw(boss_bullet.bullet);
            if (boss.life > 0)
                window.draw(boss.enemy);
            else
            {
                std::cout << "Finished" << std::endl;
                std::cout << "Points: " << ship.points << std::endl;
                break;
            }
        }
        if (ship.life < 0)
        {
            std::cout << "Game over" << std::endl;
            std::cout << "Points: " << ship.points << std::endl;
            break;
        }
        window.display();
        counter++;
    }
    return 0;
}