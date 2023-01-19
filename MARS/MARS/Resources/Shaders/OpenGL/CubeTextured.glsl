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

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D EmissionTexture;

layout(binding = 1) uniform MaterialData {
    float Shininess;
} u_Material;

layout(binding = 9) uniform LightsData {
    vec4  Ambient;
    vec4  Diffuse;
    vec4  Specular;
    vec3  Location;
} u_Light;

layout(binding = 10) uniform DirectionalLightData {
    vec4  Ambient;
    vec4  Diffuse;
    vec4  Specular;
    vec3  Direction;
} u_DirLight;

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

float CalculateDiffuseAmount(vec3 lightDirection, vec3 pixelNormal)
{
    return max(dot(pixelNormal, lightDirection), 0.0);
}

float CalculateSpecularAmount(vec3 lightDirection, vec3 pixelNormal)
{
    const vec3 viewDirection       = normalize(cameraPos - pixelPos);
    const vec3 reflectionDirection = reflect(-lightDirection, pixelNormal);
    return pow(max(dot(viewDirection, reflectionDirection), 0.0), u_Material.Shininess);
}

LightingColor CalculateDirectionalLight()
{
    // Light Params:
    const vec3 lightDirection = normalize(u_DirLight.Direction); // TODO: imlement correct forward vector (at least z is incorrect)
    const vec3 pixelNormal    = normalize(normal);

    const float diffuseAmount  = CalculateDiffuseAmount(lightDirection, pixelNormal);
    const float specularAmount = CalculateSpecularAmount(lightDirection, pixelNormal);

    // Textures:
    const vec4 diffuseSample  = texture(DiffuseTexture, texCoord);
    const vec4 specularSample = texture(SpecularTexture, texCoord);

    // Light Color:
    LightingColor color;
    color.Ambient  = vec3(u_DirLight.Ambient * diffuseSample);
    color.Diffuse  = vec3(u_DirLight.Diffuse * (diffuseSample * diffuseAmount));
    color.Specular = vec3(u_DirLight.Specular * (specularSample * specularAmount));

    return color;
}

LightingColor CalculatePointLight()
{
   const float distance = length(u_Light.Location - pixelPos);

    // Attenuation:
    const float constant  = 1.0;
    const float linear    = 0.09f;
    const float quadratic = 0.032f ;

    const float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Light Params:
    const vec3 lightDirection = normalize(u_Light.Location - pixelPos);
    const vec3 pixelNormal    = normalize(normal);
    
    const float diffuseAmount  = CalculateDiffuseAmount(lightDirection, pixelNormal);
    const float specularAmount = CalculateSpecularAmount(lightDirection, pixelNormal);

    // Textures:
    const vec4 diffuseSample  = texture(DiffuseTexture, texCoord);
    const vec4 specularSample = texture(SpecularTexture, texCoord);

    // Light Color:
    LightingColor color;
    color.Ambient  = vec3(u_Light.Ambient * diffuseSample) * attenuation;
    color.Diffuse  = vec3(u_Light.Diffuse * (diffuseSample * diffuseAmount)) * attenuation;
    color.Specular = vec3(u_Light.Specular * (specularSample * specularAmount)) * attenuation;

    return color;
}

void main()
{
    const vec3 emissionSample = texture(EmissionTexture, texCoord).rgb;


    // const LightingColor color = CalculateDirectionalLight();
    const LightingColor color = CalculatePointLight();

    pixelColor = vec4((color.Ambient + color.Diffuse + color.Specular + emissionSample), 1.0);
}