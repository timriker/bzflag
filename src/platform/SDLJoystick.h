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

/* SDLJoystick:
 *  Encapsulates an SDL joystick
 */

#ifndef BZF_SDLJOY_H
#define BZF_SDLJOY_H

#include "BzfJoystick.h"
#include "bzfSDL.h"

class SDLJoystick : public BzfJoystick
{
public:
    SDLJoystick();
    ~SDLJoystick();

    void    initJoystick(const char* joystickName);
    bool    joystick() const;
    void    getJoy(int& x, int& y);
    unsigned long getJoyButtons();
    int     getNumHats();
    void    getJoyHat(int hat, float &hatX, float &hatY);
    void    getJoyDevices(std::vector<std::string> &list) const;
    void    getJoyDeviceAxes(std::vector<std::string> &list) const;
    void    setXAxis(const std::string &axis);
    void    setYAxis(const std::string &axis);

    bool    ffHasRumble() const;
    void    ffRumble(int count, float duration, float strong_motor, float weak_motor = 0.0f);

private:
    SDL_Joystick *joystickID;
    int     joystickButtons;
    int     numHats;
    int     xAxis;
    int     yAxis;
    bool    hasRumble;
};

#endif // BZF_SDLJOY_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 4 ***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
