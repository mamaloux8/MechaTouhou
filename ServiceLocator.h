#ifndef SERVICE_LOCATOR
#define SERVICE_LOCATOR

#include "IAudioProvider.h"

class ServiceLocator
{
public:
	static IAudioProvider* getAudio() { return m_audioProvider; }

	static void registerServiceLocator(IAudioProvider* provider)
	{
		m_audioProvider = provider;
	}


private:
	static IAudioProvider* m_audioProvider;
};

#endif
