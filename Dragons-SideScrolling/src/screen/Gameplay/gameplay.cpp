#include "gameplay.h"


#include <ctime>
#include <iostream>

#include "raylib.h"
#include "Game\game.h"
#include "Logic\mov_dragon.h"
#include <iostream>

using namespace std;

//#define MUSIC_ON
namespace Game
{
	Screen screen = MENU;
	namespace Gameplay
	{

		enum Direction
		{
			Up = 1,
			Right,
			Left,
			Down,
			UpRight,
			UpLeft,
			DownRight,
			DownLeft
		};
		enum Limits {
			LimitTop = 1,
			LimitDown,
			LimitRight,
			LimitLeft
		};
		struct Enemi {
			Texture2D tube_texture;
			float x;
			float y;
			float x_inicial;
			Vector2 position;
			Vector2 speed;
			int radius;
			bool active;
			Direction dir;
			Rectangle sourceRec;
			Rectangle destRec;
			Vector2 origin;
		};

		struct Fondo {
			Texture2D fond;
			float x;
			float y;
			float x_inicial;
			Vector2 position;
			Vector2 speed;
		};
		struct Shoot {
			Vector2 position;
			Vector2 speed;
			float radius;
			float rotation;
			int lifeSpawn;
			bool active;
			Color color;
		};

		Rectangle recExit;
		Texture2D exit;
		Texture2D negativeExit;
		Texture2D menu;
		Texture2D negativeMenu;

		Texture2D fond_black;
		Rectangle recMenu;
		Texture2D boton_pause;
		Texture2D negativePause;
		Rectangle recPause;
		static bool pauseButtonAnimationOn;
		static bool menuButtonAnimationOn;
		static bool exitButtonAnimationOn;
		static void initEnemi(Enemi _enemi[]);
		static void drawEnemi(Enemi _enemi[]);
		static void checkColisionEnemi(Enemi _enemi[]);
		static void movEnemi(Enemi _enemi[]);
		static void instanceThisEnemi(Enemi _enemi[], int thisEnemi);
		static void victory();
		static void defeat();
		// Initialization
		//-------------------------------------------- Statics

		static float time = 0;
		static int checkGame = 1;
		static const int TOTAL_ENEMI = 2;
		static Enemi _enemi[TOTAL_ENEMI];
		static short int INIT_VELOCITY = 400;
		static short int randomMusic = 0;
		static const short int MAX_POINT = 15;
		static const short int HEIGHT_BOX = (screenHeight / 15);
		static const short int WIDTH_BOX = (screenWidth / 25);
		static const short int RADIUS_BALL = (150);
		static const short int SPEED_BALL_INIT = 0;
		static const short int MOVE_BOX = 400;
		static const short int INIT_SCORE = 0;
		static const short int LIMIT_TOP = 10;
		static const short int EXTRA_VELOCITY = 20;
		static const short int PLAYER_MAX_SHOOTS = 10;
		static Sound fxWav;
		static Sound fxWav2;
		static Sound fxWav3;
		static Sound fxWav4;
		static int velocity = INIT_VELOCITY;
		static float dragonHeight;
		static const float PLAYER_BASE_SIZE = 20.0f;
		static const float PLAYER_SPEED = 300.0f;
		static Shoot shoot[PLAYER_MAX_SHOOTS];
		static short int points = 0;
		static bool pause;
		static const int framesSpeed = 8;
		static int framesCounter = 0;
		static int currentFrame = 0;
		static Rectangle enemi;
		static Rectangle enemi2;
		static Rectangle pisoenemi;
		static Rectangle piso2;
		static Fondo fondo[2];
		static Fondo piso[2];
		static Rectangle rec;
		static bool animationDragonOn;
		static bool colect = true;
		Player player;

		//--------------------------------------------

