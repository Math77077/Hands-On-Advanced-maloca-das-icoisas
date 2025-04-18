import streamlit as st
import requests
import matplotlib.pyplot as plt
from datetime import datetime
from streamlit_autorefresh import st_autorefresh

st.set_page_config(layout="wide")

# Atualiza a cada 15 segundos
st_autorefresh(interval=5000, key="autorefresh")

st.markdown(
    "<h1 style='text-align: center; color: #4CAF50;'>Sistema de Avaliação de Icterícia</h1>",
    unsafe_allow_html=True
)

st.markdown("### 👤 Dados do Paciente")
col1, col2, col3 = st.columns(3)
with col1:
    st.markdown("**Nome:**")
    st.write("João da Silva")
with col2:
    st.markdown("**Idade:**")
    st.write("70 anos")
with col3:
    st.markdown("**Condição:**")
    st.write("Estável")

st.markdown("---")
st.markdown("### 📊 Monitoramento em Tempo Real")

if "historico" not in st.session_state:
    st.session_state.historico = []

# 👉 Busca dados reais do Flask (GET)
def obter_dados_do_flask():
    try:
        response = requests.get("http://localhost:5000/dados")
        if response.status_code == 200:
            dados = response.json()
            if "result" in dados:  # Verifica se já há dados recebidos
                dados["data"] = datetime.now().strftime("%H:%M:%S")
                return dados
        return None
    except Exception as e:
        st.error(f"Erro ao conectar ao Flask: {e}")
        return None

dados_recebidos = obter_dados_do_flask()

if dados_recebidos:
    st.session_state.historico.append(dados_recebidos)

    col1, col2 = st.columns(2)
    with col1:
        st.metric("Classificação", dados_recebidos["result"])
    with col2:
        st.metric("Batimentos Cardíacos", f"{dados_recebidos['batimento']} BPM")

    historico = st.session_state.historico
    tempos = [d["data"] for d in historico]
    bpm = [d["batimento"] for d in historico]
    labels = [d["label"] for d in historico]

    col1, col2 = st.columns(2)
    with col1:
        st.markdown("#### Batimentos Cardíacos")
        plt.figure(figsize=(10, 5))
        plt.plot(tempos, bpm, marker='o', color='blue')
        plt.xlabel("Tempo")
        plt.ylabel("BPM")
        plt.title("Batimentos Cardíacos")
        plt.grid(True)
        st.pyplot(plt)

    with col2:
        st.markdown("#### Classificação de Icterícia")
        plt.figure(figsize=(10, 5))
        plt.plot(tempos, labels, marker='o', color='orange')
        plt.xlabel("Tempo")
        plt.ylabel("Classificação")
        plt.yticks([0, 1], ["Normal", "Icterícia"])
        plt.grid(True)
        st.pyplot(plt)

    st.markdown("### 📋 Histórico Completo")
    st.dataframe(historico[::-1])
else:
    st.warning("Aguardando dados do servidor Flask...")
