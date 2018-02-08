#include "WaypointManager.h"
#include <iostream>

CWaypointManager *CWaypointManager::s_instance = 0;

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

bool CWaypointManager::RemoveWaypoint(const int _ID)
{
	bool result = false;

	// If Waypoint is not empty, then we procced to search
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			if ((*it)->GetID() == _ID)
			{
				// Remove all references to this Waypoint from other Waypoints
				RemoveRelatedWaypoint(_ID);

				// Remove this Waypoint from the WaypointManager
				delete *it;
				it = listOfWaypoints.erase(it);
				result = true;
				break;
			}
			if (it != listOfWaypoints.end())
				it++;
		}
	}
	return result;
}

CWaypoint * CWaypointManager::GetWaypoint(const int _ID)
{
	// If Waypoints is not empty, then we procceed to search
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			if ((*it)->GetID() == _ID)
			{
				return (CWaypoint*)(*it);
			}
			it++;
		}
	}
	return NULL;
}

CWaypoint* CWaypointManager::GetNearestWaypoint(Vector3 _position)
{
	CWaypoint* theNearestWaypoint = NULL;
	float m_fDistance = numeric_limits<float>::max();

	// If Waypoints has related Waypoints, then we proceed to search.
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			Vector3 aRelatedWaypoint = (*it)->GetPosition();

			float xDistance = _position.x - aRelatedWaypoint.x;
			float zDistance = _position.z - aRelatedWaypoint.z;
			float distanceSquared = (float)(xDistance*xDistance + zDistance*zDistance);
			if (m_fDistance > distanceSquared)
			{
				m_fDistance = distanceSquared;
				theNearestWaypoint = (CWaypoint*)(*it);
			}
			it++;
		}
	}
	return theNearestWaypoint;
}

int CWaypointManager::GetNumberOfWaypoints(void) const
{
	return listOfWaypoints.size();
}

void CWaypointManager::PrintSelf(void)
{
	cout << "==========================================================" << endl;
	cout << "CWaypointManager::PrintSelf" << endl;
	// If Waypoints has related Waypoints, then we procceed to search.
	if (listOfWaypoints.size() != 0)
	{
		// Iterate through all the Waypoints
		vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
		while (it != listOfWaypoints.end())
		{
			(*it)->PrintSelf();
			it++;
		}
	}
	cout << "==========================================================" << endl;
}

void CWaypointManager::RemoveRelatedWaypoint(const int _removeID)
{
	// Iterate through all the Waypoints
	std::vector<CWaypoint*>::iterator it = listOfWaypoints.begin();
	while (it != listOfWaypoints.end())
	{
		(*it)->RemoveRelatedWaypoint(_removeID);
		it++;
	}
}
