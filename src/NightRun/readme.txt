La única razón que se me ocurre para que el Código Fuente no pueda ser compilado es la siguiente:
-La carpeta "SDL Source" no está siendo apuntada correctamente.

Para reapuntar al Código Fuente y que el programa compile correctamente, deberás seguir estos pasos:
1- Abrir el proyecto
2- Ir a Project --> Properties
3- Clickear Project's build options abajo a la derecha
4- Pestaña Search Directories
5- Dentro de la subpestaña Compiler, agregar un apuntador a SDL Source\Include\SDL2
6- Dentro de la subpestaña Linker, agregar un apuntador a SDL Source\lib
7- Poner Ok y compilar, debería funcionar