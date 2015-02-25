#include "EntityRenderer.h"

#include "Entity.h"
#include "ComponentBase.h"
#include "RenderComponent.h"


void EntityRenderer::Draw(WeakEntityPtr entity)
{
	StrongEntityPtr strongEntity = ConvertToStrongPtr<Entity>(entity);
	StrongComponentPtr compPtr = ConvertToStrongPtr<ComponentBase>(strongEntity->GetComponent(ComponentBase::GetIDFromName(RenderComponent::COMPONENT_NAME)));
	
	std::shared_ptr<RenderComponent> renderComp = CastComponentToDerived<RenderComponent>(compPtr);
	if (!renderComp)
		return;
	else
		m_renderWindow.draw(renderComp->GetSprite());
}

