#include "stdafx.h"
#include "GameObjects.h"
#include <iostream>

std::vector<GameObject*> gameObjects;

GameObject::GameObject(std::string texturePath, const sf::Vector2f & pos)
	: m_pos01(pos)
	, m_destroyed(false)
{
	// Load the texture
	// Assign the sprite		
	m_texture.loadFromFile(texturePath);
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_sprite.getTextureRect().width * 0.5f, m_sprite.getTextureRect().height * 0.5f);
}

void GameObject::Destroy()
{
	m_destroyed = true;
}

bool GameObject::IsDestroyed()
{
	return m_destroyed;
}

void GameObject::Draw(sf::RenderWindow * window)
{
	sf::CircleShape collisionShape;
	collisionShape.setPosition(m_pos01);
	collisionShape.setRadius(m_collisionRadius);
	collisionShape.setOutlineColor(sf::Color::Magenta);
	collisionShape.setOutlineThickness(3);
	collisionShape.setFillColor(sf::Color::Transparent);
	collisionShape.setOrigin(m_collisionRadius, m_collisionRadius);
	//window->draw(collisionShape);
	window->draw(m_sprite);
}

void GameObject::Update(float dt)
{
	float decelScalar = 0.5f;
	m_vel01 += m_accel01 * dt;

	
	LimitVelocity(dt);
	ApplyDrag(dt);

	m_pos01 += m_vel01 * dt;

	if (m_pos01.x < 0)
	{
		m_pos01.x = 1280;
	}
	else if (m_pos01.x > 1280)
	{
		m_pos01.x = 0;
	}

	if (m_pos01.y < 0)
	{
		m_pos01.y = 720;
	}
	else if (m_pos01.y > 720)
	{
		m_pos01.y = 0;
	}


	m_sprite.setRotation(m_angle01);
	m_sprite.setPosition(m_pos01);
}

void GameObject::LimitVelocity(float dt)
{
	float speed = sqrt(m_vel01.x * m_vel01.x + m_vel01.y  *m_vel01.y);	

	if (speed <= 0.1)
	{
		return;
	}

	sf::Vector2f normalizedVel = sf::Vector2f(m_vel01.x / speed, m_vel01.y / speed);

	if (speed > 500)
	{
		speed = 500;
	}

	m_vel01.x = normalizedVel.x * speed;
	m_vel01.y = normalizedVel.y * speed;
}

void GameObject::SetAngle(float angle)
{
	m_angle01 = angle;
}

float GameObject::GetAngle()
{
	return m_angle01;
}

bool GameObject::IsCollidingWith(GameObject * other)
{
	sf::Vector2f vectorToOther = other->m_pos01 - m_pos01;
	float dist = sqrt(vectorToOther.x * vectorToOther.x + vectorToOther.y * vectorToOther.y);

	if (dist < (m_collisionRadius + other->m_collisionRadius))
	{
		return true;
	}
	return false;
}

void GameObject::SetEngine(GameEngine* engine)
{
	m_engine = engine;
}

void GameObject::SetPos(const sf::Vector2f & pos)
{
	m_pos01 = pos;
}

void GameObject::SetAccel(float amount)
{	
	
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle01;
		m_accel01 = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_accel01 = sf::Vector2f(0, 0);
	}
	
	
}



void GameObject::SetVelocity(float amount)
{
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle01;
		m_vel01 = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_vel01 = sf::Vector2f(0, 0);
	}
}

void GameObject::SetVelocity(sf::Vector2f vel)
{
	m_vel01 = vel;
}

sf::Vector2f GameObject::GetPosition()
{
	return m_pos01;
}

void GameObject::ApplyDrag(float dt)
{
	if (m_accel01.x == 0 && m_accel01.y == 0)
	{
		float dragAmount = dt * 0.9f;
		m_vel01.x -= dragAmount* m_vel01.x;
		m_vel01.y -= dragAmount * m_vel01.y;
	}
}

