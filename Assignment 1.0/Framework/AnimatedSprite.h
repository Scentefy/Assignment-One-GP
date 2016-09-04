#ifndef __ANIMATEDSPRITE_H__
#define __ANIMATEDSPRITE_H__

#include "sprite.h"
#include <vector>

class BackBuffer;
class Texture;

class AnimatedSprite : public Sprite
{
	// Methods:
public:
	AnimatedSprite();
	~AnimatedSprite();

	bool Initialise(Texture& texture);
	void AddFrame(int x);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SetFrameSpeed(float f);
	float GetFrameSpeed();

	void SetCurrentFrame(int c);
	int GetCurrentFrame();

	void SetNumOfFrames(int n);
	int GetNumOfFrames();

	void Pause(bool tf);
	bool IsPaused();

	bool IsAnimating();
	void StartAnimating();

	bool IsLooping();
	void SetLooping(bool b);

	int GetFrameWidth();
	void SetFrameWidth(int w);

	int GetFrameHeight();
	void SetFrameHeight(int h);

	int GetXPos();
	void SetXPos(int x);

	int GetYPos();
	void SetYPos(int y);
protected:

private:

	// Fields:
public:

protected:
	// W02.4: Add a field, the container for frame coordinates.
	std::vector<int> coordinateContainer;

	Texture* m_pTexture;
	float m_frameSpeed;
	int m_frameWidth;
	int m_frameHeight;

	float m_timeElapsed;
	int m_currentFrame;
	int m_numOfFrames;

	bool m_paused;
	bool m_loop;
	bool m_animating;

	int xPos;
	int yPos;

private:

};

#endif // __ANIMATEDSPRITE_H__