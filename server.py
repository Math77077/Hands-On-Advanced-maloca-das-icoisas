from flask import Flask, request, jsonify
from sklearn.neighbors import KNeighborsClassifier
import pandas as pd
import numpy as np

app = Flask(__name__)

# 🔁 Carrega os dados do CSV
def carregar_dados_de_treinamento(csv_path='dados_ictericia.csv'):
    try:
        df = pd.read_csv(csv_path)
        X = df[['R', 'G', 'B']].values
        y = df['label'].values
        return X, y
    except Exception as e:
        print("Erro ao carregar CSV:", e)
        return None, None

# Treina o modelo a partir do CSV
X_train, y_train = carregar_dados_de_treinamento()
knn = KNeighborsClassifier(n_neighbors=3)
knn.fit(X_train, y_train)

# Store last received data globally
ultimo_dado = {}

@app.route('/data', methods=['POST'])
def receber_dados():
    global ultimo_dado

    data = request.get_json()
    if not all(k in data for k in ("red", "green", "blue", "heartbeat")):
        return jsonify({"error": "Dados incompletos"}), 400

    r = data['red']
    g = data['green']
    b = data['blue']
    heartbeat = data['heartbeat']

    # Normalização (ESP32 envia RGB bruto 0–255)
    r_norm = r / 255.0
    g_norm = g / 255.0
    b_norm = b / 255.0

    entrada = [r_norm, g_norm, b_norm]
    resultado = knn.predict([entrada])[0]
    status = "Icterícia Detectada" if resultado == 1 else "Pele Normal"

    # Salva o último dado para o GET
    ultimo_dado = {
        "jaundice": int(resultado),
        "status": status,
        "heartbeat": heartbeat
    }

    print(f"RGB: {r}, {g}, {b} | Heartbeat: {heartbeat} | Resultado: {status}")
    return jsonify(ultimo_dado)

@app.route('/data', methods=['GET'])
def fornecer_dados():
    global ultimo_dado
    if ultimo_dado:
        return jsonify(ultimo_dado)
    else:
        return jsonify({"message": "Sem dados ainda"}), 204


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
