#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <cstdlib>
using namespace std;

// Variables globales
int vidas, puntos;
bool ganador;
int const screenHeight = 20;
int const screenWidth = 30;
int mapa[screenHeight][screenWidth];
bool pierdeVida;
char lvUno[11][30] = {".............................",
                      ".....#.#.#.#.#.#.#.#.#.#.....",
                      ".............................",
                      ".....####.#.#...####.####....",
                      ".....#..#.#.#...##...#..#....",
                      ".....####.#.#...####.####....",
                      ".....#.#..#..#.#.#...#.#.....",
                      ".....#..#.#...#..###.#..#....",
                      ".............................",
                      ".....#.#.#.#.#.#.#.#.#.#.....",
                      "......#.#.#.#.#.#.#.#.#.#...."};
char lvUnoBKP[11][30];

// Funciones
void cargaArregloBKP();
void reloadLvUno();
void esconderCursor();
void recorreXY(int x, int y);
void display();
void perder();
void configInicial();
void paredes();
void crearLadrillo();
void tablero();
void inputmov();
void mover();
void menu();
void submenu1();
void reglasInstrucciones();
void mostrarPuntaje(int cantidadJugadores);
int cantidadJugadores();
bool ganar();

// Declaracion de struct
struct persona
{
public:
    char nombre[50];
    int puntos;
    bool ganador;
};
// Instancia de persona
struct persona player[5];

// MAIN
int main()
{
    cargaArregloBKP();
    menu();
    int cantidadPlayers = cantidadJugadores();
    for (int i = 0; i < cantidadPlayers; i++)
    {
        puntos = 0;
        ganador = false;
        system("cls");
        printf("Ingrese el nombre del %i Jugador : ", i);
        cin >> player[i].nombre;
        system("cls");
        configInicial();
        esconderCursor();
        reloadLvUno();
        while (vidas > 0)
        {
            Sleep(50);
            display();
            tablero();
            inputmov();
            mover();
        }
        player[i].puntos = puntos;
        player[i].ganador = ganador;
        if (!player[i].ganador)
        {
            perder();
            Sleep(2000);
        }
        else
        {
            Sleep(2000);
        }
    }
    mostrarPuntaje(cantidadPlayers);
}

void cargaArregloBKP()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            if (lvUno[i][j] == '#')
            {
                lvUnoBKP[i][j] = '#';
            }
            else
            {
                lvUnoBKP[i][j] = '.';
            }
            cout << lvUnoBKP[i][j];
        }
    }
}
void reloadLvUno()
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            if (lvUnoBKP[i][j] == '#')
            {
                lvUno[i][j] = '#';
            }
            else
            {
                lvUno[i][j] = '.';
            }
        }
    }
}

bool ganar()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            if (lvUno[i][j] == '#')
            {
                return false;
            }
        }
    }
    ganador = true;
    return true;
}
void mostrarPuntaje(int cantidadPlayers)
{
    for (int i = 0; i < cantidadPlayers; i++)
    {
        cout << "El puntaje de " << player[i].nombre << " es :" << player[i].puntos << "\n";
    }
}

int cantidadJugadores()
{
    system("cls");
    int cantidad;
    cout << "Ingrese cantidad de jugadores: ";
    cin >> cantidad;
    if (cantidad > 10)
    {
        cout << "La cantidad de jugadores no puede ser mayor a 10. Intente nuevamente.\n";
        cantidad = cantidadJugadores();
    }
    return cantidad;
}

void menu()
{   
    system("cls");
    cout << "Juego creado por Ger y Valen \n";
    cout << "\n";
    int opcion, salir;
    submenu1();
    cin >> opcion;
    if(opcion == 2){
        system("cls");
        reglasInstrucciones();
        cin >> salir;
        if(salir == 0){
            menu();
        }
    }else {
        return;
    }
}

void submenu1()
{
    cout << "1= Comenzar  2= Reglas/Instucciones  3= Salir \n";
    cout << "Elegi una opcion: ";
}

