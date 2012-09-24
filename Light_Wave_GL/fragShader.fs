#version 120

uniform sampler2D ShadowMap;

varying vec3 normalized_normal,normalized_vertex_to_light_vector,lightDir,normalized_light_to_vertex_vector;
varying vec3 normalized_viewing_position_vector;

varying vec4 ShadowCoord;
varying vec4 indirect_color;

void main( ){
	
	//Shadow Mapping
	vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
	
	// Used to lower moir� pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0001;
	
	float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z;
	
	float shadow = 1.0;
 	if (ShadowCoord.w >= 0.0)
 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.0 : 1.0 ;

		
	
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
	  direct_color += gl_Color*pow(SpecularTerm,5);	
	}
/////////////////////////////////////////////////////////////////
	
	//gl_FragColor =	 direct_color;
	//gl_FragColor =	 (direct_color*shadow);
	//gl_FragColor = indirect_color;
  	gl_FragColor =	 (direct_color*shadow) + indirect_color;
  	
  	//Test for Correct Normals
  	//gl_FragColor = vec4(abs(normalized_normal),1);
  	
  	//Debug Purposes
  	//gl_FragColor = debugOutput;
}
