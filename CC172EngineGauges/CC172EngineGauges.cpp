#include "CC172EngineGauges.h"
#include "allocateMem.h"
#include "commandmessenger.h"

#include "Sprites/GaugeBackground.h"
#include "Sprites/PointerSprite.h"
#include "Sprites/fuelGaugeImg.h"
#include "Sprites/leftFuelCoverImg.h"


/* **********************************************************************************
    This is just the basic code to set up your custom device.
    Change/add your code as needed.
********************************************************************************** */

void drawLeftFuelPointer();
void drawDisplay();

CC172EngineGauges::CC172EngineGauges(uint8_t Pin1, uint8_t Pin2)
{
    _pin1 = Pin1;
    _pin2 = Pin2;
}

LGFX   lcd = LGFX();

LGFX_Sprite canvas(&lcd);

static LGFX_Sprite backgroundSprite(&canvas);  
static LGFX_Sprite pointerSprite(&canvas);
static LGFX_Sprite leftFuelCoverSprite(&canvas);


bool isMobiRunning = false;
double _leftFuel = 0.0;



void CC172EngineGauges::begin()
{
    // cmdMessenger.sendCmd(kDebug, F("Starting CC172EngineGauges"));

    lcd.init();
    lcd.setRotation(1);
    lcd.fillScreen(TFT_DARKGREEN);
    lcd.setBrightness(128);
    lcd.setColorDepth(16);

    canvas.createSprite(228, 228);
//    lcd.setSwapBytes(true);
//    backgroundSprite.setSwapBytes(true);

    // put the GaugeBackground image to a sprite.
    // backgroundSprite.setSwapBytes(true);
//    backgroundSprite.createSprite(480, 480);
    backgroundSprite.setBuffer(const_cast<std::uint16_t*>(FUELGAUGE_IMG_DATA), FUELGAUGE_IMG_WIDTH, FUELGAUGE_IMG_HEIGHT, 16);

//    pointerSprite.createSprite(POINTER_IMG_WIDTH, POINTER_IMG_HEIGHT);



    pointerSprite.setBuffer(const_cast<std::uint16_t*>(POINTER_IMG_DATA), POINTER_IMG_WIDTH, POINTER_IMG_HEIGHT, 16);
    pointerSprite.setPivot(9, POINTER_IMG_HEIGHT/2);

    leftFuelCoverSprite.setBuffer(const_cast<std::uint16_t*>(LEFTFUELCOVER_IMG_DATA), LEFTFUELCOVER_IMG_WIDTH, LEFTFUELCOVER_IMG_HEIGHT, 16);


    canvas.setPivot(28,114);

    backgroundSprite.pushSprite(&canvas,0,0);
    pointerSprite.pushRotated(&canvas, 40, TFT_WHITE);
    leftFuelCoverSprite.pushSprite(&canvas,5,34, TFT_WHITE);

 //   canvas.pushSprite(&lcd,0,0);

//    delay(1000);



    // lcd.setColor(0xFF0000U);                        
    // lcd.fillCircle ( 40, 80, 20    );               
    // lcd.fillEllipse( 80, 40, 10, 20);               
    // lcd.fillArc    ( 80, 80, 20, 10, 0, 90);        
    // lcd.fillTriangle(80, 80, 60, 80, 80, 60);       
    // lcd.setColor(0x0000FFU);                        
    // lcd.drawCircle ( 40, 80, 20    );               
    // lcd.drawEllipse( 80, 40, 10, 20);               
    // lcd.drawArc    ( 80, 80, 20, 10, 0, 90);        
    // lcd.drawTriangle(60, 80, 80, 80, 80, 60);       
    // lcd.setColor(0x00FF00U);                        
    // lcd.drawBezier( 60, 80, 80, 80, 80, 60);        
    // lcd.drawBezier( 60, 80, 80, 20, 20, 80, 80, 60);
    // lcd.setTextColor(TFT_RED, TFT_BLACK);
    // lcd.drawString("V0.1", 0, 400, 1);

}

bool CC172EngineGauges::isValidNumber(const char *str)
{    // Check if the string is a valid number
    if (*str == '-' || *str == '+') str++; // skip sign
    bool hasDecimal = false;
    while (*str) {
        if (*str == '.') {
            if (hasDecimal) return false; // more than one decimal point
            hasDecimal = true;
        } else if (!isdigit(*str)) {
            return false; // non-digit character
        }
        str++;
    }
    return true;
}

void CC172EngineGauges::attach(uint16_t Pin3, char *init)
{
    _pin3 = Pin3;
}

void CC172EngineGauges::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void CC172EngineGauges::set(int16_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -2 will be send from the board when PowerSavingMode is set
            Message will be "0" for leaving and "1" for entering PowerSavingMode
        MessageID == -1 will be send from the connector when Connector stops running
        Put in your code to enter this mode (e.g. clear a display)

    ********************************************************************************** */
    // cmdMessenger.sendCmd(kStatus, F("Got a message in CC172EngineGauges"));

    // do something according your messageID
    switch (messageID) {
    case -1:
        isMobiRunning = false;
        // tbd., get's called when Mobiflight shuts down
        break;
    case -2:
        isMobiRunning = false;
        // tbd., get's called when PowerSavingMode is entered
        break;
    case 0:
        // Check to see if setPoint is '' or NULL
        if (setPoint != nullptr && strlen(setPoint)> 0 && isValidNumber(setPoint)) {
            _leftFuel = atof(setPoint);
        } else {
            _leftFuel = 0.0; // or handle error appropriately
        }
        break;
    case 1:
        /* code */
        break;
    case 2:
        /* code */
        break;
    default:
        break;
    }
}

void CC172EngineGauges::update()
{
    // Do something which is required regulary
    static int i = 0;
    static unsigned long last = 0;

    {
        // if(!isMobiRunning && (millis() - last > 10))
        // {
        //     _leftFuel += 0.1;
        //     if (_leftFuel > 56.0) _leftFuel = 0.0;
        //     last = millis();
        // }
        drawDisplay();
    }
  
//   if(millis() - last > 1000) {
//     lcd.drawPixel(i, 480 - i++);
//     if(i > 480) i = 0;
//     last = millis();
//   }
}

void drawDisplay()
{
    //lcd.startWrite();
    backgroundSprite.pushSprite(&canvas,0,0);
    drawLeftFuelPointer();

    canvas.pushSprite(&lcd,0,0);

    //lcd.endWrite();
    // lcd.display();
}

void drawLeftFuelPointer()
{
    float pointerAngle;
    // rotation angle Starts at 78 and moves backwards. At 28 gal we need to be at -80.5 degrees and at 0 gal we need to be at 78 degrees. So formula is: pointerAngle = 78 + (fuelLevel * (-80.5-78)/(26-0))) 
    pointerAngle = 78.0 + (_leftFuel * (-80.5-78.0)/(28.0));
    canvas.setPivot(35, 114);
    pointerSprite.pushRotated(&canvas, pointerAngle, TFT_WHITE);
    leftFuelCoverSprite.pushSprite(&canvas,5,34, TFT_WHITE);
}