/*
  The starting point of the project.
  The program would take input of a 2-d map from terminal input.
  find a polarium solution, and output as 2d-map.
 */
#include <iostream>
#include <vector>
#include <stack>
using namespace std;
enum directions {
  INIT,
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class polariumNode {
public:
  int connection; // numbers of nodes connecting to this node
  directions currentStep; // the decision taken this step;
  bool visited;
  char nodeStatus;
  int coordX, coordY;
public:
  polariumNode() {
    connection = 0;
    currentStep = INIT;
    visited = false;
    nodeStatus = 'x';
    coordX = 0;
    coordY = 0;
  };

};

int main() {
  vector<vector<polariumNode> > polarMap;
  int dimensionX, dimensionY;

  // get input from console
  cout << "dimensionX  dimensionY" << endl;
  cin >> dimensionX >> dimensionY;
  cout << "input map:" << endl;
  for(int y = 0; y < dimensionY; y++) {
    polarMap.push_back(vector<polariumNode>(dimensionX, polariumNode()));
    for(int x = 0; x < dimensionX; x++) {
      cin >> polarMap[y][x].nodeStatus;
      polarMap[y][x].coordX = x;
      polarMap[y][x].coordY = y;
    }
  }

  for(int y = 0; y < dimensionY; y++) {
    for(int x = 0; x < dimensionX; x++) {
      cout << polarMap[y][x].nodeStatus << ' ';
    }
    cout << endl;
  }


  // setup polarity map
  /*
    map format:
    o: accessable blocks
    x: inaccessable blocks
    s: strarting point of current polarium
    */

  // todo: apply branch cutting scheme in DFS
  int totalPathLength = 0;
  int singularties = 0;
  polariumNode startingPoint;
  for(int y = 0; y < dimensionY; y++) {
    for(int x = 0; x < dimensionX; x++) {
      if(polarMap[y][x].nodeStatus != 'x') {
        totalPathLength++;
        if(y > 0) { // check connectivity upwards
          if(polarMap[y - 1][x].nodeStatus != 'x') {
            polarMap[y][x].connection++;
          }
        }
        if(y < dimensionY - 1) { // check connectivity downwards
          if(polarMap[y + 1][x].nodeStatus != 'x') {
            polarMap[y][x].connection++;
          }
        }
        if(x > 0) { // check connectivity leftwards
          if(polarMap[y][x - 1].nodeStatus != 'x') {
            polarMap[y][x].connection++;
          }
        }
        if(x < dimensionX - 1) { // check connectivity downwards
          if(polarMap[y][x + 1].nodeStatus != 'x') {
            polarMap[y][x].connection++;
          }
        }
        if(polarMap[y][x].nodeStatus == 's') {
          startingPoint = polarMap[y][x];
          polarMap[y][x].visited = true;
        }
      }
    }
  }
  cout << "setup complete, " << totalPathLength << " steps found"<< endl;

  // applying dfs
  stack<polariumNode> DFSStack;
  DFSStack.push(startingPoint);
  cout << "starting point: " << startingPoint.coordX << ',' <<startingPoint.coordY << endl;
  totalPathLength--;
  while(DFSStack.size()) {
    cout << "step: " << totalPathLength << ' ';
    if(totalPathLength <= 0) break;
    int topX = DFSStack.top().coordX;
    int topY = DFSStack.top().coordY;

    cout << endl << "current node: " << topX << ',' << topY;
    switch(polarMap[topY][topX].currentStep){ // Branch current step, plan for next step;
      case INIT: // look upwards for possible next step
        polarMap[topY][topX].currentStep = UP;
        if(topY > 0 && polarMap[topY - 1][topX].visited == false \
          && polarMap[topY - 1][topX].nodeStatus != 'x') {
          cout << " going UP ";
          totalPathLength--;
          DFSStack.push(polarMap[topY - 1][topX]);
          polarMap[topY - 1][topX].visited = true;
        }
        break;
      case UP: // look downwards
        polarMap[topY][topX].currentStep = DOWN;
        if(topY < dimensionY - 1 && polarMap[topY + 1][topX].visited == false\
          && polarMap[topY + 1][topX].nodeStatus != 'x') {
          cout << " going DOWN ";
          totalPathLength--;
          DFSStack.push(polarMap[topY + 1][topX]);
          polarMap[topY + 1][topX].visited = true;
        }
        break;
      case DOWN: // look leftwards
        polarMap[topY][topX].currentStep = LEFT;
        if(topX > 0 && polarMap[topY][topX - 1].visited == false\
          && polarMap[topY][topX - 1].nodeStatus != 'x') {
          cout << " going LEFT ";
          totalPathLength--;
          DFSStack.push(polarMap[topY][topX - 1]);
          polarMap[topY][topX - 1].visited = true;
        }
        break;
      case LEFT: // look rightwards
        polarMap[topY][topX].currentStep = RIGHT;
        if(topX < dimensionX - 1 && polarMap[topY][topX + 1].visited == false\
          && polarMap[topY][topX + 1].nodeStatus != 'x') {
          cout << " going RIGHT ";
          totalPathLength--;
          DFSStack.push(polarMap[topY][topX + 1]);
          polarMap[topY][topX + 1].visited = true;
        }
        break;
      case RIGHT: // all possibillities tried, return
        cout << " retreating for one step" << endl;
        polarMap[topY][topX].currentStep = INIT;
        polarMap[topY][topX].visited = false;
        totalPathLength++;
        DFSStack.pop();
        break;
    }
  }
  cout << endl;

  if(totalPathLength != 0) {
    printf("cannot find any solution.\n");
    return 0;
  }

  printf("Solution Found!\n");
  for(int y = 0; y < dimensionY; y++) {
    for(int x = 0; x < dimensionX; x++) {
      char print;
      if(polarMap[y][x].nodeStatus == 's') {
        print = 's';
      }
      else if(polarMap[y][x].nodeStatus == 'x') {
        print = ' ';
      }
      else {
        switch(polarMap[y][x].currentStep) {
          case UP:
            print = 'u';
            break;
          case DOWN:
            print = 'd';
            break;
          case LEFT:
            print = 'l';
            break;
          case RIGHT:
            print = 'r';
            break;
          default:
            print = 'o';
            break;
        }
      }
     cout << print << ' ';
    }
    cout << endl;
  }
 return 0;
}
