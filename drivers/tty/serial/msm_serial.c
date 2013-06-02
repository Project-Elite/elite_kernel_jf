/*
<<<<<<< HEAD
 * drivers/serial/msm_serial.c - driver for msm7k serial device and console
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2009-2012, The Linux Foundation. All rights reserved.
 * Author: Robert Love <rlove@google.com>
=======
 * Driver for msm7k serial device and console
 *
 * Copyright (C) 2007 Google, Inc.
 * Author: Robert Love <rlove@google.com>
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
>>>>>>> remotes/linux2/linux-3.4.y
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#if defined(CONFIG_SERIAL_MSM_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
# define SUPPORT_SYSRQ
#endif

<<<<<<< HEAD
=======
#include <linux/atomic.h>
>>>>>>> remotes/linux2/linux-3.4.y
#include <linux/hrtimer.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/irq.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/delay.h>
=======
>>>>>>> remotes/linux2/linux-3.4.y
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
<<<<<<< HEAD
#include <linux/nmi.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <mach/msm_serial_pdata.h>
#include "msm_serial.h"


#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
enum msm_clk_states_e {
	MSM_CLK_PORT_OFF,     /* uart port not in use */
	MSM_CLK_OFF,          /* clock enabled */
	MSM_CLK_REQUEST_OFF,  /* disable after TX flushed */
	MSM_CLK_ON,           /* clock disabled */
};
#endif

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
/* optional low power wakeup, typically on a GPIO RX irq */
struct msm_wakeup {
	int irq;  /* < 0 indicates low power wakeup disabled */
	unsigned char ignore;  /* bool */

	/* bool: inject char into rx tty on wakeup */
	unsigned char inject_rx;
	char rx_to_inject;
};
#endif
=======
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_device.h>

#include "msm_serial.h"
>>>>>>> remotes/linux2/linux-3.4.y

struct msm_port {
	struct uart_port	uart;
	char			name[16];
	struct clk		*clk;
<<<<<<< HEAD
	unsigned int		imr;
#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	enum msm_clk_states_e	clk_state;
	struct hrtimer		clk_off_timer;
	ktime_t			clk_off_delay;
#endif
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	struct msm_wakeup wakeup;
#endif
	int			uim;
};

#define UART_TO_MSM(uart_port)	((struct msm_port *) uart_port)
#define is_console(port)	((port)->cons && \
				(port)->cons->index == (port)->line)


static inline void msm_write(struct uart_port *port, unsigned int val,
			     unsigned int off)
{
	__raw_writel(val, port->membase + off);
}

static inline unsigned int msm_read(struct uart_port *port, unsigned int off)
{
	return __raw_readl(port->membase + off);
}

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
static inline unsigned int use_low_power_wakeup(struct msm_port *msm_port)
{
	return (msm_port->wakeup.irq >= 0);
}
#endif
=======
	struct clk		*pclk;
	unsigned int		imr;
	unsigned int            *gsbi_base;
	int			is_uartdm;
	unsigned int		old_snap_state;
};

static inline void wait_for_xmitr(struct uart_port *port, int bits)
{
	if (!(msm_read(port, UART_SR) & UART_SR_TX_EMPTY))
		while ((msm_read(port, UART_ISR) & bits) != bits)
			cpu_relax();
}
>>>>>>> remotes/linux2/linux-3.4.y

static void msm_stop_tx(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

	msm_port->imr &= ~UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);
}

static void msm_start_tx(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

	msm_port->imr |= UART_IMR_TXLEV;
	msm_write(port, msm_port->imr, UART_IMR);
}

static void msm_stop_rx(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

	msm_port->imr &= ~(UART_IMR_RXLEV | UART_IMR_RXSTALE);
	msm_write(port, msm_port->imr, UART_IMR);
}

static void msm_enable_ms(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

	msm_port->imr |= UART_IMR_DELTA_CTS;
	msm_write(port, msm_port->imr, UART_IMR);
}

<<<<<<< HEAD
#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
/* turn clock off if TX buffer is empty, otherwise reschedule */
static enum hrtimer_restart msm_serial_clock_off(struct hrtimer *timer) {
	struct msm_port *msm_port = container_of(timer, struct msm_port,
						 clk_off_timer);
	struct uart_port *port = &msm_port->uart;
	struct circ_buf *xmit = &port->state->xmit;
	unsigned long flags;
	int ret = HRTIMER_NORESTART;

	spin_lock_irqsave(&port->lock, flags);

	if (msm_port->clk_state == MSM_CLK_REQUEST_OFF) {
		if (uart_circ_empty(xmit)) {
			struct msm_port *msm_port = UART_TO_MSM(port);
			clk_disable(msm_port->clk);
			msm_port->clk_state = MSM_CLK_OFF;
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
			if (use_low_power_wakeup(msm_port)) {
				msm_port->wakeup.ignore = 1;
				enable_irq(msm_port->wakeup.irq);
			}
#endif
		} else {
			hrtimer_forward_now(timer, msm_port->clk_off_delay);
			ret = HRTIMER_RESTART;
		}
	}

	spin_unlock_irqrestore(&port->lock, flags);

	return HRTIMER_NORESTART;
}

/* request to turn off uart clock once pending TX is flushed */
void msm_serial_clock_request_off(struct uart_port *port) {
	unsigned long flags;
	struct msm_port *msm_port = UART_TO_MSM(port);

	spin_lock_irqsave(&port->lock, flags);
	if (msm_port->clk_state == MSM_CLK_ON) {
		msm_port->clk_state = MSM_CLK_REQUEST_OFF;
		/* turn off TX later. unfortunately not all msm uart's have a
		 * TXDONE available, and TXLEV does not wait until completely
		 * flushed, so a timer is our only option
		 */
		hrtimer_start(&msm_port->clk_off_timer,
			      msm_port->clk_off_delay, HRTIMER_MODE_REL);
	}
	spin_unlock_irqrestore(&port->lock, flags);
}

/* request to immediately turn on uart clock.
 * ignored if there is a pending off request, unless force = 1.
 */
