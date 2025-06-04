#include "imgui.h"

namespace c {

	inline ImVec4 accent = ImColor(208, 15, 52, 255);

	namespace bg
	{
		inline ImVec4 background = ImColor(31, 35, 40, 255);

		inline ImVec4 border = ImColor(38, 42, 47, 255);


		inline ImVec2 size = ImVec2(1200, 800);
		inline float rounding = 6.f;
	}

	namespace child
	{
		inline ImVec4 background = ImColor(35, 39, 44, 255);
		inline float rounding = 6.f;
	}

	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(140, 144, 144, 255);
		inline ImVec4 text = ImColor(106, 109, 112, 255);

		inline ImVec4 text_hide = ImColor(106, 109, 112, 0);
		inline ImVec4 text_faded = ImColor(106, 109, 112, 127);

	}

	namespace textbox
	{
		inline ImVec4 background_inactive = ImColor(31, 35, 40, 255);


		inline float rounding = 2.f;
	}

	namespace checkbox
	{
		inline ImVec4 background_inactive = ImColor(31, 35, 40, 255);

		inline ImVec4 checkmark_active = ImColor(255, 255, 255, 255);
		inline ImVec4 checkmark_inactive = ImColor(255, 255, 255, 0);

		inline float rounding = 2.f;
	}

	namespace drag
	{
		inline ImVec4 background_inactive = ImColor(31, 35, 40, 255);
		inline ImVec4 background_faded = ImColor(31, 35, 40, 127);
	}

	namespace slider
	{
		inline ImVec4 background_inactive = ImColor(31, 35, 40, 255);
		inline ImVec4 navigator_inactive = ImColor(31, 35, 40, 0);

		inline float rounding = 4.f;
	}

	namespace button
	{
		inline ImVec4 background_inactive = ImColor(31, 35, 40, 255);

		inline float rounding = 4.f;
	}

	namespace tab
	{
		inline ImVec4 background_inactive = ImColor(31, 35, 40, 0);
	}

	namespace combo
	{
		inline ImVec4 background_inactive = ImColor(31, 35, 40, 255);

		inline float rounding = 4.f;
	}
}