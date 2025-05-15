#include "RoboCatClientPCH.hpp"
#include <SFML/Audio.hpp>
#include <iostream>
static sf::Music bgMusic;
bool Client::StaticInit()
{
	// Create the Client pointer first because it initializes SDL
	Client* client = new Client();
	InputManager::StaticInit();

	WindowManager::StaticInit();
	FontManager::StaticInit();
	TextureManager::StaticInit();
	RenderManager::StaticInit();
	HUD::StaticInit();

	// Load Menu Image 
	if (!client->mMenuTexture.loadFromFile("../Assets/Media/Textures/SpiritQuestBackground.png"))
	{
		std::cerr << "Failed to load menu image!\n";
	}
	else
	{
		client->mMenuSprite.setTexture(client->mMenuTexture);

		sf::Vector2u windowSize = WindowManager::sInstance->getSize();
		sf::Vector2u textureSize = client->mMenuTexture.getSize();

		client->mMenuSprite.setScale(
			float(windowSize.x) / textureSize.x,
			float(windowSize.y) / textureSize.y
		);
		client->mMenuSprite.setPosition(0.f, 0.f);
	}

	client->mMenuStartTime = Timing::sInstance.GetTimef(); // start now

	// Load intro texture
	if (!client->mIntroTexture.loadFromFile("../Assets/Media/Textures/Loading.png"))
	{
		std::cerr << "Failed to load intro image!\n";
	}
	else
	{
		client->mIntroSprite.setTexture(client->mIntroTexture);

		sf::Vector2u windowSize = WindowManager::sInstance->getSize();  // ?? Now safe
		sf::Vector2u textureSize = client->mIntroTexture.getSize();

		client->mIntroSprite.setScale(
			float(windowSize.x) / textureSize.x,
			float(windowSize.y) / textureSize.y
		);
		client->mIntroSprite.setPosition(0.f, 0.f);
	}

	client->mIntroStartTime = Timing::sInstance.GetTimef();
	client->mShowingIntro = true;

	//MUSIC ET
	if (bgMusic.openFromFile("../Assets/Media/Music/8beats Light/08-8bit08.ogg"))
	{
		bgMusic.setLoop(true);
		bgMusic.setVolume(15); 
		bgMusic.play();
	}
	else
	{
		std::cerr << "Failed to load background music!\n";
	}

	//NameTags E.I
	const string& m_name_display = StringUtils::GetCommandLineArg(2);
	RenderManager::sInstance->DrawPlayerTag(m_name_display, Vector3(450.f, 600.f, 400.f), Colors::LightBlue);

	//Platforms for world cached in Texture Manager


	RenderManager::sInstance->InitializeStaticPlatforms();


	s_instance.reset(client);

	return true;
}

Client::Client()
{
	mIntroStartTime = Timing::sInstance.GetTimef();
	mShowingIntro = true;

	
	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('MOUS', MouseClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('YARN', YarnClient::StaticCreate);
	
	string destination = StringUtils::GetCommandLineArg(1);
	string name = StringUtils::GetCommandLineArg(2);

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(destination);

	NetworkManagerClient::StaticInit(*serverAddress, name);

	//NetworkManagerClient::sInstance->SetSimulatedLatency(0.0f);
}



void Client::DoFrame()
{
	float currentTime = Timing::sInstance.GetTimef();

	if (mShowingMenu && currentTime - mMenuStartTime < 3.f)
	{
		WindowManager::sInstance->clear();
		WindowManager::sInstance->draw(mMenuSprite);
		WindowManager::sInstance->display();
		return;
	}
	else if (mShowingMenu)
	{
		mShowingMenu = false;
		mIntroStartTime = currentTime; // start intro now
		mShowingIntro = true;
	}

	if (mShowingIntro && currentTime - mIntroStartTime < 8.f)
	{
		WindowManager::sInstance->clear();
		WindowManager::sInstance->draw(mIntroSprite);
		WindowManager::sInstance->display();
		return;
	}
	else if (mShowingIntro)
	{
		mShowingIntro = false; // done showing intro
	}

	InputManager::sInstance->Update();

	Engine::DoFrame();

	NetworkManagerClient::sInstance->ProcessIncomingPackets();

	RenderManager::sInstance->Render();

	NetworkManagerClient::sInstance->SendOutgoingPackets();
}

void Client::HandleEvent(sf::Event& p_event)
{
	switch (p_event.type)
	{
	case sf::Event::KeyPressed:
		InputManager::sInstance->HandleInput(EIA_Pressed, p_event.key.code);
		break;
	case sf::Event::KeyReleased:
		InputManager::sInstance->HandleInput(EIA_Released, p_event.key.code);
		break;
	default:
		break;
	}
}

bool Client::PollEvent(sf::Event& p_event)
{
	return WindowManager::sInstance->pollEvent(p_event);
}


