#include"credits.h"

#include "raylib.h"

#include "screen\Gameplay\gameplay.h"
#include "Game\game.h"
namespace Game
{
	namespace initCredits
	{
		Texture2D fond;
		Texture2D menu;
		Texture2D negativeMenu;
		Rectangle recMenu;
		static int positionY = screenHeight- screenHeight/100*60;
		static bool menuButtonAnimationOn;
		static bool exitButtonAnimationOn;
		static bool firstInit = true;
		
		static int fontSize = 20;
		void initCredits() 
		{
			if (firstInit)
			{
				firstInit = false;
				menu = LoadTexture("res/boton_menu.png");
				negativeMenu = LoadTexture("res/boton_menu2.png");
				fond = LoadTexture("res/menu.png");
				recMenu = { (float)halfScreenWidth - menu.width / 2,(float)screenHeight- screenHeight / 100 *43 ,(float)menu.width,(float)menu.height };
			}
		}
		void UpdateCredits()
		{
			initCredits();
			if (CheckCollisionPointRec(GetMousePosition(), recMenu))
			{
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					screen = MENU;
				}
				menuButtonAnimationOn = false;
			}
			else
			{
				menuButtonAnimationOn = true;
			}
		}
		void DrawCredits() 
		{
			DrawTexture(fond, 0, 0, WHITE);
			if (menuButtonAnimationOn)
			{
				DrawTexture(menu, halfScreenWidth - menu.width / 2, screenHeight - screenHeight / 100 * 43, WHITE);
			}
			else
			{
				DrawTexture(negativeMenu, halfScreenWidth  - negativeMenu.width / 2, screenHeight - screenHeight / 100 * 43, WHITE);
			}
			DrawText("Programmer: Cusumano Cristian", halfScreenWidth - (MeasureText("Programmer: Cusumano Cristian", fontSize) / 2), positionY , fontSize, GOLD);
			DrawText("Art, Music and Efects By: Cusumano Cristian", halfScreenWidth - (MeasureText("Music and Efects By: Cusumano Cristian", fontSize) / 2), positionY + fontSize*2 + 5, fontSize, GOLD);
			DrawText("Tools: Raylib - BoscaCeoil - MusicMaker - Photoshop",halfScreenWidth - (MeasureText("Tools: Raylib - BoscaCeoil - MusicMaker - Photoshop", fontSize) / 2), positionY + fontSize * 4 + 5, fontSize, GOLD);
			DrawText("v0.2", halfScreenWidth - (MeasureText("v0.1", fontSize) / 2), positionY + fontSize * 13 + 5, fontSize, GOLD);
		}
	}
}