#include "engine.h"

namespace motheye::engine
{
	void Engine::Start(HWND hWnd)
	{

	}
	
	void Engine::Stop()
	{

	}

	void Engine::OnRunFrame()
	{       
		const float deltaSeconds = clock_.Tick();

		// TODO:  forward inputState_ to game to resolve actions
		//game_.Resolve(inputState_, deltaSeconds);
	}
}