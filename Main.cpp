#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include "MainHeader.h"

const int WIDTH = 120;
const int HEIGHT = 30;
const int PADDLELENGTH = 7;
const int PADDLEOFFSET = 5;

struct Pair {
    int x, y;
    Pair(int x, int y) : x(x), y(y) {}
};
class Ball {
    Pair* position;
    Pair* velocity;
public:
    [[nodiscard]] Pair* getPos() const {
        return position;
    }

    [[nodiscard]] Pair* getVelocity() const {
        return velocity;
    }

    void setVelocity(Pair* inVelocity) {
        Ball::velocity = inVelocity;
    }

    Ball(Pair* pos, Pair* vel) : position(pos), velocity(vel) {}
};
class Paddle {
    int length;
    Pair* position;
public:
    Paddle(int leng, Pair* pos) : length(leng), position(pos) {}
    int getLength() {
        return length;
    }
    Pair* getPosition() {
        return position;
    }
};
class Board {
    char* display;
    Pair* boardSize;
    int* playerScores;
    Ball* ball;
    int paddleLength;
    Paddle* player1;
    Paddle* player2;
    int paddleOffset;
public:
    [[nodiscard]] char* getDisplay() const {
        return display;
    }

    void setDisplay(char* inDisplay) {
        Board::display = inDisplay;
    }

    [[nodiscard]] int* getPlayerScores() const {
        return playerScores;
    }

    void setPlayerScores(int* inPlayerScores) {
        Board::playerScores = inPlayerScores;
    }

    [[nodiscard]] Ball* getBall() const {
        return ball;
    }

    void setBall(Ball* inBall) {
        Board::ball = inBall;
    }

    [[nodiscard]] Pair* getBoardSize() const {
        return boardSize;
    }
    
    Board();
    void print();
    void update();
    void updateBall() const;
    void updatePaddle(Paddle* paddle);
    boolean checkBallCollision(boolean xInc, boolean yInc) const;
    void resetBall() const;
};

void Board::resetBall() const {
    this->display[this->ball->getPos()->x + (this->ball->getPos()->y * this->boardSize->x)] = ' ';
    this->ball->getPos()->x = this->boardSize->x / 2;
    this->ball->getPos()->y = this->boardSize->y / 2;
}

Board::Board() {
    this->boardSize = new Pair(WIDTH, HEIGHT);
    //Creates 2d array
    this->display = new char[this->boardSize->x * this->boardSize->y];
    //Sets the background
    for (int col = 0; col < this->boardSize->x; col++) {
        for (int row = 0; row < this->boardSize->y; row++) {
            if (col == this->boardSize->x - 1) {
                this->display[col + (row * this->boardSize->x)] = 0;
            }
            this->display[col + (row * this->boardSize->x)] = ' ';
        }
    }
    this->playerScores = new int[2];
    this->playerScores[0] = 0;
    this->playerScores[1] = 0;
    this->ball = new Ball(new Pair(this->boardSize->x / 2, this->boardSize->y / 2), new Pair(1, 45));
    //Sets the ball on the display
    this->display[this->ball->getPos()->x + (this->ball->getPos()->y * this->boardSize->x)] = '@';
    //Initalize the paddles
    this->paddleLength = PADDLELENGTH;
    this->paddleOffset = PADDLEOFFSET;
    this->player1 = new Paddle(this->paddleLength, new Pair(this->boardSize->x - this->paddleOffset, this->boardSize->y / 2));
    this->player2 = new Paddle(this->paddleLength, new Pair(this->paddleOffset, this->boardSize->y / 2));
}

void Board::print() {
    for (int i = 0; i <= this->boardSize->x + 1; i++) {
        std::cout << '=';
    }
    std::cout << std::endl;
    //Print background
    for (int row = 0; row < this->boardSize->y; row++) {
        std::cout << '|';
        for (int col = 0; col < this->boardSize->x; col++) {
            std::cout << this->display[col + (row * this->boardSize->x)];
        }
        std::cout << '|';
        std::cout << std::endl;
    }
    for (int i = 0; i <= this->boardSize->x + 1; i++) {
        std::cout << '=';
    }
    std::cout << std::endl;
}

void Board::update() {
    //Resets ball
    this->display[this->ball->getPos()->x + (this->ball->getPos()->y * this->boardSize->x)] = ' ';
    //Moves ball to new position
    updateBall();
    //Updates paddles and displays them to the buffer
    updatePaddle(this->player1);
    updatePaddle(this->player2);
    //Updates the score
    this->display[(this->boardSize->x / 2) - 1] = '0' + this->getPlayerScores()[0];
    this->display[(this->boardSize->x / 2) + 1] = '0' + this->getPlayerScores()[1];
    //Sets the ball display
    this->display[this->ball->getPos()->x + (this->ball->getPos()->y * this->boardSize->x)] = '@';
}

