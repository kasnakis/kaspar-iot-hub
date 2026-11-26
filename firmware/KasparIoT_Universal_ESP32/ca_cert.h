// Προαιρετικό ενσωματωμένο CA (PEM). Κενό = χωρίς ενσωματωμένο CA· εισάγετε το CA από το μενού (5 → 5).
// Optional built-in CA (PEM). Empty = none; import your CA from the serial menu (5 → 5).
// Για τη δική σας εγκατάσταση KasPar-IoT κατεβάστε το CA από τη σελίδα «Συνδεσιμότητα» της πλατφόρμας.
#pragma once

static const char BUILTIN_CA_PEM[] = "";

/* Παράδειγμα / Example:
static const char BUILTIN_CA_PEM[] = R"PEM(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
)PEM";
*/
