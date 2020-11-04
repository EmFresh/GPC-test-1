#include "EmConsole.h"
#include "keyinput.h"
#include "Utilities.h"

using namespace std;
void fpsLimiter(float limit)
{
	static bool enter = false;
	static clock_t frameStart;


	//way 1: 
	if(enter)
		if(limit > 0)
			while((CLOCKS_PER_SEC / limit) > (clock() - frameStart));

	////way 2: puts the thread to sleep 
	//if(enter)
	//	if(limit > 0)
	//		sleep((CLOCKS_PER_SEC / limit) - (clock() - frameStart));

	frameStart = clock();

	enter = true;
}

float& calculateFPS()
{
	static const int SAMPLE_SIZE = 5;
	static float frameTimes[SAMPLE_SIZE];
	static ushort count=0;
	static float fps = 0;
	
	static uint timer;
	frameTimes[count++] = 1.0f / ((float(clock() - timer) / CLOCKS_PER_SEC));
	if(count == SAMPLE_SIZE)
	{
		count = 0;
		fps = 0;
		for(auto& a : frameTimes)
			fps += a;
		fps /= SAMPLE_SIZE;
	}

	timer = clock();
	return fps;
}

void main()
{
	EmConsole::init();

	Animation playerAni(SpriteSheet("Player.txt"));
	Sprite box(
		vector<wstring>
	{	L"_________",
		L"|       |",
		L"|       |",
		L"|       |",
		L"|_______|",

	});
	playerAni.setSpeed(5);
	bool jump = false;
	float
		speed = 50, jumpFactor = 50,
		gravity = jumpFactor * .01f,
		jumpTim = 0;

	int ground = 20;
	static Sprite& player = playerAni.getSprite();
	player.setColour(Colour::FG_RED);
	player.setPosition({55,(float)ground - playerAni.getSprite().getHeight()});
	box.setPosition({40, (float)ground - box.getHeight() - 6});

	clock_t t = clock();

	while(true)
	{
		fpsLimiter(70);
		clock_t dt = clock() - t;
		float dtf = (float)dt / CLOCKS_PER_SEC;
		t = clock();

		playerAni.setStop(false);



		if(KeyInput::press(VK_LEFT) || KeyInput::press(VK_RIGHT))
		{
			if(KeyInput::press(VK_LEFT))
				player.setX(player.getX() - speed * dtf);
			if(KeyInput::press(VK_RIGHT))
				player.setX(player.getX() + speed * dtf);

			playerAni.update();
			player = playerAni.getSprite();

			if(player.boxCollision(box))
				player.setX(
				float(player.getCenter().x < box.getCenter().x ?
				box.getX() - player.getWidth() :
				box.getX() + box.getWidth())
				);
		}
		else
			playerAni.setStop(true);


		if(!jump)
			if(KeyInput::press(VK_SPACE))
				jump = true;

		if(jump)
		{
			player.setY(player.getY() - (jumpFactor * dtf - (gravity * (jumpTim += dtf))));

			//ground
			if(player.getY() >= (float)ground - playerAni.getSprite().getHeight())
			{
				player.setY((float)ground - playerAni.getSprite().getHeight());
				if(KeyInput::release(VK_SPACE))
					jumpTim = 0,
					jump = false;
			}

			if(player.boxCollision(box))
				player.setY(
				float(player.getCenter().y < box.getCenter().y ?
				box.getPosition().y - player.getHeight() :
				box.getPosition().y + box.getHeight())
				);
				//will this work?
		}

		EmConsole::toConsoleBuffer(playerAni.getSprite());
		EmConsole::toConsoleBuffer(box);


		wstring fps(to_wstring(calculateFPS()));
		EmConsole::toConsoleBuffer(fps, 0, 0);

		EmConsole::drawConsole();
	}


}