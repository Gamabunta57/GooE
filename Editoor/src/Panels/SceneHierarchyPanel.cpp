#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <GooE/Scene/Components.h>

namespace GooE {
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> scene) {
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene> context) {
		this->context = context;
		selectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene hierarchy");
		context->registry.each([&](auto entityId) {
			Entity entity{ entityId, context.get() };
			DrawEntityNode(entity);			
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			selectionContext = {};

		//Right click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			if (ImGui::MenuItem("Create new entity"))
				context->CreateEntity("Empty entity");
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (selectionContext)
			DrawComponents(selectionContext);
			
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = (selectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0 )| ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
			selectionContext = entity;

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (entityDeleted) {
			if (selectionContext == entity)
				selectionContext = {};
			context->DestroyEntity(entity);
		}
	}

	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) values.y = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) values.z = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& label, Entity entity, UIFunction uiFunction) {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>()) {
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, label.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open) {
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity) {
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[128];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, 128, tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = std::string(buffer);
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent")) {
			if (ImGui::MenuItem("Camera")) {
				selectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("SpriteRenderer")) {
				selectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			DrawVec3Control("Position", component.translation);

			glm::vec3 rotation = glm::degrees(component.rotation);
			DrawVec3Control("Rotation", rotation);
			component.rotation = glm::radians(rotation);

			DrawVec3Control("Scale", component.scale, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			auto& camera = component.camera;

			ImGui::Checkbox("Primary", &component.primary);
			ImGui::Checkbox("Fixed aspect ratio", &component.fixedAspectRatio);

			const char* projectionType[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionType[(int)component.camera.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionType[i];
					if (ImGui::Selectable(projectionType[i], isSelected)) {
						currentProjectionTypeString = projectionType[i];
						component.camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
				float perspectiveFov = glm::degrees(camera.GetPerspectiveVerticalFov());
				float perspectiveNear = camera.GetPerspectiveNearClip();
				float perspectiveFar = camera.GetPerspectiveFarClip();

				if (ImGui::DragFloat("Fov", &perspectiveFov)) camera.SetPerspectiveVerticalFov(glm::radians(perspectiveFov));
				if (ImGui::DragFloat("Near", &perspectiveNear)) camera.SetPerspectiveNearClip(perspectiveNear);
				if (ImGui::DragFloat("Far", &perspectiveFar)) camera.SetPerspectiveFarClip(perspectiveFar);

			}
			else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				float orthoSize = camera.GetOrthographicSize();
				float orthoNear = camera.GetOrthographicNearClip();
				float orthoFar = camera.GetOrthographicFarClip();

				if (ImGui::DragFloat("Size", &orthoSize)) camera.SetOrthographicSize(orthoSize);
				if (ImGui::DragFloat("Near", &orthoNear)) camera.SetOrthographicNearClip(orthoNear);
				if (ImGui::DragFloat("Far", &orthoFar)) camera.SetOrthographicFarClip(orthoFar);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});
	}
}