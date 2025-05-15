class Client : public Engine
{
public:

	static bool StaticInit();
	// At the top of Client class
	sf::Texture mIntroTexture;
	sf::Sprite mIntroSprite;
	float mIntroStartTime = 0.f;
	bool mShowingIntro = true;
	sf::Texture mMenuTexture;
	sf::Sprite mMenuSprite;
	float mMenuStartTime = 0.f;
	bool mShowingMenu = true;

protected:

	Client();

	virtual void	DoFrame() override;
	virtual void	HandleEvent(sf::Event& p_event) override;
	virtual bool	PollEvent(sf::Event& p_event) override;
	

private:

};
