//
//  CGShaderTypes.h
//  metal_demo
//
//  Created by william on 2020/11/7.
//

#ifndef CGShaderTypes_h
#define CGShaderTypes_h

// 缓存区索引值 共享与 shader 和 C 代码
typedef enum _CGVertexInputIndex
{
    // 顶点
    CGVertexInputIndexVertices = 0,
    CGVertexInputIndexViewportSize = 1
}CGVertexInputIndex;


// 顶点/颜色值
typedef struct CGVertex
{
    vector_float2 position;;
    
    vector_float4 color;
}CGVertex;


#endif /* CGShaderTypes_h */
