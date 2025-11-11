#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define NUM_FILOSOFOS 5
#define PENSAR_MS 4000
#define COMER_MS 2500

// --- LEDs RGB padrão da FRDM-KL25Z ---
static const struct gpio_dt_spec led_r = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led_g = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led_b = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

// --- Mutexes (um por garfo) ---
static struct k_mutex garfos[NUM_FILOSOFOS];

// --- Controle de LEDs (estado de quem está comendo) ---
void set_leds(int r, int g, int b) {
    gpio_pin_set_dt(&led_r, r);
    gpio_pin_set_dt(&led_g, g);
    gpio_pin_set_dt(&led_b, b);
}

void atualizar_leds(int filosofo, bool comendo) {
    static bool estado[NUM_FILOSOFOS] = {false, false, false, false, false};
    estado[filosofo] = comendo;

    int r = 0, g = 0, b = 0;

    if (estado[0]) r = 1;             // Filósofo 1 → vermelho
    if (estado[1]) g = 1;             // Filósofo 2 → verde
    if (estado[2]) b = 1;             // Filósofo 3 → azul
    if (estado[3]) { r = 1; g = 1; }  // Filósofo 4 → amarelo
    if (estado[4]) { g = 1; b = 1; }  // Filósofo 5 → ciano

    set_leds(r, g, b);
}

// --- Função principal de cada filósofo ---
void filosofo(void *p1, void *p2, void *p3) {
    int id = (int)(intptr_t)p1;
    int esq = id;
    int dir = (id + 1) % NUM_FILOSOFOS;

    while (1) {
        printk("🤔 Filósofo %d está pensando...\n", id + 1);
        k_msleep(PENSAR_MS + id * 400);

        // Evita deadlock: o último filósofo pega na ordem inversa
        if (id == NUM_FILOSOFOS - 1) {
            k_mutex_lock(&garfos[dir], K_FOREVER);
            k_mutex_lock(&garfos[esq], K_FOREVER);
        } else {
            k_mutex_lock(&garfos[esq], K_FOREVER);
            k_mutex_lock(&garfos[dir], K_FOREVER);
        }

        printk("🍝 Filósofo %d está COMENDO\n", id + 1);
        atualizar_leds(id, true);
        k_msleep(COMER_MS + id * 300);

        // Solta os garfos
        k_mutex_unlock(&garfos[esq]);
        k_mutex_unlock(&garfos[dir]);
        atualizar_leds(id, false);

        printk("😌 Filósofo %d terminou de comer\n\n", id + 1);
        k_msleep(800);
    }
}

// --- Criação das threads ---
K_THREAD_DEFINE(f1_id, 512, filosofo, (void *)0, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(f2_id, 512, filosofo, (void *)1, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(f3_id, 512, filosofo, (void *)2, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(f4_id, 512, filosofo, (void *)3, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(f5_id, 512, filosofo, (void *)4, NULL, NULL, 5, 0, 0);

void main(void) {
    printk("\n=== 🍽️ PROBLEMA DOS FILÓSOFOS (Mutex — Zephyr + FRDM-KL25Z) ===\n\n");

    // Inicializa GPIOs
    gpio_pin_configure_dt(&led_r, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led_g, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led_b, GPIO_OUTPUT_INACTIVE);

    // Inicializa mutexes (garfos)
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        k_mutex_init(&garfos[i]);
    }
}
