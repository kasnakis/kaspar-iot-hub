# Σύνδεση ThingsBoard → KasPar-IoT

Ισχύει για KasPar-IoT **1.5.0** και ThingsBoard **CE, PE ή Cloud**. Η τηλεμετρία συσκευών που ήδη στέλνουν σε ένα
ThingsBoard φτάνει και στην KasPar-IoT, με έναν κόμβο **REST API Call** σε ένα rule chain. Η KasPar-IoT δεν χρειάζεται
χρήστη ή κωδικό του ThingsBoard: το ThingsBoard αποφασίζει τι στέλνει.

Για την αντίθετη κατεύθυνση (μετρήσεις της KasPar-IoT προς ThingsBoard, ως συσκευή gateway) δείτε το βιβλίο χρήσης του
διαχειριστή οργανισμού, ενότητα 3.5 ([PDF](../KasPar-IoT_Tenant_Admin_Manual_GR.pdf)).

| Αρχείο | Τι είναι |
|---|---|
| [`kaspar-iot-forward-rule-chain.json`](kaspar-iot-forward-rule-chain.json) | Rule chain για εισαγωγή στο ThingsBoard: φίλτρο συσκευών → Script → REST API Call |
| [`kaspar-iot-envelope.tbel`](kaspar-iot-envelope.tbel) | Το script (TBEL) μόνο του, αν προτιμάτε να φτιάξετε τους κόμβους με το χέρι |

## 1. Στην KasPar-IoT: νέα σύνδεση

1. **Συσκευές → ThingsBoard → Λήψη από ThingsBoard → Νέα σύνδεση.** Όνομα, προφίλ «ThingsBoard · JSON» (προεπιλογή)
   και **ζώνη ώρας** για μηνύματα με ώρα χωρίς ζώνη (προεπιλογή `Europe/Athens`).
2. Η σελίδα δείχνει τη **διεύθυνση** της σύνδεσης (`…/integrations/thingsboard/v1/<connection id>`) και το **token**.
   Το token εμφανίζεται **μία φορά**: αντιγράψτε το τώρα. Ένα νέο token ακυρώνει αμέσως το προηγούμενο.

## 2. Στο ThingsBoard: το rule chain

**Εισαγωγή:** Rule chains → «+» → **Import rule chain** → `kaspar-iot-forward-rule-chain.json`. Ανοίξτε τον κόμβο
**KasPar-IoT** (REST API Call) και αλλάξτε:

| Πεδίο | Τιμή |
|---|---|
| Endpoint URL pattern | η διεύθυνση της σύνδεσης από την KasPar-IoT (στη θέση του `https://<KASPAR-HOST>:<PORT>/integrations/thingsboard/v1/<CONNECTION-ID>`) |
| Header `Authorization` | `Bearer ` + το token (στη θέση του `<TOKEN>`) |
| Read timeout | `10000` (ms) |

Αποθηκεύστε. Μετά, στο **Root Rule Chain**: από τον κόμβο **Message Type Switch**, έξοδος **Post telemetry**,
προσθέστε έναν κόμβο **Rule chain** που δείχνει στο «KasPar-IoT forward». Ο υπάρχων κόμβος «Save Timeseries» μένει
όπως είναι: το ThingsBoard συνεχίζει να κρατά τις τιμές του.

```
Message Type Switch --Post telemetry--> [Rule chain: KasPar-IoT forward]
    entity type filter (Device) --True--> Script (TBEL) --Success--> REST API Call
```

Γιατί το φίλτρο: μηνύματα που δεν προέρχονται από συσκευή (π.χ. τα στατιστικά ενός ThingsBoard Edge κάθε 10 λεπτά)
δεν έχουν όνομα συσκευής και η KasPar-IoT τα απορρίπτει με «no device name».

**Χωρίς εισαγωγή:** φτιάξτε τους τρεις κόμβους με το χέρι. Ο κόμβος Script είναι τύπου **Transformation**, γλώσσα
**TBEL**, με το περιεχόμενο του [`kaspar-iot-envelope.tbel`](kaspar-iot-envelope.tbel).

## 3. HTTPS με την ιδιωτική CA της KasPar-IoT

Μια εγκατάσταση KasPar-IoT χρησιμοποιεί συνήθως πιστοποιητικά από τη δική της CA. Ένα ThingsBoard PE ή Cloud δεν
εμπιστεύεται αυτή τη CA και δεν μπορείτε να την εγκαταστήσετε στον server του. Η λύση, χωρίς δημόσιο πιστοποιητικό:

1. Στον κόμβο REST API Call → **Credentials: PEM Certificate**.
2. **Server CA**: το `kaspar-iot-ca.crt` (Σύνδεση συσκευών → «Πιστοποιητικό της CA (δημόσιο)»· δεν περιέχει κανένα κλειδί).
3. **Client certificate / Client key**: οποιοδήποτε ζεύγος, π.χ. αυτοϋπογεγραμμένο. Η KasPar-IoT δεν ζητά
   πιστοποιητικό πελάτη, οπότε δεν ελέγχεται· η φόρμα του ThingsBoard όμως το απαιτεί:

   ```
   openssl req -x509 -newkey rsa:2048 -nodes -days 3650 -subj "/CN=thingsboard-client" -keyout client.key -out client.crt
   ```

4. Η διεύθυνση είναι `https://<όνομα ή IP της εγκατάστασης>:<θύρα HTTPS>`, με το όνομα ή την IP που υπάρχει στα
   πιστοποιητικά της εγκατάστασης (Διαχείριση πλατφόρμας → Πιστοποιητικά).

