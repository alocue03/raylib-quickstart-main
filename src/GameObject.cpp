#include "GameObject.h"


GameObject::GameObject()
{
	id = 0;
	enabled = true;
}

GameObject::~GameObject()
{
}

//void GameObject::Init()
//{
//	std::cout << "Hola soy un objeto" << std::endl;
//	position = { (float)GetScreenWidth()/2, (float)GetScreenHeight()/2};
//	velocity = { 0,0 };
//	color = RED;
//	//Init(position, velocity);
//}
//
//void GameObject::Init(Vector2 pos, Vector2 vel)
//{
//	position = pos;
//	velocity = vel;
//	color = RED;
//
//}
//
//void GameObject::Init(Vector2 pos, Vector2 vel, Color c)
//{
//	
//	position = pos;
//	velocity = vel;
//	color = c;
//}

void GameObject::Update(float deltaTime)
{
	for (auto& comp : components)
	{
		comp->Update(deltaTime);
	}
}

void GameObject::Draw(float deltaTime)
{
	for (auto& comp : components)
	{
		if (comp->ShouldDraw())
		{
			comp->Draw(deltaTime);
		}
	}
}

void GameObject::AddComponent(ptrComponents newComp)
{
	components.push_back(newComp);
}

void GameObject::RemoveComponent()
{
}

//GameObject* GameObject::Spawn(Vector2 pos, Vector2 vel, std::string _name)
//{
//
//	GameObject* obj = new GameObject();
//	obj->Init(pos, vel);
//	obj->name = _name;
//	obj->color = DARKPURPLE;
//	obj->id = nextId;
//	nextId++;
//	return obj;
//	std::cout << "objeto" << obj->id << ":" << obj->name << std::endl;
//}
