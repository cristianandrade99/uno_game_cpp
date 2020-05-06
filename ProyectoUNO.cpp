#include <iostream>
#include <vector>
#include <string>
#include <ctime> 

using namespace std;

// PARaMETROS DEL JUEGO
string tipo_cambio_sentido = "cambio_sentido";
string tipo_cambio_color = "cambio_color";
string tipo_color_numero = "color_numero";
string tipo_bloqueo = "bloqueo";
string tipo_mas2 = "mas2";
string tipo_mas4 = "mas4";

vector<string> colores = { "azul","verde","rojo","amarillo" };
int num_numeros = 10;
int num_cartas_por_numero = 2;
int num_mas2_por_color = 2;
int num_cambio_sentido_por_color = 2;
int num_bloqueo_por_color = 2;
int num_cambio_color = 4;
int num_mas4 = 4;
int num_cartas_por_jugador = 7;
int numero_jugadores = 4;

// calcula un numero aleatorio entre min y max
int randomint(int min, int max)
{
    return (rand() % max) + min;
}

// imprime en consola con caracter de retorno
void print(string value)
{
    cout << value << endl;
}

// Estructura que representa una carta
struct Carta
{
    string tipo;
    string color;
    int numero;

    void init_values(string pTipo, string pColor, int pNumero)
    {
        tipo = pTipo;
        color = pColor;
        numero = pNumero;
    }
};

// Traslada una carta en la posicion por parametro de un arreglo origen a un arreglo destino y retorna dicha carta
vector<vector<Carta>> trasladar_carta(int pIndiceCarta, vector<Carta> vector_origen, vector<Carta> vector_destino)
{
    Carta carta_trasladada = vector_origen[pIndiceCarta];
    vector_origen.erase(vector_origen.begin() + pIndiceCarta);
    vector_destino.push_back(carta_trasladada);

    vector<vector<Carta>> vectores = { vector_origen ,vector_destino };
    return vectores;
}

// Estructura que representa un jugador
struct Jugador
{
    string nombre;
    vector<Carta> cartas;

    // Registra el nombre del jugador
    void init_values(string pNombre)
    {
        nombre = pNombre;
    };
    
    // Come las cartas que llegan por parametro y retorna la baraja
    vector<Carta> comerCartas(vector<Carta> pBaraja, int p_NumCartas)
    {
        vector<Carta> baraja_actual = pBaraja;

        for (int i = 0; i < p_NumCartas; i++)
        {
            vector<vector<Carta>> vectores = trasladar_carta(baraja_actual.size() - 1, baraja_actual, cartas);
            baraja_actual = vectores[0];
            cartas = vectores[1];
        }

        return baraja_actual;
    }
};

// Retorna un string con la informacion de una carta
string infoCarta(Carta pCarta)
{
    return "tipo: " + pCarta.tipo + " - color: " + pCarta.color + " - numero: " + to_string(pCarta.numero);
}

// Estructura que representa el juego UNO
struct Juego
{
    vector<Carta> baraja;
    vector<Carta> baraja_juego;
    vector<Jugador> jugadores;

    int indice_turno_actual = 0;
    int valor_modificacion_sentido_actual = 1;

    bool hay_ganador = false;
    bool jugador_puso_carta_en_turno = false;

    Carta carta_actual;
    Jugador jugador_actual;
    
    // Inicializa los valores del juego
    void actualizar_carta_jugador_actual()
    {
        carta_actual = baraja_juego[baraja_juego.size()-1];
        jugador_actual = jugadores[indice_turno_actual];
    }

    // Crea una carta con la informacion por parametro y la agrega a la baraja
    void insertar_carta_baraja(string pTipo, string pColor, int pNumero)
    {
        Carta carta;
        carta.init_values(pTipo, pColor, pNumero);
        baraja.push_back(carta);
    }

