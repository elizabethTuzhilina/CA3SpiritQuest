#include "RoboCatClientPCH.hpp"

std::unique_ptr< TextureManager >		TextureManager::sInstance;

void TextureManager::StaticInit()
{
	sInstance.reset(new TextureManager());
}
//ET
TextureManager::TextureManager()
{
	CacheTexture("cat", "../Assets/cat.png");
	CacheTexture("mouse", "../Assets/mouse.png");
	CacheTexture("yarn", "../Assets/yarn.png");
	CacheTexture("background", "../Assets/Media/Textures/Clouds 3/1.png");//background
	CacheTexture("moon", "../Assets/Media/Textures/Clouds 3/2.png");//moon
	CacheTexture("clouds1", "../Assets/Media/Textures/Clouds 3/3.png");//cloud 1
	CacheTexture("clouds2", "../Assets/Media/Textures/Clouds 3/4.png");//cloud 2
}

TexturePtr	TextureManager::GetTexture(const string& inTextureName)
{
	return mNameToTextureMap[inTextureName];
}

bool TextureManager::CacheTexture(string inTextureName, const char* inFileName)
{
	TexturePtr newTexture(new sf::Texture());
	newTexture->setRepeated(true); //  allow seamless tiling
	if (!newTexture->loadFromFile(inFileName))
	{
		return false;
	}

	mNameToTextureMap[inTextureName] = newTexture;

	return true;

}
