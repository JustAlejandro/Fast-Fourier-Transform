R"zzz(
#version 410 core
uniform mat4 projection;
uniform mat4 view;
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 world_position;
in vec4 localPos;
layout(location = 0) out vec4 fragment_color;
layout(location = 1) out vec4 ss_Normal;
layout(location = 2) out vec4 world_Pos;
void main() {
	vec4 pos = world_position;
	vec3 color = vec3(1.0, 0.0, 0.0);
	float dot_nl = dot(normalize(light_direction), normalize(face_normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	color = clamp(dot_nl * color, 0.0, 1.0);
	fragment_color = vec4(color, 1.0);
	ss_Normal = projection * view * normalize(face_normal);
	world_Pos = world_position;
}
)zzz"
