#pragma once

#define SDL_MAIN_HANDLED
#include <Core/Engine.h>
#include <SDL2/SDL_syswm.h>

class Entry {

public:

	int entry(int argc, const char** argv);

	int initEngine(SDL_Window* window);

	void update() const;

private:

	bool hasTerminated = false;
	UniquePtr<MEngine> engine;

};