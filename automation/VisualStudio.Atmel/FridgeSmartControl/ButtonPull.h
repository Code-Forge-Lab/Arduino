#pragma once
// detect if button was realesed
class ButtonPull {
private:
	bool _state;
	uint8_t _pin;

public:
	ButtonPull(uint8_t pin) : _pin(pin) {}

	void begin() {
		pinMode(_pin, INPUT_PULLUP);
		_state = digitalRead(_pin);
	}

	bool isReleased() {
		bool v = digitalRead(_pin);
		if (v != _state) {
			_state = v;
			if (_state) {
				return true;
			}
		}
		return false;
	}
};


// Example

/*
You can now easily have it work with many buttons :

Button myButton(3);
Button myOtherButton(4);

void setup() {
	myButton.begin();
	myOtherButton.begin();
	Serial.begin(115200);
}

void loop() {
	if (myButton.isReleased()) {
		Serial.println(F("Released 1"));
	}
	if (myOtherButton.isReleased()) {
		Serial.println(F("Released 2"));
	}
}

*/