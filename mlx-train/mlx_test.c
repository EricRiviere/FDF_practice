/*
   - Intruccion basica para uso de la libreria minilibx:
   		Siguiendo este articulo: 
			https://medium.com/@jalal92/understanding-the-minilibx-a-practical-dive-into-x-window-programming-api-in-c-cb8a6f72bec3

	MiniLibX es una pequeña biblioteca gráfica utilizada en los proyectos de 42, incluyendo 42 Paris.
	Esta librería se utiliza principalmente para aprender conceptos básicos de gráficos en 2D, 
	crear ventanas, y manejar eventos de teclado y ratón. Es especialmente útil en proyectos como 
	"Cub3D" o "FDF", donde los estudiantes deben implementar gráficos básicos en un entorno controlado

	- Características principales de MiniLibX:
		
		- Creación de ventanas: 
	
		Te permite abrir una ventana donde se pueden dibujar píxeles, líneas, formas, etc.

		- Manejo de eventos: 

		Puedes gestionar entradas de teclado y ratón para interactuar con la ventana.

		- Dibujo de gráficos simples: 

		Proporciona funciones para dibujar píxeles, formas básicas como líneas y rectángulos, 
		así como para cargar imágenes y manipular buffers de imágenes.
		Compatibilidad limitada: MiniLibX está diseñada para funcionar en sistemas UNIX como Linux 
		y macOS. En el caso de Linux, suele requerir que instales bibliotecas como X11 y Xext para 
		el manejo de gráficos y eventos.

		- Ejemplos de funciones importantes:

			-mlx_init: Inicializa la conexión con el servidor de ventanas.
			-mlx_new_window: Crea una nueva ventana.
			-mlx_pixel_put: Dibuja un píxel en una coordenada específica de la ventana.
			-mlx_key_hook y mlx_mouse_hook: Manejan eventos de teclado y ratón.
			-mlx_loop: Inicia el bucle de eventos que mantiene la ventana abierta y activa la gestión 
			de eventos.

		- Limitaciones:

			- Limitada en funcionalidad: 

			MiniLibX no es comparable a bibliotecas gráficas más completas 
			como SDL o OpenGL. Es muy básica y está destinada a enseñar fundamentos, no a crear 
			aplicaciones gráficas complejas.

			- Documentación escasa: No tiene mucha documentación oficial, por lo que aprender a usarla
			generalmente implica estudiar ejemplos o consultar a otros estudiantes.
			
		En resumen, MiniLibX es una herramienta clave en los proyectos de gráficos 42, diseñada 
		para ayudar a los estudiantes a entender conceptos básicos de gráficos sin la complejidad 
		de bibliotecas más grandes.

   - Se puede descargar la libreria de 42 Paris aqui: 
		https://github.com/42Paris/minilibx-linux/tree/7dc53a411a7d4ae286c60c6229bd1e395b0efb82

   - Una vez descargada podemos ojear las distintas funciones presentes listandolas 
   		(es una libreria para usar la libreria X11 que es mucho mas grande)

   - MinilibX es una "mini libreria" disenada para empezar en el mundo de la programacion X-Window
   	Se trata de una API basada en el sistema X-Window que seria la base para que los sistemas operativos 
	linux tengan GUIs (Graphycal User Interface - Interfaz grafica de usuario).
	Es un intermediario entre tus inputs (ya sea teclado, raton, etc) y los outputs visuales en pantalla 
	(permite interaccionar y ver el resultado en pantalla).

	- Ahora que ya se entiende que es, veamos como funciona.
	Primero de todo hay que establecer una conexion con el servidor X-Window, y para ello usamos mlx_init()
	
	-MLX_INIT():

	-Esta es la funcion mlx_init() de minilibx:

	#include	"mlx_int.h"

	void	*mlx_init()
	{
		t_xvar	*xvar;

		if (!(xvar = malloc(sizeof(*xvar))))
			return ((void*)0);
		if ((xvar->display = XOpenDisplay("")) == 0)
		{
			free(xvar);
			return ((void*)0);
		}
		xvar->screen = DefaultScreen(xvar->display);
		xvar->root = DefaultRootWindow(xvar->display);
		xvar->cmap = DefaultColormap(xvar->display,xvar->screen);
		xvar->depth = DefaultDepth(xvar->display,xvar->screen);
		if (mlx_int_get_visual(xvar)==-1)
		{
			printf(ERR_NO_TRUECOLOR);
			exit(1);
		}
		xvar->win_list = 0;
		xvar->loop_hook = 0;
		xvar->loop_param = (void *)0;
		xvar->do_flush = 1;
		xvar->wm_delete_window = XInternAtom (xvar->display, "WM_DELETE_WINDOW", False);
		xvar->wm_protocols = XInternAtom (xvar->display, "WM_PROTOCOLS", False);
		mlx_int_deal_shm(xvar);
		if (xvar->private_cmap)
			xvar->cmap = XCreateColormap(xvar->display,xvar->root,
					 xvar->visual,AllocNone);
		mlx_int_rgb_conversion(xvar);
		xvar->end_loop = 0;
		return (xvar);
	}

	*Esta funcion devuelve una estructura (xvar).
	Para establecer la conexion, se crea un puntero a una variable de tipo xvar que contiene todo lo 
	necesario para ello, y esta es su estructura:

	typedef struct	s_xvar
	{
		Display		*display;
		Window		root;
		int			screen;
		int			depth;
		Visual		*visual;
		Colormap	cmap;
		int			private_cmap;
		t_win_list	*win_list;
		int			(*loop_hook)();
		void		*loop_param;
		int			use_xshm;
		int			pshm_format;
		int			do_flush;
		int			decrgb[6];
		Atom		wm_delete_window;
		Atom		wm_protocols;
		int 		end_loop;
	}				t_xvar;


	*Como la funcion mlx_init, reserva espacio para esta estructura xvar, cuando establecemos la conexion,
	debemos asegurarnos de liberar todo antes de finalizar el programa.
	Para ello usamos la funcion mlx_destroy_display (para destruir el display) y luego liberaremos el puntero
	creado con free.
	Aqui un pequeno ejemplo de inicializar la conexion y cerrarla sin que nada pase:

		#include <stdlib.h>
		#include "minilibx_linux/mlx.h"

		int	main(void)
		{
			void	*mlx_ptr;//Creamos el puntero

			mlx_ptr = mlx_init();//El puntero apunta a la estructura (xvar) devuelta por mlx_init
			if (!mlx_ptr)//Controlamos el posible fallo en la reserva de memoria
			return (1);

			mlx_destroy_display(mlx_ptr);//Liberamos el display
			free(mlx_ptr);//Liberamos el puntero
			return (0);
		}

	*Para compilar un archivo de test usando la libreria son necesarios varios flags
	Ejemplo:

	cc test.c -Lminilibx_linux -lmlx-Linux -lx11 -lXent

	cc: compila
	test.c: nombre archivo de prueba
	-Lminilibx_Linux: Especifica el directorio para buscar las librerias
	-lmlx_Linux: Especifica a cc que linkee a las librerias de Linux (libmlx_Linux.a o libmlx_Linux.so)
	-lX11: Especifica que linkee con la libreria X11 (GUI para SO Linux)
	-Xext: Especifica que linkee con la libreria Xent que prvee extensiones para el sistema X Window

	Asi pues, le decimos donde encontrar los archivos de las librerias y que librerias linkear

	Importante entender el concepto de DISPLAY, en este esquema donde la libreria minilibx nos permite
	conectar hardware de inputs (teclado, mouse, etc) y hardware de outputs (pantalla), cuando llamamos
	a la funcion molx_init(), establecemos esta conexion y le decimos "esta conexion es para imprimir 
	cosas por pantalla", por eso hablamos de "display".
	-------------

	Una vez entendido que hay que establecer una conexion, el siguiente concepto a profundizar es el
	de 'window' o ventana.
	Para abrir una ventana, usamos la funcion mlx_new_window() de minilibx.
	Este es el prototipo de la funcion: 
		- void	*mlx_new_window(t_xvar *xvar,int size_x,int size_y,char *title)
			mlx_new_window(puntero conexion, alto, ancho, nombre de la ventana).

	Podemos ver la estructura en el archivo mlx_new_window.c dentro de la libreria minilibx:

	void	*mlx_new_window(t_xvar *xvar,int size_x,int size_y,char *title)
	{
		t_win_list				*new_win;
		...
		...
		return (new_win);
	}

	Esta funcion crea un puntero a una estructura t_win_list y la devuelve, esta estructura se puede
	ver en el archivo mlx_int.h:

	typedef struct	s_win_list
	{
		Window				window;
		GC					gc;
		struct s_win_list	*next;
		int					(*mouse_hook)();
		int					(*key_hook)();
		int					(*expose_hook)();
		void				*mouse_param;
		void				*key_param;
		void				*expose_param;
		t_event_list		hooks[MLX_MAX_EVENT];
	}				t_win_list;

	La funcion mlx_new_window() tambien reserva espacio en memoria para la estructura de tipo t_win_list 
	el puntero de la cual devuelve (new_win).
	El primer parametro de la funcion es el puntero a la conexion display, por lo que si no la hemos
	establecido no podremos crear esta ventana.
	El concepto VENTANA, si que nos es mas familiar, porque es literalmente una ventana dentro de nuestra
	pantalla.
	Como la reserva de memoria para la estructura necesaria para crear la ventana (t_win_list) puede fallar,
	debemos protejerla y liberar el display (mlx_destroy_display()) y el puntero (con free).

	Aqui un ejemplo de programa para establecer la conexion y crear una ventana:

		#include <stdlib.h>
		#include "minilibx-linux/mlx.h"
		#define WIDTH	500
		#define HEIGHT	500

		int main(void)
		{
			void	*mlx_connection;
			void	*mlx_window;

			mlx_connection = mlx_init();
			if (!mlx_connection)
				return (1);

			mlx_window = mlx_new_window(mlx_connection, HEIGHT, WIDTH, "Mi ventana");
			if (!mlx_window)
			{
				mlx_destroy_display(mlx_connection);
				free(mlx_connection);
				return (1);
			}

			//mlx_loop(mlx_connection);
			mlx_destroy_window(mlx_connection, mlx_window);
			mlx_destroy_display(mlx_connection);
			free(mlx_connection);
			return (0);
		}

	Si compilamos y ejecutamos tal y como esta con los mismos flags que antes, veremos como se crea la
	ventana y se cierra automaticamente.
	Por eso es importante la funcion comentada "mlx_loop()"
	-	MLX_LOOP() es una funcion para mantener nuestro programa activo a la espera de eventos.
	Si lo descomentamos, compilamos y ejecutamos de nuevo, la ventana permanecera abierta hata que 
	hagamos cntl + C (pero cerrarla asi, nos generara leaks, podemos comprobarlo, compilando y ejecutando
	"valgrind ./a.out" y cerrando con cntl + 'C').
	Para evitar estos leaks, necesitamos una funcion para gestionar los eventos (que vendrian a ser los
	inputs que mandamos mediante el hardware).

	Antes de profundizar en el concepto de Eventos, mencionar que es posible crear multiples ventanas a 
	la vez. Si lo hacemos es importante en la parte de control de la reserva de memoria, tener en cuenta
	las ventanas que hemos creado anteriormente y liberarlas con mlx_destroy_window().

	-----------------

	EVENTOS:
	
	Para tener una idea inicial de en que consisten los eventos, revisar la documentacion de
	minilibx en: https://harm-smits.github.io/42docs/libs/minilibx/events.html


		Eventos: Son acciones o interacciones que ocurren mientras la aplicación gráfica está en 
		ejecución, como presionar una tecla, mover el ratón o cerrar una ventana.
		Estos eventos son detectados por la biblioteca y se pueden manejar con código específico.

	HOOKS:

	Para tener una idea inicial sobre los hooks revisar la documentacion de hooks en:
	https://harm-smits.github.io/42docs/libs/minilibx/hooks.html#hooking-into-key-events

		Hooks: Son funciones que asocias a ciertos eventos. Cuando un evento ocurre (por ejemplo, 
		presionar una tecla), la función "hookeada" se ejecuta para manejarlo.
		En MiniLibX, usas funciones como mlx_key_hook o mlx_mouse_hook para conectar eventos con 
		tus propias funciones de respuesta.
	
	Teniendo una idea inicial de lo que son los eventos y los hooks, se puede entender, que con
	minilibx, podriamos por ejemplo:

		- Mediante la funcion mlx_key_hook, decirle a la biblioteca que cuando se presione una tecla,
		se llame a una funcion que se haya definido para gestionar ese evento.

		- Mediante la funcion mlx_mouse_hook, decirle a la biblioteca que cuando se realice una
		accion con el mouse (click izquierdo, click derecho, click en la rueda o scroll up/down),
		se llame a una funcion que se haya definido para gestionar ese evento.

		*El Keysym es parte del sistema de ventanas X11 en Linux y macOS, y se utiliza para traducir
		una tecla física (un código de tecla, o keycode) a un símbolo comprensible para el programa,
		como 'a', 'A', o una tecla especial como 'Escape'.
		https://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h

		*Para el raton existe una numeracion que puede variar en los distintos sistemas operativos
		pero en el caso de linux:
			-Click izquierdo:1
			-Click en rueda:2
			-Clich derecho:3
			-Scroll arriba:4
			-Srcoll abajo:5

	Veamos un ejemplo de como usar EVENTOS y HOOKS con minilibx:

	#include "minilibx-linux/mlx.h"
	#include <X11/keysym.h>//Incluimos la libreria keysym para el teclado
	#include <stdlib.h>//Para malloc
	#include <stdio.h>//Para printf

	#define MLX_ERROR		1
	#define	WINDOW_WIDTH	1000
	#define	WINDOW_HEIGHT	1000

	typedef struct	s_mlx_data//Creamos una estructura con punteros a la conexion y la ventana
	{
		void	*mlx_ptr;
		void	*win_ptr;
	}			t_mlx_data;//Esta nos servira para destruir conexion y ventana en un evento

	
	//Funcion propia para manejar los eventos de teclado. Recibe el int de keysym y la data
	int	handle_key_input(int keysym, t_mlx_data *data)
	{
		//Si la ecla es "esc" destruimos la ventana, la conexion,liberamos el puntero y salimos
		if (keysym == XK_Escape)
		{
			printf("The %d key has been pressed\n\n", keysym);
			mlx_destroy_window(data->mlx_ptr, data->win_ptr);
			mlx_destroy_display(data->mlx_ptr);
			free(data->mlx_ptr);
			exit(1);
		}
		else if (keysym == XK_Delete)//Este es un ejemplo, se puede hacer un caso para cada letra
			printf("The 'delete' key has been pressed\n\n");
		else
			printf("The %d key has been pressed\n\n", keysym);
		return (0);
	}

	//Funcion para manejar los eventos del mouse
	int	handle_mouse_input(int movement, int x, int y, t_mlx_data *data)
	{
		if (movement == 1)
			printf("Se ha apretado el boton izquierdo del mouse en (%d, %d)\n", x, y);
		if (movement == 2)
			printf("Se ha apretado la rueda de scroll del mouse en (%d, %d)\n", x, y);
		if (movement == 3)
			printf("Se ha apretado el boton derecho del mouse en (%d, %d)\n", x, y);
		if (movement == 4)
			printf("Se ha scrolleado hacia arriba\n");
		if (movement == 5)
			printf("Se ha scrolleado hacia abajo\n");
		return (0);
	}

	int	main(void)
	{
		t_mlx_data	data;

		//Iniciamos la conexion con X
		data.mlx_ptr = mlx_init();
		if (!data.mlx_ptr)
			return (MLX_ERROR);
		//Creamos la ventana
		data.win_ptr = mlx_new_window(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Test de eventos");
		if (!data.win_ptr)
		{
			mlx_destroy_display(data.mlx_ptr);
			free(data.mlx_ptr);
			return (MLX_ERROR);
		}

		//Funciones para manejar los hooks de teclado y mouse
		mlx_key_hook(data.win_ptr, handle_key_input, &data);
		mlx_mouse_hook(data.win_ptr, handle_mouse_input, &data);
		//Funcion para mantener el programa escuchando
		mlx_loop(data.mlx_ptr);
	}

	Si compilamos y ejecutamos el programa podemos testear los distintos eventos y hooks

	Se pueden hacer muchas cosas mas con la libreria minilibx, el siguiente ejemplo es sobre como 
	usar un par mas de funciones de la libreria:
	
		- mlx_string_put(): 
			
			int		mlx_string_put(t_xvar *xvar,t_win_list *win,
			       int x,int y,int color,char *string)

			Escribe un string en la ventana, en unas cordinadas concretas.

		- mlx_loop_hook():

			int	mlx_loop_hook(t_xvar *xvar,int (*funct)(),void *param)

			Aplica una funcion siempre que no se disparen eventos.
	
	
		*En este ejemplo, escribimos un texto con un color en una posicion en la pantalla.
		Tenemos un hook que ejecuta una funcion en bucle mientras no se disparen eventos
		(esta funcion cambiara el texto de color).
		Cuando se presione una tecla, se disparara un evento que imprime la tecla en consola
		y detiene el programa durante 2 segundos (no escuchara eventos ni ejecutara mas codigo):


		#include "minilibx-linux/mlx.h"
	#include <stdio.h>
	#include <unistd.h>

	typedef struct s_data
	{
		void	*mlx_ptr;
		void	*win_ptr;
		int		color;
	}			t_data;

	int	sleep_event(int keysym, t_data *data)//Funcion para alargar el evento 2 segundos
	{
		printf("Pressed %d\n", keysym);
		sleep(2);
		return (1);
	}

	int	change_color(t_data *data)//Funcion que se aplicara siempre que no se disparen eventos
	{
		mlx_string_put(data->mlx_ptr, data->win_ptr, 150, 250, data->color, "Hello World!");
	
		if (data->color == 0xFF0000)//Si es rojo
			data->color = 0x00FF00;//cambia a verde
		else if (data->color == 0x00FF00)//Si es verde
			data->color = 0x0000FF;//cambia a azul
		else
			data->color = 0xFF0000;//Sino cambia a rojo
		return (0);
	}

	int	main(void)
	{
		t_data	data;

		data.mlx_ptr = mlx_init();
		data.win_ptr = mlx_new_window(data.mlx_ptr, 500, 500, "Ventana texto de color");
		data.color = 0xFF0000;

		mlx_key_hook(data.win_ptr, sleep_event, &data);//Hook de gestion de teclado
		mlx_loop_hook(data.mlx_ptr, change_color, &data);//Hook de gestion de bucle
		mlx_loop(data.mlx_ptr);
		return(0);
	}

	*FALTA LA PARTE DE PINTAR PIXELES DE FORMA NO OPTIMIZADA Y DE FORMA OPTIMIZADA*
	mlx_pixel_put.c //puede ser interesante meterle mlx_rgb.c
	mlx_new_image.c
	mlx_destroy_image.c
	mlx_put_image_to_window.c
	mlx_get_data_addr.c
*/

