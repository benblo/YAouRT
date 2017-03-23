#include <imgui.h>

namespace ImGui
{
	static ImVector<bool> propertyStack;

	static bool BeginProperty( const char* label, bool hasContent = false )
	{
		bool node_open = false;

		ImGui::PushID(label);

		// field
		ImGui::AlignFirstTextHeightToWidgets();
		if (hasContent)
		{
			node_open = ImGui::TreeNode(label);
		}
		else
		{
			ImGui::TreeAdvanceToLabelPos();
			ImGui::Selectable(label);
		}
		ImGui::NextColumn();

		// value
		ImGui::AlignFirstTextHeightToWidgets();
		if (hasContent)
		{
			ImGui::NextColumn();
		}
		// else: draw your thing, NextColumn, EndProperty

		propertyStack.push_back(node_open);
		return node_open;
	}
	static void EndProperty()
	{
		if (propertyStack.back())
		{
			ImGui::TreePop();
		}
		propertyStack.pop_back();

		ImGui::PopID();
	}
}
