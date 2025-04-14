#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include <ctime>
#include<string>
#include<vector>	
/*
								References
1) Dr Nour
2) By GitHub Copilot with our edits on it
-drawBoundingBoxes function
-handleJump function
-updateScore function
3) From IcyTowerGEN33 Project on Github
-scrolling function

*/

using namespace std;
using namespace sf;

Font font;
Texture curs;
Image icon;
float lastBlockY;
//load icy
Texture icy;
Sprite icysprite;

// Block variables
Texture blockshape, gblockshape; // blocks shape , ground block shape
Sprite b_drawer[10], g_drawer;
Texture back1, back2;
Sprite backg11, backg12, backg21, backg22;

//Score Variables
int blockScores[10]; // Array to store score for each block
int playerScore = 0; // Player's score

Text scoreText; // Text to display the score

void loadBlockTextures() {
	//Load Blocks

	blockshape.loadFromFile("Assets/level 1.png");
	gblockshape.loadFromFile("Assets/glevel 1.png");
	//Load backgrounds
	back1.loadFromFile("Assets/backgroundnew1.png");
	back2.loadFromFile("Assets/backgroundnew2.png");
	backg11.setTexture(back1);
	backg12.setTexture(back2);
	backg21.setTexture(back1);
	backg22.setTexture(back2);
	backg11.setPosition(0, 0);
	backg12.setPosition(0, 0);
	backg21.setPosition(0, -599);
	backg22.setPosition(0, -599);
	//Load Score
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10, 550); // Position at the bottom left of the screen
}

void generateBlocks() {
	srand(static_cast<unsigned int>(time(0))); // Seed for random number generation
	b_drawer[0].setPosition(80 + rand() % 375, 400);
	b_drawer[0].setTexture(blockshape);
	blockScores[0] = 10; // Assign score to the first block
	g_drawer.setPosition(90, 568);
	g_drawer.setTexture(gblockshape);

	for (int i = 1; i < 10; i++) {
		int textureIndex = rand() % 5; // Randomly select a texture
		b_drawer[i].setTexture(blockshape);

		// Randomly set the position of the block
		Vector2f blockpos = b_drawer[i - 1].getPosition();
		blockpos.x = 80 + rand() % 375; // Assuming the window width is 800
		blockpos.y -= 150; // Set the y position 150 units above the previous block

		b_drawer[i].setPosition(blockpos.x, blockpos.y);
		blockScores[i] = (i + 1) * 10; // Assign score to each block
	}
	lastBlockY = b_drawer[9].getPosition().y;
}
void updateScore(Sprite& player) {
	for (int i = 0; i < 10; i++) {
		if (player.getGlobalBounds().intersects(b_drawer[i].getGlobalBounds())) {
			playerScore += blockScores[i];
			blockScores[i] = 0; // Reset the block score to prevent multiple scoring
		}
	}
	scoreText.setString("Score: " + to_string(playerScore));
}
void drawBlocks(RenderWindow& window) {
	window.draw(backg11);
	window.draw(backg21);
	window.draw(backg12);
	window.draw(backg22);
	window.draw(g_drawer);
	for (int i = 0; i < 10; i++) {
		window.draw(b_drawer[i]);
	}
	window.draw(scoreText); // Draw the score text
}
//Jump Variables
bool isJumping = false;
float jumpVelocity = 0.0f;
const float gravity = 0.5f;
const float jumpStrength = -15.0f;

void handleJump(Sprite& icysprite)
{

	static float targetRotation = 0.0f;
	static const float rotationSpeed = 2.0f; // Adjust this value for smoother or faster rotation
	static Clock jumpPauseClock;
	if (Keyboard::isKeyPressed(Keyboard::Space) && !isJumping && jumpPauseClock.getElapsedTime().asSeconds() >= 0.5f) {
		isJumping = true;
		
		jumpVelocity = jumpStrength;
		jumpPauseClock.restart(); // Restart the pause clock

		if (Keyboard::isKeyPressed(Keyboard::A)) {
			targetRotation = -360.0f; // Target rotation when jumping with A
		}
		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			targetRotation = 360.0f; // Target rotation when jumping with D
		}
		else {
			targetRotation = 0.0f; // Default target rotation
		}
	}

	if (isJumping) {
		icysprite.move(0.0f, jumpVelocity);
		jumpVelocity += gravity;

		float currentRotation = icysprite.getRotation();
		float rotationDifference = targetRotation - currentRotation;

		if (currentRotation < targetRotation) {
			currentRotation += rotationSpeed;
		}
		else if (currentRotation > targetRotation) {
			currentRotation -= rotationSpeed;
		}
		icysprite.setRotation(currentRotation);
	}


}
//scrolling the screen
float scrollspeed = 1;
bool isStart = false;
bool isscrolling = false;
int movespeed = 1;


