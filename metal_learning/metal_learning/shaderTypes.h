//
//  shaderTypes.h
//  metal_learning
//
//  Created by william on 2021/5/15.
//

#ifndef shaderTypes_h
#define shaderTypes_h

enum VertexAttribute
{
    Position  = 0,
    TexCoord  = 1,
};

typedef struct
{
    vector_float2 position;
} Vertex;

#endif /* shaderTypes_h */
