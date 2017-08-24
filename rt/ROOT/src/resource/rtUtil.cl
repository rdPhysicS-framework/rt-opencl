#define ANGLE 180.0

#define PI 3.14159265358979323846
#define PI2 (PI * 2)
#define INVPI 1.0 / PI

#define RANDMAX 0x7FFF

#define INFINITE 1000000000.0

#define EPSILON 0.0001

#define RMASK 0x000000FF
#define GMASK 0x0000FF00
#define BMASK 0x00FF0000
#define AMASK 0xFF000000

#define RSHIFT 0
#define GSHIFT 8
#define BSHIFT 16
#define ASHIFT 24

#define SIZE_MATRIX 4

#define TO_DEG (ANGLE/PI)
#define TO_RAD  (PI/ANGLE)

static inline float toRadians(const float degrees)
{
	return (float)(degrees * TO_RAD);
}

static inline float toDegrees(const float radians)
{
	return (float)(radians * TO_DEG);
}

/*----------------------------------------------------------------------------------------------
 *
 * Math
 *
 *----------------------------------------------------------------------------------------------*/
typedef float2 RT_Vec2f;
typedef float3 RT_Vec3f;
typedef float4 RT_Vec4f;

typedef struct
{
	float m[4][4];
} RT_Mat4f;

/*----------------------------------------------------------------------------------------------
 *
 *Methods of matrix 4f*
 *
 *----------------------------------------------------------------------------------------------*/

inline RT_Mat4f create(float a, float b, float c, float d,
						  float e, float f, float g, float h,
						  float k, float l, float m, float n,
						  float o, float p, float q, float r)
{
	RT_Mat4f mt;
	mt.m[0][0] = a; mt.m[0][1] = b; mt.m[0][2] = c; mt.m[0][3] = d;
	mt.m[1][0] = e; mt.m[1][1] = f; mt.m[1][2] = g; mt.m[1][3] = h;
	mt.m[2][0] = k; mt.m[2][1] = l; mt.m[2][2] = m; mt.m[2][3] = n;
	mt.m[3][0] = o; mt.m[3][1] = p; mt.m[3][2] = q; mt.m[3][3] = r;
	return mt;
}

inline RT_Mat4f identity()
{
	return create(1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
				  0, 0, 0, 1 );
}

inline RT_Mat4f null()
{
	return create(0, 0, 0, 0,
				  0, 0, 0, 0,
				  0, 0, 0, 0,
				  0, 0, 0, 0 );
}

RT_Mat4f copy(const RT_Mat4f *a)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j];

	return r;
}

inline RT_Mat4f transpose(const RT_Mat4f *m)
{
	RT_Mat4f mt;
	mt.m[0][0] = m->m[0][0]; mt.m[0][1] = m->m[1][0]; mt.m[0][2] = m->m[2][0]; mt.m[0][3] = m->m[3][0];
	mt.m[1][0] = m->m[0][1]; mt.m[1][1] = m->m[1][1]; mt.m[1][2] = m->m[2][1]; mt.m[1][3] = m->m[3][1];
	mt.m[2][0] = m->m[0][2]; mt.m[2][1] = m->m[1][2]; mt.m[2][2] = m->m[2][2]; mt.m[2][3] = m->m[3][2];
	mt.m[3][0] = m->m[0][3]; mt.m[3][1] = m->m[1][3]; mt.m[3][2] = m->m[2][3]; mt.m[3][3] = m->m[3][3];
	return mt;
}

