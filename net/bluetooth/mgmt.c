/*
   BlueZ - Bluetooth protocol stack for Linux
<<<<<<< HEAD
   Copyright (C) 2010  Nokia Corporation
   Copyright (c) 2011-2012 The Linux Foundation.  All rights reserved.
=======

   Copyright (C) 2010  Nokia Corporation
   Copyright (C) 2011-2012 Intel Corporation
>>>>>>> remotes/linux2/linux-3.4.y

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as
   published by the Free Software Foundation;

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.
   IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) AND AUTHOR(S) BE LIABLE FOR ANY
   CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

   ALL LIABILITY, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PATENTS,
   COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS, RELATING TO USE OF THIS
   SOFTWARE IS DISCLAIMED.
*/

/* Bluetooth HCI Management interface */

<<<<<<< HEAD
#include <linux/uaccess.h>
#include <linux/interrupt.h>
=======
#include <linux/kernel.h>
#include <linux/uaccess.h>
>>>>>>> remotes/linux2/linux-3.4.y
#include <linux/module.h>
#include <asm/unaligned.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
<<<<<<< HEAD
#include <net/bluetooth/l2cap.h>
#include <net/bluetooth/mgmt.h>
#include <net/bluetooth/smp.h>

#define MGMT_VERSION	0
#define MGMT_REVISION	1

#define SCAN_IDLE	0x00
#define SCAN_LE		0x01
#define SCAN_BR		0x02

struct pending_cmd {
	struct list_head list;
	__u16 opcode;
=======
#include <net/bluetooth/mgmt.h>
#include <net/bluetooth/smp.h>

bool enable_hs;
bool enable_le;

#define MGMT_VERSION	1
#define MGMT_REVISION	0

static const u16 mgmt_commands[] = {
	MGMT_OP_READ_INDEX_LIST,
	MGMT_OP_READ_INFO,
	MGMT_OP_SET_POWERED,
	MGMT_OP_SET_DISCOVERABLE,
	MGMT_OP_SET_CONNECTABLE,
	MGMT_OP_SET_FAST_CONNECTABLE,
	MGMT_OP_SET_PAIRABLE,
	MGMT_OP_SET_LINK_SECURITY,
	MGMT_OP_SET_SSP,
	MGMT_OP_SET_HS,
	MGMT_OP_SET_LE,
	MGMT_OP_SET_DEV_CLASS,
	MGMT_OP_SET_LOCAL_NAME,
	MGMT_OP_ADD_UUID,
	MGMT_OP_REMOVE_UUID,
	MGMT_OP_LOAD_LINK_KEYS,
	MGMT_OP_LOAD_LONG_TERM_KEYS,
	MGMT_OP_DISCONNECT,
	MGMT_OP_GET_CONNECTIONS,
	MGMT_OP_PIN_CODE_REPLY,
	MGMT_OP_PIN_CODE_NEG_REPLY,
	MGMT_OP_SET_IO_CAPABILITY,
	MGMT_OP_PAIR_DEVICE,
	MGMT_OP_CANCEL_PAIR_DEVICE,
	MGMT_OP_UNPAIR_DEVICE,
	MGMT_OP_USER_CONFIRM_REPLY,
	MGMT_OP_USER_CONFIRM_NEG_REPLY,
	MGMT_OP_USER_PASSKEY_REPLY,
	MGMT_OP_USER_PASSKEY_NEG_REPLY,
	MGMT_OP_READ_LOCAL_OOB_DATA,
	MGMT_OP_ADD_REMOTE_OOB_DATA,
	MGMT_OP_REMOVE_REMOTE_OOB_DATA,
	MGMT_OP_START_DISCOVERY,
	MGMT_OP_STOP_DISCOVERY,
	MGMT_OP_CONFIRM_NAME,
	MGMT_OP_BLOCK_DEVICE,
	MGMT_OP_UNBLOCK_DEVICE,
};

static const u16 mgmt_events[] = {
	MGMT_EV_CONTROLLER_ERROR,
	MGMT_EV_INDEX_ADDED,
	MGMT_EV_INDEX_REMOVED,
	MGMT_EV_NEW_SETTINGS,
	MGMT_EV_CLASS_OF_DEV_CHANGED,
	MGMT_EV_LOCAL_NAME_CHANGED,
	MGMT_EV_NEW_LINK_KEY,
	MGMT_EV_NEW_LONG_TERM_KEY,
	MGMT_EV_DEVICE_CONNECTED,
	MGMT_EV_DEVICE_DISCONNECTED,
	MGMT_EV_CONNECT_FAILED,
	MGMT_EV_PIN_CODE_REQUEST,
	MGMT_EV_USER_CONFIRM_REQUEST,
	MGMT_EV_USER_PASSKEY_REQUEST,
	MGMT_EV_AUTH_FAILED,
	MGMT_EV_DEVICE_FOUND,
	MGMT_EV_DISCOVERING,
	MGMT_EV_DEVICE_BLOCKED,
	MGMT_EV_DEVICE_UNBLOCKED,
	MGMT_EV_DEVICE_UNPAIRED,
};

/*
 * These LE scan and inquiry parameters were chosen according to LE General
 * Discovery Procedure specification.
 */
#define LE_SCAN_TYPE			0x01
#define LE_SCAN_WIN			0x12
#define LE_SCAN_INT			0x12
#define LE_SCAN_TIMEOUT_LE_ONLY		10240	/* TGAP(gen_disc_scan_min) */
#define LE_SCAN_TIMEOUT_BREDR_LE	5120	/* TGAP(100)/2 */

#define INQUIRY_LEN_BREDR		0x08	/* TGAP(100) */
#define INQUIRY_LEN_BREDR_LE		0x04	/* TGAP(100)/2 */

#define CACHE_TIMEOUT	msecs_to_jiffies(2 * 1000)

#define hdev_is_powered(hdev) (test_bit(HCI_UP, &hdev->flags) && \
				!test_bit(HCI_AUTO_OFF, &hdev->dev_flags))

struct pending_cmd {
	struct list_head list;
	u16 opcode;
>>>>>>> remotes/linux2/linux-3.4.y
	int index;
	void *param;
	struct sock *sk;
	void *user_data;
};

<<<<<<< HEAD
struct mgmt_pending_free_work {
	struct work_struct work;
	struct sock *sk;
};

LIST_HEAD(cmd_list);
=======
/* HCI to MGMT error code conversion table */
static u8 mgmt_status_table[] = {
	MGMT_STATUS_SUCCESS,
	MGMT_STATUS_UNKNOWN_COMMAND,	/* Unknown Command */
	MGMT_STATUS_NOT_CONNECTED,	/* No Connection */
	MGMT_STATUS_FAILED,		/* Hardware Failure */
	MGMT_STATUS_CONNECT_FAILED,	/* Page Timeout */
	MGMT_STATUS_AUTH_FAILED,	/* Authentication Failed */
	MGMT_STATUS_NOT_PAIRED,		/* PIN or Key Missing */
	MGMT_STATUS_NO_RESOURCES,	/* Memory Full */
	MGMT_STATUS_TIMEOUT,		/* Connection Timeout */
	MGMT_STATUS_NO_RESOURCES,	/* Max Number of Connections */
	MGMT_STATUS_NO_RESOURCES,	/* Max Number of SCO Connections */
	MGMT_STATUS_ALREADY_CONNECTED,	/* ACL Connection Exists */
	MGMT_STATUS_BUSY,		/* Command Disallowed */
	MGMT_STATUS_NO_RESOURCES,	/* Rejected Limited Resources */
	MGMT_STATUS_REJECTED,		/* Rejected Security */
	MGMT_STATUS_REJECTED,		/* Rejected Personal */
	MGMT_STATUS_TIMEOUT,		/* Host Timeout */
	MGMT_STATUS_NOT_SUPPORTED,	/* Unsupported Feature */
	MGMT_STATUS_INVALID_PARAMS,	/* Invalid Parameters */
	MGMT_STATUS_DISCONNECTED,	/* OE User Ended Connection */
	MGMT_STATUS_NO_RESOURCES,	/* OE Low Resources */
	MGMT_STATUS_DISCONNECTED,	/* OE Power Off */
	MGMT_STATUS_DISCONNECTED,	/* Connection Terminated */
	MGMT_STATUS_BUSY,		/* Repeated Attempts */
	MGMT_STATUS_REJECTED,		/* Pairing Not Allowed */
	MGMT_STATUS_FAILED,		/* Unknown LMP PDU */
	MGMT_STATUS_NOT_SUPPORTED,	/* Unsupported Remote Feature */
	MGMT_STATUS_REJECTED,		/* SCO Offset Rejected */
	MGMT_STATUS_REJECTED,		/* SCO Interval Rejected */
	MGMT_STATUS_REJECTED,		/* Air Mode Rejected */
	MGMT_STATUS_INVALID_PARAMS,	/* Invalid LMP Parameters */
	MGMT_STATUS_FAILED,		/* Unspecified Error */
	MGMT_STATUS_NOT_SUPPORTED,	/* Unsupported LMP Parameter Value */
	MGMT_STATUS_FAILED,		/* Role Change Not Allowed */
	MGMT_STATUS_TIMEOUT,		/* LMP Response Timeout */
	MGMT_STATUS_FAILED,		/* LMP Error Transaction Collision */
	MGMT_STATUS_FAILED,		/* LMP PDU Not Allowed */
	MGMT_STATUS_REJECTED,		/* Encryption Mode Not Accepted */
	MGMT_STATUS_FAILED,		/* Unit Link Key Used */
	MGMT_STATUS_NOT_SUPPORTED,	/* QoS Not Supported */
	MGMT_STATUS_TIMEOUT,		/* Instant Passed */
	MGMT_STATUS_NOT_SUPPORTED,	/* Pairing Not Supported */
	MGMT_STATUS_FAILED,		/* Transaction Collision */
	MGMT_STATUS_INVALID_PARAMS,	/* Unacceptable Parameter */
	MGMT_STATUS_REJECTED,		/* QoS Rejected */
	MGMT_STATUS_NOT_SUPPORTED,	/* Classification Not Supported */
	MGMT_STATUS_REJECTED,		/* Insufficient Security */
	MGMT_STATUS_INVALID_PARAMS,	/* Parameter Out Of Range */
	MGMT_STATUS_BUSY,		/* Role Switch Pending */
	MGMT_STATUS_FAILED,		/* Slot Violation */
	MGMT_STATUS_FAILED,		/* Role Switch Failed */
	MGMT_STATUS_INVALID_PARAMS,	/* EIR Too Large */
	MGMT_STATUS_NOT_SUPPORTED,	/* Simple Pairing Not Supported */
	MGMT_STATUS_BUSY,		/* Host Busy Pairing */
	MGMT_STATUS_REJECTED,		/* Rejected, No Suitable Channel */
	MGMT_STATUS_BUSY,		/* Controller Busy */
	MGMT_STATUS_INVALID_PARAMS,	/* Unsuitable Connection Interval */
	MGMT_STATUS_TIMEOUT,		/* Directed Advertising Timeout */
	MGMT_STATUS_AUTH_FAILED,	/* Terminated Due to MIC Failure */
	MGMT_STATUS_CONNECT_FAILED,	/* Connection Establishment Failed */
	MGMT_STATUS_CONNECT_FAILED,	/* MAC Connection Failed */
};

static u8 mgmt_status(u8 hci_status)
{
	if (hci_status < ARRAY_SIZE(mgmt_status_table))
		return mgmt_status_table[hci_status];

	return MGMT_STATUS_FAILED;
}
>>>>>>> remotes/linux2/linux-3.4.y

static int cmd_status(struct sock *sk, u16 index, u16 cmd, u8 status)
{
	struct sk_buff *skb;
	struct mgmt_hdr *hdr;
	struct mgmt_ev_cmd_status *ev;
<<<<<<< HEAD
=======
	int err;
>>>>>>> remotes/linux2/linux-3.4.y

	BT_DBG("sock %p, index %u, cmd %u, status %u", sk, index, cmd, status);

	skb = alloc_skb(sizeof(*hdr) + sizeof(*ev), GFP_ATOMIC);
	if (!skb)
		return -ENOMEM;

	hdr = (void *) skb_put(skb, sizeof(*hdr));

	hdr->opcode = cpu_to_le16(MGMT_EV_CMD_STATUS);
	hdr->index = cpu_to_le16(index);
	hdr->len = cpu_to_le16(sizeof(*ev));

	ev = (void *) skb_put(skb, sizeof(*ev));
	ev->status = status;
	put_unaligned_le16(cmd, &ev->opcode);

<<<<<<< HEAD
	if (sock_queue_rcv_skb(sk, skb) < 0)
		kfree_skb(skb);

	return 0;
}

static int cmd_complete(struct sock *sk, u16 index, u16 cmd, void *rp,
								size_t rp_len)
=======
	err = sock_queue_rcv_skb(sk, skb);
	if (err < 0)
		kfree_skb(skb);

	return err;
}

static int cmd_complete(struct sock *sk, u16 index, u16 cmd, u8 status,
			void *rp, size_t rp_len)
>>>>>>> remotes/linux2/linux-3.4.y
{
	struct sk_buff *skb;
	struct mgmt_hdr *hdr;
	struct mgmt_ev_cmd_complete *ev;
<<<<<<< HEAD
=======
	int err;
>>>>>>> remotes/linux2/linux-3.4.y

	BT_DBG("sock %p", sk);

	skb = alloc_skb(sizeof(*hdr) + sizeof(*ev) + rp_len, GFP_ATOMIC);
	if (!skb)
		return -ENOMEM;

	hdr = (void *) skb_put(skb, sizeof(*hdr));

	hdr->opcode = cpu_to_le16(MGMT_EV_CMD_COMPLETE);
	hdr->index = cpu_to_le16(index);
	hdr->len = cpu_to_le16(sizeof(*ev) + rp_len);

	ev = (void *) skb_put(skb, sizeof(*ev) + rp_len);
	put_unaligned_le16(cmd, &ev->opcode);
<<<<<<< HEAD
=======
	ev->status = status;
>>>>>>> remotes/linux2/linux-3.4.y

	if (rp)
		memcpy(ev->data, rp, rp_len);

<<<<<<< HEAD
	if (sock_queue_rcv_skb(sk, skb) < 0)
		kfree_skb(skb);

	return 0;
}

static int read_version(struct sock *sk)
=======
	err = sock_queue_rcv_skb(sk, skb);
	if (err < 0)
		kfree_skb(skb);

	return err;
}

static int read_version(struct sock *sk, struct hci_dev *hdev, void *data,
			u16 data_len)
>>>>>>> remotes/linux2/linux-3.4.y
{
	struct mgmt_rp_read_version rp;

	BT_DBG("sock %p", sk);

	rp.version = MGMT_VERSION;
	put_unaligned_le16(MGMT_REVISION, &rp.revision);

<<<<<<< HEAD
	return cmd_complete(sk, MGMT_INDEX_NONE, MGMT_OP_READ_VERSION, &rp,
								sizeof(rp));
}

static int read_index_list(struct sock *sk)
{
	struct mgmt_rp_read_index_list *rp;
	struct list_head *p;
=======
	return cmd_complete(sk, MGMT_INDEX_NONE, MGMT_OP_READ_VERSION, 0, &rp,
			    sizeof(rp));
}

static int read_commands(struct sock *sk, struct hci_dev *hdev, void *data,
			 u16 data_len)
{
	struct mgmt_rp_read_commands *rp;
	u16 num_commands = ARRAY_SIZE(mgmt_commands);
	u16 num_events = ARRAY_SIZE(mgmt_events);
	u16 *opcode;
	size_t rp_size;
	int i, err;

	BT_DBG("sock %p", sk);

	rp_size = sizeof(*rp) + ((num_commands + num_events) * sizeof(u16));

	rp = kmalloc(rp_size, GFP_KERNEL);
	if (!rp)
		return -ENOMEM;

	put_unaligned_le16(num_commands, &rp->num_commands);
	put_unaligned_le16(num_events, &rp->num_events);

	for (i = 0, opcode = rp->opcodes; i < num_commands; i++, opcode++)
		put_unaligned_le16(mgmt_commands[i], opcode);

	for (i = 0; i < num_events; i++, opcode++)
		put_unaligned_le16(mgmt_events[i], opcode);

	err = cmd_complete(sk, MGMT_INDEX_NONE, MGMT_OP_READ_COMMANDS, 0, rp,
			   rp_size);
	kfree(rp);

	return err;
}

static int read_index_list(struct sock *sk, struct hci_dev *hdev, void *data,
			   u16 data_len)
{
	struct mgmt_rp_read_index_list *rp;
	struct list_head *p;
	struct hci_dev *d;
>>>>>>> remotes/linux2/linux-3.4.y
	size_t rp_len;
	u16 count;
	int i, err;

	BT_DBG("sock %p", sk);

	read_lock(&hci_dev_list_lock);

	count = 0;
	list_for_each(p, &hci_dev_list) {
<<<<<<< HEAD
		struct hci_dev *d = list_entry(p, struct hci_dev, list);
		if (d->dev_type != HCI_BREDR)
			continue;
=======
>>>>>>> remotes/linux2/linux-3.4.y
		count++;
	}

	rp_len = sizeof(*rp) + (2 * count);
	rp = kmalloc(rp_len, GFP_ATOMIC);
	if (!rp) {
		read_unlock(&hci_dev_list_lock);
		return -ENOMEM;
	}

<<<<<<< HEAD
	put_unaligned_le16(0, &rp->num_controllers);

	i = 0;
	list_for_each(p, &hci_dev_list) {
		struct hci_dev *d = list_entry(p, struct hci_dev, list);

		hci_del_off_timer(d);

		if (d->dev_type != HCI_BREDR)
			continue;

		set_bit(HCI_MGMT, &d->flags);

		if (test_bit(HCI_SETUP, &d->flags))
			continue;

		put_unaligned_le16(d->id, &rp->index[i++]);
		put_unaligned_le16((u16)i, &rp->num_controllers);
=======
	put_unaligned_le16(count, &rp->num_controllers);

	i = 0;
	list_for_each_entry(d, &hci_dev_list, list) {
		if (test_bit(HCI_SETUP, &d->dev_flags))
			continue;

		put_unaligned_le16(d->id, &rp->index[i++]);
>>>>>>> remotes/linux2/linux-3.4.y
		BT_DBG("Added hci%u", d->id);
	}

	read_unlock(&hci_dev_list_lock);

<<<<<<< HEAD
	err = cmd_complete(sk, MGMT_INDEX_NONE, MGMT_OP_READ_INDEX_LIST, rp,
									rp_len);
=======
	err = cmd_complete(sk, MGMT_INDEX_NONE, MGMT_OP_READ_INDEX_LIST, 0, rp,
			   rp_len);
>>>>>>> remotes/linux2/linux-3.4.y

	kfree(rp);

	return err;
}

<<<<<<< HEAD
static int read_controller_info(struct sock *sk, u16 index)
{
	struct mgmt_rp_read_info rp;
	struct hci_dev *hdev;

	BT_DBG("sock %p hci%u", sk, index);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_READ_INFO, ENODEV);

	hci_del_off_timer(hdev);

	hci_dev_lock_bh(hdev);

	set_bit(HCI_MGMT, &hdev->flags);

	memset(&rp, 0, sizeof(rp));

	rp.type = hdev->dev_type;

	rp.powered = test_bit(HCI_UP, &hdev->flags);
	rp.connectable = test_bit(HCI_PSCAN, &hdev->flags);
	rp.discoverable = test_bit(HCI_ISCAN, &hdev->flags);
	rp.pairable = test_bit(HCI_PSCAN, &hdev->flags);

	if (test_bit(HCI_AUTH, &hdev->flags))
		rp.sec_mode = 3;
	else if (hdev->ssp_mode > 0)
		rp.sec_mode = 4;
	else
		rp.sec_mode = 2;

	bacpy(&rp.bdaddr, &hdev->bdaddr);
	memcpy(rp.features, hdev->features, 8);
	memcpy(rp.dev_class, hdev->dev_class, 3);
	put_unaligned_le16(hdev->manufacturer, &rp.manufacturer);
	rp.hci_ver = hdev->hci_ver;
	put_unaligned_le16(hdev->hci_rev, &rp.hci_rev);

	memcpy(rp.name, hdev->dev_name, sizeof(hdev->dev_name));

	rp.le_white_list_size = hdev->le_white_list_size;

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return cmd_complete(sk, index, MGMT_OP_READ_INFO, &rp, sizeof(rp));
}

static void mgmt_pending_free_worker(struct work_struct *work)
{
	struct mgmt_pending_free_work *free_work =
		container_of(work, struct mgmt_pending_free_work, work);

	BT_DBG("sk %p", free_work->sk);

	sock_put(free_work->sk);
	kfree(free_work);
}

static void mgmt_pending_free(struct pending_cmd *cmd)
{
	struct mgmt_pending_free_work *free_work;
	struct sock *sk = cmd->sk;

	BT_DBG("opcode %d, sk %p", cmd->opcode, sk);

	kfree(cmd->param);
	kfree(cmd);

	free_work = kzalloc(sizeof(*free_work), GFP_ATOMIC);
	if (free_work) {
		INIT_WORK(&free_work->work, mgmt_pending_free_worker);
		free_work->sk = sk;

		if (!schedule_work(&free_work->work))
			kfree(free_work);
	}
}

static struct pending_cmd *mgmt_pending_add(struct sock *sk, u16 opcode,
						u16 index, void *data, u16 len)
{
	struct pending_cmd *cmd;

	BT_DBG("%d", opcode);

	cmd = kmalloc(sizeof(*cmd), GFP_ATOMIC);
	if (!cmd)
		return NULL;

	cmd->opcode = opcode;
	cmd->index = index;

	cmd->param = kmalloc(len, GFP_ATOMIC);
	if (!cmd->param) {
		kfree(cmd);
		return NULL;
	}

	if (data)
		memcpy(cmd->param, data, len);

	cmd->sk = sk;
	sock_hold(sk);

	list_add(&cmd->list, &cmd_list);

	return cmd;
}

static void mgmt_pending_foreach(u16 opcode, int index,
				void (*cb)(struct pending_cmd *cmd, void *data),
				void *data)
{
	struct list_head *p, *n;

	BT_DBG(" %d", opcode);

	list_for_each_safe(p, n, &cmd_list) {
		struct pending_cmd *cmd;

		cmd = list_entry(p, struct pending_cmd, list);

		if (opcode > 0 && cmd->opcode != opcode)
			continue;

		if (index >= 0 && cmd->index != index)
			continue;

		cb(cmd, data);
	}
}

static struct pending_cmd *mgmt_pending_find(u16 opcode, int index)
{
	struct list_head *p;

	BT_DBG(" %d", opcode);

	list_for_each(p, &cmd_list) {
		struct pending_cmd *cmd;

		cmd = list_entry(p, struct pending_cmd, list);

		if (cmd->opcode != opcode)
			continue;

		if (index >= 0 && cmd->index != index)
			continue;

		return cmd;
	}

	return NULL;
}

static void mgmt_pending_remove(struct pending_cmd *cmd)
{
	BT_DBG(" %d", cmd->opcode);

	list_del(&cmd->list);
	mgmt_pending_free(cmd);
}

