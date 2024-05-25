#include <WiFi.h>
#include <PubSubClient.h>

#include "Led.h"
#include "Servo.h"
#include "Proximity.h"
#include "SD_speaker.h"

#define wifi_ssid "DB Shop"
#define wifi_password "Schmatzne"

#define mqtt_server "192.168.35.135"
#define mqtt_user "schwenglon"
#define mqtt_password "schwengl"

#define PLAYER_ID 1
#define CLIENT_ID "quantumschmuhh"

enum class Command {
  DIE,
  HEALTH,
  SOUND,
  INVALID
};


class Player {
public:
  int id;
  int health;
  String sound;
  Player() {
    health = 100;
    id = PLAYER_ID;
    sound = "";
  }
};

Player* player = new Player();

WiFiClient espClient;
PubSubClient client(espClient);

Audio audio;
bool active_sound = false;
bool trigger_servo = true;

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  // reconnect();

  setup_Servo();
  setup_Led();
  setup_Proximity();

  setup_SD();
  setup_speaker();

  pinMode(LED_BUILTIN, OUTPUT);

  client.setCallback(callback);
}

void setup_wifi() {
  delay(5000);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.publish("Im alive leggo", String("online").c_str(), true);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(CLIENT_ID)) {
      Serial.println("connected");
      subscribe_to_all_commands();

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

long lastMsg = 0;
bool alive = false;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (active_sound) {
    audio.loop();
  }
}

void subscribe_to_all_commands() {
  String prefix = "player/";
  String topic = "";

  topic = prefix + String(player->id) + "/health";
  client.subscribe(topic.c_str());

  topic = prefix + String(player->id) + "/die";
  client.subscribe(topic.c_str());

  topic = prefix + String(player->id) + "/sound";
  client.subscribe(topic.c_str());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String value = "";
  for (int i = 0; i < length; i++) {
    value += (char)payload[i];
  }
  
  char *p = topic;
  char *str;
  str = strtok(p, "/"); // delimiter is the semicolon
  String id = strtok(NULL, "/");
  String command = strtok(NULL, "/");

  execute_command(command, value);
}


void execute_command(String command, String value) {  
  switch (resolve_command(command.c_str())) {
    case Command::DIE:
      {
        die();
        break;
      }
    case Command::HEALTH:
      {
        health(value);
        break;
      }
    case Command::SOUND:
      {
        sound(value);
        break;
      }
    case Command::INVALID:
      {
        invalid_command();
        break;
      }
  }
}

Command resolve_command(const char* topic) {
  if (strncmp(topic, "die", sizeof(topic)) == 0) {
    return Command::DIE;
  }
  if (strncmp(topic, "health", sizeof(topic)) == 0) {
    return Command::HEALTH;
  }
  if (strncmp(topic, "sound", sizeof(topic)) == 0) {
    return Command::SOUND;
  }
  return Command::INVALID;
}

void send_position(String position) {
  client.publish("position", position.c_str(), true);
}

void die() {
  String answer = "I am dying.";
  Serial.println(answer);
  client.publish("answer", answer.c_str(), true);
  set_health_led(0);
  tilt_Servo();
  delay(5000);
  set_health_led(3);
}

void health(String value) {
  player->health = value.toInt();
  String answer = "Current Health: Over 9000!";
  Serial.println();
  Serial.print("New Player Health: ");
  Serial.print(player->health);
  client.publish("answer", answer.c_str(), true);
  set_health_led(value.toInt());
}

void sound(String value) {
  player->sound = value;
  String answer = "Never gonna sepp u up";
  Serial.println(answer);
  client.publish("answer", answer.c_str(), true);
  play_sound(value);
}

void invalid_command() {
  String answer = "send me a valid command du hosnbrunza!";
  Serial.println(answer);
  client.publish("invalid command", answer.c_str(), true);
}

void setup_SD() {
    pinMode(SD_CS, OUTPUT);      
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    if(!SD.begin(SD_CS))
    {
      Serial.println("Error talking to SD card!");
      while(true);  // end program
    }
}

void setup_speaker() {
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(VOLUME); // 0...21
}
 

void audio_eof_mp3(const char *info) {
    active_sound = false;
}

void audio_info(const char *info) {
    Serial.println("####### AUDIO INFO #######");
    Serial.println(info);
}

void play_sound(String sound_name) {
	  active_sound = true;
    Serial.print("Playing sound: "); Serial.println(sound_name);
    String file_name = sound_name + ".wav";
    int str_len = file_name.length() + 1;
    char file_name_chars[str_len];
    file_name.toCharArray(file_name_chars, str_len);
    audio.connecttoFS(SD, file_name_chars);
}


