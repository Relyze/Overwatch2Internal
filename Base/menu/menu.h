#pragma once
#include "../includes.h"
#include "font.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct Image {
	ID3D11ShaderResourceView* view;
	ID3D11Texture2D* tex;
	int width;
	int height;
	int channels;
};

namespace image
{
	static void RenderCurrentHero(std::string local_hero)
	{
		const ImVec2& pos = ImGui::GetWindowPos();
		std::string current_hero = enc("Current Hero: ");

		if (local_hero.empty() || local_hero == enc("None"))
			current_hero = "";
		else
			current_hero = current_hero + local_hero;

		ImGui::GetWindowDrawList()->AddText(pos + ImVec2(c::bg::size.x - 64 - ImGui::CalcTextSize(current_hero.c_str()).x, 10 + ImGui::CalcTextSize(current_hero.c_str()).y), ImGui::GetColorU32(c::text::text_active), current_hero.c_str());
	}
}

static void drawFOV() {
	if (globals::g_MenuState)
		return;

	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	draw->AddCircle(ImVec2(render::width * 0.5f, render::height * 0.5f), config->aimbotFOV, ImColor(169, 169, 169));
}


static LPCSTR AIM_TEAMS[3] = { ("Only Enemy"), ("Only Ally"), ("Both") };
static KeyBindToggle aimkey_bind = KeyBindToggle();
static KeyBindToggle triggerkey_bind = KeyBindToggle();
static bool aimkey_flag = false;
static bool triggerkey_flag = false;
static std::string LastHero;

namespace menu
{
	static int page = 0;
	static int page_two = 0;
	static ID3D11Texture2D* menuTexture = nullptr;
	Image logoData;

