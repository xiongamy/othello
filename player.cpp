#include "player.h"

#define CORNER_WEIGHT 10
#define SIDE_WEIGHT 5
#define MINIMUM_MAX_VALUE -100

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
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
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
    
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

    //calculate with heuristic for all valid moves
    //choose the move that gives the highest score
    Move *maxMove;
    int maxValue = MINIMUM_MAX_VALUE;
    Board *temp = board->copy();

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            Move *m = new Move(x, y);
            
            if (board->checkMove(m, side)) {
                temp->doMove(m, side);
                int score = temp->count(side) - temp->count(opponentsSide);

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

                if (score > maxValue) {
                    maxMove = m;
                    maxValue = score;
                }
            }
        }
    }

    board->doMove(maxMove, side);
    return maxMove;
}



