#include "player.h"

#define SMALL_NUM -1000
#define BIG_NUM 1000
#define PLY 5

#define CORNER_WEIGHT 15
#define SIDE_WEIGHT 5
#define GAME_OVER_MULTIPLIER 2

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

/**
 * Uses a minimax algorithm to assign the maximum min score for the given
 * move on the given board. If the player is black, isBlack = 1, and if the player
 * is white, isBlack = -1. move must be a valid move on the board board.
 */
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

    if (depth <= 0) {
        return calculateScore(move, side);
    }
    if (newBoard->isDone()) {
        return calculateScore(move, side) * GAME_OVER_MULTIPLIER;
    }

    int multiplier = -1;

    //if this leaves the opponent with no moves, but the game isn't over yet
    if (!newBoard->hasMoves(opponentsSide)) {
        if (depth < 2) {
            return calculateScore(move, side) * GAME_OVER_MULTIPLIER;
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

/**
 * Calculates a score for the given move using a heuristic function.
 * The base score is the number of the player's pieces minus the number of
 * the opponent's pieces (after the move). A corner piece gets a bonus,
 * but a piece adjacent to a corner gets a deduction. A side piece (that is
 * not adjacent to a corner) gets a smaller bonus, and a piece adjacent to
 * a side piece (that is not also adjacent to a corner and is not a side piece
 * itself) gets a smaller deduction.
 */
int Player::calculateScore(Move *m, Side side) {
    int x = m->getX();
    int y = m->getY();
    Board *temp = board->copy();
    temp->doMove(m, side);

    int score = temp->countBlack() - temp->countWhite();
    if (side == WHITE) {
        score = -score;
    }

    //corner and edge bonuses & penalties
    if (x == 0 || x == 7) {
        if (y == 0 || y == 7) {
            score += CORNER_WEIGHT;
        } else if (y == 1 || y == 6) {
            score -= CORNER_WEIGHT;
        } else {
            score += SIDE_WEIGHT;
        }
    } else if (x == 1 || x == 6) {
        if (y == 0 || y == 1 || y == 6 || y == 7) {
            score -= CORNER_WEIGHT;
        } else {
            score -= SIDE_WEIGHT;
        }
    } else if (y == 0 || y == 7) {
        score += SIDE_WEIGHT;
    } else if (y == 1 || y == 6) {
        score -= SIDE_WEIGHT;
    }

    return score;
}
