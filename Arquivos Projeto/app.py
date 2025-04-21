import streamlit as st
import requests
import matplotlib.pyplot as plt
from datetime import datetime
from streamlit_autorefresh import st_autorefresh

st.set_page_config(layout="wide")

# 🔁 Atualiza a cada 5 segundos
st_autorefresh(interval=5000, key="autorefresh")

st.markdown(
    "<h1 style='text-align: center; color: #00000;'>Sistema de Avaliação de Icterícia</h1>",
    unsafe_allow_html=True
)
st.markdown("---")

st.markdown("""
            <style>
            .box {
                width: 30%;  /* Largura do quadrado (pode ajustar conforme necessário) */
                margin: 0 auto;  /* Centraliza horizontalmente */
                padding: 20px;  /* Espaçamento interno */
                border: 2px solid #FFFFFF;  /* Cor e espessura da borda */
                border-radius: 10px;  /* Bordas arredondadas */
                background-color: transparent;  /* Cor de fundo do quadrado */
                text-align: center;  /* Centraliza o texto */
            }
            </style>
            <div class = "box">
            <h3 style='text-align: center;'>👤Dados do Paciente:</h3>
            <p style='text-align = center;'><b>Nome</b>: João da Silva</p>
            <p style='text-align = center;'><b>Ala</b>: 2</p>
            <p style='text-align = center;'><b>Condição</b>: Estável</p>
            <p style='text-align = center;'><b>Tipo Sanguíneo</b>: A+</p>
            </div>
            """,unsafe_allow_html= True
            )
st.markdown("---")
st.markdown("### 📊 Monitoramento")

if "historico" not in st.session_state:
    st.session_state.historico = []

# 📡 Função para obter dados do servidor Flask
def obter_dados_do_flask():
    try:
        response = requests.get("http://localhost:5000/data")
        if response.status_code == 200:
            dados = response.json()
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
        st.metric("Classificação", dados_recebidos["status"])
    with col2:
        st.metric("Batimentos Cardíacos", f"{dados_recebidos['heartbeat']} BPM")

    historico = st.session_state.historico
    tempos = [d["data"] for d in historico]
    bpm = [d["heartbeat"] for d in historico]
    labels = [d["jaundice"] for d in historico]

    col1, col2 = st.columns(2)
    with col1:
        st.markdown("#### Batimentos Cardíacos")
        plt.figure(figsize=(10, 5))
        plt.plot(tempos, bpm, marker='o', color='red')
        plt.ylabel("BPM")
        plt.xticks([])
        plt.title("Batimentos Cardíacos")
        plt.grid(True)
        st.pyplot(plt)

    with col2:
        st.markdown("#### Classificação de Icterícia")
        plt.figure(figsize=(10, 5))
        plt.plot(tempos, labels, marker='o', color='blue')
        plt.xticks([])
        plt.ylabel("Classificação")
        plt.yticks([0, 1], ["Normal", "Icterícia"])
        plt.title("Classificação de Icterícia")
        plt.grid(True)
        st.pyplot(plt)

    st.markdown("### 📋 Histórico Completo")
    st.dataframe(historico[::-1])
else:
    st.warning("Aguardando dados do servidor Flask...")
