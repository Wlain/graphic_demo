//
// Created by william on 2020/11/14.
//

#include "showGLInfo.h"
#include "background.h"
#include "point.h"
#include "triangle.h"
#include "animation.h"
#include "sharing.h"
#include "cube.h"


using namespace graphicEngine::gl;

int main()
{
    Cube obj;
    obj.initialize();
    obj.draw();
    return 0;
}
