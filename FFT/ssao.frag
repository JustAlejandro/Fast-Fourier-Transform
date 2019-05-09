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
	fragment_color = vec4(texture(depSten, tex_coord).x, texture(depSten, tex_coord).x, texture(depSten, tex_coord).x, 1.0);
	float ao = 0.0;
	vec4 start = vec4(texture(vs_Ray, tex_coord).xyz, 1.0);

	for(float i = 1; i <= 4; i++){
		for(float j = 1; j <= 4; j++){
			vec4 check = start + vec4(texture(noise,vec2(i / 4.0,j /4.0)).xyz, 0.0);
			if(dot(texture(vs_Normals, tex_coord).xyz, normalize(check.xyz)) < 0.0) ;//continue;
			check = projection * check;
			check = toTextureSpace(check / check.w);
			fragment_color = check;
			if(check.z < texture(depSten, vec2(check.x,check.y)).x || abs(check.z - texture(depSten, vec2(check.x,check.y)).x) > 0.01) ao += 1.0/8.0;
		}
	}
	fragment_color = vec4(ao,ao,ao,1.0);
}
)zzz"