static int set_powered(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct mgmt_mode *cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	int err, up;

	cp = (void *) data;

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_POWERED, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_POWERED, ENODEV);

	hci_dev_lock_bh(hdev);

	up = test_bit(HCI_UP, &hdev->flags);
	if ((cp->val && up) || (!cp->val && !up)) {
		err = cmd_status(sk, index, MGMT_OP_SET_POWERED, EALREADY);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_POWERED, index)) {
		err = cmd_status(sk, index, MGMT_OP_SET_POWERED, EBUSY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_POWERED, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	if (cp->val)
		queue_work(hdev->workqueue, &hdev->power_on);
	else
		queue_work(hdev->workqueue, &hdev->power_off);

	err = 0;

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
	return err;
=======
static u32 get_supported_settings(struct hci_dev *hdev)
{
	u32 settings = 0;

	settings |= MGMT_SETTING_POWERED;
	settings |= MGMT_SETTING_CONNECTABLE;
	settings |= MGMT_SETTING_FAST_CONNECTABLE;
	settings |= MGMT_SETTING_DISCOVERABLE;
	settings |= MGMT_SETTING_PAIRABLE;

	if (hdev->features[6] & LMP_SIMPLE_PAIR)
		settings |= MGMT_SETTING_SSP;

	if (!(hdev->features[4] & LMP_NO_BREDR)) {
		settings |= MGMT_SETTING_BREDR;
		settings |= MGMT_SETTING_LINK_SECURITY;
	}

	if (enable_hs)
		settings |= MGMT_SETTING_HS;

	if (enable_le) {
		if (hdev->features[4] & LMP_LE)
			settings |= MGMT_SETTING_LE;
	}

	return settings;
}

static u32 get_current_settings(struct hci_dev *hdev)
{
	u32 settings = 0;

	if (hdev_is_powered(hdev))
		settings |= MGMT_SETTING_POWERED;

	if (test_bit(HCI_CONNECTABLE, &hdev->dev_flags))
		settings |= MGMT_SETTING_CONNECTABLE;

	if (test_bit(HCI_DISCOVERABLE, &hdev->dev_flags))
		settings |= MGMT_SETTING_DISCOVERABLE;

	if (test_bit(HCI_PAIRABLE, &hdev->dev_flags))
		settings |= MGMT_SETTING_PAIRABLE;

	if (!(hdev->features[4] & LMP_NO_BREDR))
		settings |= MGMT_SETTING_BREDR;

	if (test_bit(HCI_LE_ENABLED, &hdev->dev_flags))
		settings |= MGMT_SETTING_LE;

	if (test_bit(HCI_LINK_SECURITY, &hdev->dev_flags))
		settings |= MGMT_SETTING_LINK_SECURITY;

	if (test_bit(HCI_SSP_ENABLED, &hdev->dev_flags))
		settings |= MGMT_SETTING_SSP;

	if (test_bit(HCI_HS_ENABLED, &hdev->dev_flags))
		settings |= MGMT_SETTING_HS;

	return settings;
}

#define PNP_INFO_SVCLASS_ID		0x1200

static u8 bluetooth_base_uuid[] = {
			0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
			0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u16 get_uuid16(u8 *uuid128)
{
	u32 val;
	int i;

	for (i = 0; i < 12; i++) {
		if (bluetooth_base_uuid[i] != uuid128[i])
			return 0;
	}

	memcpy(&val, &uuid128[12], 4);

	val = le32_to_cpu(val);
	if (val > 0xffff)
		return 0;

	return (u16) val;
}

static void create_eir(struct hci_dev *hdev, u8 *data)
{
	u8 *ptr = data;
	u16 eir_len = 0;
	u16 uuid16_list[HCI_MAX_EIR_LENGTH / sizeof(u16)];
	int i, truncated = 0;
	struct bt_uuid *uuid;
	size_t name_len;

	name_len = strlen(hdev->dev_name);

	if (name_len > 0) {
		/* EIR Data type */
		if (name_len > 48) {
			name_len = 48;
			ptr[1] = EIR_NAME_SHORT;
		} else
			ptr[1] = EIR_NAME_COMPLETE;

		/* EIR Data length */
		ptr[0] = name_len + 1;

		memcpy(ptr + 2, hdev->dev_name, name_len);

		eir_len += (name_len + 2);
		ptr += (name_len + 2);
	}

	memset(uuid16_list, 0, sizeof(uuid16_list));

	/* Group all UUID16 types */
	list_for_each_entry(uuid, &hdev->uuids, list) {
		u16 uuid16;

		uuid16 = get_uuid16(uuid->uuid);
		if (uuid16 == 0)
			return;

		if (uuid16 < 0x1100)
			continue;

		if (uuid16 == PNP_INFO_SVCLASS_ID)
			continue;

		/* Stop if not enough space to put next UUID */
		if (eir_len + 2 + sizeof(u16) > HCI_MAX_EIR_LENGTH) {
			truncated = 1;
			break;
		}

		/* Check for duplicates */
		for (i = 0; uuid16_list[i] != 0; i++)
			if (uuid16_list[i] == uuid16)
				break;

		if (uuid16_list[i] == 0) {
			uuid16_list[i] = uuid16;
			eir_len += sizeof(u16);
		}
	}

	if (uuid16_list[0] != 0) {
		u8 *length = ptr;

		/* EIR Data type */
		ptr[1] = truncated ? EIR_UUID16_SOME : EIR_UUID16_ALL;

		ptr += 2;
		eir_len += 2;

		for (i = 0; uuid16_list[i] != 0; i++) {
			*ptr++ = (uuid16_list[i] & 0x00ff);
			*ptr++ = (uuid16_list[i] & 0xff00) >> 8;
		}

		/* EIR Data length */
		*length = (i * sizeof(u16)) + 1;
	}
}

static int update_eir(struct hci_dev *hdev)
{
	struct hci_cp_write_eir cp;

	if (!hdev_is_powered(hdev))
		return 0;

	if (!(hdev->features[6] & LMP_EXT_INQ))
		return 0;

	if (!test_bit(HCI_SSP_ENABLED, &hdev->dev_flags))
		return 0;

	if (test_bit(HCI_SERVICE_CACHE, &hdev->dev_flags))
		return 0;

	memset(&cp, 0, sizeof(cp));

	create_eir(hdev, cp.data);

	if (memcmp(cp.data, hdev->eir, sizeof(cp.data)) == 0)
		return 0;

	memcpy(hdev->eir, cp.data, sizeof(cp.data));

	return hci_send_cmd(hdev, HCI_OP_WRITE_EIR, sizeof(cp), &cp);
>>>>>>> remotes/linux2/linux-3.4.y
}

static u8 get_service_classes(struct hci_dev *hdev)
{
<<<<<<< HEAD
	struct list_head *p;
	u8 val = 0;

	list_for_each(p, &hdev->uuids) {
		struct bt_uuid *uuid = list_entry(p, struct bt_uuid, list);

		val |= uuid->svc_hint;
	}
=======
	struct bt_uuid *uuid;
	u8 val = 0;

	list_for_each_entry(uuid, &hdev->uuids, list)
		val |= uuid->svc_hint;
>>>>>>> remotes/linux2/linux-3.4.y

	return val;
}

static int update_class(struct hci_dev *hdev)
{
	u8 cod[3];
<<<<<<< HEAD
	int err = 0;

	BT_DBG("%s", hdev->name);

	if (test_bit(HCI_SERVICE_CACHE, &hdev->flags))
=======
	int err;

	BT_DBG("%s", hdev->name);

	if (!hdev_is_powered(hdev))
		return 0;

	if (test_bit(HCI_SERVICE_CACHE, &hdev->dev_flags))
>>>>>>> remotes/linux2/linux-3.4.y
		return 0;

	cod[0] = hdev->minor_class;
	cod[1] = hdev->major_class;
	cod[2] = get_service_classes(hdev);

	if (memcmp(cod, hdev->dev_class, 3) == 0)
		return 0;

<<<<<<< HEAD
	err =  hci_send_cmd(hdev, HCI_OP_WRITE_CLASS_OF_DEV, sizeof(cod), cod);

	if (err == 0)
		memcpy(hdev->dev_class, cod, 3);
=======
	err = hci_send_cmd(hdev, HCI_OP_WRITE_CLASS_OF_DEV, sizeof(cod), cod);
	if (err == 0)
		set_bit(HCI_PENDING_CLASS, &hdev->dev_flags);
>>>>>>> remotes/linux2/linux-3.4.y

	return err;
}

<<<<<<< HEAD
static int set_limited_discoverable(struct sock *sk, u16 index,
						unsigned char *data, u16 len)
{
	struct mgmt_mode *cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	struct hci_cp_write_current_iac_lap dcp;
	int update_cod;
	int err = 0;
	/* General Inquiry LAP: 0x9E8B33, Limited Inquiry LAP: 0x9E8B00 */
	u8 lap[] = { 0x33, 0x8b, 0x9e, 0x00, 0x8b, 0x9e };

	cp = (void *) data;

	BT_DBG("hci%u discoverable: %d", index, cp->val);

	if (!cp || len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
								ENETDOWN);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_LIMIT_DISCOVERABLE, index)) {
		err = cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
									EBUSY);
		goto failed;
	}

	if (cp->val == test_bit(HCI_ISCAN, &hdev->flags) &&
					test_bit(HCI_PSCAN, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_LIMIT_DISCOVERABLE,
								EALREADY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_LIMIT_DISCOVERABLE, index, data,
									len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	memset(&dcp, 0, sizeof(dcp));
	dcp.num_current_iac = cp->val ? 2 : 1;
	memcpy(&dcp.lap, lap, dcp.num_current_iac * 3);
	update_cod = 1;

	if (cp->val) {
		if (hdev->major_class & MGMT_MAJOR_CLASS_LIMITED)
			update_cod = 0;
		hdev->major_class |= MGMT_MAJOR_CLASS_LIMITED;
	} else {
		if (!(hdev->major_class & MGMT_MAJOR_CLASS_LIMITED))
			update_cod = 0;
		hdev->major_class &= ~MGMT_MAJOR_CLASS_LIMITED;
	}

	if (update_cod)
		err = update_class(hdev);

	if (err >= 0)
		err = hci_send_cmd(hdev, HCI_OP_WRITE_CURRENT_IAC_LAP,
							sizeof(dcp), &dcp);

	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_discoverable(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct mgmt_mode *cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	u8 scan;
	int err;

	cp = (void *) data;

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, ENETDOWN);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_DISCOVERABLE, index) ||
			mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, index)) {
		err = cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, EBUSY);
		goto failed;
	}

	if (cp->val == test_bit(HCI_ISCAN, &hdev->flags) &&
					test_bit(HCI_PSCAN, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_DISCOVERABLE, EALREADY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_DISCOVERABLE, index, data, len);
=======
static void service_cache_off(struct work_struct *work)
{
	struct hci_dev *hdev = container_of(work, struct hci_dev,
					    service_cache.work);

	if (!test_and_clear_bit(HCI_SERVICE_CACHE, &hdev->dev_flags))
		return;

	hci_dev_lock(hdev);

	update_eir(hdev);
	update_class(hdev);

	hci_dev_unlock(hdev);
}

static void mgmt_init_hdev(struct sock *sk, struct hci_dev *hdev)
{
	if (test_and_set_bit(HCI_MGMT, &hdev->dev_flags))
		return;

	INIT_DELAYED_WORK(&hdev->service_cache, service_cache_off);

	/* Non-mgmt controlled devices get this bit set
	 * implicitly so that pairing works for them, however
	 * for mgmt we require user-space to explicitly enable
	 * it
	 */
	clear_bit(HCI_PAIRABLE, &hdev->dev_flags);
}

static int read_controller_info(struct sock *sk, struct hci_dev *hdev,
				void *data, u16 data_len)
{
	struct mgmt_rp_read_info rp;

	BT_DBG("sock %p %s", sk, hdev->name);

	hci_dev_lock(hdev);

	memset(&rp, 0, sizeof(rp));

	bacpy(&rp.bdaddr, &hdev->bdaddr);

	rp.version = hdev->hci_ver;

	put_unaligned_le16(hdev->manufacturer, &rp.manufacturer);

	rp.supported_settings = cpu_to_le32(get_supported_settings(hdev));
	rp.current_settings = cpu_to_le32(get_current_settings(hdev));

	memcpy(rp.dev_class, hdev->dev_class, 3);

	memcpy(rp.name, hdev->dev_name, sizeof(hdev->dev_name));
	memcpy(rp.short_name, hdev->short_name, sizeof(hdev->short_name));

	hci_dev_unlock(hdev);

	return cmd_complete(sk, hdev->id, MGMT_OP_READ_INFO, 0, &rp,
			    sizeof(rp));
}

static void mgmt_pending_free(struct pending_cmd *cmd)
{
	sock_put(cmd->sk);
	kfree(cmd->param);
	kfree(cmd);
}

static struct pending_cmd *mgmt_pending_add(struct sock *sk, u16 opcode,
					    struct hci_dev *hdev, void *data,
					    u16 len)
{
	struct pending_cmd *cmd;

	cmd = kmalloc(sizeof(*cmd), GFP_ATOMIC);
	if (!cmd)
		return NULL;

	cmd->opcode = opcode;
	cmd->index = hdev->id;

	cmd->param = kmalloc(len, GFP_ATOMIC);
	if (!cmd->param) {
		kfree(cmd);
		return NULL;
	}

	if (data)
		memcpy(cmd->param, data, len);

	cmd->sk = sk;
	sock_hold(sk);

	list_add(&cmd->list, &hdev->mgmt_pending);

	return cmd;
}

static void mgmt_pending_foreach(u16 opcode, struct hci_dev *hdev,
				 void (*cb)(struct pending_cmd *cmd, void *data),
				 void *data)
{
	struct list_head *p, *n;

	list_for_each_safe(p, n, &hdev->mgmt_pending) {
		struct pending_cmd *cmd;

		cmd = list_entry(p, struct pending_cmd, list);

		if (opcode > 0 && cmd->opcode != opcode)
			continue;

		cb(cmd, data);
	}
}

static struct pending_cmd *mgmt_pending_find(u16 opcode, struct hci_dev *hdev)
{
	struct pending_cmd *cmd;

	list_for_each_entry(cmd, &hdev->mgmt_pending, list) {
		if (cmd->opcode == opcode)
			return cmd;
	}

	return NULL;
}

static void mgmt_pending_remove(struct pending_cmd *cmd)
{
	list_del(&cmd->list);
	mgmt_pending_free(cmd);
}

static int send_settings_rsp(struct sock *sk, u16 opcode, struct hci_dev *hdev)
{
	__le32 settings = cpu_to_le32(get_current_settings(hdev));

	return cmd_complete(sk, hdev->id, opcode, 0, &settings,
			    sizeof(settings));
}

static int set_powered(struct sock *sk, struct hci_dev *hdev, void *data,
		       u16 len)
{
	struct mgmt_mode *cp = data;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("request for %s", hdev->name);

	hci_dev_lock(hdev);

	if (test_and_clear_bit(HCI_AUTO_OFF, &hdev->dev_flags)) {
		cancel_delayed_work(&hdev->power_off);

		if (cp->val) {
			err = send_settings_rsp(sk, MGMT_OP_SET_POWERED, hdev);
			mgmt_powered(hdev, 1);
			goto failed;
		}
	}

	if (!!cp->val == hdev_is_powered(hdev)) {
		err = send_settings_rsp(sk, MGMT_OP_SET_POWERED, hdev);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_POWERED, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_POWERED,
				 MGMT_STATUS_BUSY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_POWERED, hdev, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	if (cp->val)
		schedule_work(&hdev->power_on);
	else
		schedule_work(&hdev->power_off.work);

	err = 0;

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int mgmt_event(u16 event, struct hci_dev *hdev, void *data, u16 data_len,
		      struct sock *skip_sk)
{
	struct sk_buff *skb;
	struct mgmt_hdr *hdr;

	skb = alloc_skb(sizeof(*hdr) + data_len, GFP_ATOMIC);
	if (!skb)
		return -ENOMEM;

	hdr = (void *) skb_put(skb, sizeof(*hdr));
	hdr->opcode = cpu_to_le16(event);
	if (hdev)
		hdr->index = cpu_to_le16(hdev->id);
	else
		hdr->index = cpu_to_le16(MGMT_INDEX_NONE);
	hdr->len = cpu_to_le16(data_len);

	if (data)
		memcpy(skb_put(skb, data_len), data, data_len);

	/* Time stamp */
	__net_timestamp(skb);

	hci_send_to_control(skb, skip_sk);
	kfree_skb(skb);

	return 0;
}

static int new_settings(struct hci_dev *hdev, struct sock *skip)
{
	__le32 ev;

	ev = cpu_to_le32(get_current_settings(hdev));

	return mgmt_event(MGMT_EV_NEW_SETTINGS, hdev, &ev, sizeof(ev), skip);
}

static int set_discoverable(struct sock *sk, struct hci_dev *hdev, void *data,
			    u16 len)
{
	struct mgmt_cp_set_discoverable *cp = data;
	struct pending_cmd *cmd;
	u16 timeout;
	u8 scan;
	int err;

	BT_DBG("request for %s", hdev->name);

	timeout = get_unaligned_le16(&cp->timeout);
	if (!cp->val && timeout > 0)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_DISCOVERABLE,
				  MGMT_STATUS_INVALID_PARAMS);

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev) && timeout > 0) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_DISCOVERABLE,
				 MGMT_STATUS_NOT_POWERED);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_DISCOVERABLE, hdev) ||
			mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_DISCOVERABLE,
				 MGMT_STATUS_BUSY);
		goto failed;
	}

	if (!test_bit(HCI_CONNECTABLE, &hdev->dev_flags)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_DISCOVERABLE,
				 MGMT_STATUS_REJECTED);
		goto failed;
	}

	if (!hdev_is_powered(hdev)) {
		bool changed = false;

		if (!!cp->val != test_bit(HCI_DISCOVERABLE, &hdev->dev_flags)) {
			change_bit(HCI_DISCOVERABLE, &hdev->dev_flags);
			changed = true;
		}

		err = send_settings_rsp(sk, MGMT_OP_SET_DISCOVERABLE, hdev);
		if (err < 0)
			goto failed;

		if (changed)
			err = new_settings(hdev, sk);

		goto failed;
	}

	if (!!cp->val == test_bit(HCI_DISCOVERABLE, &hdev->dev_flags)) {
		if (hdev->discov_timeout > 0) {
			cancel_delayed_work(&hdev->discov_off);
			hdev->discov_timeout = 0;
		}

		if (cp->val && timeout > 0) {
			hdev->discov_timeout = timeout;
			queue_delayed_work(hdev->workqueue, &hdev->discov_off,
				msecs_to_jiffies(hdev->discov_timeout * 1000));
		}

		err = send_settings_rsp(sk, MGMT_OP_SET_DISCOVERABLE, hdev);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_DISCOVERABLE, hdev, data, len);
>>>>>>> remotes/linux2/linux-3.4.y
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	scan = SCAN_PAGE;

	if (cp->val)
		scan |= SCAN_INQUIRY;
<<<<<<< HEAD
=======
	else
		cancel_delayed_work(&hdev->discov_off);
>>>>>>> remotes/linux2/linux-3.4.y

	err = hci_send_cmd(hdev, HCI_OP_WRITE_SCAN_ENABLE, 1, &scan);
	if (err < 0)
		mgmt_pending_remove(cmd);

<<<<<<< HEAD
failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_connectable(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct mgmt_mode *cp;
	struct hci_dev *hdev;
=======
	if (cp->val)
		hdev->discov_timeout = timeout;

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int set_connectable(struct sock *sk, struct hci_dev *hdev, void *data,
			   u16 len)
{
	struct mgmt_mode *cp = data;
>>>>>>> remotes/linux2/linux-3.4.y
	struct pending_cmd *cmd;
	u8 scan;
	int err;

<<<<<<< HEAD
	cp = (void *) data;

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, ENETDOWN);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_DISCOVERABLE, index) ||
			mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, index)) {
		err = cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, EBUSY);
		goto failed;
	}

	if (cp->val == test_bit(HCI_PSCAN, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_SET_CONNECTABLE, EALREADY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_CONNECTABLE, index, data, len);
=======
	BT_DBG("request for %s", hdev->name);

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		bool changed = false;

		if (!!cp->val != test_bit(HCI_CONNECTABLE, &hdev->dev_flags))
			changed = true;

		if (cp->val) {
			set_bit(HCI_CONNECTABLE, &hdev->dev_flags);
		} else {
			clear_bit(HCI_CONNECTABLE, &hdev->dev_flags);
			clear_bit(HCI_DISCOVERABLE, &hdev->dev_flags);
		}

		err = send_settings_rsp(sk, MGMT_OP_SET_CONNECTABLE, hdev);
		if (err < 0)
			goto failed;

		if (changed)
			err = new_settings(hdev, sk);

		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_DISCOVERABLE, hdev) ||
			mgmt_pending_find(MGMT_OP_SET_CONNECTABLE, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_CONNECTABLE,
				 MGMT_STATUS_BUSY);
		goto failed;
	}

	if (!!cp->val == test_bit(HCI_PSCAN, &hdev->flags)) {
		err = send_settings_rsp(sk, MGMT_OP_SET_CONNECTABLE, hdev);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_CONNECTABLE, hdev, data, len);
>>>>>>> remotes/linux2/linux-3.4.y
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

<<<<<<< HEAD
	if (cp->val)
		scan = SCAN_PAGE;
	else
		scan = 0;

=======
	if (cp->val) {
		scan = SCAN_PAGE;
	} else {
		scan = 0;

		if (test_bit(HCI_ISCAN, &hdev->flags) &&
						hdev->discov_timeout > 0)
			cancel_delayed_work(&hdev->discov_off);
	}

>>>>>>> remotes/linux2/linux-3.4.y
	err = hci_send_cmd(hdev, HCI_OP_WRITE_SCAN_ENABLE, 1, &scan);
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
<<<<<<< HEAD
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int mgmt_event(u16 event, u16 index, void *data, u16 data_len,
							struct sock *skip_sk)
{
	struct sk_buff *skb;
	struct mgmt_hdr *hdr;

	BT_DBG("hci%d %d", index, event);

	skb = alloc_skb(sizeof(*hdr) + data_len, GFP_ATOMIC);
	if (!skb)
		return -ENOMEM;

	bt_cb(skb)->channel = HCI_CHANNEL_CONTROL;

	hdr = (void *) skb_put(skb, sizeof(*hdr));
	hdr->opcode = cpu_to_le16(event);
	hdr->index = cpu_to_le16(index);
	hdr->len = cpu_to_le16(data_len);

	if (data)
		memcpy(skb_put(skb, data_len), data, data_len);

	hci_send_to_sock(NULL, skb, skip_sk);
	kfree_skb(skb);

	return 0;
}

static int send_mode_rsp(struct sock *sk, u16 opcode, u16 index, u8 val)
{
	struct mgmt_mode rp;

	rp.val = val;

	return cmd_complete(sk, index, opcode, &rp, sizeof(rp));
}

static int set_pairable(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct mgmt_mode *cp, ev;
	struct hci_dev *hdev;
	int err;

	cp = (void *) data;

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_PAIRABLE, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_PAIRABLE, ENODEV);

	hci_dev_lock_bh(hdev);

	if (cp->val)
		set_bit(HCI_PAIRABLE, &hdev->flags);
	else
		clear_bit(HCI_PAIRABLE, &hdev->flags);

	err = send_mode_rsp(sk, MGMT_OP_SET_PAIRABLE, index, cp->val);
	if (err < 0)
		goto failed;

	ev.val = cp->val;

	err = mgmt_event(MGMT_EV_PAIRABLE, index, &ev, sizeof(ev), sk);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

