//********* Read me *********//

// ต้องติดตั้ง Limit Switch แกน Y ให้เรียบร้อย
// ถ้ายังไม่ติดให้ไป comment function MotorY_Down(); กับ MotorY_Up(); ใน void Position

//ตัวดันแก้ว
#define DIR1 2
#define STP1 3
#define EN1  4

//ตัวเคลื่อนสายพาน
#define DIR2 5
#define STP2 6
#define EN2  7
//***********  ต่อ Limit Switch Pin10 ด้วยค้าบ  ***********//
#define Limit_Y 10        
#define Limit_R 11
#define Limit_L 12

//********************* ปั๊มน้ำ *************************//
#define pump 14

//****************************************************//

uint16_t Count = 0;
int Current_Position = 0;

// ตำแหน่งของ ที่กดน้ำ
const int Position1 = 1850;
const int Position2 = 4300;
const int Position3 = 7800;
const int Position4 = 11700;
const int Position5 = 15100;

//******************// All Function  //******************//
int  posi (char P);                  //******************//
void Main (void);                    //******************//
void Set_Home (void);                //******************//
void Initialize (void);              //******************//
void Motor_Left(int Step);           //******************//
void Motor_Right(int Step);          //******************//
void MotorY_Up (void);               //******************//
void MotorY_Down (void);             //******************//
void Position (char Next_Posi);      //******************//
void Pump(int watertime);
//*******************************************************//

void setup() {
  Initialize();
  Set_Home();
}

void loop() {
  Main();
}

void Main (void) {
  if (Serial.available() > 0 ) {
    char Menu = Serial.read();
    Serial.print("Menu = ");
    Serial.println(Menu);

    switch (Menu) {
      case '0': {
          Set_Home();
          break;
        }
      case '1': {
        // MixedBerry Lemonade
        Position(1);
        Pump(4000);
        delay(1000);
        Position(4);
        PressWater_Sticky();
        Position(5);
        PressWater();
        
        break;
      }
      case '2': {
        //BlueHawaii
        Position(1);
        Pump(4000);
        delay(1000);
        Position(3);
        PressWater_Sticky();
        break;
      }
      case '3': {
          // Orange Breach
        Position(1);
        Pump(4000);
        delay(1000);
        Position(2);
        PressWater();
        break;
      }
      case '4': {
        //Lemonade
        Position(1);
        Pump(4000);
        delay(1000);
        Position(5);
        PressWater();
        break;
      }
      case '5': {
        //BlueHawaiiMixeBerry
        Position(1);
        Pump(4000);
        delay(1000);
        Position(3);
        PressWater_Sticky();
        Position(4);
        PressWater_Sticky();
        break;
      }
    }
  }
}

void Initialize (void) {
  Serial.begin(9600);
  for (int i = 2; i < 8; i++) pinMode(i, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(Limit_L, INPUT_PULLUP);
  pinMode(Limit_R, INPUT_PULLUP);
  pinMode(Limit_Y, INPUT_PULLUP);
  digitalWrite(pump, HIGH);
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
  delay(300);
}

void Set_Home (void) {
  MotorY_Down();

  digitalWrite(DIR2, HIGH);
  while (digitalRead(Limit_L)) {
    digitalWrite(STP2, HIGH);
    delayMicroseconds(500);
    digitalWrite(STP2, LOW);
    delayMicroseconds(500);
  }
  delay(100);
  Motor_Right(150);
  delay(100);
  digitalWrite(DIR2, HIGH);
  while (digitalRead(Limit_L)) {
    digitalWrite(STP2, HIGH);
    delayMicroseconds(500);
    digitalWrite(STP2, LOW);
    delayMicroseconds(500);
  }
  Count = 0;
  delay(100);
}

void Motor_Left(int Step) {
  digitalWrite(DIR2, HIGH);
  for (int i = 0; i < Step; i++) {
    digitalWrite(STP2, HIGH);
    delayMicroseconds(500);
    digitalWrite(STP2, LOW);
    delayMicroseconds(500);
  }
}

void Motor_Right(int Step) {
  digitalWrite(DIR2, LOW);
  for (int i = 0; i < Step; i++) {
    digitalWrite(STP2, HIGH);
    delayMicroseconds(500);
    digitalWrite(STP2, LOW);
    delayMicroseconds(500);
  }
}

void MotorY_Up (void) {
  digitalWrite(DIR1, LOW);
  for (int i = 0; i < 2400; i++) {  // เพิ่มค่า i ถ้าอยากให้เลื่อนสูงขึ้น
    digitalWrite(STP1, HIGH);
    delayMicroseconds(500);
    digitalWrite(STP1, LOW);
    delayMicroseconds(500);
  }
}
void MotorY_Up_Sticky(void) {
  digitalWrite(DIR1, LOW);
  for (int i = 0; i < 2600; i++) {  // เพิ่มค่า i ถ้าอยากให้เลื่อนสูงขึ้น
    digitalWrite(STP1, HIGH);
    delayMicroseconds(500);
    digitalWrite(STP1, LOW);
    delayMicroseconds(500);
  }
}
void MotorY_Down (void) {
  digitalWrite(DIR1, HIGH);
  while (digitalRead(Limit_Y)) {
    digitalWrite(STP1, HIGH);
    delayMicroseconds(500);
    digitalWrite(STP1, LOW);
    delayMicroseconds(500);
  }
}

int posi (char P) {
  if (P == 1)       return Position1;
  else if (P == 2)  return Position2;
  else if (P == 3)  return Position3;
  else if (P == 4)  return Position4;
  else if (P == 5)  return Position5;
}

void Position (char Next_Posi) {  // *****  ใช้ค่าพารามิเตอร์ได้แค่ 1-5  ******
  int Count_Dri = posi(Next_Posi) - Count;
  Count = posi(Next_Posi);
  Current_Position < Next_Posi ? Motor_Right(Count_Dri) : Motor_Left(-Count_Dri);
  Current_Position = Next_Posi;
}
void Pump(int watertime){
  digitalWrite(pump,LOW);
  delay(watertime);
  digitalWrite(pump,HIGH);
}
void PressWater_Sticky(){
  delay(500);
  MotorY_Up_Sticky();
  delay(10000);   //****************  หน่วงเวลาให้น้ำไหลออกมา  **************//
  MotorY_Down(); 
}
void PressWater(){
  delay(500);
  MotorY_Up();
  delay(3000);   //****************  หน่วงเวลาให้น้ำไหลออกมา  **************//
  MotorY_Down();
}
