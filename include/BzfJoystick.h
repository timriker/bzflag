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

/* BzfJoystick:
 *  Abstract, platform independent base for Joysticks.
 */

#ifndef BZF_JOYSTICK_H
#define BZF_JOYSTICK_H

#include "common.h"
#include <string>
#include <vector>

class BzfJoystick
{
public:
    BzfJoystick();
    virtual     ~BzfJoystick();

    virtual void    initJoystick(const char* joystickName);
    virtual bool    joystick() const;
    virtual void    getJoy(int& x, int& y);
    virtual int  getNumHats();
    virtual void    getJoyHat(int hat, float &hatX, float &hatY);
    virtual unsigned long getJoyButtons();
    virtual void    getJoyDevices(std::vector<std::string> &list) const;

    /** Return a list of axes belonging to the joystick.  The first and second
     ** are assumed to be default X and Y axes, respectively.
     **/
    virtual void    getJoyDeviceAxes(std::vector<std::string> &list) const;
    virtual void    setXAxis(const std::string &axis);
    virtual void    setYAxis(const std::string &axis);

    /* Rumble force feedback support - a motor in the controller vibrates it,
     * as tactile feedback for explosions, collisions, engines starting, death,
     * that sort of thing.
     *
     * A device that supports rumble will return true from ffHasRumble(),
     * and ffRumble() will start playing a force feedback effect. For devices
     * lacking rumble, ffRumble() is a no-op.
     *
     * Our rumble interface follows the precedent set by Linux's FF_RUMBLE.
     * The joypad can have up to two motors, named the 'weak' motor and the
     * 'strong' motor. The 'strong' motor is typical of what's found in console
     * gaming systems, while the 'weak' motor is for more subtle vibration
     * effects. The strength of each motor is specified as a value in the
     * range [0,1].
     *
     * The effect starts after the given delay in seconds, runs for a duration
     * also in seconds, and repeats a total of 'count' times. Currently only
     * one effect is supported at a time, so using a count of zero will cancel
     * any rumble that may be in progress.
     */
    virtual bool    ffHasRumble() const;
    virtual void    ffRumble(int count, float duration, float strong_motor, float weak_motor=0.0f);
};

#endif // BZF_JOYSTICK_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 4 ***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
