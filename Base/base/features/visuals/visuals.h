#pragma once

namespace render {
	extern void RenderText(const std::string& text, Vector2 position, ImColor color, bool center = false, float size = 15.f);
	extern void RenderLine(const Vector2& from, const Vector2& to, ImColor color, float thickness = 1.0f);
	extern void RenderStrokeLine(const Vector2& from, const Vector2& to, ImColor color, float thickness = 1.0f);
	extern void RenderCircle(const Vector2& position, float radius, ImColor color, float thickness = 1.0f, uint32_t segments = 0);
	extern void RenderCircleFilled(const Vector2& position, float radius, ImColor color, uint32_t segments = 16);
	extern void RenderRect(const Vector2& from, const Vector2& to, ImColor color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All, float thickness = 1.0f);
	extern void RenderRectFilled(const Vector2& from, const Vector2& to, ImColor color, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All);
	extern void DrawCorneredBox(const Vector2& from, const Vector2& to, ImColor color, float rounding, float thickness = 1.f);
}

namespace visuals
{
	extern void doOutline(const Entity& entity, const Entity& bestEntity);
	extern void drawBox(const Entity& entity, const Entity& BestEntity, const Vector2& outhead, const Vector2& out);
	extern void drawSkeleton(const Entity& entity, const Entity& BestEntity);
	extern void drawBones(const Entity& entity);
	//extern void drawCurrentScripts(const Entity& entity);
	extern void drawPrediction(const Entity& localEntity, const Entity& bestEntity);
	extern void drawSnapline(const Entity& entity, const Entity& bestEntity, Vector2 headPos, Vector2 rootPos);
	extern void drawHealthBar(const Entity& entity, float width, Vector2 from);
	extern void drawEntityText(const Entity& entity, const Vector2& root_position, float distance);
}