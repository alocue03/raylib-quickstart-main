#pragma once
#include "raylib.h"
#include <iostream>
#include <string>

class GameObject
{
public:
	GameObject();
	~GameObject();
	void Init();
	void Init(Vector2 pos, Vector2 vel);
	void Init(Vector2 pos, Vector2 vel, Color c);

	void Update();
	void Draw();

	bool enabled;
	Vector2 position;
	Vector2 velocity;

	Color color;

	void SetVelocity(Vector2 vel) {velocity = vel;}
	static GameObject* Spawn(Vector2 pos, Vector2 vel, std::string _name);

private:
	unsigned int id;
	std::string name;
	static unsigned int nextId;


};