void reglasInstrucciones()
{
    cout << "_____Reglas - Instrucciones_____\n";
    cout << "\n";
    cout << "1- Si la pelota toca el suelo -1 vida \n";
    cout << "2- Romper un bloque otorga 5 puntos! \n";
    cout << "3- Los resultados se mostraran al finalizar de jugar todos los jugadores \n";
    cout << "4- Son 5 vidas por jugador \n";
    cout << "5- Si se rompen todos los bloques, con dejar caer la pelota ganaras!! \n";
    cout << "\n";
    cout << "AVISO: Solo es posible hasta 10 jugadores \n";
    cout << "\n";
    cout << "0= Para volver al menu principal \n";

}

struct Ball
{
public:
    int x;
    int y;
    int velocidad;
    int direccion;

    void dibujar()
    {
        mapa[y][x] = 5;
    }

    void moverse()
    {
        if (direccion == 0 && !colision(x - velocidad, y - velocidad))
        {
            y -= velocidad;
            x -= velocidad;
        }
        else if (direccion == 1 && !colision(x + velocidad, y - velocidad))
        {
            x += velocidad;
            y -= velocidad;
        }
        else if (direccion == 2 && !colision(x - velocidad, y + velocidad))
        {
            x -= velocidad;
            y += velocidad;
        }
        else if (direccion == 3 && !colision(x + velocidad, y + velocidad))
        {
            x += velocidad;
            y += velocidad;
        }
    }
    bool colision(int fx, int fy)
    {
        // Si toca abajo resta vida, limpia, reinicia posiciones y direccion
        if (mapa[fy][x] == 8)
        {
            pierdeVida = true;
            x = screenWidth / 2 - 1;
            y = screenHeight - screenHeight / 7 - 3;
            direccion = 4;
            int ganador = ganar();
            if (!ganador)
            {
                vidas--;
                system("cls");
            }
            else
            {
                vidas = 0;
                system("cls");
                cout << "\n";
                cout << "No te asustes, rompiste todos los bloques, Ganaste!";
                Sleep(3000);
            }
            // Si toca un ladrillo
        }
        else if (mapa[fy][fx] != 0 || mapa[y][fx] != 0 || mapa[fy][x] != 0 || mapa[fy][fx] == 2 || mapa[y][fx] == 2 || mapa[fy][x] == 2)
        {
            if (mapa[fy][fx] == 2)
            {
                puntos++;
                lvUno[fy][fx] = '.';
            }
            if (mapa[y][fx] == 2)
            {
                puntos++;
                lvUno[y][fx] = '.';
            }
            if (mapa[fy][x] == 2)
            {
                puntos++;
                lvUno[fy][x] = '.';
            }
            if (mapa[y][fx] != 0)
                rebota(fx, y);
            else if (mapa[fy][x] != 0)
                rebota(x, fy);
            else if (mapa[fy][fx] != 0)
                rebota(fx, fy);
            else if (mapa[fx][fy] != 0)
                rebota(fy, fx);

            return true;
        }
        return false;
    }

    void rebota(int fx, int fy)
    {
        if (direccion == 0)
        {
            if (fx < x)
            {
                direccion = 1;
            }
            else if (fy < y)
            {
                direccion = 2;
            }
            else if (fx < x && fy < y)
            {
                direccion = 0;
            }
        }
        else if (direccion == 1)
        {
            if (fx > x)
            {
                direccion = 0;
            }
            else if (fy < y)
            {
                direccion = 3;
            }
            else if (fx > x && fy < y)
            {
                direccion = 1;
            }
        }
        else if (direccion == 2)
        {
            if (fx < x)
            {
                direccion = 3;
            }
            else if (fy > y)
            {
                direccion = 0;
            }
            else if (fx < x && fy > y)
            {
                direccion = 2;
            }
        }
        else if (direccion == 3)
        {
            if (fx > x)
            {
                direccion = 2;
            }
            else if (fy > y)
            {
                direccion = 1;
            }
            else if (fx > x && fy > y)
            {
                direccion = 3;
            }
        }
    }
};
struct Paddle
{

public:
    int x;
    int y;
    int velocidad;
    char dir;
    int delay;
    int countDelay;

