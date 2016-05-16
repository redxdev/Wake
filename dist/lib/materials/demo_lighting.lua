local shader = Shader.new(
[[
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec3 outNormal;
out vec2 outTexCoords;

void main()
{
    gl_Position = projection * view * transform * vec4(position, 1.0);
    outNormal = normal;
    outTexCoords = texCoords;
}
]],
[[
#version 330 core
in vec3 outNormal;
in vec2 outTexCoords;

out vec4 outColor;

uniform sampler2D tex1;
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform float lightAmbience;
uniform float minBrightness;

void main()
{
    vec4 texColor = texture(tex1, outTexCoords);

    float diffuseIntensity = max(minBrightness, dot(normalize(outNormal), -normalize(lightDirection)));
    outColor = vec4(lightColor, 1.0) * vec4(lightColor * (lightAmbience * diffuseIntensity) * texColor.rgb, 1.0);
}
]]
)

local material = Material.new()
material:setShader(shader)
material:setVec3('lightColor', {1, 1, 1})
material:setVec3('lightDirection', {1, -1, 0.6})
material:setFloat('lightAmbience', 0.8)
material:setFloat('minBrightness', 0.15)

return material