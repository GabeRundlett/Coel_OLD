#include "clpch.hpp"
#include "Layer.hpp"

namespace Coel
{
LayerStack::~LayerStack()
{
	for (Layer *l : m_layers)
		delete l;
}

void LayerStack::pushLayer(Layer *l)
{
	m_layerInsert = m_layers.emplace(m_layerInsert, l);
	l->onAttach();
}

void LayerStack::pushOverlay(Layer *l)
{
	m_layers.emplace_back(l);
	l->onAttach();
}

void LayerStack::popLayer(Layer *l)
{
	auto it = std::find(m_layers.begin(), m_layers.end(), l);
	if (it != m_layers.end())
	{
		m_layers.erase(it);
		m_layerInsert--;
	}
	l->onDetach();
}

void LayerStack::popOverlay(Layer *l)
{
	auto it = std::find(m_layers.begin(), m_layers.end(), l);
	if (it != m_layers.end())
		m_layers.erase(it);
	l->onDetach();
}
} // namespace Coel
