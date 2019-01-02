#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>

const int NotUsed = system("color 02");
using namespace std;
bool lose;
bool stope=false;
char map = 0, speed = 0, ch = 0, res = 0, button = 0, speed_stop;
int maxscore = 0, pause = 0;
COORD position;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
enum Direction { Stop = 0, L, R, U, D, P };
Direction  dir;

class Snake {
public:
	const int width = 20;
	const int height = 12;
	int x, y, fX, fY, score;
	int tailX[100], tailY[100];
	int colF = 0;


	void SetColor(int text, int Fon);
	void Init();
	void Draw();
	void MoveSnake();
	void Logic();
	void Play();
	void Map();
	void tail();
	void Score();
	void Lose();
	void restart();
	void pause();
	void StartMenu2();
}snake;
void StartMenu1();
void Begin();
// Цвета
void Snake::SetColor(int text, int Fon) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, (Fon << 4) + text);
};
// Первоначальные настройки
void Snake::Init() {
	lose = false;
	dir = Stop;
	snake.x = snake.width / 2 - 1;
	snake.y = snake.height / 2 - 1;
	srand(time(NULL));
	snake.fX = rand() % (snake.width - 1);
	snake.fY = rand() % (snake.height - 1);
	snake.score = 0;
}
// Прорисовка элементов
void Snake::Draw() {
	position.X = 0; position.Y = 0;
	SetConsoleCursorPosition(hConsole, position);
	for (int i = 0; i < snake.width + 1; i++)
		cout << "8";
	cout << endl;
	for (int i = 0; i < snake.height; i++) {
		for (int j = 0; j < snake.width; j++) {
			if (j == 0 || j == snake.width - 1)
				cout << "8";

			if (i == snake.y && j == snake.x) {
				SetColor(7, 0);
				cout << "O";
				SetColor(2, 0);
			}
			else if (i == snake.fY && j == snake.fX) {
				SetColor(4, 0);
				cout << "0";
				SetColor(2, 0);
			}
			else {
				bool print = false;
				for (int n = 0; n < snake.colF; n++) {
					if (snake.tailX[n] == j && snake.tailY[n] == i) {
						print = true;
						SetColor(7, 0);
						cout << "*";
						SetColor(2, 0);
					}
				}
				if (!print)
					cout << " ";
			}

		}
		cout << endl;
	}


	for (int i = 0; i <snake.width + 1; i++)
		cout << "8";

	Score();
};

//Счёт
void Snake::Score() {
	cout << endl;
	cout << "Ваш счет: ";
	SetColor(15, 0);
	cout << "" << snake.score;
	SetColor(2, 0);
}

// Управление кнопками передвижения и выхода из игры
void Snake::MoveSnake() {
	if (_kbhit()) {
		switch (_getch()) {
		case 75: if (dir != R) dir = L; break;
		case 77: if (dir != L) dir = R; break;
		case 72: if (dir != D) dir = U; break;
		case 80: if (dir != U) dir = D; break;
		case 8: dir = P;  break;
		case 27: lose = 1; break;
		}
	}
}

// Логика игры: хвост, условия для передвижения,  граница карты
void Snake::Logic() {
	position.X = 0; position.Y = 0;
	SetConsoleCursorPosition(hConsole, position);
	int prevX = snake.tailX[0]; snake.tailX[0] = snake.x;
	int prevY = snake.tailY[0]; snake.tailY[0] = snake.y;
	int prev2x, prev2y;

	for (int i = 1; i < snake.colF; i++) {
		prev2x = snake.tailX[i]; prev2y = snake.tailY[i];
		snake.tailX[i] = prevX; snake.tailY[i] = prevY;
		prevX = prev2x; prevY = prev2y;
		
	}

	switch (dir) {
	case D: { if (dir != U) snake.y++; break; }
	case L: { if (dir != R) snake.x--; break; }
	case R: { if (dir != L) snake.x++; break; }
	case U: { if (dir != D) snake.y--; break; }
	}


	for (int i = 0; i < snake.colF; i++) {
		if (snake.tailX[i] == snake.x && snake.tailY[i] == snake.y)
			lose = true;
	}
	Map();
	tail();

}

