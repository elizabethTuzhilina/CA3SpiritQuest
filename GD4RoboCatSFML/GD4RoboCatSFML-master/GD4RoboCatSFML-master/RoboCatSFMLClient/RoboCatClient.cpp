#include "RoboCatClientPCH.hpp"
#include "iostream"

constexpr float WORLD_WIDTH = 1280.f;
constexpr float WORLD_HEIGHT = 720.f;


RoboCatClient::RoboCatClient() :
	mTimeLocationBecameOutOfSync(0.f),
	mTimeVelocityBecameOutOfSync(0.f)
{
	mSpriteComponent.reset(new PlayerSpriteComponent(this));
	//mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("cat"));


	const string& m_name_display = StringUtils::GetCommandLineArg(2);
	//std::unique_ptr<HUD> name_display;
	//NetworkManagerClient::sInstance->GetPlayerId();
	Vector3 offset(0.3f,0.3f,0.3f);
	//mSpriteComponent->ShowTag(name, this->GetLocation(), Colors::Red);

	RenderManager::sInstance->DrawPlayerTag(m_name_display, this->GetLocation()+offset, Colors::Red);


	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("reaper"));

	
}

void RoboCatClient::HandleDying()
{
	RoboCat::HandleDying();

	//and if we're local, tell the hud so our health goes away!
	if (GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
	{
		HUD::sInstance->SetPlayerHealth(0);
	}
}
bool RoboCatClient::IsOwnedByLocalPlayer() const
{
	return GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId();
}

void RoboCatClient::Update()
{
	if (IsOwnedByLocalPlayer())
	{
		const Move* pendingMove = InputManager::sInstance->GetAndClearPendingMove();


		//NameTags E.I
		//float deltaTime = pendingMove->GetDeltaTime();
		////Vector3 velocity = GetVelocity();
		//Vector3 newPos = GetLocation() + velocity * deltaTime;


		const string& m_name_display = StringUtils::GetCommandLineArg(2);
		RenderManager::sInstance->DrawPlayerTag(m_name_display, GetLocation(), Colors::Red);
		RenderManager::sInstance->GetPlayerTags();

		if (pendingMove)
		{
			float deltaTime = Timing::sInstance.GetDeltaTime();

			ProcessInput(deltaTime, pendingMove->GetInputState());


			
			

	
			SimulateMovement(deltaTime);
		}
	}
	else
	{
		float deltaTime = Timing::sInstance.GetDeltaTime();
		SimulateMovement(deltaTime);

		if (RoboMath::Is2DVectorEqual(GetVelocity(), Vector3::Zero))
		{
			mTimeLocationBecameOutOfSync = 0.f;
		}
	}
}
void RoboCatClient::SimulateMovement(float inDeltaTime)
{
	Vector3 velocity = GetVelocity();
	Vector3 position = GetLocation();
	const float radius = GetCollisionRadius();
	mIsGrounded = false;

	// Substep for tunneling prevention
	const float step = 1.f / 120.f; // simulate at 120Hz resolution
	float timeRemaining = inDeltaTime;

	while (timeRemaining > 0.f)
	{
		float dt = std::min(step, timeRemaining);
		timeRemaining -= dt;

		// === X Axis Movement ===
		position.mX += velocity.mX * dt;
		sf::FloatRect boundsX(position.mX - radius, position.mY - radius, radius * 2.f, radius * 2.f);
		for (const auto& platform : RenderManager::sInstance->GetPlatformColliders())
		{
			if (boundsX.intersects(platform))
			{
				if (velocity.mX > 0.f)
					position.mX = platform.left - radius;
				else if (velocity.mX < 0.f)
					position.mX = platform.left + platform.width + radius;

				velocity.mX = 0.f;
				break;
			}
		}

		// === Y Axis Movement ===
		position.mY += velocity.mY * dt;
		sf::FloatRect boundsY(position.mX - radius, position.mY - radius, radius * 2.f, radius * 2.f);
		for (const auto& platform : RenderManager::sInstance->GetPlatformColliders())
		{
			if (boundsY.intersects(platform))
			{
				if (velocity.mY > 0.f)
				{
					position.mY = platform.top - radius;
					mIsGrounded = true;
				}
				else
				{
					position.mY = platform.top + platform.height + radius;
				}

				velocity.mY = 0.f;
				break;
			}
		}
	}


	// Clamp to screen edges
	position.mX = std::max(radius, std::min(position.mX, WORLD_WIDTH - radius));

	position.mY = std::max(radius, std::min(position.mY, WORLD_HEIGHT - radius));

	SetLocation(position);
	SetVelocity(velocity);
}


void RoboCatClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	uint32_t readState = 0;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		uint32_t playerId;
		inInputStream.Read(playerId);
		SetPlayerId(playerId);
		readState |= ECRS_PlayerId;
	}

	
		
	

	float oldRotation = GetRotation();
	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();

	float replicatedRotation;
	Vector3 replicatedLocation;
	Vector3 replicatedVelocity;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		inInputStream.Read(replicatedVelocity.mX);
		inInputStream.Read(replicatedVelocity.mY);

		SetVelocity(replicatedVelocity);

		inInputStream.Read(replicatedLocation.mX);
		inInputStream.Read(replicatedLocation.mY);

		SetLocation(replicatedLocation);

		inInputStream.Read(replicatedRotation);
		SetRotation(replicatedRotation);

		const string& m_name_display = StringUtils::GetCommandLineArg(2);
		//inInputStream.Read(m_name_display);
		RenderManager::sInstance->DrawPlayerTag(m_name_display, this->GetLocation(), Colors::Red);
		//SetPlayerId(playerId);
		readState |= ECRS_Tag;

		readState |= ECRS_Pose;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		inInputStream.Read(stateBit);
		mThrustDir = stateBit ? 1.f : -1.f;
	}
	else
	{
		mThrustDir = 0.f;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		SetColor(color);
		readState |= ECRS_Color;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		mHealth = 0;
		inInputStream.Read(mHealth, 4);
		readState |= ECRS_Health;
	}

	if (GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
	{
		//did we get health? if so, tell the hud!
		if ((readState & ECRS_Health) != 0)
		{
			HUD::sInstance->SetPlayerHealth(mHealth);
		}

		DoClientSidePredictionAfterReplicationForLocalCat(readState);

		//if this is a create packet, don't interpolate
		if ((readState & ECRS_PlayerId) == 0)
		{
			InterpolateClientSidePrediction(oldRotation, oldLocation, oldVelocity, false);
		}
	}
	else
	{
		DoClientSidePredictionAfterReplicationForRemoteCat(readState);

		//will this smooth us out too? it'll interpolate us just 10% of the way there...
		if ((readState & ECRS_PlayerId) == 0)
		{
			InterpolateClientSidePrediction(oldRotation, oldLocation, oldVelocity, true);
		}

	}
}

