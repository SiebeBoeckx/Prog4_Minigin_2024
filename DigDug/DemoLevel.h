#pragma once
#include "Scene.h"

class DemoLevel final
{
public:
	DemoLevel(dae::Scene* pScene);
	~DemoLevel() = default;

	DemoLevel(const DemoLevel& other) = delete;
	DemoLevel(DemoLevel&& other) = delete;
	DemoLevel& operator=(const DemoLevel& other) = delete;
	DemoLevel& operator=(DemoLevel&& other) = delete;
};

