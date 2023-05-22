# Sistema automatizado de compactado de basura mediante electroneumática.

El sistema es capaz de detectar cuando el tanque de almacenamiento se ha llenado, compactar la basura y vaciar el contenedor para retomar automáticamente el proceso.

Este repositorio contiene los siguientes archivos:

1. Diagrama Eléctrico con sus respectivas protecciones.
2. Diagrama de Programación LADDER para su implementación en PLC.
3. Código de programación en C++ para su implementación en ESP32/ARDUINO.

## Funcionamiento
Al presionar `START` el motor `M` empieza a funcionar para dar movimiento a la cinta transportadora, por donde se desplaza la basura hasta un tanque de almacenamiento.

Mediante un sensor `SL` el sistema detecta que el tanque se ha llenado, en este momento se debe detener el motor e iniciar el compactado de la basura mediante el cilindro `A`, el cual se debe retraer automáticamente cuando se detecte con sensores finales de carrera que se ha expandido totalmente.

Al retraer el compactador, se debe dar apertura a la compuerta por donde va a salir la basura activando el cilindro `B`.

Cuando se detecte la apertura completa, se activará el cilindro `C` que se encargará de vaciar la basura compactada por la compuerta. Al sensar que el vástago ha salido completamente el cilindro se retrae automáticamente, y una vez se detecte su retracción completa la compuerta se debe cerrar.

Tras cerrar la compuerta el motor se vuelve a encender automáticamente para continuar el funcionamiento del sistema.

## Software utilizado
- CADe SIMU
- PlatformIO (Framework Arduino)

## Hardware Utilizado
- ESP32-WROOM-32D DEVKIT.
- PLC Siemens S7-1200.
- Electroválvulas Monoestables 5-2.
- Cilindros Neumáticos.
- Sensores de final de carrera.

## Instructor:
Joseph Santiago Portilla - Ingeniero Electrónico.
- GitHub: https://github.com/JoePortilla
- Twitter: https://twitter.com/JoePortilla
- LinkedIn: https://www.linkedin.com/in/PortillaJoe
- Kaggle: https://www.kaggle.com/joeportilla
