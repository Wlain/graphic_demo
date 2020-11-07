//
//  CGShaders..metal
//  metal_demo
//
//  Created by william on 2020/11/7.
//

#import "CGShaderTypes.h"
#include <metal_stdlib>
using namespace metal;


typedef struct _ResterizerData
{
    // [[position]] 指的是输定限定符，顶点裁剪后的位置值
    float4 clipSpacePosition [[position]];
    
    float4 color;
}ResterizerData;


// 顶点着色器,vertex:表示顶点函数， ResterizerData: 表示返回的数据类型,vertexShader:函数名称
vertex ResterizerData vertexShader(uint vertexID [[vertex_id]],
                                   constant CGVertex *vertices [[buffer(CGVertexInputIndexVertices)]],
                                   constant vector_uint2 *viewportSizePointer [[buffer(CGVertexInputIndexViewportSize)]])
{
    /* 处理顶点数据，
        1.执行坐标系转换，将生成的顶点裁剪空间写到返回值当中
        将顶点颜色值传递给返回值
     */
    
    // 定义out
    ResterizerData out;
    
    // init
    out.clipSpacePosition = vector_float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // 获得当前顶点，我们的位置是在像素维度中指定的
    float2 pixelSpacePosition = vertices[vertexID].position.xy;
    
    // 将viewportSizePointer 从 vector_uint2 转化为 vector_float2类型
    vector_float2 viewportSize = vector_float2(*viewportSizePointer);
    
    out.clipSpacePosition.xy = pixelSpacePosition / (viewportSize / 2.0);
    
    out.color = vertices[vertexID].color;
    
    return out;
}



// 片元着色器, fragment: 表示片元函数
fragment float4 fragmentShader(ResterizerData in [[stage_in]])
{
    return in.color;
}


