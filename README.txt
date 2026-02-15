AI-Driven Precision Agriculture Project - Full Code
====================================================

Contents:
- esp32/esp32_sensor.ino         -- Arduino sketch for ESP32 (DHT22 + analog sensors)
- backend/app.py                 -- Flask backend with /ingest, /latest, /history endpoints
- train_model.py                 -- Generate synthetic data and train RandomForest
- frontend/index.html            -- Dashboard using Chart.js
- requirements.txt               -- Python dependencies
- README.txt                     -- This file

Quick start:
1. Edit esp32/esp32_sensor.ino: set WIFI_SSID, WIFI_PASS, BACKEND_URL to your backend address.
2. Backend:
   - Create a Python venv: python -m venv venv && source venv/bin/activate (or venv\Scripts\activate on Windows)
   - pip install -r requirements.txt
   - python train_model.py  # creates models/irrigation_model.joblib
   - python backend/app.py
3. Frontend:
   - Open frontend/index.html in a browser (or serve via python -m http.server in the frontend folder)
4. Run ESP32 (or Wokwi) to start sending data.

Notes & next steps:
- For production use MQTT + TLS or HTTPS.
- Replace simulated NPK with a real sensor and calibrate.
- Consider adding authentication and rate-limiting to the backend.
