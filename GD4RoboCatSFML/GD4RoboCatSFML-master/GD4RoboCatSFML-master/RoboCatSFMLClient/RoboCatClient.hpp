class RoboCatClient : public RoboCat
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new RoboCatClient()); }

	virtual void Update();
	virtual void	HandleDying() override;

	virtual void	Read(InputMemoryBitStream& inInputStream) override;

	void DoClientSidePredictionAfterReplicationForLocalCat(uint32_t inReadState);
	void DoClientSidePredictionAfterReplicationForRemoteCat(uint32_t inReadState);
	// Override to check if this RoboCat belongs to the local player
	virtual bool IsOwnedByLocalPlayer() const override;
	virtual void SimulateMovement(float inDeltaTime) override;

protected:
	RoboCatClient();


private:
	void InterpolateClientSidePrediction(float inOldRotation, const Vector3& inOldLocation, const Vector3& inOldVelocity, bool inIsForRemoteCat);
	float				mTimeLocationBecameOutOfSync;
	float				mTimeVelocityBecameOutOfSync;
	std::unique_ptr<string>*				 m_name_display;
	bool mIsGrounded = false; 


	SpriteComponentPtr	mSpriteComponent;
};

