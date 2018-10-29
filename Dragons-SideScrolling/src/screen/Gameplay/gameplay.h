#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "raylib.h"

namespace Game
{
	namespace Gameplay
	{
		extern bool firstInit;
		extern bool left;
		extern bool right;
		void initGame();
		void updateGame();
		void DrawGame();
		struct Player {
			Texture2D player_texture;
			Vector2 position;
			Vector2 speed;
			float acceleration;
			float rotation;
			Vector3 collider;
			Color color;
			Rectangle sourceRec;
			Rectangle destRec;
			Rectangle frameRec;
			Vector2 origin;

		};
		extern Player player;
		extern float dragonHeight;
	}
}
#endif