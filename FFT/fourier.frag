R"zzz(
#version 410 core
uniform mat4 projection;
uniform mat4 view;
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction;
in vec4 world_position;
in vec4 localPos;
in vec4 camera_direction;
uniform float specular;
layout(location = 0) out vec4 fragment_color;
layout(location = 1) out vec4 vs_Normal;
layout(location = 2) out vec4 world_Pos;
layout(location = 3) out vec4 specs;
layout(location = 4) out vec4 vs_Ray;
void main() {
	vec4 pos = world_position;
	vec3 color = vec3(1.0, 0.0, 0.0);
	float dot_nl = dot(normalize(light_direction), normalize(face_normal));
	dot_nl = clamp(dot_nl + 0.1, 0.0, 1.0);
	color = clamp(dot_nl * color, 0.0, 1.0);
	fragment_color = vec4(color, 1.0);
	vs_Normal = normalize(vec4((normalize(face_normal)).xyz, 0.0));
	world_Pos = world_position;
	specs = vec4(specular,specular,specular,1.0);
	vs_Ray = view * world_position;
}
)zzz"
