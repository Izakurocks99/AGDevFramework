#include "WaypointManager.h"
#include <iostream>

CWaypointManager::CWaypointManager(void)
	:m_ID(-1)
{
	listOfWaypoints.clear();
}

CWaypointManager::~CWaypointManager()
{
	// If listOfWaypoints is not empty, then delete
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all waypoints
		// Destory the children first
		vector<CWaypoint*>::iterator it;
		for (it = listOfWaypoints.begin(); it != listOfWaypoints.end(); ++it)
		{
			delete *it;
		}
	}
	// Kill all waypoints
	listOfWaypoints.clear();
}

int CWaypointManager::AddWaypoint(Vector3 _position)
{
	m_ID++;
	CWaypoint* aNewWaypoint = new CWaypoint();
	aNewWaypoint->SetID(m_ID);
	aNewWaypoint->SetPosition(_position);
	listOfWaypoints.push_back(aNewWaypoint);
	return m_ID;
}

int CWaypointManager::AddWaypoint(const int _ID, Vector3 _position)
{
	// If waypoints is not empty, then search
	if (listOfWaypoints.size() != 0)
	{
		for (unsigned i = 0; i < listOfWaypoints.size(); ++i)
		{
			CWaypoint* aWaypoint = listOfWaypoints[i];
			if (aWaypoint->GetID() == _ID)
			{
				(this->m_ID)++;
				CWaypoint* aNewWaypoint = new CWaypoint();
				aNewWaypoint->SetID(this->m_ID);
				aNewWaypoint->SetPosition(_position);
				listOfWaypoints.push_back(aNewWaypoint);

				// Add the current Waypoint as a related waypoint to the new waypoint
				aNewWaypoint->AddRelatedWaypoint(aWaypoint);
				// Add this new waypoint as a related waypoint to the current waypoint
				aWaypoint->AddRelatedWaypoint(aNewWaypoint);

				return this->m_ID;
			}
		}
	}
	return -1;
}