		bool left = false;
		bool right = false;
		bool firstInit = true;
		//--------------------------------------------
		void initGame() {
			for (int i = 0; i < 2; i++)
			{
				if (i == 0)
				{
					fondo[i].x = 0;
					fondo[i].y = 0;
					fondo[i].fond = LoadTexture("res/fondo.png");
					fondo[i].position = Vector2{ 0.0f,0.0f };
					fondo[i].speed = Vector2{ 0, 0 };
				}
				else
				{
					fondo[i].x = fondo[0].fond.width;
					fondo[i].y = 0;
					fondo[i].position = Vector2{ 0.0f,0.0f };
					fondo[i].speed = Vector2{ 0, 0 };
				}
			}
			for (int i = 0; i < 2; i++)
			{
				piso[0].fond = LoadTexture("res/piso.png");
				if (i == 0)
				{

					piso[i].x = 0;
					piso[i].y = screenHeight - piso[0].fond.height;
					piso[i].position = Vector2{ 0.0f,0.0f };
					piso[i].speed = Vector2{ 0, 0 };
				}
				else
				{
					piso[i].x = fondo[0].fond.width;
					piso[i].y = screenHeight - piso[0].fond.height;;
					piso[i].position = Vector2{ 0.0f,0.0f };
					piso[i].speed = Vector2{ 0, 0 };
				}
			}

#ifdef MUSIC_ON
			fxWav = LoadSound("res/Blip_Select.wav");
			fxWav2 = LoadSound("res/Blip_Select2.wav");
			fxWav3 = LoadSound("res/Blip_Select3.wav");
			fxWav4 = LoadSound("res/Blip_Select4.wav");
#endif
			velocity = INIT_VELOCITY;
			//--------------------------------

			dragonHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD) / 2;
			player.player_texture = LoadTexture("res/dragon.png");
			player.position = Vector2{ 200.0f, (float)screenHeight / 3 - dragonHeight / 3 };
			player.speed = Vector2{ 0, 0 };
			player.acceleration = 0;
			player.rotation = 0;
			player.sourceRec = { 0.0f,0.0f,(float)player.player_texture.width / 3,(float)player.player_texture.height };
			player.destRec = { player.position.x,player.position.y,(float)player.player_texture.width / 3,(float)player.player_texture.height };
			player.origin = { (float)player.player_texture.width / 3,(float)player.player_texture.height / 3 };
			player.collider = Vector3{ player.position.x + sin(player.rotation*DEG2RAD)*(dragonHeight / 3 / 2.5f), player.position.y - cos(player.rotation*DEG2RAD)*(dragonHeight / 3 / 2.5f), 12 };
			player.color = LIGHTGRAY;
			player.frameRec = { 0.0f, 0.0f, (float)player.player_texture.width / 3, (float)player.player_texture.height };
			//--------------------------------
			initEnemi(_enemi);
			menu = LoadTexture("res/boton_menu.png");
			negativeMenu = LoadTexture("res/boton_menu2.png");
			exit = LoadTexture("res/return.png");
			negativeExit = LoadTexture("res/return2.png");
			fond_black = LoadTexture("res/fondo_oscuridad.png");
			boton_pause = LoadTexture("res/pause.png");
			negativePause = LoadTexture("res/pause2.png");
			recMenu = { (float)halfScreenWidth - menu.width / 2,(float)screenHeight / 2 ,(float)menu.width,(float)menu.height };
			recExit = { (float)halfScreenWidth - exit.width / 2,(float)screenHeight / 2 + exit.height + 5,(float)exit.width,(float)exit.height };
			recPause = { (float)screenWidth - 100, (float)5,(float)boton_pause.width,(float)boton_pause.height };
			animationDragonOn = false;
		}


