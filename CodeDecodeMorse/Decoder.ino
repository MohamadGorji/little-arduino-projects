String code = "";
int len = 0;
char ch;
char new_char;

const int but = 2;
const int led = 13;

unsigned long pres_len = 0, rel_time, pres_time = 0, old_time_len = 0, old_pres = 0, space = 0;
int state = 0;
int unit_delay = 250;
int min_delay = 10;

char MakeString()
{
  if (pres_len < (unit_delay*3) && pres_len > 50)
  {
    return '.';                        //if button press less than 0.6sec, it is a dot
  }
  else if (pres_len > (unit_delay*3))
  {
    return '-';                        //if button press more than 0.6sec, it is a dash
  }
}

void Morse_decod()
{
  static String morse[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
                             "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "!"
                            };
  int i = 0;
  while (morse[i] != "!")  
  {
    if (morse[i] == code)
    {
      Serial.print(char('A' + i));
      Serial.print(" ");
      break;
    }
    i++;
  }
  if (morse[i] == "!")
  {
    Serial.println("");
    Serial.println("This code is not exist!");
  }

  code = "";
}

void setup() {
  Serial.begin(9600);
  pinMode(but, INPUT_PULLUP);
  pinMode(led, OUTPUT);

}
void loop() {
label:
  while (digitalRead(but) == HIGH) {}
  old_pres = rel_time;
  pres_time = millis();
  digitalWrite(led, HIGH);
  while (digitalRead(but) == LOW) {}
  rel_time = millis();
  digitalWrite(led, LOW);
  pres_len = rel_time - pres_time;
  space = pres_time - old_pres;
  if (pres_len > min_delay)
  {
    code += MakeString();
  }
  while ((millis() - rel_time) < (unit_delay * 3))
  {
    if (digitalRead(but) == LOW)
    {
      goto label;
    }
  }

  Morse_decod();
}