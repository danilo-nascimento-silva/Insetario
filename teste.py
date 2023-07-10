import streamlit as st
import requests
import json

link = 'https://insetario-uesc-default-rtdb.firebaseio.com/'

def get_days():

    temperatures = []
    humiditys = []
    date_time = []
    times = []
    r = requests.get(f'{link}/2023/07/09/.json')
    contador = 0

    if r.status_code != 200:
        print('Erro client')
        return [], [], []
    
    result = r.json()
    contador = 0
    for chave, valor in result.items():
        for i, j in valor.items():
            if contador == 0:
                print(i, j)
                date_time.append(j)
                contador = 1
            elif contador == 1:
                print(i, j)
                temperatures.append(j)
                contador = 2
            elif contador == 2:
                print(i, j)
                humiditys.append(j)
                contador = 0
    return 

def get_months():

    temperatures = []
    humiditys = []
    times = []
    date_time = []
    r = requests.get(f'{link}/2023/07/.json')
    contador = 0

    if r.status_code != 200:
        print('Erro client')
        return [], [], []
    
    result = r.json()
    for chave, valor in result.items():
        print(chave, valor)
        for chave2, valor2 in valor:
            print(chave2, valor2)

    return 

get_months()

