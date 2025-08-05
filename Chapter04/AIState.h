#pragma once

#include <SDL3/SDL_log.h>

class AIComponent;

class AIState
{
public:
	AIState(AIComponent* const pOwner)
		: mpOwner(pOwner)
	{

	}

public:
	virtual ~AIState() = default;

	virtual void Update(const float deltTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual const char* GetName() const = 0;

protected:
	AIComponent* mpOwner;
};

class AIPatrol final : public AIState
{
public:
	AIPatrol(AIComponent* const pOwner)
		: AIState(pOwner)
	{

	}

	const char* GetName() const override
	{
		return "Patrol";
	}

public:
	virtual ~AIPatrol() = default;

	void Update(const float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;
};

class AIDeath final : public AIState
{
public:
	AIDeath(AIComponent* const pOwner)
		: AIState(pOwner)
	{

	}

	const char* GetName() const override
	{
		return "Death";
	}

public:
	void Update(const float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;
};

class AIAttack final : public AIState
{
public:
	AIAttack(AIComponent* const pOwner)
		: AIState(pOwner)
	{

	}

	const char* GetName() const override
	{
		return "Attack";
	}

public:
	void Update(const float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;
};
