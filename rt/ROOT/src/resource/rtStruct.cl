typedef struct
{
	RT_Vec3f position;
	RT_Vec3f color;
	float ls;
} RT_Light;

typedef struct
{
	RT_Vec3f color;
	float ambient;//RT_Lambertian
	float diffuse;//RT_Lambertian
	float specular;//RT_GlossySpecular
	float intensity;
} RT_Material;

typedef struct
{
	RT_Vec3f center;
	float radius;

	RT_Material material;
} RT_Sphere;

typedef struct
{
	RT_Vec3f normal;
	float dist;
} RT_Plane;

typedef struct
{
	RT_Vec3f position;
	RT_Vec3f size;
} RT_Box;

typedef struct 
{
	bool hit;
	float t;
	RT_Vec3f lhitPoint;
	RT_Vec3f normal;
	//RT_Vec3f color;
	RT_Material material;
	RT_Ray ray;
	int depth;
} RT_Result;

typedef struct
{
	int width;
	int height;
	RT_Vec2f sp;
	//RT_SScoord coord;
} RT_ViewPlane;