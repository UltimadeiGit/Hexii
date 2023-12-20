#define PI 3.14159265358979323846 
#define HALF_PI 1.5707963267948
#define PROGRESS_MODIFIER 1.2
// Multiplier required to make the reverse direction animation start at 2pi (but still finish at 0)
#define REVERSE_ANIM_MULTI (1.0 / (PROGRESS_MODIFIER - 1.0))
#define ALT_PI HALF_PI * PROGRESS_MODIFIER

uniform float progress;
uniform vec2 screenCenter;
uniform vec2 hexCenter;
uniform sampler2D overlayTex;

void main()
{
    // Sample the texture at the current fragment position
    vec4 color = texture2D(u_texture, cc_FragTexCoord1);
    vec4 overlay = texture2D(overlayTex, cc_FragTexCoord1);

    // Progress is multiplied up to fit in an animation and make it feel more natural
    float altProgress = progress * PROGRESS_MODIFIER;
    float allowedAngle = altProgress * HALF_PI;

    vec2 relativeCoord = gl_FragCoord.xy - (hexCenter);
    // Symmetry
    relativeCoord = abs(relativeCoord);
    // The minus means that it works clockwise (because in this case atan is guaranteed to return 0 < atan < HALF_PI)
    float angle = HALF_PI - atan(relativeCoord.y, relativeCoord.x);
    
    if(altProgress > 0.0 && altProgress <= 1.0 && angle < allowedAngle) {
        color = mix(color, overlay, overlay.a);
    }
    // Reverse direction animation
    else if(altProgress > 1.0 && angle < REVERSE_ANIM_MULTI * (ALT_PI - allowedAngle)) {
        color = mix(color, overlay, overlay.a);
    }

    gl_FragColor = color;
}