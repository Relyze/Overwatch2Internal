#include "../../../includes.h"

namespace render {
	void RenderText(const std::string& text, Vector2 position, ImColor color, bool center, float size)
	{
		ImDrawList* window = ImGui::GetBackgroundDrawList();

		if (center)
			position.x -= ImGui::CalcTextSize(text.c_str()).x / 2;

		ImVec2 shadowOffset[] = { ImVec2(-1, -1), ImVec2(1, -1), ImVec2(-1, 1), ImVec2(1, 1) };
		ImColor shadowColor(0, 0, 0, 255);

		for (const ImVec2& offset : shadowOffset)
			window->AddText(NULL, size, ImVec2(position.x + offset.x, position.y + offset.y), shadowColor, text.c_str());

		window->AddText(NULL, size, ImVec2(position.x, position.y), color, text.c_str());
	}


	void RenderLine(const Vector2& from, const Vector2& to, ImColor color, float thickness)
	{
		ImDrawList* window = ImGui::GetBackgroundDrawList();
		window->AddLine(ImVec2(from.x, from.y), ImVec2(to.x, to.y), color, thickness);
	}

	void RenderStrokeLine(const Vector2& from, const Vector2& to, ImColor color, float thickness)
	{
		ImDrawList* window = ImGui::GetBackgroundDrawList();

		window->AddLine(ImVec2(from.x + 1, from.y + 1), ImVec2(to.x + 1, to.y + 1), ImColor(0, 0, 0, 255), thickness);
		window->AddLine(ImVec2(from.x - 1, from.y - 1), ImVec2(to.x - 1, to.y - 1), ImColor(0, 0, 0, 255), thickness);
		window->AddLine(ImVec2(from.x, from.y), ImVec2(to.x, to.y), color, thickness);
	}

	void RenderCircle(const Vector2& position, float radius, ImColor color, float thickness, uint32_t segments)
	{
		ImDrawList* window = ImGui::GetBackgroundDrawList();
		window->AddCircle(ImVec2(position.x, position.y), radius, color, segments, thickness);
	}

	void RenderCircleFilled(const Vector2& position, float radius, ImColor color, uint32_t segments)
	{
		ImDrawList* window = ImGui::GetBackgroundDrawList();
		window->AddCircleFilled(ImVec2(position.x, position.y), radius, color, segments);
	}

	void RenderRect(const Vector2& from, const Vector2& to, ImColor color, float rounding, uint32_t roundingCornersFlags, float thickness)
	{
		ImDrawList* window = ImGui::GetBackgroundDrawList();
		window->AddRect(ImVec2(from.x, from.y), ImVec2(from.x + to.x, from.y + to.y), color, rounding, roundingCornersFlags, thickness);
	} 

	void RenderRectFilled(const Vector2& from, const Vector2& to, ImColor color, float rounding, uint32_t roundingCornersFlags)
	{
		ImDrawList* window = ImGui::GetBackgroundDrawList();
		window->AddRectFilled(ImVec2(from.x, from.y), ImVec2(from.x + to.x, from.y + to.y), color, rounding, roundingCornersFlags);
	}

	void DrawCorneredBox(const Vector2& from, const Vector2& to, ImColor color, float rounding, float thickness)
	{
		float X = from.x, Y = from.y, W = to.x, H = to.y;
		float lineW = W / 3, lineH = H / 3;
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();

		drawList->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), color, thickness);
		drawList->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), color, thickness);

		drawList->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), color, thickness);
		drawList->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), color, thickness);

		drawList->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), color, thickness);
		drawList->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), color, thickness);

		drawList->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), color, thickness);
		drawList->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), color, thickness);
	}
}

namespace visuals {
	void doOutline(const Entity& entity, const Entity& bestEntity)
	{
		if (!entity.IsValid() || !entity.ModelComponent.IsValid())
			return;

		float ultimateCharge = entity.StateComponent.GetScriptVariable(0x0, SKILL_ULTIMATE_CHARGE).current_value;
		uint8_t flag = entity.getVisibility() ? 0x1 : 0x2;

		ImColor color;
		if (ultimateCharge == 100.f)
			color = config->UltEnemyColor;
		else if (bestEntity.IsValid() && bestEntity == entity)
			color = config->TargetEnemyColor;
		else
			color = config->InvisibleEnemyColor;

		entity.OutlineComponent.SetOutline(flag, color);
	}


