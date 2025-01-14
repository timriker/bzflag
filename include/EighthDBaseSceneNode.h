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

/* EighthDBaseSceneNode:
 *  Encapsulates information for rendering the eighth dimension
 *  of a base building.
 */

#ifndef BZF_EIGHTHD_BASE_SCENE_NODE_H
#define BZF_EIGHTHD_BASE_SCENE_NODE_H

// Inherit from
#include "EighthDimSceneNode.h"

class EighthDBaseSceneNode : public EighthDimSceneNode
{
public:
    EighthDBaseSceneNode(const float pos[3],
                         const float size[3], float rotation);
    ~EighthDBaseSceneNode();
    void        notifyStyleChange();
    void        addRenderNodes(SceneRenderer&);
protected:
    class EighthDBaseRenderNode : public RenderNode
    {
    public:
        EighthDBaseRenderNode(const EighthDBaseSceneNode *,
                              const float pos[3],
                              const float size[3], float rotation);
        ~EighthDBaseRenderNode();
        void        render();
        const GLfloat * getPosition() const
        {
            return sceneNode->getSphere();
        }
    private:
        const EighthDBaseSceneNode *sceneNode;
        GLfloat corner[8][3];
    };
private:
    OpenGLGState      gstate;
    EighthDBaseRenderNode renderNode;
};

#endif // BZF_EIGHTHD_BASE_SCENE_NODE_H

// Local Variables: ***
// mode: C++ ***
// tab-width: 4 ***
// c-basic-offset: 4 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=4 tabstop=4
