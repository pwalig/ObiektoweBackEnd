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
    return boards[_id];
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

const int UltimateTicTacToe::inputs = 9;
const int UltimateTicTacToe::outputs = 91;

UltimateTicTacToe::UltimateTicTacToe():Game(), curBoard(-1) {
    marks[0] = 'o';
    marks[1] = 'x';
    display[0] = true;
    display[1] = true;
}


UltimateTicTacToe::UltimateTicTacToe(const int & _amount, Player** _players):Game(_amount, _players, 9, 82), curBoard(-1){
    marks[0] = 'o';
    marks[1] = 'x';
    if (_amount > 0) display[0] = this->players[0]->requireDisplay;
    if (_amount > 1) display[1] = this->players[1]->requireDisplay;
}

void UltimateTicTacToe::SetPlayers(const int & _amount, Player** _players){
    this->Game::SetPlayers(_amount, _players);
    if (_amount > 0) display[0] = this->players[0]->requireDisplay;
    if (_amount > 1) display[1] = this->players[1]->requireDisplay;
}

float* UltimateTicTacToe::GetBoardState(){
    float* out = new float[UltimateTicTacToe::outputs];
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            out[i*9+j] = board.boards[i].fields[j];
        }
    }
    for (int i = 0; i < 9; i++){
        out[81+i] = ((curBoard == i) ? 1.0 : 0.0);
    }
    out[90] = ((curBoard == -1) ? 1.0 : 0.0); // last variable informs weather the move is for choosing the board or field
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

// get the amount of different actions you can take in the Game
int UltimateTicTacToe::GetInstanceInputs(){
    return UltimateTicTacToe::inputs;
}
// get the amount of variables on which game state can be written
int UltimateTicTacToe::GetInstanceOutputs(){
    return UltimateTicTacToe::outputs;
}


void UltimateTicTacToe::Play() {
    int choice;
    int curPlayer = 0;
    board.Reset();
    while(true) {
        if (display[curPlayer]) printf("\n-----%c turn-----\n", marks[curPlayer]);
        if (display[curPlayer]) {board.Render(); board.HelperBoard(curBoard);} //render board

        // board choice
        if (curBoard < 0) {
            int requestNo = 0;
            do {
                if (display[curPlayer]) printf("Choose board: ");
                float* boardState = GetBoardState();
                choice = this->players[curPlayer]->GetDecision(UltimateTicTacToe::outputs, boardState, requestNo);
                delete [] boardState;
                requestNo++;
            } while (this->board.GetBoardByIndex(choice).IsFull() || board.wins[choice] != EMPTY_MARK);
            curBoard = choice;
        }

        int requestNo = 0;
        // field choice
        do {
            if (display[curPlayer]) printf("Choose field on board %d: ", curBoard);
            float* boardState = GetBoardState();
            choice = this->players[curPlayer]->GetDecision(UltimateTicTacToe::outputs, boardState, requestNo);
            delete [] boardState;
            requestNo++;
        } while (this->board.GetBoardByIndex(curBoard).fields[choice] != EMPTY_MARK);

        this->board.Mark(curBoard, choice, marks[curPlayer]); // set mark

        if (board.GetWinner() == marks[curPlayer]) break; // check if that was a winning move
        if (board.IsFull()) break;  // check if we filled the board

        curBoard = choice; // where we play next
        if (this->board.GetBoardByIndex(curBoard).IsFull() || board.wins[curBoard] != EMPTY_MARK) curBoard = -1; // if new current board is filled or won already then allow next player to choose

        //next player, next turn
        if (curPlayer == 1) curPlayer = 0;
        else curPlayer = 1;
    }
    if (display[0] || display[1]) {board.Render(); board.HelperBoard(curBoard);}
}


// TESTS

void BoardTest(){
    UTTTBoard b;
    b.Mark(5, 4, 'o');
    b.Mark(4, 7, 'x');
    b.Mark(7, 3, 'o');
    b.Mark(3, 5, 'x');
    b.Mark(5, 5, 'o');
    b.Mark(5, 3, 'x');
    b.Render();
}

#include "AiModel.hh"

void UltimateTicTacToeTest(){
    int in = UltimateTicTacToe::GetInputs();
    int out = UltimateTicTacToe::GetOutputs();

    HumanPlayer player1;

    int layerSizes[4] = {out, 16, 16, in};
    AiModel player2(4, layerSizes);
    player2.GetNeuralNetwork()->RandomizeNetwork();
    player2.GetNeuralNetwork()->PrintInfo(true, true, false);

    Player* playerList[2] = {&player1, &player2};
    UltimateTicTacToe ttt(2, playerList);
    ttt.Play();
}