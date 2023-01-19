Vertex Shader: #version 460 core

layout (location = 0) in vec4 a_Location;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_UV;

layout(binding = 0) uniform MeshData {
    mat4 Model;
} u_Mesh;

layout(binding = 8) uniform CameraData {
    mat4 ViewProjection;
    mat4 View;
    vec3 Location;
} u_Camera;

out vec2 texCoord;
out vec3 normal;
out vec3 pixelPos;
out vec3 cameraPos;

void main()
{
    texCoord    = a_UV;
    normal      = mat3(transpose(inverse(u_Mesh.Model))) * a_Normal;
    pixelPos    = vec3(u_Mesh.Model * a_Location);
    cameraPos   = u_Camera.Location;
    gl_Position = u_Camera.ViewProjection * u_Mesh.Model * a_Location;
}

Pixel Shader: #version 460 core

layout(binding = 1) uniform MaterialData {
    vec4  Ambient;
    vec4  Diffuse;
    vec4  Specular;
    float Shininess;
} u_Material;

layout(binding = 9) uniform LightsData {
    vec4  Ambient;
    vec4  Diffuse;
    vec4  Specular;
    vec3  Location;
} u_Light;

out vec4 pixelColor;
in  vec3 pixelPos;
in  vec3 cameraPos;
in  vec2 texCoord;
in  vec3 normal;

struct LightingColor {
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

float GetDiffuseAmount()
{
    const vec3 pixelNormal    = normalize(normal);
    const vec3 lightDirection = normalize(u_Light.Location - pixelPos);
    return max(dot(pixelNormal, lightDirection), 0.0);
}

float GetSpecularAmount()
{
    const vec3 pixelNormal         = normalize(normal);
    const vec3 lightDirection      = normalize(u_Light.Location - pixelPos);
    const vec3 viewDirection       = normalize(cameraPos - pixelPos);
    const vec3 reflectionDirection = reflect(-lightDirection, pixelNormal);
    return pow(max(dot(viewDirection, reflectionDirection), 0.0), u_Material.Shininess);
}

LightingColor CalculateLighting()
{
    LightingColor color;

    color.Ambient  = vec3(u_Light.Ambient * u_Material.Ambient);
    color.Diffuse  = vec3(u_Light.Diffuse * (u_Material.Diffuse * GetDiffuseAmount()));
    color.Specular = vec3(u_Light.Specular * (u_Material.Specular * GetSpecularAmount()));

    return color;
}

void main()
{
    const LightingColor color = CalculateLighting();
    pixelColor = vec4((color.Ambient + color.Diffuse + color.Specular), 1.0);
}