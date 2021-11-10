#include <iostream>
#include <queue>
#include <stack>

constexpr int MAX_ROWS = 8;
constexpr int MAX_COLS = 10;


enum Cell {
    FREE = 0,
    TAKEN,
    START,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct Coord {
    
    int row;
    int col;

    bool operator==(const Coord& other) { return row == other.row && col == other.col; }
    bool operator!=(const Coord& other) { return !(*this == other); }
};

constexpr Coord     START_COORD = { 0, 0 };
constexpr Coord     END_COORD = { MAX_ROWS - 1, MAX_COLS - 1 };

bool isValidCoord(Cell map[MAX_ROWS][MAX_COLS], Coord curr) {

    if (curr.row >= 0 && curr.col >= 0 && curr.row < MAX_ROWS && curr.col < MAX_COLS) {
        return map[curr.row][curr.col] == FREE;
    }
    return false;

}

void fillShortestPath(Cell map[MAX_ROWS][MAX_COLS]) {

    std::queue<Coord> wave;
    wave.push(START_COORD);

    map[START_COORD.row][START_COORD.col] = START; 

    while (!wave.empty()) {

        Coord curr = wave.front();
        wave.pop();

        if (curr == END_COORD) {
            return;
        }
        Coord downCoord{ curr.row + 1, curr.col };
        if (isValidCoord(map, downCoord))
        {
            wave.push(downCoord);
            map[curr.row + 1][curr.col] = UP;
        }

        Coord rightCoord{ curr.row, curr.col + 1 };
        if (isValidCoord(map, rightCoord))
        {
            wave.push(rightCoord);
            map[curr.row][curr.col + 1] = LEFT;
        }

        Coord upCoord{ curr.row - 1, curr.col };
        if (isValidCoord(map, upCoord))
        {
            wave.push(upCoord);
            map[curr.row - 1][curr.col] = DOWN;
        }

        Coord leftCoord{ curr.row, curr.col - 1 };
        if (isValidCoord(map, leftCoord))
        {
            wave.push(leftCoord);
            map[curr.row][curr.col - 1] = RIGHT;
        }
    
    }
}

void printPath(Cell map[MAX_ROWS][MAX_COLS])
{
    std::stack<Coord> path;

    Coord   curr = { MAX_ROWS - 1, MAX_COLS - 1 };
    bool    wrongCoord = false;
    while (curr != START_COORD && !wrongCoord)
    {
        path.push(curr);

        switch (map[curr.row][curr.col])
        {
        case UP:
            --curr.row;
            break;
        case DOWN:
            ++curr.row;
            break;
        case LEFT:
            --curr.col;
            break;
        case RIGHT:
            ++curr.col;
            break;
        default:
            wrongCoord = true;
            break;
        }
    }

    path.push(START_COORD);

    while (!path.empty())
    {
        Coord curr = path.top();
        path.pop();
        std::cout << '(' << curr.row << ", " << curr.col << ") ";
    }
}


int main()
{
    Cell map[MAX_ROWS][MAX_COLS] =
    {
        { FREE , TAKEN , FREE , FREE  , FREE , FREE  , FREE , TAKEN , FREE , FREE },
        { FREE , TAKEN , FREE , FREE  , FREE , FREE  , FREE , TAKEN , FREE , FREE },
        { FREE , TAKEN , FREE , TAKEN , FREE , TAKEN , FREE , TAKEN , FREE , FREE },
        { FREE , TAKEN , FREE , TAKEN , FREE , TAKEN , FREE , FREE  , FREE , FREE },
        { FREE , TAKEN , FREE , FREE  , FREE , TAKEN , FREE , FREE  , FREE , FREE },
        { FREE , FREE  , FREE , TAKEN , FREE , TAKEN , FREE , FREE  , FREE , FREE },
        { FREE , TAKEN , FREE , TAKEN , FREE , TAKEN , FREE , FREE  , FREE , FREE },
        { FREE , TAKEN , FREE , FREE  , FREE , TAKEN , FREE , FREE  , FREE , FREE },
    };

    fillShortestPath(map);

    printPath(map);

    return 0;
}
