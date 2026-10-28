#include"rfid1.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
RFID1 rfid; //create a variable type of RFID1

void setup()
{
  Serial.begin(9600); //initialize the serial
  lcd.init();                      // initialize the lcd 
// ports: sda = 20, scl = 21
  lcd.backlight();
  lcd.setCursor(3,0);
  rfid.begin(7, 5, 4, 3, 6, 2);  
//rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,SDA_PIN,RST_PIN)
  delay(100);//delay 1 second
  rfid.init(); //initialize the RFID
}
void loop()
{
  uchar status;
  uchar str[MAX_LEN]; //The maximum length of the array is 16

  // Search card, return card types
  status = rfid.request(PICC_REQIDL, str);
  if (status != MI_OK)
  {
    return;
  }
  Serial.print("Card type: ");
  Serial.println(rfid.readCardType(str));

  //Prevent conflict, return the 4 bytes Serial number of the card
  status = rfid.anticoll(str);
  if (status == MI_OK)
  {
    lcd.setCursor(0,0);
    Serial.print("The card's number is: ");
    lcd.print("Card Number: ");
    lcd.setCursor(0,1);
    int IDlen=4;
    for(int i=0; i<IDlen; i++)
    {
        Serial.print(str[i], HEX);
        //lcd.print(str[i], HEX);
    }
    Serial.println();
    Serial.println();
  }
  
  delay(500);
  rfid.halt(); //command the card into sleep mode 
}
