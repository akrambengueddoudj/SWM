#include <Arduino.h>

// Warehouse node definitions
const int numNodes = 6;
const char nodeNames[numNodes] = {'A', 'B', 'C', 'D', 'E', 'F'};
int D = 0;
int i;

// Adjacency matrix (distances in cm)
float graph[numNodes][numNodes] = {
    {   0, 67.5,   -1,   -1,  165,   -1}, // A
    {67.5,    0,127.5,   50,   -1,   -1}, // B
    {  -1,127.5,    0,   -1,   -1,   90}, // C
    {  -1,   50,   -1,    0, 92.5, 92.5}, // D
    { 165,   -1,   -1, 92.5,    0,   45}, // E
    {  -1,   -1,   90, 92.5,   45,    0}  // F
};

// Stores the path as indices (0-5)
int path[numNodes]; 
int pathLength = 0;
float totalDistance = 0;

const int Rel[numNodes][numNodes] = {
  {-1, 1,-1,-1, 2,-1}, // From A (0)
  {3, -1, 1, 2,-1,-1}, // From B (1)
  {-1,0, -1,-1,-1, 2}, // From C (2)
  {-1,0,-1,-1, 3, 1}, // From D (3)
  {3,-1,-1, 0, -1, 1}, // From E (4)
  {-1,-1,1, 0, 3, -1}  // From F (5)
};
void dijkstra(int start, int target) {
    float distances[numNodes];
    int previous[numNodes];
    bool visited[numNodes] = {false};

    // Initialize
    for (int i = 0; i < numNodes; i++) {
        distances[i] = INFINITY;
        previous[i] = -1;
    }
    distances[start] = 0;

    // Main algorithm
    for (int i = 0; i < numNodes; i++) {
        int current = -1;
        float minDist = INFINITY;
        for (int j = 0; j < numNodes; j++) {
            if (!visited[j] && distances[j] < minDist) {
                minDist = distances[j];
                current = j;
            }
        }

        if (current == -1 || current == target) break;
        visited[current] = true;

        for (int j = 0; j < numNodes; j++) {
            if (graph[current][j] > 0 && !visited[j]) {
                float newDist = distances[current] + graph[current][j];
                if (newDist < distances[j]) {
                    distances[j] = newDist;
                    previous[j] = current;
                }
            }
        }
    }

    // Reconstruct path
    pathLength = 0;
    int node = target;
    while (node != -1 && pathLength < numNodes) {
        path[pathLength++] = node;
        node = previous[node];
    }

    // Reverse to get start->target order
    for (int i = 0; i < pathLength / 2; i++) {
        int temp = path[i];
        path[i] = path[pathLength - 1 - i];
        path[pathLength - 1 - i] = temp;
    }

    totalDistance = distances[target];
}
void decision() {
  // Main control loop
   D = 0;
  
  Serial.println(pathLength);
  for (i = 0;i<pathLength-1; i++) {
    
    // Assuming ReA is a 2D array that needs to be defined
    int r = Rel[path[i]][path[i+1]];

    // Switch statement based on some condition (original was unclear)
    // I'm interpreting this as switching based on D's value
    switch(r-D) {
      case 0: // Forward
        Forward();
        break;
        
      case 1:
      case -3: // Right if condition met
          D = r;
          Right();
          Forward();
        
        break;
        
      case 2:
      case -2: // Set D and move forward
        D = r;
        Uturn();
        Forward();
        break;
        
      case 3:
      case -1: // Assuming this is the "3, -x" case
        D = r;
        Left();
        Forward();
        break;
    }
  }
}

void Forward() {
  // Implement forward movement
  Serial.println("Moving Forward");
  Serial.println(D);
  delay(3000);
}

void Right() {
  // Implement right turn
  Serial.println("Turning Right");
  delay(3000);
}

void Left() {
  // Implement left turn
  Serial.println("Turning Left");
  delay(3000);
}

void Uturn() {
  // Implement U-turn
  Serial.println("Making U-turn");
  delay(3000);
}
void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Warehouse Pathfinder - Vector Output");
}

void loop() {
    if (Serial.available() > 0) {
        char startChar = Serial.read();
        Serial.read(); // Consume newline
        
        Serial.print("Enter target node (A-F): ");
        while (Serial.available() == 0);
        char targetChar = Serial.read();

        // Convert to indices
        int start = startChar - 'A';
        int target = targetChar - 'A';

        if (start < 0 || start >= numNodes || target < 0 || target >= numNodes) {
            Serial.println("Invalid nodes! Use A-F");
            return;
        }

        // Compute path
        dijkstra(start, target);

        // Print raw vector (indices)
        Serial.print("Path vector: [");
        for (int i = 0; i < pathLength; i++) {
            Serial.print(nodeNames[path[i]]);
            if (i < pathLength - 1) Serial.print(", ");
        }
        Serial.println("]");

        // Print node names for reference
        Serial.print("Node sequence: ");
        for (int i = 0; i < pathLength; i++) {
            Serial.print(nodeNames[path[i]]);
            if (i < pathLength - 1) Serial.print(" -> ");
        }
        Serial.print("\nTotal distance: ");
        Serial.print(totalDistance);
        Serial.println(" cm\n");
        decision();
    }
}