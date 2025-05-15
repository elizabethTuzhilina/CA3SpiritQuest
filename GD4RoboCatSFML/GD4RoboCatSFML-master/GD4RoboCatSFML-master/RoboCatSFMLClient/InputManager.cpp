#include "RoboCatClientPCH.hpp"

unique_ptr< InputManager >	InputManager::sInstance;

namespace
{
	float kTimeBetweenInputSamples = 0.03f;
}

void InputManager::StaticInit()
{
	sInstance.reset(new InputManager());
}


namespace
{
	inline void UpdateDesireVariableFromKey(EInputAction inInputAction, bool& ioVariable)
	{
		if (inInputAction == EIA_Pressed)
		{
			ioVariable = true;
		}
		else if (inInputAction == EIA_Released)
		{
			ioVariable = false;
		}
	}

}
void InputManager::HandleInput(EInputAction inInputAction, int inKeyCode)
{
	switch (inKeyCode)
	{
	case sf::Keyboard::W:
		UpdateDesireVariableFromKey(inInputAction, mCurrentState.mMoveUp);
		break;
	case sf::Keyboard::S:
		UpdateDesireVariableFromKey(inInputAction, mCurrentState.mMoveDown);
		break;
	case sf::Keyboard::A:
		UpdateDesireVariableFromKey(inInputAction, mCurrentState.mMoveLeft);
		break;
	case sf::Keyboard::D:
		UpdateDesireVariableFromKey(inInputAction, mCurrentState.mMoveRight);
		break;
	case sf::Keyboard::K:
		UpdateDesireVariableFromKey(inInputAction, mCurrentState.mIsShooting);
		break;
	case sf::Keyboard::Add:
	case sf::Keyboard::Equal:
	{
		float latency = NetworkManagerClient::sInstance->GetSimulatedLatency();
		latency = std::min(latency + 0.1f, 0.5f);
		NetworkManagerClient::sInstance->SetSimulatedLatency(latency);
		break;
	}
	case sf::Keyboard::Subtract:
	{
		float latency = NetworkManagerClient::sInstance->GetSimulatedLatency();
		latency = std::max(latency - 0.1f, 0.0f);
		NetworkManagerClient::sInstance->SetSimulatedLatency(latency);
		break;
	}
	}
}


InputManager::InputManager() :
	mNextTimeToSampleInput(0.f),
	mPendingMove(nullptr)
{

}

const Move& InputManager::SampleInputAsMove()
{
	return mMoveList.AddMove(GetState(), Timing::sInstance.GetFrameStartTime());
}

bool InputManager::IsTimeToSampleInput()
{
	float time = Timing::sInstance.GetFrameStartTime();
	if (time > mNextTimeToSampleInput)
	{
		mNextTimeToSampleInput = mNextTimeToSampleInput + kTimeBetweenInputSamples;
		return true;
	}

	return false;
}

void InputManager::Update()
{
	if (IsTimeToSampleInput())
	{
		mPendingMove = &SampleInputAsMove();
	}
}
