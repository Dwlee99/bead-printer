#include <ESP32_Servo.h>

const int n_rows = 16;
const int n_cols = 16;

typedef enum Color {COLOR0, COLOR1} Color;

int cur_x = 0;
int cur_y = 0;

const int x_positions[] = {0, 5};
const int y_positions[] = {0, 0};

const int servo_pins[] = {13, A3};
Servo servos[sizeof(servo_pins) / sizeof(servo_pins[0])];
const int min_servo = 500;
const int max_servo = 2400;

const int STEPS_PER_TURN = 200;
const int delay_between_step_microsec = 5000;

const int y_motor_dir_pins[] = {A0};
const int y_motor_step_pins[] = {A1};

const int up_steps = -10;

const int x_motor_dir_pins[] = {A2, A4};
const int x_motor_step_pins[] = {A3, A5};

const int right_steps = 10;

int progress = 0;

void step(bool forward, const int direction_pins[], const int step_pins[])
{
  int num_pins = sizeof(direction_pins) / sizeof(direction_pins[0]);
  // setting the direction
  if (forward == true)
  {
    for(int i = 0; i < num_pins; i++) {
      digitalWrite(direction_pins[i], HIGH);
    }
  }
  else
  {
    for(int i = 0; i < num_pins; i++) {
      digitalWrite(direction_pins[i], LOW);
    }
  }
  for(int i = 0; i < num_pins; i++) {
      digitalWrite(step_pins[i], HIGH);
  }
  for(int i = 0; i < num_pins; i++) {
      digitalWrite(step_pins[i], LOW);
  }
}
void steps(int number_of_steps, const int direction_pins[], const int step_pins[])
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
    step(move_forward, direction_pins, step_pins);
    // Delay for proper speed
    delayMicroseconds(delay_between_step_microsec);
  }
}



void moveUp() {
  // move y-axis stepper motor
  cur_y--;
  steps(up_steps, y_motor_dir_pins, y_motor_step_pins);
}

void moveDown() {
  // move y-axis stepper motor
  cur_y++;
  steps(-up_steps, y_motor_dir_pins, y_motor_step_pins);
}

void moveRight() {
  // move x-axis stepper motor
  cur_x++;
  steps(right_steps, x_motor_dir_pins, x_motor_step_pins);
}

void moveLeft() {
  // move x-axis stepper motor
  cur_x--;
  steps(-right_steps, x_motor_dir_pins, x_motor_step_pins);
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
  if (amount > 0) {
    for (int i = 0; i < amount; i++) {
      moveRight();
      cur_x++;
    }
  }
  else {
    amount = -amount;
    for (int i = 0; i < amount; i++) {
      moveLeft();
      cur_x--;
    }
  }
}

void moveBoard(int r, int c, Color color) {
  moveY(r - cur_y + y_positions[color]);
  moveX(c - cur_x + x_positions[color]);
}

void dropBead(Color toDrop) {
  Servo dropper = servos[toDrop];
  dropper.write(180);
  Serial.println(dropper.read());
  delay(2000);
  dropper.write(0);
  Serial.println(dropper.read());
  delay(2000);
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

void drawImage(Color image[][n_cols]) {
  for(int r = 0; r < n_rows; r++) {
    for(int c = 0; c < n_cols; c++) {
      Color toDrop = image[r][c];
      moveBoard(r, c, toDrop);
      dropBead(toDrop);
      updateProgress(r, c, toDrop);
    }
  }
}

void setup() {
  for(int i = 0; i < sizeof(servo_pins) / sizeof(servo_pins[0]); i++) {
    servos[i].attach(servo_pins[i], min_servo, max_servo);
  }

  for(int i = 0; i < sizeof(y_motor_dir_pins) / sizeof(y_motor_dir_pins[0]); i++) {
    pinMode(y_motor_dir_pins[i], OUTPUT);
    pinMode(y_motor_step_pins[i], OUTPUT);
  }
  
  Serial.begin(9600);
}

void loop() {
  // dropBead(COLOR0);
  moveUp();
  delay(1000);
}
