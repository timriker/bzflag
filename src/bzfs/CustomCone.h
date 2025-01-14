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

#ifndef __CUSTOM_CONE_H__
#define __CUSTOM_CONE_H__

/* interface header */
#include "WorldFileObstacle.h"

/* local interface headers */
#include "WorldInfo.h"

/* common interface headers */
#include "BzMaterial.h"


class CustomCone : public WorldFileObstacle
{
public:
    CustomCone(bool pyramidStyle);
    ~CustomCone();
    virtual bool read(const char* cmd, std::istream& input);
    virtual void writeToGroupDef(GroupDefinition*) const;

private:
    enum
    {
        Edge,
        Bottom,
        StartFace,
        EndFace,
        MaterialCount
    };
    static const char* sideNames[MaterialCount];

    bool pyramidStyle;
    bool flipz;
    int divisions;
    float angle;
    float texsize[2];
    int phydrv;
    bool useNormals;
    bool smoothBounce;
    BzMaterial materials[MaterialCount];
};


#endif  /* __CUSTOM_CONE_H__ */

// Local variables: ***
// mode: C++ ***
// tab-width: 4***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
