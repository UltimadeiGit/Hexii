void main (){
    vec4 color = texture2D(u_texture, cc_FragTexCoord1);
    float sinTime = sin(cc_Time * 0.8);

    vec4 filter = vec4(1.0, 1.0, 1.0, 1.0);
    float strength = (sinTime * sinTime * 0.25) * color.a;

    gl_FragColor = color + (strength * filter);
}