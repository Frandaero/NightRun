La �nica raz�n que se me ocurre para que el C�digo Fuente no pueda ser compilado es la siguiente:
-La carpeta "SDL Source" no est� siendo apuntada correctamente.

Para reapuntar al C�digo Fuente y que el programa compile correctamente, deber�s seguir estos pasos:
1- Abrir el proyecto
2- Ir a Project --> Properties
3- Clickear Project's build options abajo a la derecha
4- Pesta�a Search Directories
5- Dentro de la subpesta�a Compiler, agregar un apuntador a SDL Source\Include\SDL2
6- Dentro de la subpesta�a Linker, agregar un apuntador a SDL Source\lib
7- Poner Ok y compilar, deber�a funcionar