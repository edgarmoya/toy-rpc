#include "../rpc.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    reportable_t parent;

    char cardNumber[20];
    int isValid;
} my_struct_t;

void *parse_parameters(void *data)
{
    const char *buf = (const char *)(data);
    my_struct_t *d = (my_struct_t *)(malloc(sizeof(my_struct_t)));

    if (d)
    {
        sscanf(buf, "%s", d->cardNumber);
    }

    return (void *)d;
}

/* El siguiente método implementa el algoritmo de Luhn utilizado 
   para validar números de tarjeta de crédito. */
void *do_work(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    int sum = 0;                               // Variable para almacenar la suma de los dígitos
    int numDigits = strlen(d->cardNumber);     // Longitud del número de tarjeta
    int isAlternateDigit = 0;                  // Variable para indicar si el dígito actual es multiplicado por 2 o no

    for (int i = numDigits - 1; i >= 0; i--) {
        int digit = d->cardNumber[i] - '0';    // Convierte el carácter numérico en un valor entero

        if (isAlternateDigit) {                // Si el dígito actual es uno de los dígitos alternos comenzando desde el último dígito
            digit *= 2;                        // Duplica el valor del dígito actual
            if (digit > 9) {                   
                digit = (digit % 10) + 1;      // Si el valor duplicado es mayor que 9, suma los dígitos individuales
            }
        }

        sum += digit;                          // Agrega el dígito a la suma acumulada
        isAlternateDigit = !isAlternateDigit;  // Alterna los dígitos
    }

    d->isValid = (sum % 10 == 0);              // Comprueba si la suma total es divisible por 10 para determinar la validez del número de tarjeta
    return data;
}

reportable_t *report(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->parent.data = (char *)(malloc(255 * sizeof(char)));

    if (d->isValid)
    {
        snprintf(d->parent.data, 255, "Card '%s' is valid\n", d->cardNumber);
    }
    else
    {
        snprintf(d->parent.data, 255, "Card '%s' is not valid\n", d->cardNumber);
    }
    
    d->parent.len = strlen(d->parent.data);

    return (reportable_t *)(data);
}

void clean_up(void *params, void *result, reportable_t *report)
{
    if (report && report->data)
    {
        free(report->data);
    }

    if (params)
    {
        free(params);
    }
}