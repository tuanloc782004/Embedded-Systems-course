#define RED_PIN 11
#define GREEN_PIN 10
#define BLUE_PIN 9

int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int duration = 0;

void setup() {
    Serial.begin(9600);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        int newRedValue = extractValue(input, 'R');
        int newGreenValue = extractValue(input, 'G');
        int newBlueValue = extractValue(input, 'B');
        int newDuration = extractValue(input, 'D');

        if (newRedValue != -1) redValue = newRedValue;
        if (newGreenValue != -1) greenValue = newGreenValue;
        if (newBlueValue != -1) blueValue = newBlueValue;
        duration = (newDuration != -1) ? newDuration : 0;

        analogWrite(RED_PIN, redValue);
        analogWrite(GREEN_PIN, greenValue);
        analogWrite(BLUE_PIN, blueValue);

        if (duration > 0) {
            delay(duration);
            analogWrite(RED_PIN, 0);
            analogWrite(GREEN_PIN, 0);
            analogWrite(BLUE_PIN, 0);
        }
    }
}

int extractValue(String data, char color) {
    int index = data.indexOf(color);
    if (index != -1) {
        int endIndex = index + 1;
        while (endIndex < data.length() && isDigit(data[endIndex])) {
            endIndex++;
        }
        int value = data.substring(index + 1, endIndex).toInt();
        return (color == 'D') ? max(value, 0) : constrain(value, 0, 255);
    }
    return -1;
}