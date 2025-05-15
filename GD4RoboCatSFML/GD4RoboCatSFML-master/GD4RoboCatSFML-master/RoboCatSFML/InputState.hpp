class InputState
{
public:
	InputState() 
		: mMoveUp(false), mMoveDown(false), mMoveLeft(false), mMoveRight(false),
		mIsShooting(false)
	{}

	// Directional checks
	bool IsUp() const { return mMoveUp; }
	bool IsDown() const { return mMoveDown; }
	bool IsLeft() const { return mMoveLeft; }
	bool IsRight() const { return mMoveRight; }
	bool IsShooting() const { return mIsShooting; }

	bool Write(OutputMemoryBitStream& inOutputStream) const;
	bool Read(InputMemoryBitStream& inInputStream);

private:
	friend class InputManager;

	bool mMoveUp, mMoveDown;
	bool mMoveLeft, mMoveRight;
	bool  mIsShooting;
};
