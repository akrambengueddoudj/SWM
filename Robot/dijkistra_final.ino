#include <Arduino.h>
  int leftSensor ;
  int rightSensorExtreme; 
  int leftSensorExtreme ;
  int rightSensor  ;

const int LEFT_SPEED  = 5;   // PWM pin for left motor speed
const int LEFT_FWD    = 4;   // Left motor forward
const int LEFT_REV    = 2;   // Left motor reverse
 int correctoncoef = 120;
// Right Motor
const int RIGHT_SPEED = 6;   // PWM pin for right motor speed
const int RIGHT_REV   = 7;   // Right motor reverse
const int RIGHT_FWD   = 3;   // Right motor forward

// ----- IR Sensor Pin Definitions -----
// Sensors assume a digital output: 0 indicates the sensor detects the line.
const int SENSOR_RIGHT_Extreme = A5;
const int SENSOR_RIGHT = A4;


const int SENSOR_LEFT  = A3;
const int SENSOR_LEFT_extreme  = A2;


// Warehouse node definitions
const int numNodes = 11;
const char nodeNames[numNodes] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
int D = 0;
int i;

// Adjacency matrix (distances in cm)
float graph[numNodes][numNodes] = {
    {   0,  67.5,    -1,    -1,   -1,   -1,   -1,  120,   -1,   -1,   -1}, // A
    {67.5,     0,  52.5,    -1,   50,   -1,   -1,   -1,   -1,   -1,   -1}, // B
    {  -1,  52.5,     0,    -1,   -1,   -1,   60,   -1,   -1,   -1,   -1}, // C
    {  -1,    -1,    -1,     0, 22.5,   -1,   -1,   -1,   70,   -1,   -1}, // D
    {  -1,    50,    -1,  22.5,    0, 22.5,   -1,   -1,   -1,   -1,   -1}, // E
    {  -1,    -1,    -1,    -1, 22.5,    0,   -1,   -1,   -1,   70,   -1},  // F
    {  -1,    -1,    60,    -1,   -1,   -1,    0,   -1,   -1,   -1,   60}, // G
    { 120,    -1,    -1,    -1,   -1,   -1,   -1,    0,   45,   -1,   -1}, // H
    {  -1,    -1,    -1,    70,   -1,   -1,   -1,   45,    0,   45,   -1}, // I
    {  -1,    -1,    -1,    -1,   -1,   70,   -1,   -1,   45,    0,   30}, // J
    {  -1,    -1,    -1,    -1,   -1,   -1,   60,   -1,   -1,   30,    0}  // K
};

// Stores the path as indices (0-5)
int path[numNodes]; 
int pathLength = 0;
float totalDistance = 0;

const int Rel[numNodes][numNodes] = {
    {  -1,     1,    -1,    -1,   -1,   -1,   -1,    2,   -1,   -1,   -1}, // A
    {   3,    -1,     1,    -1,    2,   -1,   -1,   -1,   -1,   -1,   -1}, // B
    {  -1,     3,    -1,    -1,   -1,   -1,    2,   -1,   -1,   -1,   -1}, // C
    {  -1,    -1,    -1,    -1,    1,   -1,   -1,   -1,    2,   -1,   -1}, // D
    {  -1,     0,    -1,     3,   -1,    1,   -1,   -1,   -1,   -1,   -1}, // E
    {  -1,    -1,    -1,    -1,    3,   -1,   -1,   -1,   -1,    2,   -1},  // F
    {  -1,    -1,     0,    -1,   -1,   -1,   -1,   -1,   -1,   -1,    2}, // G
    {   0,    -1,    -1,    -1,   -1,   -1,   -1,   -1,    1,   -1,   -1}, // H
    {  -1,    -1,    -1,     0,   -1,   -1,   -1,    3,   -1,    1,   -1}, // I
    {  -1,    -1,    -1,    -1,   -1,    0,   -1,   -1,    3,   -1,    1}, // J
    {  -1,    -1,    -1,    -1,   -1,   -1,    0,   -1,   -1,    3,   -1}  // K
};

void dijkstra(int start, int target) {
  float distances[numNodes];
  int previous[numNodes];
  bool visited[numNodes] = {false};

  // Initialize distances and previous arrays
  for (int i = 0; i < numNodes; i++) {
    distances[i] = INFINITY;
    previous[i] = -1;
  }
  distances[start] = 0;

  // Main Dijkstra algorithm loop
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
  
  // Reconstruct the path (store in reverse order)
  pathLength = 0;
  int node = target;
  while (node != -1 && pathLength < numNodes) {
    path[pathLength++] = node;
    node = previous[node];
  }
  
  // Reverse the path array to get start -> target order
  for (int i = 0; i < pathLength / 2; i++) {
    int temp = path[i];
    path[i] = path[pathLength - 1 - i];
    path[pathLength - 1 - i] = temp;
  }
  
  totalDistance = distances[target];
}

void decision() {
  // Main control loop that executes the motion functions based on relative direction
  D = 1;
  for (i = 0; i < pathLength - 1; i++) {
    // Get the relative direction between consecutive nodes
    int r = Rel[path[i]][path[i + 1]];
    Serial.println(r);
    // Decide action based on the difference r - D
    switch (r - D) {
      case 0: // Forward
        Forward();
        break;
        
      case 1:
      case -3: // Right turn then move forward
        D = r;
        Right();
        Forward();
        break;
        
      case 2:
      case -2: // U-turn then move forward
        D = r;
        Uturn();
        Forward();
        break;
        
      case 3:
      case -1: // Left turn then move forward
        D = r;
        Left();
        Forward();
        break;
    }
  }
  // Print success message after completing the path
  Serial.println("Path achieved successfully");
}

