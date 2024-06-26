#include "start_game.h"

#include <sys/select.h>

#include "../controller/reader_with_console.h"
#include "../figures/figuresForGames.h"
#include "../includes/common.h"
#include "../menu/menu_for_game.h"
#include "logic.h"

#define Q_KEY 113
#define NUM_KEYS 6
int validKeys[NUM_KEYS] = {KEY_LEFT, KEY_RIGHT, KEY_UP,
                           KEY_DOWN, '\n',      KEY_BACKSPACE};
void startGame() {
    GameInfo_t *game = getInstance_GameInfo();
    UserAction_t action = Start;
    StatusGame_t *statusGame = getStatus_Game();
    *statusGame = START;
    initGame(game);

    nodelay(stdscr, TRUE);  // Включаю режим немедленного ввода
    InitGameBoard(game->field);
    // printNextMap(game->field, gameWindow);
    // InformationMenu(game, stdscr);
    nextFigureGeneretion(game, stdscr);
    UpdateGameScreen(game, stdscr);

    bool hold = false;
    bool keyHeld = false;
    int buf_ch = -1;
    int heldInptKey =
        -1;  // TODO может добавлю расширение для разных клавиш зажатие

    while (action != Terminate) {
        buf_ch = myDelay(game->delay);
        action = (buf_ch == -1) ? Action : action;
        int ch = getch();
        if (buf_ch != -1) {
            ch = buf_ch;
        }
        if (*statusGame == SPAWN) {
            nextFigureGeneretion(game, stdscr);
            UpdateGameScreen(game, stdscr);
            *statusGame = START;
        }
        if (ch != ERR) {  // Если клавиша нажата
            switch (ch) {
                case KEY_LEFT:
                    action = Left;
                    break;
                case KEY_RIGHT:
                    action = Right;
                    break;
                case KEY_UP:
                    action = Up;
                    break;
                case KEY_DOWN:
                    action = Down;
                    keyHeld = true;
                    heldInptKey = ch;
                    break;
                case '\n':
                    action = Pause;
                    break;
                case KEY_BACKSPACE:
                    action = Terminate;
                    break;
                default:
                    buf_ch = -1;
                    break;
            }
        }
        if (keyHeld) {
            // Обработка удержании клавиши
            switch (heldInptKey) {
                    // Обработка действия при удержании клавиши
                case KEY_DOWN:
                    hold = true;
                    // *game = updateCurrentState();
                    keyHeld = false;
                    break;
                // TODO стрелку вверх надо будет настроить на переворот фигуры
                default:
                    continue;
            }
        } else {
            keyHeld = false;
            hold = false;
            heldInptKey = -1;
            game->delay = 10;
            game->speed = 1;
        }
        if (action == Terminate) {
            break;
        }
        // halfdelay(game->delay);
        userInput(action, hold);
        game->level += 1;
        // InformationMenu(game, stdscr);
        // nextFigureGeneretion(game, gameWindow);
        action = Start;
    }
    clearBoard(game);
    nodelay(stdscr, FALSE);  // Выключаю режим немедленного ввода
    werase(stdscr);
    wrefresh(stdscr);
    cleanupGameInfo(game);
}

GameInfo_t *getInstance_GameInfo() {
    static GameInfo_t game_info;
    return &game_info;
}

StatusGame_t *getStatus_Game() {
    static StatusGame_t status_game;
    return &status_game;
}

UserAction_t *getUserAction() {
    static UserAction_t user_info;
    return &user_info;
}

GameInfo_t updateCurrentState() {
    GameInfo_t *game = getInstance_GameInfo();
    // game->speed = 5;
    // game->delay = 5;

    return *game;
}

void cleanupGameInfo(GameInfo_t *game) {
    // Освобождение ресурсов, связанных с game_info
    my_free(game->field);
    my_free(game->menu);
    my_free(game->next);
}

bool isValidKey(int ch) {
    bool bl = false;
    for (int i = 0; i < NUM_KEYS; ++i) {
        if (ch == validKeys[i]) {
            bl = true;
        }
    }
    return bl;
}

int myDelay(int milliseconds) {
    struct timeval tv;
    fd_set fds;

    tv.tv_sec = milliseconds / 10;
    tv.tv_usec = (milliseconds % 1000) * 1000;

    FD_ZERO(&fds);
    FD_SET(fileno(stdin), &fds);

    int result = 0;
    int ch = ERR;

    while (result <= 0 || !isValidKey(ch)) {
        result = select(fileno(stdin) + 1, &fds, NULL, NULL, &tv);

        if (result > 0 && FD_ISSET(fileno(stdin), &fds)) {
            ch = getch();
        } else {
            ch = -1;
            break;
        }
    }
    return ch;
}

void userInput(UserAction_t action, bool hold) {
    GameInfo_t *game = getInstance_GameInfo();
    // StatusGame_t *statusGame = getStatus_Game();

    switch (action) {
        case Start:
            // Обработка действия "Start"
            // nextFigureGeneretion(game, stdscr);
            UpdateGameScreen(game, stdscr);
            break;
        case Pause:
            // Обработка действия "Pause"
            PauseGame(game);
            break;
        case Left:
            swapFigureLeght(game);
            UpdateGameScreen(game, stdscr);
            // Обработка действия "Left"
            break;
        case Right:
            swapFigureRight(game);
            UpdateGameScreen(game, stdscr);
            // Обработка действия "Right"
            break;
        case Up:
            // Обработка действия "Up"
            break;
        case Down:
            // Обработка действия "Down"
            if (hold) {
                game->speed = 5;
                swapFigureDown(game);
                UpdateGameScreen(game, stdscr);
                // Обработка случая, когда удерживается клавиша
            }
            break;
        case Action:
            swapFigureDown(game);
            // Обработка действия "Action"
            UpdateGameScreen(game, stdscr);
            // nextFigureGeneretion(game, stdscr);
            break;
        default:
            // Обработка неверного действия
            break;
    }
}

