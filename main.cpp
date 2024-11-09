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
    float fBatSpeed = 250.0f;

    olc::vf2d vBallPos = { 0.0f, 0.0f };
    olc::vf2d vBallDir = { 0.0f, 0.0f };
    float fBallSpeed = 40.0f;
    float fBallRadius = 5.0f;

    olc::vi2d vBlockSize = {16,16};
    std::unique_ptr<int[]> blocks;
    std::unique_ptr<olc::Sprite> sprTile;
    std::unique_ptr<olc::Decal> sprFragment;

    struct sFragment
    {
        olc::vf2d pos;
        olc::vf2d vel;
        float fAngle;
        float fTime;
        olc::Pixel colour;
    };
    std::list<sFragment> listFragments;

    
public:
	bool OnUserCreate() override
	{
        blocks = std::make_unique<int[]>(24 * 30);
        for (int y = 0; y < 30; y++)
        {
            for (int x = 0; x < 24; x++)
            {
                if (x == 0 || y == 0 || x == 23)
                    blocks[y * 24 + x] = 10;
                else
                    blocks[y * 24 + x] = 0;

                if (x > 2 && x <= 20 && y > 3 && y <= 5)
                    blocks[y * 24 + x] = 1;
                if (x > 2 && x <= 20 && y > 5 && y <= 7)
                    blocks[y * 24 + x] = 2;
                if (x > 2 && x <= 20 && y > 7 && y <= 9)
                    blocks[y * 24 + x] = 3;
            }
        }

        for (int x = 1; x < 23; x++)
        {
            blocks[14 * 24 + x] = 10;
        }

        // Load the sprite
        sprTile = std::make_unique<olc::Sprite>("./tut_tiles.png");

        // Load Fragment Sprite 
        sprFragment = std::make_unique<olc::Sprite>("./tut_fragment.png");
        // Create decal of fragment 
        decFragment = std::make_unique<olc::Decal>(sprFragment.get());

        // Start Ball
        float fAngle = float(rand()) /float(RAND_MAX) * 2.0f * 3.14159;
        fAngle = -0.4;
        vBallDir = { cos(fAngle), sin(fAngle) };
        vBallPos = { 12.5f, 12.5f };
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        Clear(olc::DARK_BLUE);

        // Calculate where ball should be, if no collision
        olc::vf2d vPotentialBallPos = vBallPos + vBallDir * fBallSpeed * fElapsedTime;

        // Test for hits 4 points around ball
        olc::vf2d vTileBallRadialDims = {fBallRadius / vBlockSize.x, fBallRadius / vBlockSize.y};

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
                if(bTileHit)
                {
                    id = tile;
                    hitpos = { float(vTestPoint.x), float(vTestPoint.y) };
                    tile--;
                }

                // Collision response
                if(point.x == 0.0f) vBallDir.y *= -1.0f;
                if(point.y == 0.0f) vBallDir.x *= -1.0f;
                return bTileHit;
            }
        };

        

        // Handle User Input
        if (GetKey(olc::Key::LEFT).bHeld) fBatPos -= fBatSpeed * fElapsedTime;
        if (GetKey(olc::Key::RIGHT).bHeld) fBatPos += fBatSpeed * fElapsedTime;
        if (fBatPos < 11.0f) fBatPos = 11.0f;
        if (fBatPos + fBatWidth > float(ScreenWidth()) - 10.0f) fBatPos = float(ScreenWidth()) - 10.0f - fBatWidth;

        // four sides
        bool bHasHitTile = false;
        olc::vf2d hitpos;
        bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(0, -1), hispos, hitid);
        bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(0, +1), hispos, hitid);
        bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(-1, 0), hispos, hitid);
        bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(+1, 0), hispos, hitid);

        if (bHashHitTile)
        {
            for (int i = 0; i < 100; i++)
            {
                sFragment f;
                f.pos = { hispos.x + 0.5f, hispos.y + 0.5f };
                float fAngle = float(rand()) / float(RAND_MAX) * 2.0f * 3.14159f;
                float fVelocity = float(rand()) / float(RAND_MAX) * 10.0f;
                f.vel = { fVelocity * cos(fAngle), fVelocity * sin(fAngle) };
                f.fAngle = fAngle;
                f.fTime = 3.0f;
                if (hitid == 1) f.colour = olc::RED;
                if (hitid == 2) f.colour = olc::GREEN;
                if (hitid == 3) f.colour = olc::YELLOW;
                listFragments.push_back(f);
            }
        }
                
        
        // Fake Floor
        //if (vBallPos.y > 20.0f) vBallDir.y *= -1.0f;

        // Actually update ball position with modified direction
        vBallPos += vBallDir * fBallSpeed * fElapsedTime;

        // Update fragments
        for  (auto& f : listFragments)
        {
            f.vel += olc::vf2d(0.0f, 20.0f) * fElapsedTime;
            f.pos += f.vel * fElapsedTime;
            f.fAngle += 5.0f * fElapsedTime;
            f.fTime -= fElapsedTime;
            f.colour.a = (f.fTime / 3.0f) * 255;
        }

        // Remove dead fragments
        listFragments.erase(
            std::remove_if(listFragments.begin(), listFragments.end(), [](const sFragment& f) { return f.fTime < 0.0f; }),listFragments.end()
        );

        // Draw Screen
        Clear(olc::DARK_BLUE);
        SetPixelMode(olc::Pixel::MASK);
        for (int y = 0; y < 30; y++)
        {
            for (int x = 0; x < 24; x++)
            {
                switch (blocks[y * 24 + x])
                {
                    case 0:
                        break;
                    case 10: // Draw Boundary
                        DrawPartialSprite(olc::vi2d(x,y) * vBlockSize, sprTile.get(), olc::vi2d(0,0) * vBlockSize, vBlockSize);
                        break;
                    case 1:
                        DrawPartialSprite(olc::vi2d(x,y) * vBlockSize, sprTile.get(), olc::vi2d(1,0) * vBlockSize, vBlockSize);
                        break;
                    case 2:
                        DrawPartialSprite(olc::vi2d(x,y) * vBlockSize, sprTile.get(), olc::vi2d(2,0) * vBlockSize, vBlockSize);
                        break;
                    case 3:
                        DrawPartialSprite(olc::vi2d(x,y) * vBlockSize, sprTile.get(), olc::vi2d(3,0) * vBlockSize, vBlockSize);
                        break;
                }
            }
        }
        SetPixelMode(olc::Pixel::NORMAL);

        // Check if Ball has gone off screen
        if(vBallPos.y > ScreenHeight())
        {
            // Reset ball location
            vBallPos = {200.0f, 200.0f};
            // Choose Random direction
            float fAngle = (float(rand()) / float(RAND_MAX)) * 2.0f * 3.14159;
            vBallDir = { cos(fAngle), sin(fAngle) };
        }

        // Draw Bat
        FillRect(int(fBatPos), ScreenHeight() - 20, int(fBatWidth), 10, olc::GREEN);

        // Draw Ball
        FillCircle(vBallPos * vBlockSize, fBallRadius, olc::CYAN);

        // Draw Fragments
        for (auto& f : listFragments)
            DrawRotatedDecal(f.pos * vBlockSize, decFragment.get(), f.fAngle, { 4, 4 }, { 1, 1 }, f.colour);

		return true;
	}
};



int main()
{
	BreakOut demo;
	if (demo.Construct(512, 480, 2, 2, false, true))
		demo.Start();

	return 0;
}
