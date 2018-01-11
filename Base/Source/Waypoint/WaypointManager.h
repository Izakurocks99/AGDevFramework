#pragma once
#include "Waypoint.h"
#include "Vector3.h"
#include <vector>

using namespace std;

class CWaypointManager
{
protected:
	static CWaypointManager *s_instance;
	CWaypointManager(void);

public:
	static CWaypointManager *GetInstance()
	{
		if (!s_instance)
			s_instance = new CWaypointManager;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}

	virtual ~CWaypointManager();

	// Add waypoint
	int AddWaypoint(Vector3 _position);
	// Add waypoint base on ID
	int AddWaypoint(const int _ID, Vector3 _position);
	// Remove waypoint based on ID
	bool RemoveWaypoint(const int _ID);

	// Get a waypoint based on its ID
	CWaypoint* GetWaypoint(const int _ID);
	// Get nearest waypoint amongst related waypoints
	CWaypoint GetNearestWaypoint(Vector3 _position);
	// Get the number of related waypoints
	int GetNumberOfWaypoints(void) const;

	// PrintSelf
	void PrintSelf(void);

protected:
	// The next ID to assign to a waypoint
	int m_ID;

	vector<CWaypoint*> listOfWaypoints;

	// Remove related waypoint
	void RemoveRelatedWaypoint(const int _removeID);
};