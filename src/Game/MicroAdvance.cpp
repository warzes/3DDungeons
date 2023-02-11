#include "MicroAdvance.h"
#include "MicroEngine.h"
//=============================================================================
// Header
//=============================================================================
#if defined(_MSC_VER)
#	pragma warning(push)
#endif // _MSC_VER

#include <map>
#include <unordered_map>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER
//-----------------------------------------------------------------------------
//=============================================================================
// Global var
//=============================================================================
//-----------------------------------------------------------------------------
std::unordered_map<std::string, Texture2D> FileTextures2D;
std::unordered_map<std::string, Model> FileModel;
//-----------------------------------------------------------------------------
//=============================================================================
// ResourceCacheSystem
//=============================================================================
//-----------------------------------------------------------------------------
ShaderProgram* ResourceCacheSystem::LoadShaderProgram(const char* fileName)
{
	assert(false);
	return nullptr;
}
//-----------------------------------------------------------------------------
Texture2D* ResourceCacheSystem::LoadTexture2D(const char* fileName, const Texture2DInfo& textureInfo)
{
	auto it = FileTextures2D.find(fileName);
	if( it != FileTextures2D.end() )
	{
		return &it->second;
	}
	else
	{
		LogPrint("Load texture: " + std::string(fileName));

		Texture2D texture;
		if( !texture.Create(fileName, textureInfo) || !texture.IsValid() )
			return nullptr;

		FileTextures2D[fileName] = texture;
		return &FileTextures2D[fileName];
	}
}
//-----------------------------------------------------------------------------
Model* ResourceCacheSystem::LoadModel(const char* fileName)
{
	auto it = FileModel.find(fileName);
	if (it != FileModel.end())
	{
		return &it->second;
	}
	else
	{
		LogPrint("Load model: " + std::string(fileName));

		Model model;
		if (!model.Create(fileName) || !model.IsValid())
			return nullptr;

		FileModel[fileName] = model;
		return &FileModel[fileName];
	}
}
//-----------------------------------------------------------------------------
bool ResourceCacheSystem::IsLoad(const ShaderProgram& shader)
{
	return false;
}
//-----------------------------------------------------------------------------
bool ResourceCacheSystem::IsLoad(const Texture2D& texture)
{
	for (auto it = FileTextures2D.begin(); it != FileTextures2D.end(); ++it)
	{
		if (texture == it->second)
			return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
void ResourceCacheSystem::Clear()
{
	for( auto it = FileTextures2D.begin(); it != FileTextures2D.end(); ++it )
		it->second.Destroy();
	FileTextures2D.clear();

	for (auto it = FileModel.begin(); it != FileModel.end(); ++it)
		it->second.Destroy();
	FileModel.clear();
}
//-----------------------------------------------------------------------------
//=============================================================================
// Debug Draw
//=============================================================================
//-----------------------------------------------------------------------------
ShaderProgram shaderProgram;
int MatrixID;
int ColorID;
GLuint vao, vbo;
std::map<unsigned, std::vector<Vector3>> Points;
std::map<unsigned, std::vector<Vector3>> Lines;
//-----------------------------------------------------------------------------
// TODO: можно оптимизировать, если хранить цвет в вершине, тогда не нужно использовать мап, можно использовать массив который только растет (а сбрасывается только счетчик). но займет больше памяти. хотя и н сильно
//-----------------------------------------------------------------------------
void drawGround(float scale)
{ // 10x10
	// outer
	for (float i = -scale, c = 0; c <= 20; c += 20, i += c * (scale / 10))
	{
		DebugDraw::DrawLine({ -scale, 0, i }, { scale, 0, i }, WHITE); // horiz
		DebugDraw::DrawLine({ i, 0, -scale }, { i, 0, +scale }, WHITE); // vert
	}
	// inner
	for (float i = -scale + scale / 10, c = 1; c < 20; ++c, i += (scale / 10))
	{
		DebugDraw::DrawLine({ -scale, 0, i }, { +scale, 0, i }, GRAY); // horiz
		DebugDraw::DrawLine({ i, 0, -scale }, { i, 0, +scale }, GRAY); // vert
	}
}
//-----------------------------------------------------------------------------
void DrawConeLowres(const Vector3& center, const Vector3& top, float radius, unsigned rgb)
{
	Vector3 diff3 = top - center;
	DebugDraw::DrawPrism(center, radius ? radius : 1, diff3.GetLength(), diff3.GetNormalize(), 3, rgb);
}
//-----------------------------------------------------------------------------
void DrawCircleWithOrientation(const Vector3& center, Vector3 dir, float radius, unsigned rgb)
{
	// we'll skip 3 segments out of 32. 1.5 per half circle.
	int segments = 32, skip = 3, drawn_segments = segments - skip;

	//  dir = norm3(dir);
	Vector3 right = CrossProduct(dir, { 0.0f, 1.0f, 0.0f });
	Vector3 up = CrossProduct(dir, right);
	right = CrossProduct(dir, up);

	Vector3 point, lastPoint;
	dir = dir * radius;
	right = right * radius;
	//lastPoint = (center + dir);

	{
		const float radians = (PI * 2) * (0 + skip / 2.f) / segments;
		Vector3 vs = right * sinf(radians);
		Vector3 vc = dir * cosf(radians);
		lastPoint = center + vs;
		lastPoint = lastPoint + vc;
	}

	DebugDraw::DrawLine(lastPoint, (center + (dir * radius * 1.25)), rgb);

	for (int i = 0; i <= drawn_segments; ++i)
	{
		const float radians = (PI * 2) * (i + skip / 2.f) / segments;

		Vector3 vs = (right * sinf(radians));
		Vector3 vc = (dir * cosf(radians));

		point = (center + vs);
		point = (point + vc);

		DebugDraw::DrawLine(lastPoint, point, rgb);
		lastPoint = point;
	}

	DebugDraw::DrawLine(lastPoint, (center + (dir * radius * 1.25)), rgb);
}
//-----------------------------------------------------------------------------

inline void  orthoVec(Vector3* left, Vector3* up, Vector3 v)
{
#if 0
	if ((v.z * v.z) > (0.7f * 0.7f)) {
		float sqrlen = v.y * v.y + v.z * v.z;
		float invlen = 1.f / sqrtf(sqrlen);
		*up = Vector3(0, v.z * invlen, -v.y * invlen);
		*left = Vector3(sqrlen * invlen, -v.x * up->z, v.x * up->y);
	}
	else {
		float sqrlen = v.x * v.x + v.y * v.y;
		float invlen = 1.f / sqrtf(sqrlen);
		*left = Vector3(-v.y * invlen, v.x * invlen, 0);
		*up = Vector3(-v.z * left->y, v.z * left->x, sqrlen * invlen);
	}
#else
	* left = (v.z * v.z) < (v.x * v.x) ? Vector3(v.y, -v.x, 0) : Vector3(0, -v.z, v.y);
	*up = CrossProduct(*left, v);
#endif
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawPoint(const Vector3& from, unsigned rgb)
{
	Points[rgb].push_back(from);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawLine(const Vector3& from, const Vector3& to, unsigned rgb)
{
	Lines[rgb].push_back(from);
	Lines[rgb].push_back(to);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawLineDashed(Vector3 from, Vector3 to, unsigned rgb)
{
	Vector3 dist = (to - from);
	Vector3 unit = dist.GetNormalize();
	for (float len = 0, mag = dist.GetLength() / 2; len < mag; ++len)
	{
		to = (from + unit);
		DrawLine(from, to, rgb);
		from = (to + unit);
	}
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawAxis(float units)
{
	DrawLine(Vector3(0, 0, 0), Vector3(units, 0, 0), RED);
	DrawLineDashed(Vector3(0, 0, 0), Vector3(-units, 0, 0), RED);

	DrawLine(Vector3(0, 0, 0), Vector3(0, units, 0), GREEN);
	DrawLineDashed(Vector3(0, 0, 0), Vector3(0, -units, 0), GREEN);

	DrawLine(Vector3(0, 0, 0), Vector3(0, 0, units), BLUE);
	DrawLineDashed(Vector3(0, 0, 0), Vector3(0, 0, -units), BLUE);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawGround(float scale)
{
	if (scale)
	{
		drawGround(scale);
	}
	else
	{
		drawGround(100);
		drawGround(10);
		drawGround(1);
		drawGround(0.1);
		drawGround(0.01);
	}
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawGrid(float scale)
{
	DrawGround(scale);
	DrawAxis(scale ? scale : 100.0f);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawTriangle(const Vector3& pa, const Vector3& pb, const Vector3& pc, unsigned rgb)
{
	DrawLine(pa, pb, rgb);
	DrawLine(pa, pc, rgb);
	DrawLine(pb, pc, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawArrow(const Vector3& begin, const Vector3& end, unsigned rgb)
{
	Vector3 diff = (end - begin);
	float len = diff.GetLength(), stick_len = len * 2 / 3;

	DrawLine(begin, end, rgb);
	DrawConeLowres((begin + (diff.GetNormalize() * stick_len)), end, len / 6, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawBounds(const Vector3 points[8], unsigned rgb)
{
	for (int i = 0; i < 4; ++i)
	{
		DrawLine(points[i], points[(i + 1) & 3], rgb);
		DrawLine(points[i], points[4 + i], rgb);
		DrawLine(points[4 + i], points[4 + ((i + 1) & 3)], rgb);
	}
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawBox(const Vector3& c, const Vector3& extents, unsigned rgb)
{
	Vector3 points[8], whd = (extents * 0.5f);
#define DD_BOX_V(v, op1, op2, op3) (v).x = c.x op1 whd.x; (v).y = c.y op2 whd.y; (v).z = c.z op3 whd.z
	DD_BOX_V(points[0], -, +, +);
	DD_BOX_V(points[1], -, +, -);
	DD_BOX_V(points[2], +, +, -);
	DD_BOX_V(points[3], +, +, +);
	DD_BOX_V(points[4], -, -, +);
	DD_BOX_V(points[5], -, -, -);
	DD_BOX_V(points[6], +, -, -);
	DD_BOX_V(points[7], +, -, +);
#undef DD_BOX_V
	DrawBounds(points, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawCube(const Vector3& center, float radius, unsigned rgb)
{
	// draw_prism(center, 1, -1, vec3(0,1,0), 4);
	float half = radius * 0.5f;
	Vector3 l = Vector3(center.x - half, center.y + half, center.z - half); // left-top-far
	Vector3 r = Vector3(center.x + half, center.y - half, center.z + half); // right-bottom-near

	DrawLine(l, Vector3(r.x, l.y, l.z), rgb);
	DrawLine(Vector3(r.x, l.y, l.z), Vector3(r.x, l.y, r.z), rgb);
	DrawLine(Vector3(r.x, l.y, r.z), Vector3(l.x, l.y, r.z), rgb);
	DrawLine(Vector3(l.x, l.y, r.z), l, rgb);
	DrawLine(l, Vector3(l.x, r.y, l.z), rgb);

	DrawLine(r, Vector3(l.x, r.y, r.z), rgb);
	DrawLine(Vector3(l.x, r.y, r.z), Vector3(l.x, r.y, l.z), rgb);
	DrawLine(Vector3(l.x, r.y, l.z), Vector3(r.x, r.y, l.z), rgb);
	DrawLine(Vector3(r.x, r.y, l.z), r, rgb);
	DrawLine(r, Vector3(r.x, l.y, r.z), rgb);

	DrawLine(Vector3(l.x, l.y, r.z), Vector3(l.x, r.y, r.z), rgb);
	DrawLine(Vector3(r.x, l.y, l.z), Vector3(r.x, r.y, l.z), rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawPlane(const Vector3& p, const Vector3& n, float scale, unsigned rgb)
{
	// if n is too similar to up vector, use right. else use up vector
	Vector3 v1 = CrossProduct(n, DotProduct(n, Vector3(0, 1, 0)) > 0.8f ? Vector3(1, 0, 0) : Vector3(0, 1, 0));
	Vector3 v2 = CrossProduct(n, v1);

	// draw axis
	DrawLine(p, (p + n), rgb);
	DrawLine(p, (p + v1), rgb);
	DrawLine(p, (p + v2), rgb);

	// get plane coords
	v1 = (v1 * scale);
	v2 = (v2 *  scale);
	Vector3 p1 = ((p + v1) + v2);
	Vector3 p2 = ((p - v1) + v2);
	Vector3 p3 = ((p - v1) - v2);
	Vector3 p4 = ((p + v1) - v2);

	// draw plane
	DrawLine(p1, p2, rgb);
	DrawLine(p2, p3, rgb);
	DrawLine(p3, p4, rgb);
	DrawLine(p4, p1, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawSphere(const Vector3& center, float radius, unsigned rgb)
{
	float lod = 6, yp = -radius, rp = 0, y, r, x, z;
	for (int j = 1; j <= lod / 2; ++j, yp = y, rp = r)
	{
		y = j * 2.f / (lod / 2) - 1;
		r = cosf(y * 3.14159f / 2) * radius;
		y = sinf(y * 3.14159f / 2) * radius;

		float xp = 1, zp = 0;
		for (int i = 1; i <= lod; ++i, xp = x, zp = z)
		{
			x = 3.14159f * 2 * i / lod;
			z = sinf(x);
			x = cosf(x);

			Vector3 a1 = (center + Vector3(xp * rp, yp, zp * rp));
			Vector3 b1 = (center + Vector3(xp * r, y, zp * r));
			Vector3 c1 = (center + Vector3(x * r, y, z * r));

			DrawLine(a1, b1, rgb);
			DrawLine(b1, c1, rgb);
			DrawLine(c1, a1, rgb);

			Vector3 a2 = (center + Vector3(xp * rp, yp, zp * rp));
			Vector3 b2 = (center + Vector3(x * r, y, z * r));
			Vector3 c2 = (center + Vector3(x * rp, yp, z * rp));

			DrawLine(a2, b2, rgb);
			DrawLine(b2, c2, rgb);
			DrawLine(c2, a2, rgb);
		}
	}
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawCapsule(const Vector3& from, const Vector3& to, float r, unsigned rgb)
{
	/* calculate axis */
	Vector3 up, right, forward;
	forward = (to - from);
	forward = forward.GetNormalize();
	orthoVec(&right, &up, forward);

	/* calculate first two cone verts (buttom + top) */
	Vector3 lastf, lastt;
	lastf = (up * r);
	lastt = (to + lastf);
	lastf = (from + lastf);

	/* step along circle outline and draw lines */
	enum { step_size = 20 };
	for (int i = step_size; i <= 360; i += step_size)
	{
		/* calculate current rotation */
		Vector3 ax = (right * sinf(i * DEG2RAD));
		Vector3 ay = (up * cosf(i * DEG2RAD));

		/* calculate current vertices on cone */
		Vector3 tmp = (ax + ay);
		Vector3 pf = (tmp * r);
		Vector3 pt = (tmp * r);

		pf = (pf + from);
		pt = (pt + to);

		/* draw cone vertices */
		DrawLine(lastf, pf, rgb);
		DrawLine(lastt, pt, rgb);
		DrawLine(pf, pt, rgb);

		lastf = pf;
		lastt = pt;

		/* calculate first top sphere vert */
		Vector3 prevt = (tmp * r);
		Vector3 prevf = (prevt + from);
		prevt = (prevt + to);

		/* sphere (two half spheres )*/
		for (int j = 1; j < 180 / step_size; j++)
		{
			/* angles */
			float ta = j * step_size;
			float fa = 360 - (j * step_size);

			/* top half-sphere */
			ax = (forward * sinf(ta * DEG2RAD));
			ay = (tmp * cosf(ta * DEG2RAD));

			Vector3 t = (ax + ay);
			pf = (t * r);
			pf = (pf + to);
			DrawLine(pf, prevt, rgb);
			prevt = pf;

			/* bottom half-sphere */
			ax = (forward * sinf(fa * DEG2RAD));
			ay = (tmp * cosf(fa * DEG2RAD));

			t = (ax + ay);
			pf = (t * r);
			pf = (pf + from);
			DrawLine(pf, prevf, rgb);
			prevf = pf;
		}
	}
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawDiamond(const Vector3& from, const Vector3& to, float size, unsigned rgb)
{

	class poly // TODO: удалить?
	{
	public:
		std::vector<Vector3> verts;
		int cnt = 0;

		static poly Pyramid(const Vector3& from, const Vector3& to, float size)
		{
			/* calculate axis */
			Vector3 up, right, forward = (to - from).GetNormalize();
			orthoVec(&right, &up, forward);

			/* calculate extend */
			Vector3 xext = (right * size);
			Vector3 yext = (up * size);
			Vector3 nxext = (right * -size);
			Vector3 nyext = (up * -size);

			/* calculate base vertices */
			poly p;
			p.verts = {
				((from + xext) + yext), /*a*/
				((from + xext) + nyext), /*b*/
				((from + nxext) + nyext), /*c*/
				((from + nxext) + yext), /*d*/
				to, /*r*/
				{} // empty
			};
			p.cnt = 5; /*+1 for diamond case*/ // array_resize(p.verts, 5+1); p.cnt = 5;

			return p;
		}

		static poly Diamond(const Vector3& from, const Vector3& to, float size)
		{
			Vector3 mid = (from + ((to - from) * 0.5f));
			poly p = Pyramid(mid, to, size);
			p.verts[5] = from; p.cnt = 6;
			return p;
		}
	};


	poly p = poly::Diamond(from, to, size);
	Vector3* dmd = p.verts.data();

	Vector3* a = dmd + 0;
	Vector3* b = dmd + 1;
	Vector3* c = dmd + 2;
	Vector3* d = dmd + 3;
	Vector3* t = dmd + 4;
	Vector3* f = dmd + 5;

	/* draw vertices */
	DrawLine(*a, *b, rgb);
	DrawLine(*b, *c, rgb);
	DrawLine(*c, *d, rgb);
	DrawLine(*d, *a, rgb);

	/* draw roof */
	DrawLine(*a, *t, rgb);
	DrawLine(*b, *t, rgb);
	DrawLine(*c, *t, rgb);
	DrawLine(*d, *t, rgb);

	/* draw floor */
	DrawLine(*a, *f, rgb);
	DrawLine(*b, *f, rgb);
	DrawLine(*c, *f, rgb);
	DrawLine(*d, *f, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawPyramid(const Vector3& center, float height, int segments, unsigned rgb)
{
	DrawPrism(center, 1, height, Vector3(0, 1, 0), segments, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawPrism(const Vector3& center, float radius, float height, const Vector3& normal, int segments, unsigned rgb)
{
	Vector3 left = Vector3{ 0 }, up = Vector3{ 0 };
	orthoVec(&left, &up, normal);

	Vector3 point, lastPoint;
	up = (up * radius);
	left = (left * radius);
	lastPoint = (center + up);
	Vector3 pivot = (center + (normal * height));

	for (int i = 1; i <= segments; ++i)
	{
		const float radians = (PI * 2) * i / segments;

		Vector3 vs = (left * sinf(radians));
		Vector3 vc = (up * cosf(radians));

		point = (center + vs);
		point = (point + vc);

		DrawLine(lastPoint, point, rgb);
		if (height > 0)
			DrawLine(point, pivot, rgb);
		else if (height < 0)
		{
			DrawLine(point, (point + (normal * -height)), rgb);
		}
		lastPoint = point;
	}

	if (height < 0)
		DrawPrism((center + (normal * -height)), radius, 0, normal, segments, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawSquare(const Vector3& pos, float radius, unsigned rgb)
{
	DrawPrism(pos, radius, 0, Vector3(0, 1, 0), 4, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawCylinder(const Vector3& center, float height, int segments, unsigned rgb)
{
	DrawPrism(center, 1, -height, Vector3(0, 1, 0), segments, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawPentagon(const Vector3& pos, float radius, unsigned rgb)
{
	DrawPrism(pos, radius, 0, Vector3(0, 1, 0), 5, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawHexagon(const Vector3& pos, float radius, unsigned rgb)
{
	DrawPrism(pos, radius, 0, Vector3(0, 1, 0), 6, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawCone(const Vector3& center, const Vector3& top, float radius, unsigned rgb)
{
	Vector3 diff3 = (top - center);
	DrawPrism(center, radius ? radius : 1, diff3.GetLength(), diff3.GetNormalize(), 24, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawCircle(const Vector3& pos, const Vector3& n, float radius, unsigned rgb)
{
	DrawPrism(pos, radius, 0, n, 32, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawAABB(const Vector3& minbb, const Vector3& maxbb, unsigned rgb)
{
	Vector3 points[8], bb[2] = { minbb, maxbb };
	for (int i = 0; i < 8; ++i)
	{
		points[i].x = bb[(i ^ (i >> 1)) & 1].x;
		points[i].y = bb[(i >> 1) & 1].y;
		points[i].z = bb[(i >> 2) & 1].z;
	}
	DrawBounds/*_corners*/(points, rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawPosition(const Vector3& pos, float radius)
{
	DrawPositionDir(pos, Vector3(0, 0, 0), radius);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawPositionDir(const Vector3& position, const Vector3& direction, float radius)
{
	// idea from http://www.cs.caltech.edu/~keenan/m3drv.pdf and flotilla game UI

	Vector3 ground = Vector3(position.x, 0, position.z);
	unsigned clr = position.y < 0 ? PINK/*ORANGE*/ : CYAN;

	DrawPoint(ground, clr);
	DrawPoint(position, clr);
	(position.y < 0 ? DrawLineDashed(ground, position, clr) : DrawLine(ground, position, clr));

	Vector3 n = direction.GetNormalize(), up = Vector3(0, 1, 0);
	for (int i = 0; i < 10 && i <= fabs(position.y); ++i)
	{
		if (i < 2 && direction.GetLength())
			DrawCircleWithOrientation(ground, n, radius, clr);
		else
			DrawCircle(ground, up, radius, clr);
		radius *= 0.9f;
	}
}//-----------------------------------------------------------------------------

void DebugDraw::DrawNormal(const Vector3& pos, const Vector3& n)
{
	DrawLine(pos, (pos + n.GetNormalize()), YELLOW);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawBone(const Vector3& center, const Vector3& end, unsigned rgb)
{
	Vector3 diff3 = (end - center);
	float len = diff3.GetLength(), len10 = len / 10;
	DrawPrism(center, len10, 0, Vector3(1, 0, 0), 24, rgb);
	DrawPrism(center, len10, 0, Vector3(0, 1, 0), 24, rgb);
	DrawPrism(center, len10, 0, Vector3(0, 0, 1), 24, rgb);
	DrawLine(end, (center + Vector3(0, +len10, 0)), rgb);
	DrawLine(end, (center + Vector3(0, -len10, 0)), rgb);
}
//-----------------------------------------------------------------------------
void DebugDraw::DrawBoid(const Vector3& position, Vector3 dir)
{
	dir = dir.GetNormalize();

	// if n is too similar to up vector, use right. else use up vector
	Vector3 v1 = CrossProduct(dir, DotProduct(dir, Vector3(0, 1, 0)) > 0.8f ? Vector3(1, 0, 0) : Vector3(0, 1, 0));
	Vector3 v2 = CrossProduct(dir, v1);
	v1 = CrossProduct(dir, v2);

	uint32_t clr = position.y < 0 ? ORANGE : CYAN;

	Vector3 front = (position + (dir * 1));
	Vector3 back = (position + (dir * -0.25f));
	Vector3 right = (back + (v1 * 0.5f));
	Vector3 left = (back + (v1 * -0.5f));
	DrawLine(front, left, clr);
	DrawLine(left, position, clr);
	DrawLine(position, right, clr);
	DrawLine(right, front, clr);
}
//-----------------------------------------------------------------------------
void DebugDraw::Flush(const Matrix4& ViewProj)
{
	if (Points.empty() && Lines.empty())
		return;

	shaderProgram.Bind();
	shaderProgram.SetUniform(MatrixID, ViewProj);

	// TODO: сделать интерфейс
	VertexArrayBuffer::UnBind();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_PROGRAM_POINT_SIZE); // for GL_POINTS
	//glEnable(GL_LINE_SMOOTH); // for GL_LINES (thin)

	// Draw Points
	{
		//glPointSize(6);
		for (auto& it : Points)
		{
			shaderProgram.SetUniform(ColorID, RGBToVec(it.first));
			const size_t count = it.second.size();
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vector3), it.second.data(), GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, count);
		}
		//glPointSize(1);
	}

	// Draw Lines
	{
		for (auto& it : Lines)
		{
			shaderProgram.SetUniform(ColorID, RGBToVec(it.first));
			const size_t count = it.second.size();
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vector3), it.second.data(), GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, count);
		}
	}

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glDisable(GL_LINE_SMOOTH);
	//glDisable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(0);

	Points.clear(); // TODO: надо по другому - без реальной очистки памяти
	Lines.clear(); // TODO: надо по другому - без реальной очистки памяти
}
//-----------------------------------------------------------------------------
bool DebugDraw::Init()
{
	const char* vertexSource = R"(
#version 330 core
layout(location = 0) in vec3 vertexPosition;
uniform mat4 MVP;
uniform vec3 u_color;
out vec3 out_color;
void main()
{
	gl_Position =  MVP * vec4(vertexPosition, 1);
	out_color = u_color;
}
)";

	const char* fragmentSource = R"(
#version 330 core
in vec3 out_color;
out vec4 fragcolor;
void main()
{
	fragcolor = vec4(out_color, 1.0);
}
)";
	if (!shaderProgram.CreateFromMemories(vertexSource, fragmentSource))
		return false;
	MatrixID = shaderProgram.GetUniformLocation("MVP");
	ColorID = shaderProgram.GetUniformLocation("u_color");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	return true;
}
//-----------------------------------------------------------------------------
void DebugDraw::Close()
{
	shaderProgram.Destroy();
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
//-----------------------------------------------------------------------------