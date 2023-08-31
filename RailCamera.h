#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera {
public:
	void Initialize(Vector3 trans, Vector3 rot);
	void Update();
	WorldTransform& GetWorldTransform() { return worldTransform_; }
	ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};
