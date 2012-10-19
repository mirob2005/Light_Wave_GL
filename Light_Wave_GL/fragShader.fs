#version 120

#extension GL_EXT_texture_array : enable

uniform sampler2DArrayShadow ShadowMap;

varying vec3 normalized_normal,normalized_vertex_to_light_vector,lightDir,normalized_light_to_vertex_vector;
varying vec3 normalized_viewing_position_vector;

varying vec4 ShadowCoord;
varying vec4 INDShadowCoord[20];
varying vec4 indirect_color;

void main( ){
	
	//Shadow Mapping
	float shadow = 0.0;
	shadow += shadow2DArray(ShadowMap, vec4(ShadowCoord.xy / ShadowCoord.w, 0, ShadowCoord.z / ShadowCoord.w)).r;

	//INDIRECT Shadow Mapping	
	float INDshadow = 0.0;
	for(int i = 0; i < 20; i++)
	{
		INDshadow += shadow2DArray(ShadowMap, vec4(INDShadowCoord[i].xy / INDShadowCoord[i].w, i+1, INDShadowCoord[i].z / INDShadowCoord[i].w)).r;
	}
	
	INDshadow /= 20.0;	
	
/*
 *	Primary Lighting Calculations
 */
 
    // DIFFUSE
    vec4 direct_color = vec4(0.0,0.0,0.0,1.0);
	 
	vec3 n = normalize(normalized_normal);
	vec3 VtL = normalize(normalized_vertex_to_light_vector);
	
	vec3 ldir = normalize(lightDir);
	vec3 LtV = normalize(normalized_light_to_vertex_vector);
	
  	// Reflection term of object
	float DiffuseTermObj = clamp(dot(n, VtL), 0.0, 1.0);
	
	//Reflection term of directional light
	float DiffuseTermLight = clamp(dot(ldir, LtV),0.0,1.0);
	
	// Calculating The Color from the primary light
	direct_color += gl_Color*DiffuseTermLight*DiffuseTermObj;
	
	
	//SPECULAR
	vec3 viewVec = normalize(normalized_viewing_position_vector);
	float SpecularTerm = 0.0;
	float normal_dot_light = dot(n,VtL);
	if(normal_dot_light > 0.0)
	{
	  vec3 Reflection = vec3(2.0*vec3(n)*normal_dot_light)-VtL;
	  SpecularTerm = dot(Reflection, viewVec);
	}	
	if(SpecularTerm > 0.0)
	{
	  direct_color += gl_Color*0.5*pow(SpecularTerm,10);	
	}
/////////////////////////////////////////////////////////////////
	
	//gl_FragColor =	 direct_color;
	//gl_FragColor =	 (direct_color*shadow);
	//gl_FragColor = indirect_color;
	//gl_FragColor = indirect_color*INDshadow;
	//gl_FragColor =	 (direct_color*shadow) + (indirect_color);
    gl_FragColor =	 (direct_color*shadow) + (indirect_color*INDshadow);
    //gl_FragColor =	 (direct_color*0.5*shadow) + (direct_color*0.5) + (0.2*indirect_color) + (0.8*indirect_color*INDshadow);
  	
  	//Test for Correct Normals
  	//gl_FragColor = vec4(abs(normalized_normal),1);
  	
  	//Debug Purposes
  	//gl_FragColor = vec4(1,1,1,1);
}
