#version 330 core
//layout (location = 0) in vec4 vertex;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 fragmentUV;

//uniform mat4 view;
//uniform mat4 projection;
uniform vec2 lightPos;
uniform vec4 lightColor;
uniform float screenHeight;
uniform vec3 lightAttenuation;
uniform float radius;

void main() {
    //vec4 currentPos = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
    vec2 pixel = gl_FragCoord.xy;
    pixel.y = screenHeight-pixel.y;
    vec2 aux = lightPos - pixel;
    float distance = length(aux);
    float attenuation =
        1.0 / (lightAttenuation.x + lightAttenuation.y * distance + lightAttenuation.z * distance * distance);

    FragColor = vec4(attenuation, attenuation, attenuation, 1.0f) * lightColor;

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.9)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}