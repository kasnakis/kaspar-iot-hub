// KasPar-IoT Universal ESP32 MQTT Client — εργοστασιακές προεπιλογές / factory defaults
// Δημόσιο template: ΜΗΝ βάζετε εδώ πραγματικά SSID, κωδικούς, tokens ή hostnames παραγωγής.
// Public template: do NOT commit real SSIDs, passwords, tokens or production hostnames.
// Όλες οι τιμές αλλάζουν από το μενού Serial και αποθηκεύονται στη συσκευή.
// Οι προεπιλογές ακολουθούν τον οδηγό README.md (v1.0.1-dev).
#pragma once

#define FIRMWARE_VERSION      "1.0.1-dev"
#define SERIAL_BAUD_RATE      115200
#define NVS_NAMESPACE         "univmqtt"
#define MAX_CERT_SIZE         16384
#define MAX_VIRTUAL_SENSORS   16

// Wi-Fi
#define DEFAULT_SSID          ""
#define DEFAULT_PASS          ""

// Δίκτυο / Network (0 = DHCP, 1 = static)
#define DEFAULT_STATIC        0
#define DEFAULT_IP            "192.168.1.50"
#define DEFAULT_GW            "192.168.1.1"
#define DEFAULT_MASK          "255.255.255.0"
#define DEFAULT_DNS           "1.1.1.1"
#define DEFAULT_DNS2          "8.8.8.8"

// Broker
#define DEFAULT_HOST          ""
#define DEFAULT_PORT          8883
#define DEFAULT_TLS           2      // 0 plain, 1 TLS insecure, 2 TLS + CA, 3 mTLS
#define DEFAULT_AUTH          1      // 0 anonymous, 1 user+password, 2 token as username
#define DEFAULT_USER          ""
#define DEFAULT_PASSWORD      ""
#define DEFAULT_KEEPALIVE     30
#define DEFAULT_RETRY         5
#define DEFAULT_TIMEOUT       5

// Topics ({device_id}, {device_key} αντικαθίστανται / are expanded)
#define DEFAULT_TOPIC         "devices/{device_id}/telemetry"
#define DEFAULT_STATUS        ""
#define DEFAULT_SUB           ""
#define DEFAULT_ONLINE        "online"
#define DEFAULT_OFFLINE       "offline"

// Τηλεμετρία / Telemetry
#define DEFAULT_INTERVAL      60
#define DEFAULT_QOS           1
#define DEFAULT_RETAIN        0
#define DEFAULT_FORMAT        0      // 0 flat JSON, 1 KasPar, 2 timestamped values, 3 custom template
#define DEFAULT_TEMPLATE      "{\"temperature\":{temperature},\"humidity\":{humidity}}"

// NTP (απαιτείται για επαλήθευση TLS / required for TLS validation)
#define DEFAULT_NTP           "pool.ntp.org"
#define DEFAULT_NTP2          "time.nist.gov"
