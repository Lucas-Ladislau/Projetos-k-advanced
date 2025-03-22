import streamlit as st
import requests
import pandas as pd
import plotly.express as px
import time

API_URL = "http://192.168.1.76:5000/dados"

st.title("Monitor de ECG - AD8232 com ESP32")

# Função para buscar os dados do servidor
def get_ecg_data():
    response = requests.get(API_URL)
    if response.status_code == 200:
        return response.json()
    return {}

# Criar o espaço para o gráfico
chart_placeholder = st.empty()
user_id_placeholder = st.empty()  # Para exibir o ID do usuário

last_user_id = None  # Variável para armazenar o ID do usuário anterior
ecg_data_buffer = []  # Variável para armazenar os dados do ECG enquanto atualiza

while True:
    ecg_data = get_ecg_data()

    if ecg_data:
        # Obtendo o ID do usuário atual
        current_user_id = ecg_data.get("usuario")

        # Verifica se houve mudança no ID do usuário
        if current_user_id != last_user_id:
            last_user_id = current_user_id  # Atualiza o ID do usuário
            ecg_data_buffer = []  # Limpa os dados antigos do ECG

        # Adiciona os dados do ECG ao buffer (sempre que chegar novos dados)
        if "ecg" in ecg_data:
            # Acessa a chave "ecg" e pega apenas os valores de ECG
            new_ecg_values = [entry["ecg"] for entry in ecg_data["ecg"]]
            ecg_data_buffer.extend(new_ecg_values)

            # Mantém apenas os 100 valores mais recentes
            ecg_data_buffer = ecg_data_buffer[-100:]

        # Exibir o ID do usuário no dashboard
        user_id_placeholder.subheader(f"ID do Usuário: {current_user_id}")

        # Preparar os dados do ECG para plotar
        if ecg_data_buffer:
            df = pd.DataFrame(ecg_data_buffer, columns=["ECG"])  # Cria o DataFrame com os novos dados

            # Criar o gráfico
            fig = px.line(df, y="ECG", title=f"Sinal de ECG em Tempo Real (ID: {current_user_id})")
            
            # Gerar um identificador único baseado no tempo atual para evitar duplicação de ID
            unique_key = f"ecg_chart_{int(time.time() * 1000)}"
            
            # Passar o unique_key como parâmetro
            chart_placeholder.plotly_chart(fig, use_container_width=True, key=unique_key)

    time.sleep(1)  # Atualiza a cada 1 segundo
