#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_GRAPHICS

#include <bgfx/bgfx.h>

#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>

#include <Enlivengine/Math/AABB.hpp>
#include <Enlivengine/Math/Frustum.hpp>
#include <Enlivengine/Math/Plane.hpp>
#include <Enlivengine/Math/Ray.hpp>
#include <Enlivengine/Math/Sphere.hpp>

#include <Enlivengine/Graphics/Shader.hpp>

namespace en
{

class DebugDraw
{
public:
	DebugDraw();
	~DebugDraw();

	// Primitive shapes
	void DrawLine(const Vector3f& p1, const Vector3f& p2, const Color& color = Colors::Magenta);
	void DrawBox(const Vector3f& min, const Vector3f& max, const Color& color = Colors::Magenta);
	void DrawSphere(const Vector3f& center, F32 radius, const Color& color = Colors::Magenta);
	void DrawCross(const Vector3f& position);
	void DrawTransform(const Matrix4f& transform);
	void DrawPoint(const Vector3f& point, const Color& color = Colors::Magenta);
	void DrawGrid(const Vector3f& point, const Vector3f& up, F32 begin, F32 end, F32 interval, const Color& color = Colors::Magenta);

	// Shapes
	void DrawAABB(const AABB& aabb, const Color& color = Colors::Magenta);
	void DrawFrustum(const Frustum& frustum, const Color& color = Colors::Magenta);
	void DrawPlane(const Plane& plane, const Color& color = Colors::Magenta);
	void DrawRay(const Ray& ray, const Color& color = Colors::Magenta);
	void DrawSphere(const Sphere& sphere, const Color& color = Colors::Magenta);

	void Render(const bgfx::ViewId& viewId);

private:
	void AddVertex(const Vector3f& pos, const Color& color);

private:
	struct Vertex
	{
		Vector3f pos;
		Color color;

		static bgfx::VertexLayout kLayout;
	};

	static constexpr U32 kMaxVertices = 1000;

	Vertex mVertices[kMaxVertices];
	U32 mVertexCount;
	bgfx::VertexBufferHandle mBuffer;

private:

	static Shader kShader;

public:
	// TODO : Make these private
	//friend class BgfxWrapper;
	static bool InitializeDebugDraws();
	static bool ReleaseDebugDraws();
};

} // namespace en

#endif // ENLIVE_MODULE_GRAPHICS