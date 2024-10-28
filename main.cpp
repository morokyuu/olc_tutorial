// VS2019 C++ install
// compiler option -> C++17 (this is mandatory)
//
// git config, suggested settings of new line code.
// https://qiita.com/uggds/items/00a1974ec4f115616580
// git config --global core.autocrlf input
//

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class BreakOut : public olc::PixelGameEngine
{
public:
	BreakOut()
	{
		sAppName = "Tutorial - BreakOut clone";
	}

private:
    float fBatPos = 20.0f;
    float fBatWidth = 40.0f;
    olc::vf2d vBall = { 200.0f, 200.0f };
    
public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        Clear(olc::DARK_BLUE);

        //Boundary
        DrawLine(10, 10, ScreenWidth()-10, 10, olc::YELLOW);
        DrawLine(10, 10, 10, ScreenHeight()-10, olc::YELLOW);
        DrawLine(ScreenWidth()-10, 10, ScreenWidth()-10, ScreenHeight()-10, olc::YELLOW);
        //Draw Bat
        FillRect(int(fBatPos),ScreenHeight()-20,int(fBatWidth),10,olc::GREEN);

        //Draw Ball
        FillCircle(vBall, 5, olc::CYAN);

		DrawString(olc::vf2d(100, 200), "Hello", olc::WHITE, 1);

		return true;
	}
};


int main()
{
	BreakOut demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();

	return 0;
}
