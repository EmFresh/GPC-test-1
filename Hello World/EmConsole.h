/*************************************************
**Name     : Emory Wynn							**
**Student# : 100655604							**
**************************************************/

#pragma once
#include <Windows.h>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <map>
#include "Utilities.h"

//#define newstr new char
//typedef unsigned short ushort;

/*Enums*/
/*
summary:
These are coulers that can be used for text coulour manipulation.

* FG_COLOUR - change the text colour
* BG_COLOUR - Change the colour behind each character of text

Usage:
to use these colours in toConsoleBuffer();, use eather  FG_COLOUR or BG_COLOUR for colour manipulation.
to combine colours or set a FG_COLOUR and BG_COLOUR at the same time, use the
"|" operator (i.e. FG_COLOUR | BG_COLOUR ) for more combonations.
*/
enum Colour
{

	FG_BLUE = FOREGROUND_BLUE,
	FG_GREEN = FOREGROUND_GREEN,
	FG_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
	FG_RED = FOREGROUND_RED,
	FG_PURPLE = FOREGROUND_BLUE | FOREGROUND_RED,
	FG_YELLOW = FOREGROUND_GREEN | FOREGROUND_RED,
	FG_WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
	FG_INTENSIFY = FOREGROUND_INTENSITY,
	//FG_UNDERLINE = UNDERLINE,

	BG_BLUE = BACKGROUND_BLUE,
	BG_GREEN = BACKGROUND_GREEN,
	BG_CYAN = BACKGROUND_BLUE | BACKGROUND_GREEN,
	BG_RED = BACKGROUND_RED,
	BG_PURPLE = BACKGROUND_BLUE | BACKGROUND_RED,
	BG_YELLOW = BACKGROUND_GREEN | BACKGROUND_RED,
	BG_WHITE = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,
	BG_INTENSIFY = BACKGROUND_INTENSITY

};

enum MouseButtons
{
	LEFT_CLICK = VK_LBUTTON,
	RIGHT_CLICK = VK_RBUTTON,
	MIDDLE_CLICK = VK_MBUTTON,
	UNKNOWN_CLICK_1 = VK_XBUTTON1,
	UNKNOWN_CLICK_2 = VK_XBUTTON2
};

/*Structs*/
struct Sprite
{
	Sprite() {}

	Sprite(const Sprite& sprite, char* tag = nullptr):
		m_sprite(sprite.m_sprite), m_tag(sprite.m_tag),
		m_width(sprite.m_width), m_height(sprite.m_height),
		m_colour(sprite.m_colour)
	{}
	//Sprite(Sprite&& sprite, char* tag = nullptr):
	//	m_sprite(sprite.m_sprite), m_tag(sprite.m_tag),
	//	m_width(sprite.m_width), m_height(sprite.m_height),
	//	m_colour(sprite.m_colour)
	//{}
	Sprite(const char* file, char* tag = nullptr)
	{
		create(file, tag);
	}
	Sprite(std::vector<std::wstring>* sprite, char* tag = nullptr)
	{
		create(sprite, tag);
	}
	Sprite(std::vector<std::wstring> sprite, char* tag = nullptr)
	{
		create(&sprite, tag);
	}

	/*
	Takes a .txt file and outputs into the Sprite.
	The sprite will be overridden once this is called
	*/
	virtual void create(const char* file, char* tag = nullptr)
	{
		setTag(tag);
		FILE* f;
		fopen_s(&f, file, "r, ccs=UNICODE");

		//std::vector<std::wstring>* tmp = _sprite;
		m_sprite.clear();

		//std::vector<std::wstring>sprite;

		m_height = m_width = 0;

		wchar_t* str = new wchar_t[255];

		while(str = fgetws(str, 255, f),
			  m_sprite.push_back((str == nullptr ? L"" : (str[wcslen(str) - 1] = (str[wcslen(str) - 1] == '\n' ? '\0' : str[wcslen(str) - 1]), str))), str != nullptr)
			m_width = m_width < (ushort)(m_sprite[m_height]).size() ? (ushort)(m_sprite[m_height]).size() : m_width,
			m_height++;

		m_sprite.pop_back();
		if(f)
			fclose(f);
		delete[] str;
	}

	virtual void create(const std::vector<std::wstring>* sprite, char* tag = nullptr)
	{
		setTag(tag);

		m_sprite.clear();
		m_height = (ushort)sprite->size();
		m_sprite = *sprite;
		for(int a = 0; a < m_height; a++)
			m_width = m_width < (ushort)m_sprite[a].size() ? (ushort)m_sprite[a].size() : m_width;
	}

