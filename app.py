import streamlit as st
import requests

link = 'https://insetario-uesc-default-rtdb.firebaseio.com/'

def getDHT():

    r = requests.get(f'{link}.json')
    h, t = r.json()
    return h, t

humidity, temperature = getDHT()
print(humidity, temperature)

col1, col2, col3 = st.columns(3)
col1.metric("Temperatura", temperature, "anterior")
col3.metric("Umidade", humidity, "Anterior")

