<<<<<<< HEAD
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
>>>>>>> remotes/linux2/linux-3.4.y
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
<<<<<<< HEAD
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
>>>>>>> remotes/linux2/linux-3.4.y
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/usb/msm_hsusb_hw.h>
#include <linux/usb/ulpi.h>
<<<<<<< HEAD
#include <linux/gpio.h>
=======
>>>>>>> remotes/linux2/linux-3.4.y

#include "ci13xxx_udc.c"

#define MSM_USB_BASE	(udc->regs)

<<<<<<< HEAD
struct ci13xxx_udc_context {
	int irq;
	void __iomem *regs;
	int wake_gpio;
	int wake_irq;
	bool wake_irq_state;
};

static struct ci13xxx_udc_context _udc_ctxt;

=======
>>>>>>> remotes/linux2/linux-3.4.y
static irqreturn_t msm_udc_irq(int irq, void *data)
{
	return udc_irq();
}

<<<<<<< HEAD
static void ci13xxx_msm_suspend(void)
{
	struct device *dev = _udc->gadget.dev.parent;
	dev_dbg(dev, "ci13xxx_msm_suspend\n");

	if (_udc_ctxt.wake_irq && !_udc_ctxt.wake_irq_state) {
		enable_irq_wake(_udc_ctxt.wake_irq);
		enable_irq(_udc_ctxt.wake_irq);
		_udc_ctxt.wake_irq_state = true;
	}
}

static void ci13xxx_msm_resume(void)
{
	struct device *dev = _udc->gadget.dev.parent;
	dev_dbg(dev, "ci13xxx_msm_resume\n");

	if (_udc_ctxt.wake_irq && _udc_ctxt.wake_irq_state) {
		disable_irq_wake(_udc_ctxt.wake_irq);
		disable_irq_nosync(_udc_ctxt.wake_irq);
		_udc_ctxt.wake_irq_state = false;
	}
}

