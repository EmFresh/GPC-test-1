#include "EmConsole.h"
#include "keyinput.h"
#include "Utilities.h"

int main()
{
	EmConsole::init();
	Animation playerAni(SpriteSheet("Player.txt"));
	playerAni.setSpeed(.75);
	Coord2D<float> playerPos = {0,10};
	bool jump = false;
	float jumpFactor = .1, gravity = .001;
	int jumpTim = 0;
	while(true)
	{
		playerAni.setStop(false);
		if(KeyInput::press(VK_LEFT) || KeyInput::press(VK_RIGHT))
		{
			if(KeyInput::press(VK_LEFT))
				playerPos += Coord2D<float>(-.05, 0);
			if(KeyInput::press(VK_RIGHT))
				playerPos += Coord2D<float>(.05, 0);

			playerAni.update();
		}
		else
			playerAni.setStop(true);

		if(!jump)
		if(KeyInput::press(VK_SPACE))
			jump = true;
		
		

		if(jump)
		{
			playerPos.y -= jumpFactor - (gravity * jumpTim++);
			if(playerPos.y > 10)
				playerPos.y = 10,
				jumpTim = 0,
				jump=false;
		}

		//fajdlakfjl;fkdjsf;lja
		EmConsole::toConsoleBuffer(playerAni.getSprite(), (int)playerPos.x, (int)playerPos.y);

		EmConsole::drawConsole();
	}




	return 0;
}