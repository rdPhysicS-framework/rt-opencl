typedef struct __attribute__((package))
{
	RT_Vec3f position;
	RT_Vec3f color;
	float ls;
} RT_Light;

typedef struct __attribute__((package))
{
	RT_Vec3f color;
	float ambient;//RT_Lambertian
	float diffuse;//RT_Lambertian
	float specular;//RT_GlossySpecular
	float intensity;
} RT_Material;

typedef struct __attribute__((package))
{
	RT_Vec3f center;
	float radius;

	RT_Material material;
} RT_Sphere;

typedef struct __attribute__((package))
{
	RT_Vec3f point;
	RT_Vec3f normal;
	//float dist;

	RT_Material material;
} RT_Plane;

typedef struct __attribute__((package))
{
	RT_Vec3f position;
	RT_Vec3f size;

	RT_Material material;
} RT_Box;

typedef struct __attribute__((package))
{
	bool hit;
	float t;
	RT_Vec3f lhitPoint;
	RT_Vec3f normal;
	//RT_Vec3f color;
	RT_Material material;
	RT_Ray ray;
	int depth;
	RT_Vec3f dir;
} RT_Result;

typedef struct __attribute__((package))
{
	int width;
	int height;
	RT_Vec2f sp;
	//RT_SScoord coord;
} RT_ViewPlane;

 typedef struct __attribute__((package))
{ 
	RT_Vec3f u;
	RT_Vec3f v;
	RT_Vec3f w;
} UVW;

typedef struct __attribute__((package))
{ 
	/*position*/
	RT_Vec3f eye;
	RT_Vec3f lookAt;
	RT_Vec3f up;

	float viewPlaneDistance;
	float zoom;

	float rollAngle;
	float exposureTime;

	RT_Vec3f u, v, w;
} RT_Camera;

typedef struct __attribute__((package))
{ 
	RT_Vec3f color;
	float ls;
} RT_AmbientLight;

typedef struct __attribute__((package))
{ 
	/*data of the lights*/
	int numLights;

	/*data of the objects*/
	int numSpheres;
	int numPlanes;
	int numBox;

	/*data of the samples*/
	int numSamples;
	int numSets;
	int jump;
	ulong count;
	ulong numShuffledIndices;
	/*seed random*/
	ulong seed;
} RT_DataScene;
