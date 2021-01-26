//////////////////////////////////////////////////////////////////////////////////////
//  
// Program:     Scale Calculator
// Author:      Tim Hachey
//
// Description: this program will take a desired pwm clock 
//              frequency and calculate the PWM_CLK_PRESCALE
//              and PWM_TIMERx_PRESCALE values and display the top 25 
//              closest matches for actual output frequency according to 
//              the formulas found in the esp32 Technical Reference Manual
//              register decriptions (section 17.5)
//
//////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scale_Calculator
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Enter desired frequency (Hz): ");

            float desiredFreq;

            float.TryParse(Console.ReadLine(),out desiredFreq);

            float desiredPer = 1.0f / desiredFreq;

            float baseCLK = 160000000;
            float basePer = 1 / baseCLK;

            float PWM_CLK_PRESCALE;
            float PWM_TIMERx_PRESCALE;

            float PWM_clk_Per;
            float PT0_clk_per;

            Console.WriteLine($"desired Period = {desiredPer}s");            

            List<Stat> list = new List<Stat>();

            for (int i = 0; i < 0xFF; i++)
            {
                for (int j = 0; j < 0xFF; j++)
                {
                    PWM_CLK_PRESCALE = i;
                    PWM_TIMERx_PRESCALE = j;

                    PWM_clk_Per = basePer * (PWM_CLK_PRESCALE + 1);
                    PT0_clk_per = PWM_clk_Per * (PWM_TIMERx_PRESCALE + 1);

                    list.Add(new Stat(PT0_clk_per, PWM_CLK_PRESCALE, PWM_TIMERx_PRESCALE));
                }
            }
            list=list.OrderBy(s => Math.Abs(desiredFreq - s.freq)).ToList();

            foreach(Stat s in list.Take(25))
            {
                Console.WriteLine($"Freq: {(1 / s.period):f3} - Period: {s.period:f9}- PWM_CLK_PRESCALE: {s.PWM_CLK_PRESCALE}- PWM_TIMERx_PRESCALE: {s.PWM_TIMERx_PRESCALE}");
            }
            Console.ReadKey();
        }
    }
    class Stat
    {
        public float freq;
        public float period;
        public float PWM_CLK_PRESCALE;
        public float PWM_TIMERx_PRESCALE;

        public Stat(float per, float PRESCALE, float TIMER0_PRESCALE)
        {
            freq = 1 / per;
            period = per;
            PWM_CLK_PRESCALE = PRESCALE;
            PWM_TIMERx_PRESCALE = TIMER0_PRESCALE;
        }
    }
}
