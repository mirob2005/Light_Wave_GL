void main( ){
	// Transforming The Vertex (both are equivalent)
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	// foreach vertex v
	// gl_Position[v] = gl_ModelViewProjectionMatrix * gl_Vertex[v]
	// Or
	//gl_Position = ftransform( );
}