void msm_serial_clock_on(struct uart_port *port, int force) {
	unsigned long flags;
	struct msm_port *msm_port = UART_TO_MSM(port);

	spin_lock_irqsave(&port->lock, flags);

	switch (msm_port->clk_state) {
	case MSM_CLK_OFF:
		clk_enable(msm_port->clk);
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
		if (use_low_power_wakeup(msm_port))
			disable_irq(msm_port->wakeup.irq);
#endif
		force = 1;
	case MSM_CLK_REQUEST_OFF:
		if (force) {
			hrtimer_try_to_cancel(&msm_port->clk_off_timer);
			msm_port->clk_state = MSM_CLK_ON;
		}
		break;
	case MSM_CLK_ON: break;
	case MSM_CLK_PORT_OFF: break;
	}

	spin_unlock_irqrestore(&port->lock, flags);
}
#endif

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
static irqreturn_t msm_rx_irq(int irq, void *dev_id)
{
	unsigned long flags;
	struct uart_port *port = dev_id;
	struct msm_port *msm_port = UART_TO_MSM(port);
	int inject_wakeup = 0;

	spin_lock_irqsave(&port->lock, flags);

	if (msm_port->clk_state == MSM_CLK_OFF) {
		/* ignore the first irq - it is a pending irq that occured
		 * before enable_irq() */
		if (msm_port->wakeup.ignore)
			msm_port->wakeup.ignore = 0;
		else
			inject_wakeup = 1;
	}

	msm_serial_clock_on(port, 0);

	/* we missed an rx while asleep - it must be a wakeup indicator
	 */
	if (inject_wakeup) {
		struct tty_struct *tty = port->state->port.tty;
		tty_insert_flip_char(tty, WAKE_UP_IND, TTY_NORMAL);
		tty_flip_buffer_push(tty);
	}

	spin_unlock_irqrestore(&port->lock, flags);
	return IRQ_HANDLED;
}
#endif
=======
static void handle_rx_dm(struct uart_port *port, unsigned int misr)
{
	struct tty_struct *tty = port->state->port.tty;
	unsigned int sr;
	int count = 0;
	struct msm_port *msm_port = UART_TO_MSM(port);

	if ((msm_read(port, UART_SR) & UART_SR_OVERRUN)) {
		port->icount.overrun++;
		tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	}

	if (misr & UART_IMR_RXSTALE) {
		count = msm_read(port, UARTDM_RX_TOTAL_SNAP) -
			msm_port->old_snap_state;
		msm_port->old_snap_state = 0;
	} else {
		count = 4 * (msm_read(port, UART_RFWR));
		msm_port->old_snap_state += count;
	}

	/* TODO: Precise error reporting */

	port->icount.rx += count;

	while (count > 0) {
		unsigned int c;

		sr = msm_read(port, UART_SR);
		if ((sr & UART_SR_RX_READY) == 0) {
			msm_port->old_snap_state -= count;
			break;
		}
		c = msm_read(port, UARTDM_RF);
		if (sr & UART_SR_RX_BREAK) {
			port->icount.brk++;
			if (uart_handle_break(port))
				continue;
		} else if (sr & UART_SR_PAR_FRAME_ERR)
			port->icount.frame++;

		/* TODO: handle sysrq */
		tty_insert_flip_string(tty, (char *) &c,
				       (count > 4) ? 4 : count);
		count -= 4;
	}

	tty_flip_buffer_push(tty);
	if (misr & (UART_IMR_RXSTALE))
		msm_write(port, UART_CR_CMD_RESET_STALE_INT, UART_CR);
	msm_write(port, 0xFFFFFF, UARTDM_DMRX);
	msm_write(port, UART_CR_CMD_STALE_EVENT_ENABLE, UART_CR);
}
>>>>>>> remotes/linux2/linux-3.4.y

static void handle_rx(struct uart_port *port)
{
	struct tty_struct *tty = port->state->port.tty;
	unsigned int sr;

	/*
	 * Handle overrun. My understanding of the hardware is that overrun
	 * is not tied to the RX buffer, so we handle the case out of band.
	 */
	if ((msm_read(port, UART_SR) & UART_SR_OVERRUN)) {
		port->icount.overrun++;
		tty_insert_flip_char(tty, 0, TTY_OVERRUN);
		msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	}

	/* and now the main RX loop */
	while ((sr = msm_read(port, UART_SR)) & UART_SR_RX_READY) {
		unsigned int c;
		char flag = TTY_NORMAL;

		c = msm_read(port, UART_RF);

		if (sr & UART_SR_RX_BREAK) {
			port->icount.brk++;
			if (uart_handle_break(port))
				continue;
		} else if (sr & UART_SR_PAR_FRAME_ERR) {
			port->icount.frame++;
		} else {
			port->icount.rx++;
		}

		/* Mask conditions we're ignorning. */
		sr &= port->read_status_mask;

		if (sr & UART_SR_RX_BREAK) {
			flag = TTY_BREAK;
		} else if (sr & UART_SR_PAR_FRAME_ERR) {
			flag = TTY_FRAME;
		}

		if (!uart_handle_sysrq_char(port, c))
			tty_insert_flip_char(tty, c, flag);
	}

	tty_flip_buffer_push(tty);
}

<<<<<<< HEAD
=======
static void reset_dm_count(struct uart_port *port)
{
	wait_for_xmitr(port, UART_ISR_TX_READY);
	msm_write(port, 1, UARTDM_NCF_TX);
}

>>>>>>> remotes/linux2/linux-3.4.y
static void handle_tx(struct uart_port *port)
{
	struct circ_buf *xmit = &port->state->xmit;
	struct msm_port *msm_port = UART_TO_MSM(port);
<<<<<<< HEAD
	int sent_tx = 0;

	if (port->x_char) {
		msm_write(port, port->x_char, UART_TF);
=======
	int sent_tx;

	if (port->x_char) {
		if (msm_port->is_uartdm)
			reset_dm_count(port);

		msm_write(port, port->x_char,
			  msm_port->is_uartdm ? UARTDM_TF : UART_TF);
>>>>>>> remotes/linux2/linux-3.4.y
		port->icount.tx++;
		port->x_char = 0;
	}

<<<<<<< HEAD
=======
	if (msm_port->is_uartdm)
		reset_dm_count(port);

>>>>>>> remotes/linux2/linux-3.4.y
	while (msm_read(port, UART_SR) & UART_SR_TX_READY) {
		if (uart_circ_empty(xmit)) {
			/* disable tx interrupts */
			msm_port->imr &= ~UART_IMR_TXLEV;
			msm_write(port, msm_port->imr, UART_IMR);
			break;
		}
<<<<<<< HEAD

		msm_write(port, xmit->buf[xmit->tail], UART_TF);
=======
		msm_write(port, xmit->buf[xmit->tail],
			  msm_port->is_uartdm ? UARTDM_TF : UART_TF);

		if (msm_port->is_uartdm)
			reset_dm_count(port);
>>>>>>> remotes/linux2/linux-3.4.y

		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
		sent_tx = 1;
	}

<<<<<<< HEAD
#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	if (sent_tx && msm_port->clk_state == MSM_CLK_REQUEST_OFF)
		/* new TX - restart the timer */
		if (hrtimer_try_to_cancel(&msm_port->clk_off_timer) == 1)
			hrtimer_start(&msm_port->clk_off_timer,
				msm_port->clk_off_delay, HRTIMER_MODE_REL);
#endif

=======
>>>>>>> remotes/linux2/linux-3.4.y
	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);
}

