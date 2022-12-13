
uniform float progress;
uniform vec2 dims;

void main() {
    vec4 color = texture2D(u_texture, cc_FragTexCoord1);

    if(color != vec4(1.0, 1.0, 1.0, 1.0) && color.a == 1.0 && gl_FragCoord.x < (progress) * dims.x) {
        color = vec4(0.713, 1.0, 0.0, 1.0);
    }

    gl_FragColor = color; // vec4(0, 0, 0, gl_FragCoord.x / 255);
}