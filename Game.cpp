#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include "stb_image.h"

#include "Game.h"
#include "SplashScreen.h"
#include "MainMenu.h"

#include "Player.h"
#include "Boss.h"
#include "MathVector.h"
#include "Bullet.h"
#include "BulletPattern.h"

#include "SFMLSoundProvider.h"
#include "ServiceLocator.h"

#include "TextureCache.h"

#include <string>
#include "BulletTypes.h"
#include "Character.h"


const char* vertexShaderSource{
	"#version 330 core\n"
	"layout (location=0) in vec3 aPos;\n"
	"layout (location=1) in vec3 aColor;\n"
	"layout (location=2) in vec2 aTexCoord;\n"
	"out vec3 outColor;\n"
	"out vec2 TexCoord;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
	"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	"outColor = aColor;\n"
	"TexCoord = aTexCoord;\n"
	"}\0"
};

const char* fragmentShaderSource{
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 ourColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = ourColor;\n"
	"}\0"
};

const char* fragmentShaderSource2{
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec4 outColor;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D myTexture;\n"
	"void main()\n"
	"{\n"
	"FragColor = texture(myTexture, TexCoord);\n"
	"}\0"
};

using MoveTimeline = std::vector<std::pair<std::pair<MathVector, MathVector>, sf::Time>>;
using PatternQueue = std::vector<std::pair<BulletPattern*, sf::Time>>;

struct BulletPatterns
{
	Bullet nullBullet{};
	BulletPattern nullPattern{ &nullBullet };

	//Joroide
	Bullet aimedBulletJ{ 0,900.0, Bullet::aimed };
	BulletPattern aimedSinglePatternJ{ &aimedBullet };

	//Gros
	Bullet bullet{ 40.f, 300.0, Bullet::fixed };
	BulletPattern pattern{ &bullet, 3000.f };
	Bullet reverseBullet{ 130.f, 300.0, Bullet::fixed };
	BulletPattern reversePattern{ &reverseBullet, -3000.f };

	Bullet aimedBullet{ 0,500.0, Bullet::aimed};
	BulletPattern aimedSinglePattern{ &aimedBullet };
	BulletPattern grosAimedSinglePattern{ &aimedBullet };

	Bullet randomBullet{ 0,750.0, Bullet::random};
	BulletPattern randomCirclePattern{ &randomBullet };
	BulletPattern randomSinglePattern{ &randomBullet};

	//Guepos 
	Bullet wavyBullet{ 0,750.0, Bullet::fixed, 60.f, 10.f };
	Bullet aimedWavyBullet{ 0,750.0, Bullet::aimed, 60.f, 10.f};
	
	BulletPattern aimedWavySinglePattern{ &aimedWavyBullet, BulletPattern::fixReference, 0.f };
	BulletPattern referenceWavySinglePattern{ &wavyBullet, BulletPattern::useReference, 0.f };

	Bullet curvedBullet{ 0,750.0, Bullet::random, 0.2f};
	BulletPattern randomCurvedSinglePattern{ &curvedBullet };
	BulletPattern curvedCirclePattern{ &curvedBullet, new BulletPattern::Circle{30} };

	//Boss bullets
	Bullet slowBullet{ 40.f, 500.0, Bullet::fixed };
	BulletPattern circlePattern{ &slowBullet, new BulletPattern::Circle{30} };
	Bullet accBullet{ 40.f, 1000.0, Bullet::fixed, "res/img/BigBullet.png"};
	BulletPattern accCirclePattern{ &accBullet, new BulletPattern::Circle{20} };

	//Pattern 3
	Bullet reverseSlowBossBullet{ 130.f, 100.0, Bullet::fixed };
	BulletPattern reverseSlowBossPattern{ &reverseSlowBossBullet, -5000.f };

	//Pattern 4
	Bullet bossBullet{ 40.f, 250.0, Bullet::fixed };
	Bullet reverseBossBullet{ 130.f, 250.0, Bullet::fixed };

	BulletPattern bossPattern{ &bossBullet, 5000.f };
	BulletPattern reverseBossPattern{ &reverseBossBullet, -5000.f };

	//Pattern 5
	Bullet aimedBigBullet{ 40.f, 300.0, Bullet::aimed, "res/img/BigBullet.png" };
	Bullet bigBullet{ 40.f, 300.0, Bullet::fixed, "res/img/BigBullet.png" };

	BulletPattern aimedBigBulletPattern{ &aimedBigBullet,  BulletPattern::fixReference, 0.f };
	BulletPattern leftReferenceBigBulletPattern{ &bigBullet, BulletPattern::useReference, 8.f };
	BulletPattern rightReferenceBigBulletPattern{ &bigBullet, BulletPattern::useReference, -8.f };

	//Pattern 6
	Bullet fastAimedBigBullet{ 40.f, 1000.0, Bullet::aimed, "res/img/BigBullet.png" };
	Bullet fastBigBullet{ 40.f, 1000.0, Bullet::fixed, "res/img/BigBullet.png" };

