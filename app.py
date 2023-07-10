import streamlit as st
import pandas as pd
import requests
import json
from datetime import datetime, timedelta
from streamlit_autorefresh import st_autorefresh

st.set_page_config(page_title = "Monitoramento", layout = "wide")
link = 'https://insetario-uesc-default-rtdb.firebaseio.com/'
count = st_autorefresh(interval=60000, key="refresh")
default_date = datetime.now()
year, mouth, day = default_date.strftime('%Y'), default_date.strftime('%m'), default_date.strftime('%d')

def get_days():

    temperatures = []
    humiditys = []
    date_time = []
    times = []
    r = requests.get(f'{link}/{year}/{mouth}/{day}/.json')
    contador = 0

    if r.status_code != 200:
        print('Erro client')
        return [], [], []
    
    result = r.json()
    for chave, valor in result.items():
        times.append(chave)
        for i, j in valor.items():
            if contador == 0:
                date_time.append(j)
                contador = 1
            elif contador == 1:
                temperatures.append(j)
                contador = 2
            elif contador == 2:
                humiditys.append(j)
                contador = 0
    return temperatures, humiditys, times, date_time

def get_months():

    temperatures = []
    humiditys = []
    times = []
    r = requests.get(f'{link}/{year}/{mouth}/.json')
    contador = 0

    if r.status_code != 200:
        print('Erro client')
        return [], [], []
    
    result = r.json()
    contador = 0
    for chave, valor in result.items():
        times.append(chave)
        for i, j in valor.items():
            if contador == 0:
                date_time.append(j)
                contador = 1
            elif contador == 1:
                temperatures.append(j)
                contador = 2
            elif contador == 2:
                humiditys.append(j)
                contador = 0

    return temperatures, humiditys, times

def generationDT(times, **kwargs):
    if 'temperatures' in kwargs:
        data = {
            'Data/Hora':times,
            'Temperatura':kwargs['temperatures']
        }
    elif 'humiditys' in kwargs:
        data = {
            'Data/Hora':times,
            'Umidade':kwargs['humiditys']
        }
    elif 'temperatures_day' and 'humiditys_day' in kwargs:
        data = {
            'Data/Hora':times,
            'Temperatura':kwargs['temperatures_day'],
            'Umidade':kwargs['humiditys_day']
        }

    df = pd.DataFrame(data)
    df.set_index('Data/Hora', inplace = True)
    return df


temperatures_day, humiditys_day, times_day, date_time = get_days()
temperatures_mouth, humiditys_mouth, times_mouth = get_months()

dt_day = generationDT(date_time, temperatures_day=temperatures_day, humiditys_day=humiditys_day)
dt_day_temp = generationDT(times_day, temperatures=temperatures_day)
dt_day_humi = generationDT(times_day, humiditys=humiditys_day)







st.title("Monitoramento")

col1, col2  = st.columns(2)
col1.metric("Temperatura", str(temperatures_day[-1]) + '°C  🌡️', str(temperatures_day[-2]) + '°C')
col2.metric("Umidade", str(humiditys_day[-1]) + '%  💧', str(humiditys_day[-2]) + '%')

date1, date2  = st.columns(2)
with date1:
    date = st.date_input('Selecione a data desejada!! 📅')
with date2:
    select = st.selectbox('Selecione a opção desejada', ('Dia', 'Semana', 'Mês'))

if select == "Dia":
    date1, date2  = st.columns(2)
    with date1:
        st.subheader('Temperatura 🌡️')
        st.bar_chart(dt_day_temp)
    with date2:
        st.subheader('Umidade 💧')
        st.bar_chart(dt_day_humi)
    st.table(dt_day)

elif select == 'Semana':
    st.write("Em construção!!")
    st.image("image/images.png")

elif select == 'Mês':
    st.write("Em construção!!")
    st.image("image/images.png")
    #st.line_chart(dt_mouth)
    #st.table(dt_mouth)
