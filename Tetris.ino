
//PIN ASSIGNMENT

//latch pin for freshing the output
const int latch = 13;
//clock signals for shifting bits inside
const int clock_y = 12;
const int clock_x = 11;
//data lanes for shifting bits inside
const int data_y = 10;
const int data_x = 9;

const int randomPort = A0;
const int joystick_x = A1;
const int joystick_y = A2;

const int joystick_btn = 8;


// GAME DATA
byte points = 0;
byte current = 6;
byte next = 3;

int currentOrientation = 0;

int shiftCounter = 0;
int joyStickCounter = 0;

bool gameOver = false;

const int SIZE_X_DATA = 10;
const int SIZE_Y_DATA = 23;

byte data[SIZE_Y_DATA][SIZE_X_DATA] = {
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0}
};

const int EMPTY = 0;
const int STONE = 1;
const int DROPPED = 2;
const int GAP = 3;


//STONES
const int I = 0;
const int J = 1;
const int L = 2;
const int O = 3;
const int S = 4;
const int T = 5;
const int Z = 6;

//STONES ORIENTATION
const int A = 0;
const int B = 1;
const int C = 2;
const int D = 3;

//DISPLAYED OUTPUT

const int SIZE_X = 16;
const int SIZE_Y = 24;

byte content[SIZE_Y][SIZE_X] = {
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
    {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  };

void setup() {
  pinMode (latch, OUTPUT);
  pinMode (clock_x, OUTPUT);
  pinMode (clock_y, OUTPUT);
  pinMode (data_x, OUTPUT);
  pinMode (data_y, OUTPUT);

  pinMode (joystick_btn,INPUT);
  Serial.begin(9600);

  gameOver = placeNextStone();
  
}

void loop() {
  if(gameOver){
    return;
  }
  //NORMAL LOOP
  
  if(joyStickCounter ==5){
    readJoyStick();
    joyStickCounter = 0;
  }
  joyStickCounter++;
  
  //SHIT LOOP
  if(shiftCounter == 10){
    shiftCounter = 0;
    bool dropped = shiftStoneDown();
    removeRows();
    shiftRowsDown();
    if(dropped){
      current = next;
      next = getNextStone();
      gameOver = placeNextStone();
    }
    
  }
  

  //DISPLAY:
  displayData();
  displayPoints();
  displayNextStone();
  out();
  shiftCounter++;
}

void readJoyStick(){
  int buttonState = digitalRead(joystick_btn);
  int val_x = analogRead(joystick_x);
  int val_y = analogRead(joystick_y);
  

  if(val_y < 100){
    Serial.write("moveRight");
    moveRight();
  }
  if(val_y > 900){
    Serial.write("moveLeft");
    moveLeft();
  }

  if(val_x < 100){
    rotateItem((currentOrientation-1)%4);
  }

  if(val_x > 900){
    rotateItem((currentOrientation+1)%4);
  }
}

//GAME LOGIC
void removeRows(){
  int x;
  int y;
  for(y = 0; y < SIZE_Y_DATA; y++){
    byte counter = 0;
    for(x = 0; x < SIZE_X_DATA; x++){
      if(data[y][x] == DROPPED){
        counter++;
      }
    }
    if(counter == SIZE_X_DATA){
      for(x = 0; x < SIZE_X_DATA; x++){
        data[y][x] = GAP;
      }
      points++;
    }
  }
}

void shiftRowsDown(){
  int y;
  int y_0;
  for(y = SIZE_Y_DATA-1; y > 0; y--){
    if(data[y][0] == GAP){
      for(y_0 = y-1; y_0 >= 0; y_0--){
        int x;
        for(x = 0; x < SIZE_X_DATA; x++){
          if(data[y_0][x] == DROPPED){
            data[y][x] = data[y_0][x];
            data[y_0][x] = EMPTY;
          }else{
            data[y][x] = EMPTY;
          }
           
        }
      }
    }
  }
}

bool shiftStoneDown(){
  int y;
  int x;
  bool dropped = false;

  //detect all stone fields
  for(y = 0; y < SIZE_Y_DATA; y++){
    for(x = 0; x < SIZE_X_DATA; x++){
      if(data[y][x] == STONE){
        if(y+1 >= SIZE_Y_DATA){
          dropped = true;
        }else{
          if(data[y+1][x] == DROPPED){
            dropped = true;
          }
        }
      }
    }
  }
  if(dropped){
    for(y = 0; y < SIZE_Y_DATA; y++){
      for(x = 0; x < SIZE_X_DATA; x++){
        if(data[y][x] == STONE){
            data[y][x] = DROPPED;
        }
      }
    }
  }else{
    for(y = SIZE_Y_DATA-1; y >= 0; y--){
      for(x = 0; x < SIZE_X_DATA; x++){
        if(data[y][x] == STONE){
            data[y+1][x] = STONE;
            data[y][x] = EMPTY;
        }
      }
    }
  }

  return dropped;
}

bool placeNextStone(){
  switch(current){
    case I:
      if(data[0][4] == DROPPED){ return true; } else{ data[0][4] = STONE;}
      if(data[0][5] == DROPPED){ return true; } else{ data[0][5] = STONE;}
      if(data[0][6] == DROPPED){ return true; } else{ data[0][6] = STONE;}
      if(data[0][7] == DROPPED){ return true; } else{ data[0][7] = STONE;}
      break;
    case J:
      if(data[0][4] == DROPPED){ return true; } else{ data[0][4] =STONE;}
      if(data[1][4] == DROPPED){ return true; } else{ data[1][4] =STONE;}
      if(data[1][5] == DROPPED){ return true; } else{ data[1][5] =STONE;}
      if(data[1][6] == DROPPED){ return true; } else{ data[1][6] =STONE;}
      break;
    case L:
      if(data[1][5] == DROPPED){ return true; } else{ data[1][5] =STONE;}
      if(data[1][6] == DROPPED){ return true; } else{ data[1][6] =STONE;}
      if(data[0][7] == DROPPED){ return true; } else{ data[0][7] =STONE;}
      if(data[1][7] == DROPPED){ return true; } else{ data[1][7] =STONE;}
      break;
    case O:
      if(data[0][5] == DROPPED){ return true; } else{ data[0][5] =STONE;}
      if(data[0][6] == DROPPED){ return true; } else{ data[0][6] =STONE;}
      if(data[1][5] == DROPPED){ return true; } else{ data[1][5] =STONE;}
      if(data[1][6] == DROPPED){ return true; } else{ data[1][6] =STONE;}
      break;
    case S:
      if(data[0][5] == DROPPED){ return true; } else{ data[0][5] =STONE;}
      if(data[0][6] == DROPPED){ return true; } else{ data[0][6] =STONE;}
      if(data[1][6] == DROPPED){ return true; } else{ data[1][6] =STONE;}
      if(data[1][7] == DROPPED){ return true; } else{ data[1][7] =STONE;}
      break;
    case T:
      if(data[0][6] == DROPPED){ return true; } else{ data[0][6] =STONE;}
      if(data[1][5] == DROPPED){ return true; } else{ data[1][5] =STONE;}
      if(data[1][6] == DROPPED){ return true; } else{ data[1][6] =STONE;}
      if(data[1][7] == DROPPED){ return true; } else{ data[1][7] =STONE;}
      break;
    case Z:
      if(data[0][5] == DROPPED){ return true; } else{ data[0][5] =STONE;}
      if(data[0][6] == DROPPED){ return true; } else{ data[0][6] =STONE;}
      if(data[1][6] == DROPPED){ return true; } else{ data[1][6] =STONE;}
      if(data[1][7] == DROPPED){ return true; } else{ data[1][7] =STONE;}
      break;
  
  }
  currentOrientation = A;
  return false;
  
}

void moveLeft(){
  int y;
  int x;
  bool collision = false;

  //check whether it is possible to move stone to the left
  for(y = 0; y < SIZE_Y_DATA; y++){
    for(x = 0; x < SIZE_X_DATA; x++){
      if(data[y][x] == STONE){
        if(x-1 < 0){
          collision = true;
        }else{
          if( data[y][x-1] == DROPPED){
            collision = true;
          }
        }
      }
    }
  }
  if(!collision){
    for(y = SIZE_Y_DATA-1; y >= 0; y--){
      for(x = 0; x < SIZE_X_DATA; x++){
        if(data[y][x] == STONE){
            data[y][x-1] = STONE;
            data[y][x] = EMPTY;
        }
      }
    }
  }
}

void moveRight(){
  int y;
  int x;
  bool collision = false;

  //check whether it is possible to move stone to the left
  for(y = 0; y < SIZE_Y_DATA; y++){
    for(x = 0; x < SIZE_X_DATA; x++){
      if(data[y][x] == STONE){
        if(x+1 >= SIZE_X_DATA){
          collision = true;
        }else{
          if(data[y][x+1] == DROPPED){
            collision = true;
          }
        }
      }
    }
  }
  if(!collision){
    for(y = SIZE_Y_DATA-1; y >= 0; y--){
      for(x = SIZE_X_DATA-1; x >=0; x--){
        if(data[y][x] == STONE){
            data[y][x+1] = STONE;
            data[y][x] = EMPTY;
        }
      }
    }
  }
}

void dropDown(){

  bool dropped = false;
  while(!dropped){
    dropped = shiftStoneDown();
  }
  
}

void rotateItem(int newOrientation){
  switch(current){
    case I:
      rotateI(newOrientation);
    break;
    case J:
      rotateJ(newOrientation);
    break;
    case L:
      rotateL(newOrientation);
    break;
    case O:
      rotateO(newOrientation);
    break;
    case S:
      rotateS(newOrientation);
    break;
    case T:
      rotateT(newOrientation);
    break;
  }
}

void rotateI(int newOrientation){
  switch(newOrientation){
    case A:
      
    break;
    case B:
    break;
    case C:
    break;
    case D:
    break;
  }
}

void rotateJ(int newOrientation){
  
}

void rotateL(int newOrientation){
  
}

void rotateO(int newOrientation){
  
}

void rotateS(int newOrientation){
  
}

void rotateT(int newOrientation){
  
}




int getNextStone(){
  return analogRead(randomPort)%7;
}

//DISPLAY METHODS

void displayData(){
  int x;
  int y;
  for(y = 0; y < SIZE_Y_DATA; y++){
    for(x = 0; x < SIZE_X_DATA; x++){
      if(data[y][x] == STONE || data[y][x] == DROPPED){
        content[y][x+1] = 1;
      }else{
        content[y][x+1] = 0;
      }
      
    }
  }
}

void displayPoints(){
  if(points > 45){
    //you win
  }else{
    int x;
    int y;
    //Reset first;
    for(y = 22; y >= 8; y--){
        for(x = 12; x <= 14; x++){
          content[y][x] = 0;
        }
      }
    
    byte p = 0;
    for(y = 22; y >= 8; y--){
      for(x = 12; x <= 14; x++){
          if(p >= points){
            break;
          }
          p++;
          content[y][x] = 1;
          
          
        
      }
    }
  }
}

void displayNextStone(){
  //Reset first:
  int x;
  int y;
  for(y = 1; y <= 4; y++){
    for(x = 13; x <= 14; x++){
      content[y][x] = 0;
    }
  }
  switch(next){
    case I:
      content[1][13] = 1;
      content[2][13] = 1;
      content[3][13] = 1;
      content[4][13] = 1;
      break;
    case J:
      content[1][14] = 1;
      content[2][14] = 1;
      content[3][14] = 1;
      content[3][13] = 1;
      break;
    case L:
      content[1][13] = 1;
      content[2][13] = 1;
      content[3][13] = 1;
      content[3][14] = 1;
      break;
    case O:
      content[2][13] = 1;
      content[2][14] = 1;
      content[3][13] = 1;
      content[3][14] = 1;
      break;
    case S:
      content[1][13] = 1;
      content[2][13] = 1;
      content[2][14] = 1;
      content[3][14] = 1;
      break;
    case T:
      content[1][13] = 1;
      content[2][13] = 1;
      content[2][14] = 1;
      content[3][13] = 1;
      break;
    case Z:
      content[1][14] = 1;
      content[2][14] = 1;
      content[2][13] = 1;
      content[3][13] = 1;
      break;
  }
}

void out(){

  int x;
  int y;
  unsigned long dat_x = 0x0000000000000001;
  
  for(x = 0; x < SIZE_X; x++){
    long dat_y = 0x00;
    for(y = 23; y > -1; y--){
      dat_y = dat_y <<1;
      dat_y = dat_y | content[y][x];
      
    }
    digitalWrite(latch,LOW);
    shiftOutLong(data_x,clock_x,~dat_x);
    shiftOutLong(data_y,clock_y,dat_y);
    digitalWrite(latch,HIGH);
    dat_x = dat_x << 1; //shift low bit
  }
  
  
}

void shiftOutLong(int dataPin, int clockPin, unsigned long value){
  const unsigned long mask = 0x80000000;
  int counter;
  for(counter=0; counter < 32; counter++){
    digitalWrite(clockPin,LOW);
    digitalWrite(dataPin,(value & mask) == mask ? HIGH : LOW);
    digitalWrite(clockPin, HIGH);
    value = value << 1;
  }

}
