R"zzz(#version 410 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec4 light_position[20];
uniform int light_count;
in vec4 vs_camera_direction[];
in vec4 vs_normal[];
in vec2 vs_uv[];
out vec4 light_direction[20];
out vec4 camera_direction;
out vec4 world_position;
out vec4 vertex_normal;
out vec2 uv_coords;
void main() {
	int n = 0;
	for (n = 0; n < gl_in.length(); n++) {
		for(int i = 0; i < light_count; i++){
			light_direction[i] = normalize(view * normalize(light_position[i] - gl_in[n].gl_Position));
		}
		camera_direction = (vs_camera_direction[n]);
		world_position = gl_in[n].gl_Position;
		vertex_normal = view * vs_normal[n];
		uv_coords = vs_uv[n];
		gl_Position = projection * view * gl_in[n].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
)zzz"
