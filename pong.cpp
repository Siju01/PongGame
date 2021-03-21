#include <iostream> //using IO functions
#include <string> //using string function
#include <stdlib.h> //srand function
#include <fstream> //fstream funtion
#include <thread> //using thread function
#include <mutex> //to lock
#include <time.h> //randomize funtion
#include <condition_variable> //cond variable for mutex

using namespace std;
mutex mu; //lock variable
condition_variable cond;

class Score { //create class score
private:
	int playerXScore;
	int playerYScore;

public:
	void displayGame();

	Score(int x, int y) { //define variable player x and y
		playerXScore = x;
		playerYScore = y;
	}

	void setScore(int x, int y) {//setter score
		playerXScore += x;
		playerYScore += y;
	}

	int getX() { //getter X score
		return playerXScore;
	}

	int getY() { //getter Y score
		return playerYScore;
	}
};

Score scr(0, 0); //Create score instance as global variable

class Player { //create class player
	private:
		enum player { //create enum
			playerX,
			playerY
		}
		posisiPlayer;
		float hits;

	public:
		void displayGame();

		Player(char cek) { //cek posisiPlayer is X or Y
			hits = 0;
			if (cek == 'X') {
				posisiPlayer = player::playerX;
			}
			else {
				posisiPlayer = player::playerY;
			}
		}

		player getPlayer() { //getter player
			return posisiPlayer;
		}

		Player(Player& clone) { //clone player
			hits = clone.hits;
			//if else about clone previous to new player 
			if (clone.posisiPlayer == player::playerX) { 
				posisiPlayer = player::playerY;
			}
			else {
				posisiPlayer = player::playerX;
			}
		}

		char randomPlayer() { //random player X or O
			int r;
			int* i = new int;
			srand(int(i));
			delete i;

			r = rand() % 2;
			if (r == 0)
				return 'X';
			else
				return 'Y';
		}

		void setHits(float h) { //setter hits
			this->hits = h;
		}

		float getHits() { //getter hits
			return hits;
		}
};

void hitPlayer(Player* p, bool* h) { //function of player hits
	//lock hitPlayer
	mu.lock();
	
	//random hit number 0 to 100 
	int* r = new int;
	srand((int)r);
	delete r;
	p->setHits(rand() % 100);

	//print player hits number
	if (p->getPlayer() == 0)
		cout << "Player X Hits";
	else
		cout << "Player Y Hits";
	cout << "\t= " << p->getHits() << endl;

	//if Player.Hits is less than or equal to 50; update score +1 to opponent score
	if (p->getHits() <= 50) {
		if (p->getPlayer() == 0) {
			scr.setScore(0, 1);
			cout << "Score X \t= " << scr.getX() << "\nScore Y \t= " << scr.getY() << endl;
		}
		else {
			scr.setScore(1, 0);
			cout << "Score X \t= " << scr.getX() << "\nScore Y \t= " << scr.getY() << endl;
		}
		cout << "//Player is successful hitting" << endl;
		*h = true;
	}
	
	else { //if didn't hit.
		cout << "\nRepeat again to other player" << endl; 
	}

	//delay 1 second after every printing the odd number
	this_thread::sleep_for(chrono::seconds(1));
	mu.unlock(); //unlock hitPlayer using mutex
}

void Thread() { //function of thread
	srand(time(NULL)); //generate random
	Player PlayerX('X'); //create player instance once
	Player PlayerY = PlayerX; //clone previous to new player (deep player)

start:
	//simulation flow (Main Loop)
	while (scr.getX() < 10 && scr.getY() < 10) { //stop apllication if any player has score 10
		bool h = false;
		// Start from random player, player X or player Y
		while (h == false) {
			if (PlayerX.randomPlayer() == 'X')
			{
				cout << "\nPLAYER X : " << endl;

			startAgain:
				//Create 2 thread and cast each player instance to each thread
				//Player X thread 
				thread tx(hitPlayer, &PlayerX, &h);
				tx.join();

				if (h == true)
					break;

				// Player Y Thread
				thread ty(hitPlayer, &PlayerY, &h);
				ty.join();
				if (h == true)
					break;
				else
					goto startAgain;
			}

			else { //if random player is player Y
				cout << "\nPLAYER Y : " << endl;
			startAgain2:
				// Player Y Thread
				thread ty(hitPlayer, &PlayerY, &h);
				ty.join();

				if (h == true)
					break;

				// Player X Thread
				thread tx(hitPlayer, &PlayerX, &h);
				tx.join();

				if (h == true)
					break;
				else
					goto startAgain2;
			}
		}
	}
}

void printWinner() {
	//show player win with final score
	if (scr.getX() >= 10) { //if player X win
		cout << "\n===PLAYER X WIN=== \nSCORE :" << scr.getX() << " - " << scr.getY()<<endl;
	}
	else { //if player Y win
		cout << "\n===PLAYER Y WIN=== \nSCORE : " << scr.getY() << " - " << scr.getX()<<endl;
	}
}

int main() {
	cout << "=======PONG SIMULATION=======" << endl;
	Thread(); //call thread function in program
	printWinner(); //call printWinner function in program
	return 0;
}