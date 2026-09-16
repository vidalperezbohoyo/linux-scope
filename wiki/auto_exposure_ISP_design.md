# Light weight Auto-Exposure ISP Design (LuckFox)

- La luminosidad de un pixel es un valor entre 0-255
- Podemos ajustar el "exposure" del sensor entre 1-536
- Podemos ajustar el "analogue_gain" del sensor entre 128-99614 

(Puede ser ejecutado cada 10 frames)

Algoritmo:
1. Obtener una muestra de píxeles de la imagen NV12 original
   - Centro de la pantalla, un ROI
   - Calcular luminosidad como el plano Y que es la Luminancia:
       - Y PLANE: W × H ← luminancia
       - UV PLANE: W × H / 2 ← color  
2. De esa muestra calcular el histograma (percentiles)
   - El percentil establece N porcentajes, por ejemplo: 0% 10% 20% 30%...
     Y para cada porcentaje, un valor, por ejemplo: 0% = 20, 10% = 40, 20% = 100, 30% = 120...
     Y significa que, por ejemplo, para el valor 20%, el 20% de los píxeles tienen una luminancia MENOR a 100.
     Esto te da mucha más información que una simple media, y te permite hacer una tabla de exposiciones según el porcentaje de píxeles.
     Por ejemplo yo puedo considerar una imagen bien expuesta cuando la mitad de los píxeles están por encima del = 127 (Punto medio del rango de luminancia), es decir 50% = 127.

   - Para cada sensor hay que establecer una tabla que ajuste la luminancia como queremos y se hace por el método práctico.
   - Opcion 2. Decrementar o incrementar ganancia si esta por encima o por debajo de lo que queremos. Esto lo hace mas gradual pero ira como una pupila. Se puede hacer tipo Proporcional tambien.
  
3. Se ajusta mediante v4l2 desde C++ los valores de exposición y ganancia deseados. 
  
