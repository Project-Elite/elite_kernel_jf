#ifndef __LINUX_PWM_H
#define __LINUX_PWM_H

struct pwm_device;

<<<<<<< HEAD
/* Add __weak functions to support PWM */

/*
 * pwm_request - request a PWM device
 */
struct pwm_device __weak *pwm_request(int pwm_id, const char *label);
=======
/*
 * pwm_request - request a PWM device
 */
struct pwm_device *pwm_request(int pwm_id, const char *label);
>>>>>>> remotes/linux2/linux-3.4.y

/*
 * pwm_free - free a PWM device
 */
<<<<<<< HEAD
void __weak pwm_free(struct pwm_device *pwm);
=======
void pwm_free(struct pwm_device *pwm);
>>>>>>> remotes/linux2/linux-3.4.y

/*
 * pwm_config - change a PWM device configuration
 */
<<<<<<< HEAD
int __weak pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns);
=======
int pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns);
>>>>>>> remotes/linux2/linux-3.4.y

/*
 * pwm_enable - start a PWM output toggling
 */
<<<<<<< HEAD
int __weak pwm_enable(struct pwm_device *pwm);
=======
int pwm_enable(struct pwm_device *pwm);
>>>>>>> remotes/linux2/linux-3.4.y

/*
 * pwm_disable - stop a PWM output toggling
 */
<<<<<<< HEAD
void __weak pwm_disable(struct pwm_device *pwm);
=======
void pwm_disable(struct pwm_device *pwm);
>>>>>>> remotes/linux2/linux-3.4.y

#endif /* __LINUX_PWM_H */
