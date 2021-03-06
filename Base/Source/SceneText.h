#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph\SceneNode.h"
#include "SceneGraph\SceneGraph.h"
#include "Enemy\Enemy.h"
#include "../Lua/LuaInterface.h"
#include <vector>

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	//SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[3];
	Light* lights[2];

	GenericEntity* theCube;
	CEnemy* theEnemy;
	CEnemy* new_Enemy;

	std::vector<CEnemy*> theEnemyList;

	//static SceneText* sInstance; // The pointer to the object that gets registered
	float enemySpawnTimer = 0.f;
	float enemySpawnCooldown = 5.f;
	bool newEnemySpawn = true;
};

#endif