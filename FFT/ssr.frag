R"zzz(#version 410 core
out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2D sampler;
uniform sampler2D depSten;
uniform sampler2D specular;
void main() {
	fragment_color = vec4(texture(sampler, tex_coord).xyz, 1.0);
	//fragment_color = vec4(texture(depSten, tex_coord).x,texture(depSten, tex_coord).x,texture(depSten, tex_coord).x,1.0);
	//fragment_color = texture(specular, tex_coord);
}
)zzz"
