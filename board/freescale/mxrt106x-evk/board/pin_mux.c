/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MIMXRT1052xxx5A
package_id: MIMXRT1052CVL5A
mcu_data: ksdk2_0
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: L14, peripheral: LPUART1, signal: RX, pin_signal: GPIO_AD_B0_13, slew_rate: Slow, software_input_on: Disable, open_drain: Disable, speed: MHZ_100, drive_strength: R0_6,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Down_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: K14, peripheral: LPUART1, signal: TX, pin_signal: GPIO_AD_B0_12, slew_rate: Slow, software_input_on: Disable, open_drain: Disable, speed: MHZ_100, drive_strength: R0_6,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Down_100K_Ohm, hysteresis_enable: Disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-A7[ca7] */
    CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */

    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 is configured as LPUART1_TX */
                     0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 is configured as LPUART1_RX */
                     0U);                                    /* Software Input On Field: Input Path is determined by functionality */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 PAD functional properties : */
                     0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 PAD functional properties : */
                     0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */

#if 1
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3, 0);
    /* card detect pin */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_USDHC1_CD_B, 0);
    /* SD0_VSELECT */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_14_USDHC1_VSELECT, 0);// IOMUXC_GPIO_B1_14_USDHC1_VSELECT, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_15_GPIO2_IO31, 0);// IOMUXC_GPIO_B1_14_USDHC1_VSELECT, 0);

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                                       IOMUXC_SW_PAD_CTL_PAD_DSE(4));
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                       IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                                       IOMUXC_SW_PAD_CTL_PAD_DSE(4));
    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(1) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(4));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(1) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(4));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(1) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(4));

    IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3, IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                           IOMUXC_SW_PAD_CTL_PAD_SPEED(1) |
                                                           IOMUXC_SW_PAD_CTL_PAD_PUS(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(4));

    /* card detect pin */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_12_USDHC1_CD_B, IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                            IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);

    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_14_USDHC1_VSELECT, IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | 
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 

    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_15_GPIO2_IO31, IOMUXC_SW_PAD_CTL_PAD_PUS(3) | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | 
                                                            IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                                            IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
#endif
}

void BOARD_I2C_ConfigurePins(void)
{
    /*!< Function assigned for the core: Cortex-M7[ca7] */
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL, 1U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_ODE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA, 1U);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA, 
                        IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_DSE(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
                        IOMUXC_SW_PAD_CTL_PAD_ODE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PUS(2U));
}

void BOARD_Ethernet_InitPins(void)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */
    /* Initialize UART1 pins below */

    /* Initialize the ENET1 pins below. */
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_41_ENET_MDIO, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_40_ENET_MDC, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_ENET_RX_DATA00, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_ENET_RX_DATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_ENET_RX_EN, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_ENET_TX_DATA00, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_ENET_TX_DATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_ENET_TX_EN, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_ENET_REF_CLK, 1);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_ENET_RX_ER, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_10_GPIO1_IO10, 0);

    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_40_ENET_MDC, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | 
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_41_ENET_MDIO, IOMUXC_SW_PAD_CTL_PAD_SPEED(0) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | 
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_ODE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_04_ENET_RX_DATA00, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_05_ENET_RX_DATA01, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_06_ENET_RX_EN, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_11_ENET_RX_ER, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_07_ENET_TX_DATA00, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_08_ENET_TX_DATA01, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_09_ENET_TX_EN, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) |
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_10_ENET_REF_CLK, IOMUXC_SW_PAD_CTL_PAD_SPEED(3) | IOMUXC_SW_PAD_CTL_PAD_DSE(6) | 
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK); 

    // RESET pin
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | 
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 
    // INI PIN pull up
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_10_GPIO1_IO10, IOMUXC_SW_PAD_CTL_PAD_SPEED(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(5) | 
          IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
          IOMUXC_SW_PAD_CTL_PAD_PKE_MASK); 

    // set TX clk direction
    IOMUXC_GPR->GPR1 |= 0x20000;  
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