	BulletPattern fastAimedBigBulletPattern{ &fastAimedBigBullet,  BulletPattern::fixReference, 0.f };
	BulletPattern fastLeftReferenceBigBulletPattern{ &fastBigBullet, BulletPattern::useReference, 10.f };
	BulletPattern fastRightReferenceBigBulletPattern{ &fastBigBullet, BulletPattern::useReference, -10.f };

	//Pattern 7
	Bullet slowRandomBullet{ 40.f, 100.f, Bullet::random };
	BulletPattern slowRandomCirclePattern{ &slowRandomBullet, new BulletPattern::Circle{20} };

};

struct Ennemies
{
	BulletPatterns bp{};

	std::vector<sf::IntRect> johnnoAnimations{ {81,0,146,150}, {227,0,146,150}, {373,0,146,150}, {81,150,146,150}, {227,150,146,150}, {373,150,146,150} };
	std::pair<std::string, std::vector<sf::IntRect>> johnnoAnimator{ {"res/img/JohnnoroideSpritesheet.png"}, johnnoAnimations };

	std::pair<std::string, std::vector<sf::IntRect>> gueposAnimator{ {"res/img/Guepos.png"}, {{0,0,200,219}} };
	std::pair<std::string, std::vector<sf::IntRect>> grosAnimator{ {"res/img/grosTest.png"}, {{0,0,550,345}} };
	std::pair<std::string, std::vector<sf::IntRect>> mitchAnimator{ {"res/img/MitchSpritesheet.png"}, {{0,0,179,179}, {179, 0, 179, 179}} };

	std::pair<std::string, std::vector<sf::IntRect>> shalomiumAnimator{ {"res/img/Boss.png"}, {{0,0,975,922}} };

	Ennemy joroide{
		{
			std::pair{&bp.aimedSinglePatternJ, sf::seconds(1.25f)}
		}
		, 5, 3, &johnnoAnimator, 0.8f};

	Ennemy shotlessJoroide{
	{
		std::pair{&bp.aimedSinglePatternJ, sf::seconds(999)}
	}
	, 5, 3, &johnnoAnimator, 0.8f };

	Ennemy guepos{
		{
			std::pair{bp.referenceWavySinglePattern.setRepeatCounter(8,0.f), sf::seconds(0.1)},
			std::pair{&bp.aimedWavySinglePattern, sf::seconds(1.f)},
		}
		, 75, 10, &gueposAnimator };

	Ennemy gros{
		{
			std::pair{bp.grosAimedSinglePattern.setRepeatCounter(20,100000.f), sf::seconds(0.03f)},
			std::pair{&bp.nullPattern, sf::seconds(1.5f)},


			std::pair{bp.reversePattern.setRepeatCounter(10,100000.f), sf::seconds(0.02f)},
			std::pair{&bp.nullPattern, sf::seconds(1.5f)},

			std::pair{bp.grosAimedSinglePattern.setRepeatCounter(20,100000.f), sf::seconds(0.03f)},
			std::pair{&bp.nullPattern, sf::seconds(1.5f)},


			std::pair{bp.pattern.setRepeatCounter(15,100000.f), sf::seconds(0.02f)},
			std::pair{&bp.nullPattern, sf::seconds(1.5f)},
		}
		, 300, 30, &grosAnimator, 0.8f};

	Ennemy mitch{
	{
		std::pair{&bp.curvedCirclePattern, sf::seconds(999)},
	}
	, 1, 1, &mitchAnimator , 0.5f};

	Ennemy ennemy5{
	{
		std::pair{&bp.randomCurvedSinglePattern, sf::seconds(10)},
	}
	, 10 };

	Ennemy ennemyTest{
	{
		std::pair{&bp.aimedWavySinglePattern, sf::seconds(3)},
	}
	, 10 };

	//Create Boss
	MoveTimeline moveTimeline1 = { { std::pair<MathVector, MathVector>{ {},{0.f, -300.f}} , sf::seconds(3.f) },
							{ std::pair<MathVector, MathVector>{ {0.f, 0.f},{}} , sf::seconds(300.f) },
							{ std::pair<MathVector, MathVector>{ {0.f, 200.f},{0.f, -50.f}} , sf::seconds(1.f) } };

	PatternQueue nullPatternQueue = { {std::pair{&bp.nullPattern, sf::seconds(999.f)}} };

	PatternQueue patternQueue1 = {
			std::pair{&bp.accCirclePattern, sf::seconds(0.5)},
			std::pair{bp.circlePattern.setRepeatCounter(3, 0.f), sf::seconds(0.5)},
			std::pair{ &bp.nullPattern, sf::seconds(2.f) } };

	PatternQueue patternQueue2 = {
		std::pair{&bp.accCirclePattern, sf::seconds(0.5)} };

