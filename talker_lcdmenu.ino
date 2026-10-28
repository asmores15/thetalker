#include <LcdMenu.h>
#include <MenuScreen.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>
#include <input/SimpleRotaryAdapter.h>

MENU_SCREEN(mainScreen, mainItems,
    ITEM_BASIC("Item 1"),
    ITEM_BASIC("Item 2"),
    ITEM_BASIC("Item 3"),
    ITEM_BASIC("Item 4"));

const int clkPin = 2;
const int dtPin = 3;
const int swPin = 4;
int encoderVal = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, 16, 2);
LcdMenu menu(renderer);
SimpleRotary encoder(clkPin, dtPin, swPin);
SimpleRotaryAdapter encoderA(&menu, &encoder);

void setup() 
{
    pinMode(clkPin, INPUT);
    pinMode(dtPin, INPUT);
    pinMode(swPin, INPUT);
    digitalWrite(swPin, HIGH);
    Serial.begin(9600);
    renderer.begin();
    menu.setScreen(mainScreen);
}

void loop() 
{
    int change = getEncoderTurn();
    encoderVal = encoderVal + change;
    if (digitalRead(swPin) == LOW)
    {
        encoderVal = 0;
    }
    encoderA.observe();
    Serial.println(encoderVal);
}

int getEncoderTurn(void)
{
  static int oldA = HIGH; //set the oldA as HIGH
  static int oldB = HIGH; //set the oldB as HIGH
  int result = 0;
  int newA = digitalRead(dtPin);//read the value of clkPin to newA
  int newB = digitalRead(clkPin);//read the value of dtPin to newB
  if (newA != oldA || newB != oldB) //if the value of clkPin or the dtPin has changed
  {
    // something has changed
    if (oldA == HIGH && newA == LOW)
    {
      result = (oldB * 2 - 1);
    }
  }
  oldA = newA;
  oldB = newB;
  return result;
}