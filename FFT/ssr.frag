R"zzz(#version 410 core
layout(location = 0) out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2D sampler;
uniform sampler2D depSten;
uniform sampler2D specular;
uniform sampler2D vs_Normals;
uniform sampler2D vs_Ray;

uniform float aspect;
uniform mat4 projection;

vec4 toTextureSpace(vec4 r){
	r.x = (r.x + 1.0) / 2.0;
	//Aspect ration correction
	r.y = ((r.y + 1.0) / 2.0) * aspect;
	r.z = (r.z + 1.0) / 2.0;
	return r;
}

//Here's where we'll trace our SS ray
vec4 traceRay(){
	//Start position of the ray
	vec3 normal = texture(vs_Normals,tex_coord).xyz;

	//Start postion in view Space
	vec3 viewRay = (texture(vs_Ray,tex_coord)).xyz;
	vec4 start = vec4((viewRay).xyz,1.0);

	//Reflected view space Ray
	vec3 vsRefl = reflect(normalize(viewRay.xyz), normalize(normal));

	//End Postition in view Space
	vec4 end = start + vec4(vsRefl * 1000000, 1.0);

	//Start Position in texture Space;
	start = (projection * start);
	start = toTextureSpace(start / start.w);

	//End Position in texture Space
	end = (projection * end);
	end = toTextureSpace(end / end.w);

	vec4 tsRefl = vec4(normalize((end - start).xyz),0.0);
	vec4 pos = start + tsRefl * 0.005;

	bool hit = false;
	int step = 0;
	while(!hit && pos.x >= 0.0 && (pos.x) < 1.0 && pos.y >= 0.0 && (pos.y) < 1.0 && pos.z < 1.0 && pos.z > 0.0 && step < 4000){
		step++;
		//Found a problem where since the depth values aren't linear reflections at a distance wouldn't work. So now we depend on
		//	reflected ray's depth value for hit detection.
		if(abs(pos.z - (texture(depSten, vec2(pos.x, pos.y))).x) < tsRefl.z/2000 && texture(depSten, vec2(pos.x,pos.y)).x < 0.999){
			hit = true;
			return texture(sampler, vec2(pos.x, pos.y));
		}
		pos += tsRefl * 0.0005;
	}
	return vec4(-1.0,-1.0,-1.0,-1.0);
}

void main() {
	//Don't run reflections if specular not specified
	if(texture(specular,tex_coord).x <= 0.01){
		fragment_color = vec4(0.0,0.0,0.0,1.0);//vec4(texture(sampler, tex_coord).xyz, 1.0);
		return;
	}
	//Reflection running will be put in here.
	vec4 rayCol = traceRay();
	if(rayCol.w > -0.999){
		fragment_color = rayCol * texture(specular, tex_coord).x;// + vec4(texture(sampler,tex_coord).xyz, 0.0);
	}
	else{
		fragment_color = vec4(0.0,0.0,0.0,1.0);//fragment_color = vec4(texture(sampler, tex_coord).xyz, 1.0);
	}
}
)zzz"
