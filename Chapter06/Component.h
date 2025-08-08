#pragma once

class Actor;

class Component
{
public:
	Component(Actor* const pOwner, const int updateOrder);
	Component(Actor* const pOwner);
	virtual ~Component();

	virtual void Update(const float deltaTime);

	virtual void ProcessInput(const bool* const pbKeyStates);

	virtual void OnUpdateWorldTransform();

	inline int GetUpdateOrder() const;
protected:
	Actor* mpOwner;

	int mUpdateOrder;
};

inline int Component::GetUpdateOrder() const
{
	return mUpdateOrder;
}