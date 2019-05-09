R"zzz(#version 410 core
layout(location = 0) out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2D depSten;
uniform sampler2D vs_Normals;
uniform sampler2D vs_Ray;
uniform sampler2D noise;

uniform float aspect;
uniform mat4 projection;

vec4 toTextureSpace(vec4 r){
	r.x = (r.x + 1.0) / 2.0;
	//Aspect ration correction
	r.y = ((r.y + 1.0) / 2.0) * aspect;
	r.z = (r.z + 1.0) / 2.0;
	return r;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	float ao = 0.0;
	vec4 start = vec4(texture(vs_Ray, tex_coord).xyz, 1.0);

	for(float i = 0; i < 12; i++){
		for(float j = 0; j < 12; j++){
			vec4 check = start + vec4(texture(noise,vec2(i / 12.0,j /12.0)).xyz, 0.0) / (i / 2.0 + 5.0);
			check = projection * check;
			check = toTextureSpace(check / check.w);
			fragment_color = check;
			if(check.z <= texture(depSten, vec2(check.x,check.y)).x) ao += 1.0/72.0;
		}
	}
	ao = min(ao + 0.1, 1.0);
	fragment_color = vec4(ao,ao,ao,1.0);
}
)zzz"