	virtual void toBuffer(ushort x, ushort y);
	virtual void toBufferNS(ushort x, ushort y);

	void render();

	/*

	************
	  ********     *******
		****
		 **

	*/
	bool boxCollision(Sprite s2, COORD p1, COORD p2)
	{
		p1 = {p1.X + m_width / 2,p1.Y + m_height / 2};
		p2 = {p2.X + s2.m_width / 2,p2.Y + s2.m_height / 2};

		if(abs(p1.X - p2.X) <= m_width / 2 + s2.m_width / 2)
			if(abs(p1.Y - p2.Y) <= m_height / 2 + s2.m_height / 2)
				return true;
		return false;
	}

	bool mouseCollision(COORD p1, COORD p2)
	{
		p1 = {p1.X + m_width / 2,p1.Y + m_height / 2};
		p2 = {p2.X ,p2.Y};

		if(abs(p1.X - p2.X) <= m_width / 2)
			if(abs(p1.Y - p2.Y) <= m_height / 2)
				return true;
		return false;
	}

	void setTag(const char* tag)
	{
		m_tag = tag;
	}

	const char* getTag()
	{
		return m_tag;
	}

	void setColour(Colour colour)
	{
		m_colour = colour;
	}

	Colour getColour() { return m_colour; }

	std::wstring* getSprite()
	{
		return m_sprite.data();
	}

	unsigned short getWidth()
	{
		return m_width;
	}

	unsigned short getHeight()
	{
		return m_height;
	}

	void operator=(Sprite sprite)
	{
		m_sprite = (sprite.m_sprite);
		m_tag = (sprite.m_tag);
		m_width = (sprite.m_width);
		m_height = (sprite.m_height);
		m_colour = (sprite.m_colour);
	}

protected:
	std::vector<std::wstring> m_sprite;
	const char* m_tag = nullptr;
	ushort m_width = 0, m_height = 0;
	Colour m_colour = FG_WHITE;
};

struct SpriteSheet
{
	SpriteSheet() {}

	/*
	Creates SpriteSheet from another spritesheet. Creates a new
	Sprite when it finds a line with a lenght of zero
	*/
	SpriteSheet(const SpriteSheet& cpy, const wchar_t* split = L"")
	{
		m_sheet->clear();
		for(auto& a : *cpy.m_sheet)
			m_sheet->push_back(new Sprite(*a));
	}

	/*
	Creates SpriteSheet from a .txt file. Creates a new
	Sprite when it finds a line with a lenght of zero
	*/
	SpriteSheet(const char* file, const wchar_t* split = L"")
	{
		create(file, split);
	}

	~SpriteSheet()
	{
		for(auto& a : *m_sheet)
			delete a;
		delete m_sheet;
	}

	/*
	Creates SpriteSheet from a .txt file. Creates a new
	Sprite when it finds a line with a lenght of zero
	*/
	void create(const char* file, const wchar_t* split = L"")
	{
		FILE* f;

		wchar_t* str2 = new wchar_t[255];

		std::vector<std::wstring> sprite;
		unsigned short width = 0, height = 0;
		bool seg = 0;
		std::wstring str;

		fopen_s(&f, file, "r, ccs=UNICODE");

		while(str2 = fgetws(str2, 255, f),
			  str = (str2 == nullptr ? L"" : (str2[wcslen(str2) - 1] = (str2[wcslen(str2) - 1] == '\n' ? '\0' : str2[wcslen(str2) - 1]), str2)),
			  str2 != nullptr)
		{
			if(str == split)
			{
				if(!seg)
				{
					add(&sprite);

					width = 0, height = 0;
					sprite.clear();
				}
				seg = true;
				continue;
			}

			width = width < (ushort)str.size() ? (ushort)str.size() : width;
			sprite.push_back(str);
			height++;
			seg = false;
		}
		fclose(f);

		if(height > 0)
			add(&sprite);
		delete[] str2;
	}

	void add(Sprite sprite) { m_sheet->push_back(new Sprite(sprite)); }

	void add(std::vector<std::wstring>* sprite)
	{
		Sprite* tmp = new Sprite;
		tmp->create(sprite);
		m_sheet->push_back(tmp);

	}

