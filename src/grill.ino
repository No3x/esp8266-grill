#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <EmbAJAX.h>

const int fanPin = D7;
const char* ssid = "Grill Settings";
const char* password = "barbeqool";
IPAddress apIP(172, 16, 1, 1);

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;
ESP8266WebServer http ( 80 );
EmbAJAXOutputDriverESP8266 driver(&http);

// Define the main elements of interest as variables, so we can access them later in our sketch.
EmbAJAXCheckButton button("enabled", "");
EmbAJAXSlider slider("slider", 500, 1023, 700);   // slider, from 0 to 500, initial value 400
EmbAJAXMutableSpan display("display");         // a plain text display

// Define a page (named "page") with our elements of interest, above, interspersed by some uninteresting
// static HTML. Note: MAKE_EmbAJAXPage is just a convenience macro around the EmbAJAXPage<>-class.
MAKE_EmbAJAXPage(page, "Settings", "",
  new EmbAJAXStatic("<h1>Grill Settings</h1>"),
  new EmbAJAXStatic( "<p><label for=\"enabled\">The system should run: </label>"),
  &button,
  new EmbAJAXStatic("</p><p><label>Speed: </label>"),
  &slider,
  new EmbAJAXStatic("</p><p>Current speed is: <b>"),
  &display,
  new EmbAJAXStatic("</b></p>"),
  new EmbAJAXStatic("Connection status: "),
  new EmbAJAXConnectionIndicator(),
  new EmbAJAXStatic("<script type=\"text/javascript\">"
  "var viewPortTag=document.createElement('meta');"
  "viewPortTag.id=\"viewport\";"
  "viewPortTag.name = \"viewport\";"
  "viewPortTag.content = \"width=320; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;\";"
  "document.getElementsByTagName('head')[0].appendChild(viewPortTag);"
  "</script>"),
  new EmbAJAXStatic("<script type=\"text/javascript\">"
  "document.querySelector('head').innerHTML += '<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\"/>';"
  "</script>")
)

// This is all you need to write for the page handler
void handlePage() {
  if(http.method() == HTTP_POST) { // AJAX request
    page.handleRequest(updateUI);
  } else {  // Page load
    page.print();
  }
}

void setup() {
  Serial.begin ( 115200 );

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  Serial.print("Configuring access point...");

  //  WiFi connection is OK
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  if (!SPIFFS.begin())
  {
    // Serious problem
    Serial.println("SPIFFS Mount failed");
  } else {
    Serial.println("SPIFFS Mount succesfull");
  }

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  //http.serveStatic("/img", SPIFFS, "/img");
  //http.serveStatic("/", SPIFFS, "/index.html");
  http.onNotFound(handlePage);
  // Tell the server to serve our EmbAJAX test page on root
  http.on("/", handlePage);
  http.serveStatic("/style.css", SPIFFS, "/style.css");

  // Démarre le serveur web - Start Web Server
  http.begin();
  Serial.println ( "HTTP server started" );

  analogWriteFreq(1);

  updateUI();  // initialize display
}

char buf[16];
void updateUI() {
  slider.setEnabled(button.isChecked());
  display.setEnabled(button.isChecked());
  // And the following line is all that is needed to read the slider value, convert to a string, and send it
  // back to the client for display:
  display.setValue(itoa(slider.intValue(), buf, 10));

  // NOTE: Instead of in this separate function, you could also place the above line in loop(). However, having
  // it here allows the library to send back the updated display, immediately, resulting in a snappier UI on
  // the client.
}

void loop() {
  dnsServer.processNextRequest();
  http.handleClient();
  analogWrite(fanPin, (button.isChecked() ? slider.intValue() : 0));
  //delay(50);
}