static void handle_delta_cts(struct uart_port *port)
{
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
	port->icount.cts++;
	wake_up_interruptible(&port->state->port.delta_msr_wait);
}

static irqreturn_t msm_irq(int irq, void *dev_id)
{
<<<<<<< HEAD
	unsigned long flags;
=======
>>>>>>> remotes/linux2/linux-3.4.y
	struct uart_port *port = dev_id;
	struct msm_port *msm_port = UART_TO_MSM(port);
	unsigned int misr;

<<<<<<< HEAD
	spin_lock_irqsave(&port->lock, flags);
	misr = msm_read(port, UART_MISR);
	msm_write(port, 0, UART_IMR); /* disable interrupt */

	if (misr & (UART_IMR_RXLEV | UART_IMR_RXSTALE))
		handle_rx(port);
=======
	spin_lock(&port->lock);
	misr = msm_read(port, UART_MISR);
	msm_write(port, 0, UART_IMR); /* disable interrupt */

	if (misr & (UART_IMR_RXLEV | UART_IMR_RXSTALE)) {
		if (msm_port->is_uartdm)
			handle_rx_dm(port, misr);
		else
			handle_rx(port);
	}
>>>>>>> remotes/linux2/linux-3.4.y
	if (misr & UART_IMR_TXLEV)
		handle_tx(port);
	if (misr & UART_IMR_DELTA_CTS)
		handle_delta_cts(port);

	msm_write(port, msm_port->imr, UART_IMR); /* restore interrupt */
<<<<<<< HEAD
	spin_unlock_irqrestore(&port->lock, flags);
=======
	spin_unlock(&port->lock);
>>>>>>> remotes/linux2/linux-3.4.y

	return IRQ_HANDLED;
}

static unsigned int msm_tx_empty(struct uart_port *port)
{
<<<<<<< HEAD
	unsigned int ret;

	ret = (msm_read(port, UART_SR) & UART_SR_TX_EMPTY) ? TIOCSER_TEMT : 0;
	return ret;
=======
	return (msm_read(port, UART_SR) & UART_SR_TX_EMPTY) ? TIOCSER_TEMT : 0;
>>>>>>> remotes/linux2/linux-3.4.y
}

static unsigned int msm_get_mctrl(struct uart_port *port)
{
	return TIOCM_CAR | TIOCM_CTS | TIOCM_DSR | TIOCM_RTS;
}

<<<<<<< HEAD
static void msm_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	unsigned int mr;

=======

static void msm_reset(struct uart_port *port)
{
	/* reset everything */
	msm_write(port, UART_CR_CMD_RESET_RX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_TX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_BREAK_INT, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
	msm_write(port, UART_CR_CMD_SET_RFR, UART_CR);
}

void msm_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	unsigned int mr;
>>>>>>> remotes/linux2/linux-3.4.y
	mr = msm_read(port, UART_MR1);

	if (!(mctrl & TIOCM_RTS)) {
		mr &= ~UART_MR1_RX_RDY_CTL;
		msm_write(port, mr, UART_MR1);
		msm_write(port, UART_CR_CMD_RESET_RFR, UART_CR);
	} else {
		mr |= UART_MR1_RX_RDY_CTL;
		msm_write(port, mr, UART_MR1);
	}
}

static void msm_break_ctl(struct uart_port *port, int break_ctl)
{
	if (break_ctl)
		msm_write(port, UART_CR_CMD_START_BREAK, UART_CR);
	else
		msm_write(port, UART_CR_CMD_STOP_BREAK, UART_CR);
}

