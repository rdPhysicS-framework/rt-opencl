/*----------------------------------------------------------------------------------------------
 *
 * Methods of the PointLight
 *
 *----------------------------------------------------------------------------------------------*/
 RT_Light Create_PointLight(const RT_Vec3f p, const RT_Vec3f c, const float ls)
 {
	RT_Light l;
	l.position = p;
	l.color = c;
	l.ls = ls;
	return l;
 }

 inline RT_Vec3f Direction(const RT_Light *l, const RT_Result *r)
 {
	return normalize(l->position - r->lhitPoint);
 }

 inline RT_Vec3f Color(const RT_Light *l)
 {
	return l->color * l->ls;
 }

 /*bool InShadow(const RT_Light *l, const RT_Ray *ray, const RT_Result *r)
 {
	return r->shadowHit()
 }*/
