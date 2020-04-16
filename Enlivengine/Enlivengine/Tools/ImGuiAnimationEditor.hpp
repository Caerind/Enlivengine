#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#if defined(ENLIVE_ENABLE_IMGUI)

#include <Enlivengine/Application/ImGuiToolManager.hpp>

#include <Enlivengine/Graphics/Animation.hpp>
#include <Enlivengine/Graphics/AnimationController.hpp>
#include <Enlivengine/Graphics/AnimationStateMachine.hpp>

#include <imgui-node-editor/imgui_node_editor.h>
#include <imgui/imgui_internal.h>

namespace en
{

class ImGuiAnimationEditor : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiAnimationEditor);
	~ImGuiAnimationEditor();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

    virtual void Display();

	bool Initialize(AnimationStateMachinePtr stateMachinePtr);
	void Uninitialize();
	bool IsInitialized() const;

	int GetStateID(const AnimationStateMachine::State& state, const AnimationStateMachine& stateMachine);
	int GetTransitionID(const AnimationStateMachine::Transition& transition, const AnimationStateMachine& stateMachine);
	int GetTransitionInputID(const AnimationStateMachine::Transition& transition, const AnimationStateMachine& stateMachine);
	int GetTransitionOutputID(const AnimationStateMachine::Transition& transition, const AnimationStateMachine& stateMachine);

private:
	void LeftPanel(AnimationStateMachine& stateMachine);
    void StateMachine(AnimationStateMachine& stateMachine);
	void Selection(AnimationStateMachine& stateMachine);
	void SelectedNode(AnimationStateMachine& stateMachine, ax::NodeEditor::NodeId node);
	void SelectedLink(AnimationStateMachine& stateMachine, ax::NodeEditor::LinkId link);
	void NewState(AnimationStateMachine& stateMachine);
	void NewTransition(AnimationStateMachine& stateMachine);
	void NewParameter(AnimationStateMachine& stateMachine);
	void ParametersList(AnimationStateMachine& stateMachine);
	void Debug(AnimationStateMachine& stateMachine);
	void Preview(AnimationStateMachine& stateMachine);

	void NodeEditor(AnimationStateMachine& stateMachine);
	struct PinInfo
	{
		int id;
		ImVec2 pos;
	};
	void GetStatePinPositions(const AnimationStateMachine& stateMachine, const U32 stateIndex, const std::vector<ImRect>& stateRects, std::vector<PinInfo>& inputPins, std::vector<PinInfo>& outputPins);
	ImVec2 GetCorrectedPinPosition(const ImVec2& pinPos, const ImVec2& otherPinPos, const ImRect& pinRect);
	bool GetLineIntersection(const ImVec2& lineA1, const ImVec2& lineA2, const ImVec2& lineB1, const ImVec2& lineB2, ImVec2& intersection);

	void AnimationStateMachineList();
	
private:
	AnimationStateMachinePtr mStateMachine;
    ax::NodeEditor::EditorContext* mEditorContext;

	// Left Panel elements
	static constexpr U32 kBufferSize{ 256 };
	char mStateInputTextBuffer[kBufferSize];
	char mParameterInputTextBuffer[kBufferSize];

	AnimationController mAnimationController;

    bool mDisplayPreviews; 
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
