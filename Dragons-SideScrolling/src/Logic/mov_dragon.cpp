#include "mov_dragon.h"

#include <ctime>
#include <iostream>
#include <cmath>

#include "raylib.h"

#include "screen/Gameplay/gameplay.h"

namespace Game
{
	namespace Logic_dragon
	{
		void mov_dragon()
		{
			using namespace Gameplay;
			static float delta_y;
			static float delta_x;
			static float position_y=0.0f;
			static int maxPosition_y = 900;
			static int position_x = 1000;
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				if (position_y > 0)
				{
					position_y = -100;

				}
			}
			else
			{
				
				if (position_y < maxPosition_y)
				{
					position_y += 1;
				}
			}
			
			delta_y = player.destRec.y - position_y;
			delta_x = player.destRec.x - position_x;
			player.rotation = (atan2(delta_y, delta_x)*RAD2DEG)-90;
			player.speed.x = sin(player.rotation*DEG2RAD);
			player.speed.y = cos(player.rotation*DEG2RAD)*5;
			player.acceleration = 200;
			player.position.y -= (player.speed.y*player.acceleration)* GetFrameTime();
		}
	}
}