inline float determinant(const RT_Mat4f *m)
{
	return m->m[0][0] * (m->m[1][1] * (m->m[2][2] * m->m[3][3] - m->m[2][3]   * m->m[3][2]) +
           m->m[1][2] * (m->m[2][3] *  m->m[3][1] - m->m[2][1] * m->m[3][3])  +
           m->m[1][3] * (m->m[2][1] *  m->m[3][2] - m->m[2][2] * m->m[3][1])) -

           m->m[0][1] * (m->m[1][0] * (m->m[2][2] * m->m[3][3] - m->m[2][3]   * m->m[3][2]) +
           m->m[1][2] * (m->m[2][3] *  m->m[3][0] - m->m[2][0] * m->m[3][3])  +
           m->m[1][3] * (m->m[2][0] *  m->m[3][2] - m->m[2][2] * m->m[3][0])) +
		   
           m->m[0][2] * (m->m[1][0] * (m->m[2][1] * m->m[3][3] - m->m[2][3]   * m->m[3][1]) +
           m->m[1][1] * (m->m[2][3] *  m->m[3][0] - m->m[2][0] * m->m[3][3])  +
           m->m[1][3] * (m->m[2][0] *  m->m[3][1] - m->m[2][1] * m->m[3][0])) -
		   
           m->m[0][3] * (m->m[1][0] * (m->m[2][1] * m->m[3][2] - m->m[2][2]   * m->m[3][1]) +
           m->m[1][1] * (m->m[2][2] *  m->m[3][0] - m->m[2][0] * m->m[3][2])  +
           m->m[1][2] * (m->m[2][0] *  m->m[3][1] - m->m[2][1] * m->m[3][0]));
}

/*inline RT_Mat4f inverse(const RT_Mat4f *m)
{
	RT_Mat4f inv;

	return m4_Create();
}*/

inline RT_Mat4f affTranslation(const RT_Vec3f *v)
{
	return create(1, 0, 0, v->x,
					 0, 1, 0, v->y,
					 0, 0, 1, v->z,
					 0, 0, 0, 1    );
}

inline RT_Mat4f affTranslationF(const float x, const float y, const float z)
{
	return create(1, 0, 0, x,
					 0, 1, 0, y,
					 0, 0, 1, z,
					 0, 0, 0, 1 );
}

inline RT_Mat4f affRotationX(const float a)
{
	float c = cos(a);
	float s = sin(a);

	return create(1, 0,  0, 0,
					 0, c, -s, 0,
					 0, s,  c, 0,
					 0, 0,  0, 1 );
}

inline RT_Mat4f affRotationY(const float a)
{
	float c = cos(a);
	float s = sin(a);

	return create( c, 0, s, 0,
					  0, 1, 0, 0,
					 -s, 0, c, 0,
					  0, 0, 0, 1 );
}

inline RT_Mat4f affRotationZ(const float a)
{
	float c = cos(a);
	float s = sin(a);
	
	return create(c, -s, 0, 0,
					 s,  c, 0, 0,
					 0,  0, 1, 0,
					 0,  0, 0, 1 );
}

inline RT_Mat4f affScaling(const float s)
{
	return create(s, 0, 0, 0,
					 0, s, 0, 0,
					 0, 0, s, 0,
					 0, 0, 0, 1 );
}

inline RT_Mat4f affScalingF(const float x, const float y, const float z)
{
	return create(x, 0, 0, 0,
				  0, y, 0, 0,
				  0, 0, z, 0,
				  0, 0, 0, 1 );
}

inline RT_Mat4f affScalingV(const RT_Vec3f *v)
{
	return affScalingF(v->x, v->y, v->z);
}

inline RT_Mat4f affInvTranslation(const RT_Vec3f *v)
{
	return affTranslationF(-v->x, -v->y, -v->z);
}

inline RT_Mat4f affInvTranslationF(const float x, const float y, const float z)
{
	return affTranslationF(-x, -y, -z);
}

inline RT_Mat4f affInvRotationX(const float a)
{
	float c = cos(a);
	float s = sin(a);

	return create(1,  0, 0, 0,
				  0,  c, s, 0,
				  0, -s, c, 0,
				  0,  0, 0, 1 );
}

inline RT_Mat4f affInvRotationY(const float a)
{
	float c = cos(a);
	float s = sin(a);

	return create( c, 0, -s, 0,
				   0, 1,  0, 0,
				   s, 0,  c, 0,
				   0, 0,  0, 1 );
}

inline RT_Mat4f affInvRotationZ(const float a)
{
	float c = cos(a);
	float s = sin(a);
	
	return create( c, s, 0, 0,
				  -s,  c, 0, 0,
				   0,  0, 1, 0,
				   0,  0, 0, 1 );
}

inline RT_Mat4f affInvScaling(const float s)
{
	return affScaling(1.0/s);
}

inline RT_Mat4f affInvScalingV(const RT_Vec3f *v)
{
	return affScalingF(1.0/v->x, 1.0/v->y, 1.0/v->z);
}


