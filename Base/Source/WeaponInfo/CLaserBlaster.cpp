#include "CLaserBlaster.h"
#include "../Projectile/Laser.h"

CLaserBlaster::CLaserBlaster()
{

}

CLaserBlaster::~CLaserBlaster()
{

}

// Initialize shit
void CLaserBlaster::Init()
{
	CWeaponInfo::Init();

	// Number of ammo per clip
	magRounds = 5;
	// Max ammo per clip
	maxMagRounds = 5;
	// number of rounds carried by player
	totalRounds = 10;
	// Max number of rounds can be carried by player
	maxTotalRounds = 10;

	// Firing Delay
	timeBetweenShots = 0.1667;
	// Current delay 
	elapsedTime = 0.0;
	// Boolean flag if check if weapon can fire
	bFire = true;
}

// Discharge this weapon
void CLaserBlaster::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		//If there is ammo in mag, fire is allowed
		if (magRounds > 0)
		{
			Vector3 _direction = (target - position).Normalized();
			// Create a laser 
			CLaser* aLaser = Create::Laser("laser", position, _direction, 10.f, 2.f, 100.f, _source);
			aLaser->SetCollider(true);
			aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			--magRounds;
		}
	}
}