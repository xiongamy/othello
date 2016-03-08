#include "player.h"

#define MAXIMUM_MIN_VALUE 100
#define MINIMUM_MAX_VALUE -100

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
    if (side == BLACK) {
        opponentsSide = WHITE;
    } else {
        opponentsSide = BLACK;
    }

    board->doMove(opponentsMove, opponentsSide);

    //calculate own move
    if (!board->hasMoves(side)) {
        return NULL;
    }

    //uses a 2-ply minimax algorithm to choose the next move.
    Move *maxMinMove;
    int maxMinScore = MINIMUM_MAX_VALUE;

    for (int x1 = 0; x1 < 8; x1++) {
        for (int y1 = 0; y1 < 8; y1++) {

            Move *m1 = new Move(x1, y1);
            
            if (board->checkMove(m1, side)) {
                Board *temp1 = board->copy();
                temp1->doMove(m1, side);
                int minScore = MAXIMUM_MIN_VALUE;

                if (!temp1->hasMoves(opponentsSide)) {
                    minScore = temp1->count(side) - temp1->count(opponentsSide);
                } else {
                    for (int x2 = 0; x2 < 8; x2++) {
                        for (int y2 = 0; y2 < 8; y2++) {

                            Move *m2 = new Move(x2, y2);
                            
                            if (temp1->checkMove(m2, opponentsSide)) {
                                Board *temp2 = temp1->copy();
                                temp2->doMove(m2, opponentsSide);
                                int score = temp2->count(side) - temp2->count(opponentsSide);
                                if (score < minScore) {
                                    minScore = score;
                                }
                            }
                        }

                    }
                }

                if (minScore > maxMinScore) {
                    maxMinMove = m1;
                    maxMinScore = minScore;
                }
            }

        }
    }

    board->doMove(maxMinMove, side);
    return maxMinMove;
}