#define EIR_FLAGS		0x01 /* flags */
#define EIR_UUID16_SOME		0x02 /* 16-bit UUID, more available */
#define EIR_UUID16_ALL		0x03 /* 16-bit UUID, all listed */
#define EIR_UUID32_SOME		0x04 /* 32-bit UUID, more available */
#define EIR_UUID32_ALL		0x05 /* 32-bit UUID, all listed */
#define EIR_UUID128_SOME	0x06 /* 128-bit UUID, more available */
#define EIR_UUID128_ALL		0x07 /* 128-bit UUID, all listed */
#define EIR_NAME_SHORT		0x08 /* shortened local name */
#define EIR_NAME_COMPLETE	0x09 /* complete local name */
#define EIR_TX_POWER		0x0A /* transmit power level */
#define EIR_DEVICE_ID		0x10 /* device ID */

#define PNP_INFO_SVCLASS_ID		0x1200

static u8 bluetooth_base_uuid[] = {
			0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
			0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u16 get_uuid16(u8 *uuid128)
{
	u32 val;
	int i;

	for (i = 0; i < 12; i++) {
		if (bluetooth_base_uuid[i] != uuid128[i])
			return 0;
	}

	memcpy(&val, &uuid128[12], 4);

	val = le32_to_cpu(val);
	if (val > 0xffff)
		return 0;

	return (u16) val;
}

static void create_eir(struct hci_dev *hdev, u8 *data)
{
	u8 *ptr = data;
	u16 eir_len = 0;
	u16 uuid16_list[HCI_MAX_EIR_LENGTH / sizeof(u16)];
	int i, truncated = 0;
	struct list_head *p;
	size_t name_len;

	name_len = strnlen(hdev->dev_name, HCI_MAX_EIR_LENGTH);

	if (name_len > 0) {
		/* EIR Data type */
		if (name_len > 48) {
			name_len = 48;
			ptr[1] = EIR_NAME_SHORT;
		} else
			ptr[1] = EIR_NAME_COMPLETE;

		/* EIR Data length */
		ptr[0] = name_len + 1;

		memcpy(ptr + 2, hdev->dev_name, name_len);

		eir_len += (name_len + 2);
		ptr += (name_len + 2);
	}

	memset(uuid16_list, 0, sizeof(uuid16_list));

	/* Group all UUID16 types */
	list_for_each(p, &hdev->uuids) {
		struct bt_uuid *uuid = list_entry(p, struct bt_uuid, list);
		u16 uuid16;

		uuid16 = get_uuid16(uuid->uuid);
		if (uuid16 == 0)
			return;

		if (uuid16 < 0x1100)
			continue;

		if (uuid16 == PNP_INFO_SVCLASS_ID)
			continue;

		/* Stop if not enough space to put next UUID */
		if (eir_len + 2 + sizeof(u16) > HCI_MAX_EIR_LENGTH) {
			truncated = 1;
			break;
		}

		/* Check for duplicates */
		for (i = 0; uuid16_list[i] != 0; i++)
			if (uuid16_list[i] == uuid16)
				break;

		if (uuid16_list[i] == 0) {
			uuid16_list[i] = uuid16;
			eir_len += sizeof(u16);
		}
	}

	if (uuid16_list[0] != 0) {
		u8 *length = ptr;

		/* EIR Data type */
		ptr[1] = truncated ? EIR_UUID16_SOME : EIR_UUID16_ALL;

		ptr += 2;
		eir_len += 2;

		for (i = 0; uuid16_list[i] != 0; i++) {
			*ptr++ = (uuid16_list[i] & 0x00ff);
			*ptr++ = (uuid16_list[i] & 0xff00) >> 8;
		}

		/* EIR Data length */
		*length = (i * sizeof(u16)) + 1;
	}
}

static int update_eir(struct hci_dev *hdev)
{
	struct hci_cp_write_eir cp;

	if (!(hdev->features[6] & LMP_EXT_INQ))
		return 0;

	if (hdev->ssp_mode == 0)
		return 0;

	if (test_bit(HCI_SERVICE_CACHE, &hdev->flags))
		return 0;

	memset(&cp, 0, sizeof(cp));

	create_eir(hdev, cp.data);

	if (memcmp(cp.data, hdev->eir, sizeof(cp.data)) == 0)
		return 0;

	memcpy(hdev->eir, cp.data, sizeof(cp.data));

	return hci_send_cmd(hdev, HCI_OP_WRITE_EIR, sizeof(cp), &cp);
}

static int add_uuid(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct mgmt_cp_add_uuid *cp;
	struct hci_dev *hdev;
	struct bt_uuid *uuid;
	int err;

	cp = (void *) data;

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_ADD_UUID, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_ADD_UUID, ENODEV);

	hci_dev_lock_bh(hdev);
=======
	hci_dev_unlock(hdev);
	return err;
}

static int set_pairable(struct sock *sk, struct hci_dev *hdev, void *data,
			u16 len)
{
	struct mgmt_mode *cp = data;
	int err;

	BT_DBG("request for %s", hdev->name);

	hci_dev_lock(hdev);

	if (cp->val)
		set_bit(HCI_PAIRABLE, &hdev->dev_flags);
	else
		clear_bit(HCI_PAIRABLE, &hdev->dev_flags);

	err = send_settings_rsp(sk, MGMT_OP_SET_PAIRABLE, hdev);
	if (err < 0)
		goto failed;

	err = new_settings(hdev, sk);

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int set_link_security(struct sock *sk, struct hci_dev *hdev, void *data,
			     u16 len)
{
	struct mgmt_mode *cp = data;
	struct pending_cmd *cmd;
	u8 val;
	int err;

	BT_DBG("request for %s", hdev->name);

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		bool changed = false;

		if (!!cp->val != test_bit(HCI_LINK_SECURITY,
							&hdev->dev_flags)) {
			change_bit(HCI_LINK_SECURITY, &hdev->dev_flags);
			changed = true;
		}

		err = send_settings_rsp(sk, MGMT_OP_SET_LINK_SECURITY, hdev);
		if (err < 0)
			goto failed;

		if (changed)
			err = new_settings(hdev, sk);

		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_LINK_SECURITY, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_LINK_SECURITY,
				 MGMT_STATUS_BUSY);
		goto failed;
	}

	val = !!cp->val;

	if (test_bit(HCI_AUTH, &hdev->flags) == val) {
		err = send_settings_rsp(sk, MGMT_OP_SET_LINK_SECURITY, hdev);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_LINK_SECURITY, hdev, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	err = hci_send_cmd(hdev, HCI_OP_WRITE_AUTH_ENABLE, sizeof(val), &val);
	if (err < 0) {
		mgmt_pending_remove(cmd);
		goto failed;
	}

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int set_ssp(struct sock *sk, struct hci_dev *hdev, void *data, u16 len)
{
	struct mgmt_mode *cp = data;
	struct pending_cmd *cmd;
	u8 val;
	int err;

	BT_DBG("request for %s", hdev->name);

	hci_dev_lock(hdev);

	if (!(hdev->features[6] & LMP_SIMPLE_PAIR)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_SSP,
				 MGMT_STATUS_NOT_SUPPORTED);
		goto failed;
	}

	val = !!cp->val;

	if (!hdev_is_powered(hdev)) {
		bool changed = false;

		if (val != test_bit(HCI_SSP_ENABLED, &hdev->dev_flags)) {
			change_bit(HCI_SSP_ENABLED, &hdev->dev_flags);
			changed = true;
		}

		err = send_settings_rsp(sk, MGMT_OP_SET_SSP, hdev);
		if (err < 0)
			goto failed;

		if (changed)
			err = new_settings(hdev, sk);

		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_SET_SSP, hdev)) {
	     err = cmd_status(sk, hdev->id, MGMT_OP_SET_SSP,
			      MGMT_STATUS_BUSY);
		goto failed;
	}

	if (test_bit(HCI_SSP_ENABLED, &hdev->dev_flags) == val) {
		err = send_settings_rsp(sk, MGMT_OP_SET_SSP, hdev);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_SSP, hdev, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	err = hci_send_cmd(hdev, HCI_OP_WRITE_SSP_MODE, sizeof(val), &val);
	if (err < 0) {
		mgmt_pending_remove(cmd);
		goto failed;
	}

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int set_hs(struct sock *sk, struct hci_dev *hdev, void *data, u16 len)
{
	struct mgmt_mode *cp = data;

	BT_DBG("request for %s", hdev->name);

	if (!enable_hs)
		return cmd_status(sk, hdev->id, MGMT_OP_SET_HS,
				  MGMT_STATUS_NOT_SUPPORTED);

	if (cp->val)
		set_bit(HCI_HS_ENABLED, &hdev->dev_flags);
	else
		clear_bit(HCI_HS_ENABLED, &hdev->dev_flags);

	return send_settings_rsp(sk, MGMT_OP_SET_HS, hdev);
}

static int set_le(struct sock *sk, struct hci_dev *hdev, void *data, u16 len)
{
	struct mgmt_mode *cp = data;
	struct hci_cp_write_le_host_supported hci_cp;
	struct pending_cmd *cmd;
	int err;
	u8 val, enabled;

	BT_DBG("request for %s", hdev->name);

	hci_dev_lock(hdev);

	if (!enable_le || !(hdev->features[4] & LMP_LE)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_LE,
				 MGMT_STATUS_NOT_SUPPORTED);
		goto unlock;
	}

	val = !!cp->val;
	enabled = !!(hdev->host_features[0] & LMP_HOST_LE);

	if (!hdev_is_powered(hdev) || val == enabled) {
		bool changed = false;

		if (val != test_bit(HCI_LE_ENABLED, &hdev->dev_flags)) {
			change_bit(HCI_LE_ENABLED, &hdev->dev_flags);
			changed = true;
		}

		err = send_settings_rsp(sk, MGMT_OP_SET_LE, hdev);
		if (err < 0)
			goto unlock;

		if (changed)
			err = new_settings(hdev, sk);

		goto unlock;
	}

	if (mgmt_pending_find(MGMT_OP_SET_LE, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_LE,
				 MGMT_STATUS_BUSY);
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_LE, hdev, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto unlock;
	}

	memset(&hci_cp, 0, sizeof(hci_cp));

	if (val) {
		hci_cp.le = val;
		hci_cp.simul = !!(hdev->features[6] & LMP_SIMUL_LE_BR);
	}

	err = hci_send_cmd(hdev, HCI_OP_WRITE_LE_HOST_SUPPORTED, sizeof(hci_cp),
			   &hci_cp);
	if (err < 0) {
		mgmt_pending_remove(cmd);
		goto unlock;
	}

unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int add_uuid(struct sock *sk, struct hci_dev *hdev, void *data, u16 len)
{
	struct mgmt_cp_add_uuid *cp = data;
	struct pending_cmd *cmd;
	struct bt_uuid *uuid;
	int err;

	BT_DBG("request for %s", hdev->name);

	hci_dev_lock(hdev);

	if (test_bit(HCI_PENDING_CLASS, &hdev->dev_flags)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_ADD_UUID,
				 MGMT_STATUS_BUSY);
		goto failed;
	}
>>>>>>> remotes/linux2/linux-3.4.y

	uuid = kmalloc(sizeof(*uuid), GFP_ATOMIC);
	if (!uuid) {
		err = -ENOMEM;
		goto failed;
	}

	memcpy(uuid->uuid, cp->uuid, 16);
	uuid->svc_hint = cp->svc_hint;

	list_add(&uuid->list, &hdev->uuids);

<<<<<<< HEAD
	if (test_bit(HCI_UP, &hdev->flags)) {

		err = update_class(hdev);
		if (err < 0)
			goto failed;

		err = update_eir(hdev);
		if (err < 0)
			goto failed;
	} else
		err = 0;

	err = cmd_complete(sk, index, MGMT_OP_ADD_UUID, NULL, 0);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int remove_uuid(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct list_head *p, *n;
	struct mgmt_cp_remove_uuid *cp;
	struct hci_dev *hdev;
	u8 bt_uuid_any[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int err, found;

	cp = (void *) data;

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_REMOVE_UUID, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_REMOVE_UUID, ENODEV);

	hci_dev_lock_bh(hdev);

	if (memcmp(cp->uuid, bt_uuid_any, 16) == 0) {
		err = hci_uuids_clear(hdev);
		goto unlock;
=======
	err = update_class(hdev);
	if (err < 0)
		goto failed;

	err = update_eir(hdev);
	if (err < 0)
		goto failed;

	if (!test_bit(HCI_PENDING_CLASS, &hdev->dev_flags)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_ADD_UUID, 0,
				   hdev->dev_class, 3);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_ADD_UUID, hdev, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

failed:
	hci_dev_unlock(hdev);
	return err;
}

static bool enable_service_cache(struct hci_dev *hdev)
{
	if (!hdev_is_powered(hdev))
		return false;

	if (!test_and_set_bit(HCI_SERVICE_CACHE, &hdev->dev_flags)) {
		schedule_delayed_work(&hdev->service_cache, CACHE_TIMEOUT);
		return true;
	}

	return false;
}

static int remove_uuid(struct sock *sk, struct hci_dev *hdev, void *data,
								u16 len)
{
	struct mgmt_cp_remove_uuid *cp = data;
	struct pending_cmd *cmd;
	struct list_head *p, *n;
	u8 bt_uuid_any[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int err, found;

	BT_DBG("request for %s", hdev->name);

	hci_dev_lock(hdev);

	if (test_bit(HCI_PENDING_CLASS, &hdev->dev_flags)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_REMOVE_UUID,
				 MGMT_STATUS_BUSY);
		goto unlock;
	}

	if (memcmp(cp->uuid, bt_uuid_any, 16) == 0) {
		err = hci_uuids_clear(hdev);

		if (enable_service_cache(hdev)) {
			err = cmd_complete(sk, hdev->id, MGMT_OP_REMOVE_UUID,
					   0, hdev->dev_class, 3);
			goto unlock;
		}

		goto update_class;
>>>>>>> remotes/linux2/linux-3.4.y
	}

	found = 0;

	list_for_each_safe(p, n, &hdev->uuids) {
		struct bt_uuid *match = list_entry(p, struct bt_uuid, list);

		if (memcmp(match->uuid, cp->uuid, 16) != 0)
			continue;

		list_del(&match->list);
		found++;
	}

	if (found == 0) {
<<<<<<< HEAD
		err = cmd_status(sk, index, MGMT_OP_REMOVE_UUID, ENOENT);
		goto unlock;
	}

	if (test_bit(HCI_UP, &hdev->flags)) {
		err = update_class(hdev);
		if (err < 0)
			goto unlock;

		err = update_eir(hdev);
		if (err < 0)
			goto unlock;
	} else
		err = 0;

	err = cmd_complete(sk, index, MGMT_OP_REMOVE_UUID, NULL, 0);

unlock:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_dev_class(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_set_dev_class *cp;
	int err;

	cp = (void *) data;

	BT_DBG("request for hci%u", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_DEV_CLASS, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_DEV_CLASS, ENODEV);

	hci_dev_lock_bh(hdev);

	hdev->major_class &= ~MGMT_MAJOR_CLASS_MASK;
	hdev->major_class |= cp->major & MGMT_MAJOR_CLASS_MASK;
	hdev->minor_class = cp->minor;

	if (test_bit(HCI_UP, &hdev->flags)) {
		err = update_class(hdev);
		if (err == 0)
			err = cmd_complete(sk, index,
		MGMT_OP_SET_DEV_CLASS, hdev->dev_class, sizeof(u8)*3);
	} else
		err = cmd_complete(sk, index, MGMT_OP_SET_DEV_CLASS, NULL, 0);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_service_cache(struct sock *sk, u16 index,  unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_set_service_cache *cp;
	int err;

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_SERVICE_CACHE, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_SERVICE_CACHE, ENODEV);

	hci_dev_lock_bh(hdev);

	BT_DBG("hci%u enable %d", index, cp->enable);

	if (cp->enable) {
		set_bit(HCI_SERVICE_CACHE, &hdev->flags);
		err = 0;
	} else {
		clear_bit(HCI_SERVICE_CACHE, &hdev->flags);
		if (test_bit(HCI_UP, &hdev->flags)) {
			err = update_class(hdev);
			if (err == 0)
				err = update_eir(hdev);
		} else
			err = 0;
	}

	if (err == 0)
		err = cmd_complete(sk, index, MGMT_OP_SET_SERVICE_CACHE, NULL,
									0);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int load_keys(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_load_keys *cp;
	u16 key_count, expected_len;
	int i, err;

	cp = (void *) data;

	if (len < sizeof(*cp))
		return -EINVAL;

	key_count = get_unaligned_le16(&cp->key_count);

	expected_len = sizeof(*cp) + key_count * sizeof(struct mgmt_key_info);
	if (expected_len > len) {
		BT_ERR("load_keys: expected at least %u bytes, got %u bytes",
							expected_len, len);
		return -EINVAL;
	}

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_LOAD_KEYS, ENODEV);

	BT_DBG("hci%u debug_keys %u key_count %u", index, cp->debug_keys,
								key_count);

	hci_dev_lock_bh(hdev);

	hci_link_keys_clear(hdev);

	set_bit(HCI_LINK_KEYS, &hdev->flags);

	if (cp->debug_keys)
		set_bit(HCI_DEBUG_KEYS, &hdev->flags);
	else
		clear_bit(HCI_DEBUG_KEYS, &hdev->flags);

	len -= sizeof(*cp);
	i = 0;

	while (i < len) {
		struct mgmt_key_info *key = (void *) cp->keys + i;

		i += sizeof(*key);

		if (key->key_type == KEY_TYPE_LTK) {
			struct key_master_id *id = (void *) key->data;

			if (key->dlen != sizeof(struct key_master_id))
				continue;

			hci_add_ltk(hdev, 0, &key->bdaddr, key->addr_type,
					key->pin_len, key->auth, id->ediv,
					id->rand, key->val);

			continue;
		}

		hci_add_link_key(hdev, 0, &key->bdaddr, key->val, key->key_type,
								key->pin_len);
	}

	err = cmd_complete(sk, index, MGMT_OP_LOAD_KEYS, NULL, 0);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int remove_key(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_remove_key *cp;
	struct hci_conn *conn;
	int err;

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_REMOVE_KEY, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_REMOVE_KEY, ENODEV);

	hci_dev_lock_bh(hdev);

	err = hci_remove_link_key(hdev, &cp->bdaddr);
	if (err < 0) {
		err = cmd_status(sk, index, MGMT_OP_REMOVE_KEY, -err);
		goto unlock;
	}

	err = 0;

	if (!test_bit(HCI_UP, &hdev->flags) || !cp->disconnect)
		goto unlock;

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);
	if (conn) {
		struct hci_cp_disconnect dc;

		put_unaligned_le16(conn->handle, &dc.handle);
		dc.reason = 0x13; /* Remote User Terminated Connection */
		err = hci_send_cmd(hdev, HCI_OP_DISCONNECT, 0, NULL);
	}

unlock:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int disconnect(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_disconnect *cp;
=======
		err = cmd_status(sk, hdev->id, MGMT_OP_REMOVE_UUID,
				 MGMT_STATUS_INVALID_PARAMS);
		goto unlock;
	}

update_class:
	err = update_class(hdev);
	if (err < 0)
		goto unlock;

	err = update_eir(hdev);
	if (err < 0)
		goto unlock;

	if (!test_bit(HCI_PENDING_CLASS, &hdev->dev_flags)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_REMOVE_UUID, 0,
				   hdev->dev_class, 3);
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_REMOVE_UUID, hdev, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto unlock;
	}

unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int set_dev_class(struct sock *sk, struct hci_dev *hdev, void *data,
			 u16 len)
{
	struct mgmt_cp_set_dev_class *cp = data;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("request for %s", hdev->name);

	hci_dev_lock(hdev);

	if (test_bit(HCI_PENDING_CLASS, &hdev->dev_flags)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_DEV_CLASS,
				 MGMT_STATUS_BUSY);
		goto unlock;
	}

	hdev->major_class = cp->major;
	hdev->minor_class = cp->minor;

	if (!hdev_is_powered(hdev)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_SET_DEV_CLASS, 0,
				   hdev->dev_class, 3);
		goto unlock;
	}

	if (test_and_clear_bit(HCI_SERVICE_CACHE, &hdev->dev_flags)) {
		hci_dev_unlock(hdev);
		cancel_delayed_work_sync(&hdev->service_cache);
		hci_dev_lock(hdev);
		update_eir(hdev);
	}

	err = update_class(hdev);
	if (err < 0)
		goto unlock;

	if (!test_bit(HCI_PENDING_CLASS, &hdev->dev_flags)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_SET_DEV_CLASS, 0,
				   hdev->dev_class, 3);
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_DEV_CLASS, hdev, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto unlock;
	}

unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int load_link_keys(struct sock *sk, struct hci_dev *hdev, void *data,
								u16 len)
{
	struct mgmt_cp_load_link_keys *cp = data;
	u16 key_count, expected_len;
	int i;

	key_count = get_unaligned_le16(&cp->key_count);

	expected_len = sizeof(*cp) + key_count *
					sizeof(struct mgmt_link_key_info);
	if (expected_len != len) {
		BT_ERR("load_link_keys: expected %u bytes, got %u bytes",
							len, expected_len);
		return cmd_status(sk, hdev->id, MGMT_OP_LOAD_LINK_KEYS,
				  MGMT_STATUS_INVALID_PARAMS);
	}

	BT_DBG("%s debug_keys %u key_count %u", hdev->name, cp->debug_keys,
								key_count);

	hci_dev_lock(hdev);

	hci_link_keys_clear(hdev);

	set_bit(HCI_LINK_KEYS, &hdev->dev_flags);

	if (cp->debug_keys)
		set_bit(HCI_DEBUG_KEYS, &hdev->dev_flags);
	else
		clear_bit(HCI_DEBUG_KEYS, &hdev->dev_flags);

	for (i = 0; i < key_count; i++) {
		struct mgmt_link_key_info *key = &cp->keys[i];

		hci_add_link_key(hdev, NULL, 0, &key->addr.bdaddr, key->val,
				 key->type, key->pin_len);
	}

	cmd_complete(sk, hdev->id, MGMT_OP_LOAD_LINK_KEYS, 0, NULL, 0);

	hci_dev_unlock(hdev);

	return 0;
}

static int device_unpaired(struct hci_dev *hdev, bdaddr_t *bdaddr,
			   u8 addr_type, struct sock *skip_sk)
{
	struct mgmt_ev_device_unpaired ev;

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = addr_type;

	return mgmt_event(MGMT_EV_DEVICE_UNPAIRED, hdev, &ev, sizeof(ev),
			  skip_sk);
}

static int unpair_device(struct sock *sk, struct hci_dev *hdev, void *data,
			 u16 len)
{
	struct mgmt_cp_unpair_device *cp = data;
	struct mgmt_rp_unpair_device rp;
>>>>>>> remotes/linux2/linux-3.4.y
	struct hci_cp_disconnect dc;
	struct pending_cmd *cmd;
	struct hci_conn *conn;
	int err;

<<<<<<< HEAD
	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_DISCONNECT, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_DISCONNECT, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_DISCONNECT, ENETDOWN);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_DISCONNECT, index)) {
		err = cmd_status(sk, index, MGMT_OP_DISCONNECT, EBUSY);
		goto failed;
	}

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);
	if (!conn) {
		conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);
		if (!conn) {
			err = cmd_status(sk, index, MGMT_OP_DISCONNECT,
							ENOTCONN);
			goto failed;
		}
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_DISCONNECT, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
=======
	hci_dev_lock(hdev);

	memset(&rp, 0, sizeof(rp));
	bacpy(&rp.addr.bdaddr, &cp->addr.bdaddr);
	rp.addr.type = cp->addr.type;

	if (!hdev_is_powered(hdev)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_UNPAIR_DEVICE,
				   MGMT_STATUS_NOT_POWERED, &rp, sizeof(rp));
		goto unlock;
	}

	if (cp->addr.type == MGMT_ADDR_BREDR)
		err = hci_remove_link_key(hdev, &cp->addr.bdaddr);
	else
		err = hci_remove_ltk(hdev, &cp->addr.bdaddr);

	if (err < 0) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_UNPAIR_DEVICE,
				   MGMT_STATUS_NOT_PAIRED, &rp, sizeof(rp));
		goto unlock;
	}

	if (cp->disconnect) {
		if (cp->addr.type == MGMT_ADDR_BREDR)
			conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK,
							&cp->addr.bdaddr);
		else
			conn = hci_conn_hash_lookup_ba(hdev, LE_LINK,
							&cp->addr.bdaddr);
	} else {
		conn = NULL;
	}

	if (!conn) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_UNPAIR_DEVICE, 0,
				   &rp, sizeof(rp));
		device_unpaired(hdev, &cp->addr.bdaddr, cp->addr.type, sk);
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_UNPAIR_DEVICE, hdev, cp,
			       sizeof(*cp));
	if (!cmd) {
		err = -ENOMEM;
		goto unlock;
>>>>>>> remotes/linux2/linux-3.4.y
	}

	put_unaligned_le16(conn->handle, &dc.handle);
	dc.reason = 0x13; /* Remote User Terminated Connection */
