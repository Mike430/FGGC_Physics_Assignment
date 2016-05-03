#pragma once

#include "PlaneParticleModel.h"
#include "GameObject.h"

class Plane
{
public:
	Plane(GameObject* planeBody);
	~Plane();

	virtual void Update(float t);
	void Draw(ID3D11DeviceContext* _pImmediateContext);

	// Plane Input
	void Input();

	// Get Plane Body and Wheels
	GameObject* GetPlaneBody()									const { return _planeBody; };
	//vector< GameObject* > GetPlaneWheels()						const { planeWheels; };

	// Get/Set Plane Position
	XMFLOAT3 GetPlanePosition()									const { return _planePos; };
	void SetPlanePosition(XMFLOAT3 _planePos)					{ _planePos = _planePos; };
	void SetPlanePosition(float x, float y, float z)			{ _planePos.x = x, _planePos.y = y, _planePos.z = z; };

	// Get/Calculate Plane Forward Vector
	XMFLOAT3 GetForwardVector()									const { return _planeForwardVector; };
	void CalculateForwardVector();

	// Get/Set Plane Rotation
	float GetPlaneRotation()									const { return _planeRotation; };
	void SetPlaneRotation(float _planeRotation)					{ _planeRotation = _planeRotation; _planeRotation = _planeRotation * (XM_PI / 180);
																_planeBody->GetTransform()->SetRotation(0.0f, _planeRotation, 0.0f); };
	void AddPlaneRotation(float _planeRotation)					{ _planeRotation = _planeRotation; _planeRotation = _planeRotation * (XM_PI / 180);
																_planeBody->GetTransform()->SetRotation(0.0f, _planeRotation, 0.0f); };

	// Get/Set Plane Rotation Speed
	// float GetPlaneRotationSpeed()								const { return planeRotationSpeed; };
	// void SetPlaneRotationSpeed(float _planeRotationSpeed)		{ planeRotationSpeed = planeRotationSpeed; };

	// Get/Set Plane Wheel Rotation
	float GetPlaneWheelRotation() const							{ return _planeTurningAngle; };
	void SetPlaneWheelRotation(float _planeWheelRotation)		{ _planeTurningAngle = _planeWheelRotation; };
	void AddPlaneWheelRotation(float _planeWheelRotation)		{ _planeTurningAngle += _planeWheelRotation; };

private:
	// Plane Object
	GameObject* _planeBody;

	// Plane Properties
	XMFLOAT3 _planePos;
	XMFLOAT3 _planeForwardVector;

	float _planeRotation;
	float _planeRotationSpeed;
	float _planeTurningAngle;

	float _engineSpeedAdd;
};