#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "maze.h"
#include "search.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our main() during grading

int main()
{
    if (runSimpleTests(NO_TESTS)) {
        return 0;
    }

    //Grid<bool> maze;
    //readMazeFile("res/21x23.maze", maze);
    //solveMaze(maze);


    searchEngine("res/website.txt");

    cout << endl << "All done!" << endl;
    return 0;
}


// Do not remove or edit this test. It is here to to confirm that your code
// conforms to the expected function prototypes needed for grading
PROVIDED_TEST("Confirm function prototypes") {
    bool execute = false;
    if (execute) {
        Grid<bool> g;
        GridLocation loc;
        Set<GridLocation> set;
        Stack<GridLocation> path;
        string str;

        set = generateValidMoves(g, loc);
        checkSolution(g, path);
        readMazeFile(str, g);
        readSolutionFile(str, path);
        path = solveMaze(g);

        Set<string> strset;
        Map<std::string, Set<std::string>> map;
        str = cleanToken(str);
        strset = gatherTokens(str);
        buildIndex(str, map);
        strset = findQueryMatches(map, str);
        searchEngine(str);
    }
}