		void updateGame()
		{
			if (!pause)
			{
				if (player.position.x >  _enemi[0].position.x && colect)
				{
					points++;
					colect = !colect;
#ifdef MUSIC_ON
					if (music)
					{
						randomMusic = GetRandomValue(1, 4);
						switch (randomMusic) {
						case 1:
							PlaySound(fxWav);
							break;
						case 2:
							PlaySound(fxWav2);
							break;
						case 3:
							PlaySound(fxWav3);
							break;
						case 4:
							PlaySound(fxWav4);
							break;
						}

					}
#endif					
				}
				for (int i = 0; i < 2; i++)
				{

					fondo[i].position = Vector2{ fondo[i].x,fondo[i].y };
					fondo[i].speed = Vector2{ SPEED_BALL_INIT, SPEED_BALL_INIT };
				}
				for (int i = 0; i < 2; i++)
				{
					fondo[i].x -= velocity / 4 * GetFrameTime();
					if (fondo[i].x <= -fondo[0].fond.width)
					{
						fondo[i].x = (int)fondo[0].fond.width;
					}
				}
				for (int i = 0; i < 2; i++)
				{

					piso[i].position = Vector2{ piso[i].x,piso[i].y };
					piso[i].speed = Vector2{ SPEED_BALL_INIT, SPEED_BALL_INIT };
				}
				for (int i = 0; i < 2; i++)
				{
					piso[i].x -= velocity * GetFrameTime();
					if (piso[i].x <= -piso[0].fond.width)
					{
						piso[i].x = (int)piso[0].fond.width;
					}
				}

				//---------------------------------------------- Colision piso
				for (int i = 0; i < TOTAL_ENEMI; i++)
				{
					if (i == 1)
					{
						pisoenemi = { (float)piso[i].x - (float)piso[0].fond.width / 2,(float)piso[i].y - (float)piso[0].fond.height / 2 + 50,(float)piso[0].fond.width,(float)piso[0].fond.height };
					}
					else
					{
						piso2 = { (float)piso[i].x - (float)piso[0].fond.width / 2,(float)piso[i].y - (float)piso[0].fond.height / 2 + 50,(float)piso[0].fond.width,(float)piso[0].fond.height };
					}
				}
				for (int i = 0; i < 2; i++)
				{
					if (CheckCollisionRecs(rec, pisoenemi) || CheckCollisionRecs(rec, piso2))
					{
							defeat();
					}

				}
				for (int i = 0; i < TOTAL_ENEMI; i++)
				{
					if (i == 1)
					{
						enemi = { (float)_enemi[i].x - (float)_enemi[0].tube_texture.width / 2,(float)_enemi[i].y - (float)_enemi[0].tube_texture.height / 2,(float)_enemi[0].tube_texture.width,(float)_enemi[0].tube_texture.height };
					}
					else
					{
						enemi2 = { (float)_enemi[i].x - (float)_enemi[0].tube_texture.width / 2,(float)_enemi[i].y - (float)_enemi[0].tube_texture.height / 2,(float)_enemi[0].tube_texture.width,(float)_enemi[0].tube_texture.height };
					}
				}

				rec = { (float)player.position.x - (float)player.player_texture.width / 3 / 2,(float)player.position.y - (float)player.player_texture.height, (float)player.player_texture.width / 3, (float)player.player_texture.height };

				if (CheckCollisionPointRec(GetMousePosition(), recPause))
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						pause = !pause;
					}
					pauseButtonAnimationOn = false;
				}
				else
				{
					pauseButtonAnimationOn = true;
				}
				victory();
				SetExitKey(0);
				Logic_dragon::mov_dragon();
				if (IsKeyDown(KEY_M))
				{
					pause = !pause;
				}
				//---------------------------------------------------------
				//Meteor Movement
				movEnemi(_enemi);
				//---------------------------------------------------------
				//Bordes pantalla
				for (int i = 0; i < TOTAL_ENEMI; i++)
				{
					if (
						_enemi[i].x <= (0 - RADIUS_BALL * 4))
					{
						instanceThisEnemi(_enemi, i);
						colect =true;
					}

				}
				//--------------------------------------------------------
				checkColisionEnemi(_enemi);
				for (int i = 0; i < TOTAL_ENEMI; i++)
				{
					_enemi[i].position = Vector2{ _enemi[i].x, _enemi[i].y };
				}
				//-------------------------------------------------------
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					if (!animationDragonOn)
					{
						animationDragonOn = true;
						framesCounter += 1;

						if (framesCounter*GetFrameTime() >= (1 / framesSpeed))
						{
							currentFrame++;

							if (currentFrame > 1) currentFrame = 1;

							player.frameRec.x = (float)currentFrame*(float)player.player_texture.width / 3;
						}
					}

				}
				else
				{
					time += 1 * GetFrameTime();
					if (time > 0.5f) {
						time = 0;
						if (framesCounter*GetFrameTime() >= (1 / framesSpeed))
						{
							framesCounter = 0;
							currentFrame = 0;
							player.frameRec.x = (float)currentFrame*(float)player.player_texture.width / 3;
						}
						animationDragonOn = false;
					}
				}
				for (int i = 0; i < TOTAL_ENEMI; i++)
				{
					_enemi[i].destRec = { _enemi[i].x,_enemi[i].y,(float)_enemi[0].tube_texture.width,(float)_enemi[0].tube_texture.height };
				}
				player.destRec = { player.position.x,player.position.y,(float)player.player_texture.width / 3,(float)player.player_texture.height };
			}
			else
			{
				if (CheckCollisionPointRec(GetMousePosition(), recMenu))
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						initEnemi(_enemi);
						points = INIT_SCORE;
						player.position = Vector2{ (float)halfScreenWidth, (float)screenHeight / 2 - dragonHeight / 2 };
						player.rotation = 0;
						pause = !pause;
						screen = MENU;
					}
					menuButtonAnimationOn = false;
				}
				else
				{
					menuButtonAnimationOn = true;
				}
				if (CheckCollisionPointRec(GetMousePosition(), recExit))
				{
					exitButtonAnimationOn = false;

					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						pause = !pause;
					}
				}
				else
				{
					exitButtonAnimationOn = true;
				}
			}
		}
		void DrawGame()
		{

			for (int i = 0; i < 2; i++)
			{
				DrawTexture(fondo[0].fond, fondo[i].position.x, fondo[i].position.y, WHITE);
			}
			//DrawTextureRec(player.player_texture, , player.position, WHITE);
			DrawTexturePro(player.player_texture, player.frameRec, player.destRec, player.origin, player.rotation, WHITE);
			drawEnemi(_enemi);
			for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
			{
				if (shoot[i].active) DrawCircleV(shoot[i].position, shoot[i].radius, BLACK);
			}

			DrawText(FormatText("%01i", points), (halfScreenWidth)-20, screenHeight / 20, 30, LIGHTGRAY);
			for (int i = 0; i < 2; i++)
			{
				DrawTexture(piso[0].fond, piso[i].position.x, piso[i].position.y, WHITE);
			}

			if (pause)
			{
				DrawTexture(fond_black, 0, 0, WHITE);
				if (menuButtonAnimationOn)
				{
					DrawTexture(menu, halfScreenWidth - menu.width / 2, screenHeight / 2, WHITE);
				}
				else
				{
					DrawTexture(negativeMenu, halfScreenWidth - negativeMenu.width / 2, screenHeight / 2, WHITE);
				}
				if (exitButtonAnimationOn)
				{
					DrawTexture(exit, halfScreenWidth - exit.width / 2, screenHeight / 2 + exit.height + 5, WHITE);
				}
				else
				{
					DrawTexture(negativeExit, halfScreenWidth - exit.width / 2, screenHeight / 2 + exit.height + 5, WHITE);
				}
			}
			if (pauseButtonAnimationOn)
			{
				DrawTexture(boton_pause, screenWidth - 100, 5, WHITE);
			}
			else
			{
				DrawTexture(negativePause, screenWidth - 100, 5, WHITE);
			}
		}
		void initEnemi(Enemi _enemi[])
		{
			_enemi[0].tube_texture = LoadTexture("res/tube.png");
			for (int i = 0; i < TOTAL_ENEMI; i++)
			{
				if (i == 0)
				{
					_enemi[i].x_inicial = GetRandomValue(screenWidth + RADIUS_BALL, screenWidth * 2) + RADIUS_BALL;
					if (i == 0)
					{
						_enemi[i].x = _enemi[i].x_inicial;
					}
					else
					{
						_enemi[i].x = _enemi[i].x_inicial + _enemi[0].tube_texture.width;
					}

					_enemi[i].y = screenHeight - RADIUS_BALL + GetRandomValue(100, 600);
					_enemi[i].position = Vector2{ _enemi[i].x, _enemi[i].y };
					_enemi[i].speed = Vector2{ SPEED_BALL_INIT, SPEED_BALL_INIT };
					_enemi[i].radius = RADIUS_BALL;
					_enemi[i].active = true;
					_enemi[i].dir = (Direction)GetRandomValue(1, 8);
					_enemi[i].sourceRec = { 0.0f,0.0f,(float)_enemi[0].tube_texture.width,(float)_enemi[0].tube_texture.height };
					_enemi[i].destRec = { _enemi[i].x,_enemi[i].y,(float)_enemi[0].tube_texture.width,(float)_enemi[0].tube_texture.height };
					_enemi[i].origin = { (float)_enemi[0].tube_texture.width / 2,(float)_enemi[0].tube_texture.height / 2 };
				}
				else
				{
					_enemi[i].x = _enemi[i - 1].x;
					_enemi[i].y = _enemi[0].y - screenHeight - 400;
					_enemi[i].position = Vector2{ _enemi[i].x, _enemi[i].y };
					_enemi[i].speed = Vector2{ SPEED_BALL_INIT, SPEED_BALL_INIT };
					_enemi[i].radius = RADIUS_BALL;
					_enemi[i].active = true;
					_enemi[i].dir = (Direction)GetRandomValue(1, 8);
					_enemi[i].sourceRec = { 0.0f,0.0f,(float)_enemi[0].tube_texture.width,(float)_enemi[0].tube_texture.height };
					_enemi[i].destRec = { _enemi[i].x,_enemi[i].y,(float)_enemi[0].tube_texture.width,(float)_enemi[0].tube_texture.height };
					_enemi[i].origin = { (float)_enemi[0].tube_texture.width / 2,(float)_enemi[0].tube_texture.height / 2 };
				}


			}
		}
		static void drawEnemi(Enemi _enemi[])
		{
			for (int i = 0; i < TOTAL_ENEMI; i++)
			{
				if (i == 0)
				{
					DrawTexturePro(_enemi[0].tube_texture, _enemi[i].sourceRec, _enemi[i].destRec, _enemi[i].origin, 0.0f, WHITE);
					//DrawRectangleRec(enemi2, BLACK);
				}
				else
				{
					DrawTexturePro(_enemi[0].tube_texture, _enemi[i].sourceRec, _enemi[i].destRec, _enemi[i].origin, 180.0f, WHITE);
					//DrawRectangleRec(enemi, BLACK);
				}

			}
		}
		static void checkColisionEnemi(Enemi _enemi[])
		{
			//---------------------------------------------------------
			//Colision
			for (int i = 0; i < TOTAL_ENEMI; i++)
			{
				if (CheckCollisionRecs(rec, enemi) || CheckCollisionRecs(rec, enemi2))
				{
					instanceThisEnemi(_enemi, i);
					defeat();
				}

			}
		}
		static void movEnemi(Enemi _enemi[])
		{
			for (int i = 0; i < TOTAL_ENEMI; i++)
			{
				_enemi[i].x -= velocity * GetFrameTime();
			}
		}
		static void instanceThisEnemi(Enemi _enemi[], int thisTube)
		{


			_enemi[thisTube].x_inicial = GetRandomValue(screenWidth + RADIUS_BALL, screenWidth * 2) + RADIUS_BALL;
			if (thisTube == 0)
			{
				_enemi[thisTube].x = _enemi[thisTube].x_inicial;
				_enemi[thisTube].y = screenHeight - RADIUS_BALL + GetRandomValue(100, 600);
			}
			else
			{
				_enemi[thisTube].x = _enemi[0].x_inicial;
				_enemi[thisTube].y = _enemi[0].y - screenHeight - 300;
				//_enemi[thisTube].x = _enemi[thisTube].x_inicial + _enemi[thisTube - 1].x_inicial/3 + _enemi[thisTube].tube_texture.width;
			}

		}
		static void victory()
		{
			if (points >= MAX_POINT)
			{
				screen = WIN;
				initEnemi(_enemi);
				points = INIT_SCORE;
				player.position = Vector2{ 200.0f, (float)screenHeight / 3 - dragonHeight / 3 };
				player.rotation = 0;
				colect = true;
			}
		}
		static void defeat()
		{
			screen = DEFEAT;
			initEnemi(_enemi);
			points = INIT_SCORE;
			player.position = Vector2{ 200.0f, (float)screenHeight / 3 - dragonHeight / 3 };
			player.rotation = 0;
			colect = true;
		}
	}

}

