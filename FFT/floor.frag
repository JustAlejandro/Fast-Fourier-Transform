R"zzz(
#version 410 core
uniform mat4 projection;
uniform mat4 view;
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 world_position;
uniform float specular;
layout(location = 0) out vec4 fragment_color;
layout(location = 1) out vec4 ss_Normal;
layout(location = 2) out vec4 world_Pos;
layout(location = 3) out vec4 specs;
float rand(vec2 co){
    return fract(sin(dot(co,co)));
}

void main() {
	vec4 pos = world_position;
	float check_width = 5.0;
	float i = floor(pos.x / check_width);
	float j  = floor(pos.z / check_width);
	vec3 color = mod(i + j, 2) * vec3(1.0, 1.0, 1.0);
	vec2 rands = vec2(rand(vec2(world_position.x, world_position.z)), rand(vec2(world_position.z, world_position.x)));
	vec4 norm = vec4(face_normal.x + rands.x * 0.4, face_normal.y, face_normal.z + rands.y * 0.4,0.0);
	float dot_nl = dot(normalize(light_direction), normalize(norm));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	color = clamp(dot_nl * color, 0.0, 1.0);
	fragment_color = vec4(color, 1.0);
	ss_Normal = projection * view * normalize(norm);
	world_Pos = world_position;
	specs = vec4(specular,specular,specular,1.0);
}
)zzz"
