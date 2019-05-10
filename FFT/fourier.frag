R"zzz(
#version 410 core
uniform mat4 projection;
uniform mat4 view;
uniform int light_count;
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction[20];
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
	float dot_nl = 0.0f;
	for(int i = 0; i < light_count; i++){
		dot_nl += abs(dot(normalize(light_direction[i].xyz), (face_normal.xyz)));
	}
	color = clamp(dot_nl * color, 0.0, 1.0);
	fragment_color = vec4(color, 1.0);
	vs_Normal = vec4((normalize(face_normal)).xyz, 0.0);
	world_Pos = world_position;
	specs = vec4(specular,specular,specular,1.0);
	vs_Ray = view * world_position;
}
)zzz"