<<<<<<< HEAD

=======
>>>>>>> remotes/linux2/linux-3.4.y
	err = hci_send_cmd(hdev, HCI_OP_DISCONNECT, sizeof(dc), &dc);
	if (err < 0)
		mgmt_pending_remove(cmd);

<<<<<<< HEAD
failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int get_connections(struct sock *sk, u16 index)
{
	struct mgmt_rp_get_connections *rp;
	struct hci_dev *hdev;
	struct list_head *p;
	size_t rp_len;
	u16 count;
	int i, err;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_GET_CONNECTIONS, ENODEV);

	hci_dev_lock_bh(hdev);

	count = 0;
	list_for_each(p, &hdev->conn_hash.list) {
		count++;
	}

	rp_len = sizeof(*rp) + (count * sizeof(bdaddr_t));
	rp = kmalloc(rp_len, GFP_ATOMIC);
	if (!rp) {
		err = -ENOMEM;
		goto unlock;
	}

	put_unaligned_le16(count, &rp->conn_count);

	read_lock(&hci_dev_list_lock);

	i = 0;
	list_for_each(p, &hdev->conn_hash.list) {
		struct hci_conn *c = list_entry(p, struct hci_conn, list);

		bacpy(&rp->conn[i++], &c->dst);
	}

	read_unlock(&hci_dev_list_lock);

	err = cmd_complete(sk, index, MGMT_OP_GET_CONNECTIONS, rp, rp_len);

unlock:
	kfree(rp);
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
	return err;
}

static int pin_code_reply(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_pin_code_reply *cp;
	struct hci_cp_pin_code_reply reply;
	struct pending_cmd *cmd;
=======
unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int disconnect(struct sock *sk, struct hci_dev *hdev, void *data,
		      u16 len)
{
	struct mgmt_cp_disconnect *cp = data;
	struct hci_cp_disconnect dc;
	struct pending_cmd *cmd;
	struct hci_conn *conn;
>>>>>>> remotes/linux2/linux-3.4.y
	int err;

	BT_DBG("");

<<<<<<< HEAD
	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_PIN_CODE_REPLY, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_PIN_CODE_REPLY, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_PIN_CODE_REPLY, ENETDOWN);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_PIN_CODE_REPLY, index, data, len);
=======
	hci_dev_lock(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_DISCONNECT,
				 MGMT_STATUS_NOT_POWERED);
		goto failed;
	}

	if (mgmt_pending_find(MGMT_OP_DISCONNECT, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_DISCONNECT,
				 MGMT_STATUS_BUSY);
		goto failed;
	}

	if (cp->addr.type == MGMT_ADDR_BREDR)
		conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->addr.bdaddr);
	else
		conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->addr.bdaddr);

	if (!conn) {
		err = cmd_status(sk, hdev->id, MGMT_OP_DISCONNECT,
				 MGMT_STATUS_NOT_CONNECTED);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_DISCONNECT, hdev, data, len);
>>>>>>> remotes/linux2/linux-3.4.y
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

<<<<<<< HEAD
	bacpy(&reply.bdaddr, &cp->bdaddr);
	reply.pin_len = cp->pin_len;
	memcpy(reply.pin_code, cp->pin_code, 16);

	err = hci_send_cmd(hdev, HCI_OP_PIN_CODE_REPLY, sizeof(reply), &reply);
=======
	put_unaligned_le16(conn->handle, &dc.handle);
	dc.reason = 0x13; /* Remote User Terminated Connection */

	err = hci_send_cmd(hdev, HCI_OP_DISCONNECT, sizeof(dc), &dc);
>>>>>>> remotes/linux2/linux-3.4.y
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
<<<<<<< HEAD
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int encrypt_link(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_encrypt_link *cp;
	struct hci_cp_set_conn_encrypt enc;
	struct hci_conn *conn;
	int err = 0;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, ENETDOWN);
		goto done;
	}

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->bdaddr);
	if (!conn) {
		err = cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, ENOTCONN);
		goto done;
	}

	if (test_and_set_bit(HCI_CONN_ENCRYPT_PEND, &conn->pend)) {
		err = cmd_status(sk, index, MGMT_OP_ENCRYPT_LINK, EINPROGRESS);
		goto done;
	}

	if (conn->link_mode & HCI_LM_AUTH) {
		enc.handle = cpu_to_le16(conn->handle);
		enc.encrypt = cp->enable;
		err = hci_send_cmd(hdev,
				HCI_OP_SET_CONN_ENCRYPT, sizeof(enc), &enc);
	} else {
		conn->auth_initiator = 1;
		if (!test_and_set_bit(HCI_CONN_AUTH_PEND, &conn->pend)) {
			struct hci_cp_auth_requested cp;
			cp.handle = cpu_to_le16(conn->handle);
			err = hci_send_cmd(conn->hdev,
				HCI_OP_AUTH_REQUESTED, sizeof(cp), &cp);
		}
	}

done:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}


static int pin_code_neg_reply(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_pin_code_neg_reply *cp;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_PIN_CODE_NEG_REPLY,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_PIN_CODE_NEG_REPLY,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_PIN_CODE_NEG_REPLY,
								ENETDOWN);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_PIN_CODE_NEG_REPLY, index,
								data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	err = hci_send_cmd(hdev, HCI_OP_PIN_CODE_NEG_REPLY, sizeof(cp->bdaddr),
								&cp->bdaddr);
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int le_add_dev_white_list(struct sock *sk, u16 index,
					unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_le_add_dev_white_list *cp;
	int err = 0;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_LE_ADD_DEV_WHITE_LIST,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_LE_ADD_DEV_WHITE_LIST,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_LE_ADD_DEV_WHITE_LIST,
								ENETDOWN);
		goto failed;
	}

	hci_le_add_dev_white_list(hdev, &cp->bdaddr);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int le_remove_dev_white_list(struct sock *sk, u16 index,
					unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_le_remove_dev_white_list *cp;
	int err = 0;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_LE_REMOVE_DEV_WHITE_LIST,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_LE_REMOVE_DEV_WHITE_LIST,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_LE_REMOVE_DEV_WHITE_LIST,
								ENETDOWN);
		goto failed;
	}

	hci_le_remove_dev_white_list(hdev, &cp->bdaddr);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
=======
	hci_dev_unlock(hdev);
	return err;
}

static u8 link_to_mgmt(u8 link_type, u8 addr_type)
{
	switch (link_type) {
	case LE_LINK:
		switch (addr_type) {
		case ADDR_LE_DEV_PUBLIC:
			return MGMT_ADDR_LE_PUBLIC;
		case ADDR_LE_DEV_RANDOM:
			return MGMT_ADDR_LE_RANDOM;
		default:
			return MGMT_ADDR_INVALID;
		}
	case ACL_LINK:
		return MGMT_ADDR_BREDR;
	default:
		return MGMT_ADDR_INVALID;
	}
}

static int get_connections(struct sock *sk, struct hci_dev *hdev, void *data,
			   u16 data_len)
{
	struct mgmt_rp_get_connections *rp;
	struct hci_conn *c;
	size_t rp_len;
	int err;
	u16 i;

	BT_DBG("");

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_GET_CONNECTIONS,
				 MGMT_STATUS_NOT_POWERED);
		goto unlock;
	}

	i = 0;
	list_for_each_entry(c, &hdev->conn_hash.list, list) {
		if (test_bit(HCI_CONN_MGMT_CONNECTED, &c->flags))
			i++;
	}

	rp_len = sizeof(*rp) + (i * sizeof(struct mgmt_addr_info));
	rp = kmalloc(rp_len, GFP_ATOMIC);
	if (!rp) {
		err = -ENOMEM;
		goto unlock;
	}

	i = 0;
	list_for_each_entry(c, &hdev->conn_hash.list, list) {
		if (!test_bit(HCI_CONN_MGMT_CONNECTED, &c->flags))
			continue;
		bacpy(&rp->addr[i].bdaddr, &c->dst);
		rp->addr[i].type = link_to_mgmt(c->type, c->dst_type);
		if (rp->addr[i].type == MGMT_ADDR_INVALID)
			continue;
		i++;
	}

	put_unaligned_le16(i, &rp->conn_count);

	/* Recalculate length in case of filtered SCO connections, etc */
	rp_len = sizeof(*rp) + (i * sizeof(struct mgmt_addr_info));

	err = cmd_complete(sk, hdev->id, MGMT_OP_GET_CONNECTIONS, 0, rp,
			   rp_len);

	kfree(rp);

unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int send_pin_code_neg_reply(struct sock *sk, struct hci_dev *hdev,
				   struct mgmt_cp_pin_code_neg_reply *cp)
{
	struct pending_cmd *cmd;
	int err;

	cmd = mgmt_pending_add(sk, MGMT_OP_PIN_CODE_NEG_REPLY, hdev, cp,
			       sizeof(*cp));
	if (!cmd)
		return -ENOMEM;

	err = hci_send_cmd(hdev, HCI_OP_PIN_CODE_NEG_REPLY,
			   sizeof(cp->addr.bdaddr), &cp->addr.bdaddr);
	if (err < 0)
		mgmt_pending_remove(cmd);
>>>>>>> remotes/linux2/linux-3.4.y

	return err;
}

<<<<<<< HEAD
static int le_create_conn_white_list(struct sock *sk, u16 index)
{
	struct hci_dev *hdev;
	struct hci_conn *conn;
	u8 sec_level, auth_type;
	struct pending_cmd *cmd;
	bdaddr_t bdaddr;
	int err = 0;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_LE_CREATE_CONN_WHITE_LIST,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_LE_CREATE_CONN_WHITE_LIST,
								ENETDOWN);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_LE_CREATE_CONN_WHITE_LIST, index,
								NULL, 0);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	sec_level = BT_SECURITY_MEDIUM;
	auth_type = HCI_AT_GENERAL_BONDING;
	memset(&bdaddr, 0, sizeof(bdaddr));
	conn = hci_le_connect(hdev, 0, BDADDR_ANY, sec_level, auth_type, NULL);
	if (IS_ERR(conn)) {
		err = PTR_ERR(conn);
		mgmt_pending_remove(cmd);
	}

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int le_cancel_create_conn_white_list(struct sock *sk, u16 index)
{
	struct hci_dev *hdev;
	int err = 0;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index,
			MGMT_OP_LE_CANCEL_CREATE_CONN_WHITE_LIST, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index,
			MGMT_OP_LE_CANCEL_CREATE_CONN_WHITE_LIST, ENETDOWN);
		goto failed;
	}

	hci_le_cancel_create_connect(hdev, BDADDR_ANY);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int le_clear_white_list(struct sock *sk, u16 index)
{
	struct hci_dev *hdev;
=======
static int pin_code_reply(struct sock *sk, struct hci_dev *hdev, void *data,
			  u16 len)
{
	struct hci_conn *conn;
	struct mgmt_cp_pin_code_reply *cp = data;
	struct hci_cp_pin_code_reply reply;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("");

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_PIN_CODE_REPLY,
				 MGMT_STATUS_NOT_POWERED);
		goto failed;
	}

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, &cp->addr.bdaddr);
	if (!conn) {
		err = cmd_status(sk, hdev->id, MGMT_OP_PIN_CODE_REPLY,
				 MGMT_STATUS_NOT_CONNECTED);
		goto failed;
	}

	if (conn->pending_sec_level == BT_SECURITY_HIGH && cp->pin_len != 16) {
		struct mgmt_cp_pin_code_neg_reply ncp;

		memcpy(&ncp.addr, &cp->addr, sizeof(ncp.addr));

		BT_ERR("PIN code is not 16 bytes long");

		err = send_pin_code_neg_reply(sk, hdev, &ncp);
		if (err >= 0)
			err = cmd_status(sk, hdev->id, MGMT_OP_PIN_CODE_REPLY,
					 MGMT_STATUS_INVALID_PARAMS);

		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_PIN_CODE_REPLY, hdev, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	bacpy(&reply.bdaddr, &cp->addr.bdaddr);
	reply.pin_len = cp->pin_len;
	memcpy(reply.pin_code, cp->pin_code, sizeof(reply.pin_code));

	err = hci_send_cmd(hdev, HCI_OP_PIN_CODE_REPLY, sizeof(reply), &reply);
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int pin_code_neg_reply(struct sock *sk, struct hci_dev *hdev,
			      void *data, u16 len)
{
	struct mgmt_cp_pin_code_neg_reply *cp = data;
>>>>>>> remotes/linux2/linux-3.4.y
	int err;

	BT_DBG("");

<<<<<<< HEAD
	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index,
			MGMT_OP_LE_CLEAR_WHITE_LIST, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index,
			MGMT_OP_LE_CLEAR_WHITE_LIST, ENETDOWN);
		goto failed;
	}

	err = hci_send_cmd(hdev, HCI_OP_LE_CLEAR_WHITE_LIST, 0, NULL);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_io_capability(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_set_io_capability *cp;

	BT_DBG("");

	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_IO_CAPABILITY, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_IO_CAPABILITY, ENODEV);

	hci_dev_lock_bh(hdev);
=======
	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_PIN_CODE_NEG_REPLY,
				 MGMT_STATUS_NOT_POWERED);
		goto failed;
	}

	err = send_pin_code_neg_reply(sk, hdev, cp);

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int set_io_capability(struct sock *sk, struct hci_dev *hdev, void *data,
			     u16 len)
{
	struct mgmt_cp_set_io_capability *cp = data;

	BT_DBG("");

	hci_dev_lock(hdev);
>>>>>>> remotes/linux2/linux-3.4.y

	hdev->io_capability = cp->io_capability;

	BT_DBG("%s IO capability set to 0x%02x", hdev->name,
							hdev->io_capability);

<<<<<<< HEAD
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return cmd_complete(sk, index, MGMT_OP_SET_IO_CAPABILITY, NULL, 0);
=======
	hci_dev_unlock(hdev);

	return cmd_complete(sk, hdev->id, MGMT_OP_SET_IO_CAPABILITY, 0, NULL,
			    0);
>>>>>>> remotes/linux2/linux-3.4.y
}

static inline struct pending_cmd *find_pairing(struct hci_conn *conn)
{
	struct hci_dev *hdev = conn->hdev;
<<<<<<< HEAD
	struct list_head *p;

	list_for_each(p, &cmd_list) {
		struct pending_cmd *cmd;

		cmd = list_entry(p, struct pending_cmd, list);

		if (cmd->opcode != MGMT_OP_PAIR_DEVICE)
			continue;

		if (cmd->index != hdev->id)
			continue;

=======
	struct pending_cmd *cmd;

	list_for_each_entry(cmd, &hdev->mgmt_pending, list) {
		if (cmd->opcode != MGMT_OP_PAIR_DEVICE)
			continue;

>>>>>>> remotes/linux2/linux-3.4.y
		if (cmd->user_data != conn)
			continue;

		return cmd;
	}

	return NULL;
}

static void pairing_complete(struct pending_cmd *cmd, u8 status)
{
	struct mgmt_rp_pair_device rp;
	struct hci_conn *conn = cmd->user_data;

<<<<<<< HEAD
	BT_DBG(" %u", status);

	bacpy(&rp.bdaddr, &conn->dst);
	rp.status = status;

	cmd_complete(cmd->sk, cmd->index, MGMT_OP_PAIR_DEVICE, &rp, sizeof(rp));
=======
	bacpy(&rp.addr.bdaddr, &conn->dst);
	rp.addr.type = link_to_mgmt(conn->type, conn->dst_type);

	cmd_complete(cmd->sk, cmd->index, MGMT_OP_PAIR_DEVICE, status,
		     &rp, sizeof(rp));
>>>>>>> remotes/linux2/linux-3.4.y

	/* So we don't get further callbacks for this connection */
	conn->connect_cfm_cb = NULL;
	conn->security_cfm_cb = NULL;
	conn->disconn_cfm_cb = NULL;

<<<<<<< HEAD
=======
	hci_conn_put(conn);

>>>>>>> remotes/linux2/linux-3.4.y
	mgmt_pending_remove(cmd);
}

static void pairing_complete_cb(struct hci_conn *conn, u8 status)
{
	struct pending_cmd *cmd;

<<<<<<< HEAD
	BT_DBG(" %u", status);

	cmd = find_pairing(conn);
	if (!cmd) {
		BT_DBG("Unable to find a pending command");
		return;
	}

	pairing_complete(cmd, status);
	hci_conn_put(conn);
}

static void pairing_security_complete_cb(struct hci_conn *conn, u8 status)
{
	struct pending_cmd *cmd;

	BT_DBG(" %u", status);

	cmd = find_pairing(conn);
	if (!cmd) {
		BT_DBG("Unable to find a pending command");
		return;
	}

	if (conn->type == LE_LINK)
		smp_link_encrypt_cmplt(conn->l2cap_data, status,
				status ? 0 : 1);
	else
		pairing_complete(cmd, status);
}

static void pairing_connect_complete_cb(struct hci_conn *conn, u8 status)
{
	struct pending_cmd *cmd;

	BT_DBG("conn: %p %u", conn, status);

	cmd = find_pairing(conn);
	if (!cmd) {
		BT_DBG("Unable to find a pending command");
		return;
	}

	if (status)
		pairing_complete(cmd, status);

	hci_conn_put(conn);
}

static void discovery_terminated(struct pending_cmd *cmd, void *data)
{
	struct hci_dev *hdev;
	struct mgmt_mode ev = {0};

	BT_DBG("");
	hdev = hci_dev_get(cmd->index);
	if (!hdev)
		goto not_found;

	del_timer(&hdev->disco_le_timer);
	del_timer(&hdev->disco_timer);
	hci_dev_put(hdev);

not_found:
	mgmt_event(MGMT_EV_DISCOVERING, cmd->index, &ev, sizeof(ev), NULL);

	list_del(&cmd->list);

	mgmt_pending_free(cmd);
}

static int pair_device(struct sock *sk, u16 index, unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_pair_device *cp;
	struct pending_cmd *cmd;
	u8 sec_level, auth_type, io_cap;
	struct hci_conn *conn;
	struct adv_entry *entry;
=======
	BT_DBG("status %u", status);

	cmd = find_pairing(conn);
	if (!cmd)
		BT_DBG("Unable to find a pending command");
	else
		pairing_complete(cmd, mgmt_status(status));
}

static int pair_device(struct sock *sk, struct hci_dev *hdev, void *data,
		       u16 len)
{
	struct mgmt_cp_pair_device *cp = data;
	struct mgmt_rp_pair_device rp;
	struct pending_cmd *cmd;
	u8 sec_level, auth_type;
	struct hci_conn *conn;
>>>>>>> remotes/linux2/linux-3.4.y
	int err;

	BT_DBG("");

<<<<<<< HEAD
	cp = (void *) data;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_PAIR_DEVICE, EINVAL);

	hdev = hci_dev_get(index);

	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_PAIR_DEVICE, ENODEV);

	hci_dev_lock_bh(hdev);

	io_cap = cp->io_cap;

	sec_level = BT_SECURITY_MEDIUM;
	auth_type = HCI_AT_DEDICATED_BONDING;

	entry = hci_find_adv_entry(hdev, &cp->bdaddr);
	if (entry && entry->flags & 0x04) {
		conn = hci_le_connect(hdev, 0, &cp->bdaddr, sec_level,
							auth_type, NULL);
	} else {
		/* ACL-SSP does not support io_cap 0x04 (KeyboadDisplay) */
		if (io_cap == 0x04)
			io_cap = 0x01;
		conn = hci_connect(hdev, ACL_LINK, 0, &cp->bdaddr, sec_level,
								auth_type);
		conn->auth_initiator = 1;
	}

	if (IS_ERR(conn)) {
		err = PTR_ERR(conn);
=======
	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_PAIR_DEVICE,
				 MGMT_STATUS_NOT_POWERED);
		goto unlock;
	}

	sec_level = BT_SECURITY_MEDIUM;
	if (cp->io_cap == 0x03)
		auth_type = HCI_AT_DEDICATED_BONDING;
	else
		auth_type = HCI_AT_DEDICATED_BONDING_MITM;

	if (cp->addr.type == MGMT_ADDR_BREDR)
		conn = hci_connect(hdev, ACL_LINK, &cp->addr.bdaddr, sec_level,
				   auth_type);
	else
		conn = hci_connect(hdev, LE_LINK, &cp->addr.bdaddr, sec_level,
				   auth_type);

	memset(&rp, 0, sizeof(rp));
	bacpy(&rp.addr.bdaddr, &cp->addr.bdaddr);
	rp.addr.type = cp->addr.type;

	if (IS_ERR(conn)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_PAIR_DEVICE,
				   MGMT_STATUS_CONNECT_FAILED, &rp,
				   sizeof(rp));
>>>>>>> remotes/linux2/linux-3.4.y
		goto unlock;
	}

	if (conn->connect_cfm_cb) {
		hci_conn_put(conn);
<<<<<<< HEAD
		err = cmd_status(sk, index, MGMT_OP_PAIR_DEVICE, EBUSY);
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_PAIR_DEVICE, index, data, len);
=======
		err = cmd_complete(sk, hdev->id, MGMT_OP_PAIR_DEVICE,
				   MGMT_STATUS_BUSY, &rp, sizeof(rp));
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_PAIR_DEVICE, hdev, data, len);
>>>>>>> remotes/linux2/linux-3.4.y
	if (!cmd) {
		err = -ENOMEM;
		hci_conn_put(conn);
		goto unlock;
	}

<<<<<<< HEAD
	conn->connect_cfm_cb = pairing_connect_complete_cb;
	conn->security_cfm_cb = pairing_security_complete_cb;
	conn->disconn_cfm_cb = pairing_complete_cb;
	conn->io_capability = io_cap;
=======
	/* For LE, just connecting isn't a proof that the pairing finished */
	if (cp->addr.type == MGMT_ADDR_BREDR)
		conn->connect_cfm_cb = pairing_complete_cb;

	conn->security_cfm_cb = pairing_complete_cb;
	conn->disconn_cfm_cb = pairing_complete_cb;
	conn->io_capability = cp->io_cap;
>>>>>>> remotes/linux2/linux-3.4.y
	cmd->user_data = conn;

	if (conn->state == BT_CONNECTED &&
				hci_conn_security(conn, sec_level, auth_type))
		pairing_complete(cmd, 0);

	err = 0;

