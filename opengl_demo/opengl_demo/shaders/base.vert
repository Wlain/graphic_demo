#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif // GL_FRAGMENT_PRECISION_HIGH

attribute vec3 aPosition;
attribute vec3 aColor;
varying lowp vec3 vColor;

void main()
{
    vColor = aColor;
    gl_Position = vec4(aPosition, 1.0);
}
