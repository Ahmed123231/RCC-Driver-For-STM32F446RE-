#ifndef RCC_INTERFACE_H
#define RCC_INTERFACE_H

#include "RCC_private.h"

/**
 * @brief Sets the status of the specified clock.
 * 
 * This function enables or disables a specific clock based on the provided status.
 *
 * @param Clk_Type The type of clock to set (HSI, HSE, PLL).
 * @param Status The status to set for the clock (ON, OFF).
 */
uint8_t RCC_SetClkStatus(CLK_t Clk_Type, STATUS_t Status);

/**
 * @brief Configures the system clock source.
 * 
 * This function selects the clock source for the system clock (HSI, HSE, or PLL).
 *
 * @param ClkType The type of clock source to use (HSI, HSE, PLL).
 */
uint8_t RCC_SetSysClk(CLK_t ClkType);

/**
 * @brief Configures the High-Speed External (HSE) mode.
 * 
 * This function sets the mode for the High-Speed External clock.
 *
 * @param HSE_MODE The mode to set for the HSE (BYPASSED, NOT_BYPASSED).
 */
uint8_t RCC_HSE_Mode(HSE_t HSE_MODE);

/**
 * @brief Configures the Phase-Locked Loop (PLL).
 * 
 * This function sets the PLL configuration parameters including multipliers and dividers.
 *
 * @param PLL_Multiplexer The PLL multiplier/divider value.
 * @param Src The clock source type for PLL (HSI, HSE).
 */
uint8_t RCC_PLL_Config(uint32_t PLL_Multiplexer,uint8_t PLL_Division ,CLK_t Src);

/**
 * @brief Enables the clock for a specific AHB1 peripheral.
 * 
 * This function enables the clock for an AHB1 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to enable (e.g., GPIOAEN, GPIOBEN).
 */
uint8_t RCC_AHB1_EnableClk(RCC_AHB1_PERIPHERAL_t PeripheralName);

/**
 * @brief Disables the clock for a specific AHB1 peripheral.
 * 
 * This function disables the clock for an AHB1 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to disable (e.g., GPIOAEN, GPIOBEN).
 */
uint8_t RCC_AHB1_DisableClk(RCC_AHB1_PERIPHERAL_t PeripheralName);

/**
 * @brief Enables the clock for a specific AHB2 peripheral.
 * 
 * This function enables the clock for an AHB2 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to enable (e.g., DCMIEN, OTGFSEN).
 */
uint8_t RCC_AHB2_EnableClk(RCC_AHB2_PERIPHERAL_t PeripheralName);

/**
 * @brief Disables the clock for a specific AHB2 peripheral.
 * 
 * This function disables the clock for an AHB2 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to disable (e.g., DCMIEN, OTGFSEN).
 */
uint8_t RCC_AHB2_DisableClk(RCC_AHB2_PERIPHERAL_t PeripheralName);

/**
 * @brief Enables the clock for a specific AHB3 peripheral.
 * 
 * This function enables the clock for an AHB3 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to enable (e.g., FMCEN, QSPIEN).
 */
uint8_t RCC_AHB3_EnableClk(RCC_AHB3_PERIPHERAL_t PeripheralName);

/**
 * @brief Disables the clock for a specific AHB3 peripheral.
 * 
 * This function disables the clock for an AHB3 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to disable (e.g., FMCEN, QSPIEN).
 */
uint8_t RCC_AHB3_DisableClk(RCC_AHB3_PERIPHERAL_t PeripheralName);

/**
 * @brief Enables the clock for a specific APB1 peripheral.
 * 
 * This function enables the clock for an APB1 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to enable (e.g., TIM2EN, USART2EN).
 */
uint8_t RCC_APB1_EnableClk(RCC_APB1_PERIPHERAL_t PeripheralName);

/**
 * @brief Disables the clock for a specific APB1 peripheral.
 * 
 * This function disables the clock for an APB1 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to disable (e.g., TIM2EN, USART2EN).
 */
uint8_t RCC_APB1_DisableClk(RCC_APB1_PERIPHERAL_t PeripheralName);

/**
 * @brief Enables the clock for a specific APB2 peripheral.
 * 
 * This function enables the clock for an APB2 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to enable (e.g., TIM1EN, ADC1EN).
 */
uint8_t RCC_APB2_EnableClk(RCC_APB2_PERIPHERAL_t PeripheralName);

/**
 * @brief Disables the clock for a specific APB2 peripheral.
 * 
 * This function disables the clock for an APB2 peripheral identified by PeripheralName.
 *
 * @param PeripheralName The peripheral to disable (e.g., TIM1EN, ADC1EN).
 */
uint8_t RCC_APB2_DisableClk(RCC_APB2_PERIPHERAL_t PeripheralName);

#endif // RCC_INTERFACE_H