    void dibujar()
    {
        for (int i = 0; i < 10; i++)
        {
            for (int p = 0; p < 4; p++)
            {
                mapa[y][x + p] = 1;
                mapa[y][x - p] = 1;
            }
        }
    }
    void moverse()
    {
        if (countDelay == delay)
        {
            // Defino los limites de la paleta
            if (dir == 'L' && x - velocidad > 3)
            {
                x -= velocidad;
            }
            else if (dir == 'R' && x + velocidad < screenWidth - 4)
            {
                x += velocidad;
            }
            countDelay = 0;
        }
        countDelay++;
        if (pierdeVida)
        {
            dir = 'S';
            x = screenWidth / 2 - 5;
            y = screenHeight - screenHeight / 7 - 1;
            pierdeVida = false;
        }
    }
};

// Definimos el objeto de la clase
Ball ball;
Paddle paddle;

void crearLadrillo()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            if (lvUno[i][j] == '#')
                mapa[i][j] = 2;
        }
    }
}

void mover()
{
    paddle.moverse();
    ball.moverse();
}

// Paredes
void paredes()
{
    // Iteracion de paredes
    for (int i = 0; i < screenHeight; i++)
    {
        for (int k = 0; k < screenWidth; k++)
        {
            if (k == 0 || k == screenWidth - 1)
            {
                mapa[i][k] = 9;
            }
            else if (i == 0)
            {
                mapa[i][k] = 7; // Superior
            }
            else if (i == screenHeight - 1)
            {
                mapa[i][k] = 8; // Inferior
            }
            else
            {
                mapa[i][k] = 0;
            }
        }
    }
}

void configInicial()
{
    srand(time(NULL));
    pierdeVida = false;
    vidas = 5;
    paddle.x = screenWidth / 2 - 5;
    paddle.y = screenHeight - screenHeight / 7 - 1;
    paddle.velocidad = 1;
    paddle.delay = 1;
    ball.x = screenWidth / 2;
    ball.y = screenHeight - screenHeight / 7 - 2;
    ball.velocidad = 1;
    ball.direccion = rand() % 4;
}

void tablero()
{
    paredes();
    paddle.dibujar();
    ball.dibujar();
    crearLadrillo();
}
// Captura de movimientos
void inputmov()
{
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    { // Mascara que consigue el bit mas significativo y verifica realmente que se esta precionando la tecla
        paddle.dir = 'L';
        Sleep(10);
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        paddle.dir = 'R';
        Sleep(10);
    }
    else
    {
        paddle.dir = 'S';
    }
    if (ball.direccion == 4)
        ball.direccion = rand() % 2;
}

// Limpieza de la consola y cout
void perder()
{
    system("cls");
    cout << "----------------- \n";
    cout << "    Perdiste :( \n";
    cout << "----------------- \n";
    system("cls");
}
// Esconder cursor
void esconderCursor()
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// Mueve el cursor de la consola
void recorreXY(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Reemplazo numeros x caracteres
void display()
{
    // Agregar quien esta jugando
    recorreXY(2, 1);
    cout << "Vidas: " << vidas;
    recorreXY(22, 1);
    cout << "Puntos: " << puntos;
    // 9=Laterales 8=Puntaje 7=Superior 5=Pelota 2=Bloques 1=Barra
    for (int i = 0; i < screenHeight; i++)
    {
        for (int k = 0; k < screenWidth; k++)
        {
            recorreXY(k + 2, i + 3);
            if (mapa[i][k] == 9)
                cout << char(219);
            if (mapa[i][k] == 8)
                cout << char(240);
            if (mapa[i][k] == 7)
                cout << char(219);
            if (mapa[i][k] == 5)
                cout << char(254);
            if (mapa[i][k] == 2)
                cout << char(233);
            if (mapa[i][k] == 1)
                cout << char(219);
            if (mapa[i][k] == 0)
                cout << char(32);
        }
    }
}