<<<<<<< HEAD
static void msm_set_baud_rate(struct uart_port *port, unsigned int baud)
{
	unsigned int baud_code, rxstale, watermark;
=======
static int msm_set_baud_rate(struct uart_port *port, unsigned int baud)
{
	unsigned int baud_code, rxstale, watermark;
	struct msm_port *msm_port = UART_TO_MSM(port);
>>>>>>> remotes/linux2/linux-3.4.y

	switch (baud) {
	case 300:
		baud_code = UART_CSR_300;
		rxstale = 1;
		break;
	case 600:
		baud_code = UART_CSR_600;
		rxstale = 1;
		break;
	case 1200:
		baud_code = UART_CSR_1200;
		rxstale = 1;
		break;
	case 2400:
		baud_code = UART_CSR_2400;
		rxstale = 1;
		break;
	case 4800:
		baud_code = UART_CSR_4800;
		rxstale = 1;
		break;
	case 9600:
		baud_code = UART_CSR_9600;
		rxstale = 2;
		break;
	case 14400:
		baud_code = UART_CSR_14400;
		rxstale = 3;
		break;
	case 19200:
		baud_code = UART_CSR_19200;
		rxstale = 4;
		break;
	case 28800:
		baud_code = UART_CSR_28800;
		rxstale = 6;
		break;
	case 38400:
		baud_code = UART_CSR_38400;
		rxstale = 8;
		break;
	case 57600:
		baud_code = UART_CSR_57600;
		rxstale = 16;
		break;
	case 115200:
	default:
		baud_code = UART_CSR_115200;
<<<<<<< HEAD
=======
		baud = 115200;
>>>>>>> remotes/linux2/linux-3.4.y
		rxstale = 31;
		break;
	}

<<<<<<< HEAD
=======
	if (msm_port->is_uartdm)
		msm_write(port, UART_CR_CMD_RESET_RX, UART_CR);

>>>>>>> remotes/linux2/linux-3.4.y
	msm_write(port, baud_code, UART_CSR);

	/* RX stale watermark */
	watermark = UART_IPR_STALE_LSB & rxstale;
	watermark |= UART_IPR_RXSTALE_LAST;
	watermark |= UART_IPR_STALE_TIMEOUT_MSB & (rxstale << 2);
	msm_write(port, watermark, UART_IPR);

	/* set RX watermark */
	watermark = (port->fifosize * 3) / 4;
	msm_write(port, watermark, UART_RFWR);

	/* set TX watermark */
	msm_write(port, 10, UART_TFWR);
<<<<<<< HEAD
}

static void msm_reset(struct uart_port *port)
{
	/* reset everything */
	msm_write(port, UART_CR_CMD_RESET_RX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_TX, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_ERR, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_BREAK_INT, UART_CR);
	msm_write(port, UART_CR_CMD_RESET_CTS, UART_CR);
	msm_write(port, UART_CR_CMD_SET_RFR, UART_CR);
}

static void msm_init_clock(struct uart_port *port)
{
	int ret;
	struct msm_port *msm_port = UART_TO_MSM(port);

	ret = clk_prepare_enable(msm_port->clk);
	if (ret) {
		pr_err("%s(): Can't enable uartclk. ret:%d\n", __func__, ret);
		return;
	}

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	msm_port->clk_state = MSM_CLK_ON;
#endif

	if (msm_port->uim) {
		msm_write(port,
			UART_SIM_CFG_STOP_BIT_LEN_N(2) |
			UART_SIM_CFG_SIM_CLK_ON |
			UART_SIM_CFG_SIM_CLK_STOP_HIGH |
			UART_SIM_CFG_MASK_RX |
			UART_SIM_CFG_SIM_SEL,
			UART_SIM_CFG);

		/* (TCXO * 16) / (5 * 372) = TCXO * 16 / 1860 */
		msm_write(port, 0x08, UART_MREG);
		msm_write(port, 0x19, UART_NREG);
		msm_write(port, 0xe8, UART_DREG);
		msm_write(port, 0x0e, UART_MNDREG);
	} else if (port->uartclk == 19200000) {
		/* clock is TCXO (19.2MHz) */
		msm_write(port, 0x06, UART_MREG);
		msm_write(port, 0xF1, UART_NREG);
		msm_write(port, 0x0F, UART_DREG);
		msm_write(port, 0x1A, UART_MNDREG);
	} else {
		/* clock must be TCXO/4 */
		msm_write(port, 0x18, UART_MREG);
		msm_write(port, 0xF6, UART_NREG);
		msm_write(port, 0x0F, UART_DREG);
		msm_write(port, 0x0A, UART_MNDREG);
	}
}

static void msm_deinit_clock(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	if (msm_port->clk_state != MSM_CLK_OFF)
		clk_disable(msm_port->clk);
	msm_port->clk_state = MSM_CLK_PORT_OFF;
#else
	clk_disable_unprepare(msm_port->clk);
#endif

}
=======

	if (msm_port->is_uartdm) {
		msm_write(port, UART_CR_CMD_RESET_STALE_INT, UART_CR);
		msm_write(port, 0xFFFFFF, UARTDM_DMRX);
		msm_write(port, UART_CR_CMD_STALE_EVENT_ENABLE, UART_CR);
	}

	return baud;
}


static void msm_init_clock(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

	clk_enable(msm_port->clk);
	if (!IS_ERR(msm_port->pclk))
		clk_enable(msm_port->pclk);
	msm_serial_set_mnd_regs(port);
}

>>>>>>> remotes/linux2/linux-3.4.y
static int msm_startup(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);
	unsigned int data, rfr_level;
	int ret;

	snprintf(msm_port->name, sizeof(msm_port->name),
		 "msm_serial%d", port->line);

	ret = request_irq(port->irq, msm_irq, IRQF_TRIGGER_HIGH,
			  msm_port->name, port);
	if (unlikely(ret))
		return ret;

<<<<<<< HEAD
	if (unlikely(irq_set_irq_wake(port->irq, 1))) {
		free_irq(port->irq, port);
		return -ENXIO;
	}

#ifndef CONFIG_PM_RUNTIME
	msm_init_clock(port);
#endif
	pm_runtime_get_sync(port->dev);
=======
	msm_init_clock(port);
>>>>>>> remotes/linux2/linux-3.4.y

	if (likely(port->fifosize > 12))
		rfr_level = port->fifosize - 12;
	else
		rfr_level = port->fifosize;

	/* set automatic RFR level */
	data = msm_read(port, UART_MR1);
	data &= ~UART_MR1_AUTO_RFR_LEVEL1;
	data &= ~UART_MR1_AUTO_RFR_LEVEL0;
	data |= UART_MR1_AUTO_RFR_LEVEL1 & (rfr_level << 2);
	data |= UART_MR1_AUTO_RFR_LEVEL0 & rfr_level;
	msm_write(port, data, UART_MR1);

	/* make sure that RXSTALE count is non-zero */
	data = msm_read(port, UART_IPR);
	if (unlikely(!data)) {
		data |= UART_IPR_RXSTALE_LAST;
		data |= UART_IPR_STALE_LSB;
		msm_write(port, data, UART_IPR);
	}

<<<<<<< HEAD
	msm_reset(port);

	msm_write(port, 0x05, UART_CR);	/* enable TX & RX */
=======
	data = 0;
	if (!port->cons || (port->cons && !(port->cons->flags & CON_ENABLED))) {
		msm_write(port, UART_CR_CMD_PROTECTION_EN, UART_CR);
		msm_reset(port);
		data = UART_CR_TX_ENABLE;
	}

	data |= UART_CR_RX_ENABLE;
	msm_write(port, data, UART_CR);	/* enable TX & RX */

	/* Make sure IPR is not 0 to start with*/
	if (msm_port->is_uartdm)
		msm_write(port, UART_IPR_STALE_LSB, UART_IPR);
>>>>>>> remotes/linux2/linux-3.4.y

	/* turn on RX and CTS interrupts */
	msm_port->imr = UART_IMR_RXLEV | UART_IMR_RXSTALE |
			UART_IMR_CURRENT_CTS;
<<<<<<< HEAD
	msm_write(port, msm_port->imr, UART_IMR);

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	if (use_low_power_wakeup(msm_port)) {
		ret = irq_set_irq_wake(msm_port->wakeup.irq, 1);
		if (unlikely(ret))
			return ret;
		ret = request_irq(msm_port->wakeup.irq, msm_rx_irq,
				  IRQF_TRIGGER_FALLING,
				  "msm_serial_wakeup", msm_port);
		if (unlikely(ret))
			return ret;
		disable_irq(msm_port->wakeup.irq);
	}
