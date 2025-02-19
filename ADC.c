#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"

#define I2C_BUS i2c1
#define I2C_DATA_PIN 14
#define I2C_CLOCK_PIN 15
#define OLED_ADDRESS 0x3C
#define JOYSTICK_X_AXIS_PIN 27
#define JOYSTICK_Y_AXIS_PIN 26
#define JOYSTICK_BUTTON_PIN 22
#define BUTTON_ACTION_PIN 5
#define RED_LED_PIN 13
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12

#define PWM_MAX_VALUE 4095

// Variáveis globais
ssd1306_t oled_display;
bool pwm_active = true;
bool green_led_state = false;
int border_thickness = 2;

volatile uint32_t last_joystick_press = 0;
volatile uint32_t last_button_press = 0;
const uint32_t debounce_delay = 200;

// Função de interrupção para tratar eventos dos botões
void handle_button_press(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    // Verifica se o botão do joystick foi pressionado e aplica debounce
    if (gpio == JOYSTICK_BUTTON_PIN && (current_time - last_joystick_press > debounce_delay)) {
        last_joystick_press = current_time;
        green_led_state = !green_led_state;
        gpio_put(GREEN_LED_PIN, green_led_state);
        border_thickness = (border_thickness == 2) ? 4 : 2;
    } 
    // Verifica se o botão A foi pressionado e aplica debounce
    else if (gpio == BUTTON_ACTION_PIN && (current_time - last_button_press > debounce_delay)) {
        last_button_press = current_time;
        pwm_active = !pwm_active;
    }
}

// Função para configurar um pino como saída PWM
void configure_pwm_output(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint pwm_slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(pwm_slice, PWM_MAX_VALUE);
    pwm_set_enabled(pwm_slice, true);
}

// Função para ajustar o brilho de um LED usando PWM
void adjust_led_intensity(uint pin, uint16_t intensity) {
    uint pwm_slice = pwm_gpio_to_slice_num(pin);
    uint pwm_channel = pwm_gpio_to_channel(pin);
    pwm_set_chan_level(pwm_slice, pwm_channel, intensity);
}

int main() {
    stdio_init_all();

    // Inicialização do I2C para o display OLED
    i2c_init(I2C_BUS, 400 * 1000);
    gpio_set_function(I2C_DATA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_CLOCK_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_DATA_PIN);
    gpio_pull_up(I2C_CLOCK_PIN);

    // Inicialização do display OLED
    ssd1306_init(&oled_display, 128, 64, false, OLED_ADDRESS, I2C_BUS);
    ssd1306_config(&oled_display);
    ssd1306_fill(&oled_display, false);
    ssd1306_send_data(&oled_display);

    // Inicialização do ADC para o joystick
    adc_init();
    adc_gpio_init(JOYSTICK_X_AXIS_PIN);
    adc_gpio_init(JOYSTICK_Y_AXIS_PIN);

    // Configuração dos botões com interrupções
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &handle_button_press);

    gpio_init(BUTTON_ACTION_PIN);
    gpio_set_dir(BUTTON_ACTION_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_ACTION_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_ACTION_PIN, GPIO_IRQ_EDGE_FALL, true, &handle_button_press);

    // Configuração dos LEDs
    configure_pwm_output(RED_LED_PIN);
    configure_pwm_output(BLUE_LED_PIN);
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);

    // Loop principal
    uint16_t joystick_x, joystick_y;
    while (true) {
        // Leitura do eixo X do joystick
        adc_select_input(1);
        joystick_x = adc_read();

        // Leitura do eixo Y do joystick
        adc_select_input(0);
        joystick_y = adc_read();

        // Calcula os valores de PWM para os LEDs com base na posição do joystick
        uint16_t red_intensity = pwm_active ? abs(joystick_x - 2048) : 0;
        uint16_t blue_intensity = pwm_active ? abs(joystick_y - 2048) : 0;

        // Ajusta o brilho do LED vermelho
        if (joystick_x > 2100 || joystick_x < 1900) {
            adjust_led_intensity(RED_LED_PIN, red_intensity);
        } else {
            adjust_led_intensity(RED_LED_PIN, 0);
        }

        // Ajusta o brilho do LED azul
        if (joystick_y > 2100 || joystick_y < 1900) {
            adjust_led_intensity(BLUE_LED_PIN, blue_intensity);
        } else {
            adjust_led_intensity(BLUE_LED_PIN, 0);
        }

        // Converte os valores do joystick para coordenadas do display
        uint8_t square_x = (joystick_x * (128 - 8)) / 4095; // WIDTH = 128
        uint8_t square_y = ((4095 - joystick_y) * (64 - 8)) / 4095; // HEIGHT = 64

        // Atualiza o display OLED
        ssd1306_fill(&oled_display, false);
        ssd1306_rect(&oled_display, square_y, square_x, 8, 8, true, true); // Desenha o quadrado

        // Desenha a borda da tela, com tamanho alternável
        for (int i = 0; i < border_thickness; i++) {
            ssd1306_rect(&oled_display, i, i, 128 - (2 * i), 64 - (2 * i), true, false);
        }

        ssd1306_send_data(&oled_display); // Envia os dados para o display

        sleep_ms(50);
    }
}