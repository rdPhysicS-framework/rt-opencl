typedef struct 
{
	float x;
	float y;

} RT_Vec2f;

typedef struct
{
	float x;
	float y;
	float z;
} RT_Vec3f;

typedef struct
{
	float x;
	float y;
	float z;
	float w;
} RT_Vec4f;

typedef struct
{
	float m[4][4];
} RT_Mat4f;

/*----------------------------------------------------------------------------------------------
 *
 *Methods of vectors 2f*
 *
 *----------------------------------------------------------------------------------------------*/

 inline RT_Vec2f v2_Create(float x, float y)
 {
	RT_Vec2f v;
	v.x = x;
	v.y = y;
	return v;
 }

 inline RT_Vec2f v2_Add(RT_Vec2f *v1, RT_Vec2f *v2)
 {
	return v2_Create(v1->x + v2->x,
					 v1->y + v2->y);
 }

 inline RT_Vec2f v2_AddS(RT_Vec2f *v, float s)
 {
	return v2_Create(v->x + s,
					 v->y + s);
 }

  inline RT_Vec2f v2_Sub(RT_Vec2f *v1, RT_Vec2f *v2)
 {
	return v2_Create(v1->x - v2->x,
					 v1->y - v2->y);
 }

 inline RT_Vec2f v2_SubS(RT_Vec2f *v, float s)
 {
	return v2_Create(v->x - s,
					 v->y - s);
 }

  inline RT_Vec2f v2_Mul(RT_Vec2f *v1, RT_Vec2f *v2)
 {
	return v2_Create(v1->x * v2->x,
					 v1->y * v2->y);
 }

 inline RT_Vec2f v2_MulS(RT_Vec2f *v, float s)
 {
	return v2_Create(v->x * s,
					 v->y * s);
 }

  inline RT_Vec2f v2_Div(RT_Vec2f *v1, RT_Vec2f *v2)
 {
	return v2_Create(v1->x / v2->x,
					 v1->y / v2->y);
 }

 inline RT_Vec2f v2_DivS(RT_Vec2f *v, float s)
 {
	return v2_Create(v->x / s,
					 v->y / s);
 }

 inline bool v2_Compare(const RT_Vec2f *a, const RT_Vec2f *b)
{
	return (a->x == b->x) && (a->y == b->y);
}

inline bool v2_Larger(const RT_Vec2f *a, const float v)
{
	return (a->x > v) && (a->y > v);
}

inline bool v2_Smaller(const RT_Vec2f *a, const float v)
{
	return (a->x < v) && (a->y < v);
}

inline float v2_SizeSQR(const RT_Vec2f *v)
{
	return (v->x * v->x + v->y * v->y);
}

inline float v2_Size(const RT_Vec2f *v)
{
	return sqrt(v2_SizeSQR(v));
}

inline RT_Vec2f v2_Normalize(const RT_Vec2f *v)
{
	float invValue = 1.0 / v2_Size(v); 
	
	return (invValue == 0.0)? *v : v2_MulS(v, invValue);
}

inline float v2_Dot(const RT_Vec2f *a, const RT_Vec2f *b)
{
	return (a->x * b->x) + 
		   (a->y * b->y)  ;
}

inline RT_Vec2f v2_Mix(const RT_Vec2f *a, const RT_Vec2f *b, const float p)
{
	return v2_Create(a->x * (1.0 - p) + b->x * p,
					 a->y * (1.0 - p) + b->y * p );
}

inline RT_Vec2f v2_Reflect(const RT_Vec2f *v, const RT_Vec2f *n)
{
	float d = v2_Dot(v, n);

	return v2_Create(v->x - 2.0 * d * n->x,
					 v->y - 2.0 * d * n->y );
}

inline RT_Vec2f v2_Refract(const RT_Vec2f *v, const RT_Vec2f *n, const float i)
{
	float d = v2_Dot(v, n);
	float k = 1.0 - pow(i, 2) * (1.0 - pow(d, 2));

	if(k < 0.0)
		return *v;

	float sr = sqrt(k);

	return v2_Create(i * v->x - n->x * (i * d + sr),
					 i * v->y - n->y * (i * d + sr) );
}