	static ImColor GetBoneColor(const Entity& entity, const Entity& BestEntity, const Vector3& bonePosition, const Vector3& camera) {
		float ultimateCharge = entity.StateComponent.GetScriptVariable(0x0, SKILL_ULTIMATE_CHARGE).current_value;

		ImColor color = ImColor(0, 255, 0);
		if (ultimateCharge == 100.f)
			color = config->BoxUltEnemyColor;
		else if (BestEntity.IsValid() && BestEntity == entity)
			color = config->BoxTargetEnemyColor;
		return entity.getVisibility() ? color : ImColor(config->BoxColorInvisible);
	}

	void drawSkeleton(const Entity& entity, const Entity& BestEntity) {
		const Vector3 camera = sdk::GetProjectMatrix().GetCameraVec();
		Matrix vm = sdk::GetMatrix();
		Vector3 previous(0.f, 0.f, 0.f), current(0.f, 0.f, 0.f);
		Vector2 p1, c1;

		for (const auto& bonegroup : skeleton) {
			previous = Vector3(0.f, 0.f, 0.f);
			for (const auto& bone : bonegroup) {
				current = entity.GetBonePos(bone);
				if(current == Vector3(0.f, 0.f, 0.f))
					continue;

				if (previous.x != 0.f && vm.WorldToScreen(previous, &p1) && vm.WorldToScreen(current, &c1)) {
					ImColor color = GetBoneColor(entity, BestEntity, current, camera);
					render::RenderLine(p1, c1, color);
				}
				previous = current;
			}
		}
	}

	// Debug function to draw entity bone ids at their respective positions.
	//void drawBones(const Entity& entity) {
	//		if (!entity.IsValid())
	//			return;

	//		const Vector3 camera = sdk::GetProjectMatrix().GetCameraVec();
	//		Matrix vm = sdk::GetMatrix();
	//		Vector3 current(0.f, 0.f, 0.f);
	//		Vector2 c1;

	//		auto skeleton = entity.ModelComponent.GetSkeleton();
	//		if (skeleton == nullptr || !IsValidPtr((uint64_t)skeleton) || !IsValidPtr((uint64_t)skeleton + 0x28))
	//			return;

	//		uint16_t bonecount = RPM<uint16_t>((uint64_t)skeleton + 0x54);
	//		BoneList* boneList = skeleton->boneList;

	//		for (int i = 0; i < bonecount && i < 1000; i++) {
	//			if (!IsValidPtr((uint64_t)boneList) || !IsValidPtr((uint64_t)boneList + 0x38))
	//				return;

	//			uint32_t boneIndex = entity.ModelComponent.GetBoneIndex(i);
	//			if (boneIndex <= 0)
	//				continue;

	//			Vector3 bone = entity.ModelComponent.GetBonePosition(boneIndex);

	//			if (vm.WorldToScreen(bone, &c1)) {
	//				ImColor color = ImColor(255, 255, 255);
	//				if (i == 17) 
	//					color = ImColor(255, 0, 0);