	PatternQueue patternQueue3 = {
		std::pair{bp.circlePattern.setRepeatCounter(3, 0.f), sf::seconds(0.5)},
		std::pair{bp.aimedSinglePattern.setRepeatCounter(10, 75'000.f), sf::seconds(0.03)} };

	PatternQueue patternQueue4 = {
		std::pair{bp.bossPattern.setRepeatCounter(2, 500.f), sf::seconds(0.01)},
		std::pair{bp.reverseBossPattern.setRepeatCounter(2, 500.f), sf::seconds(0.01)} };

	PatternQueue patternQueue5 = {
		std::pair{&bp.leftReferenceBigBulletPattern, sf::seconds(0)},
		std::pair{&bp.rightReferenceBigBulletPattern, sf::seconds(0)},
		std::pair{&bp.aimedBigBulletPattern, sf::seconds(0.5)},
		std::pair{bp.circlePattern.setRepeatCounter(3, 0.f), sf::seconds(0.5)} };

	PatternQueue patternQueue6 = {
		std::pair{&bp.fastLeftReferenceBigBulletPattern, sf::seconds(0)},
		std::pair{&bp.fastRightReferenceBigBulletPattern, sf::seconds(0)},
		std::pair{&bp.fastAimedBigBulletPattern, sf::seconds(0.5)},
		std::pair{bp.circlePattern.setRepeatCounter(3, 0.f), sf::seconds(0.25)} };

	PatternQueue patternQueue7 = {
	std::pair{bp.slowRandomCirclePattern.setRepeatCounter(4, 75.f), sf::seconds(0.1)}, };

	BossState introState = { 9999, 0, moveTimeline1, nullPatternQueue};

	BossState shalomiumState1 = { 500, 800, moveTimeline1, patternQueue1 };
	BossState shalomiumState2 = { 750, 1000, moveTimeline1, patternQueue2 };

	BossState shalomiumState3 = { 500, 800, moveTimeline1, patternQueue3 };
	BossState shalomiumState4 = { 750, 1000, moveTimeline1, patternQueue4 };

	BossState shalomiumState5 = { 500, 800, moveTimeline1, patternQueue5 };
	BossState shalomiumState6 = { 750, 1000, moveTimeline1, patternQueue6 };

	BossState shalomiumState7 = { 1000, 5000, moveTimeline1, patternQueue7 };

	Boss shalomium{ {introState, shalomiumState1, shalomiumState2, shalomiumState3, shalomiumState4, shalomiumState5, shalomiumState6, shalomiumState7 }, &shalomiumAnimator, 0.7f};
};

void Game::start()
{
	
	if (m_gameState != Game::uninitialized)
		return;

	//Creating context settings for the window
	sf::ContextSettings context_settings;
	context_settings.depthBits = 24;
	context_settings.stencilBits = 8;
	context_settings.antialiasingLevel = 4;
	context_settings.majorVersion = 4;
	context_settings.minorVersion = 5;

	//Create window
	m_mainWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "MechaTouhou!", sf::Style::Default, context_settings);
	m_uiWindow.create(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_view.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_view.setCenter(m_view.getSize().x / 2, m_view.getSize().y / 2);
	setLetterboxView(SCREEN_WIDTH, SCREEN_HEIGHT);

	//début test opengl
	m_mainWindow.setActive(true);
	gladLoadGL();

	glViewport(0, 0, static_cast<GLsizei>(m_mainWindow.getSize().x), static_cast<GLsizei>(m_mainWindow.getSize().y));
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);





	//Define Triangle
	constexpr std::array<GLfloat, 180> cube
	{
	//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	// 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

	};

	//Define Plane
	constexpr std::array<GLfloat, 20> plane
	{
	//Coordinates			//Texture
	0.5f,  0.5f, 0.0f,		1.0f, 1.0f,		// top right
	 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		// bottom right
	-0.5f, -0.5f, 0.0f,		0.f, 0.f,		// bottom left
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,		// top left 
	};

	constexpr std::array<GLuint, 6> indices
	{
		0,1,3,
		1,2,3
	};

	constexpr std::array<GLfloat, 9> texCoords
	{
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};





	//Build and compile shaders (juste pour tester opengl, sinon utiliser le struct shader)
	unsigned int vertexShader{ glCreateShader(GL_VERTEX_SHADER) };
	unsigned int fragmentShader{ glCreateShader(GL_FRAGMENT_SHADER) };
	unsigned int fragmentShader2{ glCreateShader(GL_FRAGMENT_SHADER) };
	Game::shaderProgram = glCreateProgram();
	Game::shaderProgram2 = glCreateProgram();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	//Link the first shader program
	glAttachShader(Game::shaderProgram, vertexShader);
	glAttachShader(Game::shaderProgram, fragmentShader);
	glLinkProgram(Game::shaderProgram);

	glDeleteShader(fragmentShader);

	//Link the second shader program
	glAttachShader(Game::shaderProgram2, vertexShader);
	glAttachShader(Game::shaderProgram2, fragmentShader2);

	glLinkProgram(Game::shaderProgram2);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader2);





	//Textures setup
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, textures);

	stbi_set_flip_vertically_on_load(true);
	//Load first image
	int width{};
	int height{};
	int nrChannels{};

	unsigned char* data{ stbi_load("res/img/GroundTexture.png", &width, &height, &nrChannels, 4) };