Bullet::Bullet(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserBlue08.png", pos)
	, m_timeAlive(0.0f)
{
	SetCollisionRadius(10.0f);
	m_sprite.setScale(0.5, 0.5);
}

Bullet2::Bullet2(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserBlue08.png", pos)
	, m_timeAlive(0.0f)
{
	SetCollisionRadius(10.0f);
	m_sprite.setScale(0.5, 0.5);
}

void Bullet::ApplyDrag(float dt)
{
	// do nothing
}

void Bullet2::ApplyDrag(float dt)
{
	// do nothing
}

void Bullet::Update(float dt)
{
	GameObject::Update(dt);
	m_angle01 += dt * 360;
	m_timeAlive += dt;

	if (m_timeAlive > 3.0f)
	{
		Destroy();
	}
}

void Bullet2::Update(float dt)
{
	GameObject::Update(dt);
	m_angle01 += dt * 360;
	m_timeAlive += dt;

	if (m_timeAlive > 3.0f)
	{
		Destroy();
	}
}

void Bullet::CollidedWith(GameObject * other)
{	
	Player* thePlayer = dynamic_cast<Player*>(other);
	Player2* thePlayer2 = dynamic_cast<Player2*>(other);
	if (thePlayer == NULL && thePlayer2 == NULL)
	{
		m_engine->AddScore(100);
		Destroy();
		other->Destroy();
	}

}

void Bullet2::CollidedWith(GameObject * other)
{

	Player2* thePlayer2 = dynamic_cast<Player2*>(other);
	Player* thePlayer = dynamic_cast<Player*>(other);
	if (thePlayer2 == NULL && thePlayer == NULL)
	{
		m_engine->AddScore2(100);
		Destroy();
		other->Destroy();

	}
}

Player::Player(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos)	
	, weaponCooldown(0)
	, invulnerableTimeLeft(2.0f)
{
	SetCollisionRadius(35.0f);
}

Player2::Player2(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos)
	, weaponCooldown(0)
	, invulnerableTimeLeft(2.0f)
{
	SetCollisionRadius(35.0f);
}

void Player::CollidedWith(GameObject* other)
{
	if (invulnerableTimeLeft <= 0.0f)
	{
		Asteroid* anAsteroid = dynamic_cast<Asteroid*>(other);
		if (anAsteroid != NULL)
		{
			m_engine->RespawnPlayer();
			Destroy();
		}
	}
}


void Player2::CollidedWith(GameObject* other)
{
	if (invulnerableTimeLeft <= 0.0f)
	{
		Asteroid* anAsteroid = dynamic_cast<Asteroid*>(other);
		if (anAsteroid != NULL)
		{
			m_engine->RespawnPlayer2();
			Destroy();
		}
	}
}

void Player::Update(float dt)
{
	GameObject::Update(dt);	
	invulnerableTimeLeft -= dt;
	weaponCooldown -= dt;	
	SetAccel(0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		SetAngle(GetAngle() + 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		SetAngle(GetAngle() - 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		SetAccel(400.0f);
	}
	
	if (weaponCooldown <= 0.0f && invulnerableTimeLeft < 0.0f)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{			
			weaponCooldown = .2f;
			Bullet* newBullet = new Bullet(m_pos01);
			sf::Vector2f newVelocity;
			newVelocity.x = sin(m_angle01 * 2 * 3.1415 / 360.0f) * 300.0f;
			newVelocity.y = -cos(m_angle01 * 2 * 3.1415 / 360.0f) * 300.0f;
			newBullet->SetVelocity(newVelocity);
			m_engine->AddObject(newBullet);
		}	
	}

	
}

void Player2::Update(float dt)
{
	GameObject::Update(dt);
	invulnerableTimeLeft -= dt;
	weaponCooldown -= dt;
	SetAccel(0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		SetAngle(GetAngle() + 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		SetAngle(GetAngle() - 180 * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		SetAccel(400.0f);
	}

	if (weaponCooldown <= 0.0f && invulnerableTimeLeft < 0.0f)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			weaponCooldown = .2f;
			Bullet2* newBullet = new Bullet2(m_pos01);
			sf::Vector2f newVelocity;
			newVelocity.x = sin(m_angle01 * 2 * 3.1415 / 360.0f) * 300.0f;
			newVelocity.y = -cos(m_angle01 * 2 * 3.1415 / 360.0f) * 300.0f;
			newBullet->SetVelocity(newVelocity);
			m_engine->AddObject(newBullet);
		}
	}


}



void Player::Draw(sf::RenderWindow* window)
{
	GameObject::Draw(window);

	if (invulnerableTimeLeft > 0)
	{
		sf::CircleShape invulnCircle;
		invulnCircle.setRadius(60);
		invulnCircle.setFillColor(sf::Color::Transparent);
		invulnCircle.setOutlineThickness(5 * invulnerableTimeLeft);
		invulnCircle.setOutlineColor(sf::Color::Blue);
		invulnCircle.setOrigin(60, 60);
		invulnCircle.setPosition(m_pos01);
		window->draw(invulnCircle);

		float shipOpacity = 0.5f * (sin(invulnerableTimeLeft * 20) + 1);		
		m_sprite.setColor(sf::Color(255, 255, 255, shipOpacity * 255));
	}
	else
	{
		m_sprite.setColor(sf::Color(255, 255, 255, 255));
	}
}

void Player2::Draw(sf::RenderWindow* window)
{
	GameObject::Draw(window);

	if (invulnerableTimeLeft > 0)
	{
		sf::CircleShape invulnCircle;
		invulnCircle.setRadius(60);
		invulnCircle.setFillColor(sf::Color::Transparent);
		invulnCircle.setOutlineThickness(5 * invulnerableTimeLeft);
		invulnCircle.setOutlineColor(sf::Color::Blue);
		invulnCircle.setOrigin(60, 60);
		invulnCircle.setPosition(m_pos01);
		window->draw(invulnCircle);

		float shipOpacity = 0.5f * (sin(invulnerableTimeLeft * 20) + 1);
		m_sprite.setColor(sf::Color(255, 255, 255, shipOpacity * 255));
	}
	else
	{
		m_sprite.setColor(sf::Color(255, 255, 255, 255));
	}
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void GameEngine::AddObject(GameObject* obj)
{
	obj->SetEngine(this);
	m_gameObjects.push_back(obj);
}

void GameEngine::Draw(sf::RenderWindow* window)
{


	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		current->Draw(window);
	}

	if (m_isGameOver)
	{			
		sf::Text myText;
		myText.setFont(m_font);
		myText.setPosition(400, 300);
		myText.setCharacterSize(80);
		myText.setString("YOU LOSE");
		window->draw(myText);		
	}

	std::string scoreString;
	scoreString += "Score: " + std::to_string(m_playerScore);
	sf::Text scoreText;
	scoreText.setFont(m_font);
	scoreText.setPosition(50, 100);
	scoreText.setCharacterSize(40);
	scoreText.setString(scoreString.c_str());
	window->draw(scoreText);

	std::string scoreString2;
	scoreString2 += "Score: " + std::to_string(m_playerScore2);
	sf::Text scoreText2;
	scoreText2.setFont(m_font);
	scoreText2.setPosition(850, 100);
	scoreText2.setCharacterSize(40);
	scoreText2.setString(scoreString2.c_str());
	window->draw(scoreText2);

	for (int i = 0; i < m_playerLives; i++)
	{
		sf::Texture shipTexture;
		shipTexture.loadFromFile("Sprites/PNG/playerShip2_red.png");
		sf::Sprite shipSprite;
		shipSprite.setTexture(shipTexture);
		shipSprite.setScale(0.5, 0.5);
		shipSprite.setColor(sf::Color(255, 255, 255, 128));
		shipSprite.setPosition(i * 50 + 50, 50);
		window->draw(shipSprite);
	}

	for (int i = 0; i < m_playertwoLives; i++)
	{
		sf::Texture shipTexture2;
		shipTexture2.loadFromFile("Sprites/PNG/playerShip3_blue.png");
		sf::Sprite shipSprite2;
		shipSprite2.setTexture(shipTexture2);
		shipSprite2.setScale(0.5, 0.5);
		shipSprite2.setColor(sf::Color(255, 255, 255, 128));
		shipSprite2.setPosition(i * 50 + 850, 50);
		window->draw(shipSprite2);
	}
}

void GameEngine::Update(float dt)
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		current->Update(dt);
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		for (int j = 0; j < m_gameObjects.size(); j++)
		{
			GameObject* other = m_gameObjects[j];
			if (other != current
				&& !other->IsDestroyed()
				&& !current->IsDestroyed()
				&& current->IsCollidingWith(other))
			{
				current->CollidedWith(other);
			}
		}
	}

	for (int i = m_gameObjects.size() - 1; i >= 0; i--)
	{
		GameObject* current = m_gameObjects[i];
		if (current->IsDestroyed())
		{
			delete current;
			m_gameObjects.erase(m_gameObjects.begin() + i);
		}
	}	

	bool allAsteroidsDestroyed = true;
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		Asteroid* asteroid = dynamic_cast<Asteroid*>(current);
		if (asteroid != NULL)
		{
			allAsteroidsDestroyed = false;
			// Found an asteroid, game is still going
			break;
		}
	}

	if (allAsteroidsDestroyed)
	{
		RespawnAsteroids();
	}
}

