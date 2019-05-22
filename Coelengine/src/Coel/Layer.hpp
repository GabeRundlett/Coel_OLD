#pragma once
#include "Core.hpp"
#include "Events/Event.hpp"

namespace Coel
{
class Layer
{
public:
	Layer() {}
	virtual ~Layer() {}
	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onEvent(Event &event) {}
};

class LayerStack
{
	std::vector<Layer *> m_layers;
	std::vector<Layer *>::iterator m_layerInsert;

public:
	LayerStack()
		: m_layerInsert(m_layers.begin()) {}
	~LayerStack();

	void pushLayer(Layer *l);
	void pushOverlay(Layer *l);
	void popLayer(Layer *l);
	void popOverlay(Layer *l);

	std::vector<Layer *>::iterator begin() { return m_layers.begin(); }
	std::vector<Layer *>::iterator end() { return m_layers.end(); }
};
} // namespace Coel
