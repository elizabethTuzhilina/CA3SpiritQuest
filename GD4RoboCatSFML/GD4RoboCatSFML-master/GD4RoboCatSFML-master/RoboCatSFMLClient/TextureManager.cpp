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

	CacheTexture("platBottom", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/parallax2(exp).png");
	CacheTexture("plat1A", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/plat1/plat1-A.png");
	CacheTexture("plat1B", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/plat1/plat1-B.png");
	CacheTexture("plat2A", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/plat2/Plat2-A.png");
	CacheTexture("plat2B", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/plat2/plat2-B.png");
	CacheTexture("plat2C", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/plat2/plat2-C.png");
	CacheTexture("plat2D", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/plat2/plat2-D.png");
	CacheTexture("plat2E", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/plat2/plat2-E.png");
	CacheTexture("plat2F", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/plat2/plat2-F.png");
	CacheTexture("plat2G", "../Assets/Media/Textures/Pixel-16px Graveyard/parallax/plat2/plat2-G.png");
	

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