inline RT_Vec2f v2_Lerp(const RT_Vec2f *a, const RT_Vec2f *b, const float t)
{
	return v2_Create(a->x + (b->x - a->x) * t,
					 a->y + (b->y - a->y) * t );
}

inline RT_Vec2f v2_Negate(const RT_Vec2f *v)
{
	return v2_MulS(v, -1);
}

inline void v2_Swap(__global RT_Vec2f *a, const RT_Vec2f *b)
{
	a->x = b->x;
	a->y = b->y;
}

inline RT_Vec2f v2_Clamp(const RT_Vec2f *v, const float min, const float max)
{
	return v2_Create((v->x < min)? min : (v->x > max)? max : v->x,
					 (v->y < min)? min : (v->y > max)? max : v->y );	
}

inline float v2_Angle(const RT_Vec2f *v)
{
	return atan2(v->y, v->x);
}

inline RT_Vec2f v2_Rotate(const RT_Vec2f *v, float r)
{
	float s = sin(r);
	float c = cos(r);

	return v2_Create(v->x * c - v->y * s,
					 v->x * s + v->y * c );
}

/*----------------------------------------------------------------------------------------------
 *
 *Methods of vectors 3f*
 *
 *----------------------------------------------------------------------------------------------*/

inline RT_Vec3f v3_Create(float x, float y, float z)
{
	RT_Vec3f v;
	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

inline RT_Vec3f v3_CreateV2(RT_Vec2f *v, float z)
{
	return v3_Create(v->x, v->y, z);
}

inline RT_Vec3f v3_Add(const RT_Vec3f *a, const RT_Vec3f *b)
{
	return v3_Create(a->x + b->x, 
					 a->y + b->y, 
					 a->z + b->z);
}

inline RT_Vec3f v3_AddS(const RT_Vec3f *a, const float s)
{
	return v3_Create(a->x + s, 
					 a->y + s, 
					 a->z + s);
}

inline RT_Vec3f v3_Sub(const RT_Vec3f *a, const RT_Vec3f *b)
{
	return v3_Create(a->x - b->x, 
					 a->y - b->y, 
					 a->z - b->z);
}

inline RT_Vec3f v3_SubS(const RT_Vec3f *a, const float s)
{
	return v3_Create(a->x - s, 
					 a->y - s, 
					 a->z - s);
}

inline RT_Vec3f v3_Mul(const RT_Vec3f *a, const RT_Vec3f *b)
{
	return v3_Create(a->x * b->x, 
					 a->y * b->y, 
					 a->z * b->z);
}

inline RT_Vec3f v3_MulS(const RT_Vec3f *a, const float s)
{
	return v3_Create(a->x * s, 
					 a->y * s, 
					 a->z * s);
}

inline RT_Vec3f v3_Div(const RT_Vec3f *a, const RT_Vec3f *b)
{
	return v3_Create(a->x / b->x, 
					 a->y / b->y, 
					 a->z / b->z);
}

inline RT_Vec3f v3_DivS(const RT_Vec3f *a, const float s)
{
	return v3_Create(a->x / s, 
					 a->y / s, 
					 a->z / s);
}

inline bool v3_Compare(const RT_Vec3f *a, const RT_Vec3f *b)
{
	return (a->x == b->x) && (a->y == b->y) && (a->z == b->z);
}

inline bool v3_Larger(const RT_Vec3f *a, const float v)
{
	return (a->x > v) && (a->y > v) && (a->z > v);
}

inline bool v3_Smaller(const RT_Vec3f *a, const float v)
{
	return (a->x < v) && (a->y < v) && (a->z < v);
}

inline float v3_SizeSQR(const RT_Vec3f *v)
{
	return (v->x * v->x + v->y * v->y + v->z * v->z);
}

inline float v3_Size(const RT_Vec3f *v)
{
	return sqrt(v3_SizeSQR(v));
}

inline RT_Vec3f v3_Normalize(const RT_Vec3f *v)
{
	float invValue = 1.0 / v3_Size(v); 
	
	return (invValue == 0.0)? *v : v3_MulS(v, invValue);
}

inline float v3_Dot(const RT_Vec3f *a, const RT_Vec3f *b)
{
	return (a->x * b->x) + 
		   (a->y * b->y) + 
		   (a->z * b->z);
}

inline RT_Vec3f v3_Cross(const RT_Vec3f *a, const RT_Vec3f *b)
{
	return v3_Create((a->y * b->z) - (a->z * b->y),
					 (a->z * b->x) - (a->x * b->z),
					 (a->x * b->y) - (a->y * b->x) );
}

inline RT_Vec3f v3_Mix(const RT_Vec3f *a, const RT_Vec3f *b, const float p)
{
	return v3_Create(a->x * (1.0 - p) + b->x * p,
					 a->y * (1.0 - p) + b->y * p,
					 a->z * (1.0 - p) + b->z * p );
}

inline RT_Vec3f v3_Reflect(const RT_Vec3f *v, const RT_Vec3f *n)
{
	float d = v3_Dot(v, n);

	return v3_Create(v->x - 2.0 * d * n->x,
					 v->y - 2.0 * d * n->y,
					 v->z - 2.0 * d * n->z );
}

inline RT_Vec3f v3_Refract(const RT_Vec3f *v, const RT_Vec3f *n, const float i)
{
	float d = v3_Dot(v, n);
	float k = 1.0 - pow(i, 2) * (1.0 - pow(d, 2));

	if(k < 0.0)
		return *v;

	float sr = sqrt(k);

	return v3_Create(i * v->x - n->x * (i * d + sr),
					 i * v->y - n->y * (i * d + sr),
					 i * v->z - n->z * (i * d + sr) );
}

inline RT_Vec3f v3_Lerp(const RT_Vec3f *a, const RT_Vec3f *b, const float t)
{
	return v3_Create(a->x + (b->x - a->x) * t,
					 a->y + (b->y - a->y) * t,
					 a->z + (b->z - a->z) * t );
}

inline RT_Vec3f v3_Negate(const RT_Vec3f *v)
{
	return v3_MulS(v, -1);
}

inline void v3_Swap(__global RT_Vec3f *a, const RT_Vec3f *b)
{
	a->x = b->x;
	a->y = b->y;
	a->z = b->z;
}

inline RT_Vec3f v3_Clamp(const RT_Vec3f *v, const float min, const float max)
{
	return v3_Create((v->x < min)? min : (v->x > max)? max : v->x,
					 (v->y < min)? min : (v->y > max)? max : v->y,
					 (v->z < min)? min : (v->z > max)? max : v->z );	
}

/*----------------------------------------------------------------------------------------------
 *
 *Methods of vectors 4f*
 *
 *----------------------------------------------------------------------------------------------*/

 inline RT_Vec4f v4_Create(float x, float y, float z, float w)
 {
	RT_Vec4f v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
 }

 inline RT_Vec4f v4_CreateV3(RT_Vec3f *v, float w)
 {
	return v4_Create(v->x, v->y, v->z, w);
 }

  inline RT_Vec4f v4_CreateV2(RT_Vec2f *v, float z, float w)
 {
	return v4_Create(v->x, v->y, z, w);
 }

 inline RT_Vec4f v4_Add(const RT_Vec4f *a, const RT_Vec4f *b)
{
	return v4_Create(a->x + b->x, 
					 a->y + b->y, 
					 a->z + b->z,
					 a->w + b->w );
}

inline RT_Vec4f v4_AddS(const RT_Vec4f *a, const float s)
{
	return v4_Create(a->x + s, 
					 a->y + s, 
					 a->z + s,
					 a->w + s );
}

inline RT_Vec4f v4_Sub(const RT_Vec4f *a, const RT_Vec4f *b)
{
	return v4_Create(a->x - b->x, 
					 a->y - b->y, 
					 a->z - b->z,
					 a->w - b->w );
}

inline RT_Vec4f v4_SubS(const RT_Vec4f *a, const float s)
{
	return v4_Create(a->x - s, 
					 a->y - s, 
					 a->z - s,
					 a->w - s );
}

inline RT_Vec4f v4_Mul(const RT_Vec4f *a, const RT_Vec4f *b)
{
	return v4_Create(a->x * b->x, 
					 a->y * b->y, 
					 a->z * b->z,
					 a->w * b->w );
}

inline RT_Vec4f v4_MulS(const RT_Vec4f *a, const float s)
{
	return v4_Create(a->x * s, 
					 a->y * s, 
					 a->z * s,
					 a->w * s );
}

inline RT_Vec4f v4_Div(const RT_Vec4f *a, const RT_Vec4f *b)
{
	return v4_Create(a->x / b->x, 
					 a->y / b->y, 
					 a->z / b->z,
					 a->w / b->w );
}

inline RT_Vec4f v4_DivS(const RT_Vec4f *a, const float s)
{
	return v4_Create(a->x / s, 
					 a->y / s, 
					 a->z / s,
					 a->w / s);
}

inline bool v4_Compare(const RT_Vec4f *a, const RT_Vec4f *b)
{
	return (a->x == b->x) && (a->y == b->y) && (a->z == b->z);
}

inline bool v4_Larger(const RT_Vec4f *a, const float v)
{
	return (a->x > v) && (a->y > v) && (a->z > v);
}

inline bool v4_Smaller(const RT_Vec4f *a, const float v)
{
	return (a->x < v) && (a->y < v) && (a->z < v);
}

inline float v4_SizeSQR(const RT_Vec4f *v)
{
	return (v->x * v->x + v->y * v->y + v->z * v->z);
}

inline float v4_Size(const RT_Vec4f *v)
{
	return sqrt(v4_SizeSQR(v));
}

inline RT_Vec4f v4_Normalize(const RT_Vec4f *v)
{
	float invValue = 1.0 / v4_Size(v); 
	
	return (invValue == 0.0)? *v : v4_MulS(v, invValue);
}

inline float v4_Dot(const RT_Vec4f *a, const RT_Vec4f *b)
{
	return (a->x * b->x) + 
		   (a->y * b->y) + 
		   (a->z * b->z) +
		   (a->w * b->w)  ;
}

inline RT_Vec4f v4_Lerp(const RT_Vec4f *a, const RT_Vec4f *b, float t)
{
	return v4_Create(a->x + (b->x - a->x) * t,
					 a->y + (b->y - a->y) * t,
					 a->z + (b->z - a->z) * t,
					 a->w + (b->w - a->w) * t );
}

inline RT_Vec4f v4_Mix(const RT_Vec4f *a, const RT_Vec4f *b, float p)
{
	return v4_Create(a->x * (1.0 - p) + b->x * p,
					 a->y + (1.0 - p) + b->y * p,
					 a->z + (1.0 - p) + b->z * p,
					 a->w + (1.0 - p) + b->w * p );
}

inline RT_Vec4f v4_Negate(const RT_Vec4f *v)
{
	return v4_MulS(v, -1);
}

inline void v4_Swap(__global RT_Vec4f *a, const RT_Vec4f *b)
{
	a->x = b->x;
	a->y = b->y;
	a->z = b->z;
	a->w = b->w;
}

inline RT_Vec4f v4_Clamp(const RT_Vec4f *v, const float min, const float max)
{
	return v4_Create((v->x < min)? min : (v->x > max)? max : v->x,
					 (v->y < min)? min : (v->y > max)? max : v->y,
					 (v->z < min)? min : (v->z > max)? max : v->z,
					 (v->w < min)? min : (v->w > max)? max : v->w );	
}

/*----------------------------------------------------------------------------------------------
 *
 *Methods of matrix 4f*
 *
 *----------------------------------------------------------------------------------------------*/

inline RT_Mat4f m4_Create(float a, float b, float c, float d,
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

inline RT_Mat4f m4_Identity()
{
	return m4_Create(1, 0, 0, 0,
					 0, 1, 0, 0,
					 0, 0, 1, 0,
					 0, 0, 0, 1 );
}

inline RT_Mat4f m4_Null()
{
	return m4_Create(0, 0, 0, 0,
					 0, 0, 0, 0,
					 0, 0, 0, 0,
					 0, 0, 0, 0 );
}

inline RT_Mat4f m4_Copy(const RT_Mat4f *a)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j];

	//memcpy(r.m, (void*)m->m, sizeof(r.m));
	return r;
}

inline RT_Mat4f m4_Transpose(const RT_Mat4f *m)
{
	RT_Mat4f mt;
	mt.m[0][0] = m->m[0][0]; mt.m[0][1] = m->m[1][0]; mt.m[0][2] = m->m[2][0]; mt.m[0][3] = m->m[3][0];
	mt.m[1][0] = m->m[0][1]; mt.m[1][1] = m->m[1][1]; mt.m[1][2] = m->m[2][1]; mt.m[1][3] = m->m[3][1];
	mt.m[2][0] = m->m[0][2]; mt.m[2][1] = m->m[1][2]; mt.m[2][2] = m->m[2][2]; mt.m[2][3] = m->m[3][2];
	mt.m[3][0] = m->m[0][3]; mt.m[3][1] = m->m[1][3]; mt.m[3][2] = m->m[2][3]; mt.m[3][3] = m->m[3][3];
	return mt;
}

inline float m4_Determinant(const RT_Mat4f *m)
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

/*inline RT_Mat4f m4_Inverse(const RT_Mat4f *m)
{
	RT_Mat4f inv;

	return m4_Create();
}*/

inline RT_Mat4f m4_AffTranslation(const RT_Vec3f *v)
{
	return m4_Create(1, 0, 0, v->x,
					 0, 1, 0, v->y,
					 0, 0, 1, v->z,
					 0, 0, 0, 1    );
}

inline RT_Mat4f m4_AffTranslationF(const float x, const float y, const float z)
{
	return m4_Create(1, 0, 0, x,
					 0, 1, 0, y,
					 0, 0, 1, z,
					 0, 0, 0, 1 );
}

inline RT_Mat4f m4_AffRotationX(const float a)
{
	float c = cos(a);
	float s = sin(a);

	return m4_Create(1, 0,  0, 0,
					 0, c, -s, 0,
					 0, s,  c, 0,
					 0, 0,  0, 1 );
}

inline RT_Mat4f m4_AffRotationY(const float a)
{
	float c = cos(a);
	float s = sin(a);

	return m4_Create( c, 0, s, 0,
					  0, 1, 0, 0,
					 -s, 0, c, 0,
					  0, 0, 0, 1 );
}

inline RT_Mat4f m4_AffRotationZ(const float a)
{
	float c = cos(a);
	float s = sin(a);
	
	return m4_Create(c, -s, 0, 0,
					 s,  c, 0, 0,
					 0,  0, 1, 0,
					 0,  0, 0, 1 );
}

inline RT_Mat4f m4_AffScaling(const float s)
{
	return m4_Create(s, 0, 0, 0,
					 0, s, 0, 0,
					 0, 0, s, 0,
					 0, 0, 0, 1 );
}

inline RT_Mat4f m4_AffScalingF(const float x, const float y, const float z)
{
	return m4_Create(x, 0, 0, 0,
					 0, y, 0, 0,
					 0, 0, z, 0,
					 0, 0, 0, 1 );
}

inline RT_Mat4f m4_AffScalingV(const RT_Vec3f *v)
{
	return m4_AffScalingF(v->x, v->y, v->z);
}

inline RT_Mat4f m4_AffInvTranslation(const RT_Vec3f *v)
{
	return m4_AffTranslationF(-v->x, -v->y, -v->z);
}

inline RT_Mat4f m4_AffInvTranslationF(const float x, const float y, const float z)
{
	return m4_AffTranslationF(-x, -y, -z);
}

inline RT_Mat4f m4_AffInvRotationX(const float a)
{
	float c = cos(a);
	float s = sin(a);

	return m4_Create(1,  0, 0, 0,
					 0,  c, s, 0,
					 0, -s, c, 0,
					 0,  0, 0, 1 );
}

inline RT_Mat4f m4_AffInvRotationY(const float a)
{
	float c = cos(a);
	float s = sin(a);

	return m4_Create( c, 0, -s, 0,
					  0, 1,  0, 0,
					  s, 0,  c, 0,
					  0, 0,  0, 1 );
}