void GameEngine::RespawnPlayer()
{
	if (m_playerLives > 0)
	{
		m_playerLives--;
		Player* player = new Player("Sprites/PNG/playerShip2_red.png", sf::Vector2f(500, 300));
		AddObject(player);
	}
	/*if (m_playertwoLives > 0)
	{
		m_playertwoLives--;
		Player* playertwo = new Player("Sprites/PNG/playerShip3_blue.png", sf::Vector2f(600, 250));
		AddObject(playertwo);
	}*/
	else
	{
		SetIsGameOver(true);
	}
}

void GameEngine::RespawnPlayer2()
{
	if (m_playertwoLives > 0)
	{
		m_playertwoLives--;
		Player2* playertwo = new Player2("Sprites/PNG/playerShip3_blue.png", sf::Vector2f(600, 250));
		std::cout << " Spawned" << std::endl;
		AddObject(playertwo);
	}
	else
	{
		SetIsGameOver(true);
	}
}


void GameEngine::RespawnAsteroids()
{
	m_level++;
	for (int i = 0; i < m_level + 4; i++)
	{
		Asteroid* asteroid;
		asteroid = new LargeAsteroid(sf::Vector2f(100 + rand() % 1000, 100 + rand() % 500));
		sf::Vector2f randDir;
		asteroid->SetAngle(rand() % 360);
		asteroid->SetVelocity(rand() % 25 + 50);
		AddObject(asteroid);
	}
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
Asteroid::Asteroid(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos)
{
	m_rotationRate = rand() % 45 + 45;
		
	if (rand() % 2 == 0)
	{
		m_rotationRate *= -1;
	}
}

void Asteroid::Update(float dt)
{	
	GameObject::Update(dt);
	m_angle01 += m_rotationRate * dt;
}

LargeAsteroid::LargeAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/meteorBrown_big3.png", pos)
{
	SetCollisionRadius(50.0f);
}

void LargeAsteroid::Destroy()
{
	GameObject::Destroy();

	//m_engine->AddScore(100);
	
	
	for (int i = 0; i < 3; i++)
	{
		Asteroid* asteroid;
		asteroid = new MediumAsteroid(m_pos01);
		sf::Vector2f randDir;
		asteroid->SetAngle(rand() % 360);
		asteroid->SetVelocity(rand() % 25 + 50);
		m_engine->AddObject(asteroid);
	}
}

MediumAsteroid::MediumAsteroid(const sf::Vector2f& pos)
	: Asteroid("Sprites/PNG/Meteors/meteorBrown_med3.png", pos)
{
	SetCollisionRadius(30.0f);
}

void MediumAsteroid::Destroy()
{
	GameObject::Destroy();
	//m_engine->AddScore(50);
	for (int i = 0; i < 2; i++)
	{
		Asteroid* asteroid;
		asteroid = new SmallAsteroid(m_pos01);
		sf::Vector2f randDir;
		asteroid->SetAngle(rand() % 360);
		asteroid->SetVelocity(rand() % 25 + 50);
		m_engine->AddObject(asteroid);
	}
}

SmallAsteroid::SmallAsteroid(const sf::Vector2f& pos)
	: Asteroid("Sprites/PNG/Meteors/meteorBrown_small2.png", pos)
{
	SetCollisionRadius(20.0f);

}

void SmallAsteroid::Destroy()
{
	GameObject::Destroy();
	//m_engine->AddScore(20);
}

SuperBullet::SuperBullet(const sf::Vector2f & pos)
	: Bullet(pos)
{
	m_sprite.setScale(3, 3);
	m_sprite.setColor(sf::Color::Red);
	SetCollisionRadius(60);
}

void SuperBullet::CollidedWith(GameObject * other)
{
	Player* thePlayer = dynamic_cast<Player*>(other);
	if (thePlayer == NULL)
	{
		other->Destroy();
	}

}
