/*
 * hard_config.c
 *
 *  Created on: Feb 22, 2025
 *      Author: Usuario
 */


#include "hard_config.h"


ESCparams current_esc_params;
ConfigStatus current_config_status = CONFIG_OK;
volatile uint8_t esc_config_done = 0;

void set_default_esc_params(){
	current_esc_params.signature = 0x50415055;
	current_esc_params.pwm_freq_hz = 18000;
	current_esc_params.current_limit = 10;
	current_esc_params.temp_limit = 70;
	current_esc_params.deadtime_ns = 100;
	current_esc_params.speed_kp = 1.40f;
	current_esc_params.speed_ki = 0.4f;
	current_esc_params.speed_kd = 0.0f;
	current_esc_params.speed_max_rpm = 5400;

	current_esc_params.crc32 = compute_crc32(&current_esc_params);

}

void update_all_esc(){
	uint32_t tim_arr = CPU_clock / (2 * current_esc_params.pwm_freq_hz);
	TIM1 -> PSC = 0;
	TIM1->ARR = tim_arr;
	if(flash_config_has_pending_changes()){
		FlashResultCode result = flash_config_save();
		if(result == FLASH_RESULT_OK){
			esc_config_done = 1;

		} else{
			esc_config_done = 0;
		}
	}
}

uint32_t compute_crc32(ESCparams *params) {
    // Verificar alineación
    if ((uint32_t)params % 4 != 0) {
        // La estructura no está alineada a 4 bytes, usar buffer temporal
        uint32_t size_in_words = sizeof(ESCparams) / 4;
        uint32_t temp_buffer[size_in_words];

        // Guardar el valor actual del CRC
        uint32_t stored_crc = params->crc32;
        params->crc32 = 0;

        // Copiar a buffer alineado
        memcpy(temp_buffer, params, sizeof(ESCparams));

        // Calcular CRC
        uint32_t crc = HAL_CRC_Calculate(&hcrc, temp_buffer, size_in_words);

        // Restaurar el CRC original
        params->crc32 = stored_crc;

        return crc;
    } else {
        // La estructura está alineada, calcular directamente
        uint32_t stored_crc = params->crc32;
        params->crc32 = 0;

        uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)params, sizeof(ESCparams)/4);

        params->crc32 = stored_crc;
        return crc;
    }
}

uint16_t get_pwm_freq(){
	return current_esc_params.pwm_freq_hz;
}

ConfigStatus set_pwm_freq(uint16_t new_freq){

	if(new_freq < ESC_min_pwm_freq) return CONFIG_ERROR_UNDERLIMIT;
	if(new_freq > ESC_max_pwm_freq) return CONFIG_ERROR_OVERLIMIT;
	current_esc_params.pwm_freq_hz = new_freq;
	flash_config_parameter_changed();
	return CONFIG_OK;
}
