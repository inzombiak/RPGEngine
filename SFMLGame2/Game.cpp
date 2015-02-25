#include "game.h"
#include <iostream>

using std::cout;
using std::endl;

void Game::Start()
{
	if (m_gameState != Uninitialized)
		return;
	m_mainWindow.create(sf::VideoMode(m_windowWidth, m_windowHeight, 32), "SFML works!");

	m_levelLoader = new LevelLoader();
	m_gameObjectManager = new GameObjectManager();
	m_playerObserver = new PlayerObserver();

	m_UI = new UI();
	m_UI->SetPlayerMaxHP(100);
	m_UI->SetPlayerCurrentHP(70);
	m_UI->SetPlayerMaxStamina(60);
	m_UI->SetPlayerCurrentStamina(50);

	m_gameState = ShowingMenu;

	while (m_gameState != Exiting)
	{
		GameLoop();
	}

	Destroy();
	m_mainWindow.close();

}

void Game::ChangeLevel(int levelNumber)
{
	int px, py;
	m_levelLoader->CreateLevel(levelNumber);
	m_playerObserver->SetUI(m_UI);
	m_gameObjectManager->LoadEntities("");
	m_gameObjectManager->SetBackgroundTiles(m_levelLoader->GetBackgroundTiles());
	m_gameObjectManager->SetForegroundTiles(m_levelLoader->GetForegroundTiles());
	m_gameObjectManager->SetCollisionBoxes(m_levelLoader->GetCollisionEntities());

	m_gameClock.restart();
}

float Game::GetTime()
{
	return m_gameClock.restart().asSeconds();
}

void Game::GameLoop()
{
	sf::Event event;
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

	while (m_mainWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_gameState = Exiting;
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::I)
			{
				if (!m_IDown)
				{
					m_UI->ToggleInventory();
					m_IDown = true;
				}

			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::I)
			{
				m_IDown = false;
			}
		}
	}
	

	m_mainWindow.display();
}

void Game::Update()
{
	int frames;
	m_maxFrames = 5;
	m_time = GetTime();

	remainingTime += m_time;
	frames = 0;

	while ((remainingTime > m_minTimestep) && (frames < m_maxFrames))
	{

		m_gameObjectManager->Update(m_minTimestep);

		remainingTime -= m_minTimestep;
		frames++;
	}


}

void Game::Draw()
{
	//m_levelLoader->Draw(m_mainWindow);
	//m_gameObjectManager->Draw(m_mainWindow);
	m_gameObjectManager->Draw(m_mainWindow);
	m_UI->Draw(m_mainWindow);
}

void Game::Destroy()
{
	if (m_gameObjectManager)
	{
		delete m_gameObjectManager;
	}
	if (m_levelLoader)
	{
		delete  m_levelLoader;
	}
	if (m_UI)
	{
		delete m_UI;
	}
	/*if (m_itemCatalog)
	{
		m_itemCatalog->Destroy();
		delete m_itemCatalog;
	}*/
	if (m_playerObserver)
	{
		delete m_playerObserver;
	}
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