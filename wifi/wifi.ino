#include <WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "DB Shop"
#define wifi_password "Schmatzne"

#define mqtt_server "192.168.35.135"
#define mqtt_user "schwenglon"
#define mqtt_password "schwengl"



enum class Command {
  DIE,
  HEALTH,
  SOUND,
  INVALID
};

class Player {
private:
  inline static int count = 0;
public:
  int id;
  int health;
  Player() {
    health = 100;
    id = count++;
  }
};
void subscribe_to_all_players(Player (&players)[4]);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(LED_BUILTIN, OUTPUT);

  Player* player1 = new Player();
  Player* player2 = new Player();
  Player* player3 = new Player();
  Player* player4 = new Player();

  client.subscribe("schwenglon");
  client.setCallback(callback);
  delay(5000);
  Player players[] = { *player1, *player2, *player3, *player4 };

  subscribe_to_all_players(players);
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
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("quantumschmuh")) {
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

  // send_position(String("Aktuelle Position: bin bei deiner mama!"));
}

Command resolve_command(char* topic) {
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

void callback(char* topic, byte* payload, unsigned int length) {

  // switch (resolve_command(topic)) {
  //   case Command::DIE:
  //     {
  //       die();
  //       break;
  //     }
  //   case Command::HEALTH:
  //     {
  //       health();
  //       break;
  //     }
  //   case Command::SOUND:
  //     {
  //       sound();
  //       break;
  //     }
  //   case Command::INVALID:
  //     {
  //       invalid_command();
  //       break;
  //     }
  // }
  // Serial.print("Message arrived [");
  Serial.print("By Schmattes");
  Serial.print(topic);
  Serial.println();
  // Serial.print("] ");
  // String back = "";
  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)payload[i]);
  //   back += (char)payload[i];
  // }
  // // client.publish("zurueck", back.c_str(), true);
  // Serial.println();
  // Republish the received message
  // handle received message
}

void send_position(String position) {
  client.publish("position", position.c_str(), true);
}

void die() {
  String answer = "I am dying.";
  Serial.println(answer);
  client.publish("answer", answer.c_str(), true);
}

void health() {
  String answer = "Current Health: Over 9000!";
  Serial.println(answer);
  client.publish("answer", answer.c_str(), true);
}

void sound() {
  String answer = "Never gonna sepp u up";
  Serial.println(answer);
  client.publish("answer", answer.c_str(), true);
}

void invalid_command() {
  String answer = "send me a valid command du hosnbrunza!";
  Serial.println(answer);
  client.publish("invalid command", answer.c_str(), true);
}

void subscribe_to_all_players(Player (&players)[4]) {
  String prefix = "player/";
  String topic = "";

  for (Player player : players) {
    
    topic = prefix + String(player.id) + "/health";
    client.subscribe(topic.c_str());

    topic = prefix + String(player.id) + "/die";
    client.subscribe(topic.c_str());

    topic = prefix + String(player.id) + "/sound";
    client.subscribe(topic.c_str());
  }
}
