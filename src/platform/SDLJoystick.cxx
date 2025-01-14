/* bzflag
 * Copyright (c) 1993-2023 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* common headers */
#include "common.h"

/* interface headers */
#include "SDLJoystick.h"

/* system headers */
#include <vector>
#include <string>
#include <string.h>
#include <ctype.h>

/* implementation headers */
#include "ErrorHandler.h"
#include "TextUtils.h"
#include "bzfSDL.h"

SDLJoystick::SDLJoystick() : joystickID(nullptr), joystickButtons(0), numHats(0),
    xAxis(0), yAxis(1), hasRumble(false)
{
    SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE, "1");
#if SDL_VERSION_ATLEAST(2, 0, 16)
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE, "1");
#endif
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_STEAM, "1");

    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
    {
        std::vector<std::string> args;
        args.push_back(SDL_GetError());
        printError("Could not initialize SDL Joystick subsystem: %s.\n", &args);
    };
}

SDLJoystick::~SDLJoystick()
{
    // Close the joystick device, if opened
    if (joystickID != nullptr)
        SDL_JoystickClose(joystickID);

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void            SDLJoystick::initJoystick(const char* joystickName)
{
    // If the joystick is set to off or undefined, check if we had opened a joystick or haptic device and close them
    if (!strcasecmp(joystickName, "off") || !strcmp(joystickName, ""))
    {
        if (joystickID != nullptr)
        {
            SDL_JoystickClose(joystickID);
            joystickID = nullptr;
        }
        return;
    }

    char num = joystickName[0];
    int  i   = (int)num - '0';

    // If the requested joystick doesn't exist, bail out
    if (!isdigit(num) || i >= SDL_NumJoysticks())
    {
        printError("No supported SDL joysticks were found.");
        joystickID = nullptr;
        return;
    }

    // Attempt to open the joystick
    joystickID = SDL_JoystickOpen(i);

    // If opening the joystick failed, bail out
    if (joystickID == nullptr)
        return;

    // If the joystick has less than two axes, close the device and bail out
    if (SDL_JoystickNumAxes(joystickID) < 2)
    {
        SDL_JoystickClose(joystickID);
        printError("Joystick has less then 2 axes:\n");
        joystickID = nullptr;
        return;
    }

    // Fetch information about the joystick device
    joystickButtons = SDL_JoystickNumButtons(joystickID);
    numHats = SDL_JoystickNumHats(joystickID);
#if SDL_VERSION_ATLEAST(2, 0, 18)
    hasRumble = SDL_JoystickHasRumble(joystickID);
#else
    hasRumble = true;
#endif
}

bool            SDLJoystick::joystick() const
{
    return joystickID != nullptr;
}

void            SDLJoystick::getJoy(int& x, int& y)
{
    x = y = 0;

    if (!joystickID)
        return;

    SDL_JoystickUpdate();
    x = SDL_JoystickGetAxis(joystickID, xAxis);
    y = SDL_JoystickGetAxis(joystickID, yAxis);

    x = x * 1000 / 32768;
    y = y * 1000 / 32768;

    // ballistic
    x = (x * abs(x)) / 1000;
    y = (y * abs(y)) / 1000;

}

unsigned long       SDLJoystick::getJoyButtons()
{
    unsigned long buttons = 0;

    if (!joystickID)
        return 0;

    SDL_JoystickUpdate();
    for (int i = 0; i < joystickButtons; i++)
        buttons |= SDL_JoystickGetButton(joystickID, i) << i;

    return buttons;
}

int      SDLJoystick::getNumHats()
{
    if (!joystickID)
        return 0;

    return numHats;
}

void        SDLJoystick::getJoyHat(int hat, float &hatX, float &hatY)
{
    hatX = hatY = 0;

    if (!joystickID)
        return;

    if (hat >= numHats) // a precaution
        return;

    SDL_JoystickUpdate();
    int value = SDL_JoystickGetHat(joystickID, hat);
    //  9 1 3 ->   -1
    //  8 0 2 -> -1 0 1
    // 12 6 4 ->    1
    if (value - 8 >= 0)
    {
        value -= 8;
        hatX = -1;
    }
    if (value - 4 >= 0)
    {
        value -= 4;
        hatY = 1;
    }
    if (value - 2 >= 0)
    {
        value -= 2;
        hatX = 1;
    }
    if (value - 1 >= 0)
        hatY = -1;
}

void            SDLJoystick::getJoyDevices(std::vector<std::string>
        &list) const
{
    int numJoystick = SDL_NumJoysticks();
    if (numJoystick > 9) //user would have to be insane to have this many anyway
        numJoystick = 9;
    int i;
    for (i = 0; i < numJoystick; i++)
    {
        char joystickName[50]; //only room for so much on the menu
        snprintf(joystickName, 50, "%d - %s", i, SDL_JoystickNameForIndex(i));
        list.push_back(joystickName);
    }
}

void            SDLJoystick::getJoyDeviceAxes(std::vector<std::string> &list) const
{
    if (!joystickID) return;
    list.clear();
    // number all the axes and send them off
    for (int i = 0; i < SDL_JoystickNumAxes(joystickID); ++i)
        list.push_back(TextUtils::format("%d", i));
}

void            SDLJoystick::setXAxis(const std::string &axis)
{
    // unset
    if (axis == "") return;
    xAxis = atoi(axis.c_str());
}

void            SDLJoystick::setYAxis(const std::string &axis)
{
    // unset
    if (axis == "") return;
    yAxis = atoi(axis.c_str());
}

bool            SDLJoystick::ffHasRumble() const
{
    return (joystickID != nullptr && hasRumble);
}

void            SDLJoystick::ffRumble(int count, float duration, float strong_motor,
                                      float weak_motor)
{
    if (!ffHasRumble())
        return;

    // Clamp the motor values from 0 to 1
    strong_motor = std::min(1.0f, std::max(0.0f, strong_motor));
    weak_motor = std::min(1.0f, std::max(0.0f, weak_motor));

    SDL_JoystickRumble(joystickID, (Uint16)(strong_motor*0xFFFF), (Uint16)(weak_motor*0xFFFF),
                       (Uint32)(duration*1000.0f*count));

}

// Local Variables: ***
// mode: C++ ***
// tab-width: 4 ***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