#endif

=======

	if (msm_port->is_uartdm) {
		msm_write(port, 0xFFFFFF, UARTDM_DMRX);
		msm_write(port, UART_CR_CMD_RESET_STALE_INT, UART_CR);
		msm_write(port, UART_CR_CMD_STALE_EVENT_ENABLE, UART_CR);
	}

	msm_write(port, msm_port->imr, UART_IMR);
>>>>>>> remotes/linux2/linux-3.4.y
	return 0;
}

static void msm_shutdown(struct uart_port *port)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
	if (msm_port->uim)
		msm_write(port,
			UART_SIM_CFG_SIM_CLK_STOP_HIGH,
			UART_SIM_CFG);

	msm_port->imr = 0;
	msm_write(port, 0, UART_IMR); /* disable interrupts */

	free_irq(port->irq, port);

#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	if (use_low_power_wakeup(msm_port)) {
		irq_set_irq_wake(msm_port->wakeup.irq, 0);
		free_irq(msm_port->wakeup.irq, msm_port);
	}
#endif
#ifndef CONFIG_PM_RUNTIME
	msm_deinit_clock(port);
#endif
	pm_runtime_put_sync(port->dev);
=======
	msm_port->imr = 0;
	msm_write(port, 0, UART_IMR); /* disable interrupts */

	clk_disable(msm_port->clk);

	free_irq(port->irq, port);
>>>>>>> remotes/linux2/linux-3.4.y
}

static void msm_set_termios(struct uart_port *port, struct ktermios *termios,
			    struct ktermios *old)
{
	unsigned long flags;
	unsigned int baud, mr;

<<<<<<< HEAD
	if (!termios->c_cflag)
		return;

=======
>>>>>>> remotes/linux2/linux-3.4.y
	spin_lock_irqsave(&port->lock, flags);

	/* calculate and set baud rate */
	baud = uart_get_baud_rate(port, termios, old, 300, 115200);
<<<<<<< HEAD
	msm_set_baud_rate(port, baud);
=======
	baud = msm_set_baud_rate(port, baud);
	if (tty_termios_baud_rate(termios))
		tty_termios_encode_baud_rate(termios, baud, baud);
>>>>>>> remotes/linux2/linux-3.4.y

	/* calculate parity */
	mr = msm_read(port, UART_MR2);
	mr &= ~UART_MR2_PARITY_MODE;
	if (termios->c_cflag & PARENB) {
		if (termios->c_cflag & PARODD)
			mr |= UART_MR2_PARITY_MODE_ODD;
		else if (termios->c_cflag & CMSPAR)
			mr |= UART_MR2_PARITY_MODE_SPACE;
		else
			mr |= UART_MR2_PARITY_MODE_EVEN;
	}

	/* calculate bits per char */
	mr &= ~UART_MR2_BITS_PER_CHAR;
	switch (termios->c_cflag & CSIZE) {
	case CS5:
		mr |= UART_MR2_BITS_PER_CHAR_5;
		break;
	case CS6:
		mr |= UART_MR2_BITS_PER_CHAR_6;
		break;
	case CS7:
		mr |= UART_MR2_BITS_PER_CHAR_7;
		break;
	case CS8:
	default:
		mr |= UART_MR2_BITS_PER_CHAR_8;
		break;
	}

	/* calculate stop bits */
	mr &= ~(UART_MR2_STOP_BIT_LEN_ONE | UART_MR2_STOP_BIT_LEN_TWO);
	if (termios->c_cflag & CSTOPB)
		mr |= UART_MR2_STOP_BIT_LEN_TWO;
	else
		mr |= UART_MR2_STOP_BIT_LEN_ONE;

	/* set parity, bits per char, and stop bit */
	msm_write(port, mr, UART_MR2);

	/* calculate and set hardware flow control */
	mr = msm_read(port, UART_MR1);
	mr &= ~(UART_MR1_CTS_CTL | UART_MR1_RX_RDY_CTL);
	if (termios->c_cflag & CRTSCTS) {
		mr |= UART_MR1_CTS_CTL;
		mr |= UART_MR1_RX_RDY_CTL;
	}
	msm_write(port, mr, UART_MR1);

	/* Configure status bits to ignore based on termio flags. */
	port->read_status_mask = 0;
	if (termios->c_iflag & INPCK)
		port->read_status_mask |= UART_SR_PAR_FRAME_ERR;
	if (termios->c_iflag & (BRKINT | PARMRK))
		port->read_status_mask |= UART_SR_RX_BREAK;

	uart_update_timeout(port, termios->c_cflag, baud);
<<<<<<< HEAD
=======

>>>>>>> remotes/linux2/linux-3.4.y
	spin_unlock_irqrestore(&port->lock, flags);
}

static const char *msm_type(struct uart_port *port)
{
	return "MSM";
}

static void msm_release_port(struct uart_port *port)
{
	struct platform_device *pdev = to_platform_device(port->dev);
<<<<<<< HEAD
	struct resource *resource;
	resource_size_t size;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return;
	size = resource->end - resource->start + 1;
=======
	struct msm_port *msm_port = UART_TO_MSM(port);
	struct resource *uart_resource;
	struct resource *gsbi_resource;
	resource_size_t size;

	uart_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!uart_resource))
		return;
	size = resource_size(uart_resource);
>>>>>>> remotes/linux2/linux-3.4.y

	release_mem_region(port->mapbase, size);
	iounmap(port->membase);
	port->membase = NULL;
<<<<<<< HEAD
=======

	if (msm_port->gsbi_base) {
		iowrite32(GSBI_PROTOCOL_IDLE, msm_port->gsbi_base +
			  GSBI_CONTROL);

		gsbi_resource = platform_get_resource(pdev,
							IORESOURCE_MEM, 1);

		if (unlikely(!gsbi_resource))
			return;

		size = resource_size(gsbi_resource);
		release_mem_region(gsbi_resource->start, size);
		iounmap(msm_port->gsbi_base);
		msm_port->gsbi_base = NULL;
	}
>>>>>>> remotes/linux2/linux-3.4.y
}

static int msm_request_port(struct uart_port *port)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *resource;
	resource_size_t size;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	size = resource->end - resource->start + 1;

	if (unlikely(!request_mem_region(port->mapbase, size, "msm_serial")))
