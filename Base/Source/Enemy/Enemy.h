#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../SceneGraph/SceneGraph.h"

#include "../Waypoint/Waypoint.h"

#include <vector>
using namespace std;

class Mesh;

class CEnemy :
	public GenericEntity
{
protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;

	int m_iSeed;
	CSceneNode* baseNode;

	// Vector containing IDs of Waypoints
	vector<int> listOfWaypoints;
	// Current ID of Waypoint
	int m_iWayPointIndex;

	// Enemy Type
	string enemyType_;

	// Enemy Range
	float enemyRange_;

public:
	CEnemy(void);
	virtual ~CEnemy();

	void Init(void);
	void Init(float x, float y);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);
	// Set random seed
	void SetRandomSeed(const int m_iSeed);
	// Generate New Target
	Vector3 GenerateTarget(void);

	// Get next Waypoint for this 
	CWaypoint* GetNextWaypoint(void);

	// Get random waypoint
	CWaypoint* GetRandWaypoint(void);

	// Set Enemy Type
	void Set_EnemyType(string _newEnemyType);
	// Get Enemy Type
	string Get_EnemyType(void);

	// Set Enemy Range
	void Set_EnemyRange(float _newEnemyRange);
	// Get Enemy Range
	float Get_EnemyRange(void);
};
