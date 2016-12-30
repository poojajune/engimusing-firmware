/*
  Copyright (c) 2016 Engimusing LLC.  All right reserved.
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "delay.h"
#include "em_device.h"
#include "pins_arduino.h"
#include "efm_lib/coreclk.h"
#include "efm_lib/UARTClass.h"

// Tick Counter united by ms
static volatile uint32_t _ulTickCount = 0;

#ifdef __cplusplus
extern "C" {
#endif

  uint32_t millis( void )
  {
    // todo: ensure no interrupts
    return _ulTickCount ;
  }

  // Interrupt-compatible version of micros
  // Theory: repeatedly take readings of SysTick counter, millis counter and SysTick interrupt pending flag.
  // When it appears that millis counter and pending is stable and SysTick hasn't rolled over, use these
  // values to calculate micros. If there is a pending SysTick, add one to the millis counter in the calculation.
  uint32_t micros( void )
  {
    uint32_t ticks, ticks2;
    uint32_t pend, pend2;
    uint32_t count, count2;

    ticks2  = SysTick->VAL;
    pend2   = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)  ;
    count2  = _ulTickCount ;

    do {
      ticks   = ticks2;
      pend    = pend2;
      count   = count2;
      ticks2  = SysTick->VAL;
      pend2   = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)  ;
      count2  = _ulTickCount ;
    } while ((pend != pend2) || (count != count2) || (ticks < ticks2));

    return ((count+pend) * 1000) + (((SysTick->LOAD  - ticks)*(1048576/(VARIANT_MCK/1000000)))>>20) ;
    // this is an optimization to turn a runtime division into two compile-time divisions and
    // a runtime multiplication and shift, saving a few cycles
  }

  void delayMicroseconds( uint32_t usec )
  {
    if ( usec == 0 )
      {
	return ;
      }

    uint32_t start = micros();
    while ((micros() - start) < usec)
        ;
    #if 0 //this way stalled on the Energy micro
	/*
     *  The following loop:
     *
     *    for (; ul; ul--) {
     *      __asm__ volatile("");
     *    }
     *
     *  produce the following assembly code:
     *
     *    loop:
     *      subs r3, #1        // 1 Core cycle
     *      bne.n loop         // 1 Core cycle + 1 if branch is taken
     */

    // VARIANT_MCK / 1000000 == cycles needed to delay 1uS
    //                     3 == cycles used in a loop
    uint32_t n = usec * (VARIANT_MCK / 1000000) / 3;
	Serial.println(n);
    __asm__ __volatile__(
			 "1:              \n"
			 "   sub %0, #1   \n" // substract 1 from %0 (n)
			 "   bne 1b       \n" // if result is not 0 jump to 1
			 : "+r" (n)           // '%0' is n variable with RW constraints
			 :                    // no input
			 :                    // no clobber
			 );
#endif
    // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
    // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Volatile
  }

  void yield(void) {}

  void delay( uint32_t ms )
  {
    uint32_t start = _ulTickCount ;

    if ( ms == 0 ) {
      return ;
    }
    do {
      yield() ;
    } while ( _ulTickCount - start <= ms ) ;
  }


  void SysTick_Handler(void)
  {
    _ulTickCount++;    // Increment tick count each ms
    //  tickReset();
  }

#ifdef __cplusplus
}
#endif