inline RT_Mat4f m4_AffInvRotationZ(const float a)
{
	float c = cos(a);
	float s = sin(a);
	
	return m4_Create( c, s, 0, 0,
					 -s,  c, 0, 0,
					  0,  0, 1, 0,
					  0,  0, 0, 1 );
}

inline RT_Mat4f m4_AffInvScaling(const float s)
{
	return m4_AffScaling(1.0/s);
}

inline RT_Mat4f m4_AffInvScalingV(const RT_Vec3f *v)
{
	return m4_AffScalingF(1.0/v->x, 1.0/v->y, 1.0/v->z);
}


inline RT_Mat4f m4_AffInvScalingF(const float x, const float y, const float z)
{
	return m4_AffScalingF(1.0/x, 1.0/y, 1.0/z);
}


inline RT_Mat4f m4_Add(const RT_Mat4f *a, const RT_Mat4f *b)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] + b->m[i][j];

	return r;
}

inline RT_Mat4f m4_AddS(const RT_Mat4f *a, const float s)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] + s;

	return r;
}

inline RT_Mat4f m4_Sub(const RT_Mat4f *a, const RT_Mat4f *b)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] - b->m[i][j];

	return r;
}

inline RT_Mat4f m4_SubS(const RT_Mat4f *a, const float s)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] - s;

	return r;
}

inline RT_Mat4f m4_Mul(const RT_Mat4f *a, const RT_Mat4f *b)
{
	RT_Mat4f r = m4_Null();

	for(int c = 0; c < SIZE_MATRIX; c++)
		for(int i = 0; i < SIZE_MATRIX; i++)
			for(int j = 0; j < SIZE_MATRIX; j++)
				r.m[c][i] += a->m[c][j] * b->m[j][i];

	return r;
}

inline RT_Mat4f m4_MulS(const RT_Mat4f *a, const float s)
{
	RT_Mat4f r;
	for(int i = 0; i < SIZE_MATRIX; i++)
		for(int j = 0; j < SIZE_MATRIX; j++)
			r.m[i][j] = a->m[i][j] * s;

	return r;
}

inline RT_Mat4f m4_Div(const RT_Mat4f *a, const float s)
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
	return v3_Create(m->m[0][0] * p->x + m->m[0][1] * p->y + m->m[0][2] * p->z + m->m[0][3],
					 m->m[1][0] * p->x + m->m[1][1] * p->y + m->m[1][2] * p->z + m->m[1][3],
					 m->m[2][0] * p->x + m->m[2][1] * p->y + m->m[2][2] * p->z + m->m[2][3] );
}

inline RT_Vec3f TransformDirection(const RT_Mat4f *m, const RT_Vec3f *d)
{
	return v3_Create(m->m[0][0] * d->x + m->m[0][1] * d->y + m->m[0][2] * d->z,
					 m->m[1][0] * d->x + m->m[1][1] * d->y + m->m[1][2] * d->z,
					 m->m[2][0] * d->x + m->m[2][1] * d->y + m->m[2][2] * d->z );
}

inline RT_Vec3f TransformNormal(const RT_Mat4f *m, const RT_Vec3f *n)
{
	RT_Vec3f r = v3_Create(m->m[0][0] * n->x + m->m[1][0] * n->y + m->m[2][0] * n->z,
						   m->m[0][1] * n->x + m->m[1][1] * n->y + m->m[2][1] * n->z,
						   m->m[0][2] * n->x + m->m[1][2] * n->y + m->m[2][2] * n->z );
	return v3_Normalize(&r);
				   
}

__kernel void Teste(__global RT_Vec3f *out) 
{
	RT_Vec3f v1 = v3_Create(10, 20, 6);
	RT_Vec3f v2 = v3_Create(8, 26, 8);
	
	RT_Mat4f m1 = m4_AffTranslationF(10, 5, 2);
	RT_Mat4f m2 = m4_Copy(&m1);

	*out = TransformPoint(&m2, &v1);
}