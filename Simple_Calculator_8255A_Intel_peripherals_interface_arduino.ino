const int a0 = A4;// a0 of ic connected with A4 in arduino
const int a1 = A5;// a1 of ic connected with A5 in arduino
const int WR = A3;// WR of ic connected with A3 in arduino
const int RD = A2;// RD of ic connected with A2 in arduino


double result = 0.0;//result of operations
int symbol = '0';   //+,-,*,/,c
int temp;           //MAKE US KNOW THE SELECTED COL
double num1 = 0;
double num2 = 0;

boolean num1ISNegative =false;
boolean num2ISNegative =false;

String st;

const byte row = 4;
const byte col = 4;
byte RowPins[row] = {0, 1, 2, 3}; //rows connected with pin 0 : 3 of 8255
byte ColPins[col] = {4, 5, 6, 7}; //columns connected with pin 4 : 7 of 8255


int keypad[row][col] = {'7', '8', '9', '/',
            '4', '5', '6', '*',
            '1', '2', '3', '-',
            '.', '0', '=', '+'
                      };
void setup8255ppl(){
  //Selecting Control Register d0->d7 of 8255
  digitalWrite(a0, 1);
  digitalWrite(a1, 1);

  digitalWrite(7, 1);  // I/O mode
  digitalWrite(6, 0);  // Mode 0
  digitalWrite(5, 0);  // Mode 0
  digitalWrite(4, 0);  // portA Output
  digitalWrite(3, 1);  // port c 4 -> 7 Input
  digitalWrite(2, 0);  // Mode 0
  digitalWrite(1, 0);  // portB Output
  digitalWrite(0, 0);  // port c 4 -> 7 Output

  digitalWrite(WR, 0);//WRITE ENABLED
  delay(10);
  digitalWrite(WR, 1);//WRITE DISABLED
}
void LCDIntialize(){
  writeOnlcd(0x38, 0); //intialize lcd
  delay(30);
  writeOnlcd(0x0E, 0); //display cursor
  delay(30);
  writeOnlcd(0x01, 0); //clear lcd
  delay(30);
  writeOnlcd(0x06, 0); //shift cursor right
  delay(30);
}
void setup()
{
  // put your setup code here, to run once:
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(a0, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(WR, OUTPUT);
  
  digitalWrite(RD, 1);   //READ DISABLED
  digitalWrite(WR, 1);  //WRITE DISABLED
  setup8255ppl();
  LCDIntialize();
}

void columns_input_mode(){ //  pins  4 : 7 input from cols
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
}

void columns_output_mode() {// pins  4 : 7 output on cols
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int Calculation(int Ops) //function to make Calculation
{
  if (Ops == '+')
  {
    if (num1ISNegative == true) {
      num1 = num1 * -1;
      num1ISNegative = false;
    }
    if (num2ISNegative == true) {
      num2 = num2 * -1;
      num2ISNegative = false;
    }
    result = (num1 + num2);
    String str = String(result);
    displayOnLCD(str);
  }
  if (Ops == '-')
  {
    if (num1ISNegative == true) {
      num1 = num1 * -1;
      num1ISNegative = false;
    }
    if (num2ISNegative == true) {
      num2 = num2 * -1;
      num2ISNegative = false;
    }
    result = (num1 - num2);
    String str = String(result);
    displayOnLCD(str);

  }
  if (Ops == '*')
  {
    if (num1ISNegative == true)
    {
      num1 = num1 * -1;
      num1ISNegative = false;
    }
    if (num2ISNegative == true) {
      num2 = num2 * -1;
      num2ISNegative = false;
    }
    result = (num1 * num2);
    String str = String(result);
    displayOnLCD(str);
  }
  if (Ops == '/')
  {
    if (num2 == 0) {//We can't divide on 0
      displayOnLCD("Error");
      delay(1);
      num1 = 0;
      num2 = 0;
      Ops = '0';
      num1ISNegative =false;
      num2ISNegative =false;
    }
    else
    {
      if (num1ISNegative == true) {
        num1 = num1 * -1;
        num1ISNegative = false;
      }
      if (num2ISNegative == true) {
        num2 = num2 * -1;
        num2ISNegative = false;
      }
      result = (num1 / num2);
      String str = String(result);
      displayOnLCD(str);
    }
  }
  return '0';
}

void displayOnLCD(String msg)
{
  for (int i = 0; i < msg.length()+2; i++)
  {
    writeOnlcd(msg[i], 1);
    delay(1);
  }
  delay(10);
}

//To send a command to LCD, RS=0  & To send Data to LCD, RS=1
void writeOnlcd(int num , int rs)
{
  //port A is selected
  digitalWrite(a0, 0);
  digitalWrite(a1, 0);
  for (int i = 0; i < 8; i++)//For loop to enter i to the pins of port A for ic
  {
    digitalWrite(i, (num >> i) & 0x01);// Here we make anding for i and 1 to take it to port A
  }

  digitalWrite(WR, 0);
  delay(10);
  digitalWrite(WR, 1);

  //port B is selected
  digitalWrite(a0, 1);
  digitalWrite(a1, 0);
  //to send command make E=1
  digitalWrite(6, rs);
  digitalWrite(7, 1);

  digitalWrite(WR, 0);
  delay(10);
  digitalWrite(WR, 1);

  //then make E=0
  digitalWrite(6, rs);
  digitalWrite(7, 0);

  digitalWrite(WR, 0);
  delay(10);
  digitalWrite(WR, 1);
}

void loop()
{
  
  for (int i = 0; i <= 3; i++)
  {
    //select PortC
    digitalWrite(a0, 0);
    digitalWrite(a1, 1);

    digitalWrite(RowPins[i], 1); //set a  row  = 1


    digitalWrite(WR, 0);
    delay(1);
    digitalWrite(WR, 1);

    digitalWrite(i, 0);// set selected row = 0 low

    digitalWrite(WR, 0);
    delay(1);
    digitalWrite(WR, 1);

    // to know the selected col
    for (int j = 4; j <= 7; j++)
    {
      columns_input_mode();
      digitalWrite(RD, 0);// enable RD
      temp = digitalRead(j); //num of the pressed/selected col
      // IF THERE IS AN ERROR WHEN YOU PRESS ANY KEY ON KEYPAD YOU CAN EDIT THE DELAY HERE TO BE COMPATABLE WITH YOUR PC
      delay(10);
      digitalWrite(RD, 1);//disable RD
      columns_output_mode();
      
      if (temp == 0)
      {
        //if the pressed key is symbol ( * , + , - , / , = , . )
        if ((keypad[i][j - 4] == keypad[0][3]) | (keypad[i][j - 4] == keypad[1][3]) | (keypad[i][j - 4] == keypad[2][3]) | (keypad[i][j - 4] == keypad[3][3]) | (keypad[i][j - 4] == keypad[3][2]) | (keypad[i][j - 4] == keypad[3][0]))
        {
          delay(1);
          if (keypad[i][j - 4] == '.')
          {
            writeOnlcd(0x01, 0);
            writeOnlcd(0x06, 0);
            symbol = '0' ;
            num1 = 0 ;
            num2 = 0 ;
            st = "";
            break;
          }
          if (keypad[i][j - 4] == '=')
          {
            writeOnlcd('=', 1);
            symbol = Calculation(symbol);
            num1 = 0 ;
            num2 = 0 ;
            st = "";
            break;
          }
          
          if (keypad[i][j - 4] == '-' && num1 == 0)
          {
            writeOnlcd('-', 1);
            symbol = '0';
            num1ISNegative = true;
          }
          else if(keypad[i][j - 4] == '-' && num2 == 0 && symbol!='0')
          {
            writeOnlcd('-', 1);
            num2ISNegative = true;
          }
          else if(symbol =='0')
          {
            symbol = keypad[i][j - 4] ;
            st += symbol;
            writeOnlcd(symbol, 1);
          }

        }
        else
        {
          st += (keypad[i][j - 4]);
          writeOnlcd(keypad[i][j - 4], 1);
          delay(2);

          if (symbol == '0')
          {
            //convert String into int
            num1 = ( num1 * 10 ) + ( keypad[i][j - 4]) - '0'  ;
          }

          else
          {
            num2 = ( num2 * 10 ) +  (keypad[i][j - 4]) - '0'  ;
          }
          break;
        }
      }
    }
    digitalWrite(i, 1);  //row = 1 (Active Low to reenter)
    digitalWrite(WR, 0);
    delay(1);
    digitalWrite(WR, 1);
  }
}
