#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"
#include "SceneGraph\SceneGraph.h"
#include "SceneGraph\SceneNode.h"

class EntityBase;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase* _newEntity);
	bool RemoveEntity(EntityBase* _existingEntity);

private:
	EntityManager();
	virtual ~EntityManager();

	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check intersection between a line segement and a plane
	bool GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 p2, Vector3 &Hit);
	// Check intersection between a line segement and a plane
	bool CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end, Vector3 minAABB, Vector3 maxAABB, Vector3 &Hit);
	// Check 2 position within box region
	bool In_Box(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);
	// Check if any Collider is colliding with another Collider
	bool CheckForCollision(void);

	std::list<EntityBase*> entityList;
};

#endif // ENTITY_MANAGER_H