Για ThingsBoard στο ίδιο τοπικό δίκτυο αρκεί `http://`, με `Credentials: Anonymous`. Χωρίς TLS το token ταξιδεύει
χωρίς κρυπτογράφηση: μόνο για τοπικό δίκτυο.

## 4. Νέες συσκευές και έγκριση

Το πρώτο μήνυμα κάθε συσκευής του ThingsBoard τη βάζει στις **Νέες συσκευές σε αναμονή**, με τα κλειδιά και την
τελευταία τιμή τους. Τα μηνύματα μιας συσκευής σε αναμονή **κρατιούνται** (έως 7 ημέρες και 1000 μηνύματα, ρυθμίζεται
από τον διαχειριστή πλατφόρμας) και αποθηκεύονται με την έγκριση, με τη σειρά τους.

**Έγκριση** → σταθμός, όνομα, κλειδί συσκευής και, για κάθε κλειδί του μηνύματος: χρονοσειρά του σταθμού, **νέα
χρονοσειρά** (παράμετρος και μονάδα) ή «χωρίς κανάλι». Όλα γίνονται μαζί. **Σύνδεση με υπάρχουσα** δένει μια
μετονομασμένη συσκευή με αυτή που υπάρχει ήδη. **Αγνόηση** σβήνει και τα κρατημένα μηνύματα.

## 5. Μορφές μηνυμάτων

Το ίδιο script καλύπτει όλες τις μορφές:

| Μορφή | Παράδειγμα τηλεμετρίας |
|---|---|
| Απλές τιμές | `{"temperature": 21.5, "humidity": 70}` |
| Με χρόνο | `{"ts": 1795345166592, "values": {"temperature": 21.5}}` ή πίνακας από αυτά |
| Εμφωλευμένες τιμές | `{"sensors": {"air_temp": 14.2}}` → κλειδί `sensors.air_temp` (έως 3 επίπεδα) |
| Datalogger | `{"channels": [{"code": "WL", "name": "Water level", "unit": "m"}], "data": [{"$ts": 261124120000, "WL": 0.359}]}` |

- Ο πίνακας εγγραφών μπορεί να λέγεται `data`, `records` ή `samples` και ο χρόνος `$ts`, `ts`, `time` ή `timestamp`.
- Χρόνος: epoch σε ms ή s, ISO 8601, ή συμπαγής τοπική ώρα `ΕΕΜΜΗΗωωλλδδ` (12 ψηφία, π.χ. `261124120000` =
  24/11/2026 12:00:00) ή `ΕΕΕΕΜΜΗΗωωλλδδ` (14). Μια ώρα χωρίς ζώνη παίρνει τη ζώνη του προφίλ της συσκευής ή της
  σύνδεσης· η θερινή ώρα λογαριάζεται σωστά.
- Τα `channels` δεν γίνονται τιμές: προτείνουν όνομα και μονάδα στην έγκριση. Κενές εγγραφές `{}` αγνοούνται.
- Ένα μήνυμα χωρίς τιμές φαίνεται στη λίστα αναμονής όπως ήρθε, με την αιτία.

## 6. Επαναλήψεις και απώλειες

- Αν η KasPar-IoT δεν απαντήσει, το ThingsBoard ξαναστέλνει σύμφωνα με τη στρατηγική της ουράς του rule engine
  (Queues → Main → Processing strategy, π.χ. «Retry failed and timed-out»). Με «Skip all failures» ένα μήνυμα που
  αποτυγχάνει χάνεται.
- Ένα μήνυμα που ξανάρχεται μέσα σε 24 ώρες αναγνωρίζεται ως επανάληψη και δεν γράφεται δεύτερη φορά (όταν όλες οι
  εγγραφές του έχουν χρόνο).

## 7. Απαντήσεις και συνηθισμένα λάθη

| Απάντηση | Σημασία | Τι κάνετε |
|---|---|---|
| 200 `queued` / `duplicate` / `pending-approval` / `ignored` | δεκτό, επανάληψη, νέα συσκευή σε αναμονή, συσκευή ανενεργή ή μήνυμα χωρίς τιμές | — |
| 401 | λάθος token ή διεύθυνση | ελέγξτε `Bearer <token>` και το id της σύνδεσης |
| 403 | η σύνδεση είναι απενεργοποιημένη | ενεργοποίηση στην KasPar-IoT |
| 413 | μήνυμα πάνω από 64 KB | μικρότερα μηνύματα |
| 422 | το μήνυμα δεν διαβάζεται (η αιτία στη σελίδα) | π.χ. «no device name»: λείπει το φίλτρο Device ή το Script |
| 429 | πάνω από το όριο ρυθμού | το ThingsBoard ξαναστέλνει μετά το `Retry-After` |
| Timeout | πολύ μικρό Read timeout | `10000` ms |
| SSL / PKIX | HTTPS χωρίς τη CA | Credentials: PEM Certificate (ενότητα 3) |

Συνηθισμένα λάθη στη ρύθμιση: κόμβος **Filter** αντί για **Transformation** στο Script· `https://` σε θύρα χωρίς TLS·
Read timeout `1000` αντί για `10000`.

## Ασφάλεια

- Μην βάζετε το token σε screenshots, εξαγωγές rule chain που μοιράζεστε ή δημόσια repositories. Αν εκτεθεί, βγάλτε
  νέο token για τη σύνδεση στην KasPar-IoT και αλλάξτε το στον κόμβο REST API Call.
- Μια εξαγωγή του rule chain σας περιέχει και τη διεύθυνση της εγκατάστασης: ελέγξτε την πριν τη στείλετε.
- Ο φάκελος αυτός δεν περιέχει κλειδιά ή διευθύνσεις: μόνο placeholders (`<KASPAR-HOST>`, `<PORT>`, `<CONNECTION-ID>`,
  `<TOKEN>`).
