#version 120

#extension GL_EXT_texture_array : enable

//Number of indirect shadow maps
const int indSMs = 5;
//Number of interpolation steps between each indirect shadow map
const int numSteps = 6;

uniform sampler2DArrayShadow ShadowMap;

varying vec3 lightDir, lightDirRef, camDir, normal;
varying vec3 INDlightDir, INDlightDirRef;
varying float INDatt;

varying vec4 ShadowCoord;
varying vec4 INDShadowCoord[indSMs];




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
	shadow += shadow2DArray(ShadowMap, vec4(ShadowCoord.xy / ShadowCoord.w, 0, (ShadowCoord.z)/ ShadowCoord.w)).r;
	
	//INDIRECT SHADOW MAPPING
	float INDshadow = 0.0;
	
	int combinations = 0;
	for(int i = 0; i < indSMs; i++){
	  for(int j = i+1; j<indSMs; j++){
	    INDshadow += calcINDshadow(i,j);
	    combinations++;
	  }
	}
	INDshadow /= ((numSteps+2)*float(combinations));
	
/*
 *	Primary Lighting Calculations
 */
 
    // DIFFUSE
	float DiffuseTerm = max(dot(normalize(lightDir), normal), 0.0);
	
	//SPECULAR
	float SpecularTerm = 0.0;
	//Get rid of the specular highlights in the shadows (should be obscured)
	if(shadow > 0.1 && DiffuseTerm > 0.1){
	  SpecularTerm = pow(max(dot(normalize(lightDirRef), normalize(camDir)), 0.0), 32.0);
	}
	
	vec4 direct_color = vec4(vec3(gl_Color) * (DiffuseTerm * 0.75) + (SpecularTerm*0.25), 1.0);
	
	float INDDiffuseTerm = max(dot(normalize(INDlightDir), normal), 0.0);
	float INDSpecularTerm = 0.0;
	if(INDshadow > 0.1){
	  INDSpecularTerm = pow(max(dot(normalize(INDlightDirRef), normalize(camDir)), 0.0), 32.0);
	}
	
	vec4 INDCOLOR = vec4(vec3(gl_Color) * (INDDiffuseTerm + SpecularTerm), 1.0);
	
	if(INDlightDir.y < 0.0){
	  INDshadow = 1.0;
	}
	
	if(shadow <1.0){
	  INDCOLOR = INDCOLOR*1.5;
	  INDshadow = INDshadow+(1.0 - shadow);
	}

    gl_FragColor =	 (direct_color*shadow) +(INDCOLOR*INDshadow*INDatt);
}
