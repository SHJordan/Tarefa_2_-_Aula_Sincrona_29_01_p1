#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define LED_RED 13   // GPIO do LED Vermelho
#define LED_GREEN 11 // GPIO do LED Verde
#define LED_BLUE 12  // GPIO do LED Azul (não usado)

struct repeating_timer timer;

bool repeating_timer_callback(struct repeating_timer *t)
{
    static int state = 0; // 0 = Vermelho, 1 = Amarelo, 2 = Verde

    // Desliga todos os LEDs
    gpio_put(LED_RED, 0);
    gpio_put(LED_GREEN, 0);
    gpio_put(LED_BLUE, 0);

    switch (state)
    {
    case 0: // Vermelho (3s)
        gpio_put(LED_RED, 1);
        state = 1;
        break;
    case 1: // Amarelo (3s): Vermelho + Verde
        gpio_put(LED_RED, 1);
        gpio_put(LED_GREEN, 1);
        state = 2;
        break;
    case 2: // Verde (3s)
        gpio_put(LED_GREEN, 1);
        state = 0;
        break;
    }
    return true;
}

int main()
{
    stdio_init_all();

    // Configuração dos LEDs
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    // Inicia com o LED vermelho ligado
    gpio_put(LED_RED, 1);

    // Configura o temporizador para 3 segundos (primeira transição após 3s)
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer); // Valor positivo!

    // Loop principal
    while (true)
    {
        printf("Semáforo operando...\n");
        sleep_ms(1000);
    }
}