unlock:
<<<<<<< HEAD
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int user_confirm_reply(struct sock *sk, u16 index, unsigned char *data,
							u16 len, u16 opcode)
{
	struct mgmt_cp_user_confirm_reply *cp = (void *) data;
	u16 mgmt_op = opcode, hci_op;
	struct pending_cmd *cmd;
	struct hci_dev *hdev;
	struct hci_conn *le_conn;
	int err;

	BT_DBG("%d", mgmt_op);

	if (mgmt_op == MGMT_OP_USER_CONFIRM_NEG_REPLY)
		hci_op = HCI_OP_USER_CONFIRM_NEG_REPLY;
	else
		hci_op = HCI_OP_USER_CONFIRM_REPLY;

	if (len < sizeof(*cp))
		return cmd_status(sk, index, mgmt_op, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, mgmt_op, ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, mgmt_op, ENETDOWN);
		goto done;
	}

	le_conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);
	if (le_conn) {
		err = le_user_confirm_reply(le_conn, mgmt_op, (void *) cp);
		goto done;
	}
	BT_DBG("BR/EDR: %s", mgmt_op == MGMT_OP_USER_CONFIRM_NEG_REPLY ?
							"Reject" : "Accept");

	cmd = mgmt_pending_add(sk, mgmt_op, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto done;
	}

	err = hci_send_cmd(hdev, hci_op, sizeof(cp->bdaddr), &cp->bdaddr);
	if (err < 0)
		mgmt_pending_remove(cmd);

done:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int resolve_name(struct sock *sk, u16 index, unsigned char *data,
								u16 len)
{
	struct mgmt_cp_resolve_name *mgmt_cp = (void *) data;
	struct hci_cp_remote_name_req hci_cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("");

	if (len != sizeof(*mgmt_cp))
		return cmd_status(sk, index, MGMT_OP_RESOLVE_NAME, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_RESOLVE_NAME, ENODEV);

	hci_dev_lock_bh(hdev);

	cmd = mgmt_pending_add(sk, MGMT_OP_RESOLVE_NAME, index, data, len);
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	memset(&hci_cp, 0, sizeof(hci_cp));
	bacpy(&hci_cp.bdaddr, &mgmt_cp->bdaddr);
	err = hci_send_cmd(hdev, HCI_OP_REMOTE_NAME_REQ, sizeof(hci_cp),
								&hci_cp);
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_connection_params(struct sock *sk, u16 index,
				unsigned char *data, u16 len)
{
	struct mgmt_cp_set_connection_params *cp = (void *) data;
	struct hci_dev *hdev;
	struct hci_conn *conn;
	int err;
=======
	hci_dev_unlock(hdev);
	return err;
}

static int cancel_pair_device(struct sock *sk, struct hci_dev *hdev, void *data,
			      u16 len)
{
	struct mgmt_addr_info *addr = data;
	struct pending_cmd *cmd;
	struct hci_conn *conn;
	int err;

	BT_DBG("");

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_CANCEL_PAIR_DEVICE,
				 MGMT_STATUS_NOT_POWERED);
		goto unlock;
	}

	cmd = mgmt_pending_find(MGMT_OP_PAIR_DEVICE, hdev);
	if (!cmd) {
		err = cmd_status(sk, hdev->id, MGMT_OP_CANCEL_PAIR_DEVICE,
				 MGMT_STATUS_INVALID_PARAMS);
		goto unlock;
	}

	conn = cmd->user_data;

	if (bacmp(&addr->bdaddr, &conn->dst) != 0) {
		err = cmd_status(sk, hdev->id, MGMT_OP_CANCEL_PAIR_DEVICE,
				 MGMT_STATUS_INVALID_PARAMS);
		goto unlock;
	}

	pairing_complete(cmd, MGMT_STATUS_CANCELLED);

	err = cmd_complete(sk, hdev->id, MGMT_OP_CANCEL_PAIR_DEVICE, 0,
			   addr, sizeof(*addr));
unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int user_pairing_resp(struct sock *sk, struct hci_dev *hdev,
			     bdaddr_t *bdaddr, u8 type, u16 mgmt_op,
			     u16 hci_op, __le32 passkey)
{
	struct pending_cmd *cmd;
	struct hci_conn *conn;
	int err;

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_status(sk, hdev->id, mgmt_op,
				 MGMT_STATUS_NOT_POWERED);
		goto done;
	}

	if (type == MGMT_ADDR_BREDR)
		conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, bdaddr);
	else
		conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, bdaddr);

	if (!conn) {
		err = cmd_status(sk, hdev->id, mgmt_op,
				 MGMT_STATUS_NOT_CONNECTED);
		goto done;
	}

	if (type == MGMT_ADDR_LE_PUBLIC || type == MGMT_ADDR_LE_RANDOM) {
		/* Continue with pairing via SMP */
		err = smp_user_confirm_reply(conn, mgmt_op, passkey);

		if (!err)
			err = cmd_status(sk, hdev->id, mgmt_op,
					 MGMT_STATUS_SUCCESS);
		else
			err = cmd_status(sk, hdev->id, mgmt_op,
					 MGMT_STATUS_FAILED);

		goto done;
	}

	cmd = mgmt_pending_add(sk, mgmt_op, hdev, bdaddr, sizeof(*bdaddr));
	if (!cmd) {
		err = -ENOMEM;
		goto done;
	}

	/* Continue with pairing via HCI */
	if (hci_op == HCI_OP_USER_PASSKEY_REPLY) {
		struct hci_cp_user_passkey_reply cp;

		bacpy(&cp.bdaddr, bdaddr);
		cp.passkey = passkey;
		err = hci_send_cmd(hdev, hci_op, sizeof(cp), &cp);
	} else
		err = hci_send_cmd(hdev, hci_op, sizeof(*bdaddr), bdaddr);

	if (err < 0)
		mgmt_pending_remove(cmd);

done:
	hci_dev_unlock(hdev);
	return err;
}

static int user_confirm_reply(struct sock *sk, struct hci_dev *hdev, void *data,
			      u16 len)
{
	struct mgmt_cp_user_confirm_reply *cp = data;
>>>>>>> remotes/linux2/linux-3.4.y

	BT_DBG("");

	if (len != sizeof(*cp))
<<<<<<< HEAD
		return cmd_status(sk, index, MGMT_OP_SET_CONNECTION_PARAMS,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_CONNECTION_PARAMS,
									ENODEV);

	hci_dev_lock_bh(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);
	if (!conn) {
		err = cmd_status(sk, index, MGMT_OP_SET_CONNECTION_PARAMS,
								ENOTCONN);
		goto failed;
	}

	hci_le_conn_update(conn, le16_to_cpu(cp->interval_min),
				le16_to_cpu(cp->interval_max),
				le16_to_cpu(cp->slave_latency),
				le16_to_cpu(cp->timeout_multiplier));

	err = cmd_status(sk, index, MGMT_OP_SET_CONNECTION_PARAMS, 0);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_rssi_reporter(struct sock *sk, u16 index,
				unsigned char *data, u16 len)
{
	struct mgmt_cp_set_rssi_reporter *cp = (void *) data;
	struct hci_dev *hdev;
	struct hci_conn *conn;
	int err = 0;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_SET_RSSI_REPORTER,
								EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_RSSI_REPORTER,
							ENODEV);

	hci_dev_lock_bh(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);

	if (!conn) {
		err = cmd_status(sk, index, MGMT_OP_SET_RSSI_REPORTER,
						ENOTCONN);
		goto failed;
	}

	BT_DBG("updateOnThreshExceed %d ", cp->updateOnThreshExceed);
	hci_conn_set_rssi_reporter(conn, cp->rssi_threshold,
			__le16_to_cpu(cp->interval), cp->updateOnThreshExceed);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int unset_rssi_reporter(struct sock *sk, u16 index,
			unsigned char *data, u16 len)
{
	struct mgmt_cp_unset_rssi_reporter *cp = (void *) data;
	struct hci_dev *hdev;
	struct hci_conn *conn;
	int err = 0;

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_UNSET_RSSI_REPORTER,
					EINVAL);

	hdev = hci_dev_get(index);

	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_UNSET_RSSI_REPORTER,
					ENODEV);

	hci_dev_lock_bh(hdev);

	conn = hci_conn_hash_lookup_ba(hdev, LE_LINK, &cp->bdaddr);

	if (!conn) {
		err = cmd_status(sk, index, MGMT_OP_UNSET_RSSI_REPORTER,
					ENOTCONN);
		goto failed;
	}

	hci_conn_unset_rssi_reporter(conn);

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static int set_local_name(struct sock *sk, u16 index, unsigned char *data,
								u16 len)
{
	struct mgmt_cp_set_local_name *mgmt_cp = (void *) data;
	struct hci_cp_write_local_name hci_cp;
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("");

	if (len != sizeof(*mgmt_cp))
		return cmd_status(sk, index, MGMT_OP_SET_LOCAL_NAME, EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_SET_LOCAL_NAME, ENODEV);

	hci_dev_lock_bh(hdev);

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_LOCAL_NAME, index, data, len);
=======
		return cmd_status(sk, hdev->id, MGMT_OP_USER_CONFIRM_REPLY,
				  MGMT_STATUS_INVALID_PARAMS);

	return user_pairing_resp(sk, hdev, &cp->addr.bdaddr, cp->addr.type,
				 MGMT_OP_USER_CONFIRM_REPLY,
				 HCI_OP_USER_CONFIRM_REPLY, 0);
}

static int user_confirm_neg_reply(struct sock *sk, struct hci_dev *hdev,
				  void *data, u16 len)
{
	struct mgmt_cp_user_confirm_neg_reply *cp = data;

	BT_DBG("");

	return user_pairing_resp(sk, hdev, &cp->addr.bdaddr, cp->addr.type,
				 MGMT_OP_USER_CONFIRM_NEG_REPLY,
				 HCI_OP_USER_CONFIRM_NEG_REPLY, 0);
}

static int user_passkey_reply(struct sock *sk, struct hci_dev *hdev, void *data,
			      u16 len)
{
	struct mgmt_cp_user_passkey_reply *cp = data;

	BT_DBG("");

	return user_pairing_resp(sk, hdev, &cp->addr.bdaddr, cp->addr.type,
				 MGMT_OP_USER_PASSKEY_REPLY,
				 HCI_OP_USER_PASSKEY_REPLY, cp->passkey);
}

static int user_passkey_neg_reply(struct sock *sk, struct hci_dev *hdev,
				  void *data, u16 len)
{
	struct mgmt_cp_user_passkey_neg_reply *cp = data;

	BT_DBG("");

	return user_pairing_resp(sk, hdev, &cp->addr.bdaddr, cp->addr.type,
				 MGMT_OP_USER_PASSKEY_NEG_REPLY,
				 HCI_OP_USER_PASSKEY_NEG_REPLY, 0);
}

static int update_name(struct hci_dev *hdev, const char *name)
{
	struct hci_cp_write_local_name cp;

	memcpy(cp.name, name, sizeof(cp.name));

	return hci_send_cmd(hdev, HCI_OP_WRITE_LOCAL_NAME, sizeof(cp), &cp);
}

static int set_local_name(struct sock *sk, struct hci_dev *hdev, void *data,
			  u16 len)
{
	struct mgmt_cp_set_local_name *cp = data;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("");

	hci_dev_lock(hdev);

	memcpy(hdev->short_name, cp->short_name, sizeof(hdev->short_name));

	if (!hdev_is_powered(hdev)) {
		memcpy(hdev->dev_name, cp->name, sizeof(hdev->dev_name));

		err = cmd_complete(sk, hdev->id, MGMT_OP_SET_LOCAL_NAME, 0,
				   data, len);
		if (err < 0)
			goto failed;

		err = mgmt_event(MGMT_EV_LOCAL_NAME_CHANGED, hdev, data, len,
				 sk);

		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_SET_LOCAL_NAME, hdev, data, len);
>>>>>>> remotes/linux2/linux-3.4.y
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

<<<<<<< HEAD
	memcpy(hci_cp.name, mgmt_cp->name, sizeof(hci_cp.name));
	err = hci_send_cmd(hdev, HCI_OP_WRITE_LOCAL_NAME, sizeof(hci_cp),
								&hci_cp);
=======
	err = update_name(hdev, cp->name);
>>>>>>> remotes/linux2/linux-3.4.y
	if (err < 0)
		mgmt_pending_remove(cmd);

failed:
<<<<<<< HEAD
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

static void discovery_rsp(struct pending_cmd *cmd, void *data)
{
	struct mgmt_mode ev;

	BT_DBG("");
	if (cmd->opcode == MGMT_OP_START_DISCOVERY) {
		ev.val = 1;
		cmd_status(cmd->sk, cmd->index, MGMT_OP_START_DISCOVERY, 0);
	} else {
		ev.val = 0;
		cmd_complete(cmd->sk, cmd->index, MGMT_OP_STOP_DISCOVERY,
								NULL, 0);
		if (cmd->opcode == MGMT_OP_STOP_DISCOVERY) {
			struct hci_dev *hdev = hci_dev_get(cmd->index);
			if (hdev) {
				del_timer(&hdev->disco_le_timer);
				del_timer(&hdev->disco_timer);
				hci_dev_put(hdev);
			}
		}
	}

	mgmt_event(MGMT_EV_DISCOVERING, cmd->index, &ev, sizeof(ev), NULL);

	list_del(&cmd->list);

	mgmt_pending_free(cmd);
}

void mgmt_inquiry_started(u16 index)
{
	BT_DBG("");
	mgmt_pending_foreach(MGMT_OP_START_DISCOVERY, index,
						discovery_rsp, NULL);
}

void mgmt_inquiry_complete_evt(u16 index, u8 status)
{
	struct hci_dev *hdev;
	struct hci_cp_le_set_scan_enable le_cp = {1, 0};
	struct mgmt_mode cp = {0};
	int err = -1;

	BT_DBG("");

	hdev = hci_dev_get(index);

	if (!hdev || !lmp_le_capable(hdev)) {

		mgmt_pending_foreach(MGMT_OP_STOP_DISCOVERY, index,
						discovery_terminated, NULL);

		mgmt_event(MGMT_EV_DISCOVERING, index, &cp, sizeof(cp), NULL);

		if (hdev)
			goto done;
		else
			return;
	}

	if (hdev->disco_state != SCAN_IDLE) {
		err = hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
						sizeof(le_cp), &le_cp);
		if (err >= 0) {
			mod_timer(&hdev->disco_le_timer, jiffies +
				msecs_to_jiffies(hdev->disco_int_phase * 1000));
			hdev->disco_state = SCAN_LE;
		} else
			hdev->disco_state = SCAN_IDLE;
	}

	if (hdev->disco_state == SCAN_IDLE)
		mgmt_event(MGMT_EV_DISCOVERING, index, &cp, sizeof(cp), NULL);

	if (err < 0)
		mgmt_pending_foreach(MGMT_OP_STOP_DISCOVERY, index,
						discovery_terminated, NULL);

done:
	hci_dev_put(hdev);
}

void mgmt_disco_timeout(unsigned long data)
{
	struct hci_dev *hdev = (void *) data;
	struct pending_cmd *cmd;
	struct mgmt_mode cp = {0};

	BT_DBG("hci%d", hdev->id);

	hdev = hci_dev_get(hdev->id);

	if (!hdev)
		return;

	hci_dev_lock_bh(hdev);
	del_timer(&hdev->disco_le_timer);

	if (hdev->disco_state != SCAN_IDLE) {
		struct hci_cp_le_set_scan_enable le_cp = {0, 0};

		if (test_bit(HCI_UP, &hdev->flags)) {
			if (hdev->disco_state == SCAN_LE)
				hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
							sizeof(le_cp), &le_cp);
			else
				hci_send_cmd(hdev, HCI_OP_INQUIRY_CANCEL, 0,
									 NULL);
		}
		hdev->disco_state = SCAN_IDLE;
	}

	mgmt_event(MGMT_EV_DISCOVERING, hdev->id, &cp, sizeof(cp), NULL);

	cmd = mgmt_pending_find(MGMT_OP_STOP_DISCOVERY, hdev->id);
	if (cmd)
		mgmt_pending_remove(cmd);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
}

void mgmt_disco_le_timeout(unsigned long data)
{
	struct hci_dev *hdev = (void *)data;
	struct hci_cp_le_set_scan_enable le_cp = {0, 0};

	BT_DBG("hci%d", hdev->id);

	hdev = hci_dev_get(hdev->id);

	if (!hdev)
		return;

	hci_dev_lock_bh(hdev);

	if (test_bit(HCI_UP, &hdev->flags)) {
		if (hdev->disco_state == SCAN_LE)
			hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
					sizeof(le_cp), &le_cp);

	/* re-start BR scan */
		if (hdev->disco_state != SCAN_IDLE) {
			struct hci_cp_inquiry cp = {{0x33, 0x8b, 0x9e}, 4, 0};
			hdev->disco_int_phase *= 2;
			hdev->disco_int_count = 0;
			cp.num_rsp = (u8) hdev->disco_int_phase;
			hci_send_cmd(hdev, HCI_OP_INQUIRY, sizeof(cp), &cp);
			hdev->disco_state = SCAN_BR;
		}
	}

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
}

static int start_discovery(struct sock *sk, u16 index)
{
	struct hci_cp_inquiry cp = {{0x33, 0x8b, 0x9e}, 8, 0};
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_START_DISCOVERY, ENODEV);

	hci_dev_lock_bh(hdev);

	if (hdev->disco_state && timer_pending(&hdev->disco_timer)) {
		err = -EBUSY;
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_START_DISCOVERY, index, NULL, 0);
=======
	hci_dev_unlock(hdev);
	return err;
}

static int read_local_oob_data(struct sock *sk, struct hci_dev *hdev,
			       void *data, u16 data_len)
{
	struct pending_cmd *cmd;
	int err;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_READ_LOCAL_OOB_DATA,
				 MGMT_STATUS_NOT_POWERED);
		goto unlock;
	}

	if (!(hdev->features[6] & LMP_SIMPLE_PAIR)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_READ_LOCAL_OOB_DATA,
				 MGMT_STATUS_NOT_SUPPORTED);
		goto unlock;
	}

	if (mgmt_pending_find(MGMT_OP_READ_LOCAL_OOB_DATA, hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_READ_LOCAL_OOB_DATA,
				 MGMT_STATUS_BUSY);
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_READ_LOCAL_OOB_DATA, hdev, NULL, 0);
	if (!cmd) {
		err = -ENOMEM;
		goto unlock;
	}

	err = hci_send_cmd(hdev, HCI_OP_READ_LOCAL_OOB_DATA, 0, NULL);
	if (err < 0)
		mgmt_pending_remove(cmd);

unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int add_remote_oob_data(struct sock *sk, struct hci_dev *hdev,
			       void *data, u16 len)
{
	struct mgmt_cp_add_remote_oob_data *cp = data;
	u8 status;
	int err;

	BT_DBG("%s ", hdev->name);

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_ADD_REMOTE_OOB_DATA,
				   MGMT_STATUS_NOT_POWERED, &cp->addr,
				   sizeof(cp->addr));
		goto unlock;
	}

	err = hci_add_remote_oob_data(hdev, &cp->addr.bdaddr, cp->hash,
				      cp->randomizer);
	if (err < 0)
		status = MGMT_STATUS_FAILED;
	else
		status = 0;

	err = cmd_complete(sk, hdev->id, MGMT_OP_ADD_REMOTE_OOB_DATA, status,
			   &cp->addr, sizeof(cp->addr));

unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int remove_remote_oob_data(struct sock *sk, struct hci_dev *hdev,
						void *data, u16 len)
{
	struct mgmt_cp_remove_remote_oob_data *cp = data;
	u8 status;
	int err;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_complete(sk, hdev->id,
				   MGMT_OP_REMOVE_REMOTE_OOB_DATA,
				   MGMT_STATUS_NOT_POWERED, &cp->addr,
				   sizeof(cp->addr));
		goto unlock;
	}

	err = hci_remove_remote_oob_data(hdev, &cp->addr.bdaddr);
	if (err < 0)
		status = MGMT_STATUS_INVALID_PARAMS;
	else
		status = 0;

	err = cmd_complete(sk, hdev->id, MGMT_OP_REMOVE_REMOTE_OOB_DATA,
			   status, &cp->addr, sizeof(cp->addr));

unlock:
	hci_dev_unlock(hdev);
	return err;
}

int mgmt_interleaved_discovery(struct hci_dev *hdev)
{
	int err;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	err = hci_do_inquiry(hdev, INQUIRY_LEN_BREDR_LE);
	if (err < 0)
		hci_discovery_set_state(hdev, DISCOVERY_STOPPED);

	hci_dev_unlock(hdev);

	return err;
}

static int start_discovery(struct sock *sk, struct hci_dev *hdev,
			   void *data, u16 len)
{
	struct mgmt_cp_start_discovery *cp = data;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_START_DISCOVERY,
				 MGMT_STATUS_NOT_POWERED);
		goto failed;
	}

	if (hdev->discovery.state != DISCOVERY_STOPPED) {
		err = cmd_status(sk, hdev->id, MGMT_OP_START_DISCOVERY,
				 MGMT_STATUS_BUSY);
		goto failed;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_START_DISCOVERY, hdev, NULL, 0);
>>>>>>> remotes/linux2/linux-3.4.y
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

<<<<<<< HEAD
	/* If LE Capable, we will alternate between BR/EDR and LE */
	if (lmp_le_capable(hdev)) {
		struct hci_cp_le_set_scan_parameters le_cp;

		/* Shorten BR scan params */
		cp.num_rsp = 1;
		cp.length /= 2;

		/* Setup LE scan params */
		memset(&le_cp, 0, sizeof(le_cp));
		le_cp.type = 0x01;		/* Active scanning */
		/* The recommended value for scan interval and window is
		 * 11.25 msec. It is calculated by: time = n * 0.625 msec */
		le_cp.interval = cpu_to_le16(0x0012);
		le_cp.window = cpu_to_le16(0x0012);
		le_cp.own_bdaddr_type = 0;	/* Public address */
		le_cp.filter = 0;		/* Accept all adv packets */

		hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_PARAMETERS,
						sizeof(le_cp), &le_cp);
	}

	err = hci_send_cmd(hdev, HCI_OP_INQUIRY, sizeof(cp), &cp);

	if (err < 0) {
		mgmt_pending_remove(cmd);
		hdev->disco_state = SCAN_IDLE;
	} else if (lmp_le_capable(hdev)) {
		cmd = mgmt_pending_find(MGMT_OP_STOP_DISCOVERY, index);
		if (!cmd)
			mgmt_pending_add(sk, MGMT_OP_STOP_DISCOVERY, index,
								NULL, 0);
		hdev->disco_int_phase = 1;
		hdev->disco_int_count = 0;
		hdev->disco_state = SCAN_BR;
		del_timer(&hdev->disco_le_timer);
		del_timer(&hdev->disco_timer);
		mod_timer(&hdev->disco_timer,
				jiffies + msecs_to_jiffies(20000));
	} else
		hdev->disco_state = SCAN_BR;

failed:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	if (err < 0)
		return cmd_status(sk, index, MGMT_OP_START_DISCOVERY, -err);

	return err;
}