	static void Render(bool menuState)
	{
		const Entity localEntity = update::GetLocalEntity();
		eHero hero_id = eHero(0);
		if (localEntity.IsValid())
			hero_id = localEntity.HeroComponent.HeroID();

		const bool IsZen = hero_id == eHero::HERO_ZENYATTA;
		const bool IsGenji = hero_id == eHero::HERO_GENJI;
		const bool IsSoj = hero_id == eHero::HERO_SOJOURN;
		const bool IsTracer = hero_id == eHero::HERO_TRACER;
		const bool IsReaper = hero_id == eHero::HERO_REAPER;

		if (menuState)
		{
			ImGuiStyle* style = &ImGui::GetStyle();

			ImGui::SetNextWindowSize({ c::bg::size + ImVec2(c::bg::rounding, c::bg::rounding) });
			ImGui::Begin(enc("##Lite").c_str(), nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
			{
				const ImVec2& pos = ImGui::GetWindowPos();

				ImGui::GetWindowDrawList()->AddRectFilled(pos + ImVec2(c::bg::rounding, c::bg::rounding), { pos + c::bg::size }, ImGui::GetColorU32(c::bg::background), c::bg::rounding);
				ImGui::GetWindowDrawList()->AddRectFilled(pos + ImVec2(c::bg::rounding, c::bg::rounding), { pos + ImVec2(260, c::bg::size.y) }, ImGui::GetColorU32(c::bg::border), c::bg::rounding, ImDrawCornerFlags_Left);
				ImGui::GetWindowDrawList()->AddRectFilled(pos + ImVec2(260, 0) + ImVec2(0, c::bg::rounding), { pos + ImVec2(c::bg::size.x, 80) }, ImGui::GetColorU32(c::child::background), c::bg::rounding, ImDrawCornerFlags_TopRight);

				image::RenderCurrentHero(globals::g_HeroName);

				ImGui::SetCursorPosY(200);
				ImGui::BeginGroup();
				{
					std::vector<std::string> tab_array = { enc("Combat"), enc("Visuals") };

					for (std::size_t i = 0; i < tab_array.size(); ++i) {
						if (ImGui::Tab(i == page, tab_array[i].c_str(), ImVec2(260, 50))) {
							page = static_cast<int>(i);
						}
					}
				}
				ImGui::EndGroup();

				ImGui::SetCursorPos(ImVec2(265, 0));
				ImGui::BeginGroup();
				{
					std::vector<std::string> subtab_array = { enc("Config") };

					for (std::size_t i = 0; i < subtab_array.size(); ++i) {
						if (ImGui::SubTab(i == page_two, subtab_array[i].c_str(), ImVec2(80, 80))) {
							page_two = static_cast<int>(i);
						}
						ImGui::SameLine();
					}
				}
				ImGui::EndGroup();


				ImDrawList* drawList = ImGui::GetWindowDrawList();
				const int padding = 10;

				ImVec2 tabSize = ImVec2(260, 80);
				ImVec2 local_component_size = ImVec2((c::bg::size - tabSize - style->ItemSpacing * 3));
				local_component_size.x = (local_component_size.x / 2);
				ImVec2 secondChildCursorPosition = ImVec2(tabSize.x + local_component_size.x + style->ItemSpacing.x * 2, tabSize.y + style->ItemSpacing.y);

				switch (page)
				{
				case 0:
					ImGui::SetCursorPos(tabSize + style->ItemSpacing);
					ImGui::BeginGroup();
					{
						ImGui::BeginChild(enc("Aimbot").c_str(), local_component_size);
						{
							ImGui::Separator();
							ImGui::Checkbox(enc("Aimbot").c_str(), &config->Aimbot);
							if (config->Aimbot)
							{
								ImGui::SliderFloat(enc("FOV").c_str(), &config->aimbotFOV, 1.f, 500.f, "%.0f");
								ImGui::SliderFloat(enc("Speed##1").c_str(), &config->aimbotSpeed, 1.f, 100.f, "%.1f %%");
								ImGui::SliderFloat(enc("Min Speed").c_str(), &config->aimbotMinSpeed, 1.f, 100.f, "%.1f %%");
								ImGuiCustom::hotkey(enc("Aim Key").c_str(), config->aimbotKeyID, aimkey_flag);

								ImGui::Combo(enc("Target").c_str(), &config->aimbotTarget, AIM_TEAMS, IM_ARRAYSIZE(AIM_TEAMS));
								ImGui::Combo(enc("Bone").c_str(), &config->boneKeyId, config->boneKeys, IM_ARRAYSIZE(config->boneKeys));
							}

							ImGui::Checkbox(enc("Trigger").c_str(), &config->Trigger);
							if (config->Trigger)
								ImGuiCustom::hotkey(enc("Trigger Key").c_str(), config->triggerbotKeyID, triggerkey_flag);

						}
						ImGui::EndChild();

						ImGui::SetCursorPos(secondChildCursorPosition);
						ImGui::BeginChild(enc("Extended##Aim").c_str(), local_component_size);
						{
							ImGui::Separator();

							ImVec2 cursorPosition = ImGui::GetCursorPos();
							ImGui::SetCursorPos(ImVec2(cursorPosition.x + 20.f, cursorPosition.y));
							ImGui::BeginGroup();
							{
								ImGui::TextColored(c::accent, enc("Aimbot Flags").c_str());
								ImGui::Checkbox(enc("AutoShoot").c_str(), &config->AutoShoot);
								ImGui::Checkbox(enc("Primary").c_str(), &config->Left);
								ImGui::Checkbox(enc("Ignore Shield").c_str(), &config->ignoreShield);
							}
							ImGui::EndGroup();

							ImGui::Separator();

							cursorPosition = ImGui::GetCursorPos();
							ImGui::SetCursorPos(ImVec2(cursorPosition.x + 20.f, cursorPosition.y));
							ImGui::BeginGroup();
							{
								ImGui::TextColored(c::accent, enc("Miscellaneous").c_str());
								ImGui::Checkbox(enc("Auto Melee").c_str(), &config->AutoMelee);

								if (IsGenji) 
								{
									ImGui::Checkbox(enc("Auto Dash").c_str(), &config->GenjiAutoDash);
									if (config->GenjiAutoDash)
										ImGui::SliderFloat(enc("HP").c_str(), &config->GenjiTargetHealth, 1.f, 50.f, "%.0f");
								}

								if (IsTracer) {
									ImGui::Checkbox(enc("Auto Recall").c_str(), &config->AutoRecall);
									ImGui::SliderFloat(enc("Recall HP").c_str(), &config->RecallHealth, 1.f, 175.f, "%.0f");
								}
								
								if (IsSoj)
								{
									ImGui::Checkbox(enc("Auto Railgun").c_str(), &config->SojAutoRightClick);
									if (config->SojAutoRightClick)
										ImGui::Checkbox(enc("Head Only").c_str(), &config->SojAutoRightClickHeadOnly);
								}
							}
							ImGui::EndGroup();
						}
						ImGui::EndChild();

					}
					ImGui::EndGroup();
					break;
				case 1:
					ImGui::SetCursorPos(tabSize + style->ItemSpacing);
					ImGui::BeginGroup();
					{
						ImGui::BeginChild(enc("Visuals").c_str(), local_component_size);
						{
							ImGui::Separator();
							ImGui::Checkbox(enc("Outline ESP").c_str(), &config->Outline);

							ImGui::Separator();
							ImGui::Checkbox(enc("Box ESP").c_str(), &config->Box);
							ImGui::Checkbox(enc("Skeleton ESP").c_str(), &config->Skeleton);
							ImGui::Checkbox(enc("Healthbar").c_str(), &config->healthBar);
							ImGui::Checkbox(enc("Distance + Hero Name").c_str(), &config->distanceHeroName);
							ImGui::Checkbox(enc("Battle Tag").c_str(), &config->BattleTagName);
							ImGui::Checkbox(enc("Prediction ESP").c_str(), &config->PredEsp);
							ImGui::Checkbox(enc("Snaplines").c_str(), &config->Snaplines);
						}
						ImGui::EndChild(); 
						
						ImGui::SetCursorPos(secondChildCursorPosition);
						ImGui::BeginChild(enc("Extended##Visuals").c_str(), local_component_size);
						{
							ImGui::Separator();
							if (config->Outline)
							{
								ImVec2 cursorPosition = ImGui::GetCursorPos();
								ImGui::SetCursorPos(ImVec2(cursorPosition.x + 20.f, cursorPosition.y));
								ImGui::TextColored(c::accent, enc("Outline Settings").c_str());
								ImGui::BeginGroup();
								{
									ImGui::ColorEdit4(enc("\t\t\t\tColor").c_str(), (float*)&config->InvisibleEnemyColor, ImGuiColorEditFlags_NoInputs);
									ImGui::ColorEdit4(enc("\t\t\t\tTarget Color").c_str(), (float*)&config->TargetEnemyColor, ImGuiColorEditFlags_NoInputs);
									ImGui::ColorEdit4(enc("\t\t\t\tUlt Color").c_str(), (float*)&config->UltEnemyColor, ImGuiColorEditFlags_NoInputs);
								}
								ImGui::EndGroup();
								ImGui::Separator();
							}

							if (config->Snaplines)
							{
								ImVec2 cursorPosition = ImGui::GetCursorPos();
								ImGui::SetCursorPos(ImVec2(cursorPosition.x + 20.f, cursorPosition.y));
								ImGui::TextColored(c::accent, enc("Snapline Settings").c_str());

								ImGui::BeginGroup();
								{
									ImGui::Combo(enc("Type##Snapline").c_str(), &config->snaplineType, config->snaplineTypes, IM_ARRAYSIZE(config->snaplineTypes));
									ImGui::ColorEdit4(enc("\t\t\t\tLine Color").c_str(), (float*)&config->snaplineColor, ImGuiColorEditFlags_NoInputs);
									ImGui::ColorEdit4(enc("\t\t\t\tLine Color Invisible").c_str(), (float*)&config->snaplineColorInvisible, ImGuiColorEditFlags_NoInputs);
									ImGui::ColorEdit4(enc("\t\t\t\tLine Enemy Target Color").c_str(), (float*)&config->snaplineTargetEnemyColor, ImGuiColorEditFlags_NoInputs);
									ImGui::ColorEdit4(enc("\t\t\t\tLine Enemy Ult Color").c_str(), (float*)&config->snaplineUltEnemyColor, ImGuiColorEditFlags_NoInputs);
								}
								ImGui::EndGroup();
								ImGui::Separator();
							}

							if (config->Box)
							{
								ImVec2 cursorPosition = ImGui::GetCursorPos();
								ImGui::SetCursorPos(ImVec2(cursorPosition.x + 20.f, cursorPosition.y));
								ImGui::TextColored(c::accent, enc("Box Settings").c_str());
								ImGui::BeginGroup();
								{
									ImGui::Combo(enc("Type##Box").c_str(), &config->boxType, config->Boxes, IM_ARRAYSIZE(config->Boxes));
									ImGui::ColorEdit4(enc("\t\t\t\tColor##Box").c_str(), (float*)&config->BoxColor, ImGuiColorEditFlags_NoInputs);
									ImGui::ColorEdit4(enc("\t\t\t\tColor Invisible").c_str(), (float*)&config->BoxColorInvisible, ImGuiColorEditFlags_NoInputs);
									ImGui::ColorEdit4(enc("\t\t\t\tEnemy Target Color").c_str(), (float*)&config->BoxTargetEnemyColor, ImGuiColorEditFlags_NoInputs);
									ImGui::ColorEdit4(enc("\t\t\t\tEnemy Ult Color").c_str(), (float*)&config->BoxUltEnemyColor, ImGuiColorEditFlags_NoInputs);
								}
								ImGui::EndGroup();
								ImGui::Separator();
							}

							ImVec2 cursorPosition = ImGui::GetCursorPos();
							ImGui::SetCursorPos(ImVec2(cursorPosition.x + 20.f, cursorPosition.y));
							ImGui::BeginGroup();
							{
								ImGui::TextColored(c::accent, enc("Miscellaneous").c_str());
								ImGui::Checkbox(enc("Draw FOV").c_str(), &config->DrawFOV);
							}
							ImGui::EndGroup();
						}
						ImGui::EndChild();
					}
					ImGui::EndGroup();
					break;
				}
			}
		}
	}
}