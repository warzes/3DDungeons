#pragma once

//а можно ведь не резать меш. можно так
//когда Я делаю коллайдер - Я же беру все вершины
//можно по другому - брать треугольник (по индексам) и смотреь где он находитсЯ на сетке, туда и писать.
//тогда надо сравниваь только клетки

#include <iterator>

#define ENABLE_FPS 0


class Poly
{
public:
	std::vector<Vector3> verts;
	int cnt = 0; // todo: delete, in verts.size
};

Poly MeshGetPoly(const Mesh& mesh)
{
	Poly poly;

#if 1
	for( size_t i = 0; i < mesh.indices.size(); i++ ) // TODO: медленно, надо переделать
	{
		poly.verts.push_back(mesh.vertices[mesh.indices[i]].position);
	}
#else
	for( size_t i = 0; i < vertices.size(); i++ ) // TODO: медленно, надо переделать
	{
		poly.verts.push_back(vertices[i].position);
	}
#endif
	poly.cnt = poly.verts.size();

	return poly;
}


class MeshColliderData
{
public:
	void Init(const Model& model)
	{
		for( int i = 0; i < model.GetSubMesh().size(); i++ )
		{
			Poly subPoly = MeshGetPoly(model.GetSubMesh()[i]);

			// https://www.techiedelight.com/ru/concatenate-two-vectors-cpp/
			std::move(subPoly.verts.begin(), subPoly.verts.end(), std::back_inserter(poly.verts));
			poly.cnt += subPoly.cnt;
		}
	}

	Poly poly;
};


namespace Collisions
{
	inline bool CheckPointInTriangle(const Vector3& tri0, const Vector3& tri1, const Vector3& tri2, const Vector3& point)
	{
		Vector3 u = tri1 - tri0;
		Vector3 v = tri2 - tri0;
		Vector3 w = point - tri0;

		Vector3 vw = CrossProduct(v, w);
		Vector3 vu = CrossProduct(v, u);

		if (DotProduct(vw, vu) < 0.0f)
			return false;

		Vector3 uw = CrossProduct(u, w);
		Vector3 uv = CrossProduct(u, v);

		if (DotProduct(uw, uv) < 0.0f)
			return false;

		float d = uv.GetLength();
		float r = vw.GetLength() / d;
		float t = uw.GetLength() / d;
		return ((r + t) <= 1.0f);
	}

	bool GetLowestRoot(float a, float b, float c, float maxR, float* root)
	{
		// Check if a solution exists
		float determinant = b * b - 4.0f * a * c;

		// If determinant is negative it means no solutions.
		if (determinant < 0.0f) return false;

		// calculate the two roots: (if determinant == 0 then
		// x1==x2 but let’s disregard that slight optimization)
		float sqrtD = sqrt(determinant);
		float r1 = (-b - sqrtD) / (2.0f * a);
		float r2 = (-b + sqrtD) / (2.0f * a);

		// Sort so x1 <= x2
		if (r1 > r2)
		{
			float temp = r2;
			r2 = r1;
			r1 = temp;
		}
		// Get lowest root:
		if (r1 > 0 && r1 < maxR)
		{
			*root = r1;
			return true;
		}
		// It is possible that we want x2 - this can happen
		// if x1 < 0
		if (r2 > 0 && r2 < maxR)
		{
			*root = r2;
			return true;
		}
		// No (valid) solutions
		return false;
	}
}

class Plane
{
public:
	Plane() = default;
	Plane(const Vector3& origin, const Vector3& normal);
	Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);

	bool IsFrontFacingTo(const Vector3& direction) const;
	float SignedDistanceTo(const Vector3& point) const;

	Vector4 equation;
	Vector3 origin;
	Vector3 normal;
};

inline Plane::Plane(const Vector3& Origin, const Vector3& Normal)
{
	origin = Origin;
	normal = Normal;
	equation.x = Normal.x;
	equation.y = Normal.y;
	equation.z = Normal.z;
	equation.w = -DotProduct(Origin, Normal);
}

// Construct from triangle:
inline Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	normal = CrossProduct(p2 - p1, p3 - p1).GetNormalize();

	origin = p1;

	equation.x = normal.x;
	equation.y = normal.y;
	equation.z = normal.z;
	equation.w = -DotProduct(normal, origin);
}

inline float Plane::SignedDistanceTo(const Vector3& point) const
{
	return (DotProduct(point, normal)) + equation.w;
}

inline bool Plane::IsFrontFacingTo(const Vector3& direction) const
{
	const float d = DotProduct(normal, direction);
	return (d <= 0.0f);
}