static int stop_discovery(struct sock *sk, u16 index)
{
	struct hci_cp_le_set_scan_enable le_cp = {0, 0};
	struct mgmt_mode mode_cp = {0};
	struct hci_dev *hdev;
	struct pending_cmd *cmd = NULL;
	int err = -EPERM;
	u8 state;

	BT_DBG("");

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_STOP_DISCOVERY, ENODEV);

	hci_dev_lock_bh(hdev);

	state = hdev->disco_state;
	hdev->disco_state = SCAN_IDLE;
	del_timer(&hdev->disco_le_timer);
	del_timer(&hdev->disco_timer);

	if (state == SCAN_LE) {
		err = hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
							sizeof(le_cp), &le_cp);
		if (err >= 0) {
			mgmt_pending_foreach(MGMT_OP_STOP_DISCOVERY, index,
						discovery_terminated, NULL);

			err = cmd_complete(sk, index, MGMT_OP_STOP_DISCOVERY,
								NULL, 0);
		}
	} else if (state == SCAN_BR)
		err = hci_send_cmd(hdev, HCI_OP_INQUIRY_CANCEL, 0, NULL);

	cmd = mgmt_pending_find(MGMT_OP_STOP_DISCOVERY, index);
	if (err < 0 && cmd)
		mgmt_pending_remove(cmd);

	mgmt_event(MGMT_EV_DISCOVERING, index, &mode_cp, sizeof(mode_cp), NULL);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	if (err < 0)
		return cmd_status(sk, index, MGMT_OP_STOP_DISCOVERY, -err);
	else
		return err;
}

static int read_local_oob_data(struct sock *sk, u16 index)
{
	struct hci_dev *hdev;
	struct pending_cmd *cmd;
	int err;

	BT_DBG("hci%u", index);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
									ENODEV);

	hci_dev_lock_bh(hdev);

	if (!test_bit(HCI_UP, &hdev->flags)) {
		err = cmd_status(sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
								ENETDOWN);
		goto unlock;
	}

	if (!(hdev->features[6] & LMP_SIMPLE_PAIR)) {
		err = cmd_status(sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
								EOPNOTSUPP);
		goto unlock;
	}

	if (mgmt_pending_find(MGMT_OP_READ_LOCAL_OOB_DATA, index)) {
		err = cmd_status(sk, index, MGMT_OP_READ_LOCAL_OOB_DATA, EBUSY);
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_READ_LOCAL_OOB_DATA, index, NULL, 0);
	if (!cmd) {
		err = -ENOMEM;
		goto unlock;
	}

	err = hci_send_cmd(hdev, HCI_OP_READ_LOCAL_OOB_DATA, 0, NULL);
	if (err < 0)
		mgmt_pending_remove(cmd);

unlock:
	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
=======
	hdev->discovery.type = cp->type;

	switch (hdev->discovery.type) {
	case DISCOV_TYPE_BREDR:
		if (lmp_bredr_capable(hdev))
			err = hci_do_inquiry(hdev, INQUIRY_LEN_BREDR);
		else
			err = -ENOTSUPP;
		break;

	case DISCOV_TYPE_LE:
		if (lmp_host_le_capable(hdev))
			err = hci_le_scan(hdev, LE_SCAN_TYPE, LE_SCAN_INT,
					  LE_SCAN_WIN, LE_SCAN_TIMEOUT_LE_ONLY);
		else
			err = -ENOTSUPP;
		break;

	case DISCOV_TYPE_INTERLEAVED:
		if (lmp_host_le_capable(hdev) && lmp_bredr_capable(hdev))
			err = hci_le_scan(hdev, LE_SCAN_TYPE, LE_SCAN_INT,
					  LE_SCAN_WIN,
					  LE_SCAN_TIMEOUT_BREDR_LE);
		else
			err = -ENOTSUPP;
		break;

	default:
		err = -EINVAL;
	}

	if (err < 0)
		mgmt_pending_remove(cmd);
	else
		hci_discovery_set_state(hdev, DISCOVERY_STARTING);

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int stop_discovery(struct sock *sk, struct hci_dev *hdev, void *data,
			  u16 len)
{
	struct mgmt_cp_stop_discovery *mgmt_cp = data;
	struct pending_cmd *cmd;
	struct hci_cp_remote_name_req_cancel cp;
	struct inquiry_entry *e;
	int err;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	if (!hci_discovery_active(hdev)) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_STOP_DISCOVERY,
				   MGMT_STATUS_REJECTED, &mgmt_cp->type,
				   sizeof(mgmt_cp->type));
		goto unlock;
	}

	if (hdev->discovery.type != mgmt_cp->type) {
		err = cmd_complete(sk, hdev->id, MGMT_OP_STOP_DISCOVERY,
				   MGMT_STATUS_INVALID_PARAMS, &mgmt_cp->type,
				   sizeof(mgmt_cp->type));
		goto unlock;
	}

	cmd = mgmt_pending_add(sk, MGMT_OP_STOP_DISCOVERY, hdev, NULL, 0);
	if (!cmd) {
		err = -ENOMEM;
		goto unlock;
	}

	if (hdev->discovery.state == DISCOVERY_FINDING) {
		err = hci_cancel_inquiry(hdev);
		if (err < 0)
			mgmt_pending_remove(cmd);
		else
			hci_discovery_set_state(hdev, DISCOVERY_STOPPING);
		goto unlock;
	}

	e = hci_inquiry_cache_lookup_resolve(hdev, BDADDR_ANY, NAME_PENDING);
	if (!e) {
		mgmt_pending_remove(cmd);
		err = cmd_complete(sk, hdev->id, MGMT_OP_STOP_DISCOVERY, 0,
				   &mgmt_cp->type, sizeof(mgmt_cp->type));
		hci_discovery_set_state(hdev, DISCOVERY_STOPPED);
		goto unlock;
	}

	bacpy(&cp.bdaddr, &e->data.bdaddr);
	err = hci_send_cmd(hdev, HCI_OP_REMOTE_NAME_REQ_CANCEL, sizeof(cp),
			   &cp);
	if (err < 0)
		mgmt_pending_remove(cmd);
	else
		hci_discovery_set_state(hdev, DISCOVERY_STOPPING);

unlock:
	hci_dev_unlock(hdev);
	return err;
}

static int confirm_name(struct sock *sk, struct hci_dev *hdev, void *data,
			u16 len)
{
	struct mgmt_cp_confirm_name *cp = data;
	struct inquiry_entry *e;
	int err;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	if (!hci_discovery_active(hdev)) {
		err = cmd_status(sk, hdev->id, MGMT_OP_CONFIRM_NAME,
				 MGMT_STATUS_FAILED);
		goto failed;
	}

	e = hci_inquiry_cache_lookup_unknown(hdev, &cp->addr.bdaddr);
	if (!e) {
		err = cmd_status(sk, hdev->id, MGMT_OP_CONFIRM_NAME,
				 MGMT_STATUS_INVALID_PARAMS);
		goto failed;
	}

	if (cp->name_known) {
		e->name_state = NAME_KNOWN;
		list_del(&e->list);
	} else {
		e->name_state = NAME_NEEDED;
		hci_inquiry_cache_update_resolve(hdev, e);
	}

	err = 0;

failed:
	hci_dev_unlock(hdev);
	return err;
}

static int block_device(struct sock *sk, struct hci_dev *hdev, void *data,
			u16 len)
{
	struct mgmt_cp_block_device *cp = data;
	u8 status;
	int err;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	err = hci_blacklist_add(hdev, &cp->addr.bdaddr, cp->addr.type);
	if (err < 0)
		status = MGMT_STATUS_FAILED;
	else
		status = 0;

	err = cmd_complete(sk, hdev->id, MGMT_OP_BLOCK_DEVICE, status,
			   &cp->addr, sizeof(cp->addr));

	hci_dev_unlock(hdev);
>>>>>>> remotes/linux2/linux-3.4.y

	return err;
}

<<<<<<< HEAD
static int add_remote_oob_data(struct sock *sk, u16 index, unsigned char *data,
									u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_add_remote_oob_data *cp = (void *) data;
	int err;

	BT_DBG("hci%u ", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_ADD_REMOTE_OOB_DATA,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_ADD_REMOTE_OOB_DATA,
									ENODEV);

	hci_dev_lock_bh(hdev);

	err = hci_add_remote_oob_data(hdev, &cp->bdaddr, cp->hash,
								cp->randomizer);
	if (err < 0)
		err = cmd_status(sk, index, MGMT_OP_ADD_REMOTE_OOB_DATA, -err);
	else
		err = cmd_complete(sk, index, MGMT_OP_ADD_REMOTE_OOB_DATA, NULL,
									0);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);
=======
static int unblock_device(struct sock *sk, struct hci_dev *hdev, void *data,
			  u16 len)
{
	struct mgmt_cp_unblock_device *cp = data;
	u8 status;
	int err;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	err = hci_blacklist_del(hdev, &cp->addr.bdaddr, cp->addr.type);
	if (err < 0)
		status = MGMT_STATUS_INVALID_PARAMS;
	else
		status = 0;

	err = cmd_complete(sk, hdev->id, MGMT_OP_UNBLOCK_DEVICE, status,
			   &cp->addr, sizeof(cp->addr));

	hci_dev_unlock(hdev);
>>>>>>> remotes/linux2/linux-3.4.y

	return err;
}

<<<<<<< HEAD
static int remove_remote_oob_data(struct sock *sk, u16 index,
						unsigned char *data, u16 len)
{
	struct hci_dev *hdev;
	struct mgmt_cp_remove_remote_oob_data *cp = (void *) data;
	int err;

	BT_DBG("hci%u ", index);

	if (len != sizeof(*cp))
		return cmd_status(sk, index, MGMT_OP_REMOVE_REMOTE_OOB_DATA,
									EINVAL);

	hdev = hci_dev_get(index);
	if (!hdev)
		return cmd_status(sk, index, MGMT_OP_REMOVE_REMOTE_OOB_DATA,
									ENODEV);

	hci_dev_lock_bh(hdev);

	err = hci_remove_remote_oob_data(hdev, &cp->bdaddr);
	if (err < 0)
		err = cmd_status(sk, index, MGMT_OP_REMOVE_REMOTE_OOB_DATA,
									-err);
	else
		err = cmd_complete(sk, index, MGMT_OP_REMOVE_REMOTE_OOB_DATA,
								NULL, 0);

	hci_dev_unlock_bh(hdev);
	hci_dev_put(hdev);

	return err;
}

int mgmt_control(struct sock *sk, struct msghdr *msg, size_t msglen)
{
	unsigned char *buf;
	struct mgmt_hdr *hdr;
	u16 opcode, index, len;
=======
static int set_fast_connectable(struct sock *sk, struct hci_dev *hdev,
				void *data, u16 len)
{
	struct mgmt_mode *cp = data;
	struct hci_cp_write_page_scan_activity acp;
	u8 type;
	int err;

	BT_DBG("%s", hdev->name);

	if (!hdev_is_powered(hdev))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				  MGMT_STATUS_NOT_POWERED);

	if (!test_bit(HCI_CONNECTABLE, &hdev->dev_flags))
		return cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				  MGMT_STATUS_REJECTED);

	hci_dev_lock(hdev);

	if (cp->val) {
		type = PAGE_SCAN_TYPE_INTERLACED;

		/* 22.5 msec page scan interval */
		acp.interval = __constant_cpu_to_le16(0x0024);
	} else {
		type = PAGE_SCAN_TYPE_STANDARD;	/* default */

		/* default 1.28 sec page scan */
		acp.interval = __constant_cpu_to_le16(0x0800);
	}

	/* default 11.25 msec page scan window */
	acp.window = __constant_cpu_to_le16(0x0012);

	err = hci_send_cmd(hdev, HCI_OP_WRITE_PAGE_SCAN_ACTIVITY, sizeof(acp),
			   &acp);
	if (err < 0) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				 MGMT_STATUS_FAILED);
		goto done;
	}

	err = hci_send_cmd(hdev, HCI_OP_WRITE_PAGE_SCAN_TYPE, 1, &type);
	if (err < 0) {
		err = cmd_status(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE,
				 MGMT_STATUS_FAILED);
		goto done;
	}

	err = cmd_complete(sk, hdev->id, MGMT_OP_SET_FAST_CONNECTABLE, 0,
			   NULL, 0);
done:
	hci_dev_unlock(hdev);
	return err;
}

static int load_long_term_keys(struct sock *sk, struct hci_dev *hdev,
			       void *cp_data, u16 len)
{
	struct mgmt_cp_load_long_term_keys *cp = cp_data;
	u16 key_count, expected_len;
	int i;

	key_count = get_unaligned_le16(&cp->key_count);

	expected_len = sizeof(*cp) + key_count *
					sizeof(struct mgmt_ltk_info);
	if (expected_len != len) {
		BT_ERR("load_keys: expected %u bytes, got %u bytes",
							len, expected_len);
		return cmd_status(sk, hdev->id, MGMT_OP_LOAD_LONG_TERM_KEYS,
				  EINVAL);
	}

	BT_DBG("%s key_count %u", hdev->name, key_count);

	hci_dev_lock(hdev);

	hci_smp_ltks_clear(hdev);

	for (i = 0; i < key_count; i++) {
		struct mgmt_ltk_info *key = &cp->keys[i];
		u8 type;

		if (key->master)
			type = HCI_SMP_LTK;
		else
			type = HCI_SMP_LTK_SLAVE;

		hci_add_ltk(hdev, &key->addr.bdaddr, key->addr.type,
			    type, 0, key->authenticated, key->val,
			    key->enc_size, key->ediv, key->rand);
	}

	hci_dev_unlock(hdev);

	return 0;
}

struct mgmt_handler {
	int (*func) (struct sock *sk, struct hci_dev *hdev, void *data,
		     u16 data_len);
	bool var_len;
	size_t data_len;
} mgmt_handlers[] = {
	{ NULL }, /* 0x0000 (no command) */
	{ read_version,           false, MGMT_READ_VERSION_SIZE },
	{ read_commands,          false, MGMT_READ_COMMANDS_SIZE },
	{ read_index_list,        false, MGMT_READ_INDEX_LIST_SIZE },
	{ read_controller_info,   false, MGMT_READ_INFO_SIZE },
	{ set_powered,            false, MGMT_SETTING_SIZE },
	{ set_discoverable,       false, MGMT_SET_DISCOVERABLE_SIZE },
	{ set_connectable,        false, MGMT_SETTING_SIZE },
	{ set_fast_connectable,   false, MGMT_SETTING_SIZE },
	{ set_pairable,           false, MGMT_SETTING_SIZE },
	{ set_link_security,      false, MGMT_SETTING_SIZE },
	{ set_ssp,                false, MGMT_SETTING_SIZE },
	{ set_hs,                 false, MGMT_SETTING_SIZE },
	{ set_le,                 false, MGMT_SETTING_SIZE },
	{ set_dev_class,          false, MGMT_SET_DEV_CLASS_SIZE },
	{ set_local_name,         false, MGMT_SET_LOCAL_NAME_SIZE },
	{ add_uuid,               false, MGMT_ADD_UUID_SIZE },
	{ remove_uuid,            false, MGMT_REMOVE_UUID_SIZE },
	{ load_link_keys,         true,  MGMT_LOAD_LINK_KEYS_SIZE },
	{ load_long_term_keys,    true,  MGMT_LOAD_LONG_TERM_KEYS_SIZE },
	{ disconnect,             false, MGMT_DISCONNECT_SIZE },
	{ get_connections,        false, MGMT_GET_CONNECTIONS_SIZE },
	{ pin_code_reply,         false, MGMT_PIN_CODE_REPLY_SIZE },
	{ pin_code_neg_reply,     false, MGMT_PIN_CODE_NEG_REPLY_SIZE },
	{ set_io_capability,      false, MGMT_SET_IO_CAPABILITY_SIZE },
	{ pair_device,            false, MGMT_PAIR_DEVICE_SIZE },
	{ cancel_pair_device,     false, MGMT_CANCEL_PAIR_DEVICE_SIZE },
	{ unpair_device,          false, MGMT_UNPAIR_DEVICE_SIZE },
	{ user_confirm_reply,     false, MGMT_USER_CONFIRM_REPLY_SIZE },
	{ user_confirm_neg_reply, false, MGMT_USER_CONFIRM_NEG_REPLY_SIZE },
	{ user_passkey_reply,     false, MGMT_USER_PASSKEY_REPLY_SIZE },
	{ user_passkey_neg_reply, false, MGMT_USER_PASSKEY_NEG_REPLY_SIZE },
	{ read_local_oob_data,    false, MGMT_READ_LOCAL_OOB_DATA_SIZE },
	{ add_remote_oob_data,    false, MGMT_ADD_REMOTE_OOB_DATA_SIZE },
	{ remove_remote_oob_data, false, MGMT_REMOVE_REMOTE_OOB_DATA_SIZE },
	{ start_discovery,        false, MGMT_START_DISCOVERY_SIZE },
	{ stop_discovery,         false, MGMT_STOP_DISCOVERY_SIZE },
	{ confirm_name,           false, MGMT_CONFIRM_NAME_SIZE },
	{ block_device,           false, MGMT_BLOCK_DEVICE_SIZE },
	{ unblock_device,         false, MGMT_UNBLOCK_DEVICE_SIZE },
};


