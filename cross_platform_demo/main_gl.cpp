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
#include "instancing.h"


using namespace graphicEngine::gl;

int main()
{
    Instancing obj;
    obj.initialize();
    obj.draw();
    return 0;
}