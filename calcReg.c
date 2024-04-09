#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define REF_FREQ 25

int channelSpacing = 100; // 100 kHz Channel Spacing

// 32 bit integer for all 6 registers
uint32_t REGISTER_0, REGISTER_1, REGISTER_2, REGISTER_3, REGISTER_4, REGISTER_5;

// Function to print uint32_t as binary string
void print_binary(uint32_t n) {
    // Start from the most significant bit
    for (int i = 31; i >= 0; i--) {
        // Check if the bit at position i is set or not
        uint32_t mask = 1 << i;
        printf("%d", (n & mask) ? 1 : 0);
    }
}


// Simple GCD Function taken from https://stackoverflow.com/questions/19738919/gcd-function-for-c
int gcd(int a, int b) {
    int remainder = a % b;

    if (remainder == 0) {
        return b;
    }

    return gcd(b, remainder);
}

// Calculate and build actual register content
void calculateRegister(int targetFreq) {

    // <--- Register 0 --->
    REGISTER_0 = 0;
    int R0_control_bits = 0;        // 3 bit control bits
    int R0_frac_value = 0;          // 12 bit fractional value
    int R0_integer_value = 0;       // 16 bit interger value
    int R0_reserved_bit = 0;        // 1 bit reserved value

    // <--- Register 1 --->
    REGISTER_1 = 0;
    int R1_control_bits = 1;        // 3 bit control bits
    int R1_mod_value = 0;           // 12 bit modulus value
    int R1_phase_value = 0;         // 12 bit phase value
    int R1_prescaler_value = 0;     // 1 bit prescaler value
    int R1_phase_adjust_value = 0;  // 1 bit phase adjust value
    int R1_reserved_bit = 0;        // 3 bit reserved value

    // <--- Register 2 --->
    REGISTER_2 = 0;
    int R2_control_bits = 2;        // 3 bit control bits
    int R2_counter_reset = 0;       // 1 bit counter reset
    int R2_cp_three_state = 0;      // 1 bit cp three state
    int R2_power_down = 0;          // 1 bit power down
    int R2_pd_polarity = 1;         // 1 bit pd polarity
    int R2_ldp = 0;                 // 1 bit ldp
    int R2_ldf = 1;                 // 1 bit ldf
    int R2_charge_pump_setting = 15; // 4 bit current charge pump setting (5 mA)
    int R2_double_buffer = 0;       // 1 bit double buffer
    int R2_R_counter = 1;           // 10 bit R Counter value
    int R2_R_DIV2 = 0;              // 1 bit RDIV2
    int R2_reference_doubler = 0;   // 1 bit reference doubler
    int R2_muxout = 0;              // 3 bit mux out setting
    int R2_low_noise_spur = 0;      // 2 bit low noise/low spur modes
    int R2_reserved_bit = 0;        // 1 bit reserved value

    // <--- Register 3 --->
    REGISTER_3 = 0;
    int R3_control_bits = 3;        // 3 bit control bits
    int R3_clock_divider = 150;       // 12 bit clock divider value
    int R3_clock_div_mode = 0;      // 2 bit clock div mode
    int R3_reserved_one_bit = 0;    // 1 bit reserved bit
    int R3_csr_value = 0;           // 1 bit CSR value
    int R3_reserved_two_bit = 0;    // 2 bit reserved bit
    int R3_charge_cancel = 0;       // 1 bit charge cancel
    int R3_abp_value = 0;           // 1 bit ABP value
    int R3_band_select_clock_mode = 0;  // 1 bit band select clock mode
    int R3_reserved_bit = 0;        // 8 bit reserved value

    // <--- Register 4 --->
    REGISTER_4 = 0;
    int R4_control_bits = 4;        // 3 bit control bits
    int R4_output_power = 3;        // 2 bit output power 
    int R4_rf_out_enable = 1;       // 1 bit RF out enable
    int R4_aux_output_power = 0;    // 2 bit auxilary output power
    int R4_aux_out_enable = 0;      // 1 bit aux out enable
    int R4_aux_out_select = 0;      // 1 bit aux out select
    int R4_MTLD_value = 0;          // 1 bit MTLD value
    int R4_VCO_power_down = 0;      // 1 bit VCO power down
    int R4_band_select_clock_divider_value = 200; // 8 bit band select clock divider value
    int R4_RF_divider_select = 0;   // 3 bit RF divider select
    int R4_feedback_select = 1;     // 1 bit feedback select
    int R4_reserved_bit = 0;        // 8 bit reserved value

    // <--- Register 5 --->
    REGISTER_5 = 0;
    int R5_control_bits = 5;        // 3 bit control bits
    int R5_low_reserved_bit = 0;    // 16 bit reserved bits
    int R5_mid_reserved_bit = 3;    // 2 bit mid reserved bits
    int R5_single_reserved = 0;     // 1 bit single reserved bit
    int R5_ld_pin_mode = 3;         // 2 bit ld pin mode value
    int R5_reserved_bit = 0;        // 8 bit reserved value

    // Calculate f_PDF according to the following formula
    // f_PFD = REF_IN * (1+REF_IN_DOUBLE_BIT) / (R_Counter * (1+RefDivByTwo))

    double fPFD = REF_FREQ * (1 + R2_reference_doubler) / (R2_R_counter * (1+R2_R_DIV2));
    
    // Set VCO Divider based on the output frequency

    int VCODivider = 1;

    if(targetFreq < 2200) { R4_RF_divider_select = 1; VCODivider = 2; }
    if(targetFreq < 1100) { R4_RF_divider_select = 2; VCODivider = 4; }
    if(targetFreq < 550) { R4_RF_divider_select = 3; VCODivider = 8; }
    if(targetFreq < 275) { R4_RF_divider_select = 4; VCODivider = 16; }
    if(targetFreq < 138) { R4_RF_divider_select = 5; VCODivider = 32; }
    if(targetFreq < 69) { R4_RF_divider_select = 6; VCODivider = 64; }

    // Now calculate the actual VCO frequency
    double VCO_Freq = targetFreq * VCODivider;

    // Now calculate the integer (N) value of the pll
    double N = VCO_Freq/fPFD;

    // Set the interger part to register 0
    R0_integer_value = N;


    // Calculate Mod value
    // Code taken from https://ez.analog.com/rf/f/q-a/75328/how-to-calculate-mod-and-frac-values-for-adf4351-manually
    // Source code for AD435x control software

    int modValue = round(1000 * fPFD/channelSpacing);

    // Calculate Frac value
    // Code taken from https://ez.analog.com/rf/f/q-a/75328/how-to-calculate-mod-and-frac-values-for-adf4351-manually

    int fracValue = round((N - R0_integer_value)*modValue);

    // Calculate GCD of Frac and Mod
    // Mentioned here: https://ez.analog.com/rf/f/q-a/75894/adf4351-worked-example
    int gcdFracMod = gcd(modValue, fracValue);

    // Update MOD and FRAC value
    fracValue = fracValue/gcdFracMod;
    modValue = modValue/gcdFracMod;

    // Special case if MOD = 1 ---> MOD = 2
    if(modValue == 1) { modValue = 2; }

    // Write FRAC and MOD to register
    R0_frac_value = fracValue;
    R1_mod_value = modValue;


    // Build 32 bit registers according to the datasheet (page 14)

    // Build register 0
    REGISTER_0 = R0_control_bits | R0_frac_value << 3 | R0_integer_value << 15 | R0_reserved_bit << 31;

    // Build register 1
    REGISTER_1 = R1_control_bits | R1_mod_value << 3 | R1_phase_value << 15 | R1_prescaler_value << 27 | R1_phase_adjust_value << 28 
                | R1_reserved_bit << 29;

    // Build register 2
    REGISTER_2 = R2_control_bits | R2_counter_reset << 3 | R2_cp_three_state << 4 | R2_power_down << 5 | R2_pd_polarity << 6 |
                 R2_ldp << 7 | R2_ldf << 8 | R2_charge_pump_setting << 9 | R2_double_buffer << 13 | R2_R_counter << 14 |
                 R2_R_DIV2 << 24 | R2_reference_doubler << 25 | R2_muxout << 26 | R2_low_noise_spur << 29 | R2_reserved_bit << 31;

    // Build register 3
    REGISTER_3 = R3_control_bits | R3_clock_divider << 3 | R3_clock_div_mode << 15 | R3_reserved_one_bit << 17 |
                 R3_csr_value << 18 | R3_reserved_two_bit << 19 | R3_charge_cancel << 21 | R3_abp_value << 22 |
                 R3_band_select_clock_mode << 23 | R3_reserved_bit << 24;

    // Build register 4
    REGISTER_4 = R4_control_bits | R4_output_power << 3 | R4_rf_out_enable << 5 | R4_aux_output_power << 6 |
                 R4_aux_out_enable << 8 | R4_aux_out_select << 9 | R4_MTLD_value << 10 | R4_VCO_power_down << 11 |
                 R4_band_select_clock_divider_value << 12 | R4_RF_divider_select << 20 | R4_feedback_select << 23 |
                 R4_reserved_bit << 24;

    // Build register 5
    REGISTER_5 = R5_control_bits | R5_low_reserved_bit << 3 | R5_mid_reserved_bit << 19 | R5_single_reserved << 21 |
                 R5_ld_pin_mode << 22 | R5_reserved_bit << 24;


    printf("\n");
    printf("INT: %d", R0_integer_value);
    printf("\nFRAC: %d", fracValue);
    printf("\nMOD: %d", modValue);
    printf("\n");

}


int main() {

    int inputFreq = 2000;

    printf("Please enter a target frequency in MHz: \n");
    scanf("%d", &inputFreq);

    calculateRegister(inputFreq);

    printf("\n");
    print_binary(REGISTER_0);
    printf("\n");
    print_binary(REGISTER_1);
    printf("\n");
    print_binary(REGISTER_2);
    printf("\n");
    print_binary(REGISTER_3);
    printf("\n");
    print_binary(REGISTER_4);
    printf("\n");
    print_binary(REGISTER_5);
    printf("\n");
    

    return 0;

}