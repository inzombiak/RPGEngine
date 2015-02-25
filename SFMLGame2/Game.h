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

#include "UI.h"
#include "Player.h"
#include "GameObjectManager.h"
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

private:
	void Update();
	void Draw();

	enum GameState {Uninitialized, Playing, ShowingMenu, Exiting};

	int m_windowWidth = 640;
	int m_windowHeight = 480;

	bool m_IDown;
	void GameLoop();

	GameObjectManager* m_gameObjectManager;
	LevelLoader* m_levelLoader;
	
	UI* m_UI;
	PlayerObserver* m_playerObserver;

	GameState m_gameState = Uninitialized;
	sf::RenderWindow m_mainWindow;
	static sf::Clock m_gameClock;

	const float m_minTimestep = sf::seconds(1.f/60.f).asSeconds();
	int m_maxFrames = 5;
	float remainingTime;
	float m_time;
};