// Режим границ
void Snake::Map() {
	if (map == 1) {
		if (snake.x >= snake.width - 1 || snake.x < 0 || snake.y == snake.height || snake.y < 0)
			lose = true;

	}

	else if (map == 2) {
		if (snake.x > snake.width - 2)
			snake.x = 0;
		if (snake.x < 0)
			snake.x = snake.width - 2;
		if (snake.y > snake.height - 1)
			snake.y = 0;
		if (snake.y < 0)
			snake.y = snake.height - 1;
		
	}

}
//  Появления фруктов, увеличения скорости
void Snake::tail() {
	if (snake.x == snake.fX && snake.y == snake.fY) {
		snake.score += 1;
		snake.fX = rand() % (snake.width - 1);
		snake.fY = rand() % (snake.height - 1);
		snake.colF++;
		speed -= 2;
	}
}

// Пауза при нажатии кнопки
void Snake::pause() {
	speed_stop = speed;
	speed_stop = _getch();
	if (_kbhit()) {
		switch (speed_stop) {
		case 'x': if (speed > 1) { speed = 1, stope = 1; }
				  else { speed_stop = speed, stope = 0; } break;
		}
	}
	//MoveSnake();
}

// Запуск игры 
void Snake::Play() {
	Init();

	//while (stope == 0) {
		//pause();
		while (!lose) {
			Draw();
			MoveSnake();
			Logic();
			Sleep(speed);
			if (snake.score > maxscore) {
				maxscore = snake.score;
			}
			
		}
	//	pause();
		Lose();
	//}
}

// Окно проигрыша
void Snake::Lose() {
	system("cls");
	SetColor(4, 0);
	cout << endl;
	cout << "`¶¶¶¶````¶¶¶¶```¶¶```¶¶`¶¶¶¶¶\t`¶¶¶¶```¶¶``¶¶``¶¶¶¶¶```¶¶¶¶¶" << endl;
	cout << "¶¶``````¶¶``¶¶``¶¶¶`¶¶¶`¶¶\t¶¶``¶¶``¶¶``¶¶``¶¶``````¶¶``¶¶" << endl;
	cout << "¶¶`¶¶¶``¶¶¶¶¶¶``¶¶`¶`¶¶`¶¶¶¶\t¶¶``¶¶``¶¶``¶¶``¶¶¶¶````¶¶¶¶¶" << endl;
	cout << "¶¶``¶¶``¶¶``¶¶``¶¶```¶¶`¶¶\t¶¶``¶¶```¶¶¶¶```¶¶``````¶¶``¶¶" << endl;
	cout << "`¶¶¶¶```¶¶``¶¶``¶¶```¶¶`¶¶¶¶¶\t`¶¶¶¶`````¶¶````¶¶¶¶¶```¶¶``¶¶" << endl;
	SetColor(2, 0);
	cout << "\nВаш счет: ";
	SetColor(15, 0);
	cout << "" << snake.score << endl;
	SetColor(2, 0);
	cout << "\nНаибольшее количество очков за данную сессию:  ";
	SetColor(15, 0);
	cout << "" << maxscore << endl;
	SetColor(14, 0);
	snake.colF = 0;
	cout << "\nПереиграем?\n" << endl;
	SetColor(2, 0);
	cout << "Нажмите ";
	SetColor(15, 0);
	cout << "Enter" << endl;
	SetColor(2, 0);
	cout << "Выход ";
	SetColor(15, 0);
	cout << "ESC" << endl;
	SetColor(2, 0);
	restart();	
}

