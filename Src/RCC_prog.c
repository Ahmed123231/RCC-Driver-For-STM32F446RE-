#include <stdint.h>
#include "RCC_private.h"
#include "STM32F446xx.h"

#define RCC     ((RCC_RegDef_t*)RCC_BASE_ADDRESS)

/**
 * @brief Sets the status of the specified clock.
 *
 * This function enables or disables a specific clock based on the provided status.
 *
 * @param Clk_Type The type of clock to set (HSI, HSE, PLL, etc.).
 * @param Status The status to set for the clock (ON, OFF).
 * @return uint8_t Returns 0 on success, 1 if the clock type is invalid.
 */
uint8_t RCC_SetClkStatus(CLK_t Clk_Type, STATUS_t Status) {
    // Check if the clock type is within a valid range (HSI, HSE, PLL, etc.)



    // Set or clear the appropriate bit in the RCC->CR register based on Status
    if (Status == ON) {
        RCC->CR |= (1 << Clk_Type);  // Enable the specified clock
    } else {
        RCC->CR &= ~(1 << Clk_Type); // Disable the specified clock
    }

    // Wait for the clock to be ready (bit is set in RCC->CR register)
    while (((RCC->CR >> (Clk_Type + 1)) & 1) == 0);

    return 0;  // Success
}

/**
 * @brief Configures the system clock source.
 *
 * This function selects the clock source for the system clock (HSI, HSE, or PLL).
 *
 * @param SYSClkType The type of clock source to use (HSI, HSE, PLL).
 * @return uint8_t Returns 0 on success, 1 if the clock source is invalid.
 */
uint8_t RCC_SetSysClk(SYS_CLK_t SYSClkType) {
    // Check if the system clock source is valid
    if (SYSClkType > SYSPLLP) {
        return 1;  // Return error for invalid system clock type
    }

    // Clear the current system clock selection bits (SW[1:0] in CFGR)
    RCC->CFGR &= ~(0b11 << 0);

    // Set the new system clock source (SYSClkType shifted into position)
    RCC->CFGR |= (SYSClkType << 0);

    // Wait for the system clock to be switched and confirmed (SWS[1:0] bits)
    while (((RCC->CFGR >> 2) & 0b11) != SYSClkType);

    return 0;  // Success
}

/**
 * @brief Configures the HSE clock mode to either bypassed or not bypassed.
 *
 * This function sets the HSE clock mode by modifying the HSEBYP bit in the RCC_CR register.
 *
 * @param HSE_MODE HSE mode to be configured, defined by the HSE_t enum.
 * @return uint8_t Returns 0 if the mode is set successfully, 1 for an invalid input.
 */
uint8_t RCC_HSE_Mode(HSE_t HSE_MODE) {
    if (HSE_MODE == BYPASSED) {
        RCC->CR |= (1 << 18);  // Set HSEBYP bit to bypass HSE with external clock signal
        return 0;             // Success
    } else if (HSE_MODE == NOT_BYPASSED) {
        RCC->CR &= ~(1 << 18); // Clear HSEBYP bit to use the HSE oscillator directly
        return 0;             // Success
    } else {
        return 1;             // Invalid mode input
    }
}

/**
 * @brief Configures the Phase-Locked Loop (PLL).
 *
 * This function sets the PLL configuration parameters including multipliers and dividers.
 *
 * @param PLL_Multiplexer The PLL multiplier value.
 * @param Src The clock source type for PLL (HSI or HSE).
 * @return uint8_t Returns 0 on success, 1 if an error occurs.
 */
uint8_t RCC_PLL_Config(uint32_t PLL_Multiplexer,uint8_t PLL_Division ,CLK_t Src) {
	// Disable PLL by clearing the PLLON bit
	    RCC->CR &= ~(1 << 24);
	    while ((RCC->CR >> 25) & 1);  // Wait until PLLRDY bit is cleared

	    // Select the clock source for PLL
	    if (Src == HSI) {
	        RCC->PLLCFGR &= ~(1 << 22); // HSI clock selected as PLL source
	    } else if (Src == HSE) {
	        RCC->PLLCFGR |= (1 << 22);  // HSE oscillator clock selected as PLL source
	    } else {
	        return 1;  // Invalid clock source
	    }

	    // Validate and set the PLL multiplier (PLLN)
	    if (PLL_Multiplexer < 50 || PLL_Multiplexer > 432) {
	        return 1;  // Invalid PLL multiplier value
	    }

	    // Validate and set the PLL prescaler (PLLM)
	    if (PLL_Division < 2 || PLL_Division > 63) {
	        return 1;  // Invalid PLLM value
	    }

	    // Clear PLLM and PLLN bits
	    RCC->PLLCFGR &= ~(0x3F << 0);  // Clear PLLM bits (PLLM[5:0])
	    RCC->PLLCFGR &= ~(0x1FF << 6); // Clear PLLN bits (PLLN[8:0])

	    // Set PLLM and PLLN to desired values
	    RCC->PLLCFGR |= (PLL_Division << 0);    // Set PLLM to desired value
	    RCC->PLLCFGR |= (PLL_Multiplexer << 6); // Set PLLN to desired value

	    // Configure PLLP divider (PLLP[1:0]) for output clock
	    switch (PLL_Division) {
	        case 2:
	            RCC->PLLCFGR &= ~(0x3 << 16); // Clear PLLP and set it to 2
	            break;
	        case 4:
	            RCC->PLLCFGR = (RCC->PLLCFGR & ~(0x3 << 16)) | (1 << 16); // Set PLLP to 4
	            break;
	        case 6:
	            RCC->PLLCFGR = (RCC->PLLCFGR & ~(0x3 << 16)) | (2 << 16); // Set PLLP to 6
	            break;
	        case 8:
	            RCC->PLLCFGR = (RCC->PLLCFGR & ~(0x3 << 16)) | (3 << 16); // Set PLLP to 8
	            break;
	        default:
	            return 1;  // Invalid PLLP divider value
	    }

	    // Enable PLL
	    RCC->CR |= (1 << 24);
	    while (!((RCC->CR >> 25) & 1));  // Wait until PLLRDY bit is set

	    return 0;  // Success
}

