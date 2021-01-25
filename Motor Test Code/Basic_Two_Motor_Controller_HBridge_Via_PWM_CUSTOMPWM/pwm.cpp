//////////////////////////////////////////////////////////////////
//File:				pwm.cpp
//Author(s):		Jesse Federkiewicz / Tim Hachey
//Description:		PWM functionality to drive all four motors
//Creation date:	2021-01-20
//Last modified:	2021-01-25
//////////////////////////////////////////////////////////////////


#include "soc/soc.h"
#include "soc/gpio_reg.h"
#include "soc/mcpwm_reg.h"


//void pwmChanInit(u8Int channel, u8Int gpioNum, u8Int unitNum, byte prClk, byte sclClk, u8Int period, u8Int duty)
void pwmChanInit()
{
	//if (channel > 6 || channel < 0)
	//	throw("PWM.cpp:pwmChanInit : Valid channels between 0-5");

	//if (gpioNum < 0 || gpioNum > 39)
	//	throw("PWM.cpp:pwmChanInit : Valid gpio are 0-39");

	//else
	//	throw("PWM.cpp:pwmChanInit : Module was not front or back - how did you even get here?");
	 
	//WRITE_PERI_REG();
	//WRITE_PERI_REG(GPIO_ENABLE_REG, 0b110000000000000000000000);

	///////////////
	WRITE_PERI_REG(GPIO_ENABLE_REG, 0xFFFFFFFF);	//5.7
	WRITE_PERI_REG(GPIO_ENABLE1_REG, 0xFF);			//5.10

	WRITE_PERI_REG(GPIO_FUNC22_OUT_SEL, 32);	//5.32, 32 from other table 5.9
	WRITE_PERI_REG(GPIO_FUNC22_OEN_SEL, 1);		//5.32 (5.3)
	WRITE_PERI_REG(GPIO_FUNC22_OEN_INV_SEL, 0); //NO INVERT 5.32
	WRITE_PERI_REG(GPIO_FUNC22_OUT_INV_SEL, 0);	//DO NOT INVERT OUTPUT VALUE
	////WRITE_PERI_REG(GPIO_ENABLE_DATA)
	WRITE_PERI_REG(GPIO_PIN22_PAD_DRIVER, 0);	//5.3.2 make push pull mode
	////WRITE_PERI_REG(IO_MUX_GPIO22_REG, );		
	////WRITE_PERI_REG(MCU_SEL, IO_MUX_GPIO22_REG);	//5.3.2
	WRITE_PERI_REG(IO_MUX_GPIO22_REG, 0b10100110101101);

	WRITE_PERI_REG(GPIO_FUNC23_OUT_SEL, 108);	//5.32, 108 from other table 5.9
	WRITE_PERI_REG(GPIO_FUNC23_OEN_SEL, 1);		//5.32 (5.3)
	WRITE_PERI_REG(GPIO_FUNC23_OEN_INV_SEL, 0); //NO INVERT 5.32
	WRITE_PERI_REG(GPIO_FUNC23_OUT_INV_SEL, 0);	//DO NOT INVERT OUTPUT VALUE
	WRITE_PERI_REG(GPIO_PIN23_PAD_DRIVER, 0);	//5.3.2 make push pull mode
	////WRITE_PERI_REG(MCU_SEL, IO_MUX_GPIO23_REG);
	WRITE_PERI_REG(IO_MUX_GPIO23_REG, 0b10100110101101);


	//// 17.2
	WRITE_PERI_REG(MCPWM_TIMER0_PERIOD_UPMETHOD, 0);	//set to immediate
	WRITE_PERI_REG(MCPWM_TIMER0_PERIOD, 200);			//PERIOD TO 100, 100 (100-200) EFFECTIVE?
	WRITE_PERI_REG(MCPWM_TIMER0_)
	//WRITE_PERI_REG(MCPWM_TIMER0_DUTY);
	//WRITE_PERI_REG(MCPWM_TIMER0_PRESCALE, 249);			//SET PRESCALE TO 249
	//WRITE_PERI_REG(MCPWM_TIMER0_CFG0_REG, 63);	//17.3 SET OTHER PRESCALE TO 63
	//WRITE_PERI_REG(0x3FF5E004, 249);			//SET PRESCALE TO 249
	//WRITE_PERI_REG(0x3FF5E014, 63);			//17.3 SET OTHER PRESCALE TO 63
	WRITE_PERI_REG(MCPWM_CLK_CFG_REG(0), 63);
	WRITE_PERI_REG(MCPWM_TIMER0_CFG0_REG(0), 0xFF000 | 249);
	//////////////

	//WRITE_PERI_REG();

	//pinMode();
	//ESP_REG();

	/////////////
	//WRITE_PERI_REG(MCPWM_CLK_CFG_REG(0), READ_PERI_REG((MCPWM_CLK_CFG_REG(0)) & 0b11111111111111111111111100000000) | 10); // 17.4 write prclk, 17.5 (17.1), 8 bit
	//WRITE_PERI_REG(MCPWM_CLK_CFG_REG(1), READ_PERI_REG((MCPWM_CLK_CFG_REG(1)) & 0b11111111111111111111111100000000) | 10); // 17.4 write prclk, 17.5 (17.1), 8 bit
	//WRITE_PERI_REG(MCPWM_TIMER0_CFG0_REG(0), READ_PERI_REG((MCPWM_TIMER0_CFG0_REG(0)) & 0b111111000000000000000000000000) | 0b100000000110010000000001);	//17.4 set upmethod, timer period, timer prescale, 17.5 (17.2)
	//																					//period to 100? prescale of 1 for now?
	//WRITE_PERI_REG(MCPWM_TIMER0_CFG1_REG(0), READ_PERI_REG((MCPWM_TIMER0_CFG1_REG(0)) & 0b1111111111111111111111111110000)| 0b0110) //17.4, 17.5 (17.3)
	//																		//increase mode (1), timer runs on (2)

	//WRITE_PERI_REG(MCPWM_OPERATOR_TIMERSEL_REG(0), READ_PERI_REG((MCPWM_OPERATOR_TIMERSEL_REG(0) & 0b11111111111111111111111111000000) | 0));	//17.15 operator 0 wit timer 0
	///////////

	//if (frontOrRearMotors == FrontMotors)
	//{
	//	
	//}

	//else if (frontOrRearMotors == RearMotors)
	//{

	//}

	
}

