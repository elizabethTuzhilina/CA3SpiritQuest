#include "RoboCatClientPCH.hpp"
#include "iostream"
//ET
std::unique_ptr< RenderManager >	RenderManager::sInstance;

RenderManager::RenderManager()
{
	// Load background
	TexturePtr bgTex = TextureManager::sInstance->GetTexture("background");
	if (bgTex)
	{
		mBackgroundSprite.setTexture(*bgTex);
		mBackgroundSprite.setScale(
			float(WindowManager::sInstance->getSize().x) / bgTex->getSize().x,
			float(WindowManager::sInstance->getSize().y) / bgTex->getSize().y
		);
	}

	// Load moon
	TexturePtr moonTex = TextureManager::sInstance->GetTexture("moon");
	if (moonTex)
	{
		mMoonSprite.setTexture(*moonTex);
		mMoonSprite.setPosition(900.f, 50.f);
		mMoonSprite.setScale(1.0f, 1.0f);
	}

	// Load cloud 1
	TexturePtr cloud1Tex = TextureManager::sInstance->GetTexture("clouds1");
	if (cloud1Tex)
	{
		cloud1Tex->setRepeated(true);
		mCloud1Sprite.setTexture(*cloud1Tex);
		mCloud1Sprite.setScale(1.f, 1.f);
	}

	// Load cloud 2
	TexturePtr cloud2Tex = TextureManager::sInstance->GetTexture("clouds2");
	if (cloud2Tex)
	{
		cloud2Tex->setRepeated(true);
		mCloud2Sprite.setTexture(*cloud2Tex);
		mCloud2Sprite.setScale(1.f, 1.f);
	}

}




void RenderManager::StaticInit()
{
	sInstance.reset(new RenderManager());
}


void RenderManager::AddComponent(SpriteComponent* inComponent)
{
	mComponents.emplace_back(inComponent);
}

void RenderManager::RemoveComponent(SpriteComponent* inComponent)
{
	int index = GetComponentIndex(inComponent);

	if (index != -1)
	{
		int lastIndex = mComponents.size() - 1;
		if (index != lastIndex)
		{
			mComponents[index] = mComponents[lastIndex];
		}
		mComponents.pop_back();
	}
}
//ET
void RenderManager::AddPlatform(const std::string& textureName, const sf::Vector2f& position)
{
	TexturePtr tex = TextureManager::sInstance->GetTexture(textureName);
	if (!tex)
	{
		std::cout << "[Platform Error] Missing texture: " << textureName << std::endl;
		return;
	}

	sf::Sprite sprite;
	sprite.setTexture(*tex);
	sprite.setPosition(position);
	sprite.setScale(0.4f, 0.4f); // Match visual

	//Store sprite 
	
	mPlatformSprites.push_back(sprite);

	// Calculate collider before storing
	sf::FloatRect fullBounds = sprite.getGlobalBounds();
	//sf::FloatRect collider(
	///	fullBounds.left,
		//fullBounds.top + fullBounds.height / 2.f,
		//fullBounds.width,
		//fullBounds.height / 2.f
	//);
	float colliderPad = 2.f;
	sf::FloatRect collider(
		fullBounds.left,
		fullBounds.top + fullBounds.height / 2.f - colliderPad,
		fullBounds.width,
		fullBounds.height / 2.f + 2 * colliderPad
	);




	// stores collider
	
	mPlatformColliders.push_back(collider);
}

void RenderManager::InitializeStaticPlatforms()
{
	AddPlatform("plat2D", sf::Vector2f(0.f, 690.f));     // left
	AddPlatform("plat2G", sf::Vector2f(215.f, 700.f));   // center
	AddPlatform("plat2D", sf::Vector2f(430.f, 690.f));   // right 1
	AddPlatform("plat2F", sf::Vector2f(645.f, 690.f));   // right 2
	AddPlatform("plat2D", sf::Vector2f(970.f, 690.f));   // far right 
	AddPlatform("plat1A", sf::Vector2f(1150.f, 695.f));

	// Level 0
	AddPlatform("plat2D", sf::Vector2f(250.f, 550.f));
	AddPlatform("plat2A", sf::Vector2f(700.f, 550.f));
	AddPlatform("plat1A", sf::Vector2f(1000.f, 605.f));

	// Level 1
	AddPlatform("plat1A", sf::Vector2f(100.f, 450.f));
	AddPlatform("plat1B", sf::Vector2f(500.f, 450.f));

	// Level 2
	AddPlatform("plat2A", sf::Vector2f(250.f, 350.f));
	AddPlatform("plat2B", sf::Vector2f(700.f, 350.f));

	// Level 3
	AddPlatform("plat2C", sf::Vector2f(150.f, 250.f));
	AddPlatform("plat2D", sf::Vector2f(600.f, 250.f));

	// Level 4
	AddPlatform("plat2E", sf::Vector2f(300.f, 150.f));
	AddPlatform("plat2F", sf::Vector2f(800.f, 150.f));
	AddPlatform("plat2B", sf::Vector2f(100.f, 110.f));

	// Top platform
	AddPlatform("plat2G", sf::Vector2f(400.f, 50.f));
}