#define unitsPerMeter 100.0f

#define SLIDE_BIAS 0.008
#define VERY_CLOSE_DIST 0.001
#define SLOPE_WALK_ANGLE 0.80

class CollisionPacket
{
public:
	// Information about the move being requested: (in R3)
	Vector3 R3Velocity;
	Vector3 R3Position;

	// ellipsoid space
	Vector3 eRadius; // ellipsoid radius
	// Information about the move being requested: (in eSpace)
	Vector3 velocity;
	Vector3 normalizedVelocity;
	Vector3 basePoint;

	// original tri points
	Vector3 a, b, c;

	// Hit information
	bool foundCollision;
	float nearestDistance;
	float t;
	Vector3 intersectionPoint;

	Plane plane;

	// iteration depth
	int collisionRecursionDepth;
};

// Assumes: p1, p2and p3 are given in ellisoid space :
inline void CheckCollisionsTriangle(CollisionPacket* colPackage, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	// Make the Plane containing this triangle.
	Plane trianglePlane(p1, p2, p3);
	// Is triangle front-facing to the velocity vector?
	// only check front-facing triangles
	if (!trianglePlane.IsFrontFacingTo(colPackage->normalizedVelocity))
		return;

	// Get interval of Plane intersection:
	float t0, t1;
	bool embeddedInPlane = false;

	// Calculate the signed distance from sphere position to triangle Plane
	float signedDistToTrianglePlane = trianglePlane.SignedDistanceTo(colPackage->basePoint);

	// cache this as we’re going to use it a few times below:
	float normalDotVelocity = DotProduct(trianglePlane.normal, colPackage->velocity);
	// if sphere is travelling parrallel to the Plane:
	if (normalDotVelocity == 0.0f)
	{
		if (fabs(signedDistToTrianglePlane) >= 1.0f)
		{
			// Sphere is not embedded in Plane.
			// No collision possible:
			return;
		}
		else
		{
			// sphere is embedded in Plane.
			// It intersects in the whole range [0..1]
			embeddedInPlane = true;
			t0 = 0.0f;
			t1 = 1.0f;
		}
	}
	else
	{
		// N dot D is not 0. Calculate intersection interval:
		t0 = (-1.0f - signedDistToTrianglePlane) / normalDotVelocity;
		t1 = (1.0f - signedDistToTrianglePlane) / normalDotVelocity;

		// Swap so t0 < t1
		if (t0 > t1)
		{
			float temp = t1;
			t1 = t0;
			t0 = temp;
		}

		// Check that at least one result is within range:
		if (t0 > 1.0f || t1 < 0.0f)
		{
			// Both t values are outside values [0,1]
			// No collision possible:
			return;
		}
		// Clamp to [0,1]
		if (t0 < 0.0f) t0 = 0.0f;
		if (t1 < 0.0f) t1 = 0.0f;
		if (t0 > 1.0f) t0 = 1.0f;
		if (t1 > 1.0f) t1 = 1.0f;
	}

	// OK, at this point we have two time values t0 and t1
	// between which the swept sphere intersects with the
	// triangle Plane. If any collision is to occur it must
	// happen within this interval.
	Vector3 collisionPoint;
	bool foundCollison = false;
	float t = 1.0f;

	// First we check for the easy case - collision inside
	// the triangle. If this happens it must be at time t0
	// as this is when the sphere rests on the front side
	// of the triangle Plane. Note, this can only happen if
	// the sphere is not embedded in the triangle Plane.
	if (!embeddedInPlane)
	{
		Vector3 PlaneIntersectionPoint = (colPackage->basePoint - trianglePlane.normal) + t0 * colPackage->velocity;

		if (Collisions::CheckPointInTriangle(p1, p2, p3, PlaneIntersectionPoint))
		{
			foundCollison = true;
			t = t0;
			collisionPoint = PlaneIntersectionPoint;
		}
	}
	// if we haven’t found a collision already we’ll have to
	// sweep sphere against points and edges of the triangle.
	// Note: A collision inside the triangle (the check above)
	// will always happen before a vertex or edge collision!
	// This is why we can skip the swept test if the above
	// gives a collision!

	if (foundCollison == false)
	{
		// some commonly used terms:
		Vector3 velocity = colPackage->velocity;
		Vector3 base = colPackage->basePoint;
		float velocitySquaredLength = DotProduct(velocity, velocity);
		float a, b, c; // Params for equation
		float newT;

		// For each vertex or edge a quadratic equation have to
		// be solved. We parameterize this equation as
		// a*t^2 + b*t + c = 0 and below we calculate the
		// parameters a,b and c for each test.

		// Check against points:
		a = velocitySquaredLength;

		// P1
		b = 2.0f * (DotProduct(velocity, base - p1));
		c =DotProduct(p1 - base, p1 - base) - 1.0f;
		if (Collisions::GetLowestRoot(a, b, c, t, &newT))
		{
			t = newT;
			foundCollison = true;
			collisionPoint = p1;
		}

		// P2
		if (!foundCollison)
		{
			b = 2.0f * (DotProduct(velocity, base - p2));
			c = DotProduct(p2 - base, p2 - base) - 1.0f;
			if (Collisions::GetLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p2;
			}
		}

		// P3
		if (!foundCollison)
		{
			b = 2.0f * (DotProduct(velocity, base - p3));
			c = DotProduct(p3 - base, p3 - base) - 1.0f;
			if (Collisions::GetLowestRoot(a, b, c, t, &newT))
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p3;
			}
		}

		// Check agains edges:

		// p1 -> p2:
		Vector3 edge = p2 - p1;
		Vector3 baseToVertex = p1 - base;
		float edgeSquaredLength = DotProduct(edge, edge);
		float edgeDotVelocity = DotProduct(edge, velocity);
		float edgeDotBaseToVertex = DotProduct(edge, baseToVertex);

		// Calculate parameters for equation
		a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
		b = edgeSquaredLength * (2.0f * DotProduct(velocity, baseToVertex)) - 2.0f * edgeDotVelocity * edgeDotBaseToVertex;
		c = edgeSquaredLength * (1.0f - DotProduct(baseToVertex, baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

		// Does the swept sphere collide against infinite edge?
		if (Collisions::GetLowestRoot(a, b, c, t, &newT))
		{
			// Check if intersection is within line segment:
			float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
			if (f >= 0.0f && f <= 1.0f)
			{
				// intersection took place within segment.
				t = newT;
				foundCollison = true;
				collisionPoint = p1 + f * edge;
			}
		}

		// p2 -> p3:
		edge = p3 - p2;
		baseToVertex = p2 - base;
		edgeSquaredLength = DotProduct(edge, edge);
		edgeDotVelocity = DotProduct(edge, velocity);
		edgeDotBaseToVertex = DotProduct(edge, baseToVertex);

		// calculate params for equation
		a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
		b = edgeSquaredLength * (2.0f * DotProduct(velocity, baseToVertex)) - 2.0f * edgeDotVelocity * edgeDotBaseToVertex;
		c = edgeSquaredLength * (1.0f - DotProduct(baseToVertex, baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

		// do we collide against infinite edge
		if (Collisions::GetLowestRoot(a, b, c, t, &newT))
		{
			// check if intersect is within line segment
			float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
			if (f >= 0.0f && f <= 1.0f)
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p2 + f * edge;
			}
		}

		// p3 -> p1:
		edge = p1 - p3;
		baseToVertex = p3 - base;
		edgeSquaredLength = DotProduct(edge, edge);
		edgeDotVelocity = DotProduct(edge, velocity);
		edgeDotBaseToVertex = DotProduct(edge, baseToVertex);

		// calculate params for equation
		a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
		b = edgeSquaredLength * (2.0f * DotProduct(velocity, baseToVertex)) - 2.0f * edgeDotVelocity * edgeDotBaseToVertex;
		c = edgeSquaredLength * (1.0f - DotProduct(baseToVertex, baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

		// do we collide against infinite edge
		if (Collisions::GetLowestRoot(a, b, c, t, &newT))
		{
			// check if intersect is within line segment
			float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
			if (f >= 0.0f && f <= 1.0f)
			{
				t = newT;
				foundCollison = true;
				collisionPoint = p3 + f * edge;
			}
		}
	}
	// Set result:
	if (foundCollison == true)
	{
		// distance to collision: ’t’ is time of collision
		float distToCollision = t * colPackage->velocity.GetLength();
		// Does this triangle qualify for the closest hit?
		// it does if it’s the first hit or the closest
		if (colPackage->foundCollision == false || distToCollision < colPackage->nearestDistance)
		{
			// Collision information nessesary for sliding
			colPackage->nearestDistance = distToCollision;
			colPackage->intersectionPoint = collisionPoint;
			colPackage->foundCollision = true;
			colPackage->t = t;
			colPackage->plane = trianglePlane;
			colPackage->a = p1;
			colPackage->b = p2;
			colPackage->c = p3;
		}
	}
}


class CharacterEntity
{
public:
	CharacterEntity(Model* model, Vector3 radius);
	CharacterEntity(std::vector<Model*> models, Vector3 radius);
	void Update(float dt);
	void CheckCollision();

	void CollideAndSlide(const Vector3& gravity);

	Vector3 CollideWithWorld(const Vector3& pos, const Vector3& velocity);

	// new
	void CollideAndSlide2();
	void CollideWithWorld2(Vector3& pos, Vector3& velocity);
	void CheckGrounded();
	void Update2(float dt);

	Vector3 position, velocity, radius;
	CollisionPacket collisionPackage;
	std::vector<Model*> models;
	std::vector<MeshColliderData> colliders;
	int grounded;
};

CharacterEntity::CharacterEntity(Model* model, Vector3 radius)
{
	this->radius = radius;
	position = Vector3(0.0f);
	velocity = Vector3(0.0f);

	models.push_back(model);
	colliders.push_back({});
	colliders[0].Init(*models[0]);

	grounded = 0;
}

CharacterEntity::CharacterEntity(std::vector<Model*> models, Vector3 radius)
{
	this->radius = radius;
	position = Vector3(0.0f);
	velocity = Vector3(0.0f);

	this->models = models;
	for( size_t i = 0; i < this->models.size(); i++ )
	{
		colliders.push_back({});
		colliders[colliders.size()-1].Init(*this->models[i]);
	}
	grounded = 0;
}

void CharacterEntity::CollideAndSlide(const Vector3& gravity)
{
#if 1
	// Do collision detection:
	collisionPackage.R3Position = position;
	collisionPackage.R3Velocity = velocity;
	collisionPackage.eRadius = radius;

	// calculate position and velocity in eSpace
	Vector3 eSpacePosition = collisionPackage.R3Position / collisionPackage.eRadius;
	Vector3 Velocity = collisionPackage.R3Velocity / collisionPackage.eRadius;
	// no gravity
	Velocity.y = 0.0f;

	// Iterate until we have our final position.
	collisionPackage.collisionRecursionDepth = 0;

	int g = grounded;
	Vector3 finalPosition = CollideWithWorld(eSpacePosition, Velocity);
	grounded = g;

	// Add gravity pull:

	// Set the new R3 position (convert back from eSpace to R3)
	collisionPackage.R3Position = finalPosition * collisionPackage.eRadius;
	collisionPackage.R3Velocity = gravity;

	// convert velocity to e-space
	Velocity = gravity / collisionPackage.eRadius;

	// gravity iteration
	collisionPackage.collisionRecursionDepth = 0;
	finalPosition = CollideWithWorld(finalPosition, Velocity);

	// finally set entity position
	position = finalPosition * collisionPackage.eRadius;
#else
	// Do collision detection:
	collisionPackage.R3Position = position;
	collisionPackage.R3Velocity = velocity;
	collisionPackage.eRadius = radius;

	// y velocity in a seperate pass
	glm::vec3 Gravity = glm::vec3{ 0.0f };
	Gravity.y = collisionPackage.R3Velocity.y;
	//collisionPackage.R3Velocity.y = 0.0f;

	// lets get e-spacey?
	// calculate position and velocity in eSpace
	glm::vec3 eSpacePosition = collisionPackage.R3Position / collisionPackage.eRadius;
	glm::vec3 eSpaceVelocity = collisionPackage.R3Velocity / collisionPackage.eRadius;

	// Iterate until we have our final position.
	collisionPackage.collisionRecursionDepth = 0;

	int g = grounded;
	glm::vec3 finalPosition = CollideWithWorld(eSpacePosition, velocity);
	grounded = g;


#if 1
	// Set the new R3 position (convert back from eSpace to R3)
	collisionPackage.R3Position = finalPosition * collisionPackage.eRadius;
	collisionPackage.R3Velocity = gravity;

	// convert velocity to e-space
	eSpaceVelocity = gravity / collisionPackage.eRadius;

	// gravity iteration
	collisionPackage.collisionRecursionDepth = 0;
	finalPosition = CollideWithWorld(finalPosition, velocity);

	// finally set entity position
	position = finalPosition * collisionPackage.eRadius;
#else
	// do gravity iteration
	collisionPackage.R3Velocity = Gravity;
	eSpaceVelocity = Gravity / collisionPackage.eRadius;
	/*glm::vec3 finalPosition = */CollideWithWorld(eSpacePosition, velocity);

	// finally set entity position & velocity
	position = eSpacePosition * collisionPackage.eRadius;
#endif
#endif
}

inline void CharacterEntity::CollideAndSlide2()
{
	// Do collision detection:
	collisionPackage.R3Position = position;
	collisionPackage.R3Velocity = velocity;
	collisionPackage.eRadius = radius;

	// y velocity in a seperate pass
	Vector3 gravity = Vector3{ 0.0f };
	gravity.y = collisionPackage.R3Velocity.y;
	collisionPackage.R3Velocity.y = 0.0f;

	// calculate position and velocity in eSpace
	Vector3 eSpacePosition = collisionPackage.R3Position / collisionPackage.eRadius;
	Vector3 eVelocity = collisionPackage.R3Velocity / collisionPackage.eRadius;

	// do velocity iteration
	CollideWithWorld2(eSpacePosition, eVelocity);

	// do gravity iteration
	collisionPackage.R3Velocity = gravity;
	eVelocity = collisionPackage.R3Velocity / collisionPackage.eRadius;
	CollideWithWorld2(eSpacePosition, eVelocity);

	// finally set entity position & velocity
	position = eSpacePosition * collisionPackage.eRadius;
}

inline void CharacterEntity::CollideWithWorld2(Vector3& e_position, Vector3& e_velocity)
{
	Vector3 dest = e_position + e_velocity;
	Vector3 src = e_position;

	Plane first_plane;

	// check for collision
	Vector3 temp;
	for (int i = 0; i < 3; i++)
	{
		// setup coll packet
		collisionPackage.velocity = e_velocity;
		collisionPackage.normalizedVelocity = e_velocity.GetNormalize();
		collisionPackage.basePoint = e_position;
		collisionPackage.eRadius = radius;
		collisionPackage.foundCollision = false;
		collisionPackage.nearestDistance = FLT_MAX;
		collisionPackage.t = 0.0f;

		// Check for collision (calls the collision routines)
	// Application specific!!
		CheckCollision();
		CheckGrounded();

		// If no collision we just move along the velocity
		if (collisionPackage.foundCollision == false)
		{
			e_position = dest; // TODO: переделать на возврат
			return;
		}

		// point touching tri
		Vector3 touch_point = e_position + e_velocity * collisionPackage.t;

		float dist = sqrtf(DotProduct(e_velocity, e_velocity)) * collisionPackage.t;
		float short_dist = Max(dist - 1.0f, 0.0f);

		e_position = e_position + e_velocity.GetNormalize() * short_dist;

		// Determine the sliding Plane
		Vector3 slidePlaneOrigin = collisionPackage.intersectionPoint; // use intersect point as origin
		// normal = touch_point - intersect_point
		// dont use normal from packet.plane.normal!
		Vector3 slidePlaneNormal = (touch_point - collisionPackage.intersectionPoint).GetNormalize();

		if (i == 0)
		{
			float long_radius = 1.0 + VERY_CLOSE_DIST;
			first_plane = Plane(slidePlaneOrigin, slidePlaneNormal);

			float dist_to_plane = first_plane.SignedDistanceTo(dest) - long_radius;

			dest = dest - first_plane.normal * dist_to_plane;
			e_velocity = dest - e_position;
		}
		else if (i == 1)
		{
			Plane second_plane(slidePlaneOrigin, slidePlaneNormal);

			Vector3 crease = CrossProduct(first_plane.normal, second_plane.normal);
			float dis = DotProduct(dest - e_position, crease);
			crease = crease.GetNormalize();

			e_velocity = crease * dis;
			dest = e_position + e_velocity;
		}
	}

	e_position = dest;
}

void CharacterEntity::CheckGrounded()
{
	if (!collisionPackage.foundCollision)
		return;

	Vector3 axis = Vector3{ 0.0f, 1.0f, 0.0f };

	Vector3 a = collisionPackage.a * radius;
	Vector3 b = collisionPackage.b * radius;
	Vector3 c = collisionPackage.c * radius;

	Plane plane(a, b, c);
	float f = DotProduct(plane.normal, axis);
	if (f >= SLOPE_WALK_ANGLE)
		grounded = 1;
}

inline void CharacterEntity::Update2(float dt)
{
	Vector3 xz = Vector3(velocity.x, 0.0f, velocity.z);
	// prevent sliding while standing still on a slope
	// change < 0.0f to > 0.0f if inverting y axis

	if (grounded && fabs(sqrtf(DotProduct(xz, xz))) < 0.1f && velocity.y < 0.0f)
		velocity.y = 0.0f;
	else
		grounded = 0;

	dt = dt / 5.0;

	velocity = velocity * dt;
	for (int i = 0; i < 5; i++)
		CollideAndSlide2();

	velocity = position - collisionPackage.R3Position;
	velocity = velocity * 1.0f / dt;
}

Vector3 CharacterEntity::CollideWithWorld(const Vector3& pos, const Vector3& vel)
{
	// All hard-coded distances in this function is
	// scaled to fit the setting above..
	float unitScale = unitsPerMeter / 100.0f;
	float veryCloseDistance = 0.0000005f * unitScale;

	// do we need to worry?
	if (collisionPackage.collisionRecursionDepth > 5)
		return pos;

	// Ok, we need to worry:
	collisionPackage.velocity = vel;
	collisionPackage.normalizedVelocity = (vel.GetNormalize());
	collisionPackage.basePoint = pos;
	collisionPackage.foundCollision = false;
	collisionPackage.nearestDistance = FLT_MAX;

	// Check for collision (calls the collision routines)
	// Application specific!!
	CheckCollision();

	// If no collision we just move along the velocity
	if (collisionPackage.foundCollision == false)
	{
		return pos + vel;
	}

	// *** Collision occured ***

	// The original destination point
	Vector3 destinationPoint = pos + vel;
	Vector3 newBasePoint = pos;
	// only update if we are not already very close
	// and if so we only move very close to intersection..not
	// to the exact spot.
	if (collisionPackage.nearestDistance >= veryCloseDistance)
	{
		Vector3 v = (float)Min(vel.GetLength(), collisionPackage.nearestDistance - veryCloseDistance) * vel;
		// TODO: может это вместо выше v.SetLength(collisionPackage->nearestDistance - veryCloseDistance);
		newBasePoint = collisionPackage.basePoint + v;

		// Adjust polygon intersection point (so sliding
		// Plane will be unaffected by the fact that we
		// move slightly less than collision tells us)
		v = v.GetNormalize();
		collisionPackage.intersectionPoint -= veryCloseDistance * v;
	}

	// Determine the sliding Plane
	Vector3 slidePlaneOrigin = collisionPackage.intersectionPoint;
	Vector3 slidePlaneNormal = newBasePoint - collisionPackage.intersectionPoint;
	slidePlaneNormal = slidePlaneNormal.GetNormalize();

	Plane slidingPlane(slidePlaneOrigin, slidePlaneNormal);

	Vector3 newDestinationPoint = destinationPoint - (float)slidingPlane.SignedDistanceTo(destinationPoint) * slidePlaneNormal;

	// Generate the slide vectpr, which will become our new
	// velocity vector for the next iteration
	Vector3 newVelocityVector = newDestinationPoint - collisionPackage.intersectionPoint;

	if (collisionPackage.intersectionPoint.y <= pos.y - collisionPackage.eRadius.y + 0.1f && vel.y <= 0.0f)
	{
		Vector3 a = collisionPackage.a * radius;
		Vector3 b = collisionPackage.b * radius;
		Vector3 c = collisionPackage.c * radius;
		Plane plane(a, b, c);
		Vector3 axis = { 0.0f, 1.0f, 0.0f };
		float f = DotProduct(plane.normal, axis);
		if (f >= SLOPE_WALK_ANGLE)
			grounded = 1;
	}

	// Recurse:

	// dont recurse if the new velocity is very small
	if (newVelocityVector.GetLength() < veryCloseDistance)
	{
		return newBasePoint;
	}

	collisionPackage.collisionRecursionDepth++;

	return CollideWithWorld(newBasePoint, newVelocityVector);
}

void CharacterEntity::CheckCollision()
{
	// сначала тестим все объекты на попадание в AABB
	// в выбранных получаем список треугольников, при этом каждый треугольник хранит (или вычисляется) свой размер - для простоты в виде AABB.
	// теперь проверяем эти треугольники в попадание зоны ААББ объекта-игрока.
	// и вот с полученными уже обрабатываем коллизии
	// при этом если меш - примитив (куб, сфера, плоскость) - то вместо проверки на треугольники делать только проверку на объемы
	// на случай пролета объектов через объект (обычно юзают Continuous Collision Detection) - разбивать движение на итерации с фиксированным расстоянием и циклом каждое проверить - например шаг 0.1, персонаж сдвинулся с 8 до 9, значит не двигать по формуле pos + velocity, где Velocity равно 1. а разделить на десять итераций со сдвигом в 0.1 и в каждую проверять столкновение. Вроде называется Supersampling

	// check collision against triangles

	for( size_t i = 0; i < colliders.size(); i++ )
	{
		for( size_t j = 0; j < colliders[i].poly.verts.size(); j += 3 )
		{
			const Vector3 a = colliders[i].poly.verts[j + 0] / collisionPackage.eRadius;
			const Vector3 b = colliders[i].poly.verts[j + 1] / collisionPackage.eRadius;
			const Vector3 c = colliders[i].poly.verts[j + 2] / collisionPackage.eRadius;

			CheckCollisionsTriangle(&collisionPackage, a, b, c);
		}
	}
}

void CharacterEntity::Update(float dt)
{
	grounded = 0;
	Vector3 gravity = { 0.0f, this->velocity.y, 0.0f };
	CollideAndSlide(gravity);

	Vector3 xz = Vector3{ velocity.x, 0.0f, velocity.z };

	// prevent sliding while standing still on a slope change < 0.0f to > 0.0f if inverting y axis
	if (grounded /* && fabs(glm::length(xz)) < 0.1f && velocity.y < 0.0f*/)
		velocity.y = 0.0f;
	//else
	//	grounded = 0;
}


CharacterEntity* entity;
// size of collision ellipse, experiment with this to change fidelity of detection
static Vector3 boundingEllipse = { 0.5f, 2.0f, 0.5f };

constexpr const char* vertexShaderText = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in vec2 vertexTexCoord;

uniform mat3 NormalMatrix;
uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 fragmentColor;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	gl_Position   = uProjection * uView * uWorld * vec4(vertexPosition, 1.0);
	fragmentColor = vertexColor;
	Normal        = NormalMatrix * vertexNormal;
	TexCoord      = vertexTexCoord;
}
)";
constexpr const char* fragmentShaderText = R"(
#version 330 core

in vec3 fragmentColor;
in vec3 Normal;
in vec2 TexCoord;

struct DirectionalLight
{
	float Ambient, Diffuse;
	vec3 Direction;
};
uniform DirectionalLight Light;

uniform sampler2D Texture;

out vec4 outColor;

void main()
{
	outColor = texture(Texture, TexCoord) * vec4(fragmentColor, 1.0);

	float NdotLD = max(dot(Light.Direction, normalize(Normal)), 0.0); // ламберт
	outColor.rgb *= Light.Ambient + Light.Diffuse * NdotLD;
	//float attenuation = saturate(1.0 - DistanceToLight / LightRadius);
	//frag_Color.rgb *= Light.Ambient + Light.Diffuse * NdotLD * attenuation;
}
)";

ShaderProgram shader;
int uniformNormalMatrix;
int uniformWorldMatrix;
int uniformViewMatrix;
int uniformProjectionMatrix;
Texture2D texture;
Model model;

FlyingCamera cam;

void ExampleInit()
{
	shader.CreateFromMemories(vertexShaderText, fragmentShaderText);
	uniformNormalMatrix = shader.GetUniformLocation("NormalMatrix");
	uniformWorldMatrix = shader.GetUniformLocation("uWorld");
	uniformViewMatrix = shader.GetUniformLocation("uView");
	uniformProjectionMatrix = shader.GetUniformLocation("uProjection");
	shader.Bind();
	shader.SetUniform(shader.GetUniformLocation("Light.Ambient"), 0.333333f);
	shader.SetUniform(shader.GetUniformLocation("Light.Diffuse"), 0.666666f);
	shader.SetUniform(shader.GetUniformLocation("Light.Diffuse"), 0.966666f);
	Vector3 LightDirection = Vector3(0.467757f, 0.424200f, -0.775409f);
	shader.SetUniform(shader.GetUniformLocation("Light.Direction"), LightDirection);
	
	shader.SetUniform(uniformNormalMatrix, Matrix3_old().Inverse().Transpose()); // пока единичную, а так оно такое  NormalMatrix = Matrix3(ModelMatrix).Inverse().Transpose();

	texture.Create("../data/textures/1mx1m.png");

	model.Create("../data/mesh/untitled.obj", "../data/mesh/");
	//model.SetMaterial({ .diffuseTexture = &texture }); // если материала нет

	entity = new CharacterEntity(&model, boundingEllipse);// initialize player infront of model
	entity->position.x = -62;
	entity->position.y = 150;
	entity->position.z = 0;
	
	cam.Look({ -62.0f, 80.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });


	//SetMouseVisible(false);
}

void ExampleClose()
{
	texture.Destroy();
	shader.Destroy();
	model.Destroy();
}

void ExampleFrame()
{
	// input
	{
		auto deltaCursor = GetCursorDelta();
		if( deltaCursor.x != 0 || deltaCursor.y != 0 )
		{
			if( IsMouseButtonDown(MouseButton::Right) )
				cam.OnMouseMove(deltaCursor.x, deltaCursor.y);
		}

#if ENABLE_FPS
		Vector3 PlayerUp = cam.y;
		Vector3 PlayerRight = cam.x;
		Vector3 PlayerForward = cam.z;

		Vector3 PlayerMovement;
		if( IsKeyDown('W')) PlayerMovement += PlayerForward;
		if( IsKeyDown('S') ) PlayerMovement -= PlayerForward;
		if( IsKeyDown('A') ) PlayerMovement -= PlayerRight;
		if( IsKeyDown('D') ) PlayerMovement += PlayerRight;

		PlayerMovement *= 1000.0f * GetDeltaTime();
		entity->velocity.x = PlayerMovement.x;
		//entity->velocity.y = -500.0f * GetDeltaTime();
		entity->velocity.z = PlayerMovement.z;

		static float impulseSpace = 0.0f;
		if( IsKeyPressed('Q') && entity->grounded == 1 && impulseSpace <= 0.0f )
		{
			impulseSpace = 400;
		}
		if( impulseSpace <= 0.0f )
		{
			entity->velocity.y = -900.0f * GetDeltaTime();
		}
		else
		{
			entity->velocity.y = 900.0f * GetDeltaTime();
			impulseSpace -= entity->velocity.y;
		}
#else
		short Keys = 0x0000;
		if( IsKeyDown('W') ) Keys |= CAMERA_KEY_W;
		if( IsKeyDown('S') ) Keys |= CAMERA_KEY_S;
		if( IsKeyDown('A') ) Keys |= CAMERA_KEY_A;
		if( IsKeyDown('D') ) Keys |= CAMERA_KEY_D;
		if( IsKeyDown('R') ) Keys |= CAMERA_KEY_R;
		if( IsKeyDown('F') ) Keys |= CAMERA_KEY_F;
		if( IsKeyDown('Q') ) Keys |= CAMERA_KEY_Q;
		if( IsKeyDown('E') ) Keys |= CAMERA_KEY_E;
		if( IsKeyDown('C') ) Keys |= CAMERA_KEY_E;

		if( IsKeyDown(0x20/*VK_SPACE*/) ) Keys |= CAMERA_KEY_SPACE;
		if( IsKeyDown(0x10/*VK_SHIFT*/) ) Keys |= CAMERA_KEY_SHIFT;
		if( IsKeyDown(0x11/*VK_CONTROL*/) ) Keys |= CAMERA_KEY_CONTROL;

		Vector3 Movement;
		bool MoveCamera = cam.OnKeys(Keys, 70 * GetDeltaTime(), Movement);
		if( MoveCamera ) cam.Move(Movement);

		entity->velocity.x = 0.0f;
		if( IsKeyDown('L') ) entity->velocity.x = 500.0f * GetDeltaTime();
		if( IsKeyDown('J') ) entity->velocity.x = -500.0f * GetDeltaTime();

		entity->velocity.y = -980.0f * GetDeltaTime();

		entity->velocity.z = 0.0f;
		if( IsKeyDown('I') ) entity->velocity.z = 500.0f * GetDeltaTime();
		if( IsKeyDown('K') ) entity->velocity.z = -500.0f * GetDeltaTime();
#endif
	}
	entity->Update2(GetDeltaTime());
#if ENABLE_FPS
	cam.SetPosition(entity->position + Vector3(0.0f, boundingEllipse.y-0.2f, 0.0f));
#endif

	Matrix4_old view = cam.GetViewMatrix();
	Matrix4_old perpective = Perspective(45.0f, GetWindowAspectRatio(), 0.01f, 100000.f);
	Matrix4_old world1;

	shader.Bind();
	shader.SetUniform(uniformViewMatrix, view);
	shader.SetUniform(uniformProjectionMatrix, perpective);
	shader.SetUniform(uniformWorldMatrix, world1);
	model.Draw();

	DebugDraw::DrawLine({ 0.0f, 0.0f, 0.0f }, { -10.0f, 2.0f, 5.0f }, RED);
	DebugDraw::DrawGrid(100);

	DebugDraw::DrawCapsule(
		{ entity->position.x, entity->position.y - boundingEllipse.y/2.0f, entity->position.z },
		{ entity->position.x, entity->position.y + boundingEllipse.y / 2.0f, entity->position.z },
		0.5f, RED);

	DebugDraw::Flush(perpective * view);
}