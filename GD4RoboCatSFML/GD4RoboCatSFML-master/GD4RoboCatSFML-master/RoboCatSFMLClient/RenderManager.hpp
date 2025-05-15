//I take care of rendering things!

class RenderManager
{
public:

	static void StaticInit();
	static std::unique_ptr< RenderManager >	sInstance;

	void Render();
	void RenderComponents();

	//vert inefficient method of tracking scene graph...
	void AddComponent(SpriteComponent* inComponent);
	void RemoveComponent(SpriteComponent* inComponent);
	int	 GetComponentIndex(SpriteComponent* inComponent) const;
	void AddPlatform(const std::string& textureName, const sf::Vector2f& position);
	void DrawPlayerTag(const string& inStr, const Vector3& origin, const Vector3& inColor);
	void InitializeStaticPlatforms();


	const std::vector<sf::FloatRect>& GetPlatformColliders() const { return mPlatformColliders; }
	const std::vector<sf::Text>& GetPlayerTags() const { return mNameTags; }
private:

	RenderManager();

	//this can't be only place that holds on to component- it has to live inside a GameObject in the world
	vector< SpriteComponent* >		mComponents;

	sf::Sprite mMoonSprite;
	sf::Sprite mCloud1Sprite;
	sf::Sprite mCloud2Sprite;
	sf::Sprite mBackgroundSprite;

	std::vector<sf::Sprite> mPlatformSprites; // platform sprites
	std::vector<sf::FloatRect> mPlatformColliders;
	std::vector<sf::Text> mNameTags;
	

	float mCloud1Offset = 0.f;
	float mCloud2Offset = 0.f;
	sf::View view;


};


