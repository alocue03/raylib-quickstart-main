#include "GameObject.h"

unsigned int GameObject::nextId = 1;

GameObject::GameObject()
{
	name = "defaultObject";
	id = 0;
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
	std::cout << "Hola soy un objeto" << std::endl;
	position = { (float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
	velocity = { 0,0 };
	color = RED;
	//Init(position, velocity);
}

void GameObject::Init(Vector2 pos, Vector2 vel)
{
	position = pos;
	velocity = vel;
	color = RED;

}

void GameObject::Init(Vector2 pos, Vector2 vel, Color c)
{
	
	position = pos;
	velocity = vel;
	color = c;
}

void GameObject::Update()
{
	position.x += velocity.x * GetFrameTime();
	position.y += velocity.y * GetFrameTime();
	if (position.x > GetScreenWidth() || position.x < 0) {
		velocity.x *= -1;
	}
	if (position.y > GetScreenHeight() || position.y < 0) {
		velocity.y *= -1;
	}

}

void GameObject::Draw()
{
	DrawCircle(position.x, position.y, 10, color);
}

GameObject* GameObject::Spawn(Vector2 pos, Vector2 vel, std::string _name)
{

	GameObject* obj = new GameObject();
	obj->Init(pos, vel);
	obj->name = _name;
	obj->color = DARKPURPLE;
	obj->id = nextId;
	nextId++;
	return obj;
	std::cout << "objeto" << obj->id << ":" << obj->name << std::endl;
}
