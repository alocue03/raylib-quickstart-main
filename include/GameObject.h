#pragma once
#include "raylib.h"
#include "Components.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>

typedef std::shared_ptr<Components> ptrComponents;

class GameObject
{
public:
	bool enabled;
	int id;

	GameObject();
	~GameObject();
	//void Init();
	//void Init(Vector2 pos, Vector2 vel);
	//void Init(Vector2 pos, Vector2 vel, Color c);

	void Start();
	void Update(float deltaTime);
	void Draw(float deltaTime);

	void AddComponent(ptrComponents newComp);
	void RemoveComponent();

	Vector2 position;
	Vector2 velocity;

	//Color color;
	//void SetVelocity(Vector2 vel) {velocity = vel;}
	//static GameObject* Spawn(Vector2 pos, Vector2 vel, std::string _name);

private:
	std::vector<ptrComponents> components;

	//unsigned int id;
	//std::string name;
	//static unsigned int nextId;

	friend class Components;


};