void scrolling(Sprite& icysprite) {
	backg11.move(Vector2f(0, 0.3));
	backg12.move(Vector2f(0, scrollspeed * movespeed));
	backg21.move(Vector2f(0, 0.3));
	backg22.move(Vector2f(0, scrollspeed * movespeed));
	g_drawer.move(Vector2f(0, scrollspeed * movespeed));


	for (int i = 0; i < 10; i++) {
		b_drawer[i].move(Vector2f(0, scrollspeed * movespeed));

		// Check if the block's y position is greater than 599
		if (b_drawer[i].getPosition().y > 599) {
			// Move the block back to the top with a new random x position
			b_drawer[i].setPosition(80 + rand() % 375, lastBlockY - 150);
			blockScores[i] = (i + 1) * 10; // Reset the block score
		}

		// Update the last block's y position
		lastBlockY = b_drawer[i].getPosition().y;
	}

	if (backg21.getPosition().y >= 0 && backg21.getPosition().y <= 1) {
		isStart = true;
		backg11.setPosition(0, -599);
	}
	else if (backg11.getPosition().y >= 0 && backg11.getPosition().y <= 1 && isStart) {
		backg21.setPosition(0, -599);
	}
	if (backg22.getPosition().y >= 0 && backg22.getPosition().y <= 1.5 * (scrollspeed * movespeed)) {
		isStart = true;
		backg12.setPosition(0, -599);
	}
	else if (backg12.getPosition().y >= 0 && backg12.getPosition().y <= 1.5 * (scrollspeed * movespeed) && isStart) {
		backg22.setPosition(0, -599);
	}

	icysprite.move(Vector2f(0, scrollspeed * movespeed));

	if (icysprite.getPosition().y < -2) {
		icysprite.setPosition(Vector2f(icysprite.getPosition().x, 1));
	}
	else if (icysprite.getPosition().y < 20)
		movespeed = 5;
	else if (icysprite.getPosition().y > 100)
		movespeed = 1;

}
// handle Collisions Function
bool checkCollision(const FloatRect& playerBounds, const Sprite& block) {
	return playerBounds.intersects(block.getGlobalBounds());
}
void handleWindows(Sprite& player)
{
	FloatRect playerBounds = player.getGlobalBounds();
	playerBounds.top += 10;
	playerBounds.width -= 40;
	playerBounds.height -= 20;
	playerBounds.left += 20;

	// Left Window
	if (player.getPosition().x < 75)
		player.setPosition(75, player.getPosition().y);
	// Right Window
	if (player.getPosition().x + player.getGlobalBounds().width > 800 - 75)
		player.setPosition(800 - 75 - player.getGlobalBounds().width, player.getPosition().y);
}
void handleCollisions(Sprite& player) {
	FloatRect playerBounds = player.getGlobalBounds();
	playerBounds.top += 10;
	playerBounds.width -= 40;
	playerBounds.height -= 20;
	playerBounds.left += 20;


	for (int i = 0; i < 10; i++) {
		if (checkCollision(playerBounds, b_drawer[i])) {

			FloatRect blockBounds = b_drawer[i].getGlobalBounds();

			float playerBottom = playerBounds.top + playerBounds.height;
			float blockTop = blockBounds.top;

			// Check if the player is falling onto the block
			if (playerBottom > blockTop + 10 && playerBounds.top < blockTop) {
				player.setPosition(player.getPosition().x, blockTop - playerBounds.height);
				isJumping = false;
				jumpVelocity = 0.0f;
				player.setRotation(0.0f);
			}
		}
	}
	if (player.getPosition().y < 500 && !isJumping) {
		icysprite.move(0.0f, jumpVelocity += gravity);
	}
}
void drawBoundingBoxes(RenderWindow& window, const Sprite& player) {
	// Draw bounding boxes for blocks
	for (int i = 0; i < 10; i++) {
		FloatRect blockBounds = b_drawer[i].getGlobalBounds();
		RectangleShape boundingBox(Vector2f(blockBounds.width, blockBounds.height));
		boundingBox.setPosition(blockBounds.left, blockBounds.top);
		boundingBox.setFillColor(Color::Transparent);
		boundingBox.setOutlineColor(Color::Cyan);
		boundingBox.setOutlineThickness(1.0f);
		window.draw(boundingBox);
	}

	// Draw bounding box for player
	FloatRect playerBounds = player.getGlobalBounds();
	playerBounds.top += 10;
	playerBounds.width -= 40;
	playerBounds.height -= 20;
	playerBounds.left += 20;
	RectangleShape playerBoundingBox(Vector2f(playerBounds.width, playerBounds.height));
	playerBoundingBox.setPosition(playerBounds.left, playerBounds.top);
	playerBoundingBox.setFillColor(Color::Transparent);
	playerBoundingBox.setOutlineColor(Color::Cyan);
	playerBoundingBox.setOutlineThickness(1.0f);
	window.draw(playerBoundingBox);
}

