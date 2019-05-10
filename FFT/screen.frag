R"zzz(#version 410 core
out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2D sampler;
uniform sampler2D depSten;
uniform sampler2D SSR;
uniform int ssr;

void main() {
	if(ssr == 0){
		fragment_color = vec4(texture(sampler, tex_coord).xyz, 1.0);
	}
	else{
		fragment_color = vec4(texture(sampler, tex_coord).xyz + texture(SSR, tex_coord).xyz, 1.0);
	}
}
)zzz"
