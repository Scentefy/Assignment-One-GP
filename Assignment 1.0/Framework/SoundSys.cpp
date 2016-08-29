// This include
#include "SoundSys.h"
#include "logmanager.h"



SoundSys::SoundSys()
{
	if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
	{
		// Report Error
		LogManager::GetInstance().Log("FMOD Init Fail!");
		return;
	}

	int driverCount = 0;
	m_pSystem->getNumDrivers(&driverCount);

	if (driverCount == 0)
	{
		// Report Error
		LogManager::GetInstance().Log("FMOD driver Init Fail!");
		return;
	}

	// Initialize our Instance with 36 Channels
	m_pSystem->init(36, FMOD_INIT_NORMAL, 0);
}


void
SoundSys::createSound(SoundClass *pSound, const char* pFile)
{
	m_pSystem->createSound(pFile, FMOD_DEFAULT, 0, pSound);
}

void
SoundSys::playSound(SoundClass pSound, bool bLoop)
{

	if (!bLoop)
		pSound->setMode(FMOD_LOOP_OFF);
	else
	{
		pSound->setMode(FMOD_LOOP_NORMAL);
		pSound->setLoopCount(-1);
	}

	// might need a channel for the last args
	m_pSystem->playSound(pSound, 0, false, 0);
}

void
SoundSys::releaseSound(SoundClass pSound)
{
	pSound->release();
}