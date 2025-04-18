from flask import Flask, request, jsonify
from sklearn.neighbors import KNeighborsClassifier
import numpy as np
import joblib

knn = joblib.load("knn_jaundice_model.pkl")

app = Flask(__name__)

@app.route('/rgb', methods=['POST'])
def classify_rgb():
    data = request.get_json()
    red = data.get("red", 0)
    green = data.get("green", 0)
    blue = data.get("blue", 0)

    r_norm = red / 255.0
    g_norm = green / 255.0
    b_norm = blue / 255.0

    prediction = knn.predict([[r_norm, g_norm, b_norm]])
    label = int(prediction[0])

    return jsonify({
        "result": "Ictericia Detectada" if label == 1 else "Pele Normal",
        "label": label
    })

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
