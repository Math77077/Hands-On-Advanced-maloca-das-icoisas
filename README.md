# 👶💡 Detector de Icterícia Neonatal — Nexora

Este repositório contém o projeto desenvolvido pela **Equipe 14 - Nexora**, como parte da fase **Hands-on Avançado** do programa **Maloca das ICoisas**, promovido pela **Universidade Federal de Roraima (UFRR)** com **patrocínio da Softex**.

Nosso objetivo foi criar uma solução **acessível, portátil e inteligente** para a **detecção precoce da icterícia em recém-nascidos**, usando hardware embarcado, aprendizado de máquina e uma dashboard interativa.  
**Este projeto está finalizado.**

---

## 🩺 Visão Geral

A **icterícia neonatal** afeta a maioria dos recém-nascidos e pode, se não for tratada a tempo, levar a complicações graves como kernicterus e danos neurológicos. Pensando nisso, desenvolvemos um **dispositivo baseado em ESP32** que analisa **a coloração da pele e os sinais vitais do bebê** para indicar possíveis casos de icterícia de forma automatizada.

---

## 🔧 Tecnologias e Componentes

- **ESP32** – Microcontrolador principal
- **TCS3200** – Sensor de cor para análise da tonalidade da pele
- **AD8232** – Sensor de batimentos cardíacos
- **KNN (Machine Learning)** – Para classificar a condição da pele (com ou sem icterícia)
- **Streamlit** – Interface Web com visualização em tempo real
- **CallMeBot API** – Envio de notificações automáticas sobre a condição do bebê via WhatsApp

---

## 📊 Funcionalidades

- Detecção da coloração da pele em tempo real
- Monitoramento da frequência cardíaca
- Interface com histórico individual de cada paciente (via dashboard)
- Visualização de dados em gráficos
- Notificações por WhatsApp sobre risco de icterícia
- Geração e uso de dados sintéticos para treino e validação do modelo
- Diagnóstico instantâneo com logging e rastreabilidade

---

## 👥 Equipe Nexora (Equipe 14)

- [Matheus Rebouças](https://github.com/Math77077)  
- [Luccas Henrique](https://github.com/Luccas-H)  
- [Vinicius Martins](https://github.com/vinimartinsufrr)

---

## 📁 Como Executar o projeto

Faça o download dos arquivos do projeto e instale as bibliotecas necessarias a partir do arquivo requirements.txt
Ou [clique aqui para baixar o arquivo `requirements.txt`](./Arquivos%20Projeto/requirements.txt)
Para isso, utilize o comando: 
```bash
pip install -r requirements.txt
```
Após isso, execute o arquivo .ino no seu ESP32, hospede o arquivo do servidor "server.py" e execute o arquivo da página web "app.py".
Com isso, o projeto deve rodar perfeitamente!

---

## 🏫 Sobre o Programa

**Maloca das ICoisas** é uma iniciativa da **Universidade Federal de Roraima (UFRR)** com apoio da **Softex**, voltada para o desenvolvimento de soluções inovadoras com IoT, sistemas embarcados e tecnologias emergentes.  
Este projeto foi desenvolvido na etapa **Hands-on Avançado**.


---

## ✅ Status do Projeto

**✔ Finalizado**  
O sistema está funcional, com hardware, software, aprendizado de máquina e interface integrados.

---

## 📬 Contato

Para dúvidas ou sugestões, abra uma issue ou entre em contato com algum dos membros da equipe via GitHub.

---

> Projeto com propósito social: **tecnologia acessível para salvar vidas** desde os primeiros dias. 💛
