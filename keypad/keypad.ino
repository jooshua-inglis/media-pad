#define VOL_UP 5
#define VOL_DOWN 6
#define NEXT_TRACK 2
#define PREV_TRACK 3
#define PAUSE_PLAY 4

#define NUM_SWITCHES 5
#define SWITCH_MASK (0b11111111)

typedef struct Buttons {
    uint8_t pin;
    uint8_t counter;
    bool switchState;
} Button_t;

Button_t buttons[NUM_SWITCHES];
uint8_t _switches[]{VOL_UP, VOL_DOWN, NEXT_TRACK, PREV_TRACK, PAUSE_PLAY};

void waitForConnection() {
    Serial.println("AudioHelper");
    int count = 0;
    while (true) {
        Serial.flush();
        delay(500);
        if (Serial.readString() == "con") break;

        Serial.println("not con " + String(count++));
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    waitForConnection();
    setupSwitches();
}

void setupSwitches() {
    Serial.println("Setup switches: ");
    for (uint8_t i = 0; i < NUM_SWITCHES; ++i) {
        buttons[i].pin = _switches[i];
        buttons[i].counter = 0;
        pinMode(_switches[i], INPUT_PULLUP);
    }
    Serial.println("Done");
}

void loop() {
    bool activePins[13] = {0};
    checkPinDown();
    delay(0.5);
}

void checkPinDown() {
    for (uint8_t i = 0; i < NUM_SWITCHES; ++i) {
        debounce(&buttons[i]);
    }
}

void debounce(Button_t *buttonPtr) {
    Button_t button = *buttonPtr;

    bool input = !digitalRead(button.pin);
    volatile uint8_t counter = button.counter;
    volatile bool state = button.switchState;

    buttonPtr->counter = (counter << 1) & SWITCH_MASK | input;

    if (counter == SWITCH_MASK) {
        if (!button.switchState) {
            checkPin(button);
        }
        buttonPtr->switchState = true;
    } else {
        buttonPtr->switchState = false;
    }
}

void checkPin(Button_t button) {
    switch (button.pin) {
        case VOL_UP:
            volUp();
            break;
        case VOL_DOWN:
            volDown();
            break;
        case NEXT_TRACK:
            nextTrack();
            break;
        case PREV_TRACK:
            prevTrack();
            break;
        case PAUSE_PLAY:
            pausePlay();
            break;
    }
}

void volUp() { Serial.println("volUp"); }

void volDown() { Serial.println("volDown"); }
void nextTrack() { Serial.println("nextTrack"); }

void prevTrack() { Serial.println("prevTrack"); }

void pausePlay() { Serial.println("pausePlay"); }
