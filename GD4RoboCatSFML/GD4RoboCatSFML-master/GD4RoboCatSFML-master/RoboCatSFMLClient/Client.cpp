#include "RoboCatClientPCH.hpp"

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

	//Platforms for world cached in Texture Manager
	const string& m_name_display = StringUtils::GetCommandLineArg(2);
	RenderManager::sInstance->DrawPlayerTag(m_name_display, Vector3(450.f, 600.f, 400.f), Colors::Red);

	RenderManager::sInstance->AddPlatform("plat2D", sf::Vector2f(0.f, 690.f));     // left
	RenderManager::sInstance->AddPlatform("plat2G", sf::Vector2f(215.f, 700.f));   // center
	RenderManager::sInstance->AddPlatform("plat2D", sf::Vector2f(430.f, 690.f));   // right 1
	RenderManager::sInstance->AddPlatform("plat2F", sf::Vector2f(645.f, 690.f));   // right 2
	RenderManager::sInstance->AddPlatform("plat2D", sf::Vector2f(970.f, 690.f));   // far right 
	RenderManager::sInstance->AddPlatform("plat1A", sf::Vector2f(1150.f, 695.f));


	// Level 0 (bottom floating platforms)
	RenderManager::sInstance->AddPlatform("plat2D", sf::Vector2f(250.f, 550.f));
	RenderManager::sInstance->AddPlatform("plat2A", sf::Vector2f(700.f, 550.f));
	RenderManager::sInstance->AddPlatform("plat1A", sf::Vector2f(1000.f, 605.f));
	// Level 1 
	RenderManager::sInstance->AddPlatform("plat1A", sf::Vector2f(100.f, 450.f));
	RenderManager::sInstance->AddPlatform("plat1B", sf::Vector2f(500.f, 450.f));

	// Level 2
	RenderManager::sInstance->AddPlatform("plat2A", sf::Vector2f(250.f, 350.f));
	RenderManager::sInstance->AddPlatform("plat2B", sf::Vector2f(700.f, 350.f));

	// Level 3
	RenderManager::sInstance->AddPlatform("plat2C", sf::Vector2f(150.f, 250.f));
	RenderManager::sInstance->AddPlatform("plat2D", sf::Vector2f(600.f, 250.f));

	// Level 4
	RenderManager::sInstance->AddPlatform("plat2E", sf::Vector2f(300.f, 150.f));
	RenderManager::sInstance->AddPlatform("plat2F", sf::Vector2f(800.f, 150.f));
	RenderManager::sInstance->AddPlatform("plat2B", sf::Vector2f(100.f, 110.f));

	// Top small platform
	RenderManager::sInstance->AddPlatform("plat2G", sf::Vector2f(400.f, 50.f));
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


