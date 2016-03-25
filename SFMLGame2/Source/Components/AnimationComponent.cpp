#include "..\Entity.h"
#include "AnimationComponent.h"
#include "RenderComponent.h"

bool AnimationComponent::Init(XMLElement* node)
{
	if (!node->Attribute("sprites"))
		return false;
	if(!ReadSpriteFile(node->Attribute("sprites")))
		return false;
	
	if (!node->Attribute("animations"))
		return false;
	if(!ReadAnimationFile(node->Attribute("animations")))
		return false;

	auto animations = node->FirstChildElement("Animations");
	auto currAnimation = animations->FirstChildElement("Animation");
	string message, animationName;
	while (currAnimation)
	{
		if (!currAnimation->Attribute("name"))
			return false;
		animationName = currAnimation->Attribute("name");
		if (!currAnimation->Attribute("message"))
			return false;
		message = currAnimation->Attribute("message");
		m_messageToAnimation[message] = animationName;

		currAnimation = currAnimation->NextSiblingElement("Animation");
	}

	SetAnimation("Idle");
	return true;
}

bool AnimationComponent::ReadSpriteFile(const string& filename)
{
	//TODO FILE NOT FOUND

	tinyxml2::XMLDocument spriteFile;
	if (spriteFile.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		return false;

	XMLElement* pImg = spriteFile.FirstChildElement("img");
	XMLElement* pDefs = pImg->FirstChildElement("definitions");
	XMLElement* pBaseDir = pDefs->FirstChildElement("dir");
	XMLElement* pDir = pBaseDir->FirstChildElement("dir");
	XMLElement* pSprite;
	sf::IntRect spriteBounds;
	Animation::SpriteDefinition newSpriteDir;

	while (pDir)
	{
		newSpriteDir.bounds.clear();
		if (!pDir->Attribute("name"))
			return false;
		newSpriteDir.dirName = pDir->Attribute("name");
		
		pSprite = pDir->FirstChildElement("spr");
		
		while (pSprite)
		{
			
			if (pSprite->QueryIntAttribute("x", &spriteBounds.left) != tinyxml2::XMLError::XML_SUCCESS)
				return false;
			if (pSprite->QueryIntAttribute("y", &spriteBounds.top) != tinyxml2::XMLError::XML_SUCCESS)
				return false;
			if (pSprite->QueryIntAttribute("w", &spriteBounds.width) != tinyxml2::XMLError::XML_SUCCESS)
				return false;
			if (pSprite->QueryIntAttribute("h", &spriteBounds.height) != tinyxml2::XMLError::XML_SUCCESS)
				return false;
			newSpriteDir.bounds.push_back(spriteBounds);
			pSprite = pSprite->NextSiblingElement("spr");
		}
		m_spriteDefinitions[newSpriteDir.dirName] = newSpriteDir;
		pDir = pDir->NextSiblingElement("dir");
	}

	return true;
}

bool AnimationComponent::ReadAnimationFile(const string& filename)
{
	//TODO FILE NOT FOUND
	tinyxml2::XMLDocument animationFile;
	if (animationFile.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		return false;

	XMLElement* pAnimations = animationFile.FirstChildElement("animations");
	XMLElement* pAnim = pAnimations->FirstChildElement("anim");
	XMLElement* pCell;
	XMLElement* pSprite;
	Animation::Animation newAnimation;
	Animation::Frame newFrame;
	Animation::Sprite newSprite;
	string trash;
	std::vector<string> trashVec;
	while (pAnim)
	{
		newAnimation.frames.clear();
		if (!pAnim->Attribute("name"))
			return false;
		newAnimation.name = pAnim->Attribute("name");
		if (pAnim->QueryIntAttribute("loops", &newAnimation.loops) != tinyxml2::XMLError::XML_SUCCESS)
			return false;
		pCell = pAnim->FirstChildElement("cell");
		while (pCell)
		{
			newFrame.sprites.clear();
			if (pCell->QueryIntAttribute("delay", &newFrame.maxDelay) != tinyxml2::XMLError::XML_SUCCESS)
				return false;

			pSprite = pCell->FirstChildElement("spr");
			while(pSprite)
			{
				if (pSprite->QueryIntAttribute("x", &newSprite.x) != tinyxml2::XMLError::XML_SUCCESS)
					return false;
				if (pSprite->QueryIntAttribute("y", &newSprite.y) != tinyxml2::XMLError::XML_SUCCESS)
					return false;
				if (pSprite->QueryIntAttribute("z", &newSprite.z) != tinyxml2::XMLError::XML_SUCCESS)
					return false;
				if (!pSprite->Attribute("name"))
					return false;
				trash = pSprite->Attribute("name");

				//Seperate directory from index
				trash.erase(trash.begin(), trash.begin()+1);
				SplitString(trash, trashVec, '/');
				newSprite.spriteDir = trashVec[0];
				newSprite.index = atoi(trashVec[1].c_str());

				newFrame.sprites.push_back(newSprite);
				pSprite = pSprite->NextSiblingElement("spr");
			}
			newAnimation.frames.push_back(newFrame);
			pCell = pCell->NextSiblingElement("cell");
		}
		m_animations[newAnimation.name] = newAnimation;
		pAnim = pAnim->NextSiblingElement("anim");
	}
	return true;
}

void AnimationComponent::Update(float dt)
{
	counter++;
	if (counter >= maxCounter)
	{
		m_currentAnimation.NextFrame();
		UpdateRenderComponenet();
		counter = 0;
	}
}

void AnimationComponent::UpdateRenderComponenet()
{
	std::shared_ptr<RenderComponent> renderComp;
	if (!CheckConvertAndCastPtr(m_owner->GetComponent(GetIDFromName(RenderComponent::COMPONENT_NAME)), renderComp))
		return;
	Animation::Frame currentFrame = m_currentAnimation.frames[m_currentAnimation.GetCurrentFrameIndex()];
	sf::IntRect currentBounds = m_spriteDefinitions[currentFrame.sprites[0].spriteDir].bounds[currentFrame.sprites[0].index];
	renderComp->SetTextureRect(currentBounds);
}

const char* AnimationComponent::COMPONENT_NAME = "AnimationComponent";