	//Creating first texture
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}





	//Creating second texture

	data = stbi_load("res/img/DestroyedWallTexture.png", &width, &height, &nrChannels, 4);

	glBindTexture(GL_TEXTURE_2D, textures[1]);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}





	//Create shaders
	/*shaderPrograms[0] = Shader{ "path/to/shaders/shader.vs", "path/to/shaders/shader.fs" };
	shaderPrograms[0] = Shader{ "path/to/shaders/shader.vs", "path/to/shaders/shader2.fs" };*/

	//Create VAOs (Vertex Array Object)
	glCreateVertexArrays(2, Game::VAOs);

	//Create VBOs (Vertex Buffer Object)
	unsigned int VBOs[2]{};
	glGenBuffers(2, VBOs);





	////First triangle setup
	//glBindVertexArray(Game::VAOs[0]);

	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), &triangle1[0], GL_STATIC_DRAW);
	//
	////Linking vertex attributes
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
	//glEnableVertexAttribArray(0);

	////Second triangle setup
	//glBindVertexArray(Game::VAOs[1]);

	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), &triangle2[0], GL_STATIC_DRAW);

	//Linking vertex attributes
	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/


	
	
	
	
	
	//Make plane

	glBindVertexArray(VAOs[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), &plane[0], GL_STATIC_DRAW);

	//Create EBO (Element Buffer Object)
	unsigned int EBO{};
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	//Linking vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	
	
	

	// Make cube

	glBindVertexArray(VAOs[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//fin test opengl

	Player* player{ new Player{} };
	player->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	player->getPlayerSubject().addObserver(&m_uiManager);
	m_dialogueManager.getSubject().addObserver(player);

	m_gameObjectManager.add("Player", player);

	sf::Time test{ sf::seconds(1) };

	Ennemies ennemies{};
	//Initialize ennemy shader
	Ennemy::m_whiteShader.loadFromFile("tint.frag", sf::Shader::Fragment);
	Ennemy::getEnnemySubject().addObserver(&m_uiManager);
	Boss::getBossSubject().addObserver(&m_ennemyTimeline);
	Boss::setDialogueSubject(&m_dialogueManager.getSubject());
	Ennemy::setPlayer(player);

	//Setup EnnemyTimeline (je devrais faire ça ailleurs)
	m_ennemyTimeline.getSubject().addObserver(&m_dialogueManager);

	int randomX{};
	int randomAcc{};
	MoveTimeline grosMoveTimeline = { { std::pair<MathVector, MathVector>{ {},{0.f, -300.f}} , sf::seconds(3.f) },
							{ std::pair<MathVector, MathVector>{ {0.f, 0.f},{}} , sf::seconds(15.f) },
							{ std::pair<MathVector, MathVector>{ {0.f, 200.f},{0.f, -50.f}} , sf::seconds(3.f) } };

	m_ennemyTimeline.addEnnemy(ennemies.shalomium, sf::seconds(3), { SCREEN_WIDTH / 2.f, -150.f }, m_dialogueManager.getSubject());
	
	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { -20.f, SCREEN_HEIGHT / 4.f + 20.f }, { 400.f, 0.f }, {});
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH + 20.f, SCREEN_HEIGHT * 0.75f - 20.f }, { -400.f, 0.f }, {});
	}

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.f), { SCREEN_WIDTH / 2.f, -50.f }, { 0.f, 300.0f }, { 0.f, -0.8f });

	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { -20.f, SCREEN_HEIGHT / 4.f + 20.f }, { 400.f, 0.f }, {});
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH + 20.f, SCREEN_HEIGHT * 0.75f - 20.f }, { -400.f, 0.f }, {});
	}

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.f), { SCREEN_WIDTH / 4.f - ennemies.guepos.getWidth() / 2.f, -50.f }, { 0.f, 300.0f }, { 0.f, -100.f});
	
	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { -20.f, SCREEN_HEIGHT / 4.f + 20.f }, { 400.f, 0.f }, {});
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH + 20.f, SCREEN_HEIGHT * 0.75f - 20.f }, { -400.f, 0.f }, {});
	}

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.f), { SCREEN_WIDTH / 2.f, -50.f }, { 0.f, 300.0f }, { 0.f, -100.f});

	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { -20.f, SCREEN_HEIGHT / 4.f + 20.f }, { 400.f, 0.f }, {});
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH + 20.f, SCREEN_HEIGHT * 0.75f - 20.f }, { -400.f, 0.f }, {});
	}

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.f), { SCREEN_WIDTH * 0.75f, -50.f }, { 0.f, 300.0f }, { 0.f, -100.f });

	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { -20.f, SCREEN_HEIGHT / 4.f + 20.f }, { 400.f, 0.f }, {});
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH + 20.f, SCREEN_HEIGHT * 0.75f - 20.f }, { -400.f, 0.f }, {});
	}
	
	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.f), { SCREEN_WIDTH / 2.f, -50.f }, { 0.f, 300.0f }, { 0.f, -100.f});
	
	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { -20.f, SCREEN_HEIGHT / 4.f + 20.f}, { 400.f, 0.f}, {});
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH + 20.f, SCREEN_HEIGHT * 0.75f - 20.f}, { -400.f, 0.f}, {});
	}

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(3.0f), { SCREEN_WIDTH / 4.f - ennemies.guepos.getWidth() / 2.f, -50.f }, { 0.f, 300.0f }, { 0.f, -100.f });





	
	for (int i{ 1 }; i < 150; ++i)
	{
		int randomX{ Random::get(0,SCREEN_WIDTH) };
		int randomAcc{ Random::get(-1000, 1000) };
		m_ennemyTimeline.addEnnemy(ennemies.mitch, sf::seconds(0.05), { static_cast<float>(randomX), 50.f }, { 0.f, 900.0f }, { static_cast<float>(randomAcc), 0.f });
	}

	m_ennemyTimeline.addEnnemy(ennemies.gros, sf::seconds(3), { SCREEN_WIDTH * 0.75f, -140.f }, grosMoveTimeline);

	for (int i{ 1 }; i < 150; ++i)
	{
		int randomX{ Random::get(0,SCREEN_WIDTH) };
		int randomAcc{ Random::get(-1000, 1000) };
		m_ennemyTimeline.addEnnemy(ennemies.mitch, sf::seconds(0.05), { static_cast<float>(randomX), 50.f }, { 0.f, 900.0f }, {static_cast<float>(randomAcc), 0.f});
	}
	randomX = Random::get(0, SCREEN_WIDTH);
	randomAcc = Random::get(-1000, 1000);
	m_ennemyTimeline.addEnnemy(ennemies.mitch, sf::seconds(4), { static_cast<float>(randomX), 50.f }, { 0.f, 600.0f }, { static_cast<float>(randomAcc), 0.f }); 

	m_ennemyTimeline.addEnnemy(ennemies.gros, sf::seconds(3), { SCREEN_WIDTH / 4.f, -140.f}, grosMoveTimeline);

	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH / 4.f, SCREEN_HEIGHT + 50.f }, { 0.f, -400.0f }, {});
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT + 50.f }, { 0.f, -400.0f }, {});
	}

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0), { SCREEN_WIDTH / 4.f - ennemies.guepos.getWidth()/2.f, -50.f}, {0.f, 300.0f}, {0.f, -100.f});
	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0), { SCREEN_WIDTH * 0.75f + ennemies.guepos.getWidth()/2.f, -50.f }, { 0.f, 300.0f }, { 0.f, -100.f});

	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH / 4.f, SCREEN_HEIGHT + 50.f }, { 0.f, -400.0f }, {});
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT + 50.f }, { 0.f, -400.0f }, {});
	}

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.5f), { SCREEN_WIDTH / 2.f , -50.f }, { 0.f, 300.0f }, { 0.f, -100.f });

	for (int i{ 1 }; i < 10; ++i)
	{
		int randomX{ Random::get(-100,100) };
		m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(0), { SCREEN_WIDTH / 2.f + randomX, -50.f }, { 0.f, 300.0f }, {});
		m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(0.2f), { SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT + 50.f }, { 0.f, -400.0f }, {});
	}

	m_ennemyTimeline.addEnnemy(ennemies.shotlessJoroide, sf::seconds(1), { SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT + 50.f }, { 0.f, -400.0f }, {});

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.5), { SCREEN_WIDTH / 4.f, -50.f }, { 0.f, 300.0f }, {0.f, -0.8f});

	for (int i{ 1 }; i < 10; ++i)
	{
		int randomX{ Random::get(-100,100) };
		m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(0.5), { SCREEN_WIDTH / 4.f + randomX, -50.f }, { 0.f, 300.0f }, {});
	}

	randomX = Random::get(-100,100);
	m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(4), { SCREEN_WIDTH / 4.f + randomX, -50.f }, { 0.f, 300.0f }, {});











	for (int i{ 1 }; i < 150; ++i)
	{
		int randomX{ Random::get(0,SCREEN_WIDTH) };
		int randomAcc{ Random::get(-1000, 1000) };
		m_ennemyTimeline.addEnnemy(ennemies.mitch, sf::seconds(0.05), { static_cast<float>(randomX), 50.f }, { 0.f, 900.0f }, {static_cast<float>(randomAcc), 0.f});
	}
	randomX = Random::get(0,SCREEN_WIDTH);
	randomAcc = Random::get(-1000, 1000);
	m_ennemyTimeline.addEnnemy(ennemies.mitch, sf::seconds(9), { static_cast<float>(randomX), 50.f }, { 0.f, 600.0f }, { static_cast<float>(randomAcc), 0.f });

	m_ennemyTimeline.addEnnemy(ennemies.gros, sf::seconds(3), { SCREEN_WIDTH / 2.f, -140.f}, grosMoveTimeline);

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.75), { SCREEN_WIDTH * 0.75f, -50.f }, { 0.f, 300.0f }, { 0.f, -100.f});
	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.5), { SCREEN_WIDTH / 4.f, -50.f }, { 0.f, 300.0f }, { 0.f, -100.f });

	for (int i{ 1 }; i < 10; ++i)
	{
		int randomX{ Random::get(-100,100) };
		m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(0.5), { SCREEN_WIDTH / 2.f + randomX, -50.f }, { 0.f, 300.0f }, {});
	}
	randomX = Random::get(-100, 100);
	m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(0.5), { SCREEN_WIDTH / 2.f + randomX, -50.f }, { 0.f, 300.0f }, {});





	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.5), { SCREEN_WIDTH * 0.75f, -50.f }, { 0.f, 300.0f }, { 0.f, -100.f });

	for (int i{ 1 }; i < 10; ++i)
	{
		int randomX{ Random::get(-100,100) };
		m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(0.5), { SCREEN_WIDTH * 0.75f + randomX, -50.f }, { 0.f, 300.0f }, {});
	}

	randomX = Random::get(-100,100);
	m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(2), { SCREEN_WIDTH * 0.75f + randomX, -50.f }, { 0.f, 300.0f }, {});

	m_ennemyTimeline.addEnnemy(ennemies.guepos, sf::seconds(0.5), { SCREEN_WIDTH / 4.f, -50.f }, { 0.f, 300.0f }, {0.f, -100.f});

	for (int i{ 1 }; i < 10; ++i)
	{
		int randomX{ Random::get(-100,100) };
		m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(0.5), { SCREEN_WIDTH / 4.f + randomX, -50.f }, { 0.f, 300.0f }, {});
	}

	randomX = Random::get(-100,100);
	m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(1), { SCREEN_WIDTH / 4.f + randomX, -50.f }, { 0.f, 300.0f }, {});


	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(0.5), { SCREEN_WIDTH * 0.75f, -50.f }, { 0.f, 200.0f }, { -200.f,20.f });
	}

	m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(1), { SCREEN_WIDTH * 0.75f, -50.f }, { 0.f, 200.0f }, { -200.f,20.f });

	for (int i{ 1 }; i < 10; ++i)
	{
		m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(0.5), {SCREEN_WIDTH/4.f, -50.f }, { 20.0f, 200.0f }, { 200.f,20.f });
	}

	m_ennemyTimeline.addEnnemy(ennemies.joroide, sf::seconds(5), { SCREEN_WIDTH / 4.f, -50.f }, { 0.f, 200.0f }, { 200.f , 20.f });


	SFMLSoundProvider soundProvider;
	ServiceLocator::registerServiceLocator(&soundProvider);
	ServiceLocator::getAudio()->playSong("res/bgm/main.wav", true);

	//à enlever/déplacer
	m_debugFont.loadFromFile("res/fonts/arial.ttf");

	m_gameState = Game::playing;
	ImGui::SFML::Init(m_mainWindow);

	while (!isExiting())
	{
		gameLoop();
	}
	
	ImGui::SFML::Shutdown();
	m_mainWindow.close();
}

