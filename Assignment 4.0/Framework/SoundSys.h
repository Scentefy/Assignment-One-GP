// COMP710 GP 2D Framework
#ifndef __SOUNDSYSTEM_H__
#define __SOUNDSYSTEM_H__

#include "fmod.hpp"
typedef FMOD::Sound* SoundClass;

class SoundSys
{
public:
	// Pointer to the FMOD instance
	FMOD::System *m_pSystem;
	SoundSys();

	// Class methods
	void createSound(SoundClass *pSound, const char* pFile);
	void playSound(SoundClass pSound, bool bLoop);
	void releaseSound(SoundClass pSound);
};

#endif // __SOUNDSYSTEM_H__