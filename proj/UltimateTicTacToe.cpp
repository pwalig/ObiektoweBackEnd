#include "UltimateTicTacToe.hh"

#include <stdio.h>

/*template <typename T>
void TicTacToeBoard<T>::Fill(const T & value){
    for (int i = 0; i < 3; i++){
        for (int i = 0; i < 3; i++){
            this->board[i][j] = T;
        }
    }
}*/


// TIC TAC TOE BOARD

TTTBoard::TTTBoard(){
    Reset();
}

bool TTTBoard::IsFull(){
    for (int i = 0; i < 9; i++)
        if (fields[i] == EMPTY_MARK) return false;

    return true;
}

void TTTBoard::Reset(){
    for (int i = 0; i < 9; i++)
        fields[i] = EMPTY_MARK;
}

char TTTBoard::GetWinner(){
    // rows
    if (fields[0] == fields[1] && fields[1] == fields[2] && fields[0] != EMPTY_MARK) return fields[0];
    if (fields[3] == fields[4] && fields[4] == fields[5] && fields[3] != EMPTY_MARK) return fields[3];
    if (fields[6] == fields[7] && fields[7] == fields[8] && fields[6] != EMPTY_MARK) return fields[6];

    // collumns
    if (fields[0] == fields[3] && fields[3] == fields[6] && fields[0] != EMPTY_MARK) return fields[0];
    if (fields[1] == fields[4] && fields[4] == fields[7] && fields[1] != EMPTY_MARK) return fields[1];
    if (fields[2] == fields[5] && fields[5] == fields[8] && fields[2] != EMPTY_MARK) return fields[2];

    // diagonals
    if (fields[0] == fields[4] && fields[4] == fields[8] && fields[0] != EMPTY_MARK) return fields[0];
    if (fields[6] == fields[4] && fields[4] == fields[2] && fields[6] != EMPTY_MARK) return fields[6];

    return EMPTY_MARK;
}

void TTTBoard::Mark(const int & _id, const char & value){
    fields[_id] = value;
}



// ULTIMATE TIC TAC TOE BOARD

UTTTBoard::UTTTBoard(){
    Reset();
}

bool UTTTBoard::IsFull(){
    for (int i = 0; i < 9; i++)
        if (!boards[i].IsFull()) return false;

    return true;
}

bool UTTTBoard::Draw(){
    if (GetWinner() == EMPTY_MARK){
        for (int i = 0; i < 9; i++){
            if (!boards[i].IsFull()){
                if (boards[i].GetWinner() == EMPTY_MARK) return false;
            } 
        }
        return true;
    }
    return false;
}

void UTTTBoard::Reset(){
    for (int i = 0; i < 9; i++) {
        boards[i].Reset();
        wins[i] = EMPTY_MARK;
    }
}

char UTTTBoard::GetWinner() {
    // rows
    if (wins[0] == wins[1] && wins[1] == wins[2] && wins[0] != EMPTY_MARK) return wins[0];
    if (wins[3] == wins[4] && wins[4] == wins[5] && wins[3] != EMPTY_MARK) return wins[3];
    if (wins[6] == wins[7] && wins[7] == wins[8] && wins[6] != EMPTY_MARK) return wins[6];

    // collumns
    if (wins[0] == wins[3] && wins[3] == wins[6] && wins[0] != EMPTY_MARK) return wins[0];
    if (wins[1] == wins[4] && wins[4] == wins[7] && wins[1] != EMPTY_MARK) return wins[1];
    if (wins[2] == wins[5] && wins[5] == wins[8] && wins[2] != EMPTY_MARK) return wins[2];

    // diagonals
    if (wins[0] == wins[4] && wins[4] == wins[8] && wins[0] != EMPTY_MARK) return wins[0];
    if (wins[6] == wins[4] && wins[4] == wins[2] && wins[6] != EMPTY_MARK) return wins[6];

    return EMPTY_MARK;
}

void UTTTBoard::Mark(const int & _board, const int & _field, const char & value){
    this->boards[_board].Mark(_field, value);
    wins[_board] = this->boards[_board].GetWinner();
}

TTTBoard UTTTBoard::GetBoardByIndex(const int & _id){
    if (_id < 9 && _id >= 0) return boards[_id];
    return boards[0];
}