bool gameover = false;
bool isSoundPlayed = false;
void reset_game(RenderWindow& window)
{
	// Reset player position
	icysprite.setPosition(400, 500); // Assuming the initial position is (400, 500)
	icysprite.setRotation(0.0f);
	isJumping = false;
	jumpVelocity = 0.0f;

	// Reset score
	playerScore = 0;
	scoreText.setString("Score: " + to_string(playerScore));
	// Reset isSoundPlayed
	isSoundPlayed = false;
	// Reset block positions and scores
	generateBlocks();

	// Reset background positions
	backg11.setPosition(0, 0);
	backg12.setPosition(0, 0);
	backg21.setPosition(0, -599);
	backg22.setPosition(0, -599);
	gameover = false;
	// Reset scrolling variables
	scrollspeed = 1;
	isStart = false;
	isscrolling = false;
	movespeed = 1;

}

//variables
int dispindex = -1; //-1 is the intro - The main menu
//Game Over

Texture over;
Sprite oversprite;
Clock gameOverClock;
bool canspace = false;
SoundBuffer gameOverBuffer;
Sound gameOverSound;

void gameOver(RenderWindow& window)
{
	gameover = true;
	
	
	over.loadFromFile("Assets/game over.png");
	oversprite.setTexture(over);
	window.draw(oversprite);
	
	if (gameOverClock.getElapsedTime().asSeconds() > 5.0f)
	{
		canspace = true;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space) && canspace)
	{
		dispindex = -1;
		reset_game(window);
		canspace = false;
		gameOverClock.restart();
	}

}

// Instruction Variables
string instruct_names[3] = { "Assets/instructions1.jpg","Assets/instructions2.jpg","Assets/instructions3.jpg" };
Texture instruct[3];
Sprite instructions[3];
int instruct_number = 0;

Clock instructionClock;
Time delayTime = seconds(0.18f); // 0.18 seconds delay
//credits variables
Texture credit;
Sprite credits;


void instructions_menu() {
	if (instructionClock.getElapsedTime() >= delayTime) {
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			if (instruct_number < 2) {
				instruct_number++;
				instructionClock.restart(); // Restart the clock
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left)) {
			if (instruct_number > 0) {
				instruct_number--;
				instructionClock.restart(); // Restart the clock
			}
		}
	}
}

