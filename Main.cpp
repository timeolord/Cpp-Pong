#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <string>
#include "Main.h"

const int WIDTH = 200;
const int HEIGHT = 40;
const int PADDLELENGTH = HEIGHT/5;
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
    void moveUp();
    void moveDown();
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
    void update();
    void updateBall() const;
    void updatePaddle(Paddle* paddle);
    void clearPaddle(Paddle* paddle);
    void displayPaddle(Paddle* paddle);
    boolean checkBallCollision(boolean xInc, boolean yInc) const;
    void resetBall() const;
    void clear() const;
    void displayText(std::string string);
};

void Board::displayText(std::string string) {
    int length = string.length();
    //Clear screen
    this->clear();

    for (int i = 0; i < length; i++) {
        this->display[(this->boardSize->x / 2) - ( (length/2) - i) + (this->boardSize->y / 2 * this->boardSize->x)] = string[i];
    }
}

void Board::clear() const {
    for (int col = 0; col < this->boardSize->x; col++) {
        for (int row = 0; row < this->boardSize->y; row++) {
            if (col == this->boardSize->x - 1) {
                this->display[col + (row * this->boardSize->x)] = 0;
            }
            this->display[col + (row * this->boardSize->x)] = ' ';
        }
    }
}

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
            ball->getVelocity()->y = 225;
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
            ball->getVelocity()->y = 135;
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
            ball->getVelocity()->y = 315;
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
            ball->getVelocity()->y = 45;
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

void Paddle::moveUp()
{
    this->getPosition()->y -= 1;
}

void Paddle::moveDown()
{
    this->getPosition()->y += 1;
}

int isKeyPressed(int key)
{
    return GetKeyState(key) & 0x8000;
}

void Board::updatePaddle(Paddle* paddle) {
    clearPaddle(paddle);
    if (paddle == this->player1) {
        if (isKeyPressed(VK_UP))
        {
            if (paddle->getPosition()->y - paddle->getLength() / 2 >= 1) {
                paddle->moveUp();
            }
        }
        if (isKeyPressed(VK_DOWN))
        {
            if (paddle->getPosition()->y + paddle->getLength() / 2 <= this->getBoardSize()->y - 1) {
                paddle->moveDown();
            }
        }
    }
    else if (paddle == this->player2) {
        if (isKeyPressed('W'))
        {
            if (paddle->getPosition()->y - paddle->getLength() / 2 >= 1) {
                paddle->moveUp();
            }
        }
        if (isKeyPressed('S'))
        {
            if (paddle->getPosition()->y + paddle->getLength() / 2 <= this->getBoardSize()->y - 1) {
                paddle->moveDown();
            }
        }
    }
    displayPaddle(paddle);
}

void Board::clearPaddle(Paddle* paddle)
{
    for (int i = paddle->getPosition()->y - paddle->getLength() / 2; i < paddle->getPosition()->y + paddle->getLength() / 2; i++) {
        this->display[paddle->getPosition()->x + (i * this->boardSize->x)] = ' ';
    }
}

void Board::displayPaddle(Paddle* paddle)
{
    for (int i = paddle->getPosition()->y - paddle->getLength() / 2; i < paddle->getPosition()->y + paddle->getLength() / 2; i++) {
        this->display[paddle->getPosition()->x + (i * this->boardSize->x)] = '|';
    }
}

void setWindowSize(int width, int height) 
{ 
    _COORD coord; 
    coord.X = width; 
    coord.Y = height; 

    _SMALL_RECT Rect; 
    Rect.Top = 0; 
    Rect.Left = 0; 
    Rect.Bottom = height - 1; 
    Rect.Right = width - 1; 

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
} 

void displayBoard(Board* board, std::chrono::milliseconds *timespan, HANDLE handle){
    DWORD bytesWritten = 0;
    std::this_thread::sleep_for(*timespan);
    WriteConsoleOutputCharacterA(handle, (LPCSTR)board->getDisplay(), board->getBoardSize()->x * board->getBoardSize()->y, { 0, 0 }, &bytesWritten);
}

int main() {

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);  

    auto* board = new Board();
    int time = 40;
    std::chrono::milliseconds timespan(time);

    setWindowSize(WIDTH, HEIGHT);

    //Prints the board
    while (true){
        if (board->getPlayerScores()[0] > 9) {
            board->displayText("Player 1 Wins!");
            displayBoard(board, &timespan, handle);
            std::cin.get();
            break;
        }
        else if (board->getPlayerScores()[1] > 9) {
            board->displayText("Player 2 Wins!");
            displayBoard(board, &timespan, handle);
            std::cin.get();
            break;
        }
        //Gets the score before update
        int player2Score = board->getPlayerScores()[0];
        int player1Score = board->getPlayerScores()[1];

        setWindowSize(WIDTH, HEIGHT);
        board->update();

        //If score changes increases the speed of the game
        if (board->getPlayerScores()[0] != player2Score || board->getPlayerScores()[1] != player1Score) {
            int delta = time * 0.15;
            time -= delta;
            std::chrono::milliseconds deltaTime(time);
            timespan = deltaTime;
        }

        displayBoard(board, &timespan, handle);
    }

}