    // Llena la baraja con las cartas segun los parametros definidos
    void llenar_baraja()
    {
        for (int i = 0; i < colores.size(); i++)
        {
            // Se crean las cartas de tipo color numero
            for (int j = 0; j < num_numeros; j++)
            {
                for (int k = 0; k < num_cartas_por_numero; k++)
                {
                    if (!(j == 0 && k == 1))
                    {
                        insertar_carta_baraja(tipo_color_numero, colores[i], j);
                    }
                }
            }

            // Se crean las cartas de tipo mas2
            for (int k = 0; k < num_mas2_por_color; k++)
            {
                insertar_carta_baraja(tipo_mas2, colores[i], -2);
            }

            // Se crean las cartas de tipo cambio de sentido
            for (int k = 0; k < num_cambio_sentido_por_color; k++)
            {
                insertar_carta_baraja(tipo_cambio_sentido, colores[i], -2);
            }

            // Se crean las cartas de tipo bloqueo
            for (int k = 0; k < num_bloqueo_por_color; k++)
            {
                insertar_carta_baraja(tipo_bloqueo, colores[i], -2);
            }
        }

        // Se crean las cartas de cambio de color
        for (int k = 0; k < num_cambio_color; k++)
        {
            insertar_carta_baraja(tipo_cambio_color, "none", -2);
        }

        // Se crean las cartas de mas4
        for (int k = 0; k < num_mas4; k++)
        {
            insertar_carta_baraja(tipo_mas4, "none", -2);
        }
    }

    // Mezcla aleatoriamente las cartas de la baraja
    void barajar_cartas()
    {
        vector<Carta> baraja_auxiliar;
        int numero_cartas = baraja.size();
        for (int i = 0; i < numero_cartas-1; i++)
        {
            int indice_aleatorio = randomint(0, baraja.size() - 1);
            vector<vector<Carta>> vectores = trasladar_carta(indice_aleatorio, baraja, baraja_auxiliar);
            baraja = vectores[0];
            baraja_auxiliar = vectores[1];
        }

        baraja = baraja_auxiliar;
    }

    // Ingresa los jugadores por consola
    void ingresarJugadores(int pNumJugadores)
    {
        for (int i = 0; i < pNumJugadores; i++)
        {
            cout << "Ingrese Nombre Jugador " + to_string(i + 1) + " :";
            string nombre;
            cin >> nombre;
            Jugador jugador;
            jugador.init_values(nombre);
            jugadores.push_back(jugador);
        }
        print("");
    }

    // Reparte las cartas a cada uno de los jugadores
    void repartir_cartas()
    {
        for (int i = 0; i < jugadores.size(); i++)
        {
            for (int j = 0; j < num_cartas_por_jugador; j++)
            {
                int indice_aleatorio = randomint(0, baraja.size() - 1);
                vector<vector<Carta>> vectores = trasladar_carta(indice_aleatorio, baraja, jugadores[i].cartas);
                baraja = vectores[0];
                jugadores[i].cartas = vectores[1];
            }
        }
    }

    // Cambia el indice del turno actual segun la diferencia establecida
    void cambiar_indice_turno_actual(int diferencia)
    {
        if (indice_turno_actual + diferencia >= jugadores.size())
        { indice_turno_actual += diferencia - jugadores.size(); }
        else if (indice_turno_actual + diferencia <= -1)
        { indice_turno_actual += jugadores.size() + diferencia ; }
        else { indice_turno_actual += diferencia; }
    }

    // Selecciona la primera carta para arrancar el juego
    void poner_primera_carta()
    {
        int indice_aleatorio = randomint(0, baraja.size() - 1);
        vector<vector<Carta>> vectores = trasladar_carta(indice_aleatorio, baraja, baraja_juego);
        baraja = vectores[0];
        baraja_juego = vectores[1];
    }

    // Verifica si al jugador actual le toca comer cartas
    bool verificar_jugador_come_cartas()
    {
        return carta_actual.tipo == tipo_mas4 || carta_actual.tipo == tipo_mas2;
    }

    // Verifica si saltaron al jugador actual
    bool verificar_saltaron_jugador()
    {
        return carta_actual.tipo == tipo_bloqueo || carta_actual.tipo == tipo_cambio_sentido;
    }

