# Plot de un archivo xlsx 

import pandas as pd
import matplotlib.pyplot as plt

# Cargar el archivo TSV
df = pd.read_csv('plot/max4satPlot.tsv', sep='\t')

# Graficar por cada columna Success Ratio una curva
for col in df.columns:
    if "Success Ratio" in col:
        # Graficar la curva, castea como float para que no se vea como string
        plt.plot(df['File'], df[col], label=col)

# Mostrar la leyenda (eliminando la palabra Success Ratio)
plt.legend([(x.replace('Success Ratio (', '')).replace(')', '')
            for x in df.columns if "Success Ratio" in x])

# Titulo del grafico
plt.title('Proporción de éxito')
plt.xticks(rotation=45)

# Mostrar y guardar el grafico en png
plt.show()
        
# Graficar tiempos de ejecución
for col in df.columns:
    if "Time" in col:
        # Graficar la curva
        plt.plot(df['File'], df[col], label=col)

# Mostrar la leyenda (eliminando la palabra Success Ratio)
plt.legend([(x.replace('Time (', '')).replace(')', '')
            for x in df.columns if "Time" in x])

# Titulo del grafico
plt.title('Tiempos de ejecución (s)')
plt.xticks(rotation=45)

# y max es de 200 
#plt.ylim(0, 50)
plt.show()
        