int RenderManager::GetComponentIndex(SpriteComponent* inComponent) const
{
	for (int i = 0, c = mComponents.size(); i < c; ++i)
	{
		if (mComponents[i] == inComponent)
		{
			return i;
		}
	}

	return -1;
}


//this part that renders the world is really a camera-
//in a more detailed engine, we'd have a list of cameras, and then render manager would
//render the cameras in order
void RenderManager::RenderComponents()
{
	//Get the logical viewport so we can pass this to the SpriteComponents when it's draw time
	for (SpriteComponent* c : mComponents)
	{	
		WindowManager::sInstance->draw(c->GetSprite());	
	}
}

void RenderManager::Render()
{
	//
	// Clear the back buffer
	//
	//applies a dark hue ?
	// use a neutral clear color or none (if background fully covers)
	WindowManager::sInstance->clear();

	//  Draw background
	WindowManager::sInstance->draw(mBackgroundSprite);

	// Draw  moon
	WindowManager::sInstance->draw(mMoonSprite);

	//  scroll cloud layers
	const float screenHeight = WindowManager::sInstance->getSize().y;
	mCloud1Offset = std::fmod(mCloud1Offset + 0.5f, mCloud1Sprite.getTexture()->getSize().x);
	mCloud2Offset = std::fmod(mCloud2Offset + 0.8f, mCloud2Sprite.getTexture()->getSize().x);

	// Calculate how many sprites we need to fully cover the screen
	float screenWidth = WindowManager::sInstance->getSize().x;
	float cloud1Width = mCloud1Sprite.getTexture()->getSize().x;
	float cloud2Width = mCloud2Sprite.getTexture()->getSize().x;

	int cloud1Count = static_cast<int>(std::ceil(screenWidth / cloud1Width)) + 2;
	int cloud2Count = static_cast<int>(std::ceil(screenWidth / cloud2Width)) + 2;

	// Cloud layer 1 (top)
	for (int i = 0; i < cloud1Count; ++i)
	{
		float x = i * cloud1Width - mCloud1Offset;
		mCloud1Sprite.setPosition(x, 100.f);
		WindowManager::sInstance->draw(mCloud1Sprite);
	}

	// Cloud layer 2 (bottom)
	for (int i = 0; i < cloud2Count; ++i)
	{
		float x = i * cloud2Width - mCloud2Offset;
		mCloud2Sprite.setPosition(x, 400.f);
		WindowManager::sInstance->draw(mCloud2Sprite);
	}
	// Platforms 
	for (const sf::Sprite& platform : mPlatformSprites)
	{
		WindowManager::sInstance->draw(platform);
	}

	// debug red
#ifdef _DEBUG
	for (const auto& rect : mPlatformColliders)
	{
		sf::RectangleShape debugRect;
		debugRect.setPosition(rect.left, rect.top);
		debugRect.setSize({ rect.width, rect.height });
		debugRect.setFillColor(sf::Color::Transparent);
		debugRect.setOutlineColor(sf::Color::Red);
		debugRect.setOutlineThickness(1.f);
		WindowManager::sInstance->draw(debugRect);
	}
	
	for (const auto& go : World::sInstance->GetGameObjects())
	{
		float radius = go->GetCollisionRadius();
		if (radius <= 0.f)
			continue;

		sf::CircleShape debugCircle;
		debugCircle.setRadius(radius);
		debugCircle.setOrigin(radius, radius);
		debugCircle.setPosition(go->GetLocation().mX, go->GetLocation().mY);
		debugCircle.setOutlineColor(sf::Color::Cyan);
		debugCircle.setOutlineThickness(1.f);
		debugCircle.setFillColor(sf::Color::Transparent);

		WindowManager::sInstance->draw(debugCircle);  // Assuming you have access to window
	}
#endif
	

	RenderManager::sInstance->RenderComponents();

	HUD::sInstance->Render();

	//
	// Present our back buffer to our front buffer
	//
	WindowManager::sInstance->display();

}