=======
	struct msm_port *msm_port = UART_TO_MSM(port);
	struct platform_device *pdev = to_platform_device(port->dev);
	struct resource *uart_resource;
	struct resource *gsbi_resource;
	resource_size_t size;
	int ret;

	uart_resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!uart_resource))
		return -ENXIO;

	size = resource_size(uart_resource);

	if (!request_mem_region(port->mapbase, size, "msm_serial"))
>>>>>>> remotes/linux2/linux-3.4.y
		return -EBUSY;

	port->membase = ioremap(port->mapbase, size);
	if (!port->membase) {
<<<<<<< HEAD
		release_mem_region(port->mapbase, size);
		return -EBUSY;
	}

	return 0;
=======
		ret = -EBUSY;
		goto fail_release_port;
	}

	gsbi_resource = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	/* Is this a GSBI-based port? */
	if (gsbi_resource) {
		size = resource_size(gsbi_resource);

		if (!request_mem_region(gsbi_resource->start, size,
						 "msm_serial")) {
			ret = -EBUSY;
			goto fail_release_port;
		}

		msm_port->gsbi_base = ioremap(gsbi_resource->start, size);
		if (!msm_port->gsbi_base) {
			ret = -EBUSY;
			goto fail_release_gsbi;
		}
	}

	return 0;

fail_release_gsbi:
	release_mem_region(gsbi_resource->start, size);
fail_release_port:
	release_mem_region(port->mapbase, size);
	return ret;
>>>>>>> remotes/linux2/linux-3.4.y
}

static void msm_config_port(struct uart_port *port, int flags)
{
<<<<<<< HEAD
	if (flags & UART_CONFIG_TYPE) {
		port->type = PORT_MSM;
		msm_request_port(port);
	}
=======
	struct msm_port *msm_port = UART_TO_MSM(port);
	int ret;
	if (flags & UART_CONFIG_TYPE) {
		port->type = PORT_MSM;
		ret = msm_request_port(port);
		if (ret)
			return;
	}

	if (msm_port->is_uartdm)
		iowrite32(GSBI_PROTOCOL_UART, msm_port->gsbi_base +
			  GSBI_CONTROL);
>>>>>>> remotes/linux2/linux-3.4.y
}

static int msm_verify_port(struct uart_port *port, struct serial_struct *ser)
{
	if (unlikely(ser->type != PORT_UNKNOWN && ser->type != PORT_MSM))
		return -EINVAL;
	if (unlikely(port->irq != ser->irq))
		return -EINVAL;
	return 0;
}

static void msm_power(struct uart_port *port, unsigned int state,
		      unsigned int oldstate)
{
<<<<<<< HEAD
	int ret;
=======
>>>>>>> remotes/linux2/linux-3.4.y
	struct msm_port *msm_port = UART_TO_MSM(port);

	switch (state) {
	case 0:
<<<<<<< HEAD
		ret = clk_prepare_enable(msm_port->clk);
		if (ret)
			pr_err("msm_serial: %s(): Can't enable uartclk.\n",
						__func__);
		break;
	case 3:
		clk_disable_unprepare(msm_port->clk);
		break;
	default:
		pr_err("msm_serial: %s(): Unknown PM state %d\n",
						__func__, state);
=======
		clk_enable(msm_port->clk);
		if (!IS_ERR(msm_port->pclk))
			clk_enable(msm_port->pclk);
		break;
	case 3:
		clk_disable(msm_port->clk);
		if (!IS_ERR(msm_port->pclk))
			clk_disable(msm_port->pclk);
		break;
	default:
		printk(KERN_ERR "msm_serial: Unknown PM state %d\n", state);
>>>>>>> remotes/linux2/linux-3.4.y
	}
}

static struct uart_ops msm_uart_pops = {
	.tx_empty = msm_tx_empty,
	.set_mctrl = msm_set_mctrl,
	.get_mctrl = msm_get_mctrl,
	.stop_tx = msm_stop_tx,
	.start_tx = msm_start_tx,
	.stop_rx = msm_stop_rx,
	.enable_ms = msm_enable_ms,
	.break_ctl = msm_break_ctl,
	.startup = msm_startup,
	.shutdown = msm_shutdown,
	.set_termios = msm_set_termios,
	.type = msm_type,
	.release_port = msm_release_port,
	.request_port = msm_request_port,
	.config_port = msm_config_port,
	.verify_port = msm_verify_port,
	.pm = msm_power,
};

static struct msm_port msm_uart_ports[] = {
	{
		.uart = {
			.iotype = UPIO_MEM,
			.ops = &msm_uart_pops,
			.flags = UPF_BOOT_AUTOCONF,
<<<<<<< HEAD
			.fifosize = 512,
=======
			.fifosize = 64,
>>>>>>> remotes/linux2/linux-3.4.y
			.line = 0,
		},
	},
	{
		.uart = {
			.iotype = UPIO_MEM,
			.ops = &msm_uart_pops,
			.flags = UPF_BOOT_AUTOCONF,
<<<<<<< HEAD
			.fifosize = 512,
=======
			.fifosize = 64,
>>>>>>> remotes/linux2/linux-3.4.y
			.line = 1,
		},
	},
	{
		.uart = {
			.iotype = UPIO_MEM,
			.ops = &msm_uart_pops,
			.flags = UPF_BOOT_AUTOCONF,
			.fifosize = 64,
			.line = 2,
		},
	},
};

#define UART_NR	ARRAY_SIZE(msm_uart_ports)

<<<<<<< HEAD
static inline struct uart_port * get_port_from_line(unsigned int line)
=======
static inline struct uart_port *get_port_from_line(unsigned int line)
>>>>>>> remotes/linux2/linux-3.4.y
{
	return &msm_uart_ports[line].uart;
}

#ifdef CONFIG_SERIAL_MSM_CONSOLE

<<<<<<< HEAD
/*
 *  Wait for transmitter & holding register to empty
 *  Derived from wait_for_xmitr in 8250 serial driver by Russell King
 */
