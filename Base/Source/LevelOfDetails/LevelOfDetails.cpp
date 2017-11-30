#include "LevelOfDetails.h"
#include "MeshBuilder.h"

// Constructor
CLevelOfDetails::CLevelOfDetails() :
	modelMesh_HighDetails(NULL),
	modelMesh_MidDetails(NULL),
	modelMesh_LowDetail(NULL),
	m_bActive(false),
	theDetailLevel(HIGH_DETAILS)
{
}

// Destructor
CLevelOfDetails::~CLevelOfDetails()
{
	modelMesh_HighDetails = NULL;
	modelMesh_MidDetails = NULL;
	modelMesh_LowDetail = NULL;
}

// Initialise the LOD System
bool CLevelOfDetails::InitLOD(const std::string & _meshName_High, const std::string & _meshName_Mid, const std::string & _meshName_Low)
{
	// Retrieve the Meshes from MeshBuilder's library and assign to the 3 Mesh pointers
	modelMesh_HighDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_High);
	modelMesh_MidDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_Mid);
	modelMesh_LowDetail = MeshBuilder::GetInstance()->GetMesh(_meshName_Low);

	// If any of the mesh is not loaded, then return false and avoid setting the LOD to active 
	if ((modelMesh_HighDetails == nullptr) || (modelMesh_MidDetails == nullptr) || (modelMesh_LowDetail == nullptr))
		return false;

	// Set the LOD to active
	SetLODStatus(true);
	return true;
}

// Destory the LOD System
bool CLevelOfDetails::DestoryLOD(void)
{
	if (modelMesh_HighDetails)
	{
		delete modelMesh_HighDetails;
		modelMesh_HighDetails = NULL;
	}
	if (modelMesh_MidDetails) 
	{
		delete modelMesh_MidDetails;
		modelMesh_MidDetails = NULL;
	}
	if (modelMesh_LowDetail)
	{
		delete modelMesh_LowDetail;
		modelMesh_LowDetail = NULL;
	}
	return false;
}

// Set the Status of the LOD
void CLevelOfDetails::SetLODStatus(const bool _bActive)
{
	m_bActive = _bActive;
}

// Get the LOD Status
bool CLevelOfDetails::GetLODStatus(void) const
{
	return m_bActive;
}

// Set the Mesh for a specified detail level
bool CLevelOfDetails::SetLODMesh(Mesh * _theMesh, const DETAIL_LEVEL _theDetailLevel)
{
	if (theDetailLevel == HIGH_DETAILS)
		modelMesh_HighDetails = _theMesh;
	else if (theDetailLevel == MID_DETAILS)
		modelMesh_MidDetails = _theMesh;
	else if (theDetailLevel == LOW_DETAILS)
		modelMesh_LowDetail = _theMesh;
	else
		return false;
	
	return true;
}

// Get the Mesh base on the current detail level
Mesh * CLevelOfDetails::GetLODMesh(void) const
{
	if (theDetailLevel == HIGH_DETAILS)
		return modelMesh_HighDetails;
	if (theDetailLevel == MID_DETAILS)
		return modelMesh_MidDetails;
	if (theDetailLevel == LOW_DETAILS)
		return modelMesh_LowDetail;

	return NULL;
}

Mesh * CLevelOfDetails::GetLODMesh(const DETAIL_LEVEL _theDetailLevel) const
{
	if (_theDetailLevel == HIGH_DETAILS)
		return modelMesh_HighDetails;
	if (_theDetailLevel == MID_DETAILS)
		return modelMesh_MidDetails;
	if (_theDetailLevel == LOW_DETAILS)
		return modelMesh_LowDetail;

	return NULL;
}

// Set the current detail level
bool CLevelOfDetails::SetDetailLevel(const DETAIL_LEVEL _theDetailLevel)
{
	if((_theDetailLevel >= NO_DETAILS)&&(_theDetailLevel < NUM_DETAILS_LEVEL))
	{ 
		this->theDetailLevel = _theDetailLevel;
		return true;
	}

	return false;
}

// Get the Current Detail Level
int CLevelOfDetails::GetDetailLevel(void) const
{
	return theDetailLevel;
}