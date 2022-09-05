#include "pch.h"
#include "RLLobby.h"


std::string RLLobby::GetPluginName()
{
	return "RLLobby";
}

void RLLobby::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> RLLobby
void RLLobby::RenderSettings()
{
	ImGui::TextUnformatted("RLLobby plugin settings");
}


// Do ImGui rendering here
void RLLobby::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string RLLobby::GetMenuName()
{
	return "RLLobby";
}

// Title to give the menu
std::string RLLobby::GetMenuTitle()
{
	return menuTitle_;
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool RLLobby::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool RLLobby::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void RLLobby::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void RLLobby::OnClose()
{
	isWindowOpen_ = false;
}