	void remove(int index)
	{
		//delete m_sheet[0][index];
		//_numSprites--;
		m_sheet->erase(m_sheet->begin() + index);

	}

	void remove(const char* tag)
	{
		for(int a = 0; a < size(); a++)
			if(m_sheet[0][a]->getTag() == tag)
			{

				//delete m_sheet[0][a];
				//break;
				m_sheet->erase(m_sheet->begin() + a);
			}

		//_numSprites--;
	}

	void clear()
	{
		//for(int a = 0; a < _numSprites; a++)
		//	delete m_sheet[0][a];
		//Sprite** tmp = m_sheet;
		//delete[] tmp;

		m_sheet->clear();
	}

	int size()
	{
		return m_sheet->size();
	}

	Sprite& at(unsigned int index)
	{
		return operator[](index);
	}

	Sprite& at(const char* tag)
	{
		for(int a = 0; a < size(); a++)
			if(m_sheet[0][a]->getTag() == tag)
				return *m_sheet[0][a];
		return *m_sheet[0][size()];
	}

	Sprite& operator[](unsigned int index)
	{
		return *m_sheet[0][index];
	}

	std::vector<Sprite*>::iterator begin() { return m_sheet->begin(); }
	std::vector<Sprite*>::iterator end() { return m_sheet->end(); }
private:
	std::vector<Sprite*>* m_sheet = new std::vector<Sprite*>;
	//unsigned short _numSprites = 0;
};

struct Animation:public SpriteSheet
{
	Animation():SpriteSheet() {}
	Animation(const SpriteSheet& cpy):SpriteSheet(cpy) {}

	void update()
	{
		float time = (float)clock() / CLOCKS_PER_SEC;

		if(!m_init)
		{
			m_lastTime = time;
			m_init = true;
		}

		if(!m_pause && !m_stop)
		{
			if((time = (time - m_lastTime)) >= m_speed)
			{
				if(m_repeat)
				{
					m_currentFrame = int(time / m_speed) % size();
				}
				else
				{
					m_currentFrame = int(time / m_speed);
					m_currentFrame = m_currentFrame >= size() - 1 ? unsigned((size() - 2) % size()) : m_currentFrame;

					//if(m_currentFrame < m_unpackedData.size() - 2)
					//	mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[m_frameNext = (m_frame + 1) % m_unpackedData.size()]);
					//else
					//	mesh->editVerts(m_unpackedData[m_frame], m_unpackedData[m_frameNext = m_frame]);
				}

			}
		}
		else
		{

			if((time = (time - m_lastTime)) >= m_speed)
			{
				if(m_pause)
				{
					m_lastTime = time;
				}
				else
					if(m_stop)
					{
						m_currentFrame = 0;
						m_lastTime = time;
					}
			}
		}
		
	
	}

	Sprite& getSprite()
	{
		return this->operator[](m_currentFrame);
	}

	void setRepeat(bool repeat)
	{
		m_repeat = repeat;
	}

	void setPause(bool pause)
	{
		m_pause = pause;
	}

	void setStop(bool stop)
	{
		m_stop = stop;
	}

	bool isRepeat()
	{
		return m_repeat;
	}

	bool isPause()
	{
		return m_pause;
	}

	bool isStop()
	{
		return m_stop;
	}

	void reset()
	{
		m_currentFrame = 0;

	}

	void setSpeed(float seconds)
	{
		m_speed = seconds;
	}

	void setCurrentFrame(ushort frame)
	{
		m_currentFrame = frame;
	}

private:
	ushort  m_currentFrame=0;
	float m_speed=0, m_lastTime=0;
	bool m_init = false, m_pause = false, m_stop = false, m_repeat = true;
};

struct MouseInput
{

	static bool doubleClick;
	static short vertWheel, horiWheel;
	static COORD position;

	static bool pressed(MouseButtons button)
	{
		return GetAsyncKeyState(button);
	}

	static bool released(MouseButtons button)
	{
		return	!GetAsyncKeyState(button);
	}

	static bool stroke(MouseButtons button)
	{
		if(GetAsyncKeyState(button))
			buttons[button] = true;
		if(!GetAsyncKeyState(button) && buttons[button])
			return (buttons[button] = false, true);

		return false;
	}

	static void update()
	{

		DWORD numEvents;
		INPUT_RECORD irBuff[128];

		GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &numEvents);
		if(numEvents)
			ReadConsoleInputA(
			GetStdHandle(STD_INPUT_HANDLE),      // input buffer handle 
			irBuff,      // buffer to read into 
			128,         // size of read buffer 
			&numEvents);

