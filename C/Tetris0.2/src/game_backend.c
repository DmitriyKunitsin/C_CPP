#include "../inc/game_backend.h"

int validKeys[NUMBER_OF_KEYS] = {LEFT_ARROW, RIGHT_ARROW, UP_ARROW,
                                 DOWN_ARROW, '\n',        KEY_BACKSPACE};
/*Проверка нажатой клавиши на валидность*/
bool isValidKey(int ch) {
    bool bl = false;
    for (int i = 0; i < NUMBER_OF_KEYS; ++i) {
        if (ch == validKeys[i]) {
            bl = true;
        }
    }
    if (IF_INPUT(ch)) {
        bl = true;
    }
    return bl;
}
bool myDelay(int milliseconds, int ch) {
    milliseconds *= 100;
    bool checkValid = false;
    clock_t start_time = clock();
    do {
        checkValid = isValidKey(ch);
    } while (((clock() - start_time) * 1000 / CLOCKS_PER_SEC) < milliseconds &&
             !checkValid);
    return checkValid;
}
void startGame() {
    int key;
    clear();
    UserAction_t input;
    userInput(Start, false);
    do {
        printCurrentFigure();
        printNextFigure();
        key = GET_USER_INPUT;
        if (myDelay(1, key)) {  // нажата валидная клавиша
            input = checkTheKeyPressed(key);
            userInput(input, true);
        } else {  // прошел таймер и валидная клавиша не нажата
            userInput(Down, false);
        }

    } while (input != Terminate);
}

void MoveFigureDown() {
    Current_Figure *figure = getCurrentFigure();
    figure->Y = (figure->Y + 1) > 19 ? figure->Y : (figure->Y + 1);
}
void MoveFigureLeft() {
    Current_Figure *figure = getCurrentFigure();
    figure->X = (checkCollisionLeft() == false) ? (figure->X - 1) : figure->X;
}

void MoveFigureRight() {
    Current_Figure *figure = getCurrentFigure();
    figure->X = (checkCollisionRight() == false) ? (figure->X + 1) : figure->X;
}

void OnPauseGame() {
    GameInfo_t *game = getGameInfo();
    game->pause = 1;
}
void OffPauseGame() {
    GameInfo_t *game = getGameInfo();
    game->pause = 0;
}
bool checkedPause() {
    const GameInfo_t *game = updateCurrentState();
    return game->pause;
}
bool isLineFull(int row) {
    GameInfo_t *game = getGameInfo();
    bool checkLine = true;
    for (int i = 0; i < SIZE_MAX_MAP_X; ++i) {
        if (game->field[row][i] == 0) {
            checkLine = false;
        }
    }
    return checkLine;
}

void removeLine(int row) {
    GameInfo_t *game = getGameInfo();
    for (int i = row; i > 0; i--) {
        for (int j = 0; j < SIZE_MAX_MAP_X; ++j) {
            game->field[i][j] = game->field[i - 1][j];
        }
    }
}

void checkLines() {
    for (int i = SIZE_MAX_MAP_Y - 1; i >= 0; i--) {
        if (isLineFull(i)) {
            removeLine(i);
        }
    }
}

void GenereatedNextFigure() {
    Current_Figure *figure = getCurrentFigure();
    int figureNumber = getRandNumberFigures();
    int *figurePointer = getFigure(figureNumber);
    figure->dimension = 4;
    for (int i = 0; i < figure->dimension; ++i) {
        for (int j = 0; j < figure->dimension; ++j) {
            int value = *(figurePointer + i * figure->dimension + j);
            figure->nextFigure[i][j] = value;
        }
    }
}
void ApperanceFigureToNextField() {
    GameInfo_t *game = getGameInfo();
    Current_Figure *currentGameFigure = getCurrentFigure();
    currentGameFigure->X = ((SIZE_MAX_MAP_X - currentGameFigure->dimension) /
                            2);  // стартовые позиции фигуры
    currentGameFigure->Y = 0;
    for (int i = 0; i < currentGameFigure->dimension; ++i) {
        for (int j = 0; j < currentGameFigure->dimension; ++j) {
            int value = currentGameFigure->curFigure[i][j];
            game->next[currentGameFigure->Y + i][currentGameFigure->X + j] =
                value;
        }
    }
    SwapFigureOldToNew();
    GenereatedNextFigure();
}
void SwapFigureOldToNew() {
    Current_Figure *GameFigure = getCurrentFigure();
    for (int i = 0; i < GameFigure->dimension; ++i) {
        for (int j = 0; j < GameFigure->dimension; ++j) {
            GameFigure->curFigure[i][j] = GameFigure->nextFigure[i][j];
        }
    }
}
int getRandNumberFigures() {
    int min = 0;
    int max = 6;
    int random_value = rand() % (max - min + 1) + min;
    random_value %= 10;
    return random_value;
}

void firstStartGame() {
    GenereatedNextFigure();
    SwapFigureOldToNew();
    ApperanceFigureToNextField();
}

bool checkCollisionRight() {
    const GameInfo_t *game = getGameInfo();
    const Current_Figure *figure = getCurrentFigure();
    bool checkCollission = false;
    for (int y = figure->Y; y < (figure->Y + figure->dimension); ++y) {
        for (int x = figure->X; x < (figure->X + figure->dimension); ++x) {
            if ((game->next[y][x] == 1)) {
                if (game->field[y][x + 1] == 1) {
                    checkCollission = true;
                }
            }
        }
    }
    return checkCollission;
}
bool checkCollisionLeft() {
    const GameInfo_t *game = getGameInfo();
    const Current_Figure *figure = getCurrentFigure();
    bool checkCollission = false;
    for (int y = figure->Y; y < (figure->Y + figure->dimension); ++y) {
        for (int x = figure->X; x < (figure->X + figure->dimension); ++x) {
            if ((game->next[y][x] == 1)) {
                if (game->field[y][x - 1] == 1) {
                    checkCollission = true;
                    mvprintw(1, 55, "TRUE COLLISION");
                }
            }
        }
    }

    return checkCollission;
}