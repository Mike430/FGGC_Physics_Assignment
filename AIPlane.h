#pragma once

#include "Plane.h"
#include "CarParticleModel.h"
#include "GameObject.h"

#include <directxmath.h>
#include <d3d11_1.h>

class AIPlane : public Plane
{
public:
	AIPlane(GameObject* carBody, vector<XMFLOAT3>* waypoints);
	~AIPlane();

	void Update(float t);

	// ----------------- PATH FINDING -------------------- //
	void FollowPoints();

	// --------------- STEERING BEHAVIOUR ---------------- //
	void GoToTarget();

private:
	vector<XMFLOAT3>* _waypoints;

	bool waypointReached;

	XMFLOAT3 targetPosition;
	int targetIndex;
};