static inline void wait_for_xmitr(struct uart_port *port, int bits)
{
	unsigned int status, mr, tmout = 10000;

	/* Wait up to 10ms for the character(s) to be sent. */
	do {
		status = msm_read(port, UART_SR);

		if (--tmout == 0)
			break;
		udelay(1);
	} while ((status & bits) != bits);

	mr = msm_read(port, UART_MR1);

	/* Wait up to 1s for flow control if necessary */
	if (mr & UART_MR1_CTS_CTL) {
		unsigned int tmout;
		for (tmout = 1000000; tmout; tmout--) {
			unsigned int isr = msm_read(port, UART_ISR);

			/* CTS input is active lo */
			if (!(isr & UART_IMR_CURRENT_CTS))
				break;
			udelay(1);
			touch_nmi_watchdog();
		}
	}
}


static void msm_console_putchar(struct uart_port *port, int c)
{
	/* This call can incur significant delay if CTS flowcontrol is enabled
	 * on port and no serial cable is attached.
	 */
	wait_for_xmitr(port, UART_SR_TX_READY);

	msm_write(port, c, UART_TF);
=======
static void msm_console_putchar(struct uart_port *port, int c)
{
	struct msm_port *msm_port = UART_TO_MSM(port);

	if (msm_port->is_uartdm)
		reset_dm_count(port);

	while (!(msm_read(port, UART_SR) & UART_SR_TX_READY))
		;
	msm_write(port, c, msm_port->is_uartdm ? UARTDM_TF : UART_TF);
>>>>>>> remotes/linux2/linux-3.4.y
}

static void msm_console_write(struct console *co, const char *s,
			      unsigned int count)
{
	struct uart_port *port;
	struct msm_port *msm_port;
<<<<<<< HEAD
	int locked;
=======
>>>>>>> remotes/linux2/linux-3.4.y

	BUG_ON(co->index < 0 || co->index >= UART_NR);

	port = get_port_from_line(co->index);
	msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
	/* not pretty, but we can end up here via various convoluted paths */
	if (port->sysrq || oops_in_progress)
		locked = spin_trylock(&port->lock);
	else {
		locked = 1;
		spin_lock(&port->lock);
	}

	uart_console_write(port, s, count, msm_console_putchar);

	if (locked)
		spin_unlock(&port->lock);
=======
	spin_lock(&port->lock);
	uart_console_write(port, s, count, msm_console_putchar);
	spin_unlock(&port->lock);
>>>>>>> remotes/linux2/linux-3.4.y
}

static int __init msm_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
<<<<<<< HEAD
	int baud = 0, flow, bits, parity;
=======
	struct msm_port *msm_port;
	int baud, flow, bits, parity;
>>>>>>> remotes/linux2/linux-3.4.y

	if (unlikely(co->index >= UART_NR || co->index < 0))
		return -ENXIO;

	port = get_port_from_line(co->index);
<<<<<<< HEAD
=======
	msm_port = UART_TO_MSM(port);
>>>>>>> remotes/linux2/linux-3.4.y

	if (unlikely(!port->membase))
		return -ENXIO;

<<<<<<< HEAD
	port->cons = co;

	pm_runtime_get_noresume(port->dev);

#ifndef CONFIG_PM_RUNTIME
	msm_init_clock(port);
#endif
	pm_runtime_resume(port->dev);
=======
	msm_init_clock(port);
>>>>>>> remotes/linux2/linux-3.4.y

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);

	bits = 8;
	parity = 'n';
	flow = 'n';
	msm_write(port, UART_MR2_BITS_PER_CHAR_8 | UART_MR2_STOP_BIT_LEN_ONE,
		  UART_MR2);	/* 8N1 */

	if (baud < 300 || baud > 115200)
		baud = 115200;
	msm_set_baud_rate(port, baud);

	msm_reset(port);

<<<<<<< HEAD
=======
	if (msm_port->is_uartdm) {
		msm_write(port, UART_CR_CMD_PROTECTION_EN, UART_CR);
		msm_write(port, UART_CR_TX_ENABLE, UART_CR);
	}

>>>>>>> remotes/linux2/linux-3.4.y
	printk(KERN_INFO "msm_serial: console setup on port #%d\n", port->line);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct uart_driver msm_uart_driver;

static struct console msm_console = {
	.name = "ttyMSM",
	.write = msm_console_write,
	.device = uart_console_device,
	.setup = msm_console_setup,
	.flags = CON_PRINTBUFFER,
	.index = -1,
	.data = &msm_uart_driver,
};

<<<<<<< HEAD
#define MSM_CONSOLE	&msm_console
=======
#define MSM_CONSOLE	(&msm_console)
>>>>>>> remotes/linux2/linux-3.4.y

#else
#define MSM_CONSOLE	NULL
#endif

static struct uart_driver msm_uart_driver = {
	.owner = THIS_MODULE,
	.driver_name = "msm_serial",
	.dev_name = "ttyMSM",
	.nr = UART_NR,
	.cons = MSM_CONSOLE,
};

<<<<<<< HEAD
=======
static atomic_t msm_uart_next_id = ATOMIC_INIT(0);

