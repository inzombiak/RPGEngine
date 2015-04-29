#include "game.h"
#include "LuaStateManager.h"
#include <iostream>

using std::cout;
using std::endl;

void Game::Start()
{
	if (m_gameState != Uninitialized)
		return;
	m_mainWindow.create(sf::VideoMode(m_windowWidth, m_windowHeight, 32), "SFML works!");
	LuaStateManager::Create();
//	m_playerObserver = new PlayerObserver();
	m_renderer = new EntityRenderer(m_mainWindow);
	m_inputManager = new InputManager();
	m_transformManager = new TransformManager();
	m_itemManager = new ItemManager();
	m_physicsManager = new PhysicsManager();
	m_scriptManager = new ScriptManager();
	m_animationManager - new AnimationManager();
	m_itemManager->SetEntityManager(&m_entityManager);
	m_UI = new UIManager();
	m_gameState = ShowingMenu;

	while (m_gameState != Exiting)
	{
		GameLoop();
	}

	Destroy();
	m_mainWindow.close();
}

void Game::CreateEntities()
{
	
}

void Game::ChangeLevel(int levelNumber)
{
	int px, py;
	LevelLoader levelLoader(m_entityManager);
	levelLoader.CreateLevel(levelNumber);
	m_itemManager->LoadItemCatalog("data/MainItemCatalog.xml");
	m_entityManager.LoadEntities("");
	m_inputManager->SetUI(m_UI);
//	m_playerObserver->SetUI(m_UI);
	
	m_gameClock.restart();

	m_renderer->DivideIntoLayers();
	m_UI->PostInit();
}

float Game::GetTime()
{
	return m_gameClock.restart().asSeconds();
}

void Game::GameLoop()
{
	m_mainWindow.clear();
	switch (m_gameState)
	{
		case Game::Playing:
		{
			Update();
			Draw();					
			break;
		}
		case Game::ShowingMenu:
		{
			ShowMenu();
			break;
		}
	}
	m_mainWindow.display();
}

void Game::Update()
{
	sf::Event event;
	
	int frames;
	m_time = GetTime();

	remainingTime += m_time;
	frames = 0;

	while ((remainingTime > m_minTimestep) && (frames < m_maxFrames))
	{
		while (m_mainWindow.pollEvent(event))
		{
			m_inputManager->Update(m_minTimestep, event, m_mainWindow);
			if (event.type == sf::Event::Closed)
			{
				m_gameState = Exiting;
			}
		}
		m_transformManager->Update(m_minTimestep);
		m_physicsManager->Update(m_minTimestep);
		m_animationManager->Update(m_minTimestep);
		m_renderer->Update(m_minTimestep);
		m_scriptManager->Update(m_minTimestep);
		m_entityManager.Update(m_minTimestep);
		m_UI->Update(m_minTimestep);
		remainingTime -= m_minTimestep;
		frames++;
	}
}

void Game::Draw()
{
	m_renderer->Draw();
	m_UI->Draw(m_mainWindow);
}

void Game::Destroy()
{
	if (m_renderer)
		delete m_renderer;
	if (m_inputManager)
		delete m_inputManager;
	if (m_transformManager)
		delete m_transformManager;
	if (m_itemManager)
		delete m_itemManager;
	if(m_physicsManager)
		delete m_physicsManager;
	if (m_UI)
		delete m_UI;
	if (m_scriptManager)
		delete m_scriptManager;
	if (m_animationManager)
		delete m_animationManager;
//	if (m_playerObserver)
//		delete m_playerObserver;
}

void Game::ShowMenu()
{
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.Show(m_mainWindow);
	
	switch (result)
	{
		case MainMenu::Exit:
		{
			m_gameState = Exiting;
			break;
		}
		
		case MainMenu::Play:
		{
			m_gameState = Playing;
			ChangeLevel(2);
			break;
		}	
	}
}

sf::Clock Game::m_gameClock;