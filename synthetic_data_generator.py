import pandas as pd
import numpy as np

def gerar_dados_sinteticos(n=500):
    dados = []
    for _ in range(n):
        if np.random.rand() > 0.5:
            # Normal skin
            r = np.random.uniform(0.8, 0.95)
            g = np.random.uniform(0.6, 0.75)
            b = np.random.uniform(0.45, 0.60)
            label = 0
        else:
            # Simulated jaundice (more yellow: higher red and green, lower blue)
            r = np.random.uniform(0.6, 0.75)
            g = np.random.uniform(0.5, 0.65)
            b = np.random.uniform(0.2, 0.35)
            label = 1
        dados.append([r, g, b, label])
    
    df = pd.DataFrame(dados, columns=['R', 'G', 'B', 'label'])
    df.to_csv('dados_ictericia.csv', index=False)
    print("✅ dados_ictericia.csv gerado com sucesso!")

gerar_dados_sinteticos()
