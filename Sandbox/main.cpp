#include <iostream>
#include "SFML/Graphics.hpp"
#include "Global.hpp"

/*
DISCLAIMER:
this code is based on the Lode's Computer Graphics Tutorial

all credits for the raycasting algorithm goes to him.
https://lodev.org/cgtutor/raycasting.html
*/


sf::RenderWindow* window;

//dt
sf::Clock timer;
float dt = 0.0f;

//Fov
sf::Vector2f plane(-0.66f, 0.0f);
//player stuff
sf::RectangleShape p;
sf::Vector2f pos(2.5f,2.5f);
sf::Vector2f direction(0.0f, 1.0f);

//wall stuff
sf::VertexArray* lines{};
sf::RenderStates state;
//map stuff
bool showMap = false;
bool mPressed = false;
std::vector<sf::RectangleShape> tiles;
std::vector<sf::CircleShape> points;
std::vector<sf::Vertex> rayArea;

long tileMap[24][24] = //this is the current map that the raycasting machine is using 1 for wall 0 for nothing
//{
//  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
//  {1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1},
//  {1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,1,0,0,1},
//  {1,0,1,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1},
//  {1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,0,0,1},
//  {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
//  {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
//  {1,0,1,1,1,1,0,0,0,0,5,6,7,4,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
//};
{
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,5,1,2,3,7,5,6,6,7,7,3,6,5,2,1,1,5,3,2,2,7,5,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1},
{1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
{1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
{1,0,0,0,0,0,9,9,9,9,9,9,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,6,0,0,0,0,0,8,8,8,8,8,8,8,8,8,8,0,0,0,0,1},
{1,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,3,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1},
{1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,4,4,7,4,4,4,1,1,6,2,7,5,4,3,5,2,3,2,6,4,2,2,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
// {
//{ 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8 },
//{ 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8 },
//{ 8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,8,8,8,0,0,8 },
//{ 8,0,0,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,8 },
//{ 8,0,0,0,0,8,0,0,0,0,0,0,0,8,8,8,8,8,0,0,8,0,0,8 },
//{ 8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,8,0,0,8 },
//{ 8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,8 },
//{ 8,0,0,0,0,0,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,8,0,0,0,0,0,8,8,8,8,8,8,8,8,8,8,0,0,0,0,8 },
//{ 8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,8,0,0,0,0,0,0,0,8,8,8,8,8,0,0,0,0,0,0,0,8 },
//{ 8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,0,8,8,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8 },
//{ 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8 },
//{ 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8 }
//};
//tex stuff
sf::Texture* texMap;
/*
INITIALIZATION
*/

void initTextures() {
	texMap = new sf::Texture;
	texMap->loadFromFile("Textures/ytfriendly_map.png");

	//set the tex parameters
	TEXTURE_MAP::texMapSize = sf::Vector2i(texMap->getSize());
	texMap->setRepeated(false);
	state = texMap;

	lines = new sf::VertexArray(sf::PrimitiveType::Lines, 18 * WINDOW::size.x);
}

void initWindow() {
	window = new sf::RenderWindow;
	window->create(sf::VideoMode(sf::Vector2u{ WINDOW::size }), "Raycasting", sf::Style::Close);

	window->setFramerateLimit(144);
}

void initPlayer() {
	p.setSize(sf::Vector2f(tileSize/2.f, tileSize / 2.f));
	p.setFillColor(sf::Color::Yellow);
	p.setPosition({ 500, 500 });
}

void initTileMap() {
	int x, y;
	for (y = 0; y < std::size(tileMap); y++) {
		for (x = 0; x < std::size(tileMap); x++) {
			sf::RectangleShape tile;
			if (tileMap[y][x] > 0) {
				tile.setFillColor(sf::Color::White);
			}
			else
				tile.setFillColor(sf::Color::Black);
			tile.setPosition(sf::Vector2f(x * tileSize, y * tileSize));
			tile.setSize(sf::Vector2f(tileSize -1, tileSize - 1));
			tiles.push_back(tile);
		}
	}
}

void init() {
	initTextures();
	initWindow();
	initPlayer();
	initTileMap();
}

/*
POLLEVENTS
*/

void pollEvents() {
	sf::Event ev;
	while (window->pollEvent(ev)) {
		switch (ev.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				window->close();
			break;
		}
	}
}

//functions
sf::Vector2f rotateVec(sf::Vector2f vec, float value) {
	return sf::Vector2f(
		vec.x * std::cos(value) - vec.y * std::sin(value),
		vec.x * std::sin(value) + vec.y * std::cos(value)
	);
}

sf::Vector2u getTexPos(int tile) {
	tile -= 1;
	int texPosY = 0;
	texPosY = std::floor(tile / (TEXTURE_MAP::texMapSize.x / TEXTURE_MAP::texSize.x));
	return sf::Vector2u(
		(tile - texPosY * (TEXTURE_MAP::texMapSize.x / TEXTURE_MAP::texSize.x)) * TEXTURE_MAP::texSize.x,
		texPosY * TEXTURE_MAP::texSize.y
	);
}

/*
UPDATE
*/

void updateMov() {
	float moveForward = 0.0f;
	float rotDir = 0.0f;
	// get input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		moveForward = 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		moveForward = -1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		rotDir = -1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		rotDir = 1.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		PLAYER::movSpeed = 15.f;
	}
	else
		PLAYER::movSpeed = 5.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && !mPressed) {
		mPressed = true;
		showMap = !showMap;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
		mPressed = false;
	}

	if (moveForward != 0.0f) {
		sf::Vector2f moveVec = direction * PLAYER::movSpeed * moveForward * dt;
		pos += moveVec;
	}

	if (rotDir != 0.0f) {
		float rotation = PLAYER::rotSpeed * rotDir * dt;
		direction = rotateVec(direction, rotation);
		plane = rotateVec(plane, rotation);
	}

	p.setPosition(pos * float(tileSize));
}

void raycast() {
	window->clear(sf::Color(40, 40, 40, 0));
	for (int x = 0; x < WINDOW::size.x; x++) {

		int mapX = int(pos.x);
		int mapY = int(pos.y);

		double cameraX = 2.f * x / double(WINDOW::size.x) - 1.f; //x-coordinate in camera space
		double rayDirX = direction.x + plane.x * cameraX;
		double rayDirY = direction.y + plane.y * cameraX;


		double sideDistX;
		double sideDistY;

		//double deltaDistX = static_cast<float>(sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX)));
		//double deltaDistY = static_cast<float>(sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY)));

		double deltaDistX = sqrt(1.0f + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		double deltaDistY = sqrt(1.0f + (rayDirX * rayDirX) / (rayDirY * rayDirY));

		int stepX;
		int stepY;

		bool hit = false;

		int side;
		int tile = 0;
		double perpWallDist;

		if (rayDirX < 0) {
			stepX = -1;
			sideDistX = float(pos.x - mapX) * deltaDistX;
		}
		else {
			stepX = 1;
			sideDistX = float(mapX + 1.0 - pos.x) * deltaDistX;
		}

		if (rayDirY < 0) {
			stepY = -1;
			sideDistY = float(pos.y - mapY) * deltaDistY;
		}
		else {
			stepY = 1;
			sideDistY = float(mapY + 1.0 - pos.y) * deltaDistY;
		}

		//dda algorythm 
		while (!hit) {
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
				perpWallDist = (mapX - pos.x + (1 - stepX) / 2) / rayDirX;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
				perpWallDist = (mapY - pos.y + (1 - stepY) / 2) / rayDirY;
			}
			if (tileMap[mapY][mapX] > 0) {
				hit = true;
				tile = tileMap[mapY][mapX];
			}
			else if (mapX > 24 || mapY > 24)
				break;

			

		}

		//tex
		int lineHeight = (int)(WINDOW::size.y / perpWallDist);
		int drawStart = WINDOW::size.y / 2 - lineHeight / 2;
		int drawEnd = WINDOW::size.y / 2 + lineHeight / 2;
		
		sf::Vector2u texCoords = getTexPos(tile);

		float wallX;
		if (side == 0)
			wallX = pos.y + perpWallDist * rayDirY;
		else 
			wallX = pos.x + perpWallDist * rayDirX; 

		wallX -= floor(wallX);

		int texX = int(wallX * float(TEXTURE_MAP::texSize.x));

		if ((side == 0 && rayDirX <= 0) || (side == 1 && rayDirY >= 0)) {
			texX = TEXTURE_MAP::texSize.x - texX - 1;
		}

	/*	if (x % 5 ==1) {*/
			lines->append(sf::Vertex(
				sf::Vector2f(x, (float)drawStart),
				sf::Color::White,
				sf::Vector2f(texCoords.x + (float)texX, texCoords.y) //1
			));
			lines->append(sf::Vertex(
				sf::Vector2f(x, (float)drawEnd),
				sf::Color::White,
				sf::Vector2f(texCoords.x + (float)texX, (float)(texCoords.y + TEXTURE_MAP::texSize.y))
			));
	/*	}*/
		
		{
			if (x % 2 != 0) {
			sf::CircleShape circle;
			circle.setFillColor(sf::Color::Red);
			circle.setRadius(2);
			sf::Vector2f move = sf::Vector2f{ (float)rayDirX, (float)rayDirY } * float(perpWallDist * tileSize);
			circle.setPosition(pos * float(tileSize) + move - sf::Vector2f{2.f,2.f});
			points.push_back(circle);
			}

			sf::Vertex v;
			v.position = sf::Vector2f(pos.x * tileSize + perpWallDist * rayDirX * tileSize, pos.y * tileSize + perpWallDist * rayDirY * tileSize);
			rayArea.push_back(v);

		}

	}
	rayArea[0].position = sf::Vector2f(pos * float(tileSize));
	window->draw(*lines, state);
}

void update() {
	dt = timer.restart().asSeconds();
	updateMov();
	raycast();
};

/*
RENDER
*/

void render() {
	if (showMap) {
		for (auto& t : tiles)
			window->draw(t);

		window->draw(p);
		for (auto& p : points)
			window->draw(p);
		window->draw(rayArea.data(), rayArea.size(), sf::PrimitiveType::TriangleFan);
	}
	window->display();
	lines->clear();
	points.clear();
	rayArea.clear();
}

int main() {
	init();
	dt = timer.restart().asSeconds();

	while(window->isOpen()) {
		pollEvents();
		update();
		render();
	}

	delete window;
	delete lines;
	delete texMap;
	return 1;
}