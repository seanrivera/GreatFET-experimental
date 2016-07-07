/*
 * Copyright 2016 Sean Rivera
 *
 * This file is part of GreatFET.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "rtc.h" 
#include <libopencm3/lpc43xx/rtc.h> 
#include <libopencm3/lpc43xx/creg.h> 
#include <libopencm3/lpc43xx/scu.h>
#include <libopencm3/lpc43xx/cgu.h>
#include <libopencm3/lpc43xx/m4/nvic.h>

void init_rtc (void) {
	// Configure clock to RTC
	CREG_CREG0 &= ~((1<<3)|(1<<2));                                    // Reset 32Khz oscillator
	CREG_CREG0 |= (1<<1)|(1<<0);                                               // Enable 32 kHz & 1 kHz on osc32k and release reset
	SCU_SFSCLK0 = 1 | (0x3<<2);                                               // function 1; CGU clk out, pull down
	CGU_BASE_OUT_CLK  = (CGU_CLKSRC_32KHZ_OSC<<24) |(1<<11);            // base clock out use 32KHz crystal and auto block
	do
	{
		/* Reset RTC clock*/
		RTC_CCR = RTC_CCR_CTCRST_MASK | RTC_CCR_CCALEN_MASK;
	}
	while(RTC_CCR!=(RTC_CCR_CTCRST_MASK | RTC_CCR_CCALEN_MASK));
	do
	{
		/* Finish resetting RTC clock*/
		RTC_CCR = RTC_CCR_CCALEN_MASK;
	}
	while(RTC_CCR != RTC_CCR_CCALEN_MASK);
	/* Clear counter increment and alarm interrupt */
	RTC_ILR = RTC_ILR_RTCCIF_MASK | RTC_ILR_RTCALF_MASK;
	while(RTC_ILR!=0);
	// Clear all register to be default
	RTC_CIIR = 0x00;
	RTC_AMR = 0xFF;
	RTC_CALIBRATION = 0x00;
 
    	nvic_set_priority(NVIC_RTC_IRQ, 1);
	nvic_enable_irq(NVIC_RTC_IRQ);

}
