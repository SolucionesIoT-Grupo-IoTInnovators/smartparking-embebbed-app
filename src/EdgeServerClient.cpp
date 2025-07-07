#include "EdgeServerClient.h"

EdgeServerClient::EdgeServerClient(const char *ssid, const char *password, const char *mqtt_server, uint16_t mqtt_port, const char *mqtt_user, const char *mqtt_password)
    : ssid(ssid), password(password), mqtt_server(mqtt_server), mqtt_port(mqtt_port), mqtt_user(mqtt_user), mqtt_password(mqtt_password), mqttClient(espClient)
{
}

void EdgeServerClient::connectToWiFi()
{
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");
}

void EdgeServerClient::reconnectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (mqttClient.connect(generateClientId().c_str(), mqtt_user, mqtt_password))
        {
            Serial.println("connected");

            // Subscribe to topics after successful connection
            if (isProvisioned)
            {
                mqttClient.subscribe("parking/reservas");
            }
            else
            {
                mqttClient.subscribe("provisioning/response");
            }
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void EdgeServerClient::setup()
{
    connectToWiFi();
    espClient.setInsecure();
    mqttClient.setServer(mqtt_server, mqtt_port);
}

void EdgeServerClient::loop()
{
    if (!mqttClient.connected())
    {
        reconnectMQTT();
    }
    mqttClient.loop();
}

void EdgeServerClient::publish(const char *topic, const char *payload)
{
    if (mqttClient.publish(topic, payload, 1))
    {
        Serial.printf("Message published to topic %s: %s\n", topic, payload);
    }
    else
    {
        Serial.printf("Failed to publish message to topic %s\n", topic);
    }
}

void EdgeServerClient::subscribe(const char *topic)
{
    if (mqttClient.subscribe(topic))
    {
        Serial.printf("Subscribed to topic %s\n", topic);
    }
    else
    {
        Serial.printf("Failed to subscribe to topic %s\n", topic);
    }
}

void EdgeServerClient::unsubscribe(const char *topic)
{
    mqttClient.unsubscribe(topic);
}

void EdgeServerClient::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    mqttClient.setCallback(callback);
}

String EdgeServerClient::getMacAddress()
{
    return WiFi.macAddress();
}

String EdgeServerClient::generateClientId()
{
    return "ESP32_" + getMacAddress().substring(9) + "_" + String(random(0xffff), HEX);
}

bool EdgeServerClient::isConnected()
{
    return mqttClient.connected();
}

void EdgeServerClient::setIsProvisioned(bool provisioned)
{
    isProvisioned = provisioned;
}