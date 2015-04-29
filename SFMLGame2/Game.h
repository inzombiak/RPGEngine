#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "EntityManager.h"
#include "TransformManager.h"
#include "InputManager.h"
#include "ItemManager.h"
#include "EntityRenderer.h"
#include "PhysicsManager.h"
#include "ScriptManager.h"
#include "AnimationManager.h"

#include "UIManager.h"
#include "Player.h"
#include "LevelLoader.h"
#include "MainMenu.h"
#include "ItemCatalog.h"
#include "PlayerObserver.h"
#include "LevelLoader.h"


#include <sstream>
class Game
{
public:
	void Destroy();
	void Start();
	float GetTime();
	void ChangeLevel(int);
	void ShowMenu();
	void CreateEntities();

private:
	void Update();
	void Draw();

	enum GameState {Uninitialized, Playing, ShowingMenu, Exiting, Paused};

	int m_windowWidth = 640;
	int m_windowHeight = 480;

	bool m_IDown;
	void GameLoop();

	EntityManager m_entityManager;
	EntityRenderer* m_renderer;
	InputManager* m_inputManager;
	TransformManager* m_transformManager;
	ItemManager* m_itemManager;
	PhysicsManager* m_physicsManager;
	ScriptManager* m_scriptManager;
	AnimationManager* m_animationManager;

	UIManager* m_UI;
	//PlayerObserver* m_playerObserver;

	GameState m_gameState = Uninitialized;
	sf::RenderWindow m_mainWindow;
	static sf::Clock m_gameClock;
	const float m_minTimestep = sf::seconds(1.f/60.f).asSeconds();
	const int m_maxFrames = 5;
	float remainingTime;
	float m_time;

	const string ENTITIES_FILE = "data/Entities.xml";
	const string LEVEL_FILE = "data/levels/level2.tmx";
};
