# TP1_Taller_1


TO DO:

- See if functions return bools or ints to indicate errors (correct the ones that return ints if bool are chosen)
- Achicar verify_block, leer comentarios de todo el codigo escrito
- Ver si se pueden generalizar los verify en uno solo
- Ver si es mejor devolver memoria dinamica en vez de pasar una matriz por parametro a sudoku_get_board
- Cambiar el atoi por una función char_to_int para no tener que pasar las cosas a un array


DUDAS:

- Está mal no poner un init o destroy si sé que no lo no lo necesita el TDA
- Está mal tener un .h que solo tenga defines que se van a usar en todo el proyecto?
- Init tiene que inicializar todos los elementos del struct? O solo los que yo considere necesarios?
- Está bien hacer un iterador interno privado para un tda? O solo ensuciaría el código del .c? (así no escribo muchas veces el doble for )
- Esta bien poner returns y breaks en las iteraciones?
- Otras funciones en un TDA que no sean primitivas son consideradas globales?
- Está mal usar memset en algo que no sea strings?
- Por qué se pone const puntero si en vez de eso se puede pasar una copia del tda y así se asegura que no se puede cambiar?
- Estoy mezclando cosas si en el init del sudoku leo el archivo?
- Programo las cosas independientemente de lo que se pida? (ej: se chequea que en la inicializacion el sudoku reciba numeros de 0 a 9 aunque no se vaya a recibir un archivo mal hecho)
- No puedo usar fgets porque retorna un char* que no puedo guardar de forma que no me rompa el compilador
- Usamos setsockopt para ver si la dirección no está disponible por un TIME_WAIT?
