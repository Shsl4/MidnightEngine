#pragma once

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/bgfx.h>
#include <imgui/imgui.h>
#include <iconfontheaders/icons_kenney.h>
#include <iconfontheaders/icons_font_awesome.h>
#include <Core/Logging/Logger.h>

class CharacterInfoWindow {

    class FlyingCharacter *character;

public:

    CharacterInfoWindow(class FlyingCharacter *character);

    void render(const char *_errorText);

};
