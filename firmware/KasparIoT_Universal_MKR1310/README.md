# KasPar-IoT Universal LoRaWAN Client 1.0.0-dev

Πειραματική έκδοση για Arduino MKR WAN 1310, ChirpStack v4, EU868, OTAA, Class A. Δεν απαιτείται Wi-Fi ή CA στη συσκευή. Συνδέστε κατάλληλη κεραία πριν την εκπομπή.

## Εγκατάσταση
1. Αποσυμπιέστε το ZIP. Ανοίξτε το ομώνυμο `.ino` στο Arduino IDE.
2. Εγκαταστήστε Arduino SAMD Boards, MKRWAN (Arduino), FlashStorage (Arduino-compatible SAMD). Επιλέξτε Arduino MKR WAN 1310 και σωστή θύρα.
3. Verify και Upload. Ανοίξτε Serial Monitor 115200, Newline. Πατήστε RESET ή στείλτε `setup`. Τα ελληνικά απαιτούν UTF-8.
4. Αν το modem δεν αρχικοποιείται, ελέγξτε κεραία, πλακέτα, βιβλιοθήκη και firmware modem. Μην αναβαθμίσετε modem χωρίς τις επίσημες οδηγίες Arduino.

## ChirpStack
- Βεβαιωθείτε ότι η gateway είναι online και προωθεί EU868 στον ChirpStack.
- Δημιουργήστε Device Profile για EU868, Class A, OTAA, LoRaWAN 1.0.x με MAC version και regional parameters συμβατά με το modem σας. Μην υποθέσετε συγκεκριμένη έκδοση χωρίς έλεγχο.
- Στο Codec επιλέξτε JavaScript και επικολλήστε το `ChirpStack_Decoder.js`.
- Δημιουργήστε Application και Device. Από το μενού `1` αντιγράψτε το DevEUI. Καταχωρίστε το ίδιο AppKey στον server και στη συσκευή. Το JoinEUI πρέπει να συμφωνεί με τις απαιτήσεις του δικτύου.

## Μενού και παράδειγμα
`2`: JoinEUI (16 HEX), AppKey (32 HEX). Ενδεικτικό JoinEUI `0102030405060708`, AppKey `00112233445566778899AABBCCDDEEFF` μόνο για απομονωμένη δοκιμή, όχι παραγωγή.

`3`: εισάγετε διαδοχικά 300 (δευτερόλεπτα), 10 (FPort), 1 (ADR), 0 (unconfirmed). Το διάστημα είναι κατώτατο όριο της εφαρμογής, όχι εγγύηση συμμόρφωσης duty cycle. Χρησιμοποιήστε μεγαλύτερο διάστημα αν απαιτείται από το δίκτυο ή το airtime.

`4`: θέση 1, temperature, 15, 35, 1. Θέση 2, humidity, 30, 80, 1. Έως 8 αισθητήρες, τυχαίες τιμές, δύο δεκαδικά. Κλειδιά ASCII έως 16 χαρακτήρες. Για binary αλλάξτε και τον πίνακα names του decoder όταν αλλάζετε ονόματα θέσεων.

`5`: 0 για binary (προτεινόμενο), 1 για JSON. Το όριο 51 bytes είναι συντηρητικό όριο εφαρμογής για EU868, όχι καθολικό όριο LoRaWAN· το πραγματικό διαθέσιμο payload εξαρτάται από data rate και MAC overhead.

`s`: αποθήκευση στη flash και επανεκκίνηση. `j`: χειροκίνητο Join. Ελέγξτε Join events στον ChirpStack. `p`: δοκιμαστική αποστολή. `x`: έξοδος από μενού και περιοδική τηλεμετρία. Μετά από reset απαιτείται νέο χειροκίνητο Join και x. `i`: προβολή ρυθμίσεων με κρυμμένο AppKey. `r`: επανεκκίνηση χωρίς αποθήκευση. `e`: εργοστασιακές ρυθμίσεις με επιβεβαίωση ERASE. `h` ή `setup`: μενού. Οι ερωτήσεις περιμένουν Newline και είναι blocking.

## Payload
Binary: byte έκδοσης 01, πλήθος, ανά αισθητήρα ID (1–8) και signed int32 big endian, τιμή ×100. Παράδειγμα 23.50 και 64.20: `01 02 01 00 00 09 2E 02 00 00 19 14`. Decoder: temperature 23.5, humidity 64.2. JSON: `{"temperature":23.50,"humidity":64.20}`. Ο decoder αναγνωρίζει JSON από το αρχικό `{`.

## Διασύνδεση με kaspar-iot.gr
MKR → gateway → ChirpStack → MQTT integration → ενδιάμεση υπηρεσία → KasPar-IoT. Στον ChirpStack v4 το συνήθες topic είναι `application/<applicationId>/device/<devEui>/event/up` (ελέγξτε την πραγματική ρύθμιση). Η υπηρεσία διαβάζει το αποκωδικοποιημένο `object`, αντιστοιχίζει DevEUI σε Device Key, και μετασχηματίζει στο πραγματικό schema KasPar-IoT. Ενδεικτικό topic προορισμού `kaspar/v1/devices/<key>/telemetry`. Χρησιμοποιεί τον πραγματικό broker, θύρα, token και CA του KasPar-IoT. Η θύρα 18883 ήταν σε προηγούμενο παράδειγμα, όχι επιβεβαιωμένο endpoint. Το hostname πρέπει να συμφωνεί με το πιστοποιητικό. Η υπηρεσία πρέπει να χειρίζεται sequence, timestamp, retries και διπλότυπα σύμφωνα με το συμβόλαιο της πλατφόρμας. Δεν περιλαμβάνεται λειτουργικός bridge ούτε επιβεβαιωμένη σύνδεση με την πλατφόρμα.

Για ThingsBoard, αντίστοιχη υπηρεσία δημοσιεύει στο `v1/devices/me/telemetry` με Access Token ως MQTT username και επίπεδο JSON. Για Mosquitto, μπορείτε να παρακολουθείτε το integration topic με MQTT subscriber, χρησιμοποιώντας τα πραγματικά credentials και TLS όπου απαιτείται.

## Ασφάλεια / περιορισμοί
AppKey στη flash χωρίς κρυπτογράφηση από το sketch. Το terminal μπορεί να εμφανίσει εισαγόμενα μυστικά. Μην χρησιμοποιείτε παραγωγικά κλειδιά σε κοινόχρηστες πλακέτες. Δεν υλοποιούνται ABP, αλλαγή region, Cayenne LPP, deep sleep, αυτόματο rejoin, downlink εντολές ή μόνιμη ουρά. Τα downlinks εμφανίζονται μόνο ως bytes όταν είναι διαθέσιμα μετά από uplink. Confirmed uplink δεν εγγυάται παραλαβή από την τελική πλατφόρμα. Η επαναφορά αφορά τις ρυθμίσεις του sketch, όχι απαραίτητα την κατάσταση modem. Δεν έχει πραγματοποιηθεί compile ή δοκιμή σε hardware. Ελέγξτε ειδικά συμβατότητα MKRWAN API, OTAA nonce/session handling και duty cycle πριν από χρήση.

## Έλεγχοι αποδοχής
- Verify / Upload, DevEUI, επανεκκίνηση και διατήρηση ρυθμίσεων.
- Join event, uplink event, decoded object, πραγματική παραλαβή στον προορισμό.
- Δοκιμή αρνητικής τιμής, αλλαγής αισθητήρα, JSON, αποτυχίας Join και factory reset.
