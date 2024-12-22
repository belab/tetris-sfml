#include <SFML/Graphics.hpp>
#include <array>
#include <chrono>

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = {0};
struct Point {
	int x{}, y{};
};

struct Tetromino {
	explicit Tetromino(int shapeNr) {
		const int figures[7][4] = {
			1, 3, 5, 7, // I     0 1
			2, 4, 5, 7, // Z     2 3
			3, 5, 4, 6, // S     4 5
			3, 5, 4, 7, // T     6 7
			2, 3, 5, 7, // L
			3, 5, 7, 6, // J
			2, 3, 4, 5, // O
		};
		for (int i = 0; i < 4; i++)	{
			shape[i].x = figures[shapeNr][i] % 2; // 1 1 1 1
			shape[i].y = figures[shapeNr][i] / 2; // 0 1 2 3
		}
	}

	bool check() {
		for (auto& point : shape) {
			if (point.x < 0 || point.x >= N || point.y >= M)
				return false;
			else if (field[point.y][point.x] > 0)
				return false;
		}
		return true;
	};

	void moveAlongX(int dx){
		auto backup = shape;
		for (auto &point : shape) {
			point.x += dx;
		}
		if (!check())
			shape = backup;
	}
	void rotate(){
		auto backup = shape;
		Point center = shape[1]; // center of rotation
		for (auto &point : shape) {
			int x = point.y - center.y;
			int y = point.x - center.x;
			point.x = center.x - x;
			point.y = center.y + y;
		}
		if (!check())
			shape = backup;
	}

	bool moveDown(){
		auto backup = shape;
		for (auto &point : shape) {
			point.y += 1; // move down
		}
		if(!check()) {
			shape = backup;
			return false;
		}
		return true;
	}

	void printToField(int colorNum) {
		for (auto &point : shape)
			field[point.y][point.x] = colorNum;
	}

	std::array<Point, 4> shape;
};

int main() {
	srand(124);

	RenderWindow window(VideoMode(360, 360), "TETRIS-sfml", Style::Titlebar | Style::Close);

	sf::Font font;
	font.loadFromFile("lcd.14.otf");
	sf::Text scoreText{"0", font};
	scoreText.setPosition(220.0f, 0.0f);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(sf::Color::White);

	Texture t1;
	t1.loadFromFile("images/tiles.png");
	Sprite s(t1);

	int colorNum = 1;
	float timer = 0.01f, delay = 0.0f;
	auto startTime = std::chrono::high_resolution_clock::now();
	Tetromino a{colorNum-1};
	int scoreValue = 0;
	while (window.isOpen()) {
		auto stopTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
		startTime = std::chrono::high_resolution_clock::now();
		timer+=time;

		int dx = 0;
		bool rotate = false;
		Event e;
		while (window.pollEvent(e))	{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up)
					rotate = true;
				else if (e.key.code == Keyboard::Left)
					dx = -1;
				else if (e.key.code == Keyboard::Right)
					dx = 1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05f;

		a.moveAlongX(dx);
		if (rotate)
			a.rotate();

		// move down
		if (timer > delay) {
			if(!a.moveDown()){
				a.printToField(colorNum);
				colorNum = 1 + rand() % 7;
				a = Tetromino{colorNum-1};
			}
			timer = 0;
		}

		// check and clear lines
		int k = M - 1;
		for (int i = M - 1; i > 0; i--) {
			int count = 0;
			for (int j = 0; j < N; j++) {
				if (field[i][j] > 0)
					count++;
				field[k][j] = field[i][j];
			}
			if (count < N)
				k--;
			else
				scoreValue+=100;
		}

		dx = 0;
		rotate = 0;
		delay = 0.3f;

		/////////draw//////////
		window.clear(Color::Black);
		RectangleShape boundary{{2,360}};
		boundary.setPosition(180.f,0);
		boundary.setFillColor(Color{55,55,55});
		window.draw(boundary);
		scoreText.setString(std::to_string(scoreValue));
		window.draw(scoreText);
		// render field
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++) {
				if (field[i][j] == 0)
					continue;
				s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				s.setPosition(j * 18.f, i * 18.f);
				window.draw(s);
			}

		// render moving tetromino
		for (int i = 0; i < 4; i++) {
			s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			s.setPosition(a.shape[i].x * 18.f, a.shape[i].y * 18.f);
			window.draw(s);
		}

		window.display();
	}

	return 0;
}
