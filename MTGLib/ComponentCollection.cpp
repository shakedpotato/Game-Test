#include "ComponentCollection.h"

ComponentCollection::ComponentCollection()
{

}

void ComponentCollection::AddComponentToList(std::weak_ptr<ComponentBase> component)
{
	m_Components.push_back(component);
}

