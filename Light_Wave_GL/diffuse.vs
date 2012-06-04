varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec3 light_to_vertex_vector;
varying vec3 lightDir;

void main( ){
	gl_FrontColor = gl_Color;
	gl_BackColor = gl_Color;
	// Transforming The Vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	// Transforming The Normal To ModelView-Space
	normal = gl_NormalMatrix * gl_Normal; 
	
	//Light Direction
	lightDir = normalize(vec3(gl_LightSource[0].position));

	// Transforming The Vertex Position To ModelView-Space
	vec4 vertex_in_modelview_space = gl_ModelViewMatrix * gl_Vertex;

	// Calculating The Vector From The Vertex Position To The Light Position
	vertex_to_light_vector = vec3(gl_LightSource[0].position - vertex_in_modelview_space);
	
	light_to_vertex_vector = vec3(vertex_in_modelview_space - gl_LightSource[0].position);
}
