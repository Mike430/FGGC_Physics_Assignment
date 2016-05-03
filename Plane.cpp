#include "Plane.h"
#include <iostream>

Plane::Plane(GameObject* planeBody)
{
	_planeBody = planeBody;

	_planePos = _planeBody->GetTransform()->GetPosition();

	_planeRotation = 0.0f;// -350.0f;
	_planeRotationSpeed = 0.001f;
	_planeTurningAngle = 0.0f;

	_engineSpeedAdd = 0.0008f;
}

Plane::~Plane()
{

}

void Plane::Input()
{
	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)_planeBody->GetParticleModel();
	float engineSpeed = planeBodyModel->GetEngineSpeed();

	_planePos = _planeBody->GetTransform()->GetPosition();

	if (GetAsyncKeyState('B'))
	{
		_planePos = XMFLOAT3(_planePos.x, 20.0f, _planePos.z);
		_planeBody->GetTransform()->SetPosition(_planePos);
	}

	if (GetAsyncKeyState('W'))
	{
		// Set Car Engine Speed
		planeBodyModel->AddEngineSpeed((_engineSpeedAdd));
	}
	else if (GetAsyncKeyState('S'))
	{
		if (engineSpeed > 0)
		{
			planeBodyModel->AddEngineSpeed(-_engineSpeedAdd * 2.0f);
		}
		else
		{
			planeBodyModel->AddEngineSpeed((-_engineSpeedAdd / 1.5f));
		}
	}
	else
	{
		float engineSpeed = planeBodyModel->GetEngineSpeed();

		if (engineSpeed > 0)
		{
			planeBodyModel->AddEngineSpeed(-_engineSpeedAdd);

			if (engineSpeed < 0.0008f && engineSpeed != 0.0f)
			{
				planeBodyModel->SetEngineSpeed(0.0f);
			}
		}
		else if (engineSpeed < 0)
		{
			planeBodyModel->AddEngineSpeed(_engineSpeedAdd);
		}
	}

	// Car Rotation Check
	if (GetAsyncKeyState('A'))
	{
		_planeTurningAngle -= 0.01f;
	}
	else if (GetAsyncKeyState('D'))
	{
		_planeTurningAngle += 0.01f;
	}
	else
	{
		if (_planeTurningAngle < 0)
		{
			_planeTurningAngle += 0.02f;
		}
		else if (_planeTurningAngle > 0)
		{
			_planeTurningAngle -= 0.02f;
		}

		if (_planeTurningAngle < 0.05f && _planeTurningAngle > -0.05f)
		{
			_planeTurningAngle = 0;
		}
	}
}

void Plane::CalculateForwardVector()
{
	_planeBody->GetTransform()->GetRotation();

	_planeForwardVector.x = sin((_planeRotation / 17.425f) * (XM_PI / 180.0f));
	_planeForwardVector.y = 0.0f;
	_planeForwardVector.z = cos((_planeRotation / 17.425f) * (XM_PI / 180.0f));

	float planeDirectionMag = sqrt((_planeForwardVector.x * _planeForwardVector.x) + (_planeForwardVector.y * _planeForwardVector.y) + (_planeForwardVector.z * _planeForwardVector.z));
	_planeForwardVector = XMFLOAT3((_planeForwardVector.x / planeDirectionMag), (_planeForwardVector.y / planeDirectionMag), (_planeForwardVector.z / planeDirectionMag));

	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)_planeBody->GetParticleModel();
	planeBodyModel->SetPlaneDirection(_planeForwardVector);

	// Reset Car Rotation if over 360 or 0 degrees
	if (_planeRotation <= -6247.0f || _planeRotation >= 6247.0f)
	{
		_planeRotation = 0.0f;
	}
}

void Plane::Update(float t)
{
	CalculateForwardVector();

	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)_planeBody->GetParticleModel();
	float engineSpeed = planeBodyModel->GetEngineSpeed();

	// Check Plane type --- you will want to add an AI controlled plane
	string type = _planeBody->GetType();

	if (type == "Plane")
	{
		//Don't edit velocity, edit the forces that edit the velocity.
		XMFLOAT3 velTemp = _planeBody->GetParticleModel()->GetVelocity();
		XMFLOAT3 carVelTemp = planeBodyModel->GetPlaneVelocity();

		velTemp.x += carVelTemp.x;
		velTemp.y += carVelTemp.y;
		velTemp.z += carVelTemp.z;

		planeBodyModel->SetVelocity(velTemp);

		// Limit Wheel Rotation
		float planeWheelLimit = 0.1f;

		if (_planeTurningAngle <= -planeWheelLimit)
		{
			_planeTurningAngle = -planeWheelLimit;
		}
		else if (_planeTurningAngle >= planeWheelLimit)
		{
			_planeTurningAngle = planeWheelLimit;
		}
	}
	else
	{
		_planePos = _planeBody->GetTransform()->GetPosition();
		XMFLOAT3 velTemp = _planeBody->GetParticleModel()->GetVelocity();

		// AI positioning
		XMFLOAT3 planeVelocity = planeBodyModel->GetPlaneVelocity();
		_planePos = XMFLOAT3((_planePos.x + planeVelocity.x), (_planePos.y + planeVelocity.y), (_planePos.z + planeVelocity.z));
		_planeBody->GetTransform()->SetPosition(_planePos);

		// Limit Wheel Rotation
		float planeWheelLimit = 0.1f;

		if (_planeTurningAngle <= -planeWheelLimit)
		{
			_planeTurningAngle = -planeWheelLimit;
		}
		else if (_planeTurningAngle >= planeWheelLimit)
		{
			_planeTurningAngle = planeWheelLimit;
		}
	}


	// Rotate Car in consideration to the Wheels Rotation
	if (engineSpeed > 0)
	{
		_planeRotation += (_planeTurningAngle * 100) * (engineSpeed * 20);
	}
	else if (engineSpeed < 0)
	{
		engineSpeed *= -1;
		_planeRotation -= (_planeTurningAngle * 100) * (engineSpeed * 20);
	}

	//Modify obj representation
	_planeBody->GetTransform()->SetRotation(0.0f, (_planeRotation * _planeRotationSpeed), 0.0f);

	// Update Transform
	_planeBody->Update(t);

	// Update Particle Model
	_planeBody->GetParticleModel()->Update(t);
}

void Plane::Draw(ID3D11DeviceContext* _pImmediateContext)
{
	_planeBody->Draw(_pImmediateContext);
}