// Рестарт
void Snake::restart(){
	res = 0;
	res = _getch();
	switch (res) {
	case 13: StartMenu2(); ; break;
	case 27: lose = 1; break;
	}
}
// окно приветствие
void Begin() {
	cout << endl;
	cout << "\t\t`¶¶¶¶```¶¶¶¶``¶¶``¶¶``¶¶¶¶```¶¶¶¶``¶¶`````¶¶¶¶¶``````````¶¶¶¶``¶¶``¶¶``¶¶¶¶``¶¶``¶¶`¶¶¶¶¶" << endl;
	cout << "\t\t¶¶``¶¶`¶¶``¶¶`¶¶¶`¶¶`¶¶`````¶¶``¶¶`¶¶`````¶¶````````````¶¶`````¶¶¶`¶¶`¶¶``¶¶`¶¶`¶¶``¶¶" << endl;
	cout << "\t\t¶¶`````¶¶``¶¶`¶¶`¶¶¶``¶¶¶¶``¶¶``¶¶`¶¶`````¶¶¶¶```¶¶¶¶¶```¶¶¶¶``¶¶`¶¶¶`¶¶¶¶¶¶`¶¶¶¶```¶¶¶¶" << endl;
	cout << "\t\t¶¶``¶¶`¶¶``¶¶`¶¶``¶¶`````¶¶`¶¶``¶¶`¶¶`````¶¶````````````````¶¶`¶¶``¶¶`¶¶``¶¶`¶¶`¶¶``¶¶" << endl;
	cout << "\t\t`¶¶¶¶```¶¶¶¶``¶¶``¶¶``¶¶¶¶```¶¶¶¶``¶¶¶¶¶¶`¶¶¶¶¶``````````¶¶¶¶``¶¶``¶¶`¶¶``¶¶`¶¶``¶¶`¶¶¶¶¶" << endl << endl << endl;

	cout << "\t\t\t¶¶```¶`¶¶``¶¶``¶¶¶¶``¶¶¶¶¶¶`¶¶``¶¶\t¶¶¶¶¶¶`¶¶``¶¶````````¶¶¶¶¶¶``¶¶¶¶" << endl;
	cout << "\t\t\t¶¶¶`¶¶``¶¶¶¶``¶¶```````¶¶```¶¶`¶¶\t``¶¶```¶¶¶`¶¶````````¶¶``¶¶`¶¶``¶¶" << endl;
	cout << "\t\t\t¶¶`¶`¶```¶¶````¶¶¶¶````¶¶```¶¶¶¶\t``¶¶```¶¶`¶¶¶`¶¶¶¶¶`````¶¶`````¶¶" << endl;
	cout << "\t\t\t¶¶```¶```¶¶```````¶¶```¶¶```¶¶`¶¶\t``¶¶```¶¶``¶¶``````````¶¶````¶¶" << endl;
	cout << "\t\t\t¶¶```¶```¶¶````¶¶¶¶``¶¶¶¶¶¶`¶¶``¶¶\t¶¶¶¶¶¶`¶¶``¶¶`````````¶¶````¶¶¶¶¶¶" << endl << endl << endl;
	StartMenu1();
}

// меню старта
void StartMenu1() {
	Snake m;
	cout << "Играть нажмите ";
	m.SetColor(15, 0);
	cout << "Enter";
	m.SetColor(2, 0);
	cout << "\nВыйти ";
	m.SetColor(15, 0);
	cout << "ESC" << endl;
	m.SetColor(2, 0);
	
	while (ch != 27) {
		ch = _getch();
		switch (ch) {
		case 13: m.StartMenu2(); break;
		case 27: lose = 1; break;
		}
	}
}
// Режимы игры
void Snake::StartMenu2() {
	cout << "Граница есть нажмите ";
	SetColor(15, 0);
	cout << "1";
	SetColor(2, 0);
	cout << " \nГраницы нет нажмите  ";
	SetColor(15, 0);
	cout << "2";
	SetColor(2, 0);
	
	map = _getch();
	switch (map) {
	case '1': map = 1; break;
	case '2': map = 2; break;
	case 27: lose = 1; break;
	}


	SetColor(15, 0);
	cout << "\n1";
	SetColor(2, 0);
	cout << " - самый легкий";
	SetColor(15, 0);
	cout << "\n2";
	SetColor(2, 0);
	cout << " - легкий";
	SetColor(15, 0);
	cout << "\n3";
	SetColor(2, 0);
	cout << " - средний";
	SetColor(15, 0);
	cout << "\n4";
	SetColor(2, 0);
	cout << " - быстрый";
	SetColor(4, 0);
	cout << "\n5";
	SetColor(2, 0);
	cout << " - ракета" << endl;
	
	speed = _getch();
	switch (speed) {
	case'1': speed = 100; break;
	case'2': speed = 90; break;
	case'3': speed = 80; break;
	case'4': speed = 70; break;
	case'5': speed = 50; break;
	case 27: lose = 1; break;
	}
	
	system("cls");
	Play();
};

int main() {
	setlocale(LC_ALL, "Russian");
	Begin();

	return 0;
}

