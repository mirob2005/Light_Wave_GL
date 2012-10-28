#version 120

#extension GL_EXT_texture_array : enable

uniform sampler2DArrayShadow ShadowMap;

varying vec3 lightDir, lightDirRef, camDir, normal;

varying vec4 ShadowCoord;
varying vec4 INDShadowCoord[20];
varying vec4 indirect_color;

void main( ){
	
	//DIRECT Shadow Mapping
	float shadow = 0.0;
	shadow += shadow2DArray(ShadowMap, vec4(ShadowCoord.xy / ShadowCoord.w, 0, ShadowCoord.z / ShadowCoord.w)).r;

	//INDIRECT Shadow Mapping	
	float INDshadow = 0.0;
	for(int i = 0; i < 20; i++)
	{
		INDshadow += shadow2DArray(ShadowMap, vec4(INDShadowCoord[i].xy / INDShadowCoord[i].w, i+1, INDShadowCoord[i].z / INDShadowCoord[i].w)).r;
	}
	
	INDshadow /= 10.0;	
	
/*
 *	Primary Lighting Calculations
 */
 
    // DIFFUSE
	float DiffuseTerm = max(dot(normalize(lightDir), normal), 0.0);
	
	//SPECULAR
	float SpecularTerm = pow(max(dot(normalize(lightDirRef), normalize(camDir)), 0.0), 32.0);
	
	vec4 direct_color = vec4(vec3(gl_Color) * (DiffuseTerm * 0.75 + SpecularTerm), 1.0);
	
 
/////////////////////////////////////////////////////////////////
	//gl_FragColor = direct_color;
	//gl_FragColor =	 (direct_color*shadow);
	//gl_FragColor = indirect_color;
	//gl_FragColor = (indirect_color*INDshadow);
	//gl_FragColor =	 (direct_color*shadow) + (indirect_color);
    gl_FragColor =	 (direct_color*shadow) + (indirect_color*INDshadow);
    //gl_FragColor =	 (direct_color*0.5*shadow) + (direct_color*0.5) + (0.2*indirect_color) + (0.8*indirect_color*INDshadow);
}
