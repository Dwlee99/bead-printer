#include <ESP32_Servo.h>

const int n_rows = 8;
const int n_cols = 8;

typedef enum Color {COLOR0, COLOR1, NONE} Color;

int cur_x = 0;
int cur_y = 0;

const int x_positions[] = {0, 0};
const int y_positions[] = {0, 9};

// const int servo_pins[] = {13};
// Servo servos[sizeof(servo_pins) / sizeof(servo_pins[0])];
// const int min_servo = 500;
// const int max_servo = 2400;

const int STEPS_PER_TURN = 200;
const int delay_between_step_microsec = 5000;

const int dropper_motor_dir_pins[] = {33, 14};
const int dropper_motor_step_pins[] = {15, 32};

const int y_motor_dir_pin = A0;
const int y_motor_step_pin = A1;

const int up_steps = 43;

const int x_motor_dir_pin = 12;
const int x_motor_step_pin = 27;

const int right_steps = -40;


int progress = 0;

void step(bool forward, int direction_pin, int step_pin)
{
  // setting the direction
  if (forward == true)
  {
    digitalWrite(direction_pin, HIGH);
  }
  else
  {
    digitalWrite(direction_pin, LOW);
  }

  digitalWrite(step_pin, HIGH);
  digitalWrite(step_pin, LOW);

}
void steps(int number_of_steps, int direction_pin, int step_pin)
{
  bool move_forward = true;
  // Establishing the direction
  if (number_of_steps >= 0)
  {
    move_forward = true;
  }
  else
  {
    move_forward = false;
    number_of_steps = -number_of_steps;
  }
  // Generating the steps
  for (int i = 0; i < number_of_steps; i++)
  {
    step(move_forward, direction_pin, step_pin);
    // Delay for proper speed
    // delay(10);
    delayMicroseconds(delay_between_step_microsec);
  }
}

void moveUp() {
  // move y-axis stepper motor
  cur_y--;

  steps(up_steps, y_motor_dir_pin, y_motor_step_pin);
  Serial.println("moving");
}

void moveUp(int amt) {
  // move y-axis stepper motor
  steps(up_steps * amt, y_motor_dir_pin, y_motor_step_pin);
}

void moveDown() {
  // move y-axis stepper motor
  cur_y++;
  steps(-up_steps, y_motor_dir_pin, y_motor_step_pin);
}

void moveDown(int amt) {
  // move y-axis stepper motor
  steps(-up_steps * amt, y_motor_dir_pin, y_motor_step_pin);
}

void moveRight() {
  // move x-axis stepper motor
  cur_x++;
  steps(right_steps, x_motor_dir_pin, x_motor_step_pin);
}


void moveRight(int amt) {
  // move x-axis stepper motor
  steps(right_steps * amt, x_motor_dir_pin, x_motor_step_pin);
}

void moveLeft() {
  // move x-axis stepper motor
  cur_x--;
  steps(-right_steps, x_motor_dir_pin, x_motor_step_pin);
}

void moveLeft(int amt) {
  // move x-axis stepper motor
  steps(right_steps * -amt, x_motor_dir_pin, x_motor_step_pin);
}

void moveY(int amount) {
  if (amount > 0) {
    for (int i = 0; i < amount; i++) {
      moveDown();
    }
  }
  else {
    amount = -amount;
    for (int i = 0; i < amount; i++) {
      moveUp();
    }
  }
}

void moveX(int amount) {
  Serial.print("amount in moveX: ");
  Serial.println(amount);
  if (amount > 0) {
    for (int i = 0; i < amount; i++) {
      moveRight();
    }
  }
  else {
    amount = -amount;
    for (int i = 0; i < amount; i++) {
      moveLeft();
    }
  }
}

void moveBoard(int r, int c, Color color) {
  int amountY = r - cur_y + y_positions[color];
  int amountX = c - cur_x + x_positions[color];

  moveY(amountY);
  moveX(amountX);
}

void dropBead(Color toDrop) {
  for(int i = 0; i < STEPS_PER_TURN; i++) {
    step(true, dropper_motor_dir_pins[toDrop], dropper_motor_step_pins[toDrop]);
    delay(50);
  }
}

void updateProgressBar() {
  // set length of progress bar on display to progress / 256 percent
}

void setPixel(int r, int c, Color color) {
  // set pixel (r, c) to color
}

void updateProgress(int r, int c, Color color) {
  progress++;
  updateProgressBar();
  setPixel(r, c, color);
}

String colorStr(Color c) {
  return (const char *[]) {
    "COLOR0",
    "COLOR1", 
    "NONE",
  }[c];
}