void RoboCatClient::DoClientSidePredictionAfterReplicationForLocalCat(uint32_t inReadState)
{
	if ((inReadState & ECRS_Pose) != 0)
	{
		//simulate pose only if we received new pose- might have just gotten thrustDir
		//in which case we don't need to replay moves because we haven't warped backwards

		//all processed moves have been removed, so all that are left are unprocessed moves
		//so we must apply them...
		const MoveList& moveList = InputManager::sInstance->GetMoveList();
		for (const Move& move : moveList)
		{
			float remaining = move.GetDeltaTime();
			ProcessInput(remaining, move.GetInputState());

			float fixedStep = 1.f / 60.f;
			while (remaining > 0.f)
			{
				float step = std::min(remaining, fixedStep);
				SimulateMovement(step);
				remaining -= step;
			}
		}
	}
}


void RoboCatClient::InterpolateClientSidePrediction(float inOldRotation, const Vector3 & inOldLocation, const Vector3 & inOldVelocity, bool inIsForRemoteCat)
{
	if (inOldRotation != GetRotation() && !inIsForRemoteCat)
	{
		LOG("ERROR! Move replay ended with incorrect rotation!", 0);
	}

	float roundTripTime = NetworkManagerClient::sInstance->GetRoundTripTime();

	if (!RoboMath::Is2DVectorEqual(inOldLocation, GetLocation()))
	{
		//LOG( "ERROR! Move replay ended with incorrect location!", 0 );

		//have we been out of sync, or did we just become out of sync?
		float time = Timing::sInstance.GetFrameStartTime();
		if (mTimeLocationBecameOutOfSync == 0.f)
		{
			mTimeLocationBecameOutOfSync = time;
		}

		float durationOutOfSync = time - mTimeLocationBecameOutOfSync;
		if (durationOutOfSync < roundTripTime)
		{
			SetLocation(Lerp(inOldLocation, GetLocation(), inIsForRemoteCat ? (durationOutOfSync / roundTripTime) : 0.1f));
		}
	}
	else
	{
		//we're in sync
		mTimeLocationBecameOutOfSync = 0.f;
	}


	if (!RoboMath::Is2DVectorEqual(inOldVelocity, GetVelocity()))
	{
		//LOG( "ERROR! Move replay ended with incorrect velocity!", 0 );

		//have we been out of sync, or did we just become out of sync?
		float time = Timing::sInstance.GetFrameStartTime();
		if (mTimeVelocityBecameOutOfSync == 0.f)
		{
			mTimeVelocityBecameOutOfSync = time;
		}

		//now interpolate to the correct value...
		float durationOutOfSync = time - mTimeVelocityBecameOutOfSync;
		if (durationOutOfSync < roundTripTime)
		{
			SetVelocity(Lerp(inOldVelocity, GetVelocity(), inIsForRemoteCat ? (durationOutOfSync / roundTripTime) : 0.1f));
		}
		//otherwise, fine...

	}
	else
	{
		//we're in sync
		mTimeVelocityBecameOutOfSync = 0.f;
	}

}


//so what do we want to do here? need to do some kind of interpolation...

void RoboCatClient::DoClientSidePredictionAfterReplicationForRemoteCat(uint32_t inReadState)
{
	if ((inReadState & ECRS_Pose) != 0)
	{

		//simulate movement for an additional RTT
		float rtt = NetworkManagerClient::sInstance->GetRoundTripTime();
		//LOG( "Other cat came in, simulating for an extra %f", rtt );

		//let's break into framerate sized chunks though so that we don't run through walls and do crazy things...
		float deltaTime = 1.f / 30.f;

		while (true)
		{
			if (rtt < deltaTime)
			{
				SimulateMovement(rtt);
				break;
			}
			else
			{
				SimulateMovement(deltaTime);
				rtt -= deltaTime;
			}
		}
	}
}