void UTTTBoard::Render(){
    printf("Main Board:\n");
    for (int j = 0; j < 9; j++){
        if (j == 3 || j == 6) printf("---+---+---\n");
        for (int i = 0; i < 9; i++){
            if (i == 3 || i == 6) printf("|");
            int _board = i / 3;
            int _field = i % 3;
            if (j >= 3) j >= 6 ? _board += 6 : _board += 3;
            _field += (j % 3) * 3;
            printf("%c", boards[_board].fields[_field]);
        }
        printf("\n");
    }
}

void UTTTBoard::HelperBoard(const int & _curBoard){
    printf("Helper Board:\n");
    for (int i = 0; i < 9; i++){
        if (i == 3 || i == 6) printf("-+-+-\n");
        if (i % 3 != 0) printf("|");
        char symbol = (_curBoard == i && wins[i] == EMPTY_MARK) ? '?' : wins[i];
        if (boards[i].IsFull() && wins[i] == EMPTY_MARK) symbol = '#';
        printf("%c", symbol);
        if (i % 3 == 2) printf("\n");
    }
}





// ULTIMATE TIC TAC TOE

const int UltimateTicTacToe::requiredPlayersCount = 2;
const int UltimateTicTacToe::inputs = 9;
const int UltimateTicTacToe::outputs = 173;

UltimateTicTacToe::UltimateTicTacToe():Game(), curBoard(-1), forceDisplay(false) {
    marks[0] = 'o';
    marks[1] = 'x';
    display[0] = true;
    display[1] = true;
    scores[0] = 0.0;
    scores[1] = 0.0;
}


UltimateTicTacToe::UltimateTicTacToe(const int & _amount, Player** _players):Game(_amount, _players, 9, 82), curBoard(-1), forceDisplay(false){
    marks[0] = 'o';
    marks[1] = 'x';
    if (_amount > 0) display[0] = this->players[0]->requireDisplay;
    if (_amount > 1) display[1] = this->players[1]->requireDisplay;
    scores[0] = 0.0;
    scores[1] = 0.0;
}

Game* UltimateTicTacToe::returnCopy(){
    UltimateTicTacToe* uttt = new UltimateTicTacToe(*this);
    return uttt;
}

void UltimateTicTacToe::SetPlayers(const int & _amount, Player** _players){
    this->Game::SetPlayers(_amount, _players);
    if (_amount > 0) display[0] = this->players[0]->requireDisplay;
    if (_amount > 1) display[1] = this->players[1]->requireDisplay;
    scores[0] = 0.0;
    scores[1] = 0.0;
}


void UltimateTicTacToe::SetForeDisplay(const bool & _disp){
    this->forceDisplay = _disp;
}
void UltimateTicTacToe::SetScoreParameters(const float & _wrongMovePenalty, const float & _giveBoardChoicePenalty, const float & _wonBoardReward, const float & _lostBoardPenalty, const float & _wonGameReward, const float & _lostGamePenalty){
    this->wrongMovePenalty = _wrongMovePenalty;
    this->giveBoardChoicePenalty = _giveBoardChoicePenalty;
    this->wonBoardReward = _wonBoardReward;
    this->lostBoardPenalty = _lostBoardPenalty;
    this->wonGameReward = _wonGameReward;
    this->lostGamePenalty = _lostGamePenalty;
}

float* UltimateTicTacToe::GetCompressedBoardState(const int & _playerId){
    float* out = new float[91];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++){
            //opponent mark
            out[(i*9)+j] = 0.5; // oponent mark presence
            if (board.boards[i].fields[j] == EMPTY_MARK) out[(i*9)+j] = 0.0; // empty mark presence
            if (board.boards[i].fields[j] == marks[_playerId]) out[(i*9)+j] = 1.0; // player mark presence
        }
    }
    for (int i = 0; i < 9; i++){
        out[81+i] = ((curBoard == i) ? 1.0 : 0.0);
    }
    out[90] = ((curBoard == -1) ? 1.0 : 0.0); // last variable informs weather the move is for choosing the board or field
    return out;
}

float* UltimateTicTacToe::GetBoardState(const int & _playerId){
    float* out = new float[UltimateTicTacToe::outputs];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++){
            //opponent mark
            out[2*((i*9)+j) + 1] = -1.0; // oponent mark presence
            if (board.boards[i].fields[j] == EMPTY_MARK) out[2*((i*9)+j) + 1] = 0.0; // lack of oponent mark

            // player mark
            out[2*((i*9)+j)] = 0.0; // lack of player mark
            if (board.boards[i].fields[j] == marks[_playerId]) out[2*((i*9)+j)] = 1.0; // player mark presence
        }
    }
    for (int i = 0; i < 9; i++){
        out[162+i] = ((curBoard == i) ? 1.0 : 0.0);
    }
    out[171] = ((curBoard == -1) ? -1.0 : 0.0); // last variable informs weather the move is for choosing the board or field
    out[172] = ((_playerId == 1) ? -1.0 : 0.0); // last variable informs weather player is o or x
    return out;
}

