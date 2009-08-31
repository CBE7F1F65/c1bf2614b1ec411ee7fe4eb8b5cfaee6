#include "Player.h"
#include "PlayerBullet.h"
#include "SE.h"
#include "Enemy.h"
#include "Chat.h"
#include "Process.h"

void Player::_Shoot0(BYTE level)
{
	switch(level)
	{
	case 1:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.1f);
			pb.push_back(_pb);
		}
		break;
	case 2:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 0.9f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 3, -300, 15, 0.24f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, 300, 15, 0.24f);
			pb.push_back(_pb);
		}
		break;
	case 3:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 0.9f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 3, -300, 15, 0.2f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, 300, 15, 0.2f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(2, 1, -400, 18, 0.25f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 400, 18, 0.25f);
			pb.push_back(_pb);
		}
		break;
	case 4:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.0f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 3, -300, 15, 0.2f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, 300, 15, 0.2f);
			pb.push_back(_pb);
			
			_pb.valueSet(2, 1, -400, 18, 0.25f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 400, 18, 0.25f);
			pb.push_back(_pb);
		}
		break;
	case 5:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.2f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 3, -300, 15, 0.25f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, 300, 15, 0.25f);
			pb.push_back(_pb);

			_pb.valueSet(2, 1, -400, 18, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 400, 18, 0.3f);
			pb.push_back(_pb);
		}
		break;
	default:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.2f);
			pb.push_back(_pb);

			_pb.valueSet(0, 3, -300, 15, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, 300, 15, 0.3f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(2, 1, -400, 18, 0.4f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 400, 18, 0.4f);
			pb.push_back(_pb);
		}
		break;
	}
}

void Player::_Shoot1(BYTE level)
{
	switch(level)
	{
	case 1:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 15, 1.4f);
			pb.push_back(_pb);
		}
		break;
	case 2:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 15, 1.0f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(4, 3, -1000, 15, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(4, 4, 1000, 15, 0.45f);
			pb.push_back(_pb);
		}
		break;
	case 3:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 15, 1.0f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(4, 3, -1000, 15, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(4, 4, 1000, 15, 0.45f);
			pb.push_back(_pb);

			_pb.valueSet(6, 1, -3000, 15, 0.2f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, 3000, 15, 0.2f);
			pb.push_back(_pb);
		}
		break;
	case 4:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 15, 1.0f);
			pb.push_back(_pb);

			_pb.valueSet(6, 1, -3000, 15, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, 3000, 15, 0.3f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(4, 3, -1000, 15, 0.4f);
			pb.push_back(_pb);
			_pb.valueSet(4, 4, 1000, 15, 0.4f);
			pb.push_back(_pb);
		}
		break;
	case 5:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 15, 1.0f);
			pb.push_back(_pb);

			_pb.valueSet(6, 1, -3000, 15, 0.5f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, 3000, 15, 0.5f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(4, 3, -1000, 15, 0.5f);
			pb.push_back(_pb);
			_pb.valueSet(4, 4, 1000, 15, 0.5f);
			pb.push_back(_pb);
		}
		break;
	default:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(6, 1, -3000, 15, 0.5f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, 3000, 15, 0.5f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 15, 1.0f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(4, 3, -1000, 15, 0.55f);
			pb.push_back(_pb);
			_pb.valueSet(4, 4, 1000, 15, 0.55f);
			pb.push_back(_pb);
		}
		break;
	}
}

