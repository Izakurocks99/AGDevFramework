#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
//#include "../PlayerInfo/PlayerInfo.h"

#include "../Waypoint/WaypointManager.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

CEnemy::CEnemy()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, m_iSeed(0)
	,m_iWayPointIndex(-1)
{
	listOfWaypoints.clear();
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set up Waypoint
	listOfWaypoints.push_back(0);
	listOfWaypoints.push_back(1);
	listOfWaypoints.push_back(2);

	// Set the current values
	//position.Set(10.0f, 0.0f, 0.0f);
	position = GenerateTarget();
	CWaypoint* nextWaypoint = GetNextWaypoint();
	if (nextWaypoint)
		target = nextWaypoint->GetPosition();
	else
		target = Vector3(0, 0, 0);
	cout << "Next target: " << target << endl;

	up.Set(0.0f, 1.0f, 0.0f);

	// Set Range
	Set_EnemyRange(50.0f);

	// Set speed
	m_dSpeed = 10.0;

	//SetLife
	SetLife(3);

	// Initialise the LOD meshes
	InitLOD("H_EnemyBody", "M_EnemyBody", "L_EnemyBody");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));


	// Add to EntityManager
	baseNode = CSceneGraph::GetInstance()->AddNode(this);
	EntityManager::GetInstance()->AddEntity(this, true);

	//add head  
	GenericEntity* head = Create::Entity("cube", Vector3(position.x, position.y + 1.1, position.z), GenericEntity::TYPE_CHARACTER);
	head->SetCollider(true);
	head->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	head->InitLOD("H_EnemyHead", "M_EnemyHead", "L_EnemyHead");
	head->SetLife(1);
	CSceneNode* childNode = baseNode->AddChild(head);
	if (childNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}
}

void CEnemy::Init(float x, float z)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(x, 0.0f, z);
	if (m_pTerrain)
		target = GenerateTarget();
	else
		target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 10.0;

	// Initialise the LOD meshes
	InitLOD("cube", "sphere", "cubeSG");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);
}

// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CEnemy::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

// Update
void CEnemy::Update(double dt)
{
	// Chasing
	if (((position - CPlayerInfo::GetInstance()->GetPos()).Length()) <= enemyRange_)
	{
		SetTarget(CPlayerInfo::GetInstance()->GetPos());
		// Debugging
		/*if (enemyType_ == "Waypoint")
			cout << "Saw Player!!" << endl;*/
	}
	// Waypoint (random)
	if (Get_EnemyType() == "Chase" && ((target - position).LengthSquared() < 25.0f))
	{
		CWaypoint* nextWaypoint = GetRandWaypoint();
		if (nextWaypoint)
			target = nextWaypoint->GetPosition();
		else
			target = Vector3(0, 0, 0);
		cout << "Chase_chan's Next target: " << target << endl;
	}
	// Waypoint (non random)
	else if (Get_EnemyType() == "Chase" && ((target - position).LengthSquared() < 25.0f))
	{
		CWaypoint* nextWaypoint = GetNextWaypoint();
		if (nextWaypoint)
			target = nextWaypoint->GetPosition();
		else
			target = Vector3(0, 0, 0);
		cout << "Normie_chan's Next target: " << target << endl;
	}
	Vector3 viewVector = (target - position).Normalized();
	position += viewVector * (float)m_dSpeed * (float)dt;
	//	cout << position << " - " << target << "..." << viewVector << endl;

	//if have child
	if (baseNode->GetNumOfChild() > 0)
	{
		for (int i = 1; i <= baseNode->GetNumOfChild(); ++i)
		{
			//put update child
			baseNode->GetEntity(baseNode->GetID() + i)->GetEntity()->SetPosition(Vector3(position.x, position.y + 2.1, position.z));
		}
	}
	//else
	//{
	//	if (CSceneGraph::GetInstance()->DeleteNode(this) == true)
	//		cout << "This Removed" << endl;
	//	SetIsDone(true);
	//}

	// Constrain the position
	Constrain();

	//// Update the target
	//if (position.z > 400.0f)
	//	target.z = position.z * -1;
	//else if (position.z < -400.0f)
	//	target.z = position.z * -1;

	
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	if (abs(((target.x - position.x) * (target.x - position.x) - (target.z - position.z)*(target.z - position.z))) < m_dSpeed)
	{
		target = GenerateTarget();
	}

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}

// Generate New Target
Vector3 CEnemy::GenerateTarget(void)
{
	return Vector3(	rand() % (int)((maxBoundary.x - minBoundary.x)*0.5),	
					-0.5f, 
					rand() % (int)((maxBoundary.x - minBoundary.x)*0.5));
}

CWaypoint* CEnemy::GetNextWaypoint(void)
{
	if ((int)listOfWaypoints.size() > 0)
	{
		m_iWayPointIndex++;
		if (m_iWayPointIndex >= (int)listOfWaypoints.size())
			m_iWayPointIndex = 0;
		
		//CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[m_iWayPointIndex]);
		return CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[m_iWayPointIndex]);
	}	

	return NULL;
}

CWaypoint * CEnemy::GetRandWaypoint(void)
{
	if ((int)listOfWaypoints.size() > 0)
	{
		int rand_Index = Math::RandIntMinMax(1, (int)listOfWaypoints.size() - 1);
		
		//CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[m_iWayPointIndex]);
		return CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[rand_Index]);
	}
	return NULL;
}

void CEnemy::Set_EnemyType(string _newEnemyType)
{
	enemyType_ = _newEnemyType;
}

string CEnemy::Get_EnemyType(void)
{
	return enemyType_;
}

void CEnemy::Set_EnemyRange(float _newEnemyRange)
{
	enemyRange_ = _newEnemyRange;
}

float CEnemy::Get_EnemyRange(void)
{
	return enemyRange_;
}

// Set random seed
void CEnemy::SetRandomSeed(const int m_iSeed)
{
	this->m_iSeed = m_iSeed;
}
