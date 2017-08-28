/*----------------------------------------------------------------------------------------------
 *
 * Result
 *
 *----------------------------------------------------------------------------------------------*/
RT_Result CreateResult()
{ 
	RT_Result r;
	r.hit = false;
	//r.ray = 0;
	//r.color = (RT_Vec3f)(0.0f);
	//r.material.color = (RT_Vec3f)(0.0f);
	return r;
}

/*----------------------------------------------------------------------------------------------
 *
 * Methods to verify intercession with the sphere
 *
 *----------------------------------------------------------------------------------------------*/

RT_Sphere CreateSphere(const RT_Vec3f c, const float r)
{ 
	RT_Sphere s;
	s.center = c;
	s.radius = r;
	return s;
}

bool Sphere_ShadowHit(const RT_Sphere *s, const RT_Ray *ray, float *tmin)
{
	RT_Vec3f temp = ray->o - s->center;
	float a = dot(ray->d, ray->d);
	float b = 2.0 * dot(temp, ray->d);
	float c = dot(temp, temp) - (s->radius * s->radius);
	float disc = b * b - 4.0 * a * c;

	if(disc < 0.0)
		return false;

	float e = sqrt(disc);
	float denom = 2.0 * a;
	float t = (-b - e) / denom;

	if(t > 0.01)
	{
		*tmin = t;
		return true;
	}

	t = (-b + e) / denom;

	if(t > 0.01)
	{
		*tmin = t;
		return true;
	}

	return false;
}

bool Sphere_Hit(const RT_Sphere *s, const RT_Ray *ray, float *tmin, RT_Result *r)
{
	if(Sphere_ShadowHit(s, ray, tmin))
	{
		RT_Vec3f temp = ray->o - s->center;
		r->normal = (temp + (ray->d * *tmin)) / s->radius;
		r->lhitPoint = HitPoint(ray, *tmin);
		return true;
	}

	return false;
}

/*----------------------------------------------------------------------------------------------
 *
 * Methods to verify intercession with the plane
 *
 *----------------------------------------------------------------------------------------------*/

bool Plane_ShadowHit(const RT_Plane *p, const RT_Ray *ray, float *tmin)
{
	float t = dot((p->point - ray->o), p->normal) / dot(ray->d, p->normal);

	if(t > 0.001)
	{ 
		*tmin = t;
		return true;
	}

	return false;
}

bool Plane_Hit(const RT_Plane *p, const RT_Ray *ray, float *tmin, RT_Result *r)
{
	if(Plane_ShadowHit(p, ray, tmin))
	{
		r->normal = p->normal;
		r->lhitPoint = HitPoint(ray, *tmin);
		return true;
	}

	return false;
}

/*----------------------------------------------------------------------------------------------
 *
 * Methods to verify intercession with the Box
 *
 *----------------------------------------------------------------------------------------------*/

RT_Vec3f Box_Normal(const RT_Box *b, const RT_Vec3f *p)
{
	float d[6];
	d[0] = fabs(b->size.x - b->position.x);
	d[1] = fabs(b->size.x + b->size.x - b->position.x);
	d[2] = fabs(b->size.y - b->position.y);
	d[3] = fabs(b->size.y + b->size.y - b->position.y);
	d[4] = fabs(b->size.z - b->position.z);
	d[5] = fabs(b->size.z + b->size.z - b->position.z);

	int f = 0;
	float t = d[0];
	for(int i = 1; i < 6; i++)
	{
		if(d[i] < t)
		{
			t = d[i];
			f = i;
		}
	}

	return (f == 0) ? (RT_Vec3f)(-1.0f,  0.0f,  0.0f) :
		   (f == 1) ? (RT_Vec3f)( 1.0f,  0.0f,  0.0f) :
		   (f == 2) ? (RT_Vec3f)( 0.0f, -1.0f,  0.0f) :
		   (f == 3) ? (RT_Vec3f)( 0.0f,  1.0f,  0.0f) :
		   (f == 4) ? (RT_Vec3f)( 0.0f,  0.0f, -1.0f) :
		              (RT_Vec3f)( 0.0f,  0.0f,  1.0f)  ;
}

bool Box_ShadowHit(const RT_Box *b, const RT_Ray *ray, float *tmin)
{
	float t[6];
	RT_Vec3f ip[6];

	RT_Vec3f d = ray->d;
	RT_Vec3f o = ray->o;

	bool hit = false;

	for (int i = 0; i < 6; i++)
	{
		t[i] = -1;
	}

	RT_Vec3f s = b->position + b->size;

	if(d.x)
	{
		float rc = 1.0f / d.x;
		t[0] = (b->position.x - o.x) * rc;
		t[3] = (s.x - o.x) * rc;
	}
	if(d.y)
	{
		float rc = 1.0f / d.y;
		t[1] = (b->position.y - o.y) * rc;
		t[4] = (s.y - o.y) * rc;
	}
	if(d.z)
	{
		float rc = 1.0f / d.z;
		t[2] = (b->position.z - o.z) * rc;
		t[5] = (s.z - o.z) * rc;
	}

	for(int i = 0; i < 6; i++)
	{
		if(t[i] > 0)
		{
			ip[i] = o + d * t[i];

			if ((ip[i].x > (b->position.x - EPSILON)) && (ip[i].x < (s.x + EPSILON)) &&
				(ip[i].y > (b->position.y - EPSILON)) && (ip[i].y < (s.y + EPSILON)) &&
				(ip[i].z > (b->position.z - EPSILON)) && (ip[i].z < (s.z + EPSILON)))
			{
				if(t[i] < *tmin)
				{
					*tmin = t[i];
					hit = true;
				}
			}
		}
	}

	return hit;
}

bool Box_Hit(const RT_Box *b, const RT_Ray *ray, float *tmin, RT_Result *r)
{
	if(Box_ShadowHit(b, ray, tmin))
	{
		r->lhitPoint = HitPoint(ray, *tmin);
		r->normal = Box_Normal(b, &r->lhitPoint);
		return true;
	}

	return false;
}