int mgmt_control(struct sock *sk, struct msghdr *msg, size_t msglen)
{
	void *buf;
	u8 *cp;
	struct mgmt_hdr *hdr;
	u16 opcode, index, len;
	struct hci_dev *hdev = NULL;
	struct mgmt_handler *handler;
>>>>>>> remotes/linux2/linux-3.4.y
	int err;

	BT_DBG("got %zu bytes", msglen);

	if (msglen < sizeof(*hdr))
		return -EINVAL;

	buf = kmalloc(msglen, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	if (memcpy_fromiovec(buf, msg->msg_iov, msglen)) {
		err = -EFAULT;
		goto done;
	}

<<<<<<< HEAD
	hdr = (struct mgmt_hdr *) buf;
=======
	hdr = buf;
>>>>>>> remotes/linux2/linux-3.4.y
	opcode = get_unaligned_le16(&hdr->opcode);
	index = get_unaligned_le16(&hdr->index);
	len = get_unaligned_le16(&hdr->len);

	if (len != msglen - sizeof(*hdr)) {
		err = -EINVAL;
		goto done;
	}

<<<<<<< HEAD
	BT_DBG("got opcode %x", opcode);
	switch (opcode) {
	case MGMT_OP_READ_VERSION:
		err = read_version(sk);
		break;
	case MGMT_OP_READ_INDEX_LIST:
		err = read_index_list(sk);
		break;
	case MGMT_OP_READ_INFO:
		err = read_controller_info(sk, index);
		break;
	case MGMT_OP_SET_POWERED:
		err = set_powered(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_DISCOVERABLE:
		err = set_discoverable(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_LIMIT_DISCOVERABLE:
		err = set_limited_discoverable(sk, index, buf + sizeof(*hdr),
									len);
		break;
	case MGMT_OP_SET_CONNECTABLE:
		err = set_connectable(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_PAIRABLE:
		err = set_pairable(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_ADD_UUID:
		err = add_uuid(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_REMOVE_UUID:
		err = remove_uuid(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_DEV_CLASS:
		err = set_dev_class(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_SERVICE_CACHE:
		err = set_service_cache(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_LOAD_KEYS:
		err = load_keys(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_REMOVE_KEY:
		err = remove_key(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_DISCONNECT:
		err = disconnect(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_GET_CONNECTIONS:
		err = get_connections(sk, index);
		break;
	case MGMT_OP_PIN_CODE_REPLY:
		err = pin_code_reply(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_PIN_CODE_NEG_REPLY:
		err = pin_code_neg_reply(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_IO_CAPABILITY:
		err = set_io_capability(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_PAIR_DEVICE:
		err = pair_device(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_USER_CONFIRM_REPLY:
	case MGMT_OP_USER_PASSKEY_REPLY:
	case MGMT_OP_USER_CONFIRM_NEG_REPLY:
		err = user_confirm_reply(sk, index, buf + sizeof(*hdr),
								len, opcode);
		break;
	case MGMT_OP_SET_LOCAL_NAME:
		err = set_local_name(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_START_DISCOVERY:
		err = start_discovery(sk, index);
		break;
	case MGMT_OP_STOP_DISCOVERY:
		err = stop_discovery(sk, index);
		break;
	case MGMT_OP_RESOLVE_NAME:
		err = resolve_name(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_CONNECTION_PARAMS:
		err = set_connection_params(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_SET_RSSI_REPORTER:
		err = set_rssi_reporter(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_UNSET_RSSI_REPORTER:
		err = unset_rssi_reporter(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_READ_LOCAL_OOB_DATA:
		err = read_local_oob_data(sk, index);
		break;
	case MGMT_OP_ADD_REMOTE_OOB_DATA:
		err = add_remote_oob_data(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_REMOVE_REMOTE_OOB_DATA:
		err = remove_remote_oob_data(sk, index, buf + sizeof(*hdr),
									len);
		break;
	case MGMT_OP_ENCRYPT_LINK:
		err = encrypt_link(sk, index, buf + sizeof(*hdr), len);
		break;
	case MGMT_OP_LE_ADD_DEV_WHITE_LIST:
		err = le_add_dev_white_list(sk, index, buf + sizeof(*hdr),
									len);
		break;
	case MGMT_OP_LE_REMOVE_DEV_WHITE_LIST:
		err = le_remove_dev_white_list(sk, index, buf + sizeof(*hdr),
									len);
		break;
	case MGMT_OP_LE_CLEAR_WHITE_LIST:
		err = le_clear_white_list(sk, index);
		break;
	case MGMT_OP_LE_CREATE_CONN_WHITE_LIST:
		err = le_create_conn_white_list(sk, index);
		break;
	case MGMT_OP_LE_CANCEL_CREATE_CONN_WHITE_LIST:
		err = le_cancel_create_conn_white_list(sk, index);
		break;
	default:
		BT_DBG("Unknown op %u", opcode);
		err = cmd_status(sk, index, opcode, 0x01);
		break;
	}

=======
	if (index != MGMT_INDEX_NONE) {
		hdev = hci_dev_get(index);
		if (!hdev) {
			err = cmd_status(sk, index, opcode,
					 MGMT_STATUS_INVALID_INDEX);
			goto done;
		}
	}

	if (opcode >= ARRAY_SIZE(mgmt_handlers) ||
					mgmt_handlers[opcode].func == NULL) {
		BT_DBG("Unknown op %u", opcode);
		err = cmd_status(sk, index, opcode,
				 MGMT_STATUS_UNKNOWN_COMMAND);
		goto done;
	}

	if ((hdev && opcode < MGMT_OP_READ_INFO) ||
			(!hdev && opcode >= MGMT_OP_READ_INFO)) {
		err = cmd_status(sk, index, opcode,
				 MGMT_STATUS_INVALID_INDEX);
		goto done;
	}

	handler = &mgmt_handlers[opcode];

	if ((handler->var_len && len < handler->data_len) ||
			(!handler->var_len && len != handler->data_len)) {
		err = cmd_status(sk, index, opcode,
				 MGMT_STATUS_INVALID_PARAMS);
		goto done;
	}

	if (hdev)
		mgmt_init_hdev(sk, hdev);

	cp = buf + sizeof(*hdr);

	err = handler->func(sk, hdev, cp, len);
>>>>>>> remotes/linux2/linux-3.4.y
	if (err < 0)
		goto done;

	err = msglen;

done:
<<<<<<< HEAD
=======
	if (hdev)
		hci_dev_put(hdev);

>>>>>>> remotes/linux2/linux-3.4.y
	kfree(buf);
	return err;
}

static void cmd_status_rsp(struct pending_cmd *cmd, void *data)
{
	u8 *status = data;

	cmd_status(cmd->sk, cmd->index, cmd->opcode, *status);
	mgmt_pending_remove(cmd);
}

<<<<<<< HEAD
int mgmt_index_added(u16 index)
{
	BT_DBG("%d", index);
	return mgmt_event(MGMT_EV_INDEX_ADDED, index, NULL, 0, NULL);
}

int mgmt_index_removed(u16 index)
{
	u8 status = ENODEV;

	BT_DBG("%d", index);

	mgmt_pending_foreach(0, index, cmd_status_rsp, &status);

	return mgmt_event(MGMT_EV_INDEX_REMOVED, index, NULL, 0, NULL);
}

struct cmd_lookup {
	u8 val;
	struct sock *sk;
};

static void mode_rsp(struct pending_cmd *cmd, void *data)
{
	struct mgmt_mode *cp = cmd->param;
	struct cmd_lookup *match = data;

	if (cp->val != match->val)
		return;

	send_mode_rsp(cmd->sk, cmd->opcode, cmd->index, cp->val);
=======
int mgmt_index_added(struct hci_dev *hdev)
{
	return mgmt_event(MGMT_EV_INDEX_ADDED, hdev, NULL, 0, NULL);
}

int mgmt_index_removed(struct hci_dev *hdev)
{
	u8 status = MGMT_STATUS_INVALID_INDEX;

	mgmt_pending_foreach(0, hdev, cmd_status_rsp, &status);

	return mgmt_event(MGMT_EV_INDEX_REMOVED, hdev, NULL, 0, NULL);
}

struct cmd_lookup {
	struct sock *sk;
	struct hci_dev *hdev;
	u8 mgmt_status;
};

static void settings_rsp(struct pending_cmd *cmd, void *data)
{
	struct cmd_lookup *match = data;

	send_settings_rsp(cmd->sk, cmd->opcode, match->hdev);
>>>>>>> remotes/linux2/linux-3.4.y

	list_del(&cmd->list);

	if (match->sk == NULL) {
		match->sk = cmd->sk;
		sock_hold(match->sk);
	}

	mgmt_pending_free(cmd);
}

<<<<<<< HEAD
int mgmt_powered(u16 index, u8 powered)
{
	struct mgmt_mode ev;
	struct cmd_lookup match = { powered, NULL };
	int ret;

	BT_DBG("hci%u %d", index, powered);

	mgmt_pending_foreach(MGMT_OP_SET_POWERED, index, mode_rsp, &match);

	if (!powered) {
		u8 status = ENETDOWN;
		mgmt_pending_foreach(0, index, cmd_status_rsp, &status);
	}

	ev.val = powered;

	ret = mgmt_event(MGMT_EV_POWERED, index, &ev, sizeof(ev), match.sk);
=======
int mgmt_powered(struct hci_dev *hdev, u8 powered)
{
	struct cmd_lookup match = { NULL, hdev };
	int err;

	if (!test_bit(HCI_MGMT, &hdev->dev_flags))
		return 0;

	mgmt_pending_foreach(MGMT_OP_SET_POWERED, hdev, settings_rsp, &match);

	if (powered) {
		u8 scan = 0;

		if (test_bit(HCI_CONNECTABLE, &hdev->dev_flags))
			scan |= SCAN_PAGE;
		if (test_bit(HCI_DISCOVERABLE, &hdev->dev_flags))
			scan |= SCAN_INQUIRY;

		if (scan)
			hci_send_cmd(hdev, HCI_OP_WRITE_SCAN_ENABLE, 1, &scan);

		if (test_bit(HCI_SSP_ENABLED, &hdev->dev_flags)) {
			u8 ssp = 1;

			hci_send_cmd(hdev, HCI_OP_WRITE_SSP_MODE, 1, &ssp);
		}

		if (test_bit(HCI_LE_ENABLED, &hdev->dev_flags)) {
			struct hci_cp_write_le_host_supported cp;

			cp.le = 1;
			cp.simul = !!(hdev->features[6] & LMP_SIMUL_LE_BR);

			hci_send_cmd(hdev, HCI_OP_WRITE_LE_HOST_SUPPORTED,
				     sizeof(cp), &cp);
		}

		update_class(hdev);
		update_name(hdev, hdev->dev_name);
		update_eir(hdev);
	} else {
		u8 status = MGMT_STATUS_NOT_POWERED;
		mgmt_pending_foreach(0, hdev, cmd_status_rsp, &status);
	}

	err = new_settings(hdev, match.sk);
>>>>>>> remotes/linux2/linux-3.4.y

	if (match.sk)
		sock_put(match.sk);

<<<<<<< HEAD
	return ret;
}

int mgmt_discoverable(u16 index, u8 discoverable)
{
	struct mgmt_mode ev;
	struct cmd_lookup match = { discoverable, NULL };
	int ret;

	mgmt_pending_foreach(MGMT_OP_SET_DISCOVERABLE, index, mode_rsp, &match);

	ev.val = discoverable;

	ret = mgmt_event(MGMT_EV_DISCOVERABLE, index, &ev, sizeof(ev),
								match.sk);
=======
	return err;
}

int mgmt_discoverable(struct hci_dev *hdev, u8 discoverable)
{
	struct cmd_lookup match = { NULL, hdev };
	bool changed = false;
	int err = 0;

	if (discoverable) {
		if (!test_and_set_bit(HCI_DISCOVERABLE, &hdev->dev_flags))
			changed = true;
	} else {
		if (test_and_clear_bit(HCI_DISCOVERABLE, &hdev->dev_flags))
			changed = true;
	}

	mgmt_pending_foreach(MGMT_OP_SET_DISCOVERABLE, hdev, settings_rsp,
			     &match);

	if (changed)
		err = new_settings(hdev, match.sk);
>>>>>>> remotes/linux2/linux-3.4.y

	if (match.sk)
		sock_put(match.sk);

<<<<<<< HEAD
	return ret;
}

int mgmt_connectable(u16 index, u8 connectable)
{
	struct mgmt_mode ev;
	struct cmd_lookup match = { connectable, NULL };
	int ret;

	mgmt_pending_foreach(MGMT_OP_SET_CONNECTABLE, index, mode_rsp, &match);

	ev.val = connectable;

	ret = mgmt_event(MGMT_EV_CONNECTABLE, index, &ev, sizeof(ev), match.sk);
=======
	return err;
}

int mgmt_connectable(struct hci_dev *hdev, u8 connectable)
{
	struct cmd_lookup match = { NULL, hdev };
	bool changed = false;
	int err = 0;

	if (connectable) {
		if (!test_and_set_bit(HCI_CONNECTABLE, &hdev->dev_flags))
			changed = true;
	} else {
		if (test_and_clear_bit(HCI_CONNECTABLE, &hdev->dev_flags))
			changed = true;
	}

	mgmt_pending_foreach(MGMT_OP_SET_CONNECTABLE, hdev, settings_rsp,
			     &match);

	if (changed)
		err = new_settings(hdev, match.sk);
>>>>>>> remotes/linux2/linux-3.4.y

	if (match.sk)
		sock_put(match.sk);

<<<<<<< HEAD
	return ret;
}

int mgmt_new_key(u16 index, struct link_key *key, u8 bonded)
{
	struct mgmt_ev_new_key *ev;
	int err, total;

	total = sizeof(struct mgmt_ev_new_key) + key->dlen;
	ev = kzalloc(total, GFP_ATOMIC);
	if (!ev)
		return -ENOMEM;

	bacpy(&ev->key.bdaddr, &key->bdaddr);
	ev->key.addr_type = key->addr_type;
	ev->key.key_type = key->key_type;
	memcpy(ev->key.val, key->val, 16);
	ev->key.pin_len = key->pin_len;
	ev->key.auth = key->auth;
	ev->store_hint = bonded;
	ev->key.dlen = key->dlen;

	memcpy(ev->key.data, key->data, key->dlen);

	err = mgmt_event(MGMT_EV_NEW_KEY, index, ev, total, NULL);

	kfree(ev);

	return err;
}

int mgmt_connected(u16 index, bdaddr_t *bdaddr, u8 le)
{
	struct mgmt_ev_connected ev;
	struct pending_cmd *cmd;
	struct hci_dev *hdev;

	BT_DBG("hci%u", index);

	hdev = hci_dev_get(index);

	if (!hdev)
		return -ENODEV;

	bacpy(&ev.bdaddr, bdaddr);
	ev.le = le;

	cmd = mgmt_pending_find(MGMT_OP_LE_CREATE_CONN_WHITE_LIST, index);
	if (cmd) {
		BT_ERR("mgmt_connected remove mgmt pending white_list");
		mgmt_pending_remove(cmd);
	}

	return mgmt_event(MGMT_EV_CONNECTED, index, &ev, sizeof(ev), NULL);
}

int mgmt_le_conn_params(u16 index, bdaddr_t *bdaddr, u16 interval,
						u16 latency, u16 timeout)
{
	struct mgmt_ev_le_conn_params ev;

	bacpy(&ev.bdaddr, bdaddr);
	ev.interval = interval;
	ev.latency = latency;
	ev.timeout = timeout;

	return mgmt_event(MGMT_EV_LE_CONN_PARAMS, index, &ev, sizeof(ev),
									NULL);
=======
	return err;
}

int mgmt_write_scan_failed(struct hci_dev *hdev, u8 scan, u8 status)
{
	u8 mgmt_err = mgmt_status(status);

	if (scan & SCAN_PAGE)
		mgmt_pending_foreach(MGMT_OP_SET_CONNECTABLE, hdev,
				     cmd_status_rsp, &mgmt_err);

	if (scan & SCAN_INQUIRY)
		mgmt_pending_foreach(MGMT_OP_SET_DISCOVERABLE, hdev,
				     cmd_status_rsp, &mgmt_err);

	return 0;
}

int mgmt_new_link_key(struct hci_dev *hdev, struct link_key *key, bool persistent)
{
	struct mgmt_ev_new_link_key ev;

	memset(&ev, 0, sizeof(ev));

	ev.store_hint = persistent;
	bacpy(&ev.key.addr.bdaddr, &key->bdaddr);
	ev.key.addr.type = MGMT_ADDR_BREDR;
	ev.key.type = key->type;
	memcpy(ev.key.val, key->val, 16);
	ev.key.pin_len = key->pin_len;

	return mgmt_event(MGMT_EV_NEW_LINK_KEY, hdev, &ev, sizeof(ev), NULL);
}

int mgmt_new_ltk(struct hci_dev *hdev, struct smp_ltk *key, u8 persistent)
{
	struct mgmt_ev_new_long_term_key ev;

	memset(&ev, 0, sizeof(ev));

	ev.store_hint = persistent;
	bacpy(&ev.key.addr.bdaddr, &key->bdaddr);
	ev.key.addr.type = key->bdaddr_type;
	ev.key.authenticated = key->authenticated;
	ev.key.enc_size = key->enc_size;
	ev.key.ediv = key->ediv;

	if (key->type == HCI_SMP_LTK)
		ev.key.master = 1;

	memcpy(ev.key.rand, key->rand, sizeof(key->rand));
	memcpy(ev.key.val, key->val, sizeof(key->val));

	return mgmt_event(MGMT_EV_NEW_LONG_TERM_KEY, hdev, &ev, sizeof(ev),
			  NULL);
}

int mgmt_device_connected(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
			  u8 addr_type, u32 flags, u8 *name, u8 name_len,
			  u8 *dev_class)
{
	char buf[512];
	struct mgmt_ev_device_connected *ev = (void *) buf;
	u16 eir_len = 0;

	bacpy(&ev->addr.bdaddr, bdaddr);
	ev->addr.type = link_to_mgmt(link_type, addr_type);

	ev->flags = __cpu_to_le32(flags);

	if (name_len > 0)
		eir_len = eir_append_data(ev->eir, 0, EIR_NAME_COMPLETE,
					  name, name_len);

	if (dev_class && memcmp(dev_class, "\0\0\0", 3) != 0)
		eir_len = eir_append_data(ev->eir, eir_len,
					  EIR_CLASS_OF_DEV, dev_class, 3);

	put_unaligned_le16(eir_len, &ev->eir_len);

	return mgmt_event(MGMT_EV_DEVICE_CONNECTED, hdev, buf,
			  sizeof(*ev) + eir_len, NULL);
>>>>>>> remotes/linux2/linux-3.4.y
}

static void disconnect_rsp(struct pending_cmd *cmd, void *data)
{
	struct mgmt_cp_disconnect *cp = cmd->param;
	struct sock **sk = data;
	struct mgmt_rp_disconnect rp;

<<<<<<< HEAD
	bacpy(&rp.bdaddr, &cp->bdaddr);

	cmd_complete(cmd->sk, cmd->index, MGMT_OP_DISCONNECT, &rp, sizeof(rp));
=======
	bacpy(&rp.addr.bdaddr, &cp->addr.bdaddr);
	rp.addr.type = cp->addr.type;

	cmd_complete(cmd->sk, cmd->index, MGMT_OP_DISCONNECT, 0, &rp,
		     sizeof(rp));
>>>>>>> remotes/linux2/linux-3.4.y

	*sk = cmd->sk;
	sock_hold(*sk);

	mgmt_pending_remove(cmd);
}

<<<<<<< HEAD
int mgmt_disconnected(u16 index, bdaddr_t *bdaddr, u8 reason)
{
	struct mgmt_ev_disconnected ev;
	struct sock *sk = NULL;
	int err;

	bacpy(&ev.bdaddr, bdaddr);
	ev.reason = reason;

	err = mgmt_event(MGMT_EV_DISCONNECTED, index, &ev, sizeof(ev), sk);

	if (sk)
		sock_put(sk);

	mgmt_pending_foreach(MGMT_OP_DISCONNECT, index, disconnect_rsp, &sk);
=======
static void unpair_device_rsp(struct pending_cmd *cmd, void *data)
{
	struct hci_dev *hdev = data;
	struct mgmt_cp_unpair_device *cp = cmd->param;
	struct mgmt_rp_unpair_device rp;

	memset(&rp, 0, sizeof(rp));
	bacpy(&rp.addr.bdaddr, &cp->addr.bdaddr);
	rp.addr.type = cp->addr.type;

	device_unpaired(hdev, &cp->addr.bdaddr, cp->addr.type, cmd->sk);

	cmd_complete(cmd->sk, cmd->index, cmd->opcode, 0, &rp, sizeof(rp));

	mgmt_pending_remove(cmd);
}

int mgmt_device_disconnected(struct hci_dev *hdev, bdaddr_t *bdaddr,
			     u8 link_type, u8 addr_type)
{
	struct mgmt_addr_info ev;
	struct sock *sk = NULL;
	int err;

	mgmt_pending_foreach(MGMT_OP_DISCONNECT, hdev, disconnect_rsp, &sk);

	bacpy(&ev.bdaddr, bdaddr);
	ev.type = link_to_mgmt(link_type, addr_type);

	err = mgmt_event(MGMT_EV_DEVICE_DISCONNECTED, hdev, &ev, sizeof(ev),
			 sk);

	if (sk)
	  sock_put(sk);

	mgmt_pending_foreach(MGMT_OP_UNPAIR_DEVICE, hdev, unpair_device_rsp,
			     hdev);
>>>>>>> remotes/linux2/linux-3.4.y

	return err;
}

<<<<<<< HEAD
int mgmt_disconnect_failed(u16 index)
{
	struct pending_cmd *cmd;
	int err;

	cmd = mgmt_pending_find(MGMT_OP_DISCONNECT, index);
	if (!cmd)
		return -ENOENT;

	err = cmd_status(cmd->sk, index, MGMT_OP_DISCONNECT, EIO);

	mgmt_pending_remove(cmd);

	return err;
}

int mgmt_connect_failed(u16 index, bdaddr_t *bdaddr, u8 status)
{
	struct mgmt_ev_connect_failed ev;

	bacpy(&ev.bdaddr, bdaddr);
	ev.status = status;

	return mgmt_event(MGMT_EV_CONNECT_FAILED, index, &ev, sizeof(ev), NULL);
}

int mgmt_pin_code_request(u16 index, bdaddr_t *bdaddr)
{
	struct mgmt_ev_pin_code_request ev;

	BT_DBG("hci%u", index);

	bacpy(&ev.bdaddr, bdaddr);
	ev.secure = 0;

	return mgmt_event(MGMT_EV_PIN_CODE_REQUEST, index, &ev, sizeof(ev),
									NULL);
}

int mgmt_pin_code_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status)
=======
int mgmt_disconnect_failed(struct hci_dev *hdev, bdaddr_t *bdaddr,
			   u8 link_type, u8 addr_type, u8 status)
{
	struct mgmt_rp_disconnect rp;
	struct pending_cmd *cmd;
	int err;

	cmd = mgmt_pending_find(MGMT_OP_DISCONNECT, hdev);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.addr.bdaddr, bdaddr);
	rp.addr.type = link_to_mgmt(link_type, addr_type);

	err = cmd_complete(cmd->sk, cmd->index, MGMT_OP_DISCONNECT,
			   mgmt_status(status), &rp, sizeof(rp));

	mgmt_pending_remove(cmd);

	mgmt_pending_foreach(MGMT_OP_UNPAIR_DEVICE, hdev, unpair_device_rsp,
									hdev);
	return err;
}

int mgmt_connect_failed(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
			u8 addr_type, u8 status)
{
	struct mgmt_ev_connect_failed ev;

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_mgmt(link_type, addr_type);
	ev.status = mgmt_status(status);

	return mgmt_event(MGMT_EV_CONNECT_FAILED, hdev, &ev, sizeof(ev), NULL);
}

int mgmt_pin_code_request(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 secure)
{
	struct mgmt_ev_pin_code_request ev;

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = MGMT_ADDR_BREDR;
	ev.secure = secure;

	return mgmt_event(MGMT_EV_PIN_CODE_REQUEST, hdev, &ev, sizeof(ev),
			  NULL);
}

int mgmt_pin_code_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				 u8 status)
>>>>>>> remotes/linux2/linux-3.4.y
{
	struct pending_cmd *cmd;
	struct mgmt_rp_pin_code_reply rp;
	int err;

<<<<<<< HEAD
	cmd = mgmt_pending_find(MGMT_OP_PIN_CODE_REPLY, index);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.bdaddr, bdaddr);
	rp.status = status;

	err = cmd_complete(cmd->sk, index, MGMT_OP_PIN_CODE_REPLY, &rp,
								sizeof(rp));
=======
	cmd = mgmt_pending_find(MGMT_OP_PIN_CODE_REPLY, hdev);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.addr.bdaddr, bdaddr);
	rp.addr.type = MGMT_ADDR_BREDR;

	err = cmd_complete(cmd->sk, hdev->id, MGMT_OP_PIN_CODE_REPLY,
			   mgmt_status(status), &rp, sizeof(rp));
>>>>>>> remotes/linux2/linux-3.4.y

	mgmt_pending_remove(cmd);

	return err;
}

<<<<<<< HEAD
int mgmt_pin_code_neg_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status)
=======
int mgmt_pin_code_neg_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				     u8 status)
>>>>>>> remotes/linux2/linux-3.4.y
{
	struct pending_cmd *cmd;
	struct mgmt_rp_pin_code_reply rp;
	int err;

<<<<<<< HEAD
	cmd = mgmt_pending_find(MGMT_OP_PIN_CODE_NEG_REPLY, index);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.bdaddr, bdaddr);
	rp.status = status;

	err = cmd_complete(cmd->sk, index, MGMT_OP_PIN_CODE_NEG_REPLY, &rp,
								sizeof(rp));
=======
	cmd = mgmt_pending_find(MGMT_OP_PIN_CODE_NEG_REPLY, hdev);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.addr.bdaddr, bdaddr);
	rp.addr.type = MGMT_ADDR_BREDR;

	err = cmd_complete(cmd->sk, hdev->id, MGMT_OP_PIN_CODE_NEG_REPLY,
			   mgmt_status(status), &rp, sizeof(rp));
>>>>>>> remotes/linux2/linux-3.4.y

	mgmt_pending_remove(cmd);

	return err;
}

<<<<<<< HEAD
int mgmt_user_confirm_request(u16 index, u8 event,
					bdaddr_t *bdaddr, __le32 value)
{
	struct mgmt_ev_user_confirm_request ev;
	struct hci_conn *conn = NULL;
	struct hci_dev *hdev;
	u8 loc_cap, rem_cap, loc_mitm, rem_mitm;

	BT_DBG("hci%u", index);

	hdev = hci_dev_get(index);

	if (!hdev)
		return -ENODEV;

	conn = hci_conn_hash_lookup_ba(hdev, ACL_LINK, bdaddr);

	ev.auto_confirm = 0;

	if (!conn || event != HCI_EV_USER_CONFIRM_REQUEST)
		goto no_auto_confirm;

	loc_cap = (conn->io_capability == 0x04) ? 0x01 : conn->io_capability;
	rem_cap = conn->remote_cap;
	loc_mitm = conn->auth_type & 0x01;
	rem_mitm = conn->remote_auth & 0x01;

	if ((conn->auth_type & HCI_AT_DEDICATED_BONDING) &&
			conn->auth_initiator && rem_cap == 0x03)
		ev.auto_confirm = 1;
	else if (loc_cap == 0x01 && (rem_cap == 0x00 || rem_cap == 0x03)) {
		if (!loc_mitm && !rem_mitm)
			value = 0;
		goto no_auto_confirm;
	}


	if ((!loc_mitm || rem_cap == 0x03) && (!rem_mitm || loc_cap == 0x03))
		ev.auto_confirm = 1;

no_auto_confirm:
	bacpy(&ev.bdaddr, bdaddr);
	ev.event = event;
	put_unaligned_le32(value, &ev.value);

	hci_dev_put(hdev);

	return mgmt_event(MGMT_EV_USER_CONFIRM_REQUEST, index, &ev, sizeof(ev),
									NULL);
}

int mgmt_user_passkey_request(u16 index, bdaddr_t *bdaddr)
{
	struct mgmt_ev_user_passkey_request ev;

	BT_DBG("hci%u", index);

	bacpy(&ev.bdaddr, bdaddr);

	return mgmt_event(MGMT_EV_USER_PASSKEY_REQUEST, index, &ev, sizeof(ev),
									NULL);
}

static int confirm_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status,
								u8 opcode)
{
	struct pending_cmd *cmd;
	struct mgmt_rp_user_confirm_reply rp;
	int err;

	cmd = mgmt_pending_find(opcode, index);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.bdaddr, bdaddr);
	rp.status = status;
	err = cmd_complete(cmd->sk, index, opcode, &rp, sizeof(rp));

	mgmt_pending_remove(cmd);
=======
int mgmt_user_confirm_request(struct hci_dev *hdev, bdaddr_t *bdaddr,
			      u8 link_type, u8 addr_type, __le32 value,
			      u8 confirm_hint)
{
	struct mgmt_ev_user_confirm_request ev;

	BT_DBG("%s", hdev->name);

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_mgmt(link_type, addr_type);
	ev.confirm_hint = confirm_hint;
	put_unaligned_le32(value, &ev.value);

	return mgmt_event(MGMT_EV_USER_CONFIRM_REQUEST, hdev, &ev, sizeof(ev),
			  NULL);
}

int mgmt_user_passkey_request(struct hci_dev *hdev, bdaddr_t *bdaddr,
						u8 link_type, u8 addr_type)
{
	struct mgmt_ev_user_passkey_request ev;

	BT_DBG("%s", hdev->name);

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_mgmt(link_type, addr_type);

	return mgmt_event(MGMT_EV_USER_PASSKEY_REQUEST, hdev, &ev, sizeof(ev),
			  NULL);
}

static int user_pairing_resp_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
					u8 link_type, u8 addr_type, u8 status,
					u8 opcode)
{
	struct pending_cmd *cmd;
	struct mgmt_rp_user_confirm_reply rp;
	int err;

	cmd = mgmt_pending_find(opcode, hdev);
	if (!cmd)
		return -ENOENT;

	bacpy(&rp.addr.bdaddr, bdaddr);
	rp.addr.type = link_to_mgmt(link_type, addr_type);
	err = cmd_complete(cmd->sk, hdev->id, opcode, mgmt_status(status),
			   &rp, sizeof(rp));

	mgmt_pending_remove(cmd);

	return err;
}

int mgmt_user_confirm_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				     u8 link_type, u8 addr_type, u8 status)
{
	return user_pairing_resp_complete(hdev, bdaddr, link_type, addr_type,
					  status, MGMT_OP_USER_CONFIRM_REPLY);
}

int mgmt_user_confirm_neg_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
					 u8 link_type, u8 addr_type, u8 status)
{
	return user_pairing_resp_complete(hdev, bdaddr, link_type, addr_type,
					  status, MGMT_OP_USER_CONFIRM_NEG_REPLY);
}

int mgmt_user_passkey_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
				     u8 link_type, u8 addr_type, u8 status)
{
	return user_pairing_resp_complete(hdev, bdaddr, link_type, addr_type,
					  status, MGMT_OP_USER_PASSKEY_REPLY);
}

int mgmt_user_passkey_neg_reply_complete(struct hci_dev *hdev, bdaddr_t *bdaddr,
					 u8 link_type, u8 addr_type, u8 status)
{
	return user_pairing_resp_complete(hdev, bdaddr, link_type, addr_type,
					  status, MGMT_OP_USER_PASSKEY_NEG_REPLY);
}

int mgmt_auth_failed(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
		     u8 addr_type, u8 status)
{
	struct mgmt_ev_auth_failed ev;

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = link_to_mgmt(link_type, addr_type);
	ev.status = mgmt_status(status);

	return mgmt_event(MGMT_EV_AUTH_FAILED, hdev, &ev, sizeof(ev), NULL);
}

int mgmt_auth_enable_complete(struct hci_dev *hdev, u8 status)
{
	struct cmd_lookup match = { NULL, hdev };
	bool changed = false;
	int err = 0;

	if (status) {
		u8 mgmt_err = mgmt_status(status);
		mgmt_pending_foreach(MGMT_OP_SET_LINK_SECURITY, hdev,
				     cmd_status_rsp, &mgmt_err);
		return 0;
	}

	if (test_bit(HCI_AUTH, &hdev->flags)) {
		if (!test_and_set_bit(HCI_LINK_SECURITY, &hdev->dev_flags))
			changed = true;
	} else {
		if (test_and_clear_bit(HCI_LINK_SECURITY, &hdev->dev_flags))
			changed = true;
	}

	mgmt_pending_foreach(MGMT_OP_SET_LINK_SECURITY, hdev, settings_rsp,
			     &match);

	if (changed)
		err = new_settings(hdev, match.sk);

	if (match.sk)
		sock_put(match.sk);
>>>>>>> remotes/linux2/linux-3.4.y

	return err;
}

<<<<<<< HEAD
int mgmt_user_confirm_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status)
{
	return confirm_reply_complete(index, bdaddr, status,
						MGMT_OP_USER_CONFIRM_REPLY);
}

int mgmt_user_confirm_neg_reply_complete(u16 index, bdaddr_t *bdaddr, u8 status)
{
	return confirm_reply_complete(index, bdaddr, status,
					MGMT_OP_USER_CONFIRM_NEG_REPLY);
}

int mgmt_auth_failed(u16 index, bdaddr_t *bdaddr, u8 status)
{
	struct mgmt_ev_auth_failed ev;

	bacpy(&ev.bdaddr, bdaddr);
	ev.status = status;

	return mgmt_event(MGMT_EV_AUTH_FAILED, index, &ev, sizeof(ev), NULL);
}

int mgmt_set_local_name_complete(u16 index, u8 *name, u8 status)
{
	struct pending_cmd *cmd;
	struct hci_dev *hdev;
	struct mgmt_cp_set_local_name ev;
	int err;

	memset(&ev, 0, sizeof(ev));
	memcpy(ev.name, name, HCI_MAX_NAME_LENGTH);

	cmd = mgmt_pending_find(MGMT_OP_SET_LOCAL_NAME, index);
	if (!cmd)
		goto send_event;

	if (status) {
		err = cmd_status(cmd->sk, index, MGMT_OP_SET_LOCAL_NAME, EIO);
		goto failed;
	}

	hdev = hci_dev_get(index);
	if (hdev) {
		update_eir(hdev);
		hci_dev_put(hdev);
	}

	err = cmd_complete(cmd->sk, index, MGMT_OP_SET_LOCAL_NAME, &ev,
								sizeof(ev));
=======
static int clear_eir(struct hci_dev *hdev)
{
	struct hci_cp_write_eir cp;

	if (!(hdev->features[6] & LMP_EXT_INQ))
		return 0;

	memset(hdev->eir, 0, sizeof(hdev->eir));

	memset(&cp, 0, sizeof(cp));

	return hci_send_cmd(hdev, HCI_OP_WRITE_EIR, sizeof(cp), &cp);
}

int mgmt_ssp_enable_complete(struct hci_dev *hdev, u8 enable, u8 status)
{
	struct cmd_lookup match = { NULL, hdev };
	bool changed = false;
	int err = 0;

	if (status) {
		u8 mgmt_err = mgmt_status(status);

		if (enable && test_and_clear_bit(HCI_SSP_ENABLED,
						 &hdev->dev_flags))
			err = new_settings(hdev, NULL);

		mgmt_pending_foreach(MGMT_OP_SET_SSP, hdev, cmd_status_rsp,
				     &mgmt_err);

		return err;
	}

	if (enable) {
		if (!test_and_set_bit(HCI_SSP_ENABLED, &hdev->dev_flags))
			changed = true;
	} else {
		if (test_and_clear_bit(HCI_SSP_ENABLED, &hdev->dev_flags))
			changed = true;
	}

	mgmt_pending_foreach(MGMT_OP_SET_SSP, hdev, settings_rsp, &match);

	if (changed)
		err = new_settings(hdev, match.sk);

	if (match.sk)
		sock_put(match.sk);

	if (test_bit(HCI_SSP_ENABLED, &hdev->dev_flags))
		update_eir(hdev);
	else
		clear_eir(hdev);

	return err;
}

static void class_rsp(struct pending_cmd *cmd, void *data)
{
	struct cmd_lookup *match = data;

	cmd_complete(cmd->sk, cmd->index, cmd->opcode, match->mgmt_status,
		     match->hdev->dev_class, 3);

	list_del(&cmd->list);

	if (match->sk == NULL) {
		match->sk = cmd->sk;
		sock_hold(match->sk);
	}

	mgmt_pending_free(cmd);
}

int mgmt_set_class_of_dev_complete(struct hci_dev *hdev, u8 *dev_class,
				   u8 status)
{
	struct cmd_lookup match = { NULL, hdev, mgmt_status(status) };
	int err = 0;

	clear_bit(HCI_PENDING_CLASS, &hdev->dev_flags);

	mgmt_pending_foreach(MGMT_OP_SET_DEV_CLASS, hdev, class_rsp, &match);
	mgmt_pending_foreach(MGMT_OP_ADD_UUID, hdev, class_rsp, &match);
	mgmt_pending_foreach(MGMT_OP_REMOVE_UUID, hdev, class_rsp, &match);

	if (!status)
		err = mgmt_event(MGMT_EV_CLASS_OF_DEV_CHANGED, hdev, dev_class,
				 3, NULL);

	if (match.sk)
		sock_put(match.sk);

	return err;
}

int mgmt_set_local_name_complete(struct hci_dev *hdev, u8 *name, u8 status)
{
	struct pending_cmd *cmd;
	struct mgmt_cp_set_local_name ev;
	bool changed = false;
	int err = 0;

	if (memcmp(name, hdev->dev_name, sizeof(hdev->dev_name)) != 0) {
		memcpy(hdev->dev_name, name, sizeof(hdev->dev_name));
		changed = true;
	}

	memset(&ev, 0, sizeof(ev));
	memcpy(ev.name, name, HCI_MAX_NAME_LENGTH);
	memcpy(ev.short_name, hdev->short_name, HCI_MAX_SHORT_NAME_LENGTH);

	cmd = mgmt_pending_find(MGMT_OP_SET_LOCAL_NAME, hdev);
	if (!cmd)
		goto send_event;

	/* Always assume that either the short or the complete name has
	 * changed if there was a pending mgmt command */
	changed = true;

	if (status) {
		err = cmd_status(cmd->sk, hdev->id, MGMT_OP_SET_LOCAL_NAME,
				 mgmt_status(status));
		goto failed;
	}

	err = cmd_complete(cmd->sk, hdev->id, MGMT_OP_SET_LOCAL_NAME, 0, &ev,
			   sizeof(ev));
>>>>>>> remotes/linux2/linux-3.4.y
	if (err < 0)
		goto failed;

send_event:
<<<<<<< HEAD
	err = mgmt_event(MGMT_EV_LOCAL_NAME_CHANGED, index, &ev, sizeof(ev),
							cmd ? cmd->sk : NULL);
=======
	if (changed)
		err = mgmt_event(MGMT_EV_LOCAL_NAME_CHANGED, hdev, &ev,
				 sizeof(ev), cmd ? cmd->sk : NULL);

	update_eir(hdev);
>>>>>>> remotes/linux2/linux-3.4.y

failed:
	if (cmd)
		mgmt_pending_remove(cmd);
	return err;
}

<<<<<<< HEAD
int mgmt_read_local_oob_data_reply_complete(u16 index, u8 *hash, u8 *randomizer,
								u8 status)
=======
int mgmt_read_local_oob_data_reply_complete(struct hci_dev *hdev, u8 *hash,
					    u8 *randomizer, u8 status)
>>>>>>> remotes/linux2/linux-3.4.y
{
	struct pending_cmd *cmd;
	int err;

<<<<<<< HEAD
	BT_DBG("hci%u status %u", index, status);

	cmd = mgmt_pending_find(MGMT_OP_READ_LOCAL_OOB_DATA, index);
=======
	BT_DBG("%s status %u", hdev->name, status);

	cmd = mgmt_pending_find(MGMT_OP_READ_LOCAL_OOB_DATA, hdev);
>>>>>>> remotes/linux2/linux-3.4.y
	if (!cmd)
		return -ENOENT;

	if (status) {
<<<<<<< HEAD
		err = cmd_status(cmd->sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
									EIO);
=======
		err = cmd_status(cmd->sk, hdev->id, MGMT_OP_READ_LOCAL_OOB_DATA,
				 mgmt_status(status));
>>>>>>> remotes/linux2/linux-3.4.y
	} else {
		struct mgmt_rp_read_local_oob_data rp;

		memcpy(rp.hash, hash, sizeof(rp.hash));
		memcpy(rp.randomizer, randomizer, sizeof(rp.randomizer));

<<<<<<< HEAD
		err = cmd_complete(cmd->sk, index, MGMT_OP_READ_LOCAL_OOB_DATA,
							&rp, sizeof(rp));
=======
		err = cmd_complete(cmd->sk, hdev->id,
				   MGMT_OP_READ_LOCAL_OOB_DATA, 0, &rp,
				   sizeof(rp));
>>>>>>> remotes/linux2/linux-3.4.y
	}

	mgmt_pending_remove(cmd);

	return err;
}

<<<<<<< HEAD
void mgmt_read_rssi_complete(u16 index, s8 rssi, bdaddr_t *bdaddr,
		u16 handle, u8 status)
{
	struct mgmt_ev_rssi_update ev;
	struct hci_conn *conn;
	struct hci_dev *hdev;

	if (status)
		return;

	hdev = hci_dev_get(index);
	conn = hci_conn_hash_lookup_handle(hdev, handle);

	if (!conn)
		return;

	BT_DBG("rssi_update_thresh_exceed : %d ",
		   conn->rssi_update_thresh_exceed);
	BT_DBG("RSSI Threshold : %d , recvd RSSI : %d ",
			conn->rssi_threshold, rssi);

	if (conn->rssi_update_thresh_exceed == 1) {
		BT_DBG("rssi_update_thresh_exceed == 1");
		if (rssi > conn->rssi_threshold) {
			memset(&ev, 0, sizeof(ev));
			bacpy(&ev.bdaddr, bdaddr);
			ev.rssi = rssi;
			mgmt_event(MGMT_EV_RSSI_UPDATE, index, &ev,
				sizeof(ev), NULL);
		} else {
			hci_conn_set_rssi_reporter(conn, conn->rssi_threshold,
				conn->rssi_update_interval,
				conn->rssi_update_thresh_exceed);
		}
	} else {
		BT_DBG("rssi_update_thresh_exceed == 0");
		if (rssi < conn->rssi_threshold) {
			memset(&ev, 0, sizeof(ev));
			bacpy(&ev.bdaddr, bdaddr);
			ev.rssi = rssi;
			mgmt_event(MGMT_EV_RSSI_UPDATE, index, &ev,
				sizeof(ev), NULL);
		} else {
			hci_conn_set_rssi_reporter(conn, conn->rssi_threshold,
				conn->rssi_update_interval,
				conn->rssi_update_thresh_exceed);
		}
	}
}


int mgmt_device_found(u16 index, bdaddr_t *bdaddr, u8 type, u8 le,
			u8 *dev_class, s8 rssi, u8 eir_len, u8 *eir)
{
	struct mgmt_ev_device_found ev;
	struct hci_dev *hdev;
	int err;

	BT_DBG("le: %d", le);

	memset(&ev, 0, sizeof(ev));

	bacpy(&ev.bdaddr, bdaddr);
	ev.rssi = rssi;
	ev.type = type;
	ev.le = le;

	if (dev_class)
		memcpy(ev.dev_class, dev_class, sizeof(ev.dev_class));

	if (eir && eir_len)
		memcpy(ev.eir, eir, eir_len);

	err = mgmt_event(MGMT_EV_DEVICE_FOUND, index, &ev, sizeof(ev), NULL);

	if (err < 0)
		return err;

	hdev = hci_dev_get(index);

	if (!hdev)
		return 0;

	if (hdev->disco_state == SCAN_IDLE)
		goto done;

	hdev->disco_int_count++;

	if (hdev->disco_int_count >= hdev->disco_int_phase) {
		/* Inquiry scan for General Discovery LAP */
		struct hci_cp_inquiry cp = {{0x33, 0x8b, 0x9e}, 4, 0};
		struct hci_cp_le_set_scan_enable le_cp = {0, 0};

		hdev->disco_int_phase *= 2;
		hdev->disco_int_count = 0;
		if (hdev->disco_state == SCAN_LE) {
			/* cancel LE scan */
			hci_send_cmd(hdev, HCI_OP_LE_SET_SCAN_ENABLE,
					sizeof(le_cp), &le_cp);
			/* start BR scan */
			cp.num_rsp = (u8) hdev->disco_int_phase;
			hci_send_cmd(hdev, HCI_OP_INQUIRY,
					sizeof(cp), &cp);
			hdev->disco_state = SCAN_BR;
			del_timer_sync(&hdev->disco_le_timer);
		}
	}

done:
	hci_dev_put(hdev);
	return 0;
}


int mgmt_remote_name(u16 index, bdaddr_t *bdaddr, u8 status, u8 *name)
{
	struct mgmt_ev_remote_name ev;

	memset(&ev, 0, sizeof(ev));

	bacpy(&ev.bdaddr, bdaddr);
	ev.status = status;
	memcpy(ev.name, name, HCI_MAX_NAME_LENGTH);

	return mgmt_event(MGMT_EV_REMOTE_NAME, index, &ev, sizeof(ev), NULL);
}

int mgmt_encrypt_change(u16 index, bdaddr_t *bdaddr, u8 status)
{
	struct mgmt_ev_encrypt_change ev;

	BT_DBG("hci%u", index);

	bacpy(&ev.bdaddr, bdaddr);
	ev.status = status;

	return mgmt_event(MGMT_EV_ENCRYPT_CHANGE, index, &ev, sizeof(ev),
									NULL);
}

int mgmt_remote_class(u16 index, bdaddr_t *bdaddr, u8 dev_class[3])
{
	struct mgmt_ev_remote_class ev;

	memset(&ev, 0, sizeof(ev));

	bacpy(&ev.bdaddr, bdaddr);
	memcpy(ev.dev_class, dev_class, 3);

	return mgmt_event(MGMT_EV_REMOTE_CLASS, index, &ev, sizeof(ev), NULL);
}

int mgmt_remote_version(u16 index, bdaddr_t *bdaddr, u8 ver, u16 mnf,
							u16 sub_ver)
{
	struct mgmt_ev_remote_version ev;

	memset(&ev, 0, sizeof(ev));

	bacpy(&ev.bdaddr, bdaddr);
	ev.lmp_ver = ver;
	ev.manufacturer = mnf;
	ev.lmp_subver = sub_ver;

	return mgmt_event(MGMT_EV_REMOTE_VERSION, index, &ev, sizeof(ev), NULL);
}

int mgmt_remote_features(u16 index, bdaddr_t *bdaddr, u8 features[8])
{
	struct mgmt_ev_remote_features ev;

	memset(&ev, 0, sizeof(ev));

	bacpy(&ev.bdaddr, bdaddr);
	memcpy(ev.features, features, sizeof(ev.features));

	return mgmt_event(MGMT_EV_REMOTE_FEATURES, index, &ev, sizeof(ev),
									NULL);
}
=======
int mgmt_le_enable_complete(struct hci_dev *hdev, u8 enable, u8 status)
{
	struct cmd_lookup match = { NULL, hdev };
	bool changed = false;
	int err = 0;

	if (status) {
		u8 mgmt_err = mgmt_status(status);

		if (enable && test_and_clear_bit(HCI_LE_ENABLED,
						 &hdev->dev_flags))
		  err = new_settings(hdev, NULL);

		mgmt_pending_foreach(MGMT_OP_SET_LE, hdev,
				     cmd_status_rsp, &mgmt_err);

		return err;
	}

	if (enable) {
		if (!test_and_set_bit(HCI_LE_ENABLED, &hdev->dev_flags))
			changed = true;
	} else {
		if (test_and_clear_bit(HCI_LE_ENABLED, &hdev->dev_flags))
			changed = true;
	}

	mgmt_pending_foreach(MGMT_OP_SET_LE, hdev, settings_rsp, &match);

	if (changed)
		err = new_settings(hdev, match.sk);

	if (match.sk)
		sock_put(match.sk);

	return err;
}

int mgmt_device_found(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
		      u8 addr_type, u8 *dev_class, s8 rssi, u8 cfm_name, u8
		      ssp, u8 *eir, u16 eir_len)
{
	char buf[512];
	struct mgmt_ev_device_found *ev = (void *) buf;
	size_t ev_size;

	/* Leave 5 bytes for a potential CoD field */
	if (sizeof(*ev) + eir_len + 5 > sizeof(buf))
		return -EINVAL;

	memset(buf, 0, sizeof(buf));

	bacpy(&ev->addr.bdaddr, bdaddr);
	ev->addr.type = link_to_mgmt(link_type, addr_type);
	ev->rssi = rssi;
	if (cfm_name)
		ev->flags[0] |= MGMT_DEV_FOUND_CONFIRM_NAME;
	if (!ssp)
		ev->flags[0] |= MGMT_DEV_FOUND_LEGACY_PAIRING;

	if (eir_len > 0)
		memcpy(ev->eir, eir, eir_len);

	if (dev_class && !eir_has_data_type(ev->eir, eir_len, EIR_CLASS_OF_DEV))
		eir_len = eir_append_data(ev->eir, eir_len, EIR_CLASS_OF_DEV,
					  dev_class, 3);

	put_unaligned_le16(eir_len, &ev->eir_len);

	ev_size = sizeof(*ev) + eir_len;

	return mgmt_event(MGMT_EV_DEVICE_FOUND, hdev, ev, ev_size, NULL);
}

int mgmt_remote_name(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 link_type,
		     u8 addr_type, s8 rssi, u8 *name, u8 name_len)
{
	struct mgmt_ev_device_found *ev;
	char buf[sizeof(*ev) + HCI_MAX_NAME_LENGTH + 2];
	u16 eir_len;

	ev = (struct mgmt_ev_device_found *) buf;

	memset(buf, 0, sizeof(buf));

	bacpy(&ev->addr.bdaddr, bdaddr);
	ev->addr.type = link_to_mgmt(link_type, addr_type);
	ev->rssi = rssi;

	eir_len = eir_append_data(ev->eir, 0, EIR_NAME_COMPLETE, name,
				  name_len);

	put_unaligned_le16(eir_len, &ev->eir_len);

	return mgmt_event(MGMT_EV_DEVICE_FOUND, hdev, ev,
			  sizeof(*ev) + eir_len, NULL);
}

int mgmt_start_discovery_failed(struct hci_dev *hdev, u8 status)
{
	struct pending_cmd *cmd;
	u8 type;
	int err;

	hci_discovery_set_state(hdev, DISCOVERY_STOPPED);

	cmd = mgmt_pending_find(MGMT_OP_START_DISCOVERY, hdev);
	if (!cmd)
		return -ENOENT;

	type = hdev->discovery.type;

	err = cmd_complete(cmd->sk, hdev->id, cmd->opcode, mgmt_status(status),
			   &type, sizeof(type));
	mgmt_pending_remove(cmd);

	return err;
}

int mgmt_stop_discovery_failed(struct hci_dev *hdev, u8 status)
{
	struct pending_cmd *cmd;
	int err;

	cmd = mgmt_pending_find(MGMT_OP_STOP_DISCOVERY, hdev);
	if (!cmd)
		return -ENOENT;

	err = cmd_complete(cmd->sk, hdev->id, cmd->opcode, mgmt_status(status),
			   &hdev->discovery.type, sizeof(hdev->discovery.type));
	mgmt_pending_remove(cmd);

	return err;
}

int mgmt_discovering(struct hci_dev *hdev, u8 discovering)
{
	struct mgmt_ev_discovering ev;
	struct pending_cmd *cmd;

	BT_DBG("%s discovering %u", hdev->name, discovering);

	if (discovering)
		cmd = mgmt_pending_find(MGMT_OP_START_DISCOVERY, hdev);
	else
		cmd = mgmt_pending_find(MGMT_OP_STOP_DISCOVERY, hdev);

	if (cmd != NULL) {
		u8 type = hdev->discovery.type;

		cmd_complete(cmd->sk, hdev->id, cmd->opcode, 0, &type,
			     sizeof(type));
		mgmt_pending_remove(cmd);
	}

	memset(&ev, 0, sizeof(ev));
	ev.type = hdev->discovery.type;
	ev.discovering = discovering;

	return mgmt_event(MGMT_EV_DISCOVERING, hdev, &ev, sizeof(ev), NULL);
}

int mgmt_device_blocked(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 type)
{
	struct pending_cmd *cmd;
	struct mgmt_ev_device_blocked ev;

	cmd = mgmt_pending_find(MGMT_OP_BLOCK_DEVICE, hdev);

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = type;

	return mgmt_event(MGMT_EV_DEVICE_BLOCKED, hdev, &ev, sizeof(ev),
			  cmd ? cmd->sk : NULL);
}

int mgmt_device_unblocked(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 type)
{
	struct pending_cmd *cmd;
	struct mgmt_ev_device_unblocked ev;

	cmd = mgmt_pending_find(MGMT_OP_UNBLOCK_DEVICE, hdev);

	bacpy(&ev.addr.bdaddr, bdaddr);
	ev.addr.type = type;

	return mgmt_event(MGMT_EV_DEVICE_UNBLOCKED, hdev, &ev, sizeof(ev),
			  cmd ? cmd->sk : NULL);
}

module_param(enable_hs, bool, 0644);
MODULE_PARM_DESC(enable_hs, "Enable High Speed support");

module_param(enable_le, bool, 0644);
MODULE_PARM_DESC(enable_le, "Enable Low Energy support");
>>>>>>> remotes/linux2/linux-3.4.y
