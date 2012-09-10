uniform sampler2D ShadowMap;

varying vec4 ShadowCoord;
varying vec4 direct_color;
varying vec4 indirect_color;

//varying vec4 debugOutput;

void main( ){
	
	//Shadow Mapping
	vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
	
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0001;
	
	float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z;
	
	float shadow = 1.0;
 	if (ShadowCoord.w >= 0.0)
 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.0 : 1.0 ;

	
  	gl_FragColor =	 (direct_color*shadow) + indirect_color;
  	
  	//Test for Correct Normals
  	//gl_FragColor = vec4(abs(normalized_normal),1);
  	
  	//Debug Purposes
  	//gl_FragColor = debugOutput;
}