bool Game::isExiting()
{
	if (m_gameState == Game::exiting)
		return true;
	else
		return false;
}

void Game::gameLoop()
{
	sf::Event currentEvent;
	if (m_mainWindow.pollEvent(currentEvent))
	{
		ImGui::SFML::ProcessEvent(currentEvent);

		if (currentEvent.type == sf::Event::Closed)
		{
			m_gameState = Game::exiting;
			return;
		}

		if (currentEvent.type == sf::Event::Resized)
		{
			setLetterboxView(currentEvent.size.width, currentEvent.size.height);
			glViewport(static_cast<GLsizei>(m_view.getViewport().left * currentEvent.size.width), static_cast<GLsizei>(m_view.getViewport().top * currentEvent.size.height),
					   static_cast<GLsizei>(m_view.getViewport().width * currentEvent.size.width), static_cast<GLsizei>(m_view.getViewport().height * currentEvent.size.height));
		}

		if (currentEvent.type == sf::Event::KeyPressed)
		{

			if (currentEvent.key.code == sf::Keyboard::Escape)
			{
				if (m_gameState == Game::playing)
				{
					m_gameState = Game::pause;

					m_gameObjectManager.pauseDelta();
					m_bulletManager.pauseDelta();
				}
				else if (m_gameState == Game::pause)
				{
					m_gameState = Game::playing;
					
					m_gameObjectManager.restartDelta();
					m_bulletManager.restartDelta();
				}

				return;
			}

			if (m_dialogueManager.getHasBegun())
			{
				if (currentEvent.key.code == sf::Keyboard::Y
					|| currentEvent.key.code == sf::Keyboard::X
					|| currentEvent.key.code == sf::Keyboard::C)
				{
					m_dialogueManager.nextDialogue();
					m_uiManager.setUpdateUi(true);
				}
			}
		}

	}

	if (m_freeCameraMode)
	{
		m_mainWindow.setMouseCursorVisible(false);
		if (m_firstMouse)
		{
			m_firstMouse = false;
			return;
		}

		sf::Vector2i center(m_mainWindow.getSize().x / 2, m_mainWindow.getSize().y / 2);

		float xoffset = sf::Mouse::getPosition(m_mainWindow).x - (float)center.x;
		float yoffset = (float)center.y - sf::Mouse::getPosition(m_mainWindow).y;

		m_yaw += xoffset * 0.1f;
		m_pitch += yoffset * 0.1f;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		sf::Mouse::setPosition(sf::Vector2<int>((int)m_mainWindow.getSize().x / 2, (int)m_mainWindow.getSize().y / 2), m_mainWindow);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			m_cameraPos += m_cameraFront * m_cameraSpeed * m_deltaClock.getElapsedTime().asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			m_cameraPos -= m_cameraFront * m_cameraSpeed * m_deltaClock.getElapsedTime().asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_cameraSpeed * m_deltaClock.getElapsedTime().asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_cameraSpeed * m_deltaClock.getElapsedTime().asSeconds();

		//Camera move
		//m_pitch = 0.f;
		//m_cameraPos += m_deltaClock.getElapsedTime().asSeconds();

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		direction.y = sin(glm::radians(m_pitch));
		direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_cameraFront = glm::normalize(direction);
	}
	//Camera move
	//verifier que le m_deltaClock marche
	m_cameraPos += glm::vec3(0.f,0.f,-1.0f) * 0.05f * m_deltaClock.getElapsedTime().asSeconds();

	switch (m_gameState)
	{
	case Game::showingMenu:
	{
		showMenu();
		break;
	}
	case Game::showingSplash:
	{
		showSplashScreen();
		break;
	}
	case Game::playing:
	{
		m_mainWindow.pushGLStates();
		m_mainWindow.clear(sf::Color(50, 10, 10, 255));
		m_mainWindow.popGLStates();

		//Test OpenGL
		
		std::vector<glm::vec3> cubePositions = {
			glm::vec3(0.6f, 0.25f, -1.5f),
			glm::vec3(-0.75f,  0.25f, -0.5f),
			glm::vec3(0.6f, 0.25f, -3.f),
			glm::vec3(-0.75f,  0.25f, -4.5f),
			glm::vec3(0.0f,  0.25f, -6.f),
			glm::vec3(0.6f, 0.25f, -7.5f),
			glm::vec3(-0.75f,  0.25f, -9.f),
			glm::vec3(0.0f,  0.25f, -10.5f),
		};


		//Configure OpenGL

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram2);


		//View matrix
		glm::mat4 view;
		view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

		GLint viewLoc{ glGetUniformLocation(shaderProgram2, "view") };
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


		//Projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f, 100.0f);

		GLint projectionLoc{ glGetUniformLocation(shaderProgram2, "projection") };
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//World matrix
		glm::mat4 model{ glm::mat4{1.0f} };
		model = glm::scale(model, glm::vec3(5.f, 1.f, 5.f));
		model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.0f, 0.f, 0.f));

		GLint modelLoc{ glGetUniformLocation(shaderProgram2, "model") };
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Draw Plane

		glBindVertexArray(VAOs[0]);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//World matrix for second plane

		glm::mat4 model2{ glm::mat4{1.0f} };
		model2 = glm::translate(model2, glm::vec3{ 0.f, 0.f, -5.f });
		model2 = glm::scale(model2, glm::vec3(5.f, 1.f, 5.f));
		model2 = glm::rotate(model2, glm::radians(-90.f), glm::vec3(1.0f, 0.f, 0.f));

		GLint modelLoc2{ glGetUniformLocation(shaderProgram2, "model") };
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));

		//Draw Second plane

		glBindVertexArray(VAOs[0]);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//Place cool cubes in the world

		glBindVertexArray(VAOs[1]);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		for (int i{}; i < cubePositions.size(); ++i)
		{
			glm::mat4 model{ glm::mat4{1.0f} };
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.0f, 0.f, 0.f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

			GLint modelLoc{ glGetUniformLocation(shaderProgram2, "model") };
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Reset OpenGL states to use SFML window again
		glBindVertexArray(0);
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_mainWindow.resetGLStates();

		//Fin test OpenGl
		m_mainWindow.pushGLStates();
		m_mainWindow.setView(m_view);


		m_ennemyTimeline.update(m_gameObjectManager);

		m_gameObjectManager.updateAll();
		m_gameObjectManager.drawAll(m_mainWindow);

		m_bulletManager.updateAll();
		m_bulletManager.drawAll(m_mainWindow);

		if (m_uiManager.getUpdateUi())
		{
			//m_mainWindow.setView(m_uiView);
			m_uiManager.draw(m_uiWindow);
			//m_mainWindow.setView(m_mainWindow.getDefaultView());

			m_dialogueManager.setUpdateUi(true);
		}
		if (m_dialogueManager.getUpdateUi())
		{
			m_dialogueManager.draw(m_uiWindow);
		}
		sf::Sprite uiSprite{ m_uiWindow.getTexture() };
		m_mainWindow.draw(uiSprite);


		//début debug

		//std::string str{};
		//str = std::to_string(m_bulletManager.getCount()) + " bullets\n" + std::to_string(static_cast<int>(1 / m_deltaClock.getElapsedTime().asSeconds())) + 
		//	" fps\nCamPos: X=" + std::to_string(m_cameraPos.x) + " Y=" +  std::to_string(m_cameraPos.y) + " Z=" + std::to_string(m_cameraPos.z)
		//	+ "\nCamFront: X=" + std::to_string(m_cameraFront.x) + " Y=" + std::to_string(m_cameraFront.y) + " Z=" + std::to_string(m_cameraFront.z);
		//sf::Text text{};
		//text.setFillColor(sf::Color::Green);

		//text.setFont(m_debugFont);

		//text.setString(str);
		//m_mainWindow.draw(text);

		//fin debug

		displayDebugUI();
		m_mainWindow.display();

		m_mainWindow.popGLStates();

		break;
	}
	case Game::pause:
	{
		showPause();
		break;
	}
	}

}

