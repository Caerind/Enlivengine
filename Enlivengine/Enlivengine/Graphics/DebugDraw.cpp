#include <Enlivengine/Graphics/DebugDraw.hpp>

#ifdef ENLIVE_MODULE_GRAPHICS

#include <Enlivengine/Utils/Assert.hpp>

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

void DebugDraw::DrawPoint(const Vector3f& point, const Color& color /*= Colors::Magenta*/)
{
	const Vector3f offset(0.01f);
	DrawBox(point - offset, point + offset, color);
}

void DebugDraw::DrawXZGrid(F32 begin, F32 end, F32 y, F32 interval, const Color& color /*= Colors::Magenta*/)
{
	for (F32 d = begin; d <= end; d += interval)
	{
		DrawLine(Vector3f(d, y, begin), Vector3f(d, y, end), color);
		DrawLine(Vector3f(begin, y, d), Vector3f(end, y, d), color);
	}
}

void DebugDraw::Render(const bgfx::ViewId& viewId)
{
	if (mVertexCount > 0 && kShader.IsValid())
	{
		if (bgfx::isValid(mBuffer))
		{
			bgfx::destroy(mBuffer);
		}
		mBuffer = bgfx::createVertexBuffer(bgfx::makeRef(mVertices, mVertexCount * sizeof(Vertex)), Vertex::kLayout);
		enAssert(bgfx::isValid(mBuffer));

		bgfx::setState(BGFX_STATE_PT_LINES | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_BLEND_ALPHA_TO_COVERAGE);
		bgfx::setVertexBuffer(0, mBuffer);
		kShader.Submit(viewId);

		mVertexCount = 0;
	}
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
	kShader.Destroy();
	return true;
}

} // namespace en

#endif // ENLIVE_MODULE_GRAPHICS