#include "pch.h"
#include "Utils.h"

void Utils::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

// Source: https://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing
int Utils::StringMatch(const char* pattern, const char* string) {
	// Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
	const char* cp = NULL, * mp = NULL;

	while ((*string) && (*pattern != '*')) {
		if ((*pattern != *string) && (*pattern != '?')) {
			return 0;
		}
		pattern++;
		string++;
	}

	while (*string) {
		if (*pattern == '*') {
			if (!*++pattern) {
				return 1;
			}
			mp = pattern;
			cp = string + 1;
		}
		else if ((*pattern == *string) || (*pattern == '?')) {
			pattern++;
			string++;
		}
		else {
			pattern = mp;
			string = cp++;
		}
	}

	while (*pattern == '*') {
		pattern++;
	}
	return !*pattern;
}