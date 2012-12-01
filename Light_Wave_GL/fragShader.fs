#version 120

#extension GL_EXT_texture_array : enable

uniform sampler2DArrayShadow ShadowMap;

varying vec3 lightDir, lightDirRef, camDir, normal;

varying vec4 ShadowCoord;
varying vec4 INDShadowCoord[5];
varying vec4 indirect_color;

int numSteps = 2;

//INDIRECT Shadow Mapping
float calcINDshadow(int vpl1, int vpl2){
	
	float INDshadow = 0.0;

	vec4 tempCoord;
	
	
	vec4 step = vec4(INDShadowCoord[vpl2]-INDShadowCoord[vpl1])/float(numSteps);
	
	//from low side
	INDshadow += shadow2DArray(ShadowMap, vec4(INDShadowCoord[vpl1].xy / INDShadowCoord[vpl1].w, vpl1+1, INDShadowCoord[vpl1].z / INDShadowCoord[vpl1].w)).r;
	for(int i =0; i<(numSteps/2); i++)
	{
		tempCoord = vec4(INDShadowCoord[vpl1]+vec4(step*(i+1)));
		INDshadow += shadow2DArray(ShadowMap, vec4(tempCoord.xy / tempCoord.w, vpl1+1, tempCoord.z / tempCoord.w)).r;		
	}		
	
	//from high side
	INDshadow += shadow2DArray(ShadowMap, vec4(INDShadowCoord[vpl2].xy / INDShadowCoord[vpl2].w, vpl2+1, INDShadowCoord[vpl2].z / INDShadowCoord[vpl2].w)).r;
	for(int i = (numSteps/2); i<numSteps; i++)
	{
		tempCoord = vec4(INDShadowCoord[vpl1]+vec4(step*(i)));
		INDshadow += shadow2DArray(ShadowMap, vec4(tempCoord.xy / tempCoord.w, vpl2+1, tempCoord.z / tempCoord.w)).r;						
	}	   
   return INDshadow;
}



void main( ){
	
	//DIRECT Shadow Mapping
	float shadow = 0.0;
	//Adding 0.0005 removes the bright artifacts on the edges of the object before the shadows
	shadow += shadow2DArray(ShadowMap, vec4(ShadowCoord.xy / ShadowCoord.w, 0, (ShadowCoord.z + 0.0005)/ ShadowCoord.w)).r;
	
	//INDIRECT SHADOW MAPPING
	float INDshadow = 0.0;
	
	INDshadow += calcINDshadow(0,1);
	INDshadow += calcINDshadow(0,2);
	INDshadow += calcINDshadow(0,3);
	INDshadow += calcINDshadow(0,4);
	INDshadow += calcINDshadow(1,2);
	INDshadow += calcINDshadow(1,3);
	INDshadow += calcINDshadow(1,4);
	INDshadow += calcINDshadow(2,3);
	INDshadow += calcINDshadow(2,4);
	INDshadow += calcINDshadow(3,4);
	
	INDshadow /= ((numSteps+2)*10.0);	
	
/*
 *	Primary Lighting Calculations
 */
 
    // DIFFUSE
	float DiffuseTerm = max(dot(normalize(lightDir), normal), 0.0);
	
	//SPECULAR
	float SpecularTerm = 0.0;
	//Get rid of the specular highlights in the shadows (should be obscured)
	if(shadow > 0.1){
	  SpecularTerm = pow(max(dot(normalize(lightDirRef), normalize(camDir)), 0.0), 32.0);
	}
	
	vec4 direct_color = vec4(vec3(gl_Color) * (DiffuseTerm * 0.75 + SpecularTerm), 1.0);
	
 
/////////////////////////////////////////////////////////////////
	//gl_FragColor = direct_color;
	//gl_FragColor =	 (direct_color*shadow);
	//gl_FragColor = indirect_color;
	//gl_FragColor = (indirect_color*INDshadow);
	//gl_FragColor =	 (direct_color*shadow) + (indirect_color);
    //gl_FragColor =	 (direct_color*shadow) + (indirect_color*INDshadow);
    gl_FragColor =	 (direct_color*0.5*shadow) + (direct_color*0.5) +(indirect_color*INDshadow);
}