void drawImage(Color image[][n_cols]) {
  for(int r = 0; r < n_rows; r++) {
    for(int c = 0; c < n_cols; c++) {
      Color toDrop = image[r][c];
      // Serial.println(colorStr(toDrop));
      if (toDrop != NONE) {
        moveBoard(r, c, toDrop);
        dropBead(toDrop);
        // Serial.println(c);
        delay(500);
        updateProgress(r, c, toDrop);
      } 
    }
  }
}
// Color XImage[][n_cols]={
//                 {COLOR0,NONE,NONE,NONE,COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,COLOR0,NONE,COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,COLOR0,NONE,COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {COLOR0,NONE,NONE,NONE,COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE}};

// Color LineImage[][n_cols]={{COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE}};

// Color LineImageVertical[][n_cols]={{COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {COLOR0,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE},
//                 {NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE}};

// Color FullBoard[][n_cols]={{COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0},
//                 {COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0}};
// :)
Color Smiley[][n_cols] = {
  {NONE, NONE, COLOR0, NONE, NONE, COLOR0, NONE, NONE},
  {NONE, NONE, COLOR0, NONE, NONE, COLOR0, NONE, NONE},
  {NONE, NONE, COLOR0, NONE, NONE, COLOR0, NONE, NONE},
  {COLOR1, NONE, NONE, NONE, NONE, NONE, NONE, COLOR1},
  {COLOR1, NONE, NONE, NONE, NONE, NONE, NONE, COLOR1},
  {NONE, COLOR1, NONE, NONE, NONE, NONE, COLOR1, NONE},
  {NONE, NONE, COLOR1, COLOR1, COLOR1, COLOR1, NONE, NONE},
  {NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}
};

Color Image[][n_cols] = {{NONE,NONE,NONE,NONE,NONE,COLOR0,COLOR1,COLOR1,},{NONE,NONE,NONE,NONE,NONE,COLOR1,COLOR1,COLOR1,},{NONE,NONE,NONE,NONE,NONE,COLOR1,NONE,NONE,},{NONE,NONE,COLOR1,COLOR0,COLOR0,COLOR1,NONE,COLOR1,},{COLOR1,COLOR1,COLOR0,COLOR0,COLOR1,COLOR1,NONE,NONE,},{COLOR0,COLOR0,COLOR0,COLOR0,COLOR1,NONE,NONE,NONE,},{COLOR0,COLOR0,COLOR0,COLOR0,COLOR1,NONE,NONE,NONE,},{COLOR0,COLOR0,COLOR0,COLOR1,COLOR1,COLOR0,COLOR1,COLOR1,},};

void backToHome() {
  if (cur_x == cur_y && cur_x == 0) {
    moveUp(3);
    moveLeft(3);
  }
  else {
    moveBoard(0, 0, COLOR0);
  }
}

void setup() {
  // for(int i = 0; i < sizeof(servo_pins) / sizeof(servo_pins[0]); i++) {
  //   servos[i].attach(servo_pins[i], min_servo, max_servo);
  // }
  for(int i = 0; i < sizeof(dropper_motor_dir_pins) / sizeof(dropper_motor_dir_pins[0]); i++) {
    pinMode(dropper_motor_dir_pins[i], OUTPUT);
    pinMode(dropper_motor_step_pins[i], OUTPUT);
  }
  
  pinMode(y_motor_dir_pin, OUTPUT);
  pinMode(y_motor_step_pin, OUTPUT);

  pinMode(x_motor_dir_pin, OUTPUT);
  pinMode(x_motor_step_pin, OUTPUT);
  
  backToHome();  

  Serial.begin(9600);
}

int counter = 0;


boolean done = false;
void loop() {
//  homeToFirst();
  // moveLeft(500);
  // moveRight(500);
  // dropBead(COLOR0);
  if(Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    drawImage(Smiley);
    // drawImage(LineImage);
    // drawImage(LineImageVertical);
    Serial.println("done");
    backToHome();
  }

// //  moveUp(-1000);
//   if(Serial.available()>0){
//     String input = Serial.readStringUntil('\n');
//     char firstChar = input.charAt(0);
//     String restOfChar = input.substring(1);
//     int givenSteps = restOfChar.toInt();
    
//     switch(firstChar) {
//       case 'u':
//         moveUp(givenSteps);
//         break; 
//       case 'd':
//         moveDown(givenSteps);
//         break;  
//       case 'l':
//         moveLeft(givenSteps);
//         break;  
//       case 'r':
//         moveRight(givenSteps);
//         break;  
//       case 'b':
//         dropBead(COLOR0);
//         break; 
//       case 'n':
//         done = false;
//         break;
//     }
    
    // Serial.println(input);
//  }
}