		for(UINT a = 0; a < numEvents; a++)
		{
			switch(irBuff[a].EventType)
			{
			case MOUSE_EVENT: // mouse input 
				MOUSE_EVENT_RECORD mer = irBuff[a].Event.MouseEvent;
				switch(mer.dwEventFlags)
				{
				case DOUBLE_CLICK:
					doubleClick = true;
					break;

				case MOUSE_MOVED:
					position = {mer.dwMousePosition.X, mer.dwMousePosition.Y};
					break;
				case MOUSE_WHEELED://vertical
					vertWheel += (int)mer.dwButtonState / std::abs((int)mer.dwButtonState);
					break;
				case MOUSE_HWHEELED://horizontal
					horiWheel += (int)mer.dwButtonState / std::abs((int)mer.dwButtonState);
					break;
				}

			}
		}



	}

private:
	static std::map <short, bool> buttons;
};



/*Classes*/
class EmConsole
{
public:
	/***Constructers***/
	//EmConsole(std::string& title);
	//
	//EmConsole(const char* title);
	//
	//EmConsole();

	static void init();

	static void setFullScreen(bool);
	static bool getFullScreen();

	/***Functions***/

	//sends a text file to a vector of vector wstring 
	static void textFileToVector(std::string file, std::vector<std::vector<std::wstring>>& str);

	//sends a text file to a vector of vector wstring
	static std::vector<std::vector<std::wstring>> textFileToVector(std::string file);

	//sets the title
	static void setTitle(std::string title);

	//sets the console size
	static void setConsoleSize(ushort x, ushort y);

	//returns
	static COORD getConsoleSize();

	static int getWidth();

	static int getHeight();

	void printf(const char*...);

	//sets weather or not the window can be resized
	static void setResizeable(bool resz);
	static bool getResizeable();

	//sets position of the console from the top left corner
	static void setConsolePosition(int x, int y);

	static void placeConsoleCenter();

	static void consoleCursorPosition(int x, int y);

	/* Start: special stuff for tetris (don't even try to understand)*/
	static std::vector<int> readConsoleLineAtributes(int x, int y, float width);

	static std::vector<std::vector<int>> readConsoleAreaAtributes(int x, int y, float width, float height);

	static std::vector<std::wstring> readConsoleArea(int x, int y, float width, float height);

	static std::wstring readConsoleLine(int x, int y, float width);

	static char readConsoleCharacter(int x, int y);

	static char readActiveConsoleCharacter(int x, int y);
	/* End: special stuff for tetris (don't even try to understand)*/

	MouseInput mouseState()
	{
		static MouseInput input;
		input.update();
		return input;
	}



	/*Console buffer overloads*/

