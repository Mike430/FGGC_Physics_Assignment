#include "Plane.h"
#include <iostream>

Plane::Plane(GameObject* planeBody, vector<GameObject*>* collidables)
{
	_planeBody = planeBody;
	_collidables = collidables;
	_planeBody->GetParticleModel()->SetExtents(XMFLOAT3(planeBody->GetWidth()/2, planeBody->GetHeight()/2, planeBody->GetDepth()/2));

	_planePos = _planeBody->GetTransform()->GetPosition();

	_planeRotation = 0.0f;// -350.0f;
	_planeRotationSpeed = 0.001f;
	_planeTurningAngle = 0.0f;
	_planeRoll = 0.0f;
	_planePitch = 0.0f;

	_engineSpeedAdd = 0.0008f;	
}

Plane::~Plane()
{

}

void Plane::Input(float t)
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
		/*else
		{
			planeBodyModel->AddEngineSpeed((-_engineSpeedAdd / 1.5f));
		}*/
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
	float deltaTime = t * 0.001;
	float turnRate = 1.0f * deltaTime;
	float visualTurnRate = 1.0f * deltaTime;

	if (GetAsyncKeyState('A'))
	{
		_planeTurningAngle -= turnRate;
		
		if (_planeRoll < _planeMaxRoll)
			_planeRoll += visualTurnRate;
	}
	else if (GetAsyncKeyState('D'))
	{
		_planeTurningAngle += turnRate;

		if (_planeRoll > -_planeMaxRoll)
			_planeRoll -= visualTurnRate;
	}
	else
	{
		if (_planeTurningAngle < 0)
			_planeTurningAngle += turnRate;

		else if (_planeTurningAngle > 0)
			_planeTurningAngle -= turnRate;

		if (_planeRoll > 0)
			_planeRoll -= visualTurnRate;
		else if (_planeRoll < 0)
			_planeRoll += visualTurnRate;

		if (_planeTurningAngle < 0.01f && _planeTurningAngle > -0.01f)
			_planeTurningAngle = 0;

		if (_planeRoll < 0.01f && _planeRoll > -0.01f)
			_planeRoll = 0;
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

	float lift = planeBodyModel->GetLift();
	if (lift > 10)
		_planePitch = ((lift * 0.05) - 0.5f) * -1.0f;
	else
		_planePitch = 0.0f;

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
		_planeRotation += (_planeTurningAngle * 40) * (engineSpeed * 20);
	}
	else if (engineSpeed < 0)
	{
		engineSpeed *= -1;
		_planeRotation -= (_planeTurningAngle * 40) * (engineSpeed * 20);
	}

	//Modify obj representation
	_planeBody->GetTransform()->SetRotation(_planePitch, (_planeRotation * _planeRotationSpeed), _planeRoll);

	// Update Transform
	_planeBody->Update(t);

	// Update Particle Model
	_planeBody->GetParticleModel()->Update(t);

	if (_boxCollisionDectectionOn)
	{
		if (CheckCollision())
			_planeBody->GetParticleModel()->ResolveCollision(_touchedObject->GetParticleModel());
	}
}

void Plane::Draw(ID3D11DeviceContext* _pImmediateContext)
{
	_planeBody->Draw(_pImmediateContext);
}

bool Plane::CheckCollision()
{
	bool returnVal = false;

	XMFLOAT3 aExtents;
	aExtents.x = _planeBody->GetWidth() * 0.5;
	aExtents.y = _planeBody->GetHeight() * 0.5;
	aExtents.z = _planeBody->GetDepth() * 0.5;

	float aX = _planeBody->GetTransform()->GetPosition().x - aExtents.x;
	float aWidth = aX + aExtents.x * 2.0f;
	float aY = _planeBody->GetTransform()->GetPosition().y - aExtents.y;
	float aHeight = aY + aExtents.y * 2.0f;
	float aZ = _planeBody->GetTransform()->GetPosition().z - aExtents.z;
	float aDepth = aZ + aExtents.z * 2.0f;

	for (int i = 0; i < _collidables->size(); i++)
	{
		if (returnVal)
			return true;

		XMFLOAT3 bExtents;
		bExtents.x = _collidables->at(i)->GetWidth() * 0.5;
		bExtents.y = _collidables->at(i)->GetHeight() * 0.5;
		bExtents.z = _collidables->at(i)->GetDepth() * 0.5;

		float bX = _collidables->at(i)->GetTransform()->GetPosition().x - bExtents.x;
		float bWidth = _collidables->at(i)->GetWidth();
		float bY = _collidables->at(i)->GetTransform()->GetPosition().y - bExtents.y;
		float bDepth = _collidables->at(i)->GetWidth();
		float bZ = _collidables->at(i)->GetTransform()->GetPosition().z - bExtents.z;
		float bHeight = _collidables->at(i)->GetWidth();

		if (aX > bX + bWidth)
			returnVal = false;
		else if (aX + aWidth < bX)
			returnVal = false;

		else if (aY > bY + bHeight)
			returnVal = false;
		else if (aY + aHeight < bY)
			returnVal = false;

		else if (aZ > bZ + bDepth)
			returnVal = false;
		else if (aZ + aDepth < bZ)
			returnVal = false;
		else
		{
			_touchedObject = _collidables->at(i);
			returnVal = true;
		}
	}
	return returnVal;
}