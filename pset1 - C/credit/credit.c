#include <cs50.h>
#include <stdio.h>

long int getTwoFirstDigits(long int cardNumber);
int lunhAlgorithm(long int cardNumber);
long int digitCounter(long int cardNumber);

int main(void)
{
    long int cardNumber;

    cardNumber = get_long("Please type your credit card number\n");
    //long int cardNumber = 4509950202997221;


    if (lunhAlgorithm(cardNumber) == 1)
    {
        int twoFirstDigits = getTwoFirstDigits(cardNumber);
        int cardLength = digitCounter(cardNumber);

        //is AMEX? All American Express numbers start with 34 or 37 and have a length of 15
        if ((twoFirstDigits == 34 ||  twoFirstDigits == 37) && cardLength == 15)
        {
            printf("AMEX\n");
        }
        //is MASTERCARD? MasterCard numbers start with 51, 52, 53, 54, or 55 and have a length of 16
        else if ((twoFirstDigits > 50 && twoFirstDigits < 56) && cardLength == 16)
        {
            printf("MASTERCARD\n");
        }
        //is VISA? VIsa cards start with 4 and have a length of 13 or 16
        else if (((twoFirstDigits / 10) == 4) && (cardLength == 13 || cardLength == 16))
        {
            printf("VISA\n");
        }
         else
        {
        printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

// Checks
    //Two First Digits
    //printf("fist two digits %li\n", getTwoFirstDigits(cardNumber));
    //Lunh's Algorithm validation
    //printf("Veredict is %s\n", lunhAlgorithm(cardNumber)? "True" : "False");
    //printf("The leght of the card is of %li\n", digitCounter(cardNumber));
}

// esta funcion nos devuelve un boolean: true para una tarjeta validada por Lunh's Algorithm o false para una que no pasa la validacion.
int lunhAlgorithm(long int cardNumber)
{
    // Keep es la suma total de Lunh's Algorithm
    int keep = 0;
    // Discard es una variable temporal para almacenar el numero de la tarjeta que voy modificando en cada iteracion
    long int discard = cardNumber;

    // Este ciclo hace el calculo del algoritmo el cual si el valor de keep termina en cero, 20,30,70 etc la tarjeta es valida
    // si termina en cualquier otro numero la tarjeta no es valida.
    do
    {
        //Se le suma a Keep el ultimo valor de la tarjeta
        keep += discard % 10;
        //Se descarta el ultimo valor de la tarjeta (ya sumado a Keep previamente)
        discard = discard / 10;
        //Se multiplica por dos el ultimo digito de la tarjeta
        int module2 = (discard % 10) * 2;

        // Este if else sirve porque si el resultado de module2 da mas de 9 hay que:
        if (module2 > 9)
        {
            // sumar entre si los dos digitos, es decir 11 suma a Keep 2 no 11
            keep += (module2 % 10) + 1;
        }
        else
        {
            // si da menos de 9 sumar directo a Keep el resultado
            keep += module2;
        };
        // sacarle ese ultimo numero con el que se estuvo trabajando en el ciclo y arrancar devuelta
        discard = discard / 10;

    }
    // repetir el loop mientras hayan numeros de la tarjeta de credito
    while (discard > 0);

    bool veredict;
    
    if ((keep % 10) == 0)
    {
        veredict = true;
    }
    else
    {
        veredict = false;
    }

    return veredict;

}

// esta funcion homebrew devuelve los dos primeros numeros de una tarjeta de credito dada como input
long int getTwoFirstDigits(long int cardNumber)
{
    long int twoFirstDigits = cardNumber;

    while (twoFirstDigits > 99)
    {
        twoFirstDigits = twoFirstDigits / 10;

    }

    return twoFirstDigits;

}

// esta funcion toma como argumento un numero y devuelve el numero de digitos del mismo == 457846 --> 6
long int digitCounter(long int cardNumber)
{
    long int counter = 1;

    while (cardNumber > 9)
    {
        cardNumber /= 10;
        counter ++;
    }

    return counter;
}