	/*
	Note: NS means No Spaces, wrighting only characters excluding spaces.
	Extrymely inefficient
	*/

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, int x, int y, Colour colour);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBufferNS(std::wstring& str, float& poX, float& poY, int x, int y, int colour);

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, int x, int y);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBufferNS(std::wstring& str, float& poX, float& poY, int x, int y);

	/*
	toConsoleBuffer(wstring &str, int x, int y, int colour);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBufferNS(std::wstring& str, int x, int y, int colour);

	/*
	toConsoleBuffer(wstring &str, int x, int y);
	* str    - wstring to be drawn
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBufferNS(std::wstring& str, int x, int y);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, int x, int y, int colour);
	* str    - vector of wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen
	* colour - colour of text

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the specified origin (poX, poY)
	to be drawn after drawConsole(); is called (with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBufferNS(Sprite& str, float& poX, float& poY, int x, int y, int colour);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, int x, int y);
	* str    - vector of wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBufferNS(Sprite& str, float& poX, float& poY, int x, int y);

	/*
	toConsoleBuffer(vector<wstring>& str, int x, int y, int colour);
	* str    - vector of wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBufferNS(Sprite& str, int x, int y, int colour);

	/*
	toConsoleBuffer(vector<wstring>& str, int x, int y);
	* str    - vector of wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBufferNS(Sprite& str, int x, int y);

	/*
	toConsoleBuffer(const char* str, float poX, float poY, int x, int y, int colour);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBufferNS(const wchar_t* str, float& poX, float& poY, int x, int y, int colour);

	/*
	toConsoleBuffer(const wchar_t* str, float poX, float poY, int x, int y);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBufferNS(const wchar_t* str, float& poX, float& poY, int x, int y);

	/*
	toConsoleBuffer(const wchar_t *str, int x, int y, int colour);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the specified top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBufferNS(const wchar_t* str, int x, int y, int colour);

	/*
	toConsoleBuffer(const wchar_t *str, int x, int y);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the specified top left corner
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBufferNS(const wchar_t* str, int x, int y);

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, int x, int y, int colour);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBuffer(std::wstring& str, float& poX, float& poY, int x, int y, std::vector<int>& colour);

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, int x, int y, int colour);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBuffer(std::wstring& str, float& poX, float& poY, int x, int y, int colour);

	/*
	toConsoleBuffer(wstring &str, float poX, float poY, int x, int y);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBuffer(std::wstring& str, float& poX, float& poY, int x, int y);

	/*
	toConsoleBuffer(wstring &str, int x, int y, int colour);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBuffer(std::wstring& str, int x, int y, int colour);

	/*
	toConsoleBuffer(wstring &str, int x, int y);
	* str    - wstring to be drawn
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBuffer(std::wstring& str, int x, int y);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, int x, int y, std::vector<std::vector<int>> &colour);
	* str    - vector of wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen
	* colour - colour of text

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the specified origin (poX, poY)
	to be drawn after drawConsole(); is called (with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBuffer(Sprite& str, int x, int y, std::vector<std::vector<int>>& colour);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, int x, int y, int colour);
	* str    - vector of wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen
	* colour - colour of text

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the specified origin (poX, poY)
	to be drawn after drawConsole(); is called (with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBuffer(Sprite& str, float& poX, float& poY, int x, int y, int colour);

	/*
	toConsoleBuffer(vector<wstring>& str, float poX, float poY, int x, int y);
	* str    - vector of wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBuffer(Sprite& str, float& poX, float& poY, int x, int y);

	/*
	toConsoleBuffer(vector<wstring>& str, int x, int y, int colour);
	* str    - vector of wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBuffer(Sprite& str, int x, int y, int colour);

	/*
	toConsoleBuffer(vector<wstring>& str, int x, int y);
	* str    - vector of wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds multiple lines to the console starting at the
	specified (x,y) position from the top left corner
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBuffer(Sprite& str, int x, int y);

	/*
	toConsoleBuffer(const wchar_t* str, float poX, float poY, int x, int y, int colour);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBuffer(const wchar_t* str, float& poX, float& poY, int x, int y, int colour);

	/*
	toConsoleBuffer(const wchar_t* str, float poX, float poY, int x, int y);
	* str    - wstring to be drawn to buffer
	* poX    - the x coord of origin position of the console (where you consoder (0,0) to be)
	* poY    - the y coord of origin position of the console (where you consoder (0,0) to be)
	* x      - x position from the origin poX coordinate on the screen
	* y      - y position from the origin poY coordinate on the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the origin (poX, poY)
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBuffer(const wchar_t* str, float& poX, float& poY, int x, int y);

	/*
	toConsoleBuffer(const wchar_t *str, int x, int y, int colour);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen
	* colour - colour of text

	summary:
	Adds single line to the console to the
	specified (x,y) position from the specified top left corner
	to be drawn after drawConsole(); is called
	(with text colour modification [see: enum colour in EmConsole.h])
	*/
	static void toConsoleBuffer(const wchar_t* str, int x, int y, int colour);

	/*
	toConsoleBuffer(const wchar_t *str, int x, int y);
	* str    - wstring to be drawn to buffer
	* x      - x position from the top left corner of the screen
	* y      - y position from the top left corner of the screen

	summary:
	Adds single line to the console to the
	specified (x,y) position from the specified top left corner
	to be drawn after drawConsole(); is called
	*/
	static void toConsoleBuffer(const wchar_t* str, int x, int y);

	/*
	drawConsole(bool clear = true);
	* clear = clear each frame

	Summary:
	Draws the console (choose to clear console buffer after)
	*Note:
	Default is true
	*/
	static void drawConsole(bool clear = true);

	//clears the console
	static void clearConsole();

private:
	/**Variables**/

	static DWORD oldInputMode, newInputMode;

	//static COORD _cursorPosition;
	static HANDLE m_con[2]/*, _input*/;
	static INPUT_RECORD m_inputRecord[128];

	//static UINT _conWidth, _conHeight;
	static bool m_buff, m_resizable, m_full, m_init;

};
