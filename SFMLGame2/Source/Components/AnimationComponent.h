#pragma once

#include "IComponent.h"

namespace Animation
{
	//Contains the bounds for all the frames of the animation that shares its name with dirName
	//NOTE: SHOULD SPRITE JUST BE REPlACED WITH THIS??
	struct SpriteDefinition
	{
		string dirName; //Name of parent directory
		std::vector<sf::IntRect> bounds; //Contains all the bounds for this animation
	};

	//Actual sprites in the animation, 
	//NOTE: NO REAL POINT TO THIS FOR NOW, CANT HAVE MORE THAN ONE SPRITE ANYWAY
	struct Sprite
	{
		string spriteDir; //Name of the directiory contianing the bounds for this sprite
		int index; //Which element of the SpriteDefinition.bounds vector belongs to this sprite
		int x; //X coordinate of sprite in the frame
		int y; //Y coordinate of sprite in the frame
		int z; //Z coordinate of sprite in the frame
	};

	//Contains delay fro the frame as well as all the sprites in the frame. 
	//NOTE: ONLY ONE SPRITE PER FRAME UNTIL I CAN FIGURE OUT HOW TO ADD MORE
	struct Frame
	{
		int maxDelay; //How much to wait before moving to next frame
		int currDelay = 0; //How many have been waited
		std::vector<Sprite> sprites; //Sprites in frame. NOTE: NO POINT ONLY ONE SPRITE PER FRAME FOR NOW
	};

	//Contians name, number of loops, index of current frame and the frames of the animaiton
	struct Animation
	{
		int loops; //Number of times animation should loop
		string name; //Name of animation
		
		int GetCurrentFrameIndex()
		{
			return currentFrame;
		}

		void NextFrame() //Increments frame, safer to use this than increment currentFrame manually
		{
			//Increment frame delay counter
			frames[currentFrame].currDelay++;
			//If we still need to wait, return
			if (frames[currentFrame].currDelay < frames[currentFrame].maxDelay)
				return;
			//Otherwise, reset the delay counter and move ahead
			frames[currentFrame].currDelay = 0;
			currentFrame++;
			if (currentFrame >= frames.size())
				currentFrame = 0;
		}
		std::vector<Frame> frames; //Contianer for the frames in animation

	private:
		//Current frame of animation
		int currentFrame = 0;
	};
}


class AnimationComponent : public IComponent
{
public:
	
	virtual bool Init(XMLElement* node) override;
	virtual void PostInit() override {};
	void Update(float dt);

	const static char* COMPONENT_NAME;
	const char* GetName()
	{
		return COMPONENT_NAME;
	}

	//Swtiches to animation corresponding to message
	bool SetAnimation(const string& message)
	{
		//Check if message exists
		auto it = m_messageToAnimation.find(message);
		if (it == m_messageToAnimation.end())
			return false;
		if (it->second.compare(m_currentAnimation.name) == 0)
			return true;
		//Check if corresponding animaiton exists
		auto it2 = m_animations.find(it->second);
		if (it2 == m_animations.end())
			return false;
		m_currentAnimation = it2->second;
		return true;
	}

	//Get the name of the current animaiton
	const string& GetCurrentAnimationName()
	{
		return m_currentAnimation.name;
	}

private:

	void UpdateRenderComponenet();//Updates RenderComponenet to the new bounds of the sprite
	//Reads sprite definitions from darkFunction .sprite file
	bool ReadSpriteFile(const string& filename);
	//Reads animations from darkFunction .anim file
	bool ReadAnimationFile(const string& filename);
	
	int counter; //Number of times Update has bee called
	const int maxCounter = 3; //Animations moves forward by a a frame when counters reaches this number

	Animation::Animation m_currentAnimation; //Current naimation being played
	std::map<string, string> m_messageToAnimation; //Mapping for message to animation name
	std::map<string, Animation::SpriteDefinition> m_spriteDefinitions; //Contains texture bounds for each sprite
	std::map<string, Animation::Animation> m_animations; //Contains animaitons
};