    // Indica si una carta por lanzar o lanzada es correcta
    bool carta_por_lanzar_lanzada_correcta(Carta pCartaActual, Carta pCarta_a_lanzar_lanzada)
    {
        bool carta_correcta = false;

        if (pCarta_a_lanzar_lanzada.tipo == tipo_cambio_color || pCarta_a_lanzar_lanzada.tipo == tipo_mas4)
        {
            carta_correcta = true;
        }
        else
        {
            if (pCartaActual.tipo == tipo_cambio_color)
            {
                if (pCarta_a_lanzar_lanzada.color == pCartaActual.color)
                {
                    carta_correcta = true;
                }
            }
            else if (pCartaActual.tipo == tipo_color_numero)
            {
                if (pCarta_a_lanzar_lanzada.color == pCartaActual.color || pCarta_a_lanzar_lanzada.numero == pCartaActual.numero)
                {
                    carta_correcta = true;
                }
            }
            else
            {
                if (pCarta_a_lanzar_lanzada.color == pCartaActual.color)
                {
                    carta_correcta = true;
                }
            }
        }

        return carta_correcta;
    }

    // Verifica si el jugador puede lanzar carta segun las que tiene en su poder
    bool verificar_jugador_puede_lanzar_carta()
    {
        bool puede_lanzar = false;
        for (int i = 0; i < jugador_actual.cartas.size() && !puede_lanzar; i++)
        {
            puede_lanzar = carta_por_lanzar_lanzada_correcta(carta_actual, jugador_actual.cartas[i]);
        }

        return puede_lanzar;
    }

    // Retorna el numero de cartas que tiene que comer el jugador
    int numero_cartas_comer()
    {
        int numero_cartas = 0;
        if (carta_actual.tipo == tipo_mas2) { numero_cartas = 2; }
        else if (carta_actual.tipo == tipo_mas4) { numero_cartas = 4; }
        return numero_cartas;
    }

    // Actualiza el turno segun la forma en como saltaron al jugador
    void actualizar_indice_sentido_turno_segun_salto_jugador()
    {
        if (carta_actual.tipo == tipo_cambio_sentido)
        { 
            valor_modificacion_sentido_actual = -valor_modificacion_sentido_actual;
            cambiar_indice_turno_actual(2*valor_modificacion_sentido_actual);
        }
        else if (carta_actual.tipo == tipo_bloqueo)
        {
            cambiar_indice_turno_actual(valor_modificacion_sentido_actual);
        }
    }

    // Indica por consola al jugador las cartas que tiene
    void mostrar_cartas_jugador_actual()
    {
        print("Estas son sus cartas:");
        for (int i = 0; i < jugador_actual.cartas.size(); i++)
        {
            print(to_string(i) + " - " + infoCarta(jugador_actual.cartas[i]));
        }
        print("");
    }

    // Retorna el indice de la carta escogida por el jugador para lanzar
    int solicitar_indice_carta_jugador()
    {
        cout << "Ingresa el indice de la carta que quieres lanzar: ";
        int indice;
        cin >> indice;
        return indice;
    }

    // Indica si el jugador escogio una carta que si puede ser lanzada segun la carta actual
    bool jugador_escogio_carta_correcta(int pIndice_carta_escogida)
    {
        bool escogio_correcta = false;

        if (pIndice_carta_escogida >= 0 && pIndice_carta_escogida <= jugador_actual.cartas.size() - 1)
        {
            escogio_correcta = carta_por_lanzar_lanzada_correcta(carta_actual, jugador_actual.cartas[pIndice_carta_escogida]);
        }

        return escogio_correcta;
    }

    // Se pregunta al jugador para que escoja un color
    int pregunta_a_jugador_a_que_color_cambia()
    {
        int indice_color;
        print("Indique el indice del color que va a escoger:");

        bool tomo_indice_correcto = false;
        while (!tomo_indice_correcto)
        {
            for (int i = 0; i < colores.size(); i++)
            {
                cout << " [" + to_string(i) + "] " + colores[i] +"   ";
            }
            cout << ": ";
            
            cin >> indice_color;
            tomo_indice_correcto = indice_color == 0 || indice_color == 1 || indice_color == 2 || indice_color == 3;
            if (!tomo_indice_correcto) { print("Por favor seleccione un indice valido"); }
        }

        return indice_color;
    }

