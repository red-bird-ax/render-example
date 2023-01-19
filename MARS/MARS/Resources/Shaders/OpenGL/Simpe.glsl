Vertex Shader: #version 460 core

layout (location = 0) in vec4 a_Location;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec2 a_UV;

uniform CameraData {
    mat4 MVP;
    mat4 View;
} cam;

out vec2 texCoord;
out vec3 color;

void main()
{
    texCoord    = a_UV;
    color       = a_Color;
    gl_Position = cam.MVP * a_Location;
}

Pixel Shader: #version 460 core

out vec4 pixelColor;
in  vec2 texCoord;
in  vec3 color;

uniform sampler2D texSampler;
uniform sampler2D texSampler2;

void main()
{
    pixelColor = mix(texture(texSampler, texCoord), texture(texSampler2, texCoord), 0.2);
}