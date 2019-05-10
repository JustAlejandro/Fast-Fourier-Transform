R"zzz(
#version 410 core
uniform mat4 projection;
uniform mat4 view;
uniform int light_count;
in vec4 face_normal;
in vec4 vertex_normal;
in vec4 light_direction[20];
in vec4 world_position;
in vec4 camera_direction;
uniform float specular;
uniform float time;
layout(location = 0) out vec4 fragment_color;
layout(location = 1) out vec4 vs_Normal;
layout(location = 2) out vec4 world_Pos;
layout(location = 3) out vec4 specs;
layout(location = 4) out vec4 vs_Ray;

void main() {
	vec4 pos = world_position;
	float check_width = 5.0;
	float i = floor((pos.x + time * 0.01) / check_width);
	float j  = floor(pos.z / check_width);
	vec3 color = mod(i + j, 2) * vec3(1.0, 1.0, 1.0);
	vec4 norm = normalize(view * normalize(face_normal));
	float dot_nl = 0.0f;
	for(int i = 0; i < light_count; i++){
		dot_nl += abs(dot(normalize(light_direction[i].xyz), (norm.xyz)));
	}
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	color = clamp(dot_nl * color, 0.0, 1.0);
	fragment_color = vec4(color, 1.0);
	vs_Normal = vec4(norm.xyz, 0.0);
	world_Pos = world_position;
	specs = vec4(specular,specular,specular,1.0);
	vs_Ray = (view * world_position);
}
)zzz"
