#pragma once
#include "Components.h"

class CirCleComponent : public Components
{
public:
	~CirCleComponent() override = default;

	void Start() override;
	void Update(float deltaTime) override;
	void Draw(float deltaTime) override;
};

