#include "player.h"

#define SMALL_NUM -1000
#define BIG_NUM 1000
#define PLY 5

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    board = new Board();
    this->side = side;
    
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    //process opponent's move
    Side opponentsSide;
    int isBlack;
    if (side == BLACK) {
        opponentsSide = WHITE;
        isBlack = 1;
    } else {
        opponentsSide = BLACK;
        isBlack = -1;
    }

    board->doMove(opponentsMove, opponentsSide);

    //calculate own move
    if (!board->hasMoves(side)) {
        return NULL;
    }

    //uses a minimax algorithm to choose the next move.
    Move *maxMove;
    int maxScore = SMALL_NUM;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {

            Move *m = new Move(x, y);
            if (board->checkMove(m, side)) {
                int score = minimax(board, m, PLY - 1, isBlack);

                if (score > maxScore) {
                    maxMove = m;
                    maxScore = score;
                }
            }
        }
    }

    board->doMove(maxMove, side);
    return maxMove;
}

int Player::minimax(Board *board, Move *move, int depth, int isBlack) {
    Side side;
    Side opponentsSide;
    if (isBlack == 1) {
        side = BLACK;
        opponentsSide = WHITE;
    } else {
        side = WHITE;
        opponentsSide = BLACK;
    }
    Board *newBoard = board->copy();
    newBoard->doMove(move, side);

    if (depth <= 0 || newBoard->isDone()) {
        return (newBoard->countBlack() - newBoard->countWhite()) * isBlack;
    }

    int multiplier = -1;

    //if this leaves the opponent with no moves, but the game isn't over yet
    if (!newBoard->hasMoves(opponentsSide)) {
        if (depth < 2) {
            return (newBoard->countBlack() - newBoard->countWhite()) * isBlack;
        } else {
            isBlack *= -1;
            depth--;
            opponentsSide = side;
            multiplier *= -1;
        }
    }

    int maxScore = SMALL_NUM;

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Move *nextMove = new Move(x, y);
            
            if (newBoard->checkMove(nextMove, opponentsSide)) {
                int score = minimax(newBoard, nextMove, depth - 1, -1 * isBlack);
                if (score > maxScore) {
                    maxScore = score;
                }
            }
        }
    }
    return multiplier * maxScore;
}

