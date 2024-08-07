#include "../inc/fsm.h"

GameInfo_t *getGameInfo() {
    static GameInfo_t gameInfo;
    return &gameInfo;
}

UserAction_t *getUserStatus() {
    static UserAction_t userAction;
    return &userAction;
}

GameInfo_t *updateCurrentState() { return getGameInfo(); }

void userInput(UserAction_t action, bool hold) {
    switch (action) {
        case Start:
            // TODO реализация страрта игры, загрузки с бд данных о прошлом
            // рекорде
            break;
        case Pause:
            printPauseMenu();
            OnPauseGame();
            do {
                // TODO отрисовка ожидания
                int keyPause = GET_USER_INPUT;
                if (IS_Q(keyPause)) {
                    OffPauseGame();
                }
            } while (checkedPause());
            clear();
            printGameMap();
            break;
        case Terminate:
            // TODO реализация выхода в меню
            break;
        case Left:
            MoveFigureLeft();
            MoveFigureDown();
            updateGameScreen();
            printGameMap();
            break;
        case Right:
            MoveFigureRight();
            MoveFigureDown();
            updateGameScreen();
            printGameMap();
            break;
        case Up:
            // TODO переворот фигуры
            RotateFigure();
            updateGameScreen();
            printGameMap();
            break;
        case Down:
            MoveFigureDown();
            updateGameScreen();
            printGameMap();
            if (hold == true) {
                // TODO реализация зажатой клавиши вниз
            }
            break;
        case Action:
            clear();
            createRandomTetromino();
            printGameMap();
            // TODO игра онлайн, не в режиме паузе, не только началась
            break;
    }
}

UserAction_t checkTheKeyPressed(int key) {
    key = convertInput(key);
    // bool hold = false;
    UserAction_t input;
    switch (key) {
        case DOWN_ARROW:
            // hold == true? false : true;
            input = Down;
            break;
        case UP_ARROW:
            input = Up;
            break;
        case LEFT_ARROW:
            input = Left;
            break;
        case RIGHT_ARROW:
            input = Right;
            break;
        case 'Q':
            input = Pause;
            break;
        case 'W':
            // TODO press Q
            break;
        case 'E':
            // TODO press E
            break;
        case 'R':
            // TODO press R
            break;
        case '\n':
            input = Terminate;
            break;
        default:
            input = Action;
            break;
    }
    return input;
}

int convertInput(int key) {
    if (IS_Q(key)) {
        key = 'Q';
    } else if (IS_E(key)) {
        key = 'E';
    } else if (IS_R(key)) {
        key = 'R';
    } else if (IS_W(key)) {
        key = 'W';
    } else if (IS_ENTER(key)) {
        key = '\n';
    }
    return key;
}