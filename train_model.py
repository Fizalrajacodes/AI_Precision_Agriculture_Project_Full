import pandas as pd
import joblib
import requests
from sklearn.ensemble import RandomForestClassifier
import numpy as np

READ_API_KEY = "WG3K9LPAS7DBGDU7"
CHANNEL_ID = "3110245"

def get_data():
    url = f"https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds.json?api_key={READ_API_KEY}&results=200"
    data = requests.get(url).json()['feeds']
    df = pd.DataFrame(data)
    df = df[['field1','field2','field3']].astype(float)
    df.columns = ['temp','hum','soil']
    return df

df = get_data()
df['irrigation_needed'] = (df['soil'] < 400).astype(int)

model = RandomForestClassifier()
model.fit(df[['temp','hum','soil']], df['irrigation_needed'])
joblib.dump(model, 'models/irrigation_model.joblib')
print("✅ Model trained and saved.")
