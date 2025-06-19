#ifndef EDGE_SERVER_CLIENT_H
#define EDGE_SERVER_CLIENT_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class EdgeServerClient
{
private:
    const char *ssid;
    const char *password;
    const char *mqtt_server;
    uint16_t mqtt_port;
    const char *mqtt_user;
    const char *mqtt_password;

    WiFiClientSecure espClient;
    PubSubClient mqttClient;

    bool isProvisioned = false;

    void connectToWiFi();
    void reconnectMQTT();

public:
    EdgeServerClient(const char *ssid, const char *password, const char *mqtt_server, uint16_t mqtt_port, const char *mqtt_user = nullptr, const char *mqtt_password = nullptr);

    void setup();
    void loop();
    void publish(const char *topic, const char *payload);
    void subscribe(const char *topic);
    void unsubscribe(const char *topic);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    String getMacAddress();
    String generateClientId();
    bool isConnected();
    void setIsProvisioned(bool provisioned);
};

#endif
