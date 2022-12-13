attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

uniform mat4 u_MVPMatrix;

#ifdef GL_ES
varying lowp vec4 cc_FragColor;
varying mediump vec2 cc_FragTexCoord1;
#else
varying vec4 cc_FragColor;
varying vec2 cc_FragTexCoord1;
#endif

uniform float cc_Random01;
uniform float cc_Time;

#define AMPLITUDE 1

void main() {
    vec2 vertexPosition = a_position.xy;

    // Calculate a random displacement for the vertex
    vec2 displacement = vec2(
        AMPLITUDE * (2.0 * cc_Random01 - 1),
        AMPLITUDE * (2.0 * cc_Random01 + 1)
    );

    // Add the displacement to the vertex position
    vec2 position = vertexPosition + displacement;

    // Transform the vertex position from local space to screen space
    gl_Position = u_MVPMatrix * vec4(position.x, position.y, a_position.z, a_position.w);
    cc_FragColor = a_color;
    cc_FragTexCoord1 = a_texCoord;
}

/*

void main()
{
    gl_Position = u_MVPMatrix * a_position;
    cc_FragColor = a_color;
    cc_FragTexCoord1 = a_texCoord;
}

*/