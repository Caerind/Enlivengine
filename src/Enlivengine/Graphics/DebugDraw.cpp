#include <Enlivengine/Graphics/DebugDraw.hpp>

#include <Enlivengine/Utils/Assert.hpp>

#include <Enlivengine/Graphics/BgfxWrapper.hpp>

namespace en
{

bgfx::VertexLayout DebugDraw::Vertex::kLayout;
Shader DebugDraw::kShader;

DebugDraw::DebugDraw()
	: mVertices()
	, mVertexCount(0)
	, mBuffer(BGFX_INVALID_HANDLE)
{
}

DebugDraw::~DebugDraw()
{
	if (!BgfxWrapper::IsInitialized())
	{
		return;
	}

	if (bgfx::isValid(mBuffer))
	{
		bgfx::destroy(mBuffer);
	}
}

void DebugDraw::DrawLine(const Vector3f& p1, const Vector3f& p2, const Color& color /*= Colors::Magenta*/)
{
	AddVertex(p1, color);
	AddVertex(p2, color);
}

void DebugDraw::DrawBox(const Vector3f& min, const Vector3f& max, const Color& color /*= Colors::Magenta*/)
{
	// Bottom
	DrawLine({ min.x, min.y, min.z }, { max.x, min.y, min.z }, color);
	DrawLine({ min.x, min.y, min.z }, { min.x, min.y, max.z }, color);
	DrawLine({ max.x, min.y, max.z }, { max.x, min.y, min.z }, color);
	DrawLine({ max.x, min.y, max.z }, { min.x, min.y, max.z }, color);

	// Top
	DrawLine({ min.x, max.y, min.z }, { max.x, max.y, min.z }, color);
	DrawLine({ min.x, max.y, min.z }, { min.x, max.y, max.z }, color);
	DrawLine({ max.x, max.y, max.z }, { max.x, max.y, min.z }, color);
	DrawLine({ max.x, max.y, max.z }, { min.x, max.y, max.z }, color);

	// Sides
	DrawLine({ min.x, min.y, min.z }, { min.x, max.y, min.z }, color);
	DrawLine({ max.x, min.y, min.z }, { max.x, max.y, min.z }, color);
	DrawLine({ max.x, min.y, max.z }, { max.x, max.y, max.z }, color);
	DrawLine({ min.x, min.y, max.z }, { min.x, max.y, max.z }, color);
}

void DebugDraw::DrawSphere(const Vector3f& center, F32 radius, const Color& color /*= Colors::Magenta*/)
{
	// http://www.songho.ca/opengl/gl_sphere.html
	// This code use UnitZ as UpVector, so it has been changed a bit

	const U32 sectorCount = 8;
	const U32 stackCount = 8;

	const F32 sectorStep = 360.0f / sectorCount;
	const F32 stackStep = 180.0f / stackCount;

	for (U32 i = 0; i < stackCount; ++i)
	{
		const F32 stackAngle = 90.0f - i * stackStep;
		const F32 stackAngleNext = 90.0f - (i + 1) * stackStep;
		const F32 t = radius * Math::Cos(stackAngle);
		const F32 z = radius * Math::Sin(stackAngle);
		const F32 tNext = radius * Math::Cos(stackAngleNext);
		const F32 zNext = radius * Math::Sin(stackAngleNext);

		for (U32 j = 0; j < sectorCount; ++j)
		{
			const F32 sectorAngle = j * sectorStep;
			const F32 sectorAngleNext = (j + 1) * sectorStep;

			const F32 x = t * Math::Cos(sectorAngle);
			const F32 y = t * Math::Sin(sectorAngle);
			const F32 xNext = t * Math::Cos(sectorAngleNext);
			const F32 yNext = t * Math::Sin(sectorAngleNext);
			const F32 xNext2 = tNext * Math::Cos(sectorAngle);
			const F32 yNext2 = tNext * Math::Sin(sectorAngle);

			DrawLine(center + Vector3f(x, z, y), center + Vector3f(xNext, z, yNext), color);
			DrawLine(center + Vector3f(x, z, y), center + Vector3f(xNext2, zNext, yNext2), color);
		}
	}
}

void DebugDraw::DrawCross(const Vector3f& position)
{
	DrawLine(position, position + Vector3f::UnitX(), Colors::Red);
	DrawLine(position, position + Vector3f::UnitY(), Colors::Green);
	DrawLine(position, position + Vector3f::UnitZ(), Colors::Blue);
}

void DebugDraw::DrawTransform(const Matrix4f& transform)
{
	const Vector3f center = transform.TransformPoint(Vector3f::Zero());
	DrawLine(center, transform.TransformPoint(Vector3f::UnitX()), Colors::Red);
	DrawLine(center, transform.TransformPoint(Vector3f::UnitY()), Colors::Green);
	DrawLine(center, transform.TransformPoint(Vector3f::UnitZ()), Colors::Blue);
}

void DebugDraw::DrawPoint(const Vector3f& point, const Color& color /*= Colors::Magenta*/)
{
	const Vector3f offset(0.01f);
	DrawBox(point - offset, point + offset, color);
}

void DebugDraw::DrawGrid(const Vector3f& point, const Vector3f& up, F32 begin, F32 end, F32 interval, const Color& color /*= Colors::Magenta*/)
{
	Vector3f right = up.Cross(ENLIVE_DEFAULT_UP);
	if (right.GetSquaredLength() < 0.05f)
	{
		right = up.Cross(ENLIVE_DEFAULT_RIGHT);
	}
	const Vector3f forward = up.Cross(right);

	const Vector3f beginRight = begin * right + point;
	const Vector3f endRight = end * right + point;
	const Vector3f beginForward = begin * forward + point;
	const Vector3f endForward = end * forward + point;

	for (F32 d = begin; d <= end; d += interval)
	{
		const Vector3f dRight = right * d;
		const Vector3f dForward = forward * d;
		DrawLine(dRight + beginForward, dRight + endForward, color);
		DrawLine(dForward + beginRight, dForward + endRight, color);
	}
}

void DebugDraw::DrawAABB(const AABB& aabb, const Color& color /*= Colors::Magenta*/)
{
	DrawBox(aabb.GetMin(), aabb.GetMax(), color);
}

void DebugDraw::DrawFrustum(const Frustum& frustum, const Color& color /*= Colors::Magenta*/)
{
	// Near
	DrawLine(frustum.GetCorner(Frustum::Corners::NBL), frustum.GetCorner(Frustum::Corners::NBR), color);
	DrawLine(frustum.GetCorner(Frustum::Corners::NBL), frustum.GetCorner(Frustum::Corners::NTL), color);
	DrawLine(frustum.GetCorner(Frustum::Corners::NTR), frustum.GetCorner(Frustum::Corners::NBR), color);
	DrawLine(frustum.GetCorner(Frustum::Corners::NTR), frustum.GetCorner(Frustum::Corners::NTL), color);

	// Far
	DrawLine(frustum.GetCorner(Frustum::Corners::FBL), frustum.GetCorner(Frustum::Corners::FBR), color);
	DrawLine(frustum.GetCorner(Frustum::Corners::FBL), frustum.GetCorner(Frustum::Corners::FTL), color);
	DrawLine(frustum.GetCorner(Frustum::Corners::FTR), frustum.GetCorner(Frustum::Corners::FBR), color);
	DrawLine(frustum.GetCorner(Frustum::Corners::FTR), frustum.GetCorner(Frustum::Corners::FTL), color);

	// Sides
	DrawLine(frustum.GetCorner(Frustum::Corners::FBL), frustum.GetCorner(Frustum::Corners::NBL), color);
	DrawLine(frustum.GetCorner(Frustum::Corners::FTL), frustum.GetCorner(Frustum::Corners::NTL), color);
	DrawLine(frustum.GetCorner(Frustum::Corners::FBR), frustum.GetCorner(Frustum::Corners::NBR), color);
	DrawLine(frustum.GetCorner(Frustum::Corners::FTR), frustum.GetCorner(Frustum::Corners::NTR), color);
}

void DebugDraw::DrawPlane(const Plane& plane, const Color& color /*= Colors::Magenta*/)
{
	const Vector3f point = plane.GetAnyPoint();
	const Vector3f up = plane.GetNormal();
	DrawGrid(point, plane.GetNormal(), -5.0f, 50.0f, 1.0f, color);
	DrawLine(point, point + up, color);
}

void DebugDraw::DrawRay(const Ray& ray, const Color& color /*= Colors::Magenta*/)
{
	DrawLine(ray.GetOrigin(), ray.GetPoint(200.0f), color);
}

void DebugDraw::DrawSphere(const Sphere& sphere, const Color& color /*= Colors::Magenta*/)
{
	DrawSphere(sphere.GetCenter(), sphere.GetRadius(), color);
}

void DebugDraw::Render()
{
	if (BgfxWrapper::IsInitialized() && mVertexCount > 0 && kShader.IsValid())
	{
		if (bgfx::isValid(mBuffer))
		{
			bgfx::destroy(mBuffer);
		}
		mBuffer = bgfx::createVertexBuffer(bgfx::makeRef(mVertices, mVertexCount * sizeof(Vertex)), Vertex::kLayout);
		enAssert(bgfx::isValid(mBuffer));

		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_PT_LINES);
		bgfx::setVertexBuffer(0, mBuffer);
		kShader.Submit(BgfxWrapper::GetCurrentView());
	}
}

void DebugDraw::Clear()
{
	mVertexCount = 0;
}

void DebugDraw::AddVertex(const Vector3f& pos, const Color& color)
{
	if (mVertexCount < kMaxVertices)
	{
		mVertices[mVertexCount].pos = pos;
		mVertices[mVertexCount].color = color;
		mVertexCount++;
	}
}

bool DebugDraw::InitializeDebugDraws()
{
	enAssert(BgfxWrapper::IsInitialized());

	Vertex::kLayout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();

	if (!kShader.Initialize("debugdraw.vs.bin", "debugdraw.fs.bin"))
	{
		enLogWarning(LogChannel::Graphics, "Can't initialize DebugDraws shader");
		return false;
	}

	return true;
}

bool DebugDraw::ReleaseDebugDraws()
{
	if (!BgfxWrapper::IsInitialized())
	{
		return true;
	}

	kShader.Destroy();
	return true;
}

} // namespace en