#include "minilibx-linux/mlx.h"
#include <stdio.h>
#include <unistd.h>

typedef struct s_data
{
	void	*mlx_ptr;
	void	*win_ptr;
	int		color;
}			t_data;

int	sleep_event(int keysym, t_data *data)//Funcion para alargar el evento 2 segundos
{
	printf("Pressed %d\n", keysym);
	sleep(2);
	return (1);
}

int	change_color(t_data *data)//Funcion que se aplicara siempre que no se disparen eventos
{
	mlx_string_put(data->mlx_ptr, data->win_ptr, 150, 250, data->color, "Hello World!");

	if (data->color == 0xFF0000)//Si es rojo
		data->color = 0x00FF00;//cambia a verde 
	else if (data->color == 0x00FF00)//Si es verde
		data->color = 0x0000FF;//cambia a azul
	else
		data->color = 0xFF0000;//Sino cambia a rojo
	return (0);
}

int	main(void)
{
	t_data	data;

	data.mlx_ptr = mlx_init();
	data.win_ptr = mlx_new_window(data.mlx_ptr, 500, 500, "Ventana texto de color");
	data.color = 0xFF0000;

	mlx_key_hook(data.win_ptr, sleep_event, &data);//Hook de gestion de teclado
	mlx_loop_hook(data.mlx_ptr, change_color, &data);//Hook de gestion de bucle
	mlx_loop(data.mlx_ptr);
	return(0);
}
