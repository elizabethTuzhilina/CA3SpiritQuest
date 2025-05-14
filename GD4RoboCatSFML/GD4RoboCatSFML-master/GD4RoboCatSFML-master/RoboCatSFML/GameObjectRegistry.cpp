#include "RoboCatPCH.hpp"
#include "iostream"

std::unique_ptr< GameObjectRegistry >	GameObjectRegistry::sInstance;

void GameObjectRegistry::StaticInit()
{
	sInstance.reset(new GameObjectRegistry());
}

GameObjectRegistry::GameObjectRegistry()
{
}

void GameObjectRegistry::RegisterCreationFunction(uint32_t inFourCCName, GameObjectCreationFunc inCreationFunction)
{
	mNameToGameObjectCreationFunctionMap[inFourCCName] = inCreationFunction;
}

GameObjectPtr GameObjectRegistry::CreateGameObject(uint32_t inFourCCName)
{
	auto it = mNameToGameObjectCreationFunctionMap.find(inFourCCName);
	if (it == mNameToGameObjectCreationFunctionMap.end())
	{
		std::cerr << "[ERROR] Unknown GameObject FourCC: " << inFourCCName << std::endl;
		assert(false && "GameObject creation function not registered!");
		return nullptr;
	}

	GameObjectCreationFunc creationFunc = it->second;

	// SAFETY: Check again before calling
	if (!creationFunc)
	{
		std::cerr << "[ERROR] Creation function is null for FourCC: " << inFourCCName << std::endl;
		assert(false && "Null creation function!");
		return nullptr;
	}

	GameObjectPtr gameObject = creationFunc();
	World::sInstance->AddGameObject(gameObject);

	return gameObject;
}
