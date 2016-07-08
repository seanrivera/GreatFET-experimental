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

#include "rtc_isr.h" 
#include <libopencm3/lpc43xx/rtc.h> 
#include <libopencm3/lpc43xx/m4/nvic.h> 
#include "greatfet_core.h"

void rtc_isr_blinky(void){
	RTC_ILR = RTC_ILR_RTCCIF_MASK | RTC_ILR_RTCALF_MASK;
	led_toggle(LED3);
}
void EVRT_IRQHandler(void) {
	nvic_clear_pending_irq(NVIC_EVENTROUTER_IRQ);
        rtc_isr_blinky();
}