	//				render::RenderText(std::to_string(i), c1, color, true, 16.f);
	//			}
	//		}
	//	}
	//}

	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2>& p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);
			return h1 ^ h2;
		}
	};

	void drawPrediction(const Entity& localEntity, const Entity& bestEntity) {
		const eHero localHero = localEntity.HeroComponent.HeroID();

		Vector3 location = RPM<Vector3>(globals::CameraManager + 0x18);
		location.z = -location.z;
		Vector3 bone = sdk::GetClosestBone(bestEntity);
		Vector3 origin = bone;

		const float distance = location.Distance(bone);
		float speed = sdk::getProjSpeed(localHero);

		bool isPredicted = (speed < 1000.f || localHero == eHero::HERO_SOJOURN || localHero == eHero::HERO_ANA || localHero == eHero::HERO_SIGMA || localHero == eHero::HERO_SOLDIER76);
		bool IsStormArrow = false;
		Vector3 compensation = Vector3(0.f, 0.f, 0.f);
		if (isPredicted)
		{
			float gravity = 0.f;
			if (localHero == eHero::HERO_HANZO)
			{
				if (!localEntity.StateComponent.IsValid())
					return;

				IsStormArrow = localEntity.StateComponent.GetScriptVariable(0, eScriptVariableIndex::SKILL_2).writer_count;
				if (IsStormArrow)
					speed = 110.f;
				else
				{
					speed = 25.f + 85.f * 1.0f;
				}
			}

			switch (localHero)
			{
			case eHero::HERO_HANZO:
			case eHero::HERO_TORBJORN:
			case eHero::HERO_JUNKRAT:
				gravity = 9.8f * 0.5f * powf(distance / speed, 2.f);
				break;
			default:
				gravity = 0.f;
				break;
			}

			compensation = bestEntity.ModelComponent.Velocity() * (distance / speed);
			compensation.y += gravity;
			bone += compensation;

			Vector2 predictedPosition2D;
			if (sdk::GetMatrix().WorldToScreen(bone, &predictedPosition2D)) {
				const float circleRadius = 5.f;
				Vector2 screenCenter(render::width * 0.5f, render::height * 0.5f);
				Vector2 circleDirection = (predictedPosition2D - screenCenter).Normalize();
				Vector2 circleEdge = predictedPosition2D - circleDirection * circleRadius;

				render::RenderCircle(predictedPosition2D, 5.f, c::accent);
				render::RenderLine(screenCenter, circleEdge, c::accent, 2.f);
			}
		}
	}

	void drawSnapline(const Entity& entity, const Entity& bestEntity, Vector2 headPos, Vector2 rootPos) {
		float height = abs(headPos.y - rootPos.y);
		const float BOX_WIDTH_RATIO = 0.65f;
		float width = height * BOX_WIDTH_RATIO;

		Vector2 boxPosition(headPos.x - (width * 0.5f) - 1, headPos.y - (height * 0.2f) - 1);
		Vector2 boxSize(width + 2, height + 12);

		Vector2 screenCenter(render::width * 0.5f, render::height * 0.5f);

		float ult_charge = entity.StateComponent.GetScriptVariable(0x0, SKILL_ULTIMATE_CHARGE).current_value;
		bool hasUltimate = ult_charge == 100.f;

		ImColor color = entity.getVisibility() ? config->snaplineColor : config->snaplineColorInvisible;
		if(hasUltimate)
			color = config->snaplineUltEnemyColor;

		if (bestEntity == entity)
			color = config->snaplineTargetEnemyColor;

		enum lineType : int {
			Top = 0,
			Bottom = 1,
			Centre = 2
		};

		Vector2 snaplinePosition(headPos.x, headPos.y - (height * 0.2f) - 1 + 12);
		lineType snaplineType = (lineType)config->snaplineType;
		if (snaplineType == Top) {
			snaplinePosition = Vector2(headPos.x, headPos.y - (height * 0.2f) - 1 + height + 12);
			render::RenderLine(Vector2(render::width * 0.5f, render::height), snaplinePosition, color, 1.f);
		}
		else if (snaplineType == Bottom) {
			snaplinePosition = Vector2(headPos.x, headPos.y - (height * 0.2f) - 1);
			render::RenderLine(Vector2(render::width * 0.5f, 0), snaplinePosition, color, 1.f);
		}
		else if (snaplineType == Centre) {
			std::vector<Vector2> corners = {
				boxPosition,
				Vector2(boxPosition.x + boxSize.x, boxPosition.y),
				Vector2(boxPosition.x + boxSize.x, boxPosition.y + boxSize.y),
				Vector2(boxPosition.x, boxPosition.y + boxSize.y),
			};

			std::pair<Vector2, Vector2> closestEdge;
			float closestDistance = std::numeric_limits<float>::max();

			for (size_t i = 0; i < corners.size(); ++i) {
				Vector2 corner1 = corners[i];
				Vector2 corner2 = corners[(i + 1) % corners.size()];

				Vector2 edge = corner2 - corner1;
				Vector2 toCenter = screenCenter - corner1;
				float t = std::max(0.0f, std::min(1.0f, edge.Dot(toCenter) / edge.Dot(edge)));
				Vector2 projection = corner1 + edge * t;
				float distance = (screenCenter - projection).GetLength();

				
				if (distance < closestDistance) {
					closestDistance = distance;
					closestEdge = { corner1, corner2 };
				}
			}

			Vector2 edge = closestEdge.second - closestEdge.first;
			Vector2 toCenter = screenCenter - closestEdge.first;
			float t = std::max(0.0f, std::min(1.0f, edge.Dot(toCenter) / edge.Dot(edge)));
			Vector2 closestPoint = closestEdge.first + edge * t;

			render::RenderLine(screenCenter, closestPoint, color, 1.f);
		}
	}

	void drawBox(const Entity& entity, const Entity& BestEntity, const Vector2& outhead, const Vector2& out) {
		float height = abs(outhead.y - out.y);
		const float BOX_WIDTH_RATIO = 0.65f;
		float width = height * BOX_WIDTH_RATIO;

		if (height < 0 || !std::isfinite(height) || !std::isfinite(width))
			return;

		Matrix vm = sdk::GetMatrix();

		ImColor color = entity.getVisibility() ? config->BoxColor : config->BoxColorInvisible;

		float ult_charge = entity.StateComponent.GetScriptVariable(0x0, SKILL_ULTIMATE_CHARGE).current_value;
		if (ult_charge == 100.f)
			color = config->BoxUltEnemyColor;

		if (BestEntity == entity)
			color = config->BoxTargetEnemyColor;

		float roundingBox = 4.0f;
		Vector2 boxPosition(outhead.x - (width * 0.5f) - 1, outhead.y - (height * 0.2f) - 1);
		Vector2 boxSize(width + 2, height + 12);

		switch (config->boxType) {
		case 0:
			render::DrawCorneredBox(boxPosition, boxSize, color, 0);
			render::RenderRectFilled(boxPosition, boxSize, ImColor(0, 0, 0, 30));
			break;
		case 1:
			render::RenderRect(boxPosition, boxSize, color, 0);
			break;
		case 2:
			render::RenderRect(boxPosition, boxSize, color, roundingBox);
			break;
		default:
			break;
		}
	}

	void drawEntityText(const Entity& entity, const Vector2& root_position, float distance) {
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
		float offset = 0;
		ImColor color(169, 169, 169);

		auto renderText = [&](const std::string& text, float yOffset) {
			render::RenderText(text, Vector2(root_position.x, root_position.y + 5 + yOffset), color, true, 16.f);
			return ImGui::CalcTextSize(text.c_str()).y;
			};

		if (config->distanceHeroName) {
			std::stringstream text;
			text << entity.HeroComponent.GetHeroName() << " [" << static_cast<int>(distance) << "]";

			float ultimateCharge = entity.StateComponent.GetScriptVariable(0x0, SKILL_ULTIMATE_CHARGE).current_value;
			if (ultimateCharge > 0.f)
				text << " [" << std::fixed << std::setprecision(0) << ultimateCharge << "%]";

			offset += renderText(text.str(), offset);
		}

		if (config->BattleTagName)
			offset += renderText(entity.TagComponent.BattleTag(), offset);

		ImGui::PopFont();
	}


	static ImVec4 Lerp(const ImVec4& a, const ImVec4& b, float t)
	{
		return ImVec4(
			a.x + (b.x - a.x) * t,
			a.y + (b.y - a.y) * t,
			a.z + (b.z - a.z) * t,
			a.w + (b.w - a.w) * t
		);
	}

	void drawHealthBar(const Entity& entity, float width, Vector2 from)
	{
		Value Life = entity.HealthComponent.Life();
		float health = (Life.Current / Life.Maximum);

		const ImVec4 colorGreen(0.f, 1.f, 0.f, 1.f);
		const ImVec4 colorYellow(1.f, 1.f, 0.f, 1.f);
		const ImVec4 colorRed(1.f, 0.f, 0.f, 1.f);

		ImVec4 startColor, endColor;
		float t;
		if (health <= 0.f) {
			startColor = endColor = colorRed;
			t = 0.f;
		}
		else if (health >= 1.f) {
			startColor = endColor = colorGreen;
			t = 0.f;
		}
		else {
			startColor = health <= 0.5f ? colorRed : colorYellow;
			endColor = health <= 0.5f ? colorYellow : colorGreen;
			t = (health <= 0.5f ? health : health - 0.5f) * 2.f;
		}
		const ImColor interpolatedColor = Lerp(startColor, endColor, t);

		const float healthbar_start_x = from.x;
		const float healthbar_end_x = healthbar_start_x + (width * health);

		const ImColor blackColor(0.f, 0.f, 0.f, 1.f);
		render::RenderStrokeLine(Vector2(healthbar_start_x, from.y), Vector2(healthbar_start_x + width, from.y), blackColor, 2.f);
		render::RenderStrokeLine(Vector2(healthbar_start_x, from.y), Vector2(healthbar_end_x, from.y), interpolatedColor, 2.f);
	}
}