#include <WiFi.h>
#include <PubSubClient.h>

#include "Led.h"
#include "Servo.h"
#include "Proximity.h"

#define wifi_ssid "DB Shop"
#define wifi_password "Schmatzne"

#define mqtt_server "192.168.35.135"
#define mqtt_user "schwenglon"
#define mqtt_password "schwengl"

#define PLAYER_ID 1

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

Player* player1 = new Player();

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  reconnect();

  setup_Servo();
  setup_Led();
  setup_Proximity();

  pinMode(LED_BUILTIN, OUTPUT);

  subscribe_to_all_commands();
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
    if (client.connect("quantumschmuhh")) {
      Serial.println("connected");

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

  long now = millis();
  if (now - lastMsg > 20000) {
    lastMsg = now;

    client.publish("sepp", String("Herr Winkla! \nIch bin nicht der Trache!!!").c_str(), true);

    digitalWrite(LED_BUILTIN, !alive);
  }
}

void subscribe_to_all_commands() {
  String prefix = "player/";
  String topic = "";

  topic = prefix + String(player1->id) + "/health";
  client.subscribe(topic.c_str());

  topic = prefix + String(player1->id) + "/die";
  client.subscribe(topic.c_str());

  topic = prefix + String(player1->id) + "/sound";
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
}

void health(String value) {
  player1->health = value.toInt();
  String answer = "Current Health: Over 9000!";
  Serial.println();
  Serial.print("New Player Health: ");
  Serial.print(player1->health);
  client.publish("answer", answer.c_str(), true);
}

void sound(String value) {
  player1->sound = value;
  String answer = "Never gonna sepp u up";
  Serial.println(answer);
  client.publish("answer", answer.c_str(), true);
}

void invalid_command() {
  String answer = "send me a valid command du hosnbrunza!";
  Serial.println(answer);
  client.publish("invalid command", answer.c_str(), true);
}

