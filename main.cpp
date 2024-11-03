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
    olc::vf2d vBallVel = { 200.0f, -100.0f };

    olc::vf2d vBallPos = { 0.0f, 0.0f };
    olc::vf2d vBallDir = { 0.0f, 0.0f };
    float fBatSpeed = 250.0f;
    float fBallRadius = 5.0f;
    
public:
	bool OnUserCreate() override
	{
        srand(100);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        // Start Ball
        float fAngle = float(rand()) / float(RAND_MAX) * 2.0f * 3.14159f;
        fAngle = -0.4f;
        vBallDir = { cos(fAngle), sin(fAngle) };
        fBallPos = { 12.5f, 15.5f };

        // Handle User Input
        if (GetKey(olc::Key::LEFT).bHeld) fBatPos -= fBatSpeed * fElapsedTime;
        if (GetKey(olc::Key::RIGHT).bHeld) fBatPos += fBatSpeed * fElapsedTime;
        if (fBatPos < 11.0f) fBatPos = 11.0f;
        if (fBatPos + fBatWidth > float(ScreenWidth()) - 10.0f) fBatPos = float(ScreenWidth()) - 10.0f - fBatWidth;

        // Update Ball
        vBall += vBallVel * fElapsedTime;


        // Calculate where ball should be, if no collision
        olc::vf2d vPotentialBallPos = vBallPos + vBallDir * fBallSpeed * fElapsedTime;
        // Test for hits 4 points around ball
        olc::vf2d vTileBallRadialDims = {fBallRadius / vBlockSize.x, fBallRadius / vBalockSize.y};

        // four sides
        bool bHasHitTile = false;
        bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(0, -1));
        bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(0, +1));
        bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(-1, 0));
        bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(+1, 0));

        // Arena detection
        if(vBall.y <= 10.0f) vBallVel.y *= -1.0f;
        if(vBall.x <= 10.0f) vBallVel.x *= -1.0f;
        if(vBall.x >= float(ScreenWidth()) - 10.0f) vBallVel.x *= -1.0f;

        if(vBall.y >= (float(ScreenHeight()) - 20.0f) && (vBall.x > fBatPos) && (vBall.x < fBatPos + fBatWidth))
            vBallVel.y *= -1.0f;

        // Check if Ball has gone off screen
        if(vBall.y > ScreenHeight())
        {
            // Reset ball location
            vBall = {200.0f, 200.0f};
            // Choose Random direction
            float fAngle = (float(rand()) / float(RAND_MAX)) * 2.0f * 3.14159;
            vBallVel = {300.0f * cos(fAngle), 300.0f * sin(fAngle)};
        }

        if(vBallPos.y > 20.0f) vBallDir.y *= -1.0f;

        // Actually update ball position with modified direction
        vBallPos += vBallDir * fBallSpeed * fElapsedTime;

        // Erase previous frame
        Clear(olc::DARK_BLUE);

        // Draw Boundary
        DrawLine(10, 10, ScreenWidth() - 10, 10, olc::YELLOW);
        DrawLine(10, 10, 10, ScreenHeight() - 10, olc::YELLOW);
        DrawLine(ScreenWidth() - 10, 10, ScreenWidth() - 10, ScreenHeight() - 10, olc::YELLOW);

        // Draw Bat
        FillRect(int(fBatPos), ScreenHeight() - 20, int(fBatWidth), 10, olc::GREEN);

        // Draw Ball
        FillCircle(vBallPos * vBlockSize, fBallRadius, olc::CYAN);

        if (GetMouse(0).bHeld)
        {
            vBall = { float(GetMouseX()), float(GetMouseY()) };
        }
		return true;
	}
};

auto TestResolveCollisionPoint = [&](const olc::vf2d& point)
{
    olc::vi2d vTestPoint = vPotentialBallPos + vTileBallRadialDims * point;
    auto& tile = blocks[vTestPoint.y * 24 + vTestPoint.x];
    if(tile == 0)
    {
        // Do Nothing, no collision
        return false;
    }
    else
    {
        // Ball has collided with a tile
        bool bTileHit = tile < 10;
        if(bTileHit) tile--;

        // Collision response
        if(point.x == 0.0f)
            vBallDir.y *= -1.0f;
        if(point.y == 0.0f)
            vBallDir.x *= -1.0f;
        return bTileHit;
    }
};


int main()
{
	BreakOut demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();

	return 0;
}
