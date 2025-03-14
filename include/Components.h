#pragma once
#include "raylib.h"
class GameObject;

class Components
{
public:
	virtual ~Components() = default;
	virtual bool ShouldDraw() const { return shouldDraw; };
private:
	GameObject* owner;
	bool shouldDraw = true;

	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(float deltaTime) = 0;

	friend class GameObject;

};

