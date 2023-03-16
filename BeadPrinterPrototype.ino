const int n_rows = 16;
const int n_cols = 16;

typedef enum Color {COLOR0, COLOR1} Color;

int cur_x = 0;
int cur_y = 0;

const int x_positions[] = {0, 5};
const int y_positions[] = {0, 0};

const int servo_pins[] = {13, A3};

int progress = 0;

void moveUp() {
  // move y-axis stepper motor
  cur_y--;
}

void moveDown() {
  // move y-axis stepper motor
  cur_y++;
}

void moveLeft() {
  // move x-axis stepper motor
  cur_x--;
}

void moveRight() {
  // move x-axis stepper motor
  cur_x++;
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
  int servo_pin = servo_pins[toDrop];
  // rotate the servo
  // unrotate the servo
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
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
