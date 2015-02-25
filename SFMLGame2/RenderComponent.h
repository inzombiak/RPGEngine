#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "ComponentBase.h"

class RenderComponent : public ComponentBase
{
public:
	virtual bool Init(const XMLElement* componentNode) override;
	virtual void PostInit() override;
	void Update(float dt);

	const char* GetName()
	{
		return COMPONENT_NAME;
	}

	//Setter and Getters
	void SetPosition(const sf::Vector2f position);

	void SetTexture(const sf::Texture& texture);
	const sf::Texture& GetTexture() const;

	void SetVisible(bool isVisible) ;
	const bool GetVisible() const;

	void SetSprite(const sf::Sprite sprite);
	const sf::Sprite& GetSprite() const;

	void SetTextureRect(const sf::Rect<int> textureBounds);
	const sf::Rect<int> GetTextureRect() const;

	const static char* COMPONENT_NAME;

private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Rect<int> m_textureBounds;
	bool m_visible = true;
};

#endif