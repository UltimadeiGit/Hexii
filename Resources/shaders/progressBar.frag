
uniform float progress;

void main() {
    vec4 color = texture2D(u_texture, cc_FragTexCoord1);

    if(color == vec4(0.0, 0.0, 0.0, 1.0) && cc_FragTexCoord1.x <= progress) {
        // Yellow
        vec4 startColor = vec4(1.0, 0.843, 0.0, 1.0);
        // Orange
        vec4 endColor = vec4(1.0, 0.416, 0.0, 1.0);

        color = mix(startColor, endColor, cc_FragTexCoord1.x);
    }    

    gl_FragColor = color; // vec4(0, 0, 0, gl_FragCoord.x / 255);
}