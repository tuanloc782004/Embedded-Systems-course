#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

int redValue = 0, greenValue = 0, blueValue = 0;

void setup() {
    Serial.begin(9600);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        if (input.indexOf(',') != -1) { 
            parseAndSetColor(input);
        }
    }
    analogWrite(RED_PIN, redValue);
    analogWrite(GREEN_PIN, greenValue);
    analogWrite(BLUE_PIN, blueValue);
}

void parseAndSetColor(String input) {
    int rIndex = input.indexOf("R");
    int gIndex = input.indexOf("G");
    int bIndex = input.indexOf("B");

    if (rIndex != -1) redValue = constrain(input.substring(rIndex + 1, gIndex != -1 ? gIndex : bIndex != -1 ? bIndex : input.length()).toInt(), 0, 255);
    if (gIndex != -1) greenValue = constrain(input.substring(gIndex + 1, bIndex != -1 ? bIndex : input.length()).toInt(), 0, 255);
    if (bIndex != -1) blueValue = constrain(input.substring(bIndex + 1).toInt(), 0, 255);
}
