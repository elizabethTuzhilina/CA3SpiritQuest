#include "RoboCatPCH.hpp"

namespace
{
	void WriteBool(OutputMemoryBitStream& inOutputStream, bool value)
	{
		inOutputStream.Write(value);
	}

	void ReadBool(InputMemoryBitStream& inInputStream, bool& outValue)
	{
		inInputStream.Read(outValue);
	}
}

bool InputState::Write(OutputMemoryBitStream& inOutputStream) const
{
	WriteBool(inOutputStream, mMoveUp);
	WriteBool(inOutputStream, mMoveDown);
	WriteBool(inOutputStream, mMoveLeft);
	WriteBool(inOutputStream, mMoveRight);
	WriteBool(inOutputStream, mIsShooting);

	return true;
}

bool InputState::Read(InputMemoryBitStream& inInputStream)
{
	ReadBool(inInputStream, mMoveUp);
	ReadBool(inInputStream, mMoveDown);
	ReadBool(inInputStream, mMoveLeft);
	ReadBool(inInputStream, mMoveRight);
	ReadBool(inInputStream, mIsShooting);

	return true;
}
