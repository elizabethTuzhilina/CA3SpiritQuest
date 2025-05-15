#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

typedef std::shared_ptr<sf::Texture> TexturePtr;
typedef std::shared_ptr<sf::Font> FontPtr;

class GameObject; // Forward declaration

class SpriteComponent
{
public:
    SpriteComponent(GameObject* inGameObject);
    virtual ~SpriteComponent();

    void SetTexture(TexturePtr inTexture);
    void ShowTag(const string& inStr, const Vector3& origin, const Vector3& inColor);
    virtual sf::Sprite& GetSprite();

    //void UpdateAnimation(float deltaTime);
    //void SetAnimationParams(int frameWidth, int frameHeight, int frameCount, float frameTime);

protected:
    sf::Sprite m_sprite;
    GameObject* mGameObject;
    string m_nametag;

   // int mFrameWidth = 64;
   // int mFrameHeight = 64;
   // int mFrameCount = 1;
   // int mCurrentFrame = 0;
   // float mFrameTime = 0.1f;
   // float mElapsedTime = 0.f;
};

typedef std::shared_ptr<SpriteComponent> SpriteComponentPtr;
