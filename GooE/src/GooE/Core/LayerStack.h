#pragma once
#include <GooE/Core/Core.h>
#include <GooE/Core/Layer.h>

namespace GooE {

	class GOOE_API LayerStack {
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);
		void PopLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }

	private:
		std::vector<Layer*> layers;
		unsigned int layerInsertIndex = 0;
	};
}
