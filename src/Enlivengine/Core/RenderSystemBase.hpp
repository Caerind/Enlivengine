#pragma once

#include <Enlivengine/Core/System.hpp>
#include <Enlivengine/Graphics/RenderTarget.hpp>
#include <Enlivengine/Graphics/View.hpp>

#ifdef ENLIVE_TOOL
#include <Enlivengine/Tools/ImGuiEditor.hpp>
#endif // ENLIVE_TOOL

namespace en
{

class RenderSystemBase : public System
{
public:
	virtual void Render(RenderTarget& renderTarget) = 0;

	View& GetCurrentView()
	{
#ifdef ENLIVE_TOOL
		if (ImGuiEditor::IsUsingEditorView())
			return *ImGuiEditor::GetView();
		else
#endif // ENLIVE_TOOL
		return mMainView;
	}

	View& GetMainView() { return mMainView; }
	const View& GetMainView() const { return mMainView; }

protected:
	View mMainView;
};

} // namespace en