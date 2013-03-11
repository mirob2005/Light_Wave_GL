#version 120

//Number of indirect shadow maps
const int indSMs = 5;

uniform sampler1D vplPosTex;
uniform sampler1D vplNorTex;
uniform mat4x4 LightTexture[indSMs];

varying vec3 lightDir, lightDirRef, camDir, normal;
varying vec3 INDlightDir, INDlightDirRef;
varying float INDatt;

varying vec4 ShadowCoord;
varying vec4 INDShadowCoord[indSMs];



void main( ){
	gl_FrontColor = gl_Color;
	gl_BackColor = gl_Color;

/*
 *  Primary Lighting Calculations:
 */
	
	//Access Primary Light Position, Normal from Texture 5
	//Use this to extract rows from the matrix (or column as it appears in .cpp):
	//(1.0,0.0,0.0,0.0) = first row
	//(0.0,1.0,0.0,0.0) = 2nd row
	//(0.0,0.0,1.0,0.0) = 3rd row
	//(0.0,0.0,0.0,1.0) = last row
	vec4 extractRow1 = vec4(1.0,0.0,0.0,0.0);
	vec4 extractRow2 = vec4(0.0,1.0,0.0,0.0);
	vec4 extractRow3 = vec4(0.0,0.0,1.0,0.0);
	vec4 extractRow4 = vec4(0.0,0.0,0.0,1.0);
	
	vec4 masterLightPosition = extractRow1*gl_TextureMatrix[5];
	vec4 masterLightNormal = extractRow2*gl_TextureMatrix[5];
	vec4 lightProperties = extractRow3*gl_TextureMatrix[5];
	vec4 cameraPosition = extractRow4*gl_TextureMatrix[5];
	
		
	lightDir = vec3(masterLightPosition) - gl_Vertex.xyz;
	lightDirRef = reflect(-lightDir, gl_Normal);
	camDir = vec3(cameraPosition) - gl_Vertex.xyz;
	
	normal = gl_Normal;
	vec3 normalized_normal = normalize(normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

/*
 *	VPL Indirect Lighting Calculations
 */
    int numLights = int(lightProperties.z);
	float maxDistance = 4.0;

	vec3 indLightDirection = vec3(0.0,0.0,0.0);
	vec3 indLightDirectionReflected = vec3(0.0,0.0,0.0);
	float attenuation = 0.0;
	
	for(int i=1; i< numLights+1; i++)
	{
	  //Calculate Coordinate for VPL textures
	   float texCoord = (float(i)/numLights);

	   vec3 vplPosition = texture1D(vplPosTex,texCoord).rgb;
	   //vec3 vplNormal = texture1D(vplNorTex,texCoord).rgb;
	   float vplAttenuation = texture1D(vplNorTex,texCoord).a;
       
       //Decode VPL texture into original data
	   vplPosition = (vplPosition-0.5)*maxDistance*4.0;
	   //vplNormal= (vplNormal-0.5)*maxDistance*4.0;
	   vplAttenuation= (vplAttenuation-0.5)*maxDistance*4.0;
       
	   //vec3 normalized_vplNormal = normalize(vplNormal);


	   // Calculating The Vector From The Vertex Position To The Light Position and vice versa
	   vec3 tempLightDirection = vec3(vplPosition - vec3(gl_Vertex));
	   vec3 normalizedLightdir = normalize(tempLightDirection);
	   float distance = length(tempLightDirection);
	   indLightDirection += tempLightDirection;
	   indLightDirectionReflected += reflect(-normalizedLightdir, gl_Normal);
	   attenuation += pow((1-vplAttenuation),distance);
   }
   
   INDlightDir = indLightDirection/float(numLights);
   INDlightDirRef = indLightDirectionReflected/float(numLights);
   INDatt = attenuation/float(numLights);

   //Shadow Map Coordinate Calculations
   for(int i = 0; i < indSMs; i++) 
   {
	  INDShadowCoord[i] = LightTexture[i] * gl_Vertex;
   }

   ShadowCoord= gl_TextureMatrix[7] * gl_Vertex;	
}