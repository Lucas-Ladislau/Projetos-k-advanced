from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

dados_ecg = []
usuario_atual = None  # Variável para armazenar o ID do usuário

@app.route('/dados', methods=['POST'])
def receber_dados():
    global dados_ecg, usuario_atual
    json_data = request.get_json()
    if "ecg" in json_data and "id" in json_data:
        dados_ecg.append({"ecg": json_data["ecg"], "id": json_data["id"]})
        usuario_atual = json_data["id"]  # Atualiza o ID do usuário
        if len(dados_ecg) > 100:  # Mantém os últimos 100 valores
            dados_ecg.pop(0)
    return jsonify({"status": "ok"})

@app.route('/dados', methods=['GET'])
def enviar_dados():
    return jsonify({"ecg": dados_ecg, "usuario": usuario_atual})  # Inclui o ID do usuário

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
