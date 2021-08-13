layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 Color;
layout (location = 3) in vec2 TexCoord;

out VS_OUT {
    vec3 Normal;
    vec3 Color;
    vec2 TexCoord;
    vec3 WorldPosition;
} o;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;


void main() {
    gl_Position = p * v * m * vec4(Pos, 1.0);
    o.Normal = mat3(transpose(inverse(v * m))) * Normal;
    o.Color = Color;
    o.TexCoord = TexCoord;
    o.WorldPosition = vec3(v * m * vec4(Pos, 1.0));
}
