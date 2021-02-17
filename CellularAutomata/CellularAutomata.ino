#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int sameScreen;

uint8_t grid[20][12];
uint8_t newgrid[20][12];

void setup()   
{
  //Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  sameScreen = 0;
}

void loop() 
{

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(25, 5);
  display.println(F("Arduino"));
  display.setCursor(20, 25);
  display.println(F("Cellular"));
  display.setCursor(20, 45);
  display.println(F("Automata"));
  display.display();
  delay(2500);

  display.setTextSize(2);
  display.clearDisplay();
  initGrid();
  drawGrid();
  delay(2000);

  for (int gen = 0; gen < 100; gen++)
  {
    computeCA();
    if (sameScreen == 1) 
    {
      gen = 100;
      sameScreen = 0;
    }
    drawGrid();
    delay(250);
  }

}

void drawGrid(void) 
{
  for (int16_t x = 1; x < 19; x++) 
  {
    for (int16_t y = 1; y < 11; y++) 
    {
      uint8_t color = grid[x][y];
      display.fillRect(6 * x, 6 * y, 6, 6, color);
    }
  }
  display.display();
}

void initGrid(void) 
{
  for (int16_t x = 0; x < 20; x++) 
  {
    for (int16_t y = 0; y < 12; y++) 
    {
      uint8_t color = random(2);
      if (x == 0 || x == 19 || y == 0 || y == 11) 
      {
        grid[x][y] = 0;
      }
      else 
      {
        if (color == 1)
          grid[x][y] = 1;
        else
          grid[x][y] = 0;
      }
    }
  }
}


void computeCA() {
  for (int16_t x = 1; x < 19; x++) 
  {
    for (int16_t y = 1; y < 11; y++) 
    {
      
      int neighbors = getNumberOfNeighbors(x, y);

      if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3 ))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 1)  newgrid[x][y] = 0;

      if (grid[x][y] == 0 && (neighbors == 3))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 0) newgrid[x][y] = 0;

    }

  }

  if (isSameScreen()) sameScreen = 1;

  for (int16_t x = 1; x < 19; x++) 
  {
    for (int16_t y = 1; y < 11; y++) 
    {
      grid[x][y] = newgrid[x][y];
    }
  }
}

int getNumberOfNeighbors(int x, int y)
{
  return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] + grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] + grid[x][y + 1] + grid[x - 1][y + 1];
}

bool isSameScreen() {
  for (int16_t x = 1; x < 20; x++) 
  {
    for (int16_t y = 1; y < 11; y++) 
    {
      if (grid[x][y] != newgrid[x][y]) return false;
    }
  }
  return true;
}