    // Logica para verificar si hay que preguntar a cual color cambia
    void logica_pregunta_jugador_en_caso_carta_especial(int pIndice_carta_escogida)
    {
        string tipo_escogido = jugador_actual.cartas[pIndice_carta_escogida].tipo;
        if (tipo_escogido == tipo_cambio_color || tipo_escogido == tipo_mas4)
        {
            int indice_color = pregunta_a_jugador_a_que_color_cambia();
            jugador_actual.cartas[pIndice_carta_escogida].color = colores[indice_color];
        }
    }

    //
    void logica_indicar_decir_uno()
    {
        int indice_decir_uno = preguntar_jugador_si_decir_UNO();
        if (indice_decir_uno == 0 && jugador_actual.cartas.size()!=1)
        {
            print("El jugador " + jugador_actual.nombre + " dijo !UNO y NO tiene solamente una carta");
        }
        else if(indice_decir_uno == 0 && jugador_actual.cartas.size() == 1)
        {
            print("El jugador " + jugador_actual.nombre + " dijo !UNO correctamente");
        }
    }

    // Realiza el proceso en el que el jugador puede lanzar una carta
    void proceso_jugador_lanza_carta()
    {
        int indice_carta_escogida;

        bool escogio_carta_correcta = false;
        while (!escogio_carta_correcta)
        {
            indice_carta_escogida = solicitar_indice_carta_jugador();
            escogio_carta_correcta = jugador_escogio_carta_correcta(indice_carta_escogida);

            if (!escogio_carta_correcta) { print("Escogiste una carta que no puedes usar, por favor intenta de nuevo"); }
        }

        if ( jugador_actual.cartas.size()==1 ) { hay_ganador = true; }
        logica_pregunta_jugador_en_caso_carta_especial(indice_carta_escogida);
        lanzar_carta_por_jugador_actual(indice_carta_escogida);

        logica_indicar_decir_uno();

        cambiar_indice_turno_actual(valor_modificacion_sentido_actual);
        jugador_puso_carta_en_turno = true;
    }

    // Informa que al jugador actual le toco comer cartas
    void informar_jugador_actual_come_cartas(int pNumeroCartas)
    {
        print("El jugador " + jugador_actual.nombre + " comio " + to_string(pNumeroCartas) + " cartas");
    }

    // Indica cual es el jugador con el turno actual
    void informar_turno_jugador_actual()
    {
        print("El turno actual corresponde al jugador " + jugador_actual.nombre);
    }

    // Indica que el jugador actual fue saltado
    void informar_jugador_actual_fue_saltado()
    {
        print("El jugador " + jugador_actual.nombre + " perdio el turno");
    }

    // Pregunta al jugador actual que decision va a tomar
    int preguntar_jugador_actual_accion()
    {
        int indice;
        print("Indique el indice de la accion que va a realizar:");

        bool tomo_indice_correcto = false;
        while (!tomo_indice_correcto)
        {
            cout << "[0] Lanzar carta - [1] Tomar carta mazo: ";
            cin >> indice;
            tomo_indice_correcto = indice == 0 || indice == 1;
            if (!tomo_indice_correcto) { print("Por favor seleccione un indice valido"); }
        }

        return indice;
    }

    // Pregunta al jugador actual si desea decir uno
    int preguntar_jugador_si_decir_UNO()
    {
        int indice;
        print("Indique si va a decir !UNO escribiendo el indice correspondiente");

        bool tomo_indice_correcto = false;
        while (!tomo_indice_correcto)
        {
            cout << "[0] SI va a decir !UNO - [1] NO va a decir !UNO: ";
            cin >> indice;
            tomo_indice_correcto = indice == 0 || indice == 1;
            if (!tomo_indice_correcto) { print("Por favor seleccione un indice valido"); }
        }

        return indice;
    }

    // Agrega la carta lanzada por el jugador a la baraja de juego, NO a la bajara general
    void lanzar_carta_por_jugador_actual(int pIndice_carta_escogida)
    {
        vector<vector<Carta>> vectores = trasladar_carta(pIndice_carta_escogida, jugador_actual.cartas, baraja_juego);
        jugador_actual.cartas = vectores[0];
        baraja_juego = vectores[1];
    }