static void ci13xxx_msm_notify_event(struct ci13xxx *udc, unsigned event)
{
	struct device *dev = udc->gadget.dev.parent;

	switch (event) {
	case CI13XXX_CONTROLLER_RESET_EVENT:
		dev_info(dev, "CI13XXX_CONTROLLER_RESET_EVENT received\n");
		writel(0, USB_AHBBURST);
		writel_relaxed(0x08, USB_AHBMODE);
		break;
	case CI13XXX_CONTROLLER_DISCONNECT_EVENT:
		dev_info(dev, "CI13XXX_CONTROLLER_DISCONNECT_EVENT received\n");
		ci13xxx_msm_resume();
		break;
	case CI13XXX_CONTROLLER_SUSPEND_EVENT:
		dev_info(dev, "CI13XXX_CONTROLLER_SUSPEND_EVENT received\n");
		ci13xxx_msm_suspend();
		break;
	case CI13XXX_CONTROLLER_RESUME_EVENT:
		dev_info(dev, "CI13XXX_CONTROLLER_RESUME_EVENT received\n");
		ci13xxx_msm_resume();
		break;

=======
static void ci13xxx_msm_notify_event(struct ci13xxx *udc, unsigned event)
{
	struct device *dev = udc->gadget.dev.parent;
	int val;

	switch (event) {
	case CI13XXX_CONTROLLER_RESET_EVENT:
		dev_dbg(dev, "CI13XXX_CONTROLLER_RESET_EVENT received\n");
		writel(0, USB_AHBBURST);
		writel(0, USB_AHBMODE);
		break;
	case CI13XXX_CONTROLLER_STOPPED_EVENT:
		dev_dbg(dev, "CI13XXX_CONTROLLER_STOPPED_EVENT received\n");
		/*
		 * Put the transceiver in non-driving mode. Otherwise host
		 * may not detect soft-disconnection.
		 */
		val = usb_phy_io_read(udc->transceiver, ULPI_FUNC_CTRL);
		val &= ~ULPI_FUNC_CTRL_OPMODE_MASK;
		val |= ULPI_FUNC_CTRL_OPMODE_NONDRIVING;
		usb_phy_io_write(udc->transceiver, val, ULPI_FUNC_CTRL);
		break;
>>>>>>> remotes/linux2/linux-3.4.y
	default:
		dev_dbg(dev, "unknown ci13xxx_udc event\n");
		break;
	}
}

<<<<<<< HEAD
static irqreturn_t ci13xxx_msm_resume_irq(int irq, void *data)
{
	struct ci13xxx *udc = _udc;

	if (udc->transceiver && udc->vbus_active && udc->suspended)
		usb_phy_set_suspend(udc->transceiver, 0);
	else if (!udc->suspended)
		ci13xxx_msm_resume();

	return IRQ_HANDLED;
}

=======
>>>>>>> remotes/linux2/linux-3.4.y
static struct ci13xxx_udc_driver ci13xxx_msm_udc_driver = {
	.name			= "ci13xxx_msm",
	.flags			= CI13XXX_REGS_SHARED |
				  CI13XXX_REQUIRE_TRANSCEIVER |
				  CI13XXX_PULLUP_ON_VBUS |
<<<<<<< HEAD
				  CI13XXX_ZERO_ITC |
				  CI13XXX_DISABLE_STREAMING |
				  CI13XXX_IS_OTG,
=======
				  CI13XXX_DISABLE_STREAMING,
>>>>>>> remotes/linux2/linux-3.4.y

	.notify_event		= ci13xxx_msm_notify_event,
};

<<<<<<< HEAD
static int ci13xxx_msm_install_wake_gpio(struct platform_device *pdev,
				struct resource *res)
{
	int wake_irq;
	int ret;

	dev_dbg(&pdev->dev, "ci13xxx_msm_install_wake_gpio\n");

	_udc_ctxt.wake_gpio = res->start;
	gpio_request(_udc_ctxt.wake_gpio, "USB_RESUME");
	gpio_direction_input(_udc_ctxt.wake_gpio);
	wake_irq = gpio_to_irq(_udc_ctxt.wake_gpio);
	if (wake_irq < 0) {
		dev_err(&pdev->dev, "could not register USB_RESUME GPIO.\n");
		return -ENXIO;
	}

	dev_dbg(&pdev->dev, "_udc_ctxt.gpio_irq = %d and irq = %d\n",
			_udc_ctxt.wake_gpio, wake_irq);
	ret = request_irq(wake_irq, ci13xxx_msm_resume_irq,
		IRQF_TRIGGER_RISING | IRQF_ONESHOT, "usb resume", NULL);
	if (ret < 0) {
		dev_err(&pdev->dev, "could not register USB_RESUME IRQ.\n");
		goto gpio_free;
	}
	disable_irq(wake_irq);
	_udc_ctxt.wake_irq = wake_irq;

	return 0;

gpio_free:
	gpio_free(_udc_ctxt.wake_gpio);
	_udc_ctxt.wake_gpio = 0;
	return ret;
}

static void ci13xxx_msm_uninstall_wake_gpio(struct platform_device *pdev)
{
	dev_dbg(&pdev->dev, "ci13xxx_msm_uninstall_wake_gpio\n");

	if (_udc_ctxt.wake_gpio) {
		gpio_free(_udc_ctxt.wake_gpio);
		_udc_ctxt.wake_gpio = 0;
	}
}

static int ci13xxx_msm_probe(struct platform_device *pdev)
{
	struct resource *res;
=======
static int ci13xxx_msm_probe(struct platform_device *pdev)
{
	struct resource *res;
	void __iomem *regs;
	int irq;
>>>>>>> remotes/linux2/linux-3.4.y
	int ret;

	dev_dbg(&pdev->dev, "ci13xxx_msm_probe\n");

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "failed to get platform resource mem\n");
		return -ENXIO;
	}

<<<<<<< HEAD
	_udc_ctxt.regs = ioremap(res->start, resource_size(res));
	if (!_udc_ctxt.regs) {
=======
	regs = ioremap(res->start, resource_size(res));
	if (!regs) {
>>>>>>> remotes/linux2/linux-3.4.y
		dev_err(&pdev->dev, "ioremap failed\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	ret = udc_probe(&ci13xxx_msm_udc_driver, &pdev->dev, _udc_ctxt.regs);
=======
	ret = udc_probe(&ci13xxx_msm_udc_driver, &pdev->dev, regs);
>>>>>>> remotes/linux2/linux-3.4.y
	if (ret < 0) {
		dev_err(&pdev->dev, "udc_probe failed\n");
		goto iounmap;
	}

<<<<<<< HEAD
	_udc_ctxt.irq = platform_get_irq(pdev, 0);
	if (_udc_ctxt.irq < 0) {
=======
	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
>>>>>>> remotes/linux2/linux-3.4.y
		dev_err(&pdev->dev, "IRQ not found\n");
		ret = -ENXIO;
		goto udc_remove;
	}

<<<<<<< HEAD
	res = platform_get_resource_byname(pdev, IORESOURCE_IO, "USB_RESUME");
	if (res) {
		ret = ci13xxx_msm_install_wake_gpio(pdev, res);
		if (ret < 0) {
			dev_err(&pdev->dev, "gpio irq install failed\n");
			goto udc_remove;
		}
	}

	ret = request_irq(_udc_ctxt.irq, msm_udc_irq, IRQF_SHARED, pdev->name,
					  pdev);
	if (ret < 0) {
		dev_err(&pdev->dev, "request_irq failed\n");
		goto gpio_uninstall;
=======
	ret = request_irq(irq, msm_udc_irq, IRQF_SHARED, pdev->name, pdev);
	if (ret < 0) {
		dev_err(&pdev->dev, "request_irq failed\n");
		goto udc_remove;
>>>>>>> remotes/linux2/linux-3.4.y
	}

	pm_runtime_no_callbacks(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

	return 0;

<<<<<<< HEAD
gpio_uninstall:
	ci13xxx_msm_uninstall_wake_gpio(pdev);
udc_remove:
	udc_remove();
iounmap:
	iounmap(_udc_ctxt.regs);
=======
udc_remove:
	udc_remove();
iounmap:
	iounmap(regs);
>>>>>>> remotes/linux2/linux-3.4.y

	return ret;
}

<<<<<<< HEAD
int ci13xxx_msm_remove(struct platform_device *pdev)
{
	pm_runtime_disable(&pdev->dev);
	free_irq(_udc_ctxt.irq, pdev);
	ci13xxx_msm_uninstall_wake_gpio(pdev);
	udc_remove();
	iounmap(_udc_ctxt.regs);
	return 0;
}

static struct platform_driver ci13xxx_msm_driver = {
	.probe = ci13xxx_msm_probe,
	.driver = { .name = "msm_hsusb", },
	.remove = ci13xxx_msm_remove,
=======
static struct platform_driver ci13xxx_msm_driver = {
	.probe = ci13xxx_msm_probe,
	.driver = { .name = "msm_hsusb", },
>>>>>>> remotes/linux2/linux-3.4.y
};
MODULE_ALIAS("platform:msm_hsusb");

static int __init ci13xxx_msm_init(void)
{
	return platform_driver_register(&ci13xxx_msm_driver);
}
module_init(ci13xxx_msm_init);

<<<<<<< HEAD
static void __exit ci13xxx_msm_exit(void)
{
	platform_driver_unregister(&ci13xxx_msm_driver);
}
module_exit(ci13xxx_msm_exit);

=======
>>>>>>> remotes/linux2/linux-3.4.y
MODULE_LICENSE("GPL v2");
