# app.py
from flask import Flask, jsonify
import requests

app = Flask(__name__)

THINGSPEAK_URL = "https://api.thingspeak.com/channels/YOUR_CHANNEL_ID/feeds.json?api_key=YOUR_READ_API_KEY&results=10"

@app.route("/data", methods=["GET"])
def get_data():
    try:
        response = requests.get(THINGSPEAK_URL)
        data = response.json()
        feeds = data["feeds"]
        formatted = [{
            "temperature": f.get("field1"),
            "humidity": f.get("field2"),
            "soil": f.get("field3"),
            "light": f.get("field4")
        } for f in feeds]
        return jsonify(formatted)
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    app.run(debug=True)