void Forward() {
  // Implement forward movement
 goForward();
 Serial.println("forward");
}

void Right() {
  // Implement right turn
 turnRight();
 Serial.println("Right");
}

void Left() {
  // Implement left turn
 turnLeft();
 Serial.println("left");
}

void Uturn() {
  // Implement U-turn
 UturnRot();
 Serial.println("u");
}
void setup() {
  Serial.begin(9600);
 
    // Read start node from Serial Monitor
    char startChar = 'A';
   
    
    
    
    char targetChar = 'I';
    

    // Convert node characters to their corresponding indices
    int start = startChar - 'A';
    int target = targetChar - 'A';
    
    
    
    // Compute the shortest path
    dijkstra(start, target);
    
    // Only execute the decision function to output the trajectory moves
    decision();
}

void goForward() {
   while(true){
      // Read sensor values.
  leftSensor   = digitalRead(SENSOR_LEFT);
  rightSensorExtreme = digitalRead(SENSOR_RIGHT_Extreme );
  leftSensorExtreme = digitalRead(SENSOR_LEFT_extreme );
  rightSensor  = digitalRead(SENSOR_RIGHT);
  
 
  if (leftSensor == 0  && rightSensor == 0) {
    goForwardCorrection();
  }
  else if (leftSensor == 1)   {
    turnLeftCorrection();
  }
  else if (rightSensor == 1)    {
    turnRightCorrection();
  }
 if (rightSensorExtreme == 1)   {
    turnRightCorrection();
  }
   if (leftSensorExtreme == 1)   {
    turnRightCorrection();
  }
  if (leftSensorExtreme == 1  && leftSensor == 1) {
 Serial.println("enter1");
 delay(150);
    stopMotors();
    delay(300);
    
    stopMotors();
    break;
  }
  if (rightSensorExtreme == 1  && rightSensor == 1) {
    Serial.println("enter2");
    stopMotors();
    delay(300);
    delay(200);
    stopMotors();
    break;
  }
} 
Serial.println("exit1");
}

void goForwardCorrection() {
     analogWrite(LEFT_SPEED, 150); 
  analogWrite(RIGHT_SPEED, 150); 
  // Left motor: forward.
  digitalWrite(LEFT_FWD, HIGH);
  digitalWrite(LEFT_REV, LOW);
  
  // Right motor: forward.
  digitalWrite(RIGHT_FWD, HIGH);
  digitalWrite(RIGHT_REV, LOW);
}
// Turns the robot to the left.
void turnLeftCorrection() {
  // Left motor: continue forward.
  digitalWrite(LEFT_FWD, LOW);
  digitalWrite(LEFT_REV, HIGH);
  
  // Right motor: stop (or continue slow forward for sharper turn).
  digitalWrite(RIGHT_FWD, HIGH);
  digitalWrite(RIGHT_REV, LOW);

   analogWrite(LEFT_SPEED, correctoncoef); 
  analogWrite(RIGHT_SPEED, correctoncoef);
}


// Turns the robot to the right.
void turnRightCorrection() {
 // Left motor: stop (or continue slow forward for sharper turn).
  digitalWrite(LEFT_FWD, HIGH);
  digitalWrite(LEFT_REV, LOW);
  
  // Right motor: forward.
  digitalWrite(RIGHT_FWD, LOW);
  digitalWrite(RIGHT_REV, HIGH);

    analogWrite(LEFT_SPEED, correctoncoef); 
  analogWrite(RIGHT_SPEED, correctoncoef);
}

// Stops both motors.
void stopMotors() {
        analogWrite(LEFT_SPEED, 0); 
  analogWrite(RIGHT_SPEED, 0);
  digitalWrite(LEFT_FWD, LOW);
  digitalWrite(LEFT_REV, LOW);
  digitalWrite(RIGHT_FWD, LOW);
  digitalWrite(RIGHT_REV, LOW);

}
void turnLeft() {
 delay(200);

  // Left motor: continue forward.
  
      analogWrite(LEFT_SPEED, 120); 
  analogWrite(RIGHT_SPEED, 120); 
  digitalWrite(LEFT_FWD, LOW);
  digitalWrite(LEFT_REV, HIGH);
  
  // Right motor: stop (or continue slow forward for sharper turn).
  digitalWrite(RIGHT_FWD, HIGH);
  digitalWrite(RIGHT_REV, LOW);
  delay(600);
   
  stopMotors() ;
    

}

void turnRight() {
  delay(500);

  // Left motor: continue forward.
  
      analogWrite(LEFT_SPEED, 120); 
  analogWrite(RIGHT_SPEED, 120); 
  digitalWrite(LEFT_FWD, HIGH);
  digitalWrite(LEFT_REV, LOW);
  
  // Right motor: stop (or continue slow forward for sharper turn).
  digitalWrite(RIGHT_FWD, LOW);
  digitalWrite(RIGHT_REV, HIGH);
    delay(600);
  
  stopMotors() ;
    

}

void UturnRot() {
 delay(500);

  // Left motor: continue forward.
  while(rightSensor == 0){
      analogWrite(LEFT_SPEED, 100); 
  analogWrite(RIGHT_SPEED, 100); 
  digitalWrite(LEFT_FWD, LOW);
  digitalWrite(LEFT_REV, HIGH);
  
  // Right motor: stop (or continue slow forward for sharper turn).
  digitalWrite(RIGHT_FWD, HIGH);
  digitalWrite(RIGHT_REV, LOW);
   rightSensor   = digitalRead(SENSOR_RIGHT);
  }
  stopMotors() ;
}

void loop() {
  
   
  }