inline RT_Mat4f affInvScalingF(const float x, const float y, const float z)
{
	return affScalingF(1.0/x, 1.0/y, 1.0/z);
}


RT_Mat4f add(const RT_Mat4f *a, const RT_Mat4f *b)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] + b->m[i][j];

	return r;
}

RT_Mat4f addS(const RT_Mat4f *a, const float s)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] + s;

	return r;
}

RT_Mat4f sub(const RT_Mat4f *a, const RT_Mat4f *b)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] - b->m[i][j];

	return r;
}

RT_Mat4f subS(const RT_Mat4f *a, const float s)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] - s;

	return r;
}

RT_Mat4f mul(const RT_Mat4f *a, const RT_Mat4f *b)
{
	RT_Mat4f r = null();

	for(int c = 0; c < SIZE_MATRIX; c++)
		for(int i = 0; i < SIZE_MATRIX; i++)
			for(int j = 0; j < SIZE_MATRIX; j++)
				r.m[c][i] += a->m[c][j] * b->m[j][i];

	return r;
}

RT_Mat4f mulS(const RT_Mat4f *a, const float s)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] * s;

	return r;
}

RT_Mat4f div(const RT_Mat4f *a, const float s)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] / s;

	return r;
}

/*----------------------------------------------------------------------------------------------
 *
 * Transform Vector
 *
 *----------------------------------------------------------------------------------------------*/

inline RT_Vec3f TransformPoint(const RT_Mat4f *m, const RT_Vec3f *p)
{
	return (RT_Vec3f)(m->m[0][0] * p->x + m->m[0][1] * p->y + m->m[0][2] * p->z + m->m[0][3],
					  m->m[1][0] * p->x + m->m[1][1] * p->y + m->m[1][2] * p->z + m->m[1][3],
					  m->m[2][0] * p->x + m->m[2][1] * p->y + m->m[2][2] * p->z + m->m[2][3] );
}

inline RT_Vec3f TransformDirection(const RT_Mat4f *m, const RT_Vec3f *d)
{
	return (RT_Vec3f)(m->m[0][0] * d->x + m->m[0][1] * d->y + m->m[0][2] * d->z,
					  m->m[1][0] * d->x + m->m[1][1] * d->y + m->m[1][2] * d->z,
					  m->m[2][0] * d->x + m->m[2][1] * d->y + m->m[2][2] * d->z );
}

inline RT_Vec3f TransformNormal(const RT_Mat4f *m, const RT_Vec3f *n)
{
	RT_Vec3f r = (RT_Vec3f)(m->m[0][0] * n->x + m->m[1][0] * n->y + m->m[2][0] * n->z,
						    m->m[0][1] * n->x + m->m[1][1] * n->y + m->m[2][1] * n->z,
						    m->m[0][2] * n->x + m->m[1][2] * n->y + m->m[2][2] * n->z );
	return normalize(r);
				   
}

inline RT_Vec3f reflect(const RT_Vec3f *v, const RT_Vec3f *n)
{
	float d = dot(*v, *n);

	return (RT_Vec3f)(v->x - 2.0f * d * n->x,
					  v->y - 2.0f * d * n->y,
					  v->z - 2.0f * d * n->z );
}

inline RT_Vec3f refract(const RT_Vec3f *v, const RT_Vec3f *n, const float i)
{
	float d = dot(*v, *n);
	float k = 1.0 - pow(i, 2) * (1.0 - pow(d, 2));

	if(k < 0.0)
		return *v;

	float sr = sqrt(k);

	return (RT_Vec3f)(i * v->x - n->x * (i * d + sr),
					  i * v->y - n->y * (i * d + sr),
					  i * v->z - n->z * (i * d + sr) );
}

inline RT_Vec3f lerp(const RT_Vec3f *a, const RT_Vec3f *b, const float t)
{
	return (RT_Vec3f)(a->x + (b->x - a->x) * t,
					  a->y + (b->y - a->y) * t,
					  a->z + (b->z - a->z) * t );
}

inline void swap(RT_Vec3f *a, const RT_Vec3f *b)
{
	a->x = b->x;
	a->y = b->y;
	a->z = b->z;
}

