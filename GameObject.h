#pragma once

#include <directxmath.h>
#include <d3d11_1.h>
#include <string>
#include <vector>

#include "Appearance.h"
#include "ParticleModel.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Appearance* appearance, Transform* transform, ParticleModel* particleModel);
	~GameObject();

	string GetType()										const { return _type; };

	Appearance* GetAppearance()								const { return _appearance; };
	Transform* GetTransform()								const { return _transform; };
	ParticleModel* GetParticleModel()						{ return _particleModel; };

	void Update(float t);
	void Draw(ID3D11DeviceContext* pImmediateContext);

	// Scene Graph
	void AddChild(GameObject* child)						{ _children.push_back(child); child->GetTransform()->SetParent(_transform); };
	vector < GameObject* > GetChildren()					{ return _children; };

	float GetWidth()										const{ return _width; }
	float GetDepth()										const{ return _depth; }
	float GetHeight()										const{ return _height; }

	void SetWidth(float width)								{ _width = width; }
	void SetDepth(float depth)								{ _depth = depth; }
	void SetHeight(float height)							{ _height = height; }

private:
	string _type;

	Appearance* _appearance;
	Transform* _transform;
	ParticleModel* _particleModel;

	// Children in Scene Graph
	vector < GameObject* > _children;

	float _width = 0.0f;
	float _depth = 0.0f;
	float _height = 0.0f;
};