>>>>>>> remotes/linux2/linux-3.4.y
static int __init msm_serial_probe(struct platform_device *pdev)
{
	struct msm_port *msm_port;
	struct resource *resource;
	struct uart_port *port;
	int irq;
<<<<<<< HEAD
#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	struct msm_serial_platform_data *pdata = pdev->dev.platform_data;
#endif
=======

	if (pdev->id == -1)
		pdev->id = atomic_inc_return(&msm_uart_next_id) - 1;
>>>>>>> remotes/linux2/linux-3.4.y

	if (unlikely(pdev->id < 0 || pdev->id >= UART_NR))
		return -ENXIO;

	printk(KERN_INFO "msm_serial: detected port #%d\n", pdev->id);

	port = get_port_from_line(pdev->id);
	port->dev = &pdev->dev;
	msm_port = UART_TO_MSM(port);

<<<<<<< HEAD
	msm_port->clk = clk_get(&pdev->dev, "core_clk");
	if (unlikely(IS_ERR(msm_port->clk)))
		return PTR_ERR(msm_port->clk);
	port->uartclk = clk_get_rate(msm_port->clk);
	if (!port->uartclk)
		port->uartclk = 19200000;

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	port->mapbase = resource->start;

	irq = platform_get_irq(pdev, 0);
	if (unlikely(irq < 0))
		return -ENXIO;
	port->irq = irq;

	platform_set_drvdata(pdev, port);


#ifdef CONFIG_SERIAL_MSM_RX_WAKEUP
	if (pdata == NULL)
		msm_port->wakeup.irq = -1;
	else {
		msm_port->wakeup.irq = pdata->wakeup_irq;
		msm_port->wakeup.ignore = 1;
		msm_port->wakeup.inject_rx = pdata->inject_rx_on_wakeup;
		msm_port->wakeup.rx_to_inject = pdata->rx_to_inject;

		if (unlikely(msm_port->wakeup.irq <= 0))
			return -EINVAL;
	}
#endif

#ifdef CONFIG_SERIAL_MSM_CLOCK_CONTROL
	msm_port->clk_state = MSM_CLK_PORT_OFF;
	hrtimer_init(&msm_port->clk_off_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	msm_port->clk_off_timer.function = msm_serial_clock_off;
	msm_port->clk_off_delay = ktime_set(0, 1000000);  /* 1 ms */
#endif

	pm_runtime_enable(port->dev);
	return uart_add_one_port(&msm_uart_driver, port);
}

static int __init msm_uim_probe(struct platform_device *pdev)
{
	struct msm_port *msm_port;
	struct resource *resource;
	struct uart_port *port;
	int irq;

	if (unlikely(pdev->id < 0 || pdev->id >= UART_NR))
		return -ENXIO;

	pr_info("msm_uim: detected port #%d\n", pdev->id);

	port = get_port_from_line(pdev->id);
	port->dev = &pdev->dev;
	msm_port = UART_TO_MSM(port);

	msm_port->uim = true;
=======
	if (platform_get_resource(pdev, IORESOURCE_MEM, 1))
		msm_port->is_uartdm = 1;
	else
		msm_port->is_uartdm = 0;

	if (msm_port->is_uartdm) {
		msm_port->clk = clk_get(&pdev->dev, "gsbi_uart_clk");
		msm_port->pclk = clk_get(&pdev->dev, "gsbi_pclk");
	} else {
		msm_port->clk = clk_get(&pdev->dev, "uart_clk");
		msm_port->pclk = ERR_PTR(-ENOENT);
	}

	if (unlikely(IS_ERR(msm_port->clk) || (IS_ERR(msm_port->pclk) &&
					       msm_port->is_uartdm)))
			return PTR_ERR(msm_port->clk);

	if (msm_port->is_uartdm)
		clk_set_rate(msm_port->clk, 7372800);

	port->uartclk = clk_get_rate(msm_port->clk);
	printk(KERN_INFO "uartclk = %d\n", port->uartclk);

>>>>>>> remotes/linux2/linux-3.4.y

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(!resource))
		return -ENXIO;
	port->mapbase = resource->start;

	irq = platform_get_irq(pdev, 0);
	if (unlikely(irq < 0))
		return -ENXIO;
	port->irq = irq;

	platform_set_drvdata(pdev, port);

	return uart_add_one_port(&msm_uart_driver, port);
}

static int __devexit msm_serial_remove(struct platform_device *pdev)
{
	struct msm_port *msm_port = platform_get_drvdata(pdev);

<<<<<<< HEAD
	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

=======
>>>>>>> remotes/linux2/linux-3.4.y
	clk_put(msm_port->clk);

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int msm_serial_suspend(struct device *dev)
{
	struct uart_port *port;
	struct platform_device *pdev = to_platform_device(dev);
	port = get_port_from_line(pdev->id);

	if (port) {
		uart_suspend_port(&msm_uart_driver, port);
		if (is_console(port))
			msm_deinit_clock(port);
	}

	return 0;
}

static int msm_serial_resume(struct device *dev)
{
	struct uart_port *port;
	struct platform_device *pdev = to_platform_device(dev);
	port = get_port_from_line(pdev->id);

	if (port) {
		if (is_console(port))
			msm_init_clock(port);
		uart_resume_port(&msm_uart_driver, port);
	}

	return 0;
}
#else
#define msm_serial_suspend NULL
#define msm_serial_resume NULL
#endif

static int msm_serial_runtime_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct uart_port *port;
	port = get_port_from_line(pdev->id);

	dev_dbg(dev, "pm_runtime: suspending\n");
	msm_deinit_clock(port);
	return 0;
}

static int msm_serial_runtime_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct uart_port *port;
	port = get_port_from_line(pdev->id);

	dev_dbg(dev, "pm_runtime: resuming\n");
	msm_init_clock(port);
	return 0;
}

static struct dev_pm_ops msm_serial_dev_pm_ops = {
	.suspend = msm_serial_suspend,
	.resume = msm_serial_resume,
	.runtime_suspend = msm_serial_runtime_suspend,
	.runtime_resume = msm_serial_runtime_resume,
=======
static struct of_device_id msm_match_table[] = {
	{ .compatible = "qcom,msm-uart" },
	{}
>>>>>>> remotes/linux2/linux-3.4.y
};

static struct platform_driver msm_platform_driver = {
	.remove = msm_serial_remove,
	.driver = {
		.name = "msm_serial",
		.owner = THIS_MODULE,
<<<<<<< HEAD
		.pm = &msm_serial_dev_pm_ops,
	},
};

static struct platform_driver msm_platform_uim_driver = {
	.remove = msm_serial_remove,
	.driver = {
		.name = "msm_uim",
		.owner = THIS_MODULE,
=======
		.of_match_table = msm_match_table,
>>>>>>> remotes/linux2/linux-3.4.y
	},
};

static int __init msm_serial_init(void)
{
	int ret;

	ret = uart_register_driver(&msm_uart_driver);
	if (unlikely(ret))
		return ret;

	ret = platform_driver_probe(&msm_platform_driver, msm_serial_probe);
	if (unlikely(ret))
		uart_unregister_driver(&msm_uart_driver);

<<<<<<< HEAD
	platform_driver_probe(&msm_platform_uim_driver, msm_uim_probe);

=======
>>>>>>> remotes/linux2/linux-3.4.y
	printk(KERN_INFO "msm_serial: driver initialized\n");

	return ret;
}

static void __exit msm_serial_exit(void)
{
#ifdef CONFIG_SERIAL_MSM_CONSOLE
	unregister_console(&msm_console);
#endif
	platform_driver_unregister(&msm_platform_driver);
	uart_unregister_driver(&msm_uart_driver);
}

module_init(msm_serial_init);
module_exit(msm_serial_exit);

MODULE_AUTHOR("Robert Love <rlove@google.com>");
MODULE_DESCRIPTION("Driver for msm7x serial device");
<<<<<<< HEAD
MODULE_LICENSE("GPL v2");
=======
MODULE_LICENSE("GPL");
>>>>>>> remotes/linux2/linux-3.4.y