void Game::displayDebugUI()
{
	Player* player{ dynamic_cast<Player*>(m_gameObjectManager.get("Player")) };
	assert(player != NULL);

	ImGui::SFML::Update(m_mainWindow, m_deltaClock.restart());

	/*ImGui::Begin("Player Options");

	static bool invicible{ false };
	ImGui::Checkbox("Invincible", &invicible);

	ImGui::Checkbox("Free Camera Mode", &m_freeCameraMode);

	ImGui::End();*/

	ImGui::SFML::Render(m_mainWindow);

	//if (invicible)
	//{
	//	player->startInvincibility();
	//}

	//if (m_freeCameraMode)
	//{
	//	player->setPosition(0, 0);
	//	player->startInvincibility();
	//}
}

void Game::showPause()
{
	sf::Sprite pauseSpr{ *m_textureCache.getImage("res/img/Pause.png") };
	m_mainWindow.draw(pauseSpr);

	displayDebugUI();
	m_mainWindow.display();
}

void Game::showSplashScreen()
{
	SplashScreen splashScreen{};
	splashScreen.show(m_mainWindow);
	m_gameState = Game::showingMenu;
}

void Game::showMenu()
{
	MainMenu mainMenu;
	MainMenu::MenuResult result = mainMenu.show(m_mainWindow);
	switch (result)
	{
	case MainMenu::exit:
		m_gameState = Game::exiting; 
		break;
	case MainMenu::play:
		m_gameState = Game::playing;
		break;
	}
}

