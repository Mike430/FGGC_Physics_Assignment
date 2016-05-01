#pragma once

#include "ParticleModel.h"

class PlaneParticleModel : public ParticleModel
{
public:
	PlaneParticleModel(Transform* transform, float mass);
	~PlaneParticleModel();

	void CalculateWheelSpeed();
	void CalculateThrust(float t);
	void CalculateVelocity();
	void CalculateLift(float t);

	void TruncateSpeed();

	void Update(float t);


	// --------------- Get/Set ------------------ //

	// Get/Set Plane Direction
	XMFLOAT3 GetPlaneDirection()							const { return _planeDirection; }
	void SetPlaneDirection(XMFLOAT3 planeDirection)			{ _planeDirection = planeDirection; }

	// Get/Set Plane Velocity
	XMFLOAT3 GetPlaneVelocity()								const { return _planeVelocity; }
	void SetEngineSpeed(XMFLOAT3 planeVelocity)				{ _planeVelocity = planeVelocity; }

	// Get/Set/Add Engine Speed
	float GetEngineSpeed()									const { return _engineSpeed; }
	void SetEngineSpeed(float engineSpeed)					{ _engineSpeed = engineSpeed; }
	void AddEngineSpeed(float engineSpeed);

	// Get/Set Engine Speed Limit
	float GetEngineSpeedLimit()								const { return _engineSpeedLimit; }
	void SetEngineSpeedLimit(float engineSpeedLimit)		{ _engineSpeedLimit = engineSpeedLimit; }

	// Get/Set Plane Lift
	float GetLift()											const { return _lift; }
	void SetLift(float lift)								{ _lift = lift; }

private:
	float _wheelRadius;
	float _wheelSpeed;
	int	_gearRatio;

	float _engineSpeed;
	float _engineSpeedLimit;

	float _thrust;
	float _lift;
	float _fwrdSpeed;

	XMFLOAT3 _planeVelocity;
	XMFLOAT3 _planeDirection;
};