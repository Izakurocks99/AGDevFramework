#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "Vector3.h"
#include <vector>

using namespace std;

class CWaypoint
{
public:
	CWaypoint();
	virtual ~CWaypoint();

	// Set position
	void SetPosition(Vector3 _position);
	// Get position
	Vector3 GetPosition(void) const;

	// Set ID
	void SetID(const int _newID);
	// Get ID
	int GetID(void) const;

	// Add related waypoint
	void AddRelatedWaypoint(CWaypoint* _relatedWaypoint);
	// Remove related waypoint
	bool RemoveRelatedWaypoint(const int _deleteID);
	// Get nearest waypoint amongst related waypoints
	CWaypoint* GetNearestWaypoint(void);
	// Get the number of related waypoints
	int GetNumberOfWaypoint(void) const;

	// PrintSelf
	void PrintSelf(void);

protected:
	Vector3 position;
	int m_ID;

	vector<CWaypoint*> relatedWaypoints;
};

#endif // !WAYPOINT_H
