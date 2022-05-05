#include <Input/Keybind.h>
#include <SDL2/SDL.h>

#define bitCheck(a, p) ((a) & (1 << (p)))

KeyBind::KeyBind(const UInt8 button) {

    this->key = button;
    this->shift = false;
    this->control = false;
    this->alt = false;
    this->command = false;
    this->isMouseButton = true;
    this->isModifierKey = false;

}

KeyBind::KeyBind(const Int32 key) {
    
    this->key = key;
    this->shift = (key == SDLK_LSHIFT) || (key == SDLK_RSHIFT);
    this->control = (key == SDLK_LCTRL) || (key == SDLK_RCTRL);
    this->alt = (key == SDLK_LALT) || (key == SDLK_RALT);
    this->command = (key == SDLK_LGUI) || (key == SDLK_RGUI);
    this->isMouseButton = false;
    this->isModifierKey = shift || control || command || alt;

}

KeyBind::KeyBind(const Int32 key, UInt16 mods) {

    this->key = key;
    this->shift = bitCheck(mods, 0);
    this->control = bitCheck(mods, 6);
    this->alt = bitCheck(mods, 8);
    this->command = bitCheck(mods, 10);
    this->isMouseButton = false;
    this->isModifierKey = shift || control || command || alt;

}

bool KeyBind::operator==(const KeyBind& other) const {

    if (other.key != this->key) return false;
    if (other.shift != this->shift) return false;
    if (other.command != this->command) return false;
    if (other.control != this->control) return false;
    if (other.alt != this->alt) return false;
    if (other.isMouseButton != this->isMouseButton) return false;
    if (other.isModifierKey != this->isModifierKey) return false;

    return true;

}

bool KeyBind::operator<(const KeyBind& other) const {
    return this->key < other.key;
}

const char* KeyBind::getName() const
{
    return SDL_GetKeyName(key);
}
