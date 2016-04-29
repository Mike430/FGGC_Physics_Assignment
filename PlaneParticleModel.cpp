#include "PlaneParticleModel.h"


PlaneParticleModel::PlaneParticleModel(Transform* transform, float mass) : ParticleModel(transform, mass)
{
	_gearRatio = 10;
	_wheelSpeed = 1.0f;
	_engineSpeed = 0.0f;
	_wheelRadius = 10.0f;

	_engineSpeedLimit = 0.3f;

	_planeVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

PlaneParticleModel::~PlaneParticleModel()
{
}

void PlaneParticleModel::AddEngineSpeed(float engineSpeed)
{
	_engineSpeed += engineSpeed;
	TruncateSpeed();
}

void PlaneParticleModel::CalculateWheelSpeed()
{
	TruncateSpeed();

	_wheelSpeed = _engineSpeed / _gearRatio;
}

void PlaneParticleModel::TruncateSpeed()
{
	if (_engineSpeed > _engineSpeedLimit)
	{
		_engineSpeed = _engineSpeedLimit;
	}
}

void PlaneParticleModel::CalculateThrust(float t)
{
	_thrust = ((_wheelSpeed * 60) * 2 * XM_PI * _wheelRadius) / t;
}

void PlaneParticleModel::CalculateVelocity()
{
	_planeVelocity.x = _planeDirection.x * _thrust;
	_planeVelocity.y = _planeDirection.y * _thrust;
	_planeVelocity.z = _planeDirection.z * _thrust;
}

void PlaneParticleModel::Update(float t)
{
	CalculateWheelSpeed();
	CalculateThrust(t);
	CalculateVelocity();

	// Update Particle Model
	ParticleModel::Update(t);
}