void Player::_Shoot2(BYTE level)
{
	switch(level)
	{
	case 1:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.2f);
			pb.push_back(_pb);
		}
		break;
	case 2:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.2f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(0, 3, rand()%700-300, 15, 0.2f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, rand()%700-300, 15, 0.2f);
			pb.push_back(_pb);
		}
		break;
	case 3:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.2f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(0, 3, rand()%800-300, 15, 0.2f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, rand()%800-500, 15, 0.2f);
			pb.push_back(_pb);
			_pb.valueSet(2, 1, pg[2].angle-9000, 15, 0.3f);
			pb.push_back(_pb);
		}
		break;
	case 4:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.2f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 3, rand()%800-300, 15, 0.25f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, rand()%800-500, 15, 0.25f);
			pb.push_back(_pb);

			_pb.valueSet(2, 1, pg[2].angle-9000, 15, 0.4f);
			pb.push_back(_pb);
		}
		break;
	case 5:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.2f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 3, rand()%800-300, 15, 0.4f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, rand()%800-500, 15, 0.4f);
			pb.push_back(_pb);

			_pb.valueSet(2, 1, pg[2].angle-9000, 15, 0.5f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, pg[3].angle-9000, 15, 0.5f);
			pb.push_back(_pb);
		}
		break;
	default:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, 0, 15, 1.2f);
			pb.push_back(_pb);

			_pb.valueSet(0, 3, rand()%800-300, 15, 0.35f);
			pb.push_back(_pb);
			_pb.valueSet(0, 4, rand()%800-500, 15, 0.35f);
			pb.push_back(_pb);
			_pb.valueSet(2, 1, pg[2].angle-9000, 15, 0.4f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, pg[3].angle-9000, 15, 0.4f);
			pb.push_back(_pb);
		}
		break;
	}
}
void Player::_Shoot3(BYTE level)
{
	switch(level)
	{
	case 1:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 12, 0.85f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(6, 3, 0, 9, 0.4f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 9, 0.4f);
			pb.push_back(_pb);
		}
		break;
	case 2:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 12, 0.8f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(6, 3, 0, 9, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 9, 0.3f);
			pb.push_back(_pb);

			_pb.valueSet(6, 1, 0, 9, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, 0, 9, 0.3f);
			pb.push_back(_pb);
		}
		break;
	case 3:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 12, 0.8f);
			pb.push_back(_pb);

			_pb.valueSet(6, 3, 0, 9, 0.35f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 9, 0.35f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(6, 1, 0, 9, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, 0, 9, 0.3f);
			pb.push_back(_pb);
		}
		break;
	case 4:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 12, 0.85f);
			pb.push_back(_pb);

			_pb.valueSet(6, 3, 0, 9, 0.4f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 9, 0.4f);
			pb.push_back(_pb);

			_pb.valueSet(6, 1, 1500, 9, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, -1500, 9, 0.3f);
			pb.push_back(_pb);
		}
		break;
	case 5:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 12, 1.0f);
			pb.push_back(_pb);

			_pb.valueSet(6, 3, 0, 9, 0.5f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 9, 0.5f);
			pb.push_back(_pb);

			_pb.valueSet(6, 1, 1000, 9, 0.5f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, -1000, 9, 0.5f);
			pb.push_back(_pb);
		}
		break;
	default:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(4, 0, 0, 12, 1.0f);
			pb.push_back(_pb);

			_pb.valueSet(6, 3, 0, 9, 0.6f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 9, 0.6f);
			pb.push_back(_pb);

			_pb.valueSet(6, 1, 1500, 9, 0.6f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, -1500, 9, 0.6f);
			pb.push_back(_pb);
		}
		break;
	}
}
void Player::_Shoot4(BYTE level)
{
	switch(level)
	{
	case 1:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, -500, 15, 0.55f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 500, 15, 0.55f);
			pb.push_back(_pb);
		}
		break;
	case 2:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, -500, 15, 0.5f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 500, 15, 0.5f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(2, 3, -1000, 15, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 1000, 15, 0.3f);
			pb.push_back(_pb);
		}
		break;
	case 3:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, -500, 15, 0.5f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 500, 15, 0.5f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(2, 3, -1000, 15, 0.4f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 1000, 15, 0.4f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(2, 1, -2500, 15, 0.25f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 2500, 15, 0.25f);
			pb.push_back(_pb);
		}
		break;
	case 4:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, -500, 15, 0.6f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 500, 15, 0.6f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(2, 3, -1000, 15, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 1000, 15, 0.45f);
			pb.push_back(_pb);

			_pb.valueSet(2, 1, -2500, 15, 0.35f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 2500, 15, 0.35f);
			pb.push_back(_pb);
		}
		break;
	case 5:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, -500, 15, 0.75f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 500, 15, 0.75f);
			pb.push_back(_pb);

			_pb.valueSet(2, 3, -1000, 15, 0.35f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 1000, 15, 0.35f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(2, 1, -2000, 15, 0.2f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 2000, 15, 0.2f);
			pb.push_back(_pb);
			_pb.valueSet(2, 1, -3000, 15, 0.2f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 3000, 15, 0.2f);
			pb.push_back(_pb);
		}
		break;
	default:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(0, 0, -500, 15, 0.75f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 500, 15, 0.75f);
			pb.push_back(_pb);

			_pb.valueSet(2, 3, -1000, 15, 0.6f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 1000, 15, 0.6f);
			pb.push_back(_pb);

			_pb.valueSet(2, 1, -2000, 15, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 2000, 15, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(2, 1, -3000, 15, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 3000, 15, 0.3f);
			pb.push_back(_pb);
		}
		break;
	}
}
void Player::_Shoot5(BYTE level)
{
	switch(level)
	{
	case 1:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(4, 0, 0, 12, 1.3f);
			pb.push_back(_pb);
		}
		break;
	case 2:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(4, 0, 0, 12, 1.0f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(6, 3, 0, 12, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 12, 0.45f);
			pb.push_back(_pb);
		}
		break;
	case 3:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(4, 0, 0, 12, 1.0f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(6, 3, 0, 12, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 12, 0.45f);
			pb.push_back(_pb);
		}
		break;
	case 4:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(4, 0, 0, 12, 1.0f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(6, 3, 0, 12, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 12, 0.45f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(6, 1, 0, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, 0, 12, 0.3f);
			pb.push_back(_pb);
		}
		break;
	case 5:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(4, 0, 0, 12, 1.0f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(6, 3, 0, 12, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 12, 0.45f);
			pb.push_back(_pb);

			_pb.valueSet(6, 1, 0, 12, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, 0, 12, 0.45f);
			pb.push_back(_pb);
		}
		break;
	default:
		if(!(shoottimer % 6))
		{
			_pb.valueSet(4, 0, 0, 12, 1.2f);

			pb.push_back(_pb);
			_pb.valueSet(6, 3, 0, 12, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(6, 4, 0, 12, 0.45f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 8))
		{
			_pb.valueSet(6, 1, 0, 12, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(6, 2, 0, 12, 0.45f);
			pb.push_back(_pb);
		}
		break;
	}
}
void Player::_Shoot6(BYTE level)
{
	switch(level)
	{
	case 1:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 0, 0, 12, 1.0f);
			pb.push_back(_pb);

			_pb.valueSet(0, 0, -1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, -2400, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 2400, 12, 0.3f);
			pb.push_back(_pb);
		}
		break;
	case 2:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 0, 0, 12, 0.9f);
			pb.push_back(_pb);

			_pb.valueSet(0, 0, -1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, -2400, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 2400, 12, 0.3f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(2, 3, 0, 15, 0.25f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 0, 15, 0.25f);
			pb.push_back(_pb);
		}
		break;
	case 3:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 0, 0, 12, 0.9f);
			pb.push_back(_pb);

			_pb.valueSet(0, 0, -1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, -2400, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 2400, 12, 0.3f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(2, 3, 0, 15, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 0, 15, 0.45f);
			pb.push_back(_pb);
		}
		break;
	case 4:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 0, 0, 12, 1.0f);
			pb.push_back(_pb);

			_pb.valueSet(0, 0, -1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, -2400, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 2400, 12, 0.3f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(2, 3, 0, 15, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 0, 15, 0.45f);
			pb.push_back(_pb);

			_pb.valueSet(2, 1, 0, 15, 0.25f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 0, 15, 0.25f);
			pb.push_back(_pb);
		}
		break;
	case 5:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 0, 0, 12, 1.0f);
			pb.push_back(_pb);

			_pb.valueSet(0, 0, -1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, -2400, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 2400, 12, 0.3f);
			pb.push_back(_pb);

			_pb.valueSet(2, 3, 0, 15, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 0, 15, 0.45f);
			pb.push_back(_pb);
		}
		if(!(shoottimer % 12))
		{
			_pb.valueSet(2, 1, 0, 15, 0.45f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 0, 15, 0.45f);
			pb.push_back(_pb);
		}
		break;
	default:
		if(!(shoottimer % 8))
		{
			_pb.valueSet(0, 0, 0, 12, 1.0f);
			pb.push_back(_pb);

			_pb.valueSet(0, 0, -1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 1200, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, -2400, 12, 0.3f);
			pb.push_back(_pb);
			_pb.valueSet(0, 0, 2400, 12, 0.3f);
			pb.push_back(_pb);

			_pb.valueSet(2, 3, 0, 15, 0.6f);
			pb.push_back(_pb);
			_pb.valueSet(2, 4, 0, 15, 0.6f);
			pb.push_back(_pb);

			_pb.valueSet(2, 1, 0, 15, 0.5f);
			pb.push_back(_pb);
			_pb.valueSet(2, 2, 0, 15, 0.5f);
			pb.push_back(_pb);
		}
		break;
	}
}
void Player::_Shoot7(BYTE level)
{
	BYTE g[3];
	if(mp.state != STATE_PAUSE)
	{
		for(int i=0;i<3;i++)
		{
			if(PlayerBullet::beams < 40)
				g[i] = (BYTE)(PlayerBullet::beams * 6.4f);
			else if(PlayerBullet::beams < 120)
				g[i] = 0xff;
			else if(PlayerBullet::beams < 160)
				g[i] = (BYTE)((160 - PlayerBullet::beams) * 6.4f - 1);
			else
				g[i] = 0;
			if(mp.state == STATE_START)
			{
				PlayerBullet::beams += 80;
				PlayerBullet::beams %= 240;
			}
		}
		PlayerBullet::beams++;
		PlayerBullet::bcol0 = g[1]<<16|g[0]<<8|g[2];
		PlayerBullet::bcol1 = g[0]<<16|g[2]<<8|g[1];
		PlayerBullet::bcol2 = g[2]<<16|g[1]<<8|g[0];
		PlayerBullet::bcol3 = g[1]<<16|g[2]<<8|g[0];
	
		PlayerBullet::spPlayerBullet[6]->quad.v[0].tx-=Player::p.nPower/10000.0f+0.001f;
		PlayerBullet::spPlayerBullet[6]->quad.v[1].tx-=Player::p.nPower/10000.0f+0.001f;
		PlayerBullet::spPlayerBullet[6]->quad.v[2].tx-=Player::p.nPower/10000.0f+0.001f;
		PlayerBullet::spPlayerBullet[6]->quad.v[3].tx-=Player::p.nPower/10000.0f+0.001f;
		PlayerBullet::spPlayerBullet[6]->SetHotSpot(0, 16);
	}

	int i = PlayerBullet::locked;
	if(i == 0xffff)
		i = 0;
	for(;i<ENEMYMAX;i++)
	{
		if(en[i].able && en[i].y < y &&
			fabsf(en[i].x - (x+Player::lastx[11])*0.5f) < 27.2f + fabsf(x-Player::lastx[11])*0.5f + Player::p.nPower/10.0f
			)
		{
			en[i].life -= (nPower / 400.0f + 0.15f) * (1 - en[i].defrate)/* * (Player::p.bCircle ? PBENEMY_COSTSCALE : 1.0f)*/;
			en[i].damage = true;
		}
	}
	if(shoottimer % 8 == 0)
		SE::push(SE_PLAYER_SHOOT_1+ID, x);
}
bool Player::Shoot()
{
	if(flag & PLAYER_SHOT || Chat::chatting)
	{
		shoottimer = 0;
		return true;
	}
//	int t = pb.size;
	BYTE level = 0;
	switch(nPower/8)
	{
	case 0:
		level = 1;
		break;
	case 1:
	case 2:
		level = 2;
		break;
	case 3:
	case 4:
	case 5:
		level = 3;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
		level = 4;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		level = 5;
		break;
	default:
		level = 6;
	}
	if(ID == 0)
	{
		_Shoot0(level);
	}
	else if(ID == 1)
	{
		_Shoot1(level);
	}
	else if(ID == 2)
	{
		_Shoot2(level);
	}
	else if(ID == 3)
	{
		_Shoot3(level);
	}
	else if(ID == 4)
	{
		_Shoot4(level);
	}
	else if(ID == 5)
	{
		_Shoot5(level);
	}
	else if(ID == 6)
	{
		_Shoot6(level);
	}
	else if(ID == 7)
	{
		_Shoot7(level);
	}
	/*
	if(t != pb.size)
		SE::push(SE_PLAYER_SHOOT_1+ID, x);
	*/
	shoottimer++;
//
	if(!hge->Input_GetDIKey(KS_FIRE_MP) && shoottimer % 24 == 0)
	{
		shoottimer = 0;
		return true;
	}
	return false;
}
