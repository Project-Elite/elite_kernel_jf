<<<<<<< HEAD
/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.
=======
/* arch/arm/mach-msm/clock.h
 *
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2007-2010, Code Aurora Forum. All rights reserved.
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
 *
 */

#ifndef __ARCH_ARM_MACH_MSM_CLOCK_H
#define __ARCH_ARM_MACH_MSM_CLOCK_H

<<<<<<< HEAD
#include <linux/clkdev.h>

/**
 * struct clock_init_data - SoC specific clock initialization data
 * @table: table of lookups to add
 * @size: size of @table
 * @pre_init: called before initializing the clock driver.
 * @post_init: called after registering @table. clock APIs can be called inside.
 * @late_init: called during late init
 */
struct clock_init_data {
	struct clk_lookup *table;
	size_t size;
	void (*pre_init)(void);
	void (*post_init)(void);
	int (*late_init)(void);
};

extern struct clock_init_data msm9615_clock_init_data;
extern struct clock_init_data apq8064_clock_init_data;
extern struct clock_init_data fsm9xxx_clock_init_data;
extern struct clock_init_data msm7x01a_clock_init_data;
extern struct clock_init_data msm7x27_clock_init_data;
extern struct clock_init_data msm7x27a_clock_init_data;
extern struct clock_init_data msm7x30_clock_init_data;
extern struct clock_init_data msm8960_clock_init_data;
extern struct clock_init_data msm8x60_clock_init_data;
extern struct clock_init_data qds8x50_clock_init_data;
extern struct clock_init_data msm8625_dummy_clock_init_data;
extern struct clock_init_data msm8930_clock_init_data;
extern struct clock_init_data msm8930_pm8917_clock_init_data;
extern struct clock_init_data msm8974_clock_init_data;
extern struct clock_init_data msm8974_rumi_clock_init_data;

int msm_clock_init(struct clock_init_data *data);
int find_vdd_level(struct clk *clk, unsigned long rate);

#ifdef CONFIG_DEBUG_FS
int clock_debug_init(void);
int clock_debug_register(struct clk_lookup *t, size_t s);
void clock_debug_print_enabled(void);
#else
static inline int clock_debug_init(void) { return 0; }
static inline int clock_debug_register(struct clk_lookup *t, size_t s)
{
	return 0;
}
static inline void clock_debug_print_enabled(void) { return; }
=======
#include <linux/init.h>
#include <linux/list.h>
#include <mach/clk.h>

#define CLKFLAG_INVERT			0x00000001
#define CLKFLAG_NOINVERT		0x00000002
#define CLKFLAG_NONEST			0x00000004
#define CLKFLAG_NORESET			0x00000008

#define CLK_FIRST_AVAILABLE_FLAG	0x00000100
#define CLKFLAG_AUTO_OFF		0x00000200
#define CLKFLAG_MIN			0x00000400
#define CLKFLAG_MAX			0x00000800

struct clk_ops {
	int (*enable)(unsigned id);
	void (*disable)(unsigned id);
	void (*auto_off)(unsigned id);
	int (*reset)(unsigned id, enum clk_reset_action action);
	int (*set_rate)(unsigned id, unsigned rate);
	int (*set_min_rate)(unsigned id, unsigned rate);
	int (*set_max_rate)(unsigned id, unsigned rate);
	int (*set_flags)(unsigned id, unsigned flags);
	unsigned (*get_rate)(unsigned id);
	unsigned (*is_enabled)(unsigned id);
	long (*round_rate)(unsigned id, unsigned rate);
	bool (*is_local)(unsigned id);
};

struct clk {
	uint32_t id;
	uint32_t remote_id;
	uint32_t count;
	uint32_t flags;
	struct clk_ops *ops;
	const char *dbg_name;
	struct list_head list;
};

#define OFF CLKFLAG_AUTO_OFF
#define CLK_MIN CLKFLAG_MIN
#define CLK_MAX CLKFLAG_MAX
#define CLK_MINMAX (CLK_MIN | CLK_MAX)

#ifdef CONFIG_DEBUG_FS
int __init clock_debug_init(void);
int __init clock_debug_add(struct clk *clock);
#else
static inline int __init clock_debug_init(void) { return 0; }
static inline int __init clock_debug_add(struct clk *clock) { return 0; }
>>>>>>> remotes/linux2/linux-3.4.y
#endif

#endif
