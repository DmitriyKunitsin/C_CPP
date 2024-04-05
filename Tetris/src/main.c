#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define X_GAME_BOARD 11
#define Y_GAME_BOARD 21
#define START_RIGHT_MENU_WIDTH 11
#define END_RIGHT_MENU_WIDTH 21
#define Y_SIZE_ARRAY 21
#define X_SIZE_ARRAY 21
#define SIDE_LINE '*'
#define TOP_LINE '*'

typedef enum {  // Конечный автома
    STATE_A,  // Enum для того, что к енаму можно обращаться только к одному
              // объекту
    STATE_B,
    STATE_C
} State;

void inputKey();
void printArrayFieldBound(int array[Y_SIZE_ARRAY][X_SIZE_ARRAY]);
void printFieldBound();
void FillinArrayMap(int array[Y_SIZE_ARRAY][X_SIZE_ARRAY]);
void printArrayWithColors(int array[Y_SIZE_ARRAY][X_SIZE_ARRAY]);

void executeMenuItem(int item);
void printMenu(int selectedItem);
State Transition(State current, char input);

int main() {
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    State currentState = STATE_A;
    char inp[] = {'0', '1', '0', '1', '0'};

    for (size_t i = 0; i < sizeof(inp) / sizeof(inp[0]); i++) {
        currentState = Transition(currentState, inp[i]);
        printf("Input: %c, State: %d\n", inp[i], currentState);
    }
    int arr[Y_SIZE_ARRAY][X_SIZE_ARRAY];
    FillinArrayMap(arr);
    printArrayWithColors(arr);
    // printFieldBound();
    inputKey();
    printArrayFieldBound(arr);

    refresh();
    getch();
    endwin();

    return 0;
}

void printArrayFieldBound(int array[Y_SIZE_ARRAY][X_SIZE_ARRAY]) {
    for (int i = 0; i < Y_SIZE_ARRAY; i++) {
        for (int j = 0; j < X_SIZE_ARRAY; j++) {
            printf("%d", array[i][j]);
        }
        printf("\n");
    }
}

void inputKey() {
    keypad(stdscr, TRUE);
    int key;
    int selectedItem = 0;
    while ((key = getch()) != KEY_BACKSPACE) {
        switch (key) {
            case KEY_UP:
                selectedItem = (selectedItem - 1 > 0) ? selectedItem - 1 : 0;
                break;
            case KEY_DOWN:
                selectedItem =
                    (selectedItem + 1 < 2) ? selectedItem + 1 : 2 - 1;
                break;
            case '\n':
                executeMenuItem(selectedItem);
                break;
            default:
                break;
        }
        printMenu(selectedItem);
    }
}

void printMenu(int selectedItem) {
    int startY = 1;
    int startX = START_RIGHT_MENU_WIDTH + 1;

    mvprintw(startY, startX, "Menu");
    for (int i = 0; i < 2; i++) {
        if (selectedItem == i) {
            attron(A_REVERSE);
        }
        mvprintw(startY + 1 + i, startX, "%d. %s", i + 1,
                 (i == 0) ? "Start Game" : "Exit");
        if (i == selectedItem) {
            attroff(A_REVERSE);
        }
    }
}

void executeMenuItem(int item) {
    switch (item) {
        case 0:
            /* code */
            break;
        case 1:
            endwin();
            exit(0);
            break;
        default:
            break;
    }
}
void printArrayWithColors(int array[Y_SIZE_ARRAY][X_SIZE_ARRAY]) {
    for (int i = 0; i < Y_SIZE_ARRAY; i++) {
        for (int j = 0; j < X_SIZE_ARRAY; j++) {
            if (array[i][j] == 0) {
                attron(COLOR_BLACK);  // цвет вместо нуля
                printw(" ");  // вывод пробела вместо нуля
                attroff(COLOR_BLACK);
            } else {
                attron(COLOR_BLACK);
                printw("#");
                attroff(COLOR_BLACK);
            }
        }
        printw("\n");
    }
}

void printFieldBound() {
    for (int i = 0; i < Y_GAME_BOARD; i++) {
        for (int j = 0; j < X_GAME_BOARD; j++) {
            printf("%c",
                   (i == 0 || i == Y_GAME_BOARD - 1)
                       ? (j != 0 && j != X_GAME_BOARD - 1 ? TOP_LINE : ' ')
                       : ' ');
            printf("%c", (j == 0 || j == X_GAME_BOARD - 1)
                             ? (i != 0 && i != Y_GAME_BOARD - 1
                                    ? SIDE_LINE
                                    : (j == X_GAME_BOARD - 1 &&
                                               (i == 0 || i == Y_GAME_BOARD - 1)
                                           ? SIDE_LINE
                                           : ' '))
                             : ' ');
        }
        printf("\n");
    }
}
void FillinArrayMap(int array[Y_SIZE_ARRAY][X_SIZE_ARRAY]) {
    for (int i = 0; i < Y_SIZE_ARRAY; i++) {
        for (int j = 0; j < X_SIZE_ARRAY; j++) {
            if ((i == 0 || i == Y_GAME_BOARD - 1) && (j < X_GAME_BOARD)) {
                array[i][j] = 1;
            } else if (j == 0 || j == X_GAME_BOARD - 1) {
                array[i][j] = 1;
            } else {
                array[i][j] = 0;
            }
        }
    }
}
State Transition(State current, char input) {
    switch (current) {
        case STATE_A:
            if (input == '0') {
                return STATE_B;
            } else {
                return STATE_A;
            }
            break;
        case STATE_B:
            if (input == '1') {
                return STATE_C;
            } else {
                return STATE_A;
            }
            break;
        case STATE_C:
            if (input == '0') {
                return STATE_B;
            } else {
                return STATE_C;
            }
            break;
    }
    return current;
}
// for(int i = 0; i < X_GAME_BOARD; i++) {
//     for(int j = 0; j < Y_GAME_BOARD; j++) {
//         if (i == 0 || i == X_GAME_BOARD - 1) {
//             if(j != 0 && j != Y_GAME_BOARD - 1) {
//             printf("%c", TOP_LINE);
//             } else {
//                 printf("%s", " ");
//             }
//         } else {
//             printf("%s"," ");
//         }
//         if(j == 0 || j == Y_GAME_BOARD - 2) {
//             if(i != 0 && i != X_GAME_BOARD - 1) {
//                 printf("%c", SIDE_LINE);
//             } else if(j == Y_GAME_BOARD - 2 && (i == 0 || i ==
//             X_GAME_BOARD - 1)) {
//                 printf(" %c", SIDE_LINE);
//             }
//         } else {
//             printf("%s", " ");
//         }
//     }
//     printf("\n");
// }
// }