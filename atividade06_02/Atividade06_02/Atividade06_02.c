#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22  // GPIO do PWM para o servo
#define LED_RGB_PIN 12 // GPIO do LED RGB (para observação do efeito do PWM)
#define PWM_FREQ 50    // Frequência do PWM para o servo (50Hz)
#define PWM_WRAP 20000 // Contagem máxima do PWM (período de 20ms)

// Configura PWM na GPIO
void setup_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, PWM_WRAP);
    pwm_set_clkdiv(slice_num, 125.0f);
    pwm_set_enabled(slice_num, true);
}

// Define o ângulo do servo
void set_servo_angle(uint gpio, uint pulse_width) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint level = (pulse_width * PWM_WRAP) / 20000;
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio), level);
}

// Ajusta o brilho do LED proporcional ao pulso do servo
void set_led_brightness(uint gpio, uint pulse_width) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint level = (pulse_width * PWM_WRAP) / 2400; // Normaliza brilho entre 0 e 100%
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio), level);
}

int main() {
    stdio_init_all();
    setup_pwm(SERVO_PIN);
    setup_pwm(LED_RGB_PIN);

    while (true) {
        // 180º (2400µs)
        set_servo_angle(SERVO_PIN, 2400);
        set_led_brightness(LED_RGB_PIN, 2400);
        sleep_ms(5000);

        // 90º (1470µs)
        set_servo_angle(SERVO_PIN, 1470);
        set_led_brightness(LED_RGB_PIN, 1470);
        sleep_ms(5000);

        // 0º (500µs)
        set_servo_angle(SERVO_PIN, 500);
        set_led_brightness(LED_RGB_PIN, 500);
        sleep_ms(5000);

        // Movimentação suave 0º → 180º
        for (uint pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_angle(SERVO_PIN, pulse);
            set_led_brightness(LED_RGB_PIN, pulse);
            sleep_ms(10);
        }

        // Movimentação suave 180º → 0º
        for (uint pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_angle(SERVO_PIN, pulse);
            set_led_brightness(LED_RGB_PIN, pulse);
            sleep_ms(10);
        }
    }
}