// get the amount of different actions you can take in the Game
int UltimateTicTacToe::GetInputs() {
    return UltimateTicTacToe::inputs;
}
// get the amount variables on which game state can be written
int UltimateTicTacToe::GetOutputs() {
    return UltimateTicTacToe::outputs;
}

int UltimateTicTacToe::GetRequiredPlayersCount(){
    return UltimateTicTacToe::requiredPlayersCount;
}

// get the amount of different actions you can take in the Game
int UltimateTicTacToe::GetInstanceInputs(){
    return UltimateTicTacToe::inputs;
}
// get the amount of variables on which game state can be written
int UltimateTicTacToe::GetInstanceOutputs(){
    return UltimateTicTacToe::outputs;
}

int UltimateTicTacToe::GetInstanceRequiredPlayersCount(){
    return UltimateTicTacToe::requiredPlayersCount;
}


void UltimateTicTacToe::Play() {
    int choice;
    int curPlayer = 0;
    curBoard = -1;
    board.Reset();
    while(true) {
        if (display[curPlayer]) printf("\n-----%c turn-----\n", marks[curPlayer]);
        if (display[curPlayer] || forceDisplay) {board.Render(); board.HelperBoard(curBoard);} //render board

        // board choice
        if (curBoard < 0) {
            int requestNo = 0;
            do {
                if (display[curPlayer]) printf("Choose board: ");
                float* boardState = GetBoardState(curPlayer);
                choice = this->players[curPlayer]->GetDecision(UltimateTicTacToe::outputs, boardState, requestNo);
                delete [] boardState;
                requestNo++;
            } while (choice >= 9 || choice < 0 || this->board.GetBoardByIndex(choice).IsFull() || board.wins[choice] != EMPTY_MARK); // if chosen board is either won or full choose again
            curBoard = choice;
            scores[curPlayer] -= (requestNo - 1) * this->wrongMovePenalty;
        }

        int requestNo = 0;
        // field choice
        do {
            if (display[curPlayer]) printf("Choose field on board %d: ", curBoard);
            float* boardState = GetBoardState(curPlayer);
            choice = this->players[curPlayer]->GetDecision(UltimateTicTacToe::outputs, boardState, requestNo);
            delete [] boardState;
            requestNo++;
        } while (choice >= 9 || choice < 0 || this->board.GetBoardByIndex(curBoard).fields[choice] != EMPTY_MARK); // if chosen field is occupied choose again
        scores[curPlayer] -= (requestNo - 1) * this->wrongMovePenalty;

        this->board.Mark(curBoard, choice, marks[curPlayer]); // set mark
        if (this->board.GetBoardByIndex(curBoard).GetWinner() == marks[curPlayer]) { // reward player of winning a singular board
            scores[curPlayer] += wonBoardReward; 
            curPlayer == 0 ? scores[1] -= this->lostBoardPenalty : scores[0] -= this->lostBoardPenalty;
        }

        if (board.GetWinner() == marks[curPlayer]) { // check if that was a winning move
            scores[curPlayer] += this->wonGameReward;
            curPlayer == 0 ? scores[1] -= this->lostGamePenalty : scores[0] -= this->lostGamePenalty;
            break;
        }
        if (board.IsFull()) break;  // check if we filled the board
        if (board.Draw()) break; // check if draw occured

        curBoard = choice; // where we play next
        if (this->board.GetBoardByIndex(curBoard).IsFull() || board.wins[curBoard] != EMPTY_MARK){ // if new current board is filled or won already then allow next player to choose
            curBoard = -1;
            curPlayer == 0 ? scores[1] -= this->giveBoardChoicePenalty : scores[0] -= this->giveBoardChoicePenalty;
        }

        //next player, next turn
        if (curPlayer == 1) curPlayer = 0;
        else curPlayer = 1;
    }
    if (display[0] || display[1] || forceDisplay) {board.Render(); board.HelperBoard(curBoard);}
}


int UltimateTicTacToe::GetWinner(){
    if (board.GetWinner() == marks[0]) return 0;
    if (board.GetWinner() == marks[1]) return 1;
    else return -1;
}
float UltimateTicTacToe::GetScore(const int & _playerId){
    return scores[_playerId];
}