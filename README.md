# TP1_Taller_1


TO DO:

- See if functions return bools or ints to indicate errors (correct the ones that return ints if bool are chosen)
- Achicar verify_block, leer comentarios de todo el codigo escrito
- Ver si se pueden generalizar los verify en uno solo
- Ver si es mejor devolver memoria dinamica en vez de pasar una matriz por parametro a sudoku_get_board
- Cambiar el atoi por una función char_to_int para no tener que pasar las cosas a un array
- Hacer para socket un handler de cliente y un handler de server
- Cambiar sudoku_init_with_file a sudoku_init
- Implementar sudoku_release (hay que hacer release a todas las celulas del tablero)
- Arreglar el funcionamiento de sudoku_verify (se fija tambien que no haya casilleros vacíos, los cuales en realidad debería ignorar)
- Agregar static a todas las funciones que no estén declaradas en el .h
- Arreglar la forma en la que se mandan los mensajes del server al cliente, la cantidad de caracteres que se van a recibir tiene que estar concatenada con los caracteres, y tiene que estar al principio del buffer
- Agregar la primitiva release a los TDA's que no la tengan
- Escribir mensajes de error para que printee cuando se reciba un comando invalido
- Sacar el chequeo de rango de posicion y de valor numerico del sudoku porque lo hace todo el cliente
- Hacer .h con constantes generales para el tp (chequeo de errores y caracteres q importan tanto para el cliente como para el server)
- Agregar el codigo del TIME_WAIT
- Ver si hay que mejorar la forma en la que se inicializa el sudoku con el archivo (como se acceden a los nros en la linea que se saca del archivo)
- IMPORTANTE: Terminar de hacer el sudoku_handler_init
- Borrar el struct limits_t y hacer la firma de la funcion en distintas lineas
- IMPORTANTE: Agregar static a todas las funciones auxiliares de los tdas


DUDAS:

- Está mal tener un .h que solo tenga defines que se van a usar en todo el proyecto?
- Init tiene que inicializar todos los elementos del struct? O solo los que yo considere necesarios?
- Por qué se pone const puntero si en vez de eso se puede pasar una copia del tda y así se asegura que no se puede cambiar?
- Estoy mezclando cosas si en el init del sudoku leo el archivo? Lo mismo si devuelvo directamente la misma metriz que quiero imprimir
- Programo las cosas independientemente de lo que se pida? (ej: se chequea que en la inicializacion el sudoku reciba numeros de 0 a 9 aunque no se vaya a recibir un archivo mal hecho)
- Usamos setsockopt para ver si la dirección no está disponible por un TIME_WAIT?
- Las constantes de las bibliotecas van en el .c o en el .h?
- IMPORANTE: Para enviar un dato por un socket siempre hay que castear su dirección a char* y mandar un array de chars (bytes)?
- Está mal incluir un header solo por los defines? (incluyo server.h en client para que solo esten definidos en un único lugar los caracteres de los comandos para no tenes que cambiarlos en distintos lugares en caso de alguna modificación)
- IMPORTANTE: ver duda en verify de server.c
- IMPORTANTE: que conviene: ¿Pasar un array como parametro y modificarlo para que guarde lo que queramos?¿O devolverlo con memoria dinamica? Ver sudoku_get_board como referencia
