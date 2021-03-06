#pragma once

#include <GooE.h>
#include "Panels/SceneHierarchyPanel.h"

namespace GooE {
	class EditoorLayer : public Layer {
	public:
		EditoorLayer();
		virtual ~EditoorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

		virtual void OnImGuiRender() override;

	private:
		bool OnKeyPressed(const KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:

		uint32_t mapWidth, mapHeight;
		glm::vec2 viewportSize = { 0.0f, 0.0f };
		glm::vec4 color = { 0.4f, 0.2f, 0.8f, 1.0f };
		bool viewportFocused = false;
		bool viewportHovered = false;

		OrthographicCameraController cameraController;

		Ref<Texture2D> texture;
		Ref<Texture2D> spriteSheet;
		Ref<SubTexture2D> barrel;
		Ref<FrameBuffer> frameBuffer;

		Ref<Scene> activeScene;

		struct ProfileResult {
			const char* name;
			float time;
		};

		std::unordered_map<char, Ref<SubTexture2D>> textureMap;

		int guizmoType = -1;

		SceneHierarchyPanel sceneHierarchyPanel;
	};
}
