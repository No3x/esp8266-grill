# esp8266-grill

A simple UI to control a fan in a lotus grill with an esp8266 with arduino interface. The chip creates an access point (AP) and uses a captive portal to provide easy access to the UI.

## UI
The UI provides the following actions:
1. Turn the fan on/off
2. Set the speed of the fan
3. View the connection status to the server

<a href="https://user-images.githubusercontent.com/2690708/42408766-416ad834-81d1-11e8-9b93-aacf9d482c1b.jpg" alt="Overview of the app"><img src="https://user-images.githubusercontent.com/2690708/42408766-416ad834-81d1-11e8-9b93-aacf9d482c1b.jpg" height="500"></a>

The UI is build with the library https://github.com/tfry-git/EmbAJAX which provides UI elements in an easy way. If there are more than one devices connected to the UI changes by one device are also propagated to all other devices.

Some things were missing so I added a EmbAJAXStylesheet(const char* stylesheet) to load a stylesheet and EmbAJAXViewport to set the viewport size so it looks better on mobile devices.

## Peripherals
There is a 5V DC brushless motor fan connected to a transistor. The arduino switches the transistor with 3.3V which connects GND and GND of the fan. It uses PWM to control the speed of the fan. 

## Problems
- The fan rpm is not linear to the speed selected in the UI.
- The fan makes buzzing sound at some low speeds