//static double pwmSetupTimerFreq(uint8_t chan, double freq, uint8_t bit_num)
//{
//	longInt clk_freq = 240000000;
//	clk_freq <<= 8;//div_num is 8 bit decimal
//	longInt div_num = (clk_freq >> bit_num) / freq;
//	bool apb_clk = true;
//	if (div_num > LEDC_DIV_NUM_HSTIMER0_V) {
//		clk_freq /= 80;
//		div_num = (clk_freq >> bit_num) / freq;
//		if (div_num > LEDC_DIV_NUM_HSTIMER0_V) {
//			div_num = LEDC_DIV_NUM_HSTIMER0_V;//lowest clock possible
//		}
//		apb_clk = false;
//	}
//	else if (div_num < 256) {
//		div_num = 256;//highest clock possible
//	}
//	_ledcSetupTimer(chan, div_num, bit_num, apb_clk);
//	//log_i("Fin: %f, Fclk: %uMhz, bits: %u, DIV: %u, Fout: %f",
//	//        freq, apb_clk?80:1, bit_num, div_num, (clk_freq >> bit_num) / (double)div_num);
//	return (clk_freq >> bit_num) / (double)div_num;
//}


//void pwmInit()
//{
//	
//}
//
//void pwmWrite(u8Int chan, uint period, uint duty)
//{
//	if (chan > 15) {
//		return;
//	}
//	
//	//PWM0_OUT0A_IDX
//	
//}
//
//void pwmRead(u8Int chan)
//{
//
//}


//void mcpwm_gpio_init(u8Int mcpwm_num/*, mcpwm_io_signals_t io_signal*/, int gpio_num)
//{
//	//periph_module_enable(PERIPH_PWM0_MODULE + mcpwm_num);
//	PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[gpio_num], PIN_FUNC_GPIO);
//	bool mcpwm_gpio_sig = (io_signal <= MCPWM2B);
//
//	if (mcpwm_num == MCPWM_UNIT_0) {
//		if (mcpwm_gpio_sig) {
//			MCPWM_CHECK((GPIO_IS_VALID_OUTPUT_GPIO(gpio_num)), MCPWM_GPIO_ERROR, ESP_ERR_INVALID_ARG);
//			gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
//			esp_rom_gpio_connect_out_signal(gpio_num, PWM0_OUT0A_IDX + io_signal, 0, 0);
//		}
//		else {
//			gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
//			esp_rom_gpio_connect_in_signal(gpio_num, PWM0_SYNC0_IN_IDX + io_signal - OFFSET_FOR_GPIO_IDX_1, 0);
//		}
//	}
//	else { //MCPWM_UNIT_1
//		if (mcpwm_gpio_sig) {
//			MCPWM_CHECK((GPIO_IS_VALID_OUTPUT_GPIO(gpio_num)), MCPWM_GPIO_ERROR, ESP_ERR_INVALID_ARG);
//			gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
//			esp_rom_gpio_connect_out_signal(gpio_num, PWM1_OUT0A_IDX + io_signal, 0, 0);
//		}
//		else if (io_signal >= MCPWM_SYNC_0 && io_signal <= MCPWM_FAULT_2) {
//			gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
//			esp_rom_gpio_connect_in_signal(gpio_num, PWM1_SYNC0_IN_IDX + io_signal - OFFSET_FOR_GPIO_IDX_1, 0);
//		}
//		else {
//			gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
//			esp_rom_gpio_connect_in_signal(gpio_num, PWM1_SYNC0_IN_IDX + io_signal - OFFSET_FOR_GPIO_IDX_2, 0);
//		}
//	}
//}



