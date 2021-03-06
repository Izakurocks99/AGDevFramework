#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"

#include "SceneGraph\UpdateTransformation.h"
#include "LuaInterface.h"

#include "../Waypoint/WaypointManager.h"

#include <iostream>
using namespace std;

//SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

//SceneText::SceneText(SceneManager* _sceneMgr)
//{
//	_sceneMgr->AddScene("Start", this);
//}

SceneText::~SceneText()
{
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	//currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	//
	//// Tell the shader program to store these uniform locations
	//currProg->AddUniform("MVP");
	//currProg->AddUniform("MV");
	//currProg->AddUniform("MV_inverse_transpose");
	//currProg->AddUniform("material.kAmbient");
	//currProg->AddUniform("material.kDiffuse");
	//currProg->AddUniform("material.kSpecular");
	//currProg->AddUniform("material.kShininess");
	//currProg->AddUniform("lightEnabled");
	//currProg->AddUniform("numLights");
	//currProg->AddUniform("lights[0].type");
	//currProg->AddUniform("lights[0].position_cameraspace");
	//currProg->AddUniform("lights[0].color");
	//currProg->AddUniform("lights[0].power");
	//currProg->AddUniform("lights[0].kC");
	//currProg->AddUniform("lights[0].kL");
	//currProg->AddUniform("lights[0].kQ");
	//currProg->AddUniform("lights[0].spotDirection");
	//currProg->AddUniform("lights[0].cosCutoff");
	//currProg->AddUniform("lights[0].cosInner");
	//currProg->AddUniform("lights[0].exponent");
	//currProg->AddUniform("lights[1].type");
	//currProg->AddUniform("lights[1].position_cameraspace");
	//currProg->AddUniform("lights[1].color");
	//currProg->AddUniform("lights[1].power");
	//currProg->AddUniform("lights[1].kC");
	//currProg->AddUniform("lights[1].kL");
	//currProg->AddUniform("lights[1].kQ");
	//currProg->AddUniform("lights[1].spotDirection");
	//currProg->AddUniform("lights[1].cosCutoff");
	//currProg->AddUniform("lights[1].cosInner");
	//currProg->AddUniform("lights[1].exponent");
	//currProg->AddUniform("colorTextureEnabled");
	//currProg->AddUniform("colorTexture");
	//currProg->AddUniform("textEnabled");
	//currProg->AddUniform("textColor");
	//
	//// Tell the graphics manager to use the shader we just loaded
	//GraphicsManager::GetInstance()->SetActiveShader("default");

	//currProg->UpdateInt("numLights", 1);
	//currProg->UpdateInt("textEnabled", 0);

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";
	
	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");

	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);

	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 10.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESHNEAR", Color(1, 0, 0), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESHMED", Color(0, 1, 0), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESHFAR", Color(0, 0, 1), 1.f);

	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);

	// Trees
	MeshBuilder::GetInstance()->GenerateCone("H_TreeLeaves", Color(0.f, 1, 0.f), 36, 10.f, 30.f);
	MeshBuilder::GetInstance()->GenerateCone("M_TreeLeaves", Color(0.f, 1, 0.f), 36, 5.f, 15.f);
	MeshBuilder::GetInstance()->GenerateCone("L_TreeLeaves", Color(0.f, 1, 0.f), 36, 2.5f, 7.5f);
	MeshBuilder::GetInstance()->GenerateCube("H_TreeTrunk", Color(0.5f, 0.2f, 0.1f), 10.0f);
	MeshBuilder::GetInstance()->GenerateCube("M_TreeTrunk", Color(0.5f, 0.2f, 0.1f), 5.0f);
	MeshBuilder::GetInstance()->GenerateCube("L_TreeTrunk", Color(0.5f, 0.2f, 0.1f), 2.5f);

	// Enemies
	MeshBuilder::GetInstance()->GenerateCube("H_EnemyHead", Color(1.f, 0.f, 0.f), 1.f);
	MeshBuilder::GetInstance()->GenerateCube("M_EnemyHead", Color(0.f, 1.f, 0.f), 2.f);
	MeshBuilder::GetInstance()->GenerateCube("L_EnemyHead", Color(0.f, 0.f, 1.f), 3.f);
	MeshBuilder::GetInstance()->GenerateCube("H_EnemyBody", Color(0.f, 0.f, 1.f), 1.f);
	MeshBuilder::GetInstance()->GenerateCube("M_EnemyBody", Color(0.f, 1.f, 0.f), 2.f);
	MeshBuilder::GetInstance()->GenerateCube("L_EnemyBody", Color(1.f, 0.f, 0.f), 3.f);

	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(40000.0f, 160000.0f);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESHNEAR", "GRIDMESHMED", "GRIDMESHFAR");
	//CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());
	
	
	// Texture
	MeshBuilder::GetInstance()->GenerateQuad("BRICK_TEXTURE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BRICK_TEXTURE")->textureID = LoadTGA("Image//bricktexture.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_TEXTURE", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_TEXTURE")->textureID = LoadTGA("Image//grasstexture.tga");

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f),GenericEntity::TYPE_NONE); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z),GenericEntity::TYPE_NONE); // Lightball

	/*GenericEntity* aCube = Create::Entity("cube", Vector3(-20.0f, 0.0f, -20.0f), GenericEntity::TYPE_OBJECT);
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	aCube->InitLOD("cube", "sphere", "cubeSG");*/


	// Add the pointer to this new entity to the Scene Graph
	/*CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(aCube);
	if (theNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}

	GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 1.1f, -20.0f), GenericEntity::TYPE_OBJECT);
	anotherCube->SetCollider(true);
	anotherCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	CSceneNode* anotherNode = theNode->AddChild(anotherCube);
	if (anotherNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}

	GenericEntity* baseCube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);

	CUpdateTransformation* baseMtx = new CUpdateTransformation();
	baseMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	baseMtx->SetSteps(-60, 60);
	baseNode->SetUpdateTransformation(baseMtx);

	GenericEntity* childCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* childNode = baseNode->AddChild(childCube);
	childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);

	GenericEntity* grandchildCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);
	CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
	aRotateMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	aRotateMtx->SetSteps(-120, 60);
	grandchildNode->SetUpdateTransformation(aRotateMtx);*/

	//// Tree
	//GenericEntity* treetrunk = Create::Entity("H_TreeTrunk", Vector3(0.f, 0.f, -50.f), GenericEntity::TYPE_CHARACTER, Vector3(1.f, 2.f, 1.f));
	//treetrunk->InitLOD("H_TreeTrunk", "M_TreeTrunk", "L_TreeTrunk");
	//treetrunk->SetCanRender(false);
	//treetrunk->SetCollider(true);
	//treetrunk->SetAABB(Vector3(5.f, 3.f, 5.f), Vector3(-5.f, -5.f, -5.f));
	//CSceneNode* treeNode1st = CSceneGraph::GetInstance()->AddNode(treetrunk);
	//GenericEntity* treeleaves = Create::Entity("H_TreeLeaves", Vector3(0.f, 8.f, -50.f), GenericEntity::TYPE_CHARACTER);
	//treeleaves->InitLOD("H_TreeLeaves", "M_TreeLeaves", "L_TreeLeaves");
	//treeleaves->SetCanRender(false);
	//treeleaves->SetCollider(true);
	//treeleaves->SetAABB(Vector3(5.f, 5.f, 5.f), Vector3(-5.f, -1.f, -5.f));
	//CSceneNode* treeNode2nd = treeNode1st->AddChild(treeleaves);

	//// Enemies
	//GenericEntity* enemyBody = Create::Entity("H_EnemyBody", Vector3(0.f, -5.f, 0.f), GenericEntity::TYPE_CHARACTER, Vector3(1.f, 2.f, 1.f));
	//enemyBody->InitLOD("H_EnemyBody", "M_EnemyBody", "L_EnemyBody");
	//enemyBody->SetCanRender(false);
	//enemyBody->SetCollider(true);
	//enemyBody->SetAABB(Vector3(2.5f, 2.5f, 2.5f), Vector3(-2.5f, -2.5f, -2.5f));
	//CSceneNode* enemynode1st = CSceneGraph::GetInstance()->AddNode(enemyBody);

	//CUpdateTransformation* enemybaseMovment = new CUpdateTransformation();
	//enemybaseMovment->ApplyUpdate(1.f, 0.f, 0.f);
	//enemybaseMovment->SetSteps(-30, 30);
	//enemynode1st->SetUpdateTransformation(enemybaseMovment);

	//GenericEntity* enemyHead = Create::Entity("H_EnemyHead", Vector3(0.f, 2.f, 0.f), GenericEntity::TYPE_CHARACTER);
	//enemyHead->InitLOD("H_EnemyHead", "M_EnemyHead", "L_EnemyHead");
	//enemyHead->SetCanRender(false);
	//CSceneNode* enemynode2nd = enemynode1st->AddChild(enemyHead);

	//CUpdateTransformation* enemyheadMovment = new CUpdateTransformation();
	//enemyheadMovment->ApplyUpdate(1.f, 0.f, 1.f, 0.f);
	//enemyheadMovment->SetSteps(-100, 100);
	//enemynode2nd->SetUpdateTransformation(enemyheadMovment);

	// Ground
	groundEntity = Create::Ground("GRASS_TEXTURE", "GRASS_TEXTURE");
	// Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	// Skybox
	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");

	
	float distanceSquare = CLuaInterface::GetInstance()->getDistanceSquareValue("CalculateDistanceSquare", Vector3(0, 0, 0), Vector3(10, 10, 10));

	int a = 1000, b = 2000, c = 3000, d = 4000;
	CLuaInterface::GetInstance()->getVariableValues("GetMinMax", a, b, c, d);

	// Create a Waypoint inside WaypointManager
	//int aWayPoint = CWaypointManager::GetInstance()->AddWaypoint(Vector3(350.0f, 0.0f, 60.0f));
	//int anotherWaypoint = CWaypointManager::GetInstance()->AddWaypoint(aWayPoint, Vector3(350.0f, 0.0f, -60.0f));
	//CWaypointManager::GetInstance()->AddWaypoint(anotherWaypoint, Vector3(0.0f, 0.0f, 0.0f));
	
	//Use lua to create waypoints
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_1");
	CWaypointManager::GetInstance()->AddWaypoint(Vector3( CLuaInterface::GetInstance()->GetField("x")
		, CLuaInterface::GetInstance()->GetField("y")
		, CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_2");
	CWaypointManager::GetInstance()->AddWaypoint(Vector3(CLuaInterface::GetInstance()->GetField("x")
		, CLuaInterface::GetInstance()->GetField("y")
		, CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_3");
	CWaypointManager::GetInstance()->AddWaypoint(Vector3(CLuaInterface::GetInstance()->GetField("x")
		, CLuaInterface::GetInstance()->GetField("y")
		, CLuaInterface::GetInstance()->GetField("z")));
	CWaypointManager::GetInstance()->PrintSelf();


	
}

void SceneText::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
		Vector3 pos = theNode->GetEntity()->GetPosition();
		theNode->GetEntity()->SetPosition(Vector3(pos.x + 50.f, pos.y, pos.z + 50.f));
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('Z')) //Save
	{
		playerInfo->GetInstance()->SavePlayerInfo();
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('X')) //Load
	{
		playerInfo->GetInstance()->LoadPlayerInfo();
	}
	// <THERE>

	//spawnenemy
	enemySpawnTimer -= dt;
	if (enemySpawnTimer < 0)
	{
		enemySpawnTimer = enemySpawnCooldown;
		theEnemy = new CEnemy();
		theEnemy->SetTerrain(groundEntity);
		theEnemy->Init();
		theEnemy->Set_EnemyType("Chase");
		theEnemyList.push_back(theEnemy);
	}
	if (newEnemySpawn)
	{
		new_Enemy = new CEnemy();
		new_Enemy->SetTerrain(groundEntity);
		new_Enemy->Init();
		new_Enemy->Set_EnemyType("Waypoint");
		newEnemySpawn = false;
	}

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);
	Vector3 v1 = playerInfo->GetPos();
	Vector3 v2 = new_Enemy->GetPos();

	// Player to enemy collison
	if ((v1 - v2).Length() <= 1)
	{
		playerInfo->GetDamaged(10);
	}
	for (int index = 0; index < theEnemyList.size(); ++index)
	{
		Vector3 v3 = theEnemyList[index]->GetPos();
		if ((v1 - v3).Length() <= 1)
		{
			playerInfo->GetDamaged(10);
		}
	}

	// Player dead will render game over
	if (playerInfo->GetLife() <= 0)
	{
		SceneManager::GetInstance()->SetActiveScene("Gameover");
	}

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	/*std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss1.str());*/

	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "New_Enemy:" << new_Enemy->GetPos();
	textObj[2]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}