    // Proceso de la toma de decision de un jugador para jugar
    void proceso_toma_decision_jugar()
    {
        int accion;
        bool tomo_accion_correcta = false;
        while (!tomo_accion_correcta)
        {
            accion = preguntar_jugador_actual_accion();
            if (accion == 0 && verificar_jugador_puede_lanzar_carta())
            {
                tomo_accion_correcta = true;
                proceso_jugador_lanza_carta();
            }
            if (accion == 1 && !verificar_jugador_puede_lanzar_carta())
            {
                tomo_accion_correcta = true;
                baraja = jugador_actual.comerCartas(baraja, 1);
                if (verificar_jugador_puede_lanzar_carta())
                {
                    proceso_jugador_lanza_carta();
                }
                else
                {
                    cambiar_indice_turno_actual(valor_modificacion_sentido_actual);
                }
            }
            if (!tomo_accion_correcta) { print("Por favor tome una accion valida"); }
        }
    }

    // Informa en el turno actual la carta actual
    void informar_carta_actual()
    {
        print("");
        print("La carta para el turno actual es la siguiente:");
        print(infoCarta(carta_actual));
        print("");
    }

    // logica del juego
    void logica_turno()
    {
        actualizar_carta_jugador_actual();
        informar_carta_actual();
        informar_turno_jugador_actual();
        mostrar_cartas_jugador_actual();
        
        if (!jugador_puso_carta_en_turno)
        {
            proceso_toma_decision_jugar();
        }
        else
        {
            if (verificar_jugador_come_cartas())
            {
                int numero_cartas = numero_cartas_comer();
                informar_jugador_actual_come_cartas(numero_cartas);
                baraja = jugador_actual.comerCartas(baraja, numero_cartas);
                cambiar_indice_turno_actual(valor_modificacion_sentido_actual);
                jugador_puso_carta_en_turno = false;
            }
            else if (verificar_saltaron_jugador())
            {
                informar_jugador_actual_fue_saltado();
                actualizar_indice_sentido_turno_segun_salto_jugador();
                jugador_puso_carta_en_turno = false;
            }
            else
            {
                proceso_toma_decision_jugar();
            }
        }
    }
};

// MeTODOS AUXILIARES

// Imprime la informacion de las cartas que llegan por parametro
void printcartas(vector<Carta> cartas)
{
    for (int i = 0; i < cartas.size(); i++)
    {
        print("tipo: " + cartas[i].tipo + " - color: " + cartas[i].color + " - numero: " + to_string(cartas[i].numero));
    }
    print("total cards: " + to_string(cartas.size()));
    print("");
}

// Imprime las cartas de cada uno de los jugadores por parametro
void printjugadores(vector<Jugador> jugadores)
{
    for (int i = 0; i < jugadores.size(); i++)
    {
        print(jugadores[i].nombre);
        printcartas(jugadores[i].cartas);
    }
}

// Metodo principal
int main()
{
    // Semilla de aletoriedad
    srand((int)time(0));

    // Se inicializa el juego
    Juego uno;

    // Se crean todas las cartas
    uno.llenar_baraja();

    // Se barajan las cartas
    uno.barajar_cartas();
    
    // Se crean los jugadores
    uno.ingresarJugadores(numero_jugadores);
    
    // Se reparten las cartas a los jugadores
    uno.repartir_cartas();

    // Se saca una carta al azar del mazo para comenzar el juego
    uno.poner_primera_carta();

    // Se lleva a cabo el juego
    while (!uno.hay_ganador)
    {
        uno.logica_turno();
    }

    // Se indica el Ganador
    print("El juego termino, el ganador es "+uno.jugador_actual.nombre);
}

/* COMPROBACIONES

//Se comprueba la creacion de las cartas
print("Comprobacion de la creacion de las cartas");
printcartas(uno.baraja);

// Se comprueba el baraje de las cartas
print("Comprobacion del baraje de las cartas");
printcartas(uno.baraja);

// Se comprueba la reparticion de las cartas
print("Comprobacion de la reparticion de las cartas");
printcartas(uno.baraja);
printjugadores(uno.jugadores);

*/