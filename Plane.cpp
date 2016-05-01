#include "Plane.h"
#include <iostream>

Plane::Plane(GameObject* _planeBody, vector < GameObject* > _planeWheels)
{
	planeBody = _planeBody;
	planeWheels = _planeWheels;

	planePos = planeBody->GetTransform()->GetPosition();

	planeRotation = 0.0f;// -350.0f;
	planeRotationSpeed = 0.001f;
	planeWheelRotation = 0.0f;

	engineSpeedAdd = 0.0008f;
}

Plane::~Plane()
{

}

void Plane::Input()
{
	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)planeBody->GetParticleModel();
	float engineSpeed = planeBodyModel->GetEngineSpeed();

	planePos = planeBody->GetTransform()->GetPosition();

	if (GetAsyncKeyState('B'))
	{
		planePos = XMFLOAT3(planePos.x, 20.0f, planePos.z);
		planeBody->GetTransform()->SetPosition(planePos);
	}

	if (GetAsyncKeyState('W'))
	{
		// Set Car Engine Speed
		planeBodyModel->AddEngineSpeed((engineSpeedAdd));
	}
	else if (GetAsyncKeyState('S'))
	{
		if (engineSpeed > 0)
		{
			planeBodyModel->AddEngineSpeed(-engineSpeedAdd * 2.0f);
		}
		else
		{
			planeBodyModel->AddEngineSpeed((-engineSpeedAdd / 1.5f));
		}
	}
	else
	{
		float engineSpeed = planeBodyModel->GetEngineSpeed();

		if (engineSpeed > 0)
		{
			planeBodyModel->AddEngineSpeed(-engineSpeedAdd);

			if (engineSpeed < 0.0008f && engineSpeed != 0.0f)
			{
				planeBodyModel->SetEngineSpeed(0.0f);
			}
		}
		else if (engineSpeed < 0)
		{
			planeBodyModel->AddEngineSpeed(engineSpeedAdd);
		}
	}

	// Car Rotation Check
	if (GetAsyncKeyState('A'))
	{
		planeWheelRotation -= 0.01f;
	}
	else if (GetAsyncKeyState('D'))
	{
		planeWheelRotation += 0.01f;
	}
	else
	{
		if (planeWheelRotation < 0)
		{
			planeWheelRotation += 0.02f;
		}
		else if (planeWheelRotation > 0)
		{
			planeWheelRotation -= 0.02f;
		}

		if (planeWheelRotation < 0.05f && planeWheelRotation > -0.05f)
		{
			planeWheelRotation = 0;
		}
	}
}

void Plane::CalculateForwardVector()
{
	planeBody->GetTransform()->GetRotation();

	planeForwardVector.x = sin((planeRotation / 17.425f) * (XM_PI / 180.0f));
	planeForwardVector.y = 0.0f;
	planeForwardVector.z = cos((planeRotation / 17.425f) * (XM_PI / 180.0f));

	float planeDirectionMag = sqrt((planeForwardVector.x * planeForwardVector.x) + (planeForwardVector.y * planeForwardVector.y) + (planeForwardVector.z * planeForwardVector.z));
	planeForwardVector = XMFLOAT3((planeForwardVector.x / planeDirectionMag), (planeForwardVector.y / planeDirectionMag), (planeForwardVector.z / planeDirectionMag));

	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)planeBody->GetParticleModel();
	planeBodyModel->SetPlaneDirection(planeForwardVector);

	// Reset Car Rotation if over 360 or 0 degrees
	if (planeRotation <= -6247.0f || planeRotation >= 6247.0f)
	{
		planeRotation = 0.0f;
	}
}

void Plane::Update(float t)
{
	CalculateForwardVector();

	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)planeBody->GetParticleModel();
	float engineSpeed = planeBodyModel->GetEngineSpeed();

	// Update Car Wheels Rotation
	planeBody->GetChildren().at(0)->GetTransform()->SetRotation(engineSpeed * 50, planeWheelRotation, 0.0f);
	planeBody->GetChildren().at(1)->GetTransform()->SetRotation(engineSpeed * 50, planeWheelRotation, 0.0f);
	planeBody->GetChildren().at(2)->GetTransform()->SetRotation(engineSpeed * 50, 0.0f, 0.0f);
	planeBody->GetChildren().at(3)->GetTransform()->SetRotation(engineSpeed * 50, 0.0f, 0.0f);

	// Check Plane type --- you will want to add an AI controlled plane
	string type = planeBody->GetType();

	if (type == "Plane")
	{
		XMFLOAT3 velTemp = planeBody->GetParticleModel()->GetVelocity();
		XMFLOAT3 carVelTemp = planeBodyModel->GetPlaneVelocity();

		velTemp.x += carVelTemp.x;
		velTemp.y += carVelTemp.y;
		velTemp.z += carVelTemp.z;

		planeBodyModel->SetVelocity(velTemp);

		// Limit Wheel Rotation
		float planeWheelLimit = 0.1f;

		if (planeWheelRotation <= -planeWheelLimit)
		{
			planeWheelRotation = -planeWheelLimit;
		}
		else if (planeWheelRotation >= planeWheelLimit)
		{
			planeWheelRotation = planeWheelLimit;
		}
	}
	else
	{
		planePos = planeBody->GetTransform()->GetPosition();
		XMFLOAT3 velTemp = planeBody->GetParticleModel()->GetVelocity();

		// AI positioning
		XMFLOAT3 planeVelocity = planeBodyModel->GetPlaneVelocity();
		planePos = XMFLOAT3((planePos.x + planeVelocity.x), (planePos.y + planeVelocity.y), (planePos.z + planeVelocity.z));
		planeBody->GetTransform()->SetPosition(planePos);

		// Limit Wheel Rotation
		float planeWheelLimit = 0.1f;

		if (planeWheelRotation <= -planeWheelLimit)
		{
			planeWheelRotation = -planeWheelLimit;
		}
		else if (planeWheelRotation >= planeWheelLimit)
		{
			planeWheelRotation = planeWheelLimit;
		}
	}


	// Rotate Car in consideration to the Wheels Rotation
	if (engineSpeed > 0)
	{
		planeRotation += (planeWheelRotation * 100) * (engineSpeed * 20);
	}
	else if (engineSpeed < 0)
	{
		engineSpeed *= -1;
		planeRotation -= (planeWheelRotation * 100) * (engineSpeed * 20);
	}

	//Modify obj representation
	planeBody->GetTransform()->SetRotation(0.0f, (planeRotation * planeRotationSpeed), 0.0f);

	// Update Transform
	planeBody->Update(t);

	// Update Particle Model
	planeBody->GetParticleModel()->Update(t);
}

void Plane::Draw(ID3D11DeviceContext* _pImmediateContext)
{
	planeBody->Draw(_pImmediateContext);
}
