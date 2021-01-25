// pwm.h
#include "typedefs.h"

#ifndef _PWM_h
#define _PWM_h


void pwmChanInit();

//17.1 define peripheral numbers
//typedef enum
//{
//	FrontMotors = 0x3FF5E000,
//	RearMotors = 0x3FF6C00
//} WhichUnitToPrescale;


//typedef struct {
//	WhichUnitToPrescale unit;
//	mcpwm_timer_t timer;
//	mcpwm_operator_t opOut;
//	mcpwm_io_signals_t signal;
//}PWM_Motor;

//typedef enum
//{
//	PWM_Chan0 = 0x01,
//	PWM_Chan1 = 0x02,
//	PWM_Chan2 = 0x04,
//	PWM_Chan3 = 0x08,
//	PWM_Chan4 = 0x10,
//	PWM_Chan5 = 0x20,
//	PWM_Chan6 = 0x40,
//	PWM_Chan7 = 0x80
//} PWMChan;

//#define DR_REG_GPIO_BASE                        0x3ff44000
//#define DR_REG_GPIO_SD_BASE                     0x3ff44f00
//
//#define GPIO_PIN0_REG          (DR_REG_GPIO_BASE + 0x0088)
///* GPIO_PIN0_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN0_INT_ENA  0x0000001F
//#define GPIO_PIN0_INT_ENA_M  ((GPIO_PIN0_INT_ENA_V)<<(GPIO_PIN0_INT_ENA_S))
//#define GPIO_PIN0_INT_ENA_V  0x1F
//#define GPIO_PIN0_INT_ENA_S  13
// /* GPIO_PIN0_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN0_CONFIG  0x00000003
//#define GPIO_PIN0_CONFIG_M  ((GPIO_PIN0_CONFIG_V)<<(GPIO_PIN0_CONFIG_S))
//#define GPIO_PIN0_CONFIG_V  0x3
//#define GPIO_PIN0_CONFIG_S  11
///* GPIO_PIN0_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN0_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN0_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN0_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN0_WAKEUP_ENABLE_S  10
///* GPIO_PIN0_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN0_INT_TYPE  0x00000007
//#define GPIO_PIN0_INT_TYPE_M  ((GPIO_PIN0_INT_TYPE_V)<<(GPIO_PIN0_INT_TYPE_S))
//#define GPIO_PIN0_INT_TYPE_V  0x7
//#define GPIO_PIN0_INT_TYPE_S  7
// /* GPIO_PIN0_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN0_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN0_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN0_PAD_DRIVER_V  0x1
//#define GPIO_PIN0_PAD_DRIVER_S  2
//
//#define GPIO_PIN1_REG          (DR_REG_GPIO_BASE + 0x008c)
///* GPIO_PIN1_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN1_INT_ENA  0x0000001F
//#define GPIO_PIN1_INT_ENA_M  ((GPIO_PIN1_INT_ENA_V)<<(GPIO_PIN1_INT_ENA_S))
//#define GPIO_PIN1_INT_ENA_V  0x1F
//#define GPIO_PIN1_INT_ENA_S  13
// /* GPIO_PIN1_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN1_CONFIG  0x00000003
//#define GPIO_PIN1_CONFIG_M  ((GPIO_PIN1_CONFIG_V)<<(GPIO_PIN1_CONFIG_S))
//#define GPIO_PIN1_CONFIG_V  0x3
//#define GPIO_PIN1_CONFIG_S  11
///* GPIO_PIN1_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN1_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN1_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN1_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN1_WAKEUP_ENABLE_S  10
///* GPIO_PIN1_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN1_INT_TYPE  0x00000007
//#define GPIO_PIN1_INT_TYPE_M  ((GPIO_PIN1_INT_TYPE_V)<<(GPIO_PIN1_INT_TYPE_S))
//#define GPIO_PIN1_INT_TYPE_V  0x7
//#define GPIO_PIN1_INT_TYPE_S  7
// /* GPIO_PIN1_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN1_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN1_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN1_PAD_DRIVER_V  0x1
//#define GPIO_PIN1_PAD_DRIVER_S  2
//
//#define GPIO_PIN2_REG          (DR_REG_GPIO_BASE + 0x0090)
///* GPIO_PIN2_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN2_INT_ENA  0x0000001F
//#define GPIO_PIN2_INT_ENA_M  ((GPIO_PIN2_INT_ENA_V)<<(GPIO_PIN2_INT_ENA_S))
//#define GPIO_PIN2_INT_ENA_V  0x1F
//#define GPIO_PIN2_INT_ENA_S  13
// /* GPIO_PIN2_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN2_CONFIG  0x00000003
//#define GPIO_PIN2_CONFIG_M  ((GPIO_PIN2_CONFIG_V)<<(GPIO_PIN2_CONFIG_S))
//#define GPIO_PIN2_CONFIG_V  0x3
//#define GPIO_PIN2_CONFIG_S  11
///* GPIO_PIN2_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN2_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN2_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN2_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN2_WAKEUP_ENABLE_S  10
///* GPIO_PIN2_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN2_INT_TYPE  0x00000007
//#define GPIO_PIN2_INT_TYPE_M  ((GPIO_PIN2_INT_TYPE_V)<<(GPIO_PIN2_INT_TYPE_S))
//#define GPIO_PIN2_INT_TYPE_V  0x7
//#define GPIO_PIN2_INT_TYPE_S  7
// /* GPIO_PIN2_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN2_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN2_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN2_PAD_DRIVER_V  0x1
//#define GPIO_PIN2_PAD_DRIVER_S  2
//
//#define GPIO_PIN3_REG          (DR_REG_GPIO_BASE + 0x0094)
///* GPIO_PIN3_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN3_INT_ENA  0x0000001F
//#define GPIO_PIN3_INT_ENA_M  ((GPIO_PIN3_INT_ENA_V)<<(GPIO_PIN3_INT_ENA_S))
//#define GPIO_PIN3_INT_ENA_V  0x1F
//#define GPIO_PIN3_INT_ENA_S  13
// /* GPIO_PIN3_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN3_CONFIG  0x00000003
//#define GPIO_PIN3_CONFIG_M  ((GPIO_PIN3_CONFIG_V)<<(GPIO_PIN3_CONFIG_S))
//#define GPIO_PIN3_CONFIG_V  0x3
//#define GPIO_PIN3_CONFIG_S  11
///* GPIO_PIN3_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN3_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN3_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN3_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN3_WAKEUP_ENABLE_S  10
///* GPIO_PIN3_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN3_INT_TYPE  0x00000007
//#define GPIO_PIN3_INT_TYPE_M  ((GPIO_PIN3_INT_TYPE_V)<<(GPIO_PIN3_INT_TYPE_S))
//#define GPIO_PIN3_INT_TYPE_V  0x7
//#define GPIO_PIN3_INT_TYPE_S  7
// /* GPIO_PIN3_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN3_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN3_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN3_PAD_DRIVER_V  0x1
//#define GPIO_PIN3_PAD_DRIVER_S  2
//
//#define GPIO_PIN4_REG          (DR_REG_GPIO_BASE + 0x0098)
///* GPIO_PIN4_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN4_INT_ENA  0x0000001F
//#define GPIO_PIN4_INT_ENA_M  ((GPIO_PIN4_INT_ENA_V)<<(GPIO_PIN4_INT_ENA_S))
//#define GPIO_PIN4_INT_ENA_V  0x1F
//#define GPIO_PIN4_INT_ENA_S  13
// /* GPIO_PIN4_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN4_CONFIG  0x00000003
//#define GPIO_PIN4_CONFIG_M  ((GPIO_PIN4_CONFIG_V)<<(GPIO_PIN4_CONFIG_S))
//#define GPIO_PIN4_CONFIG_V  0x3
//#define GPIO_PIN4_CONFIG_S  11
///* GPIO_PIN4_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN4_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN4_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN4_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN4_WAKEUP_ENABLE_S  10
///* GPIO_PIN4_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN4_INT_TYPE  0x00000007
//#define GPIO_PIN4_INT_TYPE_M  ((GPIO_PIN4_INT_TYPE_V)<<(GPIO_PIN4_INT_TYPE_S))
//#define GPIO_PIN4_INT_TYPE_V  0x7
//#define GPIO_PIN4_INT_TYPE_S  7
// /* GPIO_PIN4_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN4_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN4_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN4_PAD_DRIVER_V  0x1
//#define GPIO_PIN4_PAD_DRIVER_S  2
//
//#define GPIO_PIN5_REG          (DR_REG_GPIO_BASE + 0x009c)
///* GPIO_PIN5_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN5_INT_ENA  0x0000001F
//#define GPIO_PIN5_INT_ENA_M  ((GPIO_PIN5_INT_ENA_V)<<(GPIO_PIN5_INT_ENA_S))
//#define GPIO_PIN5_INT_ENA_V  0x1F
//#define GPIO_PIN5_INT_ENA_S  13
// /* GPIO_PIN5_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN5_CONFIG  0x00000003
//#define GPIO_PIN5_CONFIG_M  ((GPIO_PIN5_CONFIG_V)<<(GPIO_PIN5_CONFIG_S))
//#define GPIO_PIN5_CONFIG_V  0x3
//#define GPIO_PIN5_CONFIG_S  11
///* GPIO_PIN5_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN5_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN5_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN5_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN5_WAKEUP_ENABLE_S  10
///* GPIO_PIN5_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN5_INT_TYPE  0x00000007
//#define GPIO_PIN5_INT_TYPE_M  ((GPIO_PIN5_INT_TYPE_V)<<(GPIO_PIN5_INT_TYPE_S))
//#define GPIO_PIN5_INT_TYPE_V  0x7
//#define GPIO_PIN5_INT_TYPE_S  7
// /* GPIO_PIN5_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN5_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN5_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN5_PAD_DRIVER_V  0x1
//#define GPIO_PIN5_PAD_DRIVER_S  2
//
//#define GPIO_PIN6_REG          (DR_REG_GPIO_BASE + 0x00a0)
///* GPIO_PIN6_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN6_INT_ENA  0x0000001F
//#define GPIO_PIN6_INT_ENA_M  ((GPIO_PIN6_INT_ENA_V)<<(GPIO_PIN6_INT_ENA_S))
//#define GPIO_PIN6_INT_ENA_V  0x1F
//#define GPIO_PIN6_INT_ENA_S  13
// /* GPIO_PIN6_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN6_CONFIG  0x00000003
//#define GPIO_PIN6_CONFIG_M  ((GPIO_PIN6_CONFIG_V)<<(GPIO_PIN6_CONFIG_S))
//#define GPIO_PIN6_CONFIG_V  0x3
//#define GPIO_PIN6_CONFIG_S  11
///* GPIO_PIN6_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN6_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN6_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN6_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN6_WAKEUP_ENABLE_S  10
///* GPIO_PIN6_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN6_INT_TYPE  0x00000007
//#define GPIO_PIN6_INT_TYPE_M  ((GPIO_PIN6_INT_TYPE_V)<<(GPIO_PIN6_INT_TYPE_S))
//#define GPIO_PIN6_INT_TYPE_V  0x7
//#define GPIO_PIN6_INT_TYPE_S  7
// /* GPIO_PIN6_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN6_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN6_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN6_PAD_DRIVER_V  0x1
//#define GPIO_PIN6_PAD_DRIVER_S  2
//
//#define GPIO_PIN7_REG          (DR_REG_GPIO_BASE + 0x00a4)
///* GPIO_PIN7_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN7_INT_ENA  0x0000001F
//#define GPIO_PIN7_INT_ENA_M  ((GPIO_PIN7_INT_ENA_V)<<(GPIO_PIN7_INT_ENA_S))
//#define GPIO_PIN7_INT_ENA_V  0x1F
//#define GPIO_PIN7_INT_ENA_S  13
// /* GPIO_PIN7_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN7_CONFIG  0x00000003
//#define GPIO_PIN7_CONFIG_M  ((GPIO_PIN7_CONFIG_V)<<(GPIO_PIN7_CONFIG_S))
//#define GPIO_PIN7_CONFIG_V  0x3
//#define GPIO_PIN7_CONFIG_S  11
///* GPIO_PIN7_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN7_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN7_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN7_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN7_WAKEUP_ENABLE_S  10
///* GPIO_PIN7_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN7_INT_TYPE  0x00000007
//#define GPIO_PIN7_INT_TYPE_M  ((GPIO_PIN7_INT_TYPE_V)<<(GPIO_PIN7_INT_TYPE_S))
//#define GPIO_PIN7_INT_TYPE_V  0x7
//#define GPIO_PIN7_INT_TYPE_S  7
// /* GPIO_PIN7_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN7_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN7_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN7_PAD_DRIVER_V  0x1
//#define GPIO_PIN7_PAD_DRIVER_S  2
//
//#define GPIO_PIN8_REG          (DR_REG_GPIO_BASE + 0x00a8)
///* GPIO_PIN8_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN8_INT_ENA  0x0000001F
//#define GPIO_PIN8_INT_ENA_M  ((GPIO_PIN8_INT_ENA_V)<<(GPIO_PIN8_INT_ENA_S))
//#define GPIO_PIN8_INT_ENA_V  0x1F
//#define GPIO_PIN8_INT_ENA_S  13
// /* GPIO_PIN8_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN8_CONFIG  0x00000003
//#define GPIO_PIN8_CONFIG_M  ((GPIO_PIN8_CONFIG_V)<<(GPIO_PIN8_CONFIG_S))
//#define GPIO_PIN8_CONFIG_V  0x3
//#define GPIO_PIN8_CONFIG_S  11
///* GPIO_PIN8_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN8_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN8_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN8_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN8_WAKEUP_ENABLE_S  10
///* GPIO_PIN8_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN8_INT_TYPE  0x00000007
//#define GPIO_PIN8_INT_TYPE_M  ((GPIO_PIN8_INT_TYPE_V)<<(GPIO_PIN8_INT_TYPE_S))
//#define GPIO_PIN8_INT_TYPE_V  0x7
//#define GPIO_PIN8_INT_TYPE_S  7
// /* GPIO_PIN8_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN8_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN8_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN8_PAD_DRIVER_V  0x1
//#define GPIO_PIN8_PAD_DRIVER_S  2
//
//#define GPIO_PIN9_REG          (DR_REG_GPIO_BASE + 0x00ac)
///* GPIO_PIN9_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN9_INT_ENA  0x0000001F
//#define GPIO_PIN9_INT_ENA_M  ((GPIO_PIN9_INT_ENA_V)<<(GPIO_PIN9_INT_ENA_S))
//#define GPIO_PIN9_INT_ENA_V  0x1F
//#define GPIO_PIN9_INT_ENA_S  13
// /* GPIO_PIN9_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN9_CONFIG  0x00000003
//#define GPIO_PIN9_CONFIG_M  ((GPIO_PIN9_CONFIG_V)<<(GPIO_PIN9_CONFIG_S))
//#define GPIO_PIN9_CONFIG_V  0x3
//#define GPIO_PIN9_CONFIG_S  11
///* GPIO_PIN9_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN9_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN9_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN9_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN9_WAKEUP_ENABLE_S  10
///* GPIO_PIN9_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN9_INT_TYPE  0x00000007
//#define GPIO_PIN9_INT_TYPE_M  ((GPIO_PIN9_INT_TYPE_V)<<(GPIO_PIN9_INT_TYPE_S))
//#define GPIO_PIN9_INT_TYPE_V  0x7
//#define GPIO_PIN9_INT_TYPE_S  7
// /* GPIO_PIN9_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN9_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN9_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN9_PAD_DRIVER_V  0x1
//#define GPIO_PIN9_PAD_DRIVER_S  2
//
//#define GPIO_PIN10_REG          (DR_REG_GPIO_BASE + 0x00b0)
///* GPIO_PIN10_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN10_INT_ENA  0x0000001F
//#define GPIO_PIN10_INT_ENA_M  ((GPIO_PIN10_INT_ENA_V)<<(GPIO_PIN10_INT_ENA_S))
//#define GPIO_PIN10_INT_ENA_V  0x1F
//#define GPIO_PIN10_INT_ENA_S  13
// /* GPIO_PIN10_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN10_CONFIG  0x00000003
//#define GPIO_PIN10_CONFIG_M  ((GPIO_PIN10_CONFIG_V)<<(GPIO_PIN10_CONFIG_S))
//#define GPIO_PIN10_CONFIG_V  0x3
//#define GPIO_PIN10_CONFIG_S  11
///* GPIO_PIN10_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN10_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN10_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN10_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN10_WAKEUP_ENABLE_S  10
///* GPIO_PIN10_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN10_INT_TYPE  0x00000007
//#define GPIO_PIN10_INT_TYPE_M  ((GPIO_PIN10_INT_TYPE_V)<<(GPIO_PIN10_INT_TYPE_S))
//#define GPIO_PIN10_INT_TYPE_V  0x7
//#define GPIO_PIN10_INT_TYPE_S  7
// /* GPIO_PIN10_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN10_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN10_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN10_PAD_DRIVER_V  0x1
//#define GPIO_PIN10_PAD_DRIVER_S  2
//
//#define GPIO_PIN11_REG          (DR_REG_GPIO_BASE + 0x00b4)
///* GPIO_PIN11_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN11_INT_ENA  0x0000001F
//#define GPIO_PIN11_INT_ENA_M  ((GPIO_PIN11_INT_ENA_V)<<(GPIO_PIN11_INT_ENA_S))
//#define GPIO_PIN11_INT_ENA_V  0x1F
//#define GPIO_PIN11_INT_ENA_S  13
// /* GPIO_PIN11_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN11_CONFIG  0x00000003
//#define GPIO_PIN11_CONFIG_M  ((GPIO_PIN11_CONFIG_V)<<(GPIO_PIN11_CONFIG_S))
//#define GPIO_PIN11_CONFIG_V  0x3
//#define GPIO_PIN11_CONFIG_S  11
///* GPIO_PIN11_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN11_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN11_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN11_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN11_WAKEUP_ENABLE_S  10
///* GPIO_PIN11_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN11_INT_TYPE  0x00000007
//#define GPIO_PIN11_INT_TYPE_M  ((GPIO_PIN11_INT_TYPE_V)<<(GPIO_PIN11_INT_TYPE_S))
//#define GPIO_PIN11_INT_TYPE_V  0x7
//#define GPIO_PIN11_INT_TYPE_S  7
// /* GPIO_PIN11_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN11_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN11_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN11_PAD_DRIVER_V  0x1
//#define GPIO_PIN11_PAD_DRIVER_S  2
//
//#define GPIO_PIN12_REG          (DR_REG_GPIO_BASE + 0x00b8)
///* GPIO_PIN12_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN12_INT_ENA  0x0000001F
//#define GPIO_PIN12_INT_ENA_M  ((GPIO_PIN12_INT_ENA_V)<<(GPIO_PIN12_INT_ENA_S))
//#define GPIO_PIN12_INT_ENA_V  0x1F
//#define GPIO_PIN12_INT_ENA_S  13
// /* GPIO_PIN12_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN12_CONFIG  0x00000003
//#define GPIO_PIN12_CONFIG_M  ((GPIO_PIN12_CONFIG_V)<<(GPIO_PIN12_CONFIG_S))
//#define GPIO_PIN12_CONFIG_V  0x3
//#define GPIO_PIN12_CONFIG_S  11
///* GPIO_PIN12_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN12_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN12_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN12_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN12_WAKEUP_ENABLE_S  10
///* GPIO_PIN12_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN12_INT_TYPE  0x00000007
//#define GPIO_PIN12_INT_TYPE_M  ((GPIO_PIN12_INT_TYPE_V)<<(GPIO_PIN12_INT_TYPE_S))
//#define GPIO_PIN12_INT_TYPE_V  0x7
//#define GPIO_PIN12_INT_TYPE_S  7
// /* GPIO_PIN12_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN12_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN12_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN12_PAD_DRIVER_V  0x1
//#define GPIO_PIN12_PAD_DRIVER_S  2
//
//#define GPIO_PIN13_REG          (DR_REG_GPIO_BASE + 0x00bc)
///* GPIO_PIN13_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN13_INT_ENA  0x0000001F
//#define GPIO_PIN13_INT_ENA_M  ((GPIO_PIN13_INT_ENA_V)<<(GPIO_PIN13_INT_ENA_S))
//#define GPIO_PIN13_INT_ENA_V  0x1F
//#define GPIO_PIN13_INT_ENA_S  13
// /* GPIO_PIN13_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN13_CONFIG  0x00000003
//#define GPIO_PIN13_CONFIG_M  ((GPIO_PIN13_CONFIG_V)<<(GPIO_PIN13_CONFIG_S))
//#define GPIO_PIN13_CONFIG_V  0x3
//#define GPIO_PIN13_CONFIG_S  11
///* GPIO_PIN13_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN13_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN13_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN13_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN13_WAKEUP_ENABLE_S  10
///* GPIO_PIN13_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN13_INT_TYPE  0x00000007
//#define GPIO_PIN13_INT_TYPE_M  ((GPIO_PIN13_INT_TYPE_V)<<(GPIO_PIN13_INT_TYPE_S))
//#define GPIO_PIN13_INT_TYPE_V  0x7
//#define GPIO_PIN13_INT_TYPE_S  7
// /* GPIO_PIN13_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN13_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN13_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN13_PAD_DRIVER_V  0x1
//#define GPIO_PIN13_PAD_DRIVER_S  2
//
//#define GPIO_PIN14_REG          (DR_REG_GPIO_BASE + 0x00c0)
///* GPIO_PIN14_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN14_INT_ENA  0x0000001F
//#define GPIO_PIN14_INT_ENA_M  ((GPIO_PIN14_INT_ENA_V)<<(GPIO_PIN14_INT_ENA_S))
//#define GPIO_PIN14_INT_ENA_V  0x1F
//#define GPIO_PIN14_INT_ENA_S  13
// /* GPIO_PIN14_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN14_CONFIG  0x00000003
//#define GPIO_PIN14_CONFIG_M  ((GPIO_PIN14_CONFIG_V)<<(GPIO_PIN14_CONFIG_S))
//#define GPIO_PIN14_CONFIG_V  0x3
//#define GPIO_PIN14_CONFIG_S  11
///* GPIO_PIN14_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN14_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN14_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN14_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN14_WAKEUP_ENABLE_S  10
///* GPIO_PIN14_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN14_INT_TYPE  0x00000007
//#define GPIO_PIN14_INT_TYPE_M  ((GPIO_PIN14_INT_TYPE_V)<<(GPIO_PIN14_INT_TYPE_S))
//#define GPIO_PIN14_INT_TYPE_V  0x7
//#define GPIO_PIN14_INT_TYPE_S  7
// /* GPIO_PIN14_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN14_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN14_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN14_PAD_DRIVER_V  0x1
//#define GPIO_PIN14_PAD_DRIVER_S  2
//
//#define GPIO_PIN15_REG          (DR_REG_GPIO_BASE + 0x00c4)
///* GPIO_PIN15_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN15_INT_ENA  0x0000001F
//#define GPIO_PIN15_INT_ENA_M  ((GPIO_PIN15_INT_ENA_V)<<(GPIO_PIN15_INT_ENA_S))
//#define GPIO_PIN15_INT_ENA_V  0x1F
//#define GPIO_PIN15_INT_ENA_S  13
// /* GPIO_PIN15_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN15_CONFIG  0x00000003
//#define GPIO_PIN15_CONFIG_M  ((GPIO_PIN15_CONFIG_V)<<(GPIO_PIN15_CONFIG_S))
//#define GPIO_PIN15_CONFIG_V  0x3
//#define GPIO_PIN15_CONFIG_S  11
///* GPIO_PIN15_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN15_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN15_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN15_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN15_WAKEUP_ENABLE_S  10
///* GPIO_PIN15_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN15_INT_TYPE  0x00000007
//#define GPIO_PIN15_INT_TYPE_M  ((GPIO_PIN15_INT_TYPE_V)<<(GPIO_PIN15_INT_TYPE_S))
//#define GPIO_PIN15_INT_TYPE_V  0x7
//#define GPIO_PIN15_INT_TYPE_S  7
// /* GPIO_PIN15_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN15_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN15_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN15_PAD_DRIVER_V  0x1
//#define GPIO_PIN15_PAD_DRIVER_S  2
//
//#define GPIO_PIN16_REG          (DR_REG_GPIO_BASE + 0x00c8)
///* GPIO_PIN16_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN16_INT_ENA  0x0000001F
//#define GPIO_PIN16_INT_ENA_M  ((GPIO_PIN16_INT_ENA_V)<<(GPIO_PIN16_INT_ENA_S))
//#define GPIO_PIN16_INT_ENA_V  0x1F
//#define GPIO_PIN16_INT_ENA_S  13
// /* GPIO_PIN16_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN16_CONFIG  0x00000003
//#define GPIO_PIN16_CONFIG_M  ((GPIO_PIN16_CONFIG_V)<<(GPIO_PIN16_CONFIG_S))
//#define GPIO_PIN16_CONFIG_V  0x3
//#define GPIO_PIN16_CONFIG_S  11
///* GPIO_PIN16_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN16_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN16_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN16_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN16_WAKEUP_ENABLE_S  10
///* GPIO_PIN16_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN16_INT_TYPE  0x00000007
//#define GPIO_PIN16_INT_TYPE_M  ((GPIO_PIN16_INT_TYPE_V)<<(GPIO_PIN16_INT_TYPE_S))
//#define GPIO_PIN16_INT_TYPE_V  0x7
//#define GPIO_PIN16_INT_TYPE_S  7
// /* GPIO_PIN16_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN16_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN16_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN16_PAD_DRIVER_V  0x1
//#define GPIO_PIN16_PAD_DRIVER_S  2
//
//#define GPIO_PIN17_REG          (DR_REG_GPIO_BASE + 0x00cc)
///* GPIO_PIN17_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN17_INT_ENA  0x0000001F
//#define GPIO_PIN17_INT_ENA_M  ((GPIO_PIN17_INT_ENA_V)<<(GPIO_PIN17_INT_ENA_S))
//#define GPIO_PIN17_INT_ENA_V  0x1F
//#define GPIO_PIN17_INT_ENA_S  13
// /* GPIO_PIN17_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN17_CONFIG  0x00000003
//#define GPIO_PIN17_CONFIG_M  ((GPIO_PIN17_CONFIG_V)<<(GPIO_PIN17_CONFIG_S))
//#define GPIO_PIN17_CONFIG_V  0x3
//#define GPIO_PIN17_CONFIG_S  11
///* GPIO_PIN17_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN17_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN17_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN17_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN17_WAKEUP_ENABLE_S  10
///* GPIO_PIN17_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN17_INT_TYPE  0x00000007
//#define GPIO_PIN17_INT_TYPE_M  ((GPIO_PIN17_INT_TYPE_V)<<(GPIO_PIN17_INT_TYPE_S))
//#define GPIO_PIN17_INT_TYPE_V  0x7
//#define GPIO_PIN17_INT_TYPE_S  7
// /* GPIO_PIN17_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN17_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN17_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN17_PAD_DRIVER_V  0x1
//#define GPIO_PIN17_PAD_DRIVER_S  2
//
//#define GPIO_PIN18_REG          (DR_REG_GPIO_BASE + 0x00d0)
///* GPIO_PIN18_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN18_INT_ENA  0x0000001F
//#define GPIO_PIN18_INT_ENA_M  ((GPIO_PIN18_INT_ENA_V)<<(GPIO_PIN18_INT_ENA_S))
//#define GPIO_PIN18_INT_ENA_V  0x1F
//#define GPIO_PIN18_INT_ENA_S  13
// /* GPIO_PIN18_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN18_CONFIG  0x00000003
//#define GPIO_PIN18_CONFIG_M  ((GPIO_PIN18_CONFIG_V)<<(GPIO_PIN18_CONFIG_S))
//#define GPIO_PIN18_CONFIG_V  0x3
//#define GPIO_PIN18_CONFIG_S  11
///* GPIO_PIN18_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN18_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN18_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN18_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN18_WAKEUP_ENABLE_S  10
///* GPIO_PIN18_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN18_INT_TYPE  0x00000007
//#define GPIO_PIN18_INT_TYPE_M  ((GPIO_PIN18_INT_TYPE_V)<<(GPIO_PIN18_INT_TYPE_S))
//#define GPIO_PIN18_INT_TYPE_V  0x7
//#define GPIO_PIN18_INT_TYPE_S  7
// /* GPIO_PIN18_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN18_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN18_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN18_PAD_DRIVER_V  0x1
//#define GPIO_PIN18_PAD_DRIVER_S  2
//
//#define GPIO_PIN19_REG          (DR_REG_GPIO_BASE + 0x00d4)
///* GPIO_PIN19_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN19_INT_ENA  0x0000001F
//#define GPIO_PIN19_INT_ENA_M  ((GPIO_PIN19_INT_ENA_V)<<(GPIO_PIN19_INT_ENA_S))
//#define GPIO_PIN19_INT_ENA_V  0x1F
//#define GPIO_PIN19_INT_ENA_S  13
// /* GPIO_PIN19_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN19_CONFIG  0x00000003
//#define GPIO_PIN19_CONFIG_M  ((GPIO_PIN19_CONFIG_V)<<(GPIO_PIN19_CONFIG_S))
//#define GPIO_PIN19_CONFIG_V  0x3
//#define GPIO_PIN19_CONFIG_S  11
///* GPIO_PIN19_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN19_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN19_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN19_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN19_WAKEUP_ENABLE_S  10
///* GPIO_PIN19_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN19_INT_TYPE  0x00000007
//#define GPIO_PIN19_INT_TYPE_M  ((GPIO_PIN19_INT_TYPE_V)<<(GPIO_PIN19_INT_TYPE_S))
//#define GPIO_PIN19_INT_TYPE_V  0x7
//#define GPIO_PIN19_INT_TYPE_S  7
// /* GPIO_PIN19_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN19_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN19_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN19_PAD_DRIVER_V  0x1
//#define GPIO_PIN19_PAD_DRIVER_S  2
//
//#define GPIO_PIN20_REG          (DR_REG_GPIO_BASE + 0x00d8)
///* GPIO_PIN20_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-mask interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-mask interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN20_INT_ENA  0x0000001F
//#define GPIO_PIN20_INT_ENA_M  ((GPIO_PIN20_INT_ENA_V)<<(GPIO_PIN20_INT_ENA_S))
//#define GPIO_PIN20_INT_ENA_V  0x1F
//#define GPIO_PIN20_INT_ENA_S  13
// /* GPIO_PIN20_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN20_CONFIG  0x00000003
//#define GPIO_PIN20_CONFIG_M  ((GPIO_PIN20_CONFIG_V)<<(GPIO_PIN20_CONFIG_S))
//#define GPIO_PIN20_CONFIG_V  0x3
//#define GPIO_PIN20_CONFIG_S  11
///* GPIO_PIN20_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN20_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN20_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN20_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN20_WAKEUP_ENABLE_S  10
///* GPIO_PIN20_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN20_INT_TYPE  0x00000007
//#define GPIO_PIN20_INT_TYPE_M  ((GPIO_PIN20_INT_TYPE_V)<<(GPIO_PIN20_INT_TYPE_S))
//#define GPIO_PIN20_INT_TYPE_V  0x7
//#define GPIO_PIN20_INT_TYPE_S  7
// /* GPIO_PIN20_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN20_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN20_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN20_PAD_DRIVER_V  0x1
//#define GPIO_PIN20_PAD_DRIVER_S  2
//
//#define GPIO_PIN21_REG          (DR_REG_GPIO_BASE + 0x00dc)
///* GPIO_PIN21_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN21_INT_ENA  0x0000001F
//#define GPIO_PIN21_INT_ENA_M  ((GPIO_PIN21_INT_ENA_V)<<(GPIO_PIN21_INT_ENA_S))
//#define GPIO_PIN21_INT_ENA_V  0x1F
//#define GPIO_PIN21_INT_ENA_S  13
// /* GPIO_PIN21_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN21_CONFIG  0x00000003
//#define GPIO_PIN21_CONFIG_M  ((GPIO_PIN21_CONFIG_V)<<(GPIO_PIN21_CONFIG_S))
//#define GPIO_PIN21_CONFIG_V  0x3
//#define GPIO_PIN21_CONFIG_S  11
///* GPIO_PIN21_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN21_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN21_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN21_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN21_WAKEUP_ENABLE_S  10
///* GPIO_PIN21_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN21_INT_TYPE  0x00000007
//#define GPIO_PIN21_INT_TYPE_M  ((GPIO_PIN21_INT_TYPE_V)<<(GPIO_PIN21_INT_TYPE_S))
//#define GPIO_PIN21_INT_TYPE_V  0x7
//#define GPIO_PIN21_INT_TYPE_S  7
// /* GPIO_PIN21_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN21_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN21_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN21_PAD_DRIVER_V  0x1
//#define GPIO_PIN21_PAD_DRIVER_S  2
//
//#define GPIO_PIN22_REG          (DR_REG_GPIO_BASE + 0x00e0)
///* GPIO_PIN22_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: */
//#define GPIO_PIN22_INT_ENA  0x0000001F
//#define GPIO_PIN22_INT_ENA_M  ((GPIO_PIN22_INT_ENA_V)<<(GPIO_PIN22_INT_ENA_S))
//#define GPIO_PIN22_INT_ENA_V  0x1F
//#define GPIO_PIN22_INT_ENA_S  13
///* GPIO_PIN22_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN22_CONFIG  0x00000003
//#define GPIO_PIN22_CONFIG_M  ((GPIO_PIN22_CONFIG_V)<<(GPIO_PIN22_CONFIG_S))
//#define GPIO_PIN22_CONFIG_V  0x3
//#define GPIO_PIN22_CONFIG_S  11
// /* GPIO_PIN22_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN22_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN22_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN22_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN22_WAKEUP_ENABLE_S  10
///* GPIO_PIN22_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN22_INT_TYPE  0x00000007
//#define GPIO_PIN22_INT_TYPE_M  ((GPIO_PIN22_INT_TYPE_V)<<(GPIO_PIN22_INT_TYPE_S))
//#define GPIO_PIN22_INT_TYPE_V  0x7
//#define GPIO_PIN22_INT_TYPE_S  7
///* GPIO_PIN22_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
///*description: */
//#define GPIO_PIN22_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN22_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN22_PAD_DRIVER_V  0x1
//#define GPIO_PIN22_PAD_DRIVER_S  2
//
//#define GPIO_PIN23_REG          (DR_REG_GPIO_BASE + 0x00e4)
///* GPIO_PIN23_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN23_INT_ENA  0x0000001F
//#define GPIO_PIN23_INT_ENA_M  ((GPIO_PIN23_INT_ENA_V)<<(GPIO_PIN23_INT_ENA_S))
//#define GPIO_PIN23_INT_ENA_V  0x1F
//#define GPIO_PIN23_INT_ENA_S  13
// /* GPIO_PIN23_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN23_CONFIG  0x00000003
//#define GPIO_PIN23_CONFIG_M  ((GPIO_PIN23_CONFIG_V)<<(GPIO_PIN23_CONFIG_S))
//#define GPIO_PIN23_CONFIG_V  0x3
//#define GPIO_PIN23_CONFIG_S  11
///* GPIO_PIN23_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN23_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN23_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN23_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN23_WAKEUP_ENABLE_S  10
///* GPIO_PIN23_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN23_INT_TYPE  0x00000007
//#define GPIO_PIN23_INT_TYPE_M  ((GPIO_PIN23_INT_TYPE_V)<<(GPIO_PIN23_INT_TYPE_S))
//#define GPIO_PIN23_INT_TYPE_V  0x7
//#define GPIO_PIN23_INT_TYPE_S  7
// /* GPIO_PIN23_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN23_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN23_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN23_PAD_DRIVER_V  0x1
//#define GPIO_PIN23_PAD_DRIVER_S  2
//
//#define GPIO_PIN24_REG          (DR_REG_GPIO_BASE + 0x00e8)
///* GPIO_PIN24_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN24_INT_ENA  0x0000001F
//#define GPIO_PIN24_INT_ENA_M  ((GPIO_PIN24_INT_ENA_V)<<(GPIO_PIN24_INT_ENA_S))
//#define GPIO_PIN24_INT_ENA_V  0x1F
//#define GPIO_PIN24_INT_ENA_S  13
// /* GPIO_PIN24_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN24_CONFIG  0x00000003
//#define GPIO_PIN24_CONFIG_M  ((GPIO_PIN24_CONFIG_V)<<(GPIO_PIN24_CONFIG_S))
//#define GPIO_PIN24_CONFIG_V  0x3
//#define GPIO_PIN24_CONFIG_S  11
///* GPIO_PIN24_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN24_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN24_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN24_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN24_WAKEUP_ENABLE_S  10
///* GPIO_PIN24_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN24_INT_TYPE  0x00000007
//#define GPIO_PIN24_INT_TYPE_M  ((GPIO_PIN24_INT_TYPE_V)<<(GPIO_PIN24_INT_TYPE_S))
//#define GPIO_PIN24_INT_TYPE_V  0x7
//#define GPIO_PIN24_INT_TYPE_S  7
// /* GPIO_PIN24_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN24_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN24_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN24_PAD_DRIVER_V  0x1
//#define GPIO_PIN24_PAD_DRIVER_S  2
//
//#define GPIO_PIN25_REG          (DR_REG_GPIO_BASE + 0x00ec)
///* GPIO_PIN25_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN25_INT_ENA  0x0000001F
//#define GPIO_PIN25_INT_ENA_M  ((GPIO_PIN25_INT_ENA_V)<<(GPIO_PIN25_INT_ENA_S))
//#define GPIO_PIN25_INT_ENA_V  0x1F
//#define GPIO_PIN25_INT_ENA_S  13
// /* GPIO_PIN25_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN25_CONFIG  0x00000003
//#define GPIO_PIN25_CONFIG_M  ((GPIO_PIN25_CONFIG_V)<<(GPIO_PIN25_CONFIG_S))
//#define GPIO_PIN25_CONFIG_V  0x3
//#define GPIO_PIN25_CONFIG_S  11
///* GPIO_PIN25_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN25_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN25_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN25_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN25_WAKEUP_ENABLE_S  10
///* GPIO_PIN25_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN25_INT_TYPE  0x00000007
//#define GPIO_PIN25_INT_TYPE_M  ((GPIO_PIN25_INT_TYPE_V)<<(GPIO_PIN25_INT_TYPE_S))
//#define GPIO_PIN25_INT_TYPE_V  0x7
//#define GPIO_PIN25_INT_TYPE_S  7
// /* GPIO_PIN25_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN25_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN25_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN25_PAD_DRIVER_V  0x1
//#define GPIO_PIN25_PAD_DRIVER_S  2
//
//#define GPIO_PIN26_REG          (DR_REG_GPIO_BASE + 0x00f0)
///* GPIO_PIN26_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN26_INT_ENA  0x0000001F
//#define GPIO_PIN26_INT_ENA_M  ((GPIO_PIN26_INT_ENA_V)<<(GPIO_PIN26_INT_ENA_S))
//#define GPIO_PIN26_INT_ENA_V  0x1F
//#define GPIO_PIN26_INT_ENA_S  13
// /* GPIO_PIN26_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN26_CONFIG  0x00000003
//#define GPIO_PIN26_CONFIG_M  ((GPIO_PIN26_CONFIG_V)<<(GPIO_PIN26_CONFIG_S))
//#define GPIO_PIN26_CONFIG_V  0x3
//#define GPIO_PIN26_CONFIG_S  11
///* GPIO_PIN26_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN26_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN26_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN26_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN26_WAKEUP_ENABLE_S  10
///* GPIO_PIN26_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN26_INT_TYPE  0x00000007
//#define GPIO_PIN26_INT_TYPE_M  ((GPIO_PIN26_INT_TYPE_V)<<(GPIO_PIN26_INT_TYPE_S))
//#define GPIO_PIN26_INT_TYPE_V  0x7
//#define GPIO_PIN26_INT_TYPE_S  7
// /* GPIO_PIN26_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN26_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN26_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN26_PAD_DRIVER_V  0x1
//#define GPIO_PIN26_PAD_DRIVER_S  2
//
//#define GPIO_PIN27_REG          (DR_REG_GPIO_BASE + 0x00f4)
///* GPIO_PIN27_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN27_INT_ENA  0x0000001F
//#define GPIO_PIN27_INT_ENA_M  ((GPIO_PIN27_INT_ENA_V)<<(GPIO_PIN27_INT_ENA_S))
//#define GPIO_PIN27_INT_ENA_V  0x1F
//#define GPIO_PIN27_INT_ENA_S  13
// /* GPIO_PIN27_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN27_CONFIG  0x00000003
//#define GPIO_PIN27_CONFIG_M  ((GPIO_PIN27_CONFIG_V)<<(GPIO_PIN27_CONFIG_S))
//#define GPIO_PIN27_CONFIG_V  0x3
//#define GPIO_PIN27_CONFIG_S  11
///* GPIO_PIN27_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN27_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN27_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN27_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN27_WAKEUP_ENABLE_S  10
///* GPIO_PIN27_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN27_INT_TYPE  0x00000007
//#define GPIO_PIN27_INT_TYPE_M  ((GPIO_PIN27_INT_TYPE_V)<<(GPIO_PIN27_INT_TYPE_S))
//#define GPIO_PIN27_INT_TYPE_V  0x7
//#define GPIO_PIN27_INT_TYPE_S  7
// /* GPIO_PIN27_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN27_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN27_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN27_PAD_DRIVER_V  0x1
//#define GPIO_PIN27_PAD_DRIVER_S  2
//
//#define GPIO_PIN28_REG          (DR_REG_GPIO_BASE + 0x00f8)
///* GPIO_PIN28_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN28_INT_ENA  0x0000001F
//#define GPIO_PIN28_INT_ENA_M  ((GPIO_PIN28_INT_ENA_V)<<(GPIO_PIN28_INT_ENA_S))
//#define GPIO_PIN28_INT_ENA_V  0x1F
//#define GPIO_PIN28_INT_ENA_S  13
// /* GPIO_PIN28_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN28_CONFIG  0x00000003
//#define GPIO_PIN28_CONFIG_M  ((GPIO_PIN28_CONFIG_V)<<(GPIO_PIN28_CONFIG_S))
//#define GPIO_PIN28_CONFIG_V  0x3
//#define GPIO_PIN28_CONFIG_S  11
///* GPIO_PIN28_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN28_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN28_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN28_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN28_WAKEUP_ENABLE_S  10
///* GPIO_PIN28_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN28_INT_TYPE  0x00000007
//#define GPIO_PIN28_INT_TYPE_M  ((GPIO_PIN28_INT_TYPE_V)<<(GPIO_PIN28_INT_TYPE_S))
//#define GPIO_PIN28_INT_TYPE_V  0x7
//#define GPIO_PIN28_INT_TYPE_S  7
// /* GPIO_PIN28_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN28_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN28_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN28_PAD_DRIVER_V  0x1
//#define GPIO_PIN28_PAD_DRIVER_S  2
//
//#define GPIO_PIN29_REG          (DR_REG_GPIO_BASE + 0x00fc)
///* GPIO_PIN29_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN29_INT_ENA  0x0000001F
//#define GPIO_PIN29_INT_ENA_M  ((GPIO_PIN29_INT_ENA_V)<<(GPIO_PIN29_INT_ENA_S))
//#define GPIO_PIN29_INT_ENA_V  0x1F
//#define GPIO_PIN29_INT_ENA_S  13
// /* GPIO_PIN29_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN29_CONFIG  0x00000003
//#define GPIO_PIN29_CONFIG_M  ((GPIO_PIN29_CONFIG_V)<<(GPIO_PIN29_CONFIG_S))
//#define GPIO_PIN29_CONFIG_V  0x3
//#define GPIO_PIN29_CONFIG_S  11
///* GPIO_PIN29_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN29_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN29_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN29_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN29_WAKEUP_ENABLE_S  10
///* GPIO_PIN29_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN29_INT_TYPE  0x00000007
//#define GPIO_PIN29_INT_TYPE_M  ((GPIO_PIN29_INT_TYPE_V)<<(GPIO_PIN29_INT_TYPE_S))
//#define GPIO_PIN29_INT_TYPE_V  0x7
//#define GPIO_PIN29_INT_TYPE_S  7
// /* GPIO_PIN29_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN29_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN29_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN29_PAD_DRIVER_V  0x1
//#define GPIO_PIN29_PAD_DRIVER_S  2
//
//#define GPIO_PIN30_REG          (DR_REG_GPIO_BASE + 0x0100)
///* GPIO_PIN30_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN30_INT_ENA  0x0000001F
//#define GPIO_PIN30_INT_ENA_M  ((GPIO_PIN30_INT_ENA_V)<<(GPIO_PIN30_INT_ENA_S))
//#define GPIO_PIN30_INT_ENA_V  0x1F
//#define GPIO_PIN30_INT_ENA_S  13
// /* GPIO_PIN30_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN30_CONFIG  0x00000003
//#define GPIO_PIN30_CONFIG_M  ((GPIO_PIN30_CONFIG_V)<<(GPIO_PIN30_CONFIG_S))
//#define GPIO_PIN30_CONFIG_V  0x3
//#define GPIO_PIN30_CONFIG_S  11
///* GPIO_PIN30_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN30_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN30_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN30_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN30_WAKEUP_ENABLE_S  10
///* GPIO_PIN30_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN30_INT_TYPE  0x00000007
//#define GPIO_PIN30_INT_TYPE_M  ((GPIO_PIN30_INT_TYPE_V)<<(GPIO_PIN30_INT_TYPE_S))
//#define GPIO_PIN30_INT_TYPE_V  0x7
//#define GPIO_PIN30_INT_TYPE_S  7
// /* GPIO_PIN30_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN30_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN30_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN30_PAD_DRIVER_V  0x1
//#define GPIO_PIN30_PAD_DRIVER_S  2
//
//#define GPIO_PIN31_REG          (DR_REG_GPIO_BASE + 0x0104)
///* GPIO_PIN31_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN31_INT_ENA  0x0000001F
//#define GPIO_PIN31_INT_ENA_M  ((GPIO_PIN31_INT_ENA_V)<<(GPIO_PIN31_INT_ENA_S))
//#define GPIO_PIN31_INT_ENA_V  0x1F
//#define GPIO_PIN31_INT_ENA_S  13
// /* GPIO_PIN31_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN31_CONFIG  0x00000003
//#define GPIO_PIN31_CONFIG_M  ((GPIO_PIN31_CONFIG_V)<<(GPIO_PIN31_CONFIG_S))
//#define GPIO_PIN31_CONFIG_V  0x3
//#define GPIO_PIN31_CONFIG_S  11
///* GPIO_PIN31_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN31_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN31_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN31_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN31_WAKEUP_ENABLE_S  10
///* GPIO_PIN31_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN31_INT_TYPE  0x00000007
//#define GPIO_PIN31_INT_TYPE_M  ((GPIO_PIN31_INT_TYPE_V)<<(GPIO_PIN31_INT_TYPE_S))
//#define GPIO_PIN31_INT_TYPE_V  0x7
//#define GPIO_PIN31_INT_TYPE_S  7
// /* GPIO_PIN31_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN31_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN31_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN31_PAD_DRIVER_V  0x1
//#define GPIO_PIN31_PAD_DRIVER_S  2
//
//#define GPIO_PIN32_REG          (DR_REG_GPIO_BASE + 0x0108)
///* GPIO_PIN32_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN32_INT_ENA  0x0000001F
//#define GPIO_PIN32_INT_ENA_M  ((GPIO_PIN32_INT_ENA_V)<<(GPIO_PIN32_INT_ENA_S))
//#define GPIO_PIN32_INT_ENA_V  0x1F
//#define GPIO_PIN32_INT_ENA_S  13
// /* GPIO_PIN32_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN32_CONFIG  0x00000003
//#define GPIO_PIN32_CONFIG_M  ((GPIO_PIN32_CONFIG_V)<<(GPIO_PIN32_CONFIG_S))
//#define GPIO_PIN32_CONFIG_V  0x3
//#define GPIO_PIN32_CONFIG_S  11
///* GPIO_PIN32_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN32_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN32_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN32_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN32_WAKEUP_ENABLE_S  10
///* GPIO_PIN32_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN32_INT_TYPE  0x00000007
//#define GPIO_PIN32_INT_TYPE_M  ((GPIO_PIN32_INT_TYPE_V)<<(GPIO_PIN32_INT_TYPE_S))
//#define GPIO_PIN32_INT_TYPE_V  0x7
//#define GPIO_PIN32_INT_TYPE_S  7
// /* GPIO_PIN32_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN32_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN32_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN32_PAD_DRIVER_V  0x1
//#define GPIO_PIN32_PAD_DRIVER_S  2
//
//#define GPIO_PIN33_REG          (DR_REG_GPIO_BASE + 0x010c)
///* GPIO_PIN33_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN33_INT_ENA  0x0000001F
//#define GPIO_PIN33_INT_ENA_M  ((GPIO_PIN33_INT_ENA_V)<<(GPIO_PIN33_INT_ENA_S))
//#define GPIO_PIN33_INT_ENA_V  0x1F
//#define GPIO_PIN33_INT_ENA_S  13
// /* GPIO_PIN33_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN33_CONFIG  0x00000003
//#define GPIO_PIN33_CONFIG_M  ((GPIO_PIN33_CONFIG_V)<<(GPIO_PIN33_CONFIG_S))
//#define GPIO_PIN33_CONFIG_V  0x3
//#define GPIO_PIN33_CONFIG_S  11
///* GPIO_PIN33_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN33_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN33_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN33_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN33_WAKEUP_ENABLE_S  10
///* GPIO_PIN33_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN33_INT_TYPE  0x00000007
//#define GPIO_PIN33_INT_TYPE_M  ((GPIO_PIN33_INT_TYPE_V)<<(GPIO_PIN33_INT_TYPE_S))
//#define GPIO_PIN33_INT_TYPE_V  0x7
//#define GPIO_PIN33_INT_TYPE_S  7
// /* GPIO_PIN33_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN33_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN33_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN33_PAD_DRIVER_V  0x1
//#define GPIO_PIN33_PAD_DRIVER_S  2
//
//#define GPIO_PIN34_REG          (DR_REG_GPIO_BASE + 0x0110)
///* GPIO_PIN34_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN34_INT_ENA  0x0000001F
//#define GPIO_PIN34_INT_ENA_M  ((GPIO_PIN34_INT_ENA_V)<<(GPIO_PIN34_INT_ENA_S))
//#define GPIO_PIN34_INT_ENA_V  0x1F
//#define GPIO_PIN34_INT_ENA_S  13
// /* GPIO_PIN34_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN34_CONFIG  0x00000003
//#define GPIO_PIN34_CONFIG_M  ((GPIO_PIN34_CONFIG_V)<<(GPIO_PIN34_CONFIG_S))
//#define GPIO_PIN34_CONFIG_V  0x3
//#define GPIO_PIN34_CONFIG_S  11
///* GPIO_PIN34_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN34_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN34_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN34_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN34_WAKEUP_ENABLE_S  10
///* GPIO_PIN34_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN34_INT_TYPE  0x00000007
//#define GPIO_PIN34_INT_TYPE_M  ((GPIO_PIN34_INT_TYPE_V)<<(GPIO_PIN34_INT_TYPE_S))
//#define GPIO_PIN34_INT_TYPE_V  0x7
//#define GPIO_PIN34_INT_TYPE_S  7
// /* GPIO_PIN34_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN34_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN34_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN34_PAD_DRIVER_V  0x1
//#define GPIO_PIN34_PAD_DRIVER_S  2
//
//#define GPIO_PIN35_REG          (DR_REG_GPIO_BASE + 0x0114)
///* GPIO_PIN35_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN35_INT_ENA  0x0000001F
//#define GPIO_PIN35_INT_ENA_M  ((GPIO_PIN35_INT_ENA_V)<<(GPIO_PIN35_INT_ENA_S))
//#define GPIO_PIN35_INT_ENA_V  0x1F
//#define GPIO_PIN35_INT_ENA_S  13
// /* GPIO_PIN35_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN35_CONFIG  0x00000003
//#define GPIO_PIN35_CONFIG_M  ((GPIO_PIN35_CONFIG_V)<<(GPIO_PIN35_CONFIG_S))
//#define GPIO_PIN35_CONFIG_V  0x3
//#define GPIO_PIN35_CONFIG_S  11
///* GPIO_PIN35_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN35_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN35_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN35_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN35_WAKEUP_ENABLE_S  10
///* GPIO_PIN35_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN35_INT_TYPE  0x00000007
//#define GPIO_PIN35_INT_TYPE_M  ((GPIO_PIN35_INT_TYPE_V)<<(GPIO_PIN35_INT_TYPE_S))
//#define GPIO_PIN35_INT_TYPE_V  0x7
//#define GPIO_PIN35_INT_TYPE_S  7
// /* GPIO_PIN35_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN35_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN35_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN35_PAD_DRIVER_V  0x1
//#define GPIO_PIN35_PAD_DRIVER_S  2
//
//#define GPIO_PIN36_REG          (DR_REG_GPIO_BASE + 0x0118)
///* GPIO_PIN36_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN36_INT_ENA  0x0000001F
//#define GPIO_PIN36_INT_ENA_M  ((GPIO_PIN36_INT_ENA_V)<<(GPIO_PIN36_INT_ENA_S))
//#define GPIO_PIN36_INT_ENA_V  0x1F
//#define GPIO_PIN36_INT_ENA_S  13
// /* GPIO_PIN36_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN36_CONFIG  0x00000003
//#define GPIO_PIN36_CONFIG_M  ((GPIO_PIN36_CONFIG_V)<<(GPIO_PIN36_CONFIG_S))
//#define GPIO_PIN36_CONFIG_V  0x3
//#define GPIO_PIN36_CONFIG_S  11
///* GPIO_PIN36_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN36_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN36_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN36_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN36_WAKEUP_ENABLE_S  10
///* GPIO_PIN36_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN36_INT_TYPE  0x00000007
//#define GPIO_PIN36_INT_TYPE_M  ((GPIO_PIN36_INT_TYPE_V)<<(GPIO_PIN36_INT_TYPE_S))
//#define GPIO_PIN36_INT_TYPE_V  0x7
//#define GPIO_PIN36_INT_TYPE_S  7
// /* GPIO_PIN36_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN36_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN36_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN36_PAD_DRIVER_V  0x1
//#define GPIO_PIN36_PAD_DRIVER_S  2
//
//#define GPIO_PIN37_REG          (DR_REG_GPIO_BASE + 0x011c)
///* GPIO_PIN37_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN37_INT_ENA  0x0000001F
//#define GPIO_PIN37_INT_ENA_M  ((GPIO_PIN37_INT_ENA_V)<<(GPIO_PIN37_INT_ENA_S))
//#define GPIO_PIN37_INT_ENA_V  0x1F
//#define GPIO_PIN37_INT_ENA_S  13
// /* GPIO_PIN37_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN37_CONFIG  0x00000003
//#define GPIO_PIN37_CONFIG_M  ((GPIO_PIN37_CONFIG_V)<<(GPIO_PIN37_CONFIG_S))
//#define GPIO_PIN37_CONFIG_V  0x3
//#define GPIO_PIN37_CONFIG_S  11
///* GPIO_PIN37_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN37_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN37_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN37_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN37_WAKEUP_ENABLE_S  10
///* GPIO_PIN37_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN37_INT_TYPE  0x00000007
//#define GPIO_PIN37_INT_TYPE_M  ((GPIO_PIN37_INT_TYPE_V)<<(GPIO_PIN37_INT_TYPE_S))
//#define GPIO_PIN37_INT_TYPE_V  0x7
//#define GPIO_PIN37_INT_TYPE_S  7
// /* GPIO_PIN37_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN37_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN37_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN37_PAD_DRIVER_V  0x1
//#define GPIO_PIN37_PAD_DRIVER_S  2
//
//#define GPIO_PIN38_REG          (DR_REG_GPIO_BASE + 0x0120)
///* GPIO_PIN38_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN38_INT_ENA  0x0000001F
//#define GPIO_PIN38_INT_ENA_M  ((GPIO_PIN38_INT_ENA_V)<<(GPIO_PIN38_INT_ENA_S))
//#define GPIO_PIN38_INT_ENA_V  0x1F
//#define GPIO_PIN38_INT_ENA_S  13
// /* GPIO_PIN38_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN38_CONFIG  0x00000003
//#define GPIO_PIN38_CONFIG_M  ((GPIO_PIN38_CONFIG_V)<<(GPIO_PIN38_CONFIG_S))
//#define GPIO_PIN38_CONFIG_V  0x3
//#define GPIO_PIN38_CONFIG_S  11
///* GPIO_PIN38_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN38_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN38_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN38_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN38_WAKEUP_ENABLE_S  10
///* GPIO_PIN38_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN38_INT_TYPE  0x00000007
//#define GPIO_PIN38_INT_TYPE_M  ((GPIO_PIN38_INT_TYPE_V)<<(GPIO_PIN38_INT_TYPE_S))
//#define GPIO_PIN38_INT_TYPE_V  0x7
//#define GPIO_PIN38_INT_TYPE_S  7
// /* GPIO_PIN38_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN38_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN38_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN38_PAD_DRIVER_V  0x1
//#define GPIO_PIN38_PAD_DRIVER_S  2
//
//#define GPIO_PIN39_REG          (DR_REG_GPIO_BASE + 0x0124)
///* GPIO_PIN39_INT_ENA : R/W ;bitpos:[17:13] ;default: x ; */
///*description: bit0: APP CPU interrupt enable  bit1: APP CPU non-maskable interrupt
// enable  bit3: PRO CPU interrupt enable  bit4:  PRO CPU non-maskable interrupt enable  bit5: SDIO's extent interrupt enable*/
//#define GPIO_PIN39_INT_ENA  0x0000001F
//#define GPIO_PIN39_INT_ENA_M  ((GPIO_PIN39_INT_ENA_V)<<(GPIO_PIN39_INT_ENA_S))
//#define GPIO_PIN39_INT_ENA_V  0x1F
//#define GPIO_PIN39_INT_ENA_S  13
// /* GPIO_PIN39_CONFIG : R/W ;bitpos:[12:11] ;default: x ; */
// /*description: NA*/
//#define GPIO_PIN39_CONFIG  0x00000003
//#define GPIO_PIN39_CONFIG_M  ((GPIO_PIN39_CONFIG_V)<<(GPIO_PIN39_CONFIG_S))
//#define GPIO_PIN39_CONFIG_V  0x3
//#define GPIO_PIN39_CONFIG_S  11
///* GPIO_PIN39_WAKEUP_ENABLE : R/W ;bitpos:[10] ;default: x ; */
///*description: GPIO wake up enable  only available in light sleep*/
//#define GPIO_PIN39_WAKEUP_ENABLE  (BIT(10))
//#define GPIO_PIN39_WAKEUP_ENABLE_M  (BIT(10))
//#define GPIO_PIN39_WAKEUP_ENABLE_V  0x1
//#define GPIO_PIN39_WAKEUP_ENABLE_S  10
///* GPIO_PIN39_INT_TYPE : R/W ;bitpos:[9:7] ;default: x ; */
///*description: if set to 0: GPIO interrupt disable  if set to 1: rising edge
// trigger  if set to 2: falling edge trigger  if set to 3: any edge trigger  if set to 4: low level trigger  if set to 5: high level trigger*/
//#define GPIO_PIN39_INT_TYPE  0x00000007
//#define GPIO_PIN39_INT_TYPE_M  ((GPIO_PIN39_INT_TYPE_V)<<(GPIO_PIN39_INT_TYPE_S))
//#define GPIO_PIN39_INT_TYPE_V  0x7
//#define GPIO_PIN39_INT_TYPE_S  7
// /* GPIO_PIN39_PAD_DRIVER : R/W ;bitpos:[2] ;default: x ; */
// /*description: if set to 0: normal output  if set to 1: open drain*/
//#define GPIO_PIN39_PAD_DRIVER  (BIT(2))
//#define GPIO_PIN39_PAD_DRIVER_M  (BIT(2))
//#define GPIO_PIN39_PAD_DRIVER_V  0x1
//#define GPIO_PIN39_PAD_DRIVER_S  2
//
//
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif

