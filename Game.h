#ifndef GAME_H
#define GAME_H

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "GameObjectManager.h"
#include "BulletManager.h"
#include "EnnemyTimeline.h"
#include "UiManager.h"
#include "DialogueManager.h"

#include "TextureCache.h"

#include "Random.h"
#include "Shader.h"

class Game
{
public:
	enum GameState {
		uninitialized,
		showingSplash,
		pause,
		showingMenu,
		playing,
		exiting,
	};

	static void start();

	//à changer de place ?
	static void displayDebugUI();

	static const sf::RenderWindow& getWindow() { return m_mainWindow; };
	static const sf::RenderTexture& getUiWindow() { return m_uiWindow; };
	static GameObjectManager& getGameObjectManager() { return m_gameObjectManager; };
	static BulletManager& getBulletManager() { return m_bulletManager; };

	//à changer de place
	static TextureCache& getTextureCache() { return m_textureCache; };

	static GameState getGameState() { return m_gameState; };
	static void setGameState(GameState state) { m_gameState = state; };

	static const int SCREEN_WIDTH{ 960 };
	static const int SCREEN_HEIGHT{ 1120 };


private:
	static bool isExiting();
	static void gameLoop();
	
	static void showPause();
	static void showSplashScreen();
	static void showMenu();
	static void setLetterboxView(int, int);

	static GameState m_gameState;
	static sf::RenderWindow m_mainWindow;
	static sf::View m_view;
	static sf::View m_uiView;

	static sf::RenderTexture m_uiWindow;

	static GameObjectManager m_gameObjectManager;
	static BulletManager m_bulletManager;
	static UiManager m_uiManager;
	static DialogueManager m_dialogueManager;

	static TextureCache m_textureCache;
	static sf::Font m_debugFont;

	static EnnemyTimeline m_ennemyTimeline;

	static sf::Clock m_deltaClock;
	static sf::Clock m_testClock;

	//OpenGL
	static unsigned int VAOs[2];
	static unsigned int shaderProgram;
	static unsigned int shaderProgram2;
	static std::vector<Shader> shaderPrograms;

	static unsigned int textures[2];

	//Camera
	static bool m_freeCameraMode;
	static float m_cameraSpeed;
	static float m_yaw;
	static float m_pitch;


	static glm::vec3 m_cameraPos;
	static glm::vec3 m_cameraFront; //Camera Direction
	static glm::vec3 m_cameraUp;

	static bool m_firstMouse;
};

#endif