void Game::setLetterboxView(int windowWidth, int windowHeight) {

	// Compares the aspect ratio of the window to the aspect ratio of the view,
	// and sets the view's viewport accordingly in order to achieve a letterbox effect.
	// A new view (with a new viewport set) is returned.

	float windowRatio = (float)windowWidth / (float)windowHeight;
	float viewRatio = m_view.getSize().x / (float)m_view.getSize().y;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	// If horizontalSpacing is true, the black bars will appear on the left and right side.
	// Otherwise, the black bars will appear on the top and bottom.

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}

	else {
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
	}

	m_view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
}

Game::GameState Game::m_gameState = Game::uninitialized;
sf::Clock Game::m_deltaClock{};
sf::Clock Game::m_testClock{};

sf::RenderWindow Game::m_mainWindow{};
sf::View Game::m_view{};


sf::View Game::m_uiView{Game::m_mainWindow.getView()};
sf::RenderTexture Game::m_uiWindow{};
sf::Font Game::m_debugFont{};

GameObjectManager Game::m_gameObjectManager{};
BulletManager Game::m_bulletManager{};
UiManager Game::m_uiManager{ "res/img/Heart.png", 0.5f };
DialogueManager Game::m_dialogueManager{};
EnnemyTimeline Game::m_ennemyTimeline {
	{
		{
			{{blue, left}, {boss, right}},
			{{boss, L"salut poilu!"}, {blue, L"wow t'es sacrément moche pour ton âge... et pd aussi"},
			{boss, L"mais je te permets pas mon garçon... <3"}, {blue, L"g les droits admins alors tg la poufiasse au masculin"},
			{boss, L"j'ai été transformé en vampire après avoir regardé twilight"}, {blue, L"toilettes ???!!!!"},
			{boss, L"g dit twilight gros"}, {blue, L"non mais j'ai vraiment besoin d'aller aux toilettes là"},
			{boss, L"que le combat commence"}, {blue, L"et qu'il finisse vite !!!! CACA!!!!!!!!!!!!!!"}}
		},

		{
			{{blue, left}, {boss, right}},
			{{boss, L"Epic fail pour ma part"}, {blue, L"ouais c'est ça retourne manger du makeup"},
			{boss, L"mario makeup"}, {blue, L"pour faire une union en LaTeX il faut écrire cup"}}
		}
	}
};

//OpenGL
unsigned int Game::VAOs[2]{};
unsigned int Game::shaderProgram{};
unsigned int Game::shaderProgram2{};
std::vector<Shader> Game::shaderPrograms{};

unsigned int Game::textures[2]{};

//Camera
bool Game::m_freeCameraMode{ false };
float Game::m_cameraSpeed{ 75.f };
float Game::m_yaw{ -90.f };
float Game::m_pitch{};


glm::vec3 Game::m_cameraPos{ glm::vec3(0.0f, 1.0f, 3.0f) };
glm::vec3 Game::m_cameraFront{ glm::vec3(0.0f, -0.7f, -1.0f) };
glm::vec3 Game::m_cameraUp{ glm::vec3(0.0f, 1.0f, 0.0f) };

bool Game::m_firstMouse{ false };



//à changer de place

TextureCache Game::m_textureCache{};

//Ennemy

sf::Shader Ennemy::m_whiteShader{};
Player* Ennemy::m_player{};