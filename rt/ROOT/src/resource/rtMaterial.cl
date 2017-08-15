typedef struct
{
	RT_Vec3f color;
	float ambient;//RT_Lambertian
	float diffuse;//RT_Lambertian
	float specular;//RT_GlossySpecular
	float intensity;
} RT_Material;

RT_Material CreateBasicMaterial(const RT_Vec3f c, 
								const float a, 
								const float d, 
								const float s,
								const float i)
{
	RT_Material m;
	m.color = c;
	m.ambient = a;
	m.diffuse = d;
	m.specular = s;
	m.intensity = i;
	return m;
}