/**
 * @brief Enables the clock for a specific AHB1 peripheral.
 *
 * @param PeripheralName The peripheral to enable (e.g., GPIOAEN, GPIOBEN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_AHB1_EnableClk(RCC_AHB1_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->AHB1ENR |= (1 << PeripheralName);  // Enable the peripheral clock
    return 0;  // Success
}

/**
 * @brief Disables the clock for a specific AHB1 peripheral.
 *
 * @param PeripheralName The peripheral to disable (e.g., GPIOAEN, GPIOBEN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_AHB1_DisableClk(RCC_AHB1_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->AHB1ENR &= ~(1 << PeripheralName);  // Disable the peripheral clock
    return 0;  // Success
}

/**
 * @brief Enables the clock for a specific AHB2 peripheral.
 *
 * @param PeripheralName The peripheral to enable (e.g., DCMIEN, OTGFSEN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_AHB2_EnableClk(RCC_AHB2_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->AHB2ENR |= (1 << PeripheralName);  // Enable the peripheral clock
    return 0;  // Success
}

/**
 * @brief Disables the clock for a specific AHB2 peripheral.
 *
 * @param PeripheralName The peripheral to disable (e.g., DCMIEN, OTGFSEN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_AHB2_DisableClk(RCC_AHB2_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->AHB2ENR &= ~(1 << PeripheralName);  // Disable the peripheral clock
    return 0;  // Success
}

/**
 * @brief Enables the clock for a specific AHB3 peripheral.
 *
 * @param PeripheralName The peripheral to enable (e.g., FMCEN, QSPIEN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_AHB3_EnableClk(RCC_AHB3_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->AHB3ENR |= (1 << PeripheralName);  // Enable the peripheral clock
    return 0;  // Success
}

/**
 * @brief Disables the clock for a specific AHB3 peripheral.
 *
 * @param PeripheralName The peripheral to disable (e.g., FMCEN, QSPIEN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_AHB3_DisableClk(RCC_AHB3_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->AHB3ENR &= ~(1 << PeripheralName);  // Disable the peripheral clock
    return 0;  // Success
}

/**
 * @brief Enables the clock for a specific APB1 peripheral.
 *
 * @param PeripheralName The peripheral to enable (e.g., TIM2EN, USART2EN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_APB1_EnableClk(RCC_APB1_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->APB1ENR |= (1 << PeripheralName);  // Enable the peripheral clock
    return 0;  // Success
}

/**
 * @brief Disables the clock for a specific APB1 peripheral.
 *
 * @param PeripheralName The peripheral to disable (e.g., TIM2EN, USART2EN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_APB1_DisableClk(RCC_APB1_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->APB1ENR &= ~(1 << PeripheralName);  // Disable the peripheral clock
    return 0;  // Success
}

/**
 * @brief Enables the clock for a specific APB2 peripheral.
 *
 * @param PeripheralName The peripheral to enable (e.g., TIM1EN, ADC1EN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_APB2_EnableClk(RCC_APB2_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->APB2ENR |= (1 << PeripheralName);  // Enable the peripheral clock
    return 0;  // Success
}

/**
 * @brief Disables the clock for a specific APB2 peripheral.
 *
 * @param PeripheralName The peripheral to disable (e.g., TIM1EN, ADC1EN).
 * @return uint8_t Returns 0 on success, 1 if the peripheral name is invalid.
 */
uint8_t RCC_APB2_DisableClk(RCC_APB2_PERIPHERAL_t PeripheralName) {
    if (PeripheralName > 31) {
        return 1;  // Return error if the peripheral name is out of range
    }

    RCC->APB2ENR &= ~(1 << PeripheralName);  // Disable the peripheral clock
    return 0;  // Success
}
