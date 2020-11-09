#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif // GL_FRAGMENT_PRECISION_HIGH

varying lowp vec3 vColor;

void main()
{
    gl_FragColor = vec4(vColor, 1.0);
}
