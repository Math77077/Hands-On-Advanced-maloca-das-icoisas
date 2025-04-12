from flask import Flask, request, jsonify
from sklearn.neighbors import KNeighborsClassifier
import numpy as np
import joblib

knn = joblib.load("knn_jaundice_model.pkl")

app = Flask(__name__)

# Training data: [R, G, B, Label], where 1 = Jaundice
dados_treinamento = np.array([
    [0.9, 0.7, 0.5, 0],
    [0.8, 0.6, 0.4, 0],
    [0.7, 0.5, 0.3, 1],
    [0.6, 0.4, 0.2, 1],
    [0.85, 0.65, 0.45, 0],
    [0.65, 0.45, 0.25, 1]
])

X_train = dados_treinamento[:, :-1]
y_train = dados_treinamento[:, -1]

knn = KNeighborsClassifier(n_neighbors=3)
knn.fit(X_train, y_train)

@app.route('/rgb', methods=['POST'])
def classify_rgb():
    data = request.get_json()
    red = data.get("red", 0)
    green = data.get("green", 0)
    blue = data.get("blue", 0)

    # Normalize values
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
