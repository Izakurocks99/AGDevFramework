#pragma once
#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "..\FPSCamera.h"
#include "..\SpriteEntity.h"

class SceneManager;
//class TextEntity;

class CGameoverState :public Scene
{
public:
	CGameoverState();
	~CGameoverState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* GameoverStateBackground;
//	TextEntity* textObj[3];
};