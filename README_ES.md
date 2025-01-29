# Package Manager for Old Windows
 [![GitHub release](https://img.shields.io/github/v/release/MasterJayanX/pmfow.svg)](https://github.com/MasterJayanX/pmfow/releases) [![GitHub all releases](https://img.shields.io/github/downloads/MasterJayanX/pmfow/total)](https://github.com/MasterJayanX/pmfow/releases) 

## Idiomas
[English](https://github.com/MasterJayanX/pmfow/blob/main/README.md) | [Español](https://github.com/MasterJayanX/pmfow/blob/main/README_ES.md)
## Descripción
pmfow (Package Manager for Old Windows) es un programa que te permite instalar aplicaciones desde la línea de comandos en versiones antiguas de Windows de manera similar a como lo harías en Linux o en Windows 10 con [winget](https://github.com/microsoft/winget-cli).

<figure>
 <img src="docs/1.png" width=720 /> 
 <figcaption>Captura de pantalla de pmfow instalando Legacy Update en una MV de Windows XP.</figcaption>
</figure>
<figure>
 <img src="docs/2.png" width=720 />
 <figcaption>Captura de pantalla de pmfow instalando VLC Media Player en la misma MV de Windows XP.</figcaption>
</figure>

## Requisitos
### Requisitos de hardware

**Requisitos mínimos**
- Un procesador
- Algo de RAM
- Al menos 16 MB de almacenamiento
- Una conexión a internet

**Requisitos recomendados**
- Un procesador Pentium o mejor.
- 512 MB de RAM (necesarios para ejecutar versiones de Windows más nuevas que XP) o suficiente RAM para usar un navegador web.
- Al menos 300 MB de almacenamiento disponibles.
- Una buena conexión a internet (lo suficiente para descargar archivos más o menos pesados).

### Requisitos de software
pmfow funciona en la mayoría de las versiones de Windows desde Windows 2000 en adelante. Nota: En algunas versiones de Windows puede que sea necesario instalar [esta actualización](https://support.microsoft.com/es-es/topic/actualizaci%C3%B3n-del-entorno-en-tiempo-de-ejecuci%C3%B3n-de-c-universal-en-windows-c0514201-7fe6-95a3-b0a5-287930f3560c).

**Esta es la lista de versiones completamente compatibles:**

- Windows XP
- Windows Vista
- Windows 7
- Windows 8
- Windows 8.1
- Sus versiones de servidor

**Esta es la lista de versiones parcialmente compatibles:**

- Windows 10: Windows 10 es parcialmente compatible, ya que la mayoría de los paquetes en el repositorio de Windows 10 son los mismos paquetes que los de Windows 8/8.1. Para Windows 10 y 11, es mejor usar winget.
- Windows 2000: Selección limitada de paquetes disponibles para esta versión de Windows y solo se puede usar con una versión antigua de wget, que puede no funcionar correctamente.
- ReactOS: Funcionará, pero ciertas aplicaciones que se pueden instalar con pmfow no funcionarán por defecto.

## Instalación
Para instalar pmfow, debes seguir estos pasos:
1. Descarga el archivo .exe desde la pestaña de [versiones](https://github.com/MasterJayanX/pmfow/releases) (Releases) que corresponda a la arquitectura de tu CPU. Si no estás seguro de cuál debes elegir, ve a Panel de control > Sistema y verifica si dice "Sistema operativo de 32 bits" o "Sistema operativo de 64 bits". Si dice 32 bits, elige la versión que dice win32 y si dice 64 bits, elige la versión que dice win64.
2. Abre el archivo .exe e instala pmfow en cualquier directorio que desees (por defecto se instalará en la carpeta de Archivos de Programa). El instalador también añadirá el directorio al PATH del sistema por defecto, permitiéndote ejecutar pmfow desde cualquier directorio.

## Uso
Para usar pmfow, abre una ventana de CMD y ejecuta el siguiente comando: `pmfow <comando>` (Nota: si no seguiste el paso 3 de la instalación, deberás abrir la ventana de cmd en la misma carpeta donde extrajiste el ejecutable de pmfow y escribir `pmfow.exe <comando>` en su lugar).
Aquí tienes una lista de comandos que puedes utilizar:
- `pmfow install <paquete>`: Este comando te permitirá instalar el programa que desees. Alternativamente, puedes usar `pmfow i <paquete>`.
- `pmfow uninstall <paquete>`: Este comando te permitirá desinstalar el programa que desees. Alternativamente, puedes usar `pmfow remove <paquete>` o `pmfow rm <paquete>`.
- `pmfow update`: Esto actualizará los repositorios, que son simplemente archivos .txt, pero aún así. Alternativamente, puedes usar `pmfow u`.
- `pmfow search <paquete>`: Este comando te permitirá buscar un paquete específico en los repositorios de tu sistema operativo. Alternativamente, puedes usar `pmfow s <paquete>`.
- `pmfow version`: Este comando te mostrará la versión de pmfow que estás utilizando, así como tu versión de Windows. Alternativamente, puedes usar `pmfow about` o `pmfow v`.
- `pmfow help`: Este comando te mostrará una lista de comandos con una descripción para cada uno, como la que estás viendo aquí. Alternativamente, puedes usar `pmfow h`.
- `pmfow list`: Este comando lista todos los paquetes disponibles en tu repositorio actual. Alternativamente, puedes usar `pmfow l`.

Si estás utilizando los comandos install, uninstall, update, list o search, puedes utilizar estas flags (opciones):
- `-c` o `--check-certificates`: Hace que wget verifique los certificados al descargar un paquete.
- `-f <versión>` o `--force-os <versión>`: Obliga a pmfow a instalar paquetes para una versión diferente de Windows.
- `--show-url`: Muestra la URL desde la cual se descarga el paquete que estás instalando o buscando.
- `-o` o `--one-file`: El comando update solo actualizará el archivo de repositorio que corresponde a tu versión actual de Windows.
- `-w <versión>` o `--wget-version <versión>`: Obliga al comando install o update a utilizar una versión de wget para una versión específica de Windows (por ejemplo: Windows XP) en lugar de cambiar el nombre del archivo de la versión que necesitas usar.
- `-u` o `--show-url`: Muestra la URL del paquete.
- `--check`: Solo verifica las actualizaciones de pmfow en lugar de actualizar también los repositorios.
- `--unstable`: Comprueba si hay una nueva versión inestable/desarrollo de pmfow en lugar de una estable.
- `--uninstall`: Lista todos los programas que se pueden desinstalar con pmfow (solo se puede usar con el comando list).

## Compilación
### Requisitos
- Tener el compilador g++ instalado en un computador con Windows (puedes instalarlo con MinGW).

Para compilar este programa tú mismo, debes descargar los archivos .cpp y guardarlos en el mismo directorio. Alternativamente, puedes descargar los archivos del repositorio usando el siguiente comando en una ventana de terminal: `git clone https://github.com/MasterJayanX/pmfow`. 
Luego, debes abrir una ventana de terminal y compilar el archivo main.cpp con el siguiente comando: `g++ main.cpp -o output.exe -Wall` (puedes reemplazar output con el nombre que desees).

## Limitaciones y problemas conocidos
pmfow está en una etapa temprana de desarrollo, por lo que puedes esperar que algunas cosas no funcionen como deberían. Algunas de las limitaciones que tiene en este momento son:
- Los repositorios actualmente tienen una selección muy limitada de software.
- El comando update no funciona correctamente en Windows 2000, debido a limitaciones de SSL. Esto también afecta a otras descargas.

Si encuentras algún problema con pmfow, abre un problema en la sección de Issues y lo revisaré.

## Preguntas frecuentes
### ¿Qué versiones de Windows son compatibles?
Como mencioné anteriormente, pmfow funciona en las siguientes versiones de Windows: Windows 2000, Windows XP, Windows Vista, Windows 7, Windows 8, Windows 8.1, Windows 10, Windows 11 y sus equivalentes para servidores. También debería funcionar en ReactOS y en Linux mediante Wine, pero aún no los he probado.
### ¿Qué aplicaciones puedo instalar con pmfow?
Puedes encontrar la lista completa de aplicaciones que se pueden instalar en [Software_List.md](https://github.com/MasterJayanX/pmfow/blob/main/Software_List.md) (ten en cuenta que está en inglés).
### ¿Se puede usar pmfow en Windows 9x?
Desafortunadamente, debido a que estas versiones de Windows son demasiado antiguas para que wget funcione de manera confiable en ellas, decidí no agregar soporte para ellas en este proyecto.
### ¿Este proyecto admitirá otros sistemas operativos?
Desafortunadamente no, porque es probable que otros sistemas operativos ya tengan gestores de paquetes para ellos. Por ejemplo, para versiones antiguas de macOS, puedes usar [Tigerbrew](https://github.com/mistydemeo/tigerbrew) y la mayoría de las distribuciones de Linux vienen con un gestor de paquetes.
### ¿Qué cosas planeas agregar?
Pueden revisar el [Roadmap](https://github.com/MasterJayanX/pmfow/wiki/Roadmap) para más información en lo que tengo planeado para el futuro.

## Donaciones
Acepto donaciones a través de GitHub Sponsors y Ko-fi:

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D37FMC3)
