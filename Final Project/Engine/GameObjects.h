#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#define DEG_TO_RAD (0.0174532925f)

class GameObject;

class GameEngine
{
public:
	GameEngine()
		: m_isGameOver(false)
		, m_playerLives(4)
		, m_playertwoLives(4)
		, m_playerScore(0)
		, m_level(0)
	{
		m_font.loadFromFile("Fonts/kenpixel.ttf");
	};

	void AddObject(GameObject* obj);
	void Draw(sf::RenderWindow* window);
	void Update(float dt);
	void SetIsGameOver(bool val) { m_isGameOver = val; }
	void AddScore(int val) { m_playerScore += (m_level * val);  }
	void AddScore2(int val) { m_playerScore2 += (m_level * val); }
	void RespawnPlayer();
	void RespawnPlayer2();
	void RespawnAsteroids();
private:
	std::vector<GameObject*> m_gameObjects;
	bool m_isGameOver;
	int m_playerLives;
	int m_playertwoLives;
	int m_playerScore;
	int m_playerScore2;
	int m_level;
	sf::Font m_font;
};

class GameObject
{
public:
	GameObject(std::string texturePath, const sf::Vector2f& pos);
	virtual void Draw(sf::RenderWindow* window);;
	virtual void Update(float dt);
	virtual void LimitVelocity(float dt);
	virtual void ApplyDrag(float dt);

	void SetAngle(float angle);
	float GetAngle();
	bool IsCollidingWith(GameObject* other);
	virtual void CollidedWith(GameObject* other) {};

	void SetEngine(GameEngine* engine);
	void SetPos(const sf::Vector2f& pos);
	void SetAccel(float amount);
	void SetVelocity(float amount);
	void SetVelocity(sf::Vector2f vel);
	
	void SetCollisionRadius(float radius) 
	{ 
		m_collisionRadius = radius; 
	}

	sf::Vector2f GetPosition();

	virtual void Destroy();
	bool IsDestroyed();
protected:

	//player 1
	sf::Vector2f m_pos01;
	sf::Vector2f m_vel01;
	sf::Vector2f m_accel01;

	sf::Sprite m_sprite;
	sf::Texture m_texture;
	float m_collisionRadius;
	float m_angle01;
	bool m_destroyed;
	GameEngine* m_engine;
};

class Bullet : public GameObject
{
public:
	Bullet(const sf::Vector2f& pos);
	void ApplyDrag(float dt);
	virtual void Update(float dt);
	virtual void CollidedWith(GameObject* other);
	float m_timeAlive;
	
};

class Bullet2 : public GameObject
{
public:
	Bullet2(const sf::Vector2f& pos);
	void ApplyDrag(float dt);
	virtual void Update(float dt);
	virtual void CollidedWith(GameObject* other);
	float m_timeAlive;

};

class SuperBullet : public Bullet
{
public:
	SuperBullet(const sf::Vector2f& pos);
	virtual void CollidedWith(GameObject* other);
};

class Player : public GameObject
{
public:
	Player(std::string texturePath, const sf::Vector2f& pos);
	virtual void CollidedWith(GameObject* other);
	virtual void Update(float dt);	
	virtual void Draw(sf::RenderWindow* window);

private:	
	float weaponCooldown;
	float invulnerableTimeLeft;
};


class Player2 : public GameObject
{
public:
	Player2(std::string texturePath, const sf::Vector2f& pos);
	virtual void CollidedWith(GameObject* other);
	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow* window);

private:
	float weaponCooldown;
	float invulnerableTimeLeft;
};

class Asteroid : public GameObject
{
public:
	void ApplyDrag(float dt) {};
	Asteroid(std::string texturePath, const sf::Vector2f& pos);	
	virtual void Update(float dt);

protected:
	float m_rotationRate;
};

class LargeAsteroid : public Asteroid
{
public:
	LargeAsteroid(const sf::Vector2f& pos);	
	virtual void Destroy();
};

class MediumAsteroid : public Asteroid
{
public:
	MediumAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};

class SmallAsteroid : public Asteroid
{
public:
	SmallAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};

