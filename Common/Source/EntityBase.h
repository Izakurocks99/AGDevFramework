#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	enum ENTITY_TYPE
	{
		TYPE_NONE,
		TYPE_CHARACTER,
		TYPE_OBJECT,
		TYPE_PROJECTILE,
		TYPE_LASER,
		TYPE_MAX
	};
public:
	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	inline void SetType(const ENTITY_TYPE& _value) { type = _value; };
	inline ENTITY_TYPE GetType() { return type; };

	bool IsDone();
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);

protected:
	Vector3 position;
	Vector3 scale;

	bool isDone;
	bool m_bCollider;

	ENTITY_TYPE type;
};

#endif // ENTITY_BASE_H