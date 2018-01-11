#include "Waypoint.h"
#include <limits>
#include <iostream>

CWaypoint::CWaypoint()
	:position(Vector3(0.0f,0.0f,0.0f))
	,m_ID(-1)
{
	relatedWaypoints.clear();
}

CWaypoint::~CWaypoint()
{
	relatedWaypoints.clear();
}

void CWaypoint::SetPosition(Vector3 _position)
{
	this->position = _position;
}

Vector3 CWaypoint::GetPosition(void) const
{
	return this->position;
}

void CWaypoint::SetID(const int _newID)
{
	this->m_ID = _newID;
}

int CWaypoint::GetID(void) const
{
	return this->m_ID;
}

void CWaypoint::AddRelatedWaypoint(CWaypoint * _relatedWaypoint)
{
	relatedWaypoints.push_back(_relatedWaypoint);
}

bool CWaypoint::RemoveRelatedWaypoint(const int _deleteID)
{
	// If waypoints has related waypoint, then search
	if (relatedWaypoints.size() != 0)
	{
		// Iterated through all the Waypoints
		std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			if ((*it)->GetID() == m_ID)
			{
				// We erase from the vector, relatedWaypoint
				it = relatedWaypoints.erase(it);
				return true;
			}
			it++;
		}
	}
	return false;
}

CWaypoint * CWaypoint::GetNearestWaypoint(void)
{
	CWaypoint* theNearestWaypoint = NULL;
	float m_fDistance = numeric_limits<float>::max();
	
	// If waypoint has related waypoints, then search
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the waypoints
		std::vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			Vector3 aRelatedWaypoint = (*it)->GetPosition();

			float xDistance = position.x - aRelatedWaypoint.x;
			// Position from tdj is X, i change to z
			float zDistance = position.z - aRelatedWaypoint.z;
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

int CWaypoint::GetNumberOfWaypoint(void) const
{
	return relatedWaypoints.size();
}

void CWaypoint::PrintSelf(void)
{
	cout << "Waypoint ID : " << m_ID << endl;
	cout << "Position    : " << position << endl;
	cout << "Related Waypoints : ";
	// If waypoint has related waypoints, then search
	if (relatedWaypoints.size() != 0)
	{
		// Iterate through all the waypoints
		vector<CWaypoint*>::iterator it = relatedWaypoints.begin();
		while (it != relatedWaypoints.end())
		{
			cout << (*it)->GetID() << endl;
			it++;
		}
	}
	else
		cout << "NOTHING" << endl;
}
