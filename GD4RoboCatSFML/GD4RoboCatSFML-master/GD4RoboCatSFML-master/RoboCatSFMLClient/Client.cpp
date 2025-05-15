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

	//MUSIC ET
	if (bgMusic.openFromFile("../Assets/Media/Music/8beats Light/08-8bit08.ogg"))
	{
		bgMusic.setLoop(true);
		bgMusic.setVolume(20); 
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