int main() {
	RenderWindow window(VideoMode(800, 600), "ICY TOWER");
	window.setFramerateLimit(120);
	View camera = window.getDefaultView();
	//window.setMouseCursorVisible(false);    
	srand(time(NULL));
	//Font
	font.loadFromFile("Assets/ARABOLIC.ttf");
	//Icon of the game
	icon.loadFromFile("Assets/cursor.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	curs.loadFromFile("Assets/cursor.png");
	// Intro background - Main Menu Background
	Texture intro;
	intro.loadFromFile("Assets/intro-background.jpg");
	Sprite menubackg;
	menubackg.setTexture(intro);
	//Cursor load
	Sprite cursor;
	cursor.setTexture(curs);
	cursor.setScale(0.07f, 0.09f);
	//Menu options
	Text menu[4];
	menu[0].setFont(font);
	menu[0].setFillColor(Color::Red);
	menu[0].setString("Play");
	menu[0].setPosition({ 476.0f,250.0f });
	menu[0].setCharacterSize(45);
	//Instructions
	menu[1].setFont(font);
	menu[1].setFillColor(Color::Black);
	menu[1].setString("Instructions");
	menu[1].setPosition({ 476.0f,325.0f });
	menu[1].setCharacterSize(45);
	//Credits
	menu[2].setFont(font);
	menu[2].setFillColor(Color::Black);
	menu[2].setString("Credits");
	menu[2].setPosition({ 476.0f,400.0f });
	menu[2].setCharacterSize(45);
	//Exit
	menu[3].setFont(font);
	menu[3].setFillColor(Color::Black);
	menu[3].setString("Exit");
	menu[3].setPosition({ 476.0f,475.0f });
	menu[3].setCharacterSize(45);

	cursor.setPosition(Vector2f(420.0f, 255.0f));
	int menuindex = 0;

	//load icy

	icy.loadFromFile("Assets/icysprite.png");

	icysprite.setTexture(icy);
	icysprite.setScale(0.8, 0.8);
	icysprite.setPosition({ 400.0f,500.0f });
	Vector2u texturesize = icy.getSize();
	texturesize.x /= 6;
	texturesize.y /= 4;
	IntRect anim(texturesize.x * 0, texturesize.y * 0, texturesize.x, texturesize.y);
	icysprite.setTextureRect(anim);
	//Sounds
	gameOverBuffer.loadFromFile("Assets/gameover.wav");
	gameOverSound.setBuffer(gameOverBuffer);
	Clock clock;
	// Load block textures
	loadBlockTextures();
	// Generate blocks
	generateBlocks();
	bool enable = false;
	while (window.isOpen())
	{
		Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			if (windowEvent.type == Event::Closed)
				window.close();
			if (windowEvent.type == Event::KeyPressed)
			{
				if (windowEvent.key.code == Keyboard::Down && menuindex < 3)
				{
					Vector2f pos = cursor.getPosition();
					cursor.setPosition(pos.x, pos.y + 75.0f);
					menu[menuindex].setFillColor(Color::Black);
					menuindex++;
					menu[menuindex].setFillColor(Color::Red);
				}
				if (windowEvent.key.code == Keyboard::Up && menuindex > 0)
				{
					Vector2f pos = cursor.getPosition();
					cursor.setPosition(pos.x, pos.y - 75.0f);
					menu[menuindex].setFillColor(Color::Black);
					menuindex--;
					menu[menuindex].setFillColor(Color::Red);
				}
				if (windowEvent.key.code == Keyboard::Enter)
				{
					switch (menuindex)
					{
					case 0:
						dispindex = menuindex * 10;
						break;
					case 1:
						dispindex = menuindex * 10;
						break;
					case 2:
						dispindex = menuindex * 10;
						break;
					case 3:
						dispindex = menuindex * 10;
						break;
					}
				}
				if (windowEvent.key.code == Keyboard::Escape  && dispindex != 0) {
					if (instruct_number == 0)
						dispindex = -1;
					else instruct_number--;

				}

			}
		}
		//Main Menu
		if (dispindex == -1)
		{
			window.draw(menubackg);
			for (int i = 0; i < 4; i++)
				window.draw(menu[i]);
			window.draw(cursor);
		}
		//Play
		else if (dispindex == 0)
		{
			if (Keyboard::isKeyPressed(Keyboard::Key::D))
			{
				icysprite.move(2.0f, 0.0f);
				icysprite.setOrigin(0, 0);
				anim.top = texturesize.y;
				if (clock.getElapsedTime().asSeconds() > 0.1f)
				{
					if (anim.left == texturesize.x * 5) { anim.left = 0; }
					else anim.left += texturesize.x;
					icysprite.setTextureRect(anim);
					clock.restart();
				}
			}
			else if (Keyboard::isKeyPressed(Keyboard::Key::A))
			{
				icysprite.move(-2.0f, 0.0f); // Move the sprite to the left by 5 units
				icysprite.setOrigin(0, 0);
				anim.top = texturesize.y * 3; // Set the correct row for the left movement animation
				if (clock.getElapsedTime().asSeconds() > 0.1f)
				{
					if (anim.left == texturesize.x * 5) { anim.left = 0; }
					else { anim.left += texturesize.x; }
					icysprite.setTextureRect(anim);
					clock.restart();
				}
			}


			else
			{
				icysprite.setOrigin(0, 0);
				anim.top = texturesize.y * 0; // Set the correct row for the left movement animation
				if (clock.getElapsedTime().asSeconds() > 0.1f)
				{
					if (anim.left == texturesize.x * 5) { anim.left = 0; }
					else { anim.left += texturesize.x; }
					icysprite.setTextureRect(anim);
					clock.restart();
				}

			}
			
			
			window.clear(Color::White);
			if (icysprite.getPosition().y < 300)
				isscrolling = true;
			if(isscrolling)
			{
				scrolling(icysprite);
			}
			if (jumpVelocity > 0)
				handleCollisions(icysprite);
			handleWindows(icysprite);
			if (!gameover)
				handleJump(icysprite);
			// Draw blocks
			drawBlocks(window);
			if (icysprite.getPosition().y > 500 && isscrolling)
			{
				if(!isSoundPlayed){
				 gameOverSound.play();
				 isSoundPlayed = true;
				}
				gameOver(window);
				
			}
			if (Keyboard::isKeyPressed(Keyboard::Key::B))
			{
				enable = true;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Key::U))
				enable = false;
			if (enable)
				// Draw bounding boxes
				drawBoundingBoxes(window, icysprite);
			// Update and draw blocks
			updateScore(icysprite);
			window.draw(icysprite);

		}
		//Instructions
		else if (dispindex == 10) {

			for (int i = 0; i < 3; i++)
				instruct[i].loadFromFile(instruct_names[i]);

			for (int i = 0; i < 3; i++)
				instructions[i].setTexture(instruct[i]);

			instructions_menu();

			window.draw(instructions[instruct_number]);

		}
		//Credits
		else if (dispindex == 20) {
		      
			credit.loadFromFile("Assets/Credits.png");
			credits.setTexture(credit);
			window.draw(credits);
		}
		//Exit
		else if (dispindex == 30) {

			window.close();
		}
		window.display();

	}
}
