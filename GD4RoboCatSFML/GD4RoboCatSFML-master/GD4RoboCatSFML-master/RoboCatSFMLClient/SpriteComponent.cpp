#include "RoboCatClientPCH.hpp"



SpriteComponent::SpriteComponent(GameObject* inGameObject) :
	mGameObject(inGameObject)
{
	//and add yourself to the rendermanager...
	RenderManager::sInstance->AddComponent(this);
}

SpriteComponent::~SpriteComponent()
{
	//don't render me, I'm dead!
	RenderManager::sInstance->RemoveComponent(this);
}

void SpriteComponent::SetTexture(TexturePtr inTexture)
{
	auto tSize = inTexture->getSize();
	//mFrameHeight = static_cast<int>(tSize.y); 
	m_sprite.setTexture(*inTexture);
	m_sprite.setOrigin(tSize.x / 2, tSize.y / 2);
	m_sprite.setScale(sf::Vector2f(1.f * mGameObject->GetScale(), 1.f * mGameObject->GetScale()));
	
	m_sprite.setScale(sf::Vector2f(mGameObject->GetScale(), mGameObject->GetScale()));
}

void SpriteComponent::ShowTag(const string& inStr, const Vector3& origin, const Vector3& inColor)
{
	sf::Text text;
	text.setString(inStr);
	text.setFillColor(sf::Color(inColor.mX, inColor.mY, inColor.mZ, 255));
	text.setCharacterSize(20);
	text.setPosition(origin.mX, origin.mY);
	text.setFont(*FontManager::sInstance->GetFont("smw"));
	WindowManager::sInstance->draw(text);
}
	
//void SpriteComponent::SetAnimationParams(int frameWidth, int frameHeight, int frameCount, float frameTime)
//{
//	mFrameWidth = frameWidth;
//	mFrameHeight = frameHeight;
//	mFrameCount = frameCount;
//	mFrameTime = frameTime;
//
//	// Reset to first frame
//	mCurrentFrame = 0;
//	mElapsedTime = 0.f;
//
//	// Set initial texture rect
//	m_sprite.setTextureRect(sf::IntRect(0, 0, mFrameWidth, mFrameHeight));
//}
//void SpriteComponent::UpdateAnimation(float deltaTime)
//{
//	if (mFrameCount <= 1)
//		return;
//
//	mElapsedTime += deltaTime;
//	if (mElapsedTime >= mFrameTime)
//	{
//		mElapsedTime = 0.f;
//		mCurrentFrame = (mCurrentFrame + 1) % mFrameCount;
//
//		sf::IntRect frameRect(mCurrentFrame * mFrameWidth, 0, mFrameWidth, mFrameHeight);
//		m_sprite.setTextureRect(frameRect);
//	}
//}


sf::Sprite& SpriteComponent::GetSprite()
{
	// Update the sprite based on the game object stuff.
	auto pos = mGameObject->GetLocation();
	auto rot = mGameObject->GetRotation();
	m_sprite.setPosition(pos.mX, pos.mY);
	m_sprite.setRotation(rot);

	return m_sprite;
}