void PauseGame(GameInfo_t *game) {
    int ch = -1;
    strcpy(game->status, "Pause");
    printPauseGame(game, stdscr);
    while ((ch = getch()) != '\n') {
    }
    strcpy(game->status, "Game");
}

void UpdateGameScreen(GameInfo_t *game_inf, WINDOW *gameWindow) {
    wclear(gameWindow);
    // Отрисовка игрового поля
    for (int i = 0; i < Y_GAME_BOARD; i++) {
        for (int j = 0; j < X_GAME_BOARD; j++) {
            if (game_inf->field[i][j] == 0) {
                wattron(gameWindow, COLOR_PAIR(1));
                mvwprintw(gameWindow, i, j, " ");
                wattroff(gameWindow, COLOR_BLACK);
            } else {
                wattron(gameWindow, COLOR_PAIR(2));
                mvwprintw(gameWindow, i, j, "#");
                wattroff(gameWindow, COLOR_BLACK);
            }
        }
    }

    // Отрисовка информационного меню
    wattron(gameWindow, COLOR_PAIR(3));
    mvwprintw(gameWindow, 2, X_GAME_BOARD + 5, "Backspace for exit");
    mvwprintw(gameWindow, 4, X_GAME_BOARD + 5, "Enter for pause");
    mvwprintw(gameWindow, 6, X_GAME_BOARD + 5, "Level :%d", game_inf->level);
    mvwprintw(gameWindow, 8, X_GAME_BOARD + 5, "Score :%d", game_inf->score);
    mvwprintw(gameWindow, 10, X_GAME_BOARD + 5, "Speed :%d", game_inf->speed);
    mvwprintw(gameWindow, 12, X_GAME_BOARD + 5, "Record :%d",
              game_inf->high_score);
    mvwprintw(gameWindow, 14, X_GAME_BOARD + 5, "Status game: %s",
              game_inf->status);
    wattroff(gameWindow, COLOR_BLACK);

    // Обновление экрана
    wrefresh(gameWindow);
}

void nextFigureGeneretion(GameInfo_t *game, WINDOW *gameWindow) {
    // int figureNumber = getRandNumberFigures();
    Coordinat_Current_Figure *coordFigure = getCoordinate_GameFigure();
    int figureNumber = 0;
    saveOldMap(game);

    clearBoard(game);

    FigureType type = (FigureType)figureNumber;

    int *figurePointer = getFigure(type);

    int dimesion = (figureNumber == 0) ? 4 : 3;

    // Определяем начальные координаты старта новой фигуры
    coordFigure->dimension = dimesion;

    coordFigure->X = (X_GAME_BOARD / 2);  // Начальная позиция по горизонтали

    coordFigure->Y = (coordFigure->dimension / 2);  // Начальная позиция по вертикали

    for (int i = 0; i < coordFigure->dimension; ++i) {
        for (int j = 0; j < coordFigure->dimension; ++j) {
            int value = *(figurePointer + i * dimesion + j);
            coordFigure->figure[i][j] = value;
        }
    }
    for (int i = 0; i < dimesion; ++i) {
        for (int j = 0; j < dimesion; ++j) {
            int value = *(figurePointer + i * dimesion + j);
            game->field[i + 1][coordFigure->X + j] = value;
        }
    }
    printNextMap(game->field, gameWindow);
}

void clearBoard(GameInfo_t *game) {
    for (int i = 0; i < Y_GAME_BOARD; ++i) {
        for (int j = 0; j < X_GAME_BOARD; ++j) {
            if (i == 0 || i == Y_GAME_BOARD - 1) {
                game->field[i][j] = 1;
            } else if (j == 0 || j == X_GAME_BOARD - 1) {
                game->field[i][j] = 1;
            } else {
                game->field[i][j] = 0;
            }
        }
    }
}

void initGame(GameInfo_t *game) {
    initArray(&(game->field));
    initArray(&(game->menu));
    initArray(&(game->next));
    game->score = 0;
    game->high_score = 0;
    game->level = 1;
    game->speed = 1;
    game->pause = 0;
    game->delay = 10;
    strcpy(game->status, "Game");
}
void my_free(int **array) {
    for (int i = 0; i < Y_SIZE_ARRAY; i++) {
        free(array[i]);
    }
    free(array);
}

void initArray(int ***array) {
    *array = (int **)malloc(Y_SIZE_ARRAY * sizeof(int *));
    for (int i = 0; i < Y_SIZE_ARRAY; i++) {
        (*array)[i] = (int *)malloc(X_SIZE_ARRAY * sizeof(int));
    }
}

// void updateScreen() {

// }

int getRandNumberFigures() {
    int min = 0;
    int max = 6;
    int random_value = rand() % (max - min + 1) + min;
    random_value %= 10;
    return random_value;
}