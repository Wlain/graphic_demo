//
//  shaders.metal
//  metal_learning
//
//  Created by william on 2021/5/15.
//

#include <metal_stdlib>
#include "shaderTypes.h"
using namespace metal;

typedef struct
{
    float4 position [[position]];
    float2 texCoord;
} ColorInOut;

vertex ColorInOut vertShader(constant Vertex *vertexArr [[buffer(0)]],
                               uint vid [[vertex_id]])
{
    ColorInOut out;
    float4 position = vector_float4(vertexArr[vid].position, 0 , 1.0);
    out.position = position;
    return out;
}

fragment float4 fragShader(ColorInOut in [[stage_in]])
{
    return float4(1.0,0,0,0);
}


