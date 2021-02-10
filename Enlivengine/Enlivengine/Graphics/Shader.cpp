#include <Enlivengine/Graphics/Shader.hpp>

#include <cstdio>

#include <Enlivengine/Utils/Assert.hpp>
#include <Enlivengine/Resources/PathManager.hpp>

namespace en
{

Shader::Shader()
	: mShaderProgram(BGFX_INVALID_HANDLE)
	, mVertexModule(BGFX_INVALID_HANDLE)
	, mFragmentModule(BGFX_INVALID_HANDLE)
{
}

Shader::~Shader()
{
	Destroy();
}

bool Shader::Initialize(const char* vertexModule, const char* fragmentModule /*=nullptr*/)
{
	mVertexModule = CreateModule(vertexModule);
	if (!bgfx::isValid(mVertexModule))
	{
		return false;
	}

	if (fragmentModule != nullptr)
	{
		mFragmentModule = CreateModule(fragmentModule);
		if (!bgfx::isValid(mFragmentModule))
		{
			return false;
		}
	}

	mShaderProgram = bgfx::createProgram(mVertexModule, mFragmentModule, true);
	return bgfx::isValid(mShaderProgram);
}

void Shader::Destroy()
{
	if (bgfx::isValid(mShaderProgram))
	{
		bgfx::destroy(mShaderProgram);
		mShaderProgram = BGFX_INVALID_HANDLE;
	}
	if (bgfx::isValid(mVertexModule))
	{
		bgfx::destroy(mVertexModule);
		mVertexModule = BGFX_INVALID_HANDLE;
	}
	if (bgfx::isValid(mFragmentModule))
	{
		bgfx::destroy(mFragmentModule);
		mFragmentModule = BGFX_INVALID_HANDLE;
	}
}

bool Shader::IsValid() const
{
	return bgfx::isValid(mShaderProgram);
}

bool Shader::HasVertexModule() const
{
	return bgfx::isValid(mVertexModule);
}

bool Shader::HasFragmentModule() const
{
	return bgfx::isValid(mFragmentModule);
}

void Shader::Submit(bgfx::ViewId viewId)
{
	enAssert(IsValid());
	bgfx::submit(viewId, mShaderProgram);
}

bgfx::ShaderHandle Shader::CreateModule(const char* filename)
{
	if (filename == nullptr || strlen(filename) == 0)
	{
		enLogError(LogChannel::Graphics, "Invalid empty shader filename");
		return BGFX_INVALID_HANDLE;
	}

	std::string shaderPath = PathManager::GetShadersPathForRenderer(bgfx::getRendererType());
	if (shaderPath.size() == 0)
	{
		enLogError(LogChannel::Graphics, "Can't get shaders path for renderer: {}", Enum::GetValueName<bgfx::RendererType::Enum>(bgfx::getRendererType()));
		return BGFX_INVALID_HANDLE;
	}

	shaderPath.append(filename);

#ifdef ENLIVE_COMPILER_MSVC
	FILE* file;
	fopen_s(&file, shaderPath.c_str(), "rb");
#else
	FILE* file = fopen(shaderPath.c_str(), "rb");
#endif // ENLIVE_COMPILER_MSVC
	if (file == nullptr)
	{
		enLogError(LogChannel::Graphics, "Invalid shader file: {}", shaderPath);
		return BGFX_INVALID_HANDLE;
	}
	fseek(file, 0, SEEK_END);
	std::size_t fileSize = static_cast<std::size_t>(ftell(file));
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(static_cast<U32>(fileSize + 1));
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	bgfx::ShaderHandle shaderHandle = bgfx::createShader(mem);
	if (bgfx::isValid(shaderHandle))
	{
		bgfx::setName(shaderHandle, filename);
	}
	return shaderHandle;
}

} // namespace en