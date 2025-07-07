#include "ParkingSpot.h"
#include "EventHandler.h"
#include "CommandHandler.h"
#include "Sensor.h"
#include "Actuator.h"
#include <ArduinoJson.h>
#include "EdgeServerClient.h"
#include <LiquidCrystal_I2C.h>

#define THRESHOLD 40.0
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define MQTT_TOPIC_PROV_REQ "provisioning/request"
#define MQTT_TOPIC_PROV_RES "provisioning/response"
#define MQTT_TOPIC_RESERVAS "parking/reservas"
#define MQTT_TOPIC_ESTADOS "parking/estados"

// Configuración MQTT
const char *mqttServer = "effaed07cbea4a25bc4c84197d058fa8.s1.eu.hivemq.cloud";
const int mqttPort = 8883;
const char *mqttUser = "u20201c069";
const char *mqttPassword = "U20201c069";

bool isOccupied = false;
// bool needsReservasSubscription = false;

// Instancias de objetos
ParkingSpot parkingSpot;
EdgeServerClient edgeClient(WIFI_SSID, WIFI_PASSWORD, mqttServer, mqttPort, mqttUser, mqttPassword);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    // Convertir payload a String
    String message;
    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }

    Serial.print("Mensaje recibido [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(message);

    String topicStr = String(topic);
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, message);

    if (error)
    {
        Serial.println("Error al parsear JSON: " + String(error.c_str()));
        return;
    }

    if (topicStr == MQTT_TOPIC_PROV_RES)
    {
        if (doc["spotId"].isNull() || doc["label"].isNull() || doc["apiKey"].isNull())
        {
            Serial.println("Respuesta de provisioning incompleta");
            return;
        }

        parkingSpot.setSpotId(doc["spotId"].as<String>());
        parkingSpot.setDeviceLabel(doc["label"].as<String>());
        parkingSpot.setApiKey(doc["apiKey"].as<String>());

        Serial.println("Provisioning exitoso:");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PARKING SPOT:");
        lcd.setCursor(0, 1);
        lcd.print(parkingSpot.getDeviceLabel());

        edgeClient.setIsProvisioned(true);
        edgeClient.unsubscribe(MQTT_TOPIC_PROV_RES);
    }
    else if (topicStr == MQTT_TOPIC_RESERVAS)
    {
        if (doc["spotId"].isNull() || doc["reserved"].isNull() || doc["apiKey"].isNull())
        {
            Serial.println("Mensaje de reserva incompleto");
            return;
        }

        String receivedSpotId = doc["spotId"].as<String>();
        String receivedApiKey = doc["apiKey"].as<String>();
        bool reserved = doc["reserved"].as<bool>();

        if (receivedSpotId != parkingSpot.getSpotId() || receivedApiKey != parkingSpot.getApiKey())
        {
            Serial.println("Comando ignorado - SpotId o ApiKey no coinciden");
            return;
        }

        Serial.println("Comando de reserva recibido: " + String(reserved ? "RESERVAR" : "LIBERAR"));
        parkingSpot.on(reserved ? ParkingSpot::RESERVED_SPOT_EVENT : ParkingSpot::UNRESERVED_SPOT_EVENT);
    }
}

void onStatusUpdate(unsigned long currentMillis)
{
    parkingSpot.measureDistance(THRESHOLD);
    if (!parkingSpot.getSpotId().isEmpty())
    {
        if (parkingSpot.getIsOccupied() != isOccupied)
        {
            isOccupied = parkingSpot.getIsOccupied();

            JsonDocument statusDoc;
            statusDoc["spotId"] = parkingSpot.getSpotId();
            statusDoc["occupied"] = isOccupied;
            statusDoc["apiKey"] = parkingSpot.getApiKey();
            statusDoc["timestamp"] = currentMillis;

            String statusMessage;
            serializeJson(statusDoc, statusMessage);
            edgeClient.publish(MQTT_TOPIC_ESTADOS, statusMessage.c_str());
        }
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("=== Inicializando Sistema de Parking IoT ===");

    edgeClient.setup();
    edgeClient.setCallback(mqttCallback);

    while (!edgeClient.isConnected())
    {
        Serial.println("Esperando conexión MQTT...");
        delay(1000);
        edgeClient.loop();
    }

    edgeClient.subscribe(MQTT_TOPIC_PROV_RES);
    edgeClient.subscribe(MQTT_TOPIC_RESERVAS);

    Serial.println("Solicitando provisioning al Edge Server...");
    JsonDocument provRequest;
    provRequest["mac"] = edgeClient.getMacAddress();

    String provRequestStr;
    serializeJson(provRequest, provRequestStr);
    edgeClient.publish(MQTT_TOPIC_PROV_REQ, provRequestStr.c_str());

    // Inicializar LCD
    lcd.init();
    lcd.backlight();

    String mac = edgeClient.getMacAddress();
    int mid = mac.length() / 2;

    lcd.setCursor(0, 0);
    lcd.print("MAC: ");
    lcd.setCursor(5, 0);
    lcd.print(mac.substring(0, mid));
    lcd.setCursor(5, 1);
    lcd.print(mac.substring(mid));

    isOccupied = parkingSpot.getIsOccupied();

    delay(1000);
}

void loop()
{
    edgeClient.loop();

    static unsigned long lastMeasurement = 0;
    unsigned long currentMillis = millis();

    if (currentMillis - lastMeasurement >= 1500) // Medir cada segundo
    {
        lastMeasurement = currentMillis;
        onStatusUpdate(currentMillis);
    }

    delay(500);
}