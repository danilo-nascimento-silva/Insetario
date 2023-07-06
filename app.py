import streamlit as st
import requests
import json

link = 'https://insetario-uesc-default-rtdb.firebaseio.com/'


requisicao = requests.get(f'{link}/.json')
dic_requisicao = requisicao.json()

temperature = dic_requisicao['temperatura']
humidity = dic_requisicao['umidade']
datetime = dic_requisicao['datetime']

col1, col2  = st.columns(2)
col1.metric("Temperatura", temperature)
col2.metric("Umidade", humidity)