void Board::updateBall() const {
    int horizontalMultiplier = 1;
    if (ball->getVelocity()->y == 45) {
        if (ball->getPos()->y <= 0) {
            ball->getPos()->y = 0;
            ball->getVelocity()->y = 315;
            this->updateBall();
        }
        else if (ball->getPos()->x >= boardSize->x - 1) {
            this->resetBall();
            this->getPlayerScores()[0] += 1;
            this->updateBall();
        }
        else if (this->checkBallCollision(true, false)) {
            ball->getVelocity()->y = 135;
            this->updateBall();
        }
        else {
            ball->getPos()->x += ball->getVelocity()->x * horizontalMultiplier;
            ball->getPos()->y -= ball->getVelocity()->x;
        }
    }
    else if (ball->getVelocity()->y == 315) {
        if (ball->getPos()->y >= boardSize->y - 1) {
            ball->getPos()->y = boardSize->y - 1;
            ball->getVelocity()->y = 45;
            this->updateBall();
        }
        else if (ball->getPos()->x >= boardSize->x - 1) {
            this->resetBall();
            this->getPlayerScores()[0] += 1;
            this->updateBall();
        }
        else if (this->checkBallCollision(true, true)) {
            ball->getVelocity()->y = 225;
            this->updateBall();
        }
        else {
            ball->getPos()->x += ball->getVelocity()->x * horizontalMultiplier;
            ball->getPos()->y += ball->getVelocity()->x;
        }
    }
    else if (ball->getVelocity()->y == 135) {
        if (ball->getPos()->y <= 0) {
            ball->getPos()->y = 0;
            ball->getVelocity()->y = 225;
            this->updateBall();
        }
        else if (ball->getPos()->x <= 0) {
            this->resetBall();
            this->getPlayerScores()[1] += 1;
            this->updateBall();
        }
        else if (this->checkBallCollision(false, false)) {
            ball->getVelocity()->y = 45;
            this->updateBall();
        }
        else {
            ball->getPos()->x -= ball->getVelocity()->x * horizontalMultiplier;
            ball->getPos()->y -= ball->getVelocity()->x;
        }
    }
    else if (ball->getVelocity()->y == 225) {
        if (ball->getPos()->y >= boardSize->y - 1) {
            ball->getPos()->y = boardSize->y - 1;
            ball->getVelocity()->y = 135;
            this->updateBall();
        }
        else if (ball->getPos()->x <= 0) {
            this->resetBall();
            this->getPlayerScores()[1] += 1;
            this->updateBall();
        }
        else if (this->checkBallCollision(false, true)) {
            ball->getVelocity()->y = 315;
            this->updateBall();
        }
        else {
            ball->getPos()->x -= ball->getVelocity()->x * horizontalMultiplier;
            ball->getPos()->y += ball->getVelocity()->x;
        }
    }
}

boolean Board::checkBallCollision(boolean xInc, boolean yInc) const {
    if (xInc && yInc) {
        return this->display[this->ball->getPos()->x + ball->getVelocity()->x + ((this->ball->getPos()->y + ball->getVelocity()->x) * this->boardSize->x)] == '|';
    }
    else if (xInc && !yInc) {
        return this->display[this->ball->getPos()->x + ball->getVelocity()->x + ((this->ball->getPos()->y - ball->getVelocity()->x) * this->boardSize->x)] == '|';
    }
    else if(!xInc && !yInc) {
        return this->display[this->ball->getPos()->x - ball->getVelocity()->x + ((this->ball->getPos()->y - ball->getVelocity()->x) * this->boardSize->x)] == '|';
    }
    else {
        return this->display[this->ball->getPos()->x - ball->getVelocity()->x + ((this->ball->getPos()->y + ball->getVelocity()->x) * this->boardSize->x)] == '|';
    }
}

void Board::updatePaddle(Paddle* paddle) {
    for (int i = paddle->getPosition()->y - paddle->getLength() / 2; i < paddle->getPosition()->y + paddle->getLength() / 2; i++) {
        this->display[paddle->getPosition()->x + (i * this->boardSize->x)] = ' ';
    }
    if (paddle == this->player1) {
        if (GetKeyState(VK_UP) & 0x8000/*Check if high-order bit is set (1 << 15)*/)
        {
            if (paddle->getPosition()->y - paddle->getLength() / 2 >= 1) {
                paddle->getPosition()->y -= 1;
            }
        }
        if (GetKeyState(VK_DOWN) & 0x8000/*Check if high-order bit is set (1 << 15)*/)
        {
            if (paddle->getPosition()->y + paddle->getLength() / 2 <= this->getBoardSize()->y - 1) {
                paddle->getPosition()->y += 1;
            }
        }
    }
    else if (paddle == this->player2) {
        if (GetKeyState('W') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
        {
            if (paddle->getPosition()->y - paddle->getLength() / 2 >= 1) {
                paddle->getPosition()->y -= 1;
            }
        }
        if (GetKeyState('S') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
        {
            if (paddle->getPosition()->y + paddle->getLength() / 2 <= this->getBoardSize()->y - 1) {
                paddle->getPosition()->y += 1;
            }
        }
    }
    for (int i = paddle->getPosition()->y - paddle->getLength() / 2; i < paddle->getPosition()->y + paddle->getLength() / 2; i++) {
        this->display[paddle->getPosition()->x + (i * this->boardSize->x)] = '|';
    }
}

int main() {
    //Initiates the console buffer
    HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
    DWORD bytesWritten = 0;

    auto* board = new Board();

    std::chrono::milliseconds timespan(50);

    //Prints the board
    while (true){
        std::this_thread::sleep_for(timespan);
        board->update();
        WriteConsoleOutputCharacterA(console, (LPCSTR)board->getDisplay(), board->getBoardSize()->x * board->getBoardSize()->y, { 0, 0 }, &bytesWritten);
    }

    return 0;
}