inline RT_Vec3f _clamp(const RT_Vec3f *v, const float min, const float max)
{
	return (RT_Vec3f)((v->x < min)? min : (v->x > max)? max : v->x,
					  (v->y < min)? min : (v->y > max)? max : v->y,
					  (v->z < min)? min : (v->z > max)? max : v->z );	
}

/*----------------------------------------------------------------------------------------------
 *
 * Color
 *
 *----------------------------------------------------------------------------------------------*/

 typedef struct
 {
	float rf, gf, bf, af;
	int ri, gi, bi, ai;
	int rgba;

 } RT_Color;

 inline RT_Color CreatePixelColori(int r, int g, int b, int a)
 {
	RT_Color c;
	c.ri = r;
	c.gi = g;
	c.bi = b;
	c.ai = a;

	c.rf = r/255.f;
	c.gf = g/255.f;
	c.bf = b/255.f;
	c.af = a/255.f;

	c.rgba = (((r & 0xFF) << RSHIFT) |
			  ((g & 0xFF) << GSHIFT) |
			  ((b & 0xFF) << BSHIFT) |
			  ((a & 0xFF) << ASHIFT)  );

	return c;
 }

 inline RT_Color CreatePixelColorf(float r, float g, float b, float a)
 {
	return CreatePixelColori(r*255, g*255, b*255, a*255);
 }

 inline RT_Color CreatePixelColorv4(const RT_Vec4f c)
 {
	return CreatePixelColori(c.x*255, c.y*255, c.z*255, c.w*255);
 }

 inline RT_Color CreatePixelColorv3(const RT_Vec3f c)
 {
	return CreatePixelColori(c.x*255, c.y*255, c.z*255, 255);
 }

 inline RT_Color CreatePixelColori32(const int rgba)
 {
	RT_Color c;
	c.ri = (rgba & RMASK) >> RSHIFT;
	c.gi = (rgba & GMASK) >> GSHIFT;
	c.bi = (rgba & BMASK) >> BSHIFT;
	c.ai = (rgba & AMASK) >> ASHIFT;

	c.rf = c.ri/255.0f;
	c.gf = c.gi/255.0f;
	c.bf = c.bi/255.0f;
	c.af = c.ai/255.0f;

	c.rgba = rgba;

	return c;
 }

 inline void Saturate(RT_Vec3f *c)
 {
	/*c->ri = (c->ri < 0)? 0 : (c->ri > 255)? 255 : c->ri;
	c->gi = (c->gi < 0)? 0 : (c->gi > 255)? 255 : c->gi;
	c->bi = (c->bi < 0)? 0 : (c->bi > 255)? 255 : c->bi;
	c->ai = (c->ai < 0)? 0 : (c->ai > 255)? 255 : c->ai;

	c->rf = (c->rf < 0.0f)? 0.0f : (c->rf > 1.0f)? 1.0f : c->rf;
	c->gf = (c->gf < 0.0f)? 0.0f : (c->gf > 1.0f)? 1.0f : c->gf;
	c->bf = (c->bf < 0.0f)? 0.0f : (c->bf > 1.0f)? 1.0f : c->bf;
	c->af = (c->af < 0.0f)? 0.0f : (c->af > 1.0f)? 1.0f : c->af;

	c->rgba = (((c->ri & 0xFF) << RSHIFT) |
			   ((c->gi & 0xFF) << GSHIFT) |
			   ((c->bi & 0xFF) << BSHIFT) |
			   ((c->ai & 0xFF) << ASHIFT)  );*/

	c->x = (c->x < 0)? 0 : (c->x > 1)? 1 : c->x;
	c->y = (c->y < 0)? 0 : (c->y > 1)? 1 : c->y;
	c->z = (c->z < 0)? 0 : (c->z > 1)? 1 : c->z;
 }

/*----------------------------------------------------------------------------------------------
 *
 * Ray
 *
 *----------------------------------------------------------------------------------------------*/

 typedef struct
 {
	RT_Vec3f o;
	RT_Vec3f d;
 } RT_Ray;

 inline RT_Ray CreateRay(const RT_Vec3f o, const RT_Vec3f d)
 {
	RT_Ray r;
	r.o = o;
	r.d = d;
	return r;
 }

 inline RT_Vec3f HitPoint(const RT_Ray *r, const float t)
 {
	return (r->o + (r->d * t));
 }


