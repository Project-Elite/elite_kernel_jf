/*
   BlueZ - Bluetooth protocol stack for Linux
<<<<<<< HEAD
   Copyright (c) 2000-2001, 2010-2012 The Linux Foundation.  All rights reserved.
   Copyright (C) 2009-2010 Gustavo F. Padovan <gustavo@padovan.org>
   Copyright (C) 2010 Google Inc.
=======
   Copyright (C) 2000-2001 Qualcomm Incorporated
   Copyright (C) 2009-2010 Gustavo F. Padovan <gustavo@padovan.org>
   Copyright (C) 2010 Google Inc.
   Copyright (C) 2011 ProFUSION Embedded Systems
>>>>>>> remotes/linux2/linux-3.4.y

   Written 2000,2001 by Maxim Krasnyansky <maxk@qualcomm.com>

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

/* Bluetooth L2CAP core. */

#include <linux/module.h>

#include <linux/types.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/socket.h>
#include <linux/skbuff.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/crc16.h>
<<<<<<< HEAD
#include <linux/math64.h>
#include <net/sock.h>

#include <asm/system.h>
=======
#include <net/sock.h>

>>>>>>> remotes/linux2/linux-3.4.y
#include <asm/unaligned.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>
#include <net/bluetooth/l2cap.h>
#include <net/bluetooth/smp.h>
<<<<<<< HEAD
#include <net/bluetooth/amp.h>

bool disable_ertm;
bool enable_hs;
bool enable_reconfig;

static u32 l2cap_feat_mask = L2CAP_FEAT_FIXED_CHAN;
static u8 l2cap_fc_mask = L2CAP_FC_L2CAP;

struct workqueue_struct *_l2cap_wq;

struct bt_sock_list l2cap_sk_list = {
	.lock = __RW_LOCK_UNLOCKED(l2cap_sk_list.lock)
};

static void l2cap_send_move_chan_req(struct l2cap_conn *conn,
			struct l2cap_pinfo *pi, u16 icid, u8 dest_amp_id);
static void l2cap_send_move_chan_cfm(struct l2cap_conn *conn,
			struct l2cap_pinfo *pi, u16 icid, u16 result);
static void l2cap_send_move_chan_rsp(struct l2cap_conn *conn, u8 ident,
			u16 icid, u16 result);

static void l2cap_amp_move_setup(struct sock *sk);
static void l2cap_amp_move_success(struct sock *sk);
static void l2cap_amp_move_revert(struct sock *sk);

static int l2cap_ertm_rx_queued_iframes(struct sock *sk);

static struct sk_buff *l2cap_build_cmd(struct l2cap_conn *conn,
				u8 code, u8 ident, u16 dlen, void *data);
static int l2cap_answer_move_poll(struct sock *sk);
static int l2cap_create_cfm(struct hci_chan *chan, u8 status);
static int l2cap_deaggregate(struct hci_chan *chan, struct l2cap_pinfo *pi);
static void l2cap_chan_ready(struct sock *sk);
static void l2cap_conn_del(struct hci_conn *hcon, int err, u8 is_process);
static u16 l2cap_get_smallest_flushto(struct l2cap_chan_list *l);
static void l2cap_set_acl_flushto(struct hci_conn *hcon, u16 flush_to);

/* ---- L2CAP channels ---- */
static struct sock *__l2cap_get_chan_by_dcid(struct l2cap_chan_list *l, u16 cid)
{
	struct sock *s;
	for (s = l->head; s; s = l2cap_pi(s)->next_c) {
		if (l2cap_pi(s)->dcid == cid)
			break;
	}
	return s;
}

/* Find channel with given DCID.
 * Returns locked socket */
static inline struct sock *l2cap_get_chan_by_dcid(struct l2cap_chan_list *l,
						u16 cid)
{
	struct sock *s;
	read_lock(&l->lock);
	s = __l2cap_get_chan_by_dcid(l, cid);
	if (s)
		bh_lock_sock(s);
	read_unlock(&l->lock);
	return s;
}

static struct sock *__l2cap_get_chan_by_scid(struct l2cap_chan_list *l, u16 cid)
{
	struct sock *s;
	for (s = l->head; s; s = l2cap_pi(s)->next_c) {
		if (l2cap_pi(s)->scid == cid)
			break;
	}
	return s;
=======

bool disable_ertm;

static u32 l2cap_feat_mask = L2CAP_FEAT_FIXED_CHAN;
static u8 l2cap_fixed_chan[8] = { L2CAP_FC_L2CAP, };

static LIST_HEAD(chan_list);
static DEFINE_RWLOCK(chan_list_lock);

static struct sk_buff *l2cap_build_cmd(struct l2cap_conn *conn,
				u8 code, u8 ident, u16 dlen, void *data);
static void l2cap_send_cmd(struct l2cap_conn *conn, u8 ident, u8 code, u16 len,
								void *data);
static int l2cap_build_conf_req(struct l2cap_chan *chan, void *data);
static void l2cap_send_disconn_req(struct l2cap_conn *conn,
				struct l2cap_chan *chan, int err);

/* ---- L2CAP channels ---- */

static struct l2cap_chan *__l2cap_get_chan_by_dcid(struct l2cap_conn *conn, u16 cid)
{
	struct l2cap_chan *c;

	list_for_each_entry(c, &conn->chan_l, list) {
		if (c->dcid == cid)
			return c;
	}
	return NULL;
}

static struct l2cap_chan *__l2cap_get_chan_by_scid(struct l2cap_conn *conn, u16 cid)
{
	struct l2cap_chan *c;

	list_for_each_entry(c, &conn->chan_l, list) {
		if (c->scid == cid)
			return c;
	}
	return NULL;
>>>>>>> remotes/linux2/linux-3.4.y
}

/* Find channel with given SCID.
 * Returns locked socket */
<<<<<<< HEAD
static inline struct sock *l2cap_get_chan_by_scid(struct l2cap_chan_list *l, u16 cid)
{
	struct sock *s;
	read_lock(&l->lock);
	s = __l2cap_get_chan_by_scid(l, cid);
	if (s)
		bh_lock_sock(s);
	read_unlock(&l->lock);
	return s;
}

static struct sock *__l2cap_get_chan_by_ident(struct l2cap_chan_list *l, u8 ident)
{
	struct sock *s;
	for (s = l->head; s; s = l2cap_pi(s)->next_c) {
		if (l2cap_pi(s)->ident == ident)
			break;
	}
	return s;
}

static inline struct sock *l2cap_get_chan_by_ident(struct l2cap_chan_list *l, u8 ident)
{
	struct sock *s;
	read_lock(&l->lock);
	s = __l2cap_get_chan_by_ident(l, ident);
	if (s)
		bh_lock_sock(s);
	read_unlock(&l->lock);
	return s;
}

static inline struct sk_buff *l2cap_ertm_seq_in_queue(struct sk_buff_head *head,
						u16 seq)
{
	struct sk_buff *skb;

	skb_queue_walk(head, skb) {
		if (bt_cb(skb)->control.txseq == seq)
			return skb;
	}

	return NULL;
}

static int l2cap_seq_list_init(struct l2cap_seq_list *seq_list, u16 size)
{
	u16 allocSize = 1;
	int err = 0;
	int i;

	/* Actual allocated size must be a power of 2 */
	while (allocSize && allocSize <= size)
		allocSize <<= 1;
	if (!allocSize)
		return -ENOMEM;

	seq_list->list = kzalloc(sizeof(u16) * allocSize, GFP_ATOMIC);
	if (!seq_list->list)
		return -ENOMEM;

	seq_list->size = allocSize;
	seq_list->mask = allocSize - 1;
	seq_list->head = L2CAP_SEQ_LIST_CLEAR;
	seq_list->tail = L2CAP_SEQ_LIST_CLEAR;
	for (i = 0; i < allocSize; i++)
		seq_list->list[i] = L2CAP_SEQ_LIST_CLEAR;

	return err;
}

static inline void l2cap_seq_list_free(struct l2cap_seq_list *seq_list)
{
	kfree(seq_list->list);
}

static inline bool l2cap_seq_list_contains(struct l2cap_seq_list *seq_list,
					u16 seq)
{
	return seq_list->list[seq & seq_list->mask] != L2CAP_SEQ_LIST_CLEAR;
}

static u16 l2cap_seq_list_remove(struct l2cap_seq_list *seq_list, u16 seq)
{
	u16 mask = seq_list->mask;

	BT_DBG("seq_list %p, seq %d", seq_list, (int) seq);

	if (seq_list->head == L2CAP_SEQ_LIST_CLEAR) {
		/* In case someone tries to pop the head of an empty list */
		BT_DBG("List empty");
		return L2CAP_SEQ_LIST_CLEAR;
	} else if (seq_list->head == seq) {
		/* Head can be removed quickly */
		BT_DBG("Remove head");
		seq_list->head = seq_list->list[seq & mask];
		seq_list->list[seq & mask] = L2CAP_SEQ_LIST_CLEAR;

		if (seq_list->head == L2CAP_SEQ_LIST_TAIL) {
			seq_list->head = L2CAP_SEQ_LIST_CLEAR;
			seq_list->tail = L2CAP_SEQ_LIST_CLEAR;
		}
	} else {
		/* Non-head item must be found first */
		u16 prev = seq_list->head;
		BT_DBG("Find and remove");
		while (seq_list->list[prev & mask] != seq) {
			prev = seq_list->list[prev & mask];
			if (prev == L2CAP_SEQ_LIST_TAIL) {
				BT_DBG("seq %d not in list", (int) seq);
				return L2CAP_SEQ_LIST_CLEAR;
			}
		}

		seq_list->list[prev & mask] = seq_list->list[seq & mask];
		seq_list->list[seq & mask] = L2CAP_SEQ_LIST_CLEAR;
		if (seq_list->tail == seq)
			seq_list->tail = prev;
	}
	return seq;
}

static inline u16 l2cap_seq_list_pop(struct l2cap_seq_list *seq_list)
{
	return l2cap_seq_list_remove(seq_list, seq_list->head);
}

static void l2cap_seq_list_clear(struct l2cap_seq_list *seq_list)
{
	if (seq_list->head != L2CAP_SEQ_LIST_CLEAR) {
		u16 i;
		for (i = 0; i < seq_list->size; i++)
			seq_list->list[i] = L2CAP_SEQ_LIST_CLEAR;

		seq_list->head = L2CAP_SEQ_LIST_CLEAR;
		seq_list->tail = L2CAP_SEQ_LIST_CLEAR;
	}
}

static void l2cap_seq_list_append(struct l2cap_seq_list *seq_list, u16 seq)
{
	u16 mask = seq_list->mask;

	BT_DBG("seq_list %p, seq %d", seq_list, (int) seq);

	if (seq_list->list[seq & mask] == L2CAP_SEQ_LIST_CLEAR) {
		if (seq_list->tail == L2CAP_SEQ_LIST_CLEAR)
			seq_list->head = seq;
		else
			seq_list->list[seq_list->tail & mask] = seq;

		seq_list->tail = seq;
		seq_list->list[seq & mask] = L2CAP_SEQ_LIST_TAIL;
	}
}

static u16 __pack_enhanced_control(struct bt_l2cap_control *control)
{
	u16 packed;

	packed = (control->reqseq << L2CAP_CTRL_REQSEQ_SHIFT) &
		L2CAP_CTRL_REQSEQ;
	packed |= (control->final << L2CAP_CTRL_FINAL_SHIFT) &
		L2CAP_CTRL_FINAL;

	if (control->frame_type == 's') {
		packed |= (control->poll << L2CAP_CTRL_POLL_SHIFT) &
			L2CAP_CTRL_POLL;
		packed |= (control->super << L2CAP_CTRL_SUPERVISE_SHIFT) &
			L2CAP_CTRL_SUPERVISE;
		packed |= L2CAP_CTRL_FRAME_TYPE;
	} else {
		packed |= (control->sar << L2CAP_CTRL_SAR_SHIFT) &
			L2CAP_CTRL_SAR;
		packed |= (control->txseq << L2CAP_CTRL_TXSEQ_SHIFT) &
			L2CAP_CTRL_TXSEQ;
	}

	return packed;
}

static void __get_enhanced_control(u16 enhanced,
					struct bt_l2cap_control *control)
{
	control->reqseq = (enhanced & L2CAP_CTRL_REQSEQ) >>
		L2CAP_CTRL_REQSEQ_SHIFT;
	control->final = (enhanced & L2CAP_CTRL_FINAL) >>
		L2CAP_CTRL_FINAL_SHIFT;

	if (enhanced & L2CAP_CTRL_FRAME_TYPE) {
		control->frame_type = 's';
		control->poll = (enhanced & L2CAP_CTRL_POLL) >>
			L2CAP_CTRL_POLL_SHIFT;
		control->super = (enhanced & L2CAP_CTRL_SUPERVISE) >>
			L2CAP_CTRL_SUPERVISE_SHIFT;

		control->sar = 0;
		control->txseq = 0;
	} else {
		control->frame_type = 'i';
		control->sar = (enhanced & L2CAP_CTRL_SAR) >>
			L2CAP_CTRL_SAR_SHIFT;
		control->txseq = (enhanced & L2CAP_CTRL_TXSEQ) >>
			L2CAP_CTRL_TXSEQ_SHIFT;

		control->poll = 0;
		control->super = 0;
	}
}

static u32 __pack_extended_control(struct bt_l2cap_control *control)
{
	u32 packed;

	packed = (control->reqseq << L2CAP_EXT_CTRL_REQSEQ_SHIFT) &
		L2CAP_EXT_CTRL_REQSEQ;
	packed |= (control->final << L2CAP_EXT_CTRL_FINAL_SHIFT) &
		L2CAP_EXT_CTRL_FINAL;

	if (control->frame_type == 's') {
		packed |= (control->poll << L2CAP_EXT_CTRL_POLL_SHIFT) &
			L2CAP_EXT_CTRL_POLL;
		packed |= (control->super << L2CAP_EXT_CTRL_SUPERVISE_SHIFT) &
			L2CAP_EXT_CTRL_SUPERVISE;
		packed |= L2CAP_EXT_CTRL_FRAME_TYPE;
	} else {
		packed |= (control->sar << L2CAP_EXT_CTRL_SAR_SHIFT) &
			L2CAP_EXT_CTRL_SAR;
		packed |= (control->txseq << L2CAP_EXT_CTRL_TXSEQ_SHIFT) &
			L2CAP_EXT_CTRL_TXSEQ;
	}

	return packed;
}

static void __get_extended_control(u32 extended,
				struct bt_l2cap_control *control)
{
	control->reqseq = (extended & L2CAP_EXT_CTRL_REQSEQ) >>
		L2CAP_EXT_CTRL_REQSEQ_SHIFT;
	control->final = (extended & L2CAP_EXT_CTRL_FINAL) >>
		L2CAP_EXT_CTRL_FINAL_SHIFT;

	if (extended & L2CAP_EXT_CTRL_FRAME_TYPE) {
		control->frame_type = 's';
		control->poll = (extended & L2CAP_EXT_CTRL_POLL) >>
			L2CAP_EXT_CTRL_POLL_SHIFT;
		control->super = (extended & L2CAP_EXT_CTRL_SUPERVISE) >>
			L2CAP_EXT_CTRL_SUPERVISE_SHIFT;

		control->sar = 0;
		control->txseq = 0;
	} else {
		control->frame_type = 'i';
		control->sar = (extended & L2CAP_EXT_CTRL_SAR) >>
			L2CAP_EXT_CTRL_SAR_SHIFT;
		control->txseq = (extended & L2CAP_EXT_CTRL_TXSEQ) >>
			L2CAP_EXT_CTRL_TXSEQ_SHIFT;

		control->poll = 0;
		control->super = 0;
	}
}

static inline void l2cap_ertm_stop_ack_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	__cancel_delayed_work(&pi->ack_work);
}

static inline void l2cap_ertm_start_ack_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p, pending %d", pi, delayed_work_pending(&pi->ack_work));
	if (!delayed_work_pending(&pi->ack_work)) {
		queue_delayed_work(_l2cap_wq, &pi->ack_work,
				msecs_to_jiffies(L2CAP_DEFAULT_ACK_TO));
	}
}

static inline void l2cap_ertm_stop_retrans_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	__cancel_delayed_work(&pi->retrans_work);
}

static inline void l2cap_ertm_start_retrans_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	if (!delayed_work_pending(&pi->monitor_work) && pi->retrans_timeout) {
		__cancel_delayed_work(&pi->retrans_work);
		queue_delayed_work(_l2cap_wq, &pi->retrans_work,
			msecs_to_jiffies(pi->retrans_timeout));
	}
}

static inline void l2cap_ertm_stop_monitor_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	__cancel_delayed_work(&pi->monitor_work);
}

static inline void l2cap_ertm_start_monitor_timer(struct l2cap_pinfo *pi)
{
	BT_DBG("pi %p", pi);
	l2cap_ertm_stop_retrans_timer(pi);
	__cancel_delayed_work(&pi->monitor_work);
	if (pi->monitor_timeout) {
		queue_delayed_work(_l2cap_wq, &pi->monitor_work,
				msecs_to_jiffies(pi->monitor_timeout));
	}
}

static u16 l2cap_alloc_cid(struct l2cap_chan_list *l)
{
	u16 cid = L2CAP_CID_DYN_START;

	for (; cid < L2CAP_CID_DYN_END; cid++) {
		if (!__l2cap_get_chan_by_scid(l, cid))
			return cid;
	}

	return 0;
}

static inline void __l2cap_chan_link(struct l2cap_chan_list *l, struct sock *sk)
{
	sock_hold(sk);

	if (l->head)
		l2cap_pi(l->head)->prev_c = sk;

	l2cap_pi(sk)->next_c = l->head;
	l2cap_pi(sk)->prev_c = NULL;
	l->head = sk;
}

static inline void l2cap_chan_unlink(struct l2cap_chan_list *l, struct sock *sk)
{
	struct sock *next = l2cap_pi(sk)->next_c, *prev = l2cap_pi(sk)->prev_c;

	write_lock_bh(&l->lock);
	if (sk == l->head)
		l->head = next;

	if (next)
		l2cap_pi(next)->prev_c = prev;
	if (prev)
		l2cap_pi(prev)->next_c = next;
	write_unlock_bh(&l->lock);

	__sock_put(sk);
}

static void __l2cap_chan_add(struct l2cap_conn *conn, struct sock *sk)
{
	struct l2cap_chan_list *l = &conn->chan_list;

	BT_DBG("conn %p, psm 0x%2.2x, dcid 0x%4.4x", conn,
			l2cap_pi(sk)->psm, l2cap_pi(sk)->dcid);

	conn->disc_reason = 0x13;

	l2cap_pi(sk)->conn = conn;

	if (!l2cap_pi(sk)->fixed_channel &&
		(sk->sk_type == SOCK_SEQPACKET || sk->sk_type == SOCK_STREAM)) {
		if (conn->hcon->type == LE_LINK) {
			/* LE connection */
			if (l2cap_pi(sk)->imtu < L2CAP_LE_DEFAULT_MTU)
				l2cap_pi(sk)->imtu = L2CAP_LE_DEFAULT_MTU;
			if (l2cap_pi(sk)->omtu < L2CAP_LE_DEFAULT_MTU)
				l2cap_pi(sk)->omtu = L2CAP_LE_DEFAULT_MTU;

			l2cap_pi(sk)->scid = L2CAP_CID_LE_DATA;
			l2cap_pi(sk)->dcid = L2CAP_CID_LE_DATA;
		} else {
			/* Alloc CID for connection-oriented socket */
			l2cap_pi(sk)->scid = l2cap_alloc_cid(l);
			l2cap_pi(sk)->omtu = L2CAP_DEFAULT_MTU;
		}
	} else if (sk->sk_type == SOCK_DGRAM) {
		/* Connectionless socket */
		l2cap_pi(sk)->scid = L2CAP_CID_CONN_LESS;
		l2cap_pi(sk)->dcid = L2CAP_CID_CONN_LESS;
		l2cap_pi(sk)->omtu = L2CAP_DEFAULT_MTU;
	} else if (sk->sk_type == SOCK_RAW) {
		/* Raw socket can send/recv signalling messages only */
		l2cap_pi(sk)->scid = L2CAP_CID_SIGNALING;
		l2cap_pi(sk)->dcid = L2CAP_CID_SIGNALING;
		l2cap_pi(sk)->omtu = L2CAP_DEFAULT_MTU;
	}

	if (l2cap_get_smallest_flushto(l) > l2cap_pi(sk)->flush_to) {
		/*if flush timeout of the channel is lesser than existing */
		l2cap_set_acl_flushto(conn->hcon, l2cap_pi(sk)->flush_to);
	}
	/* Otherwise, do not set scid/dcid/omtu.  These will be set up
	 * by l2cap_fixed_channel_config()
	 */

	__l2cap_chan_link(l, sk);
}

/* Delete channel.
 * Must be called on the locked socket. */
void l2cap_chan_del(struct sock *sk, int err)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	struct sock *parent = bt_sk(sk)->parent;

	l2cap_sock_clear_timer(sk);

	BT_DBG("sk %p, conn %p, err %d", sk, conn, err);

	if (conn) {
		struct l2cap_chan_list *l = &conn->chan_list;
		/* Unlink from channel list */
		l2cap_chan_unlink(l, sk);
		l2cap_pi(sk)->conn = NULL;
		if (!l2cap_pi(sk)->fixed_channel)
			hci_conn_put(conn->hcon);

		read_lock(&l->lock);
		if (l2cap_pi(sk)->flush_to < l2cap_get_smallest_flushto(l))
			l2cap_set_acl_flushto(conn->hcon,
				l2cap_get_smallest_flushto(l));
		read_unlock(&l->lock);
	}

	if (l2cap_pi(sk)->ampchan) {
		struct hci_chan *ampchan = l2cap_pi(sk)->ampchan;
		struct hci_conn *ampcon = l2cap_pi(sk)->ampcon;
		l2cap_pi(sk)->ampchan = NULL;
		l2cap_pi(sk)->ampcon = NULL;
		l2cap_pi(sk)->amp_id = 0;
		if (hci_chan_put(ampchan))
			ampcon->l2cap_data = NULL;
		else
			l2cap_deaggregate(ampchan, l2cap_pi(sk));
	}

	sk->sk_state = BT_CLOSED;
	sock_set_flag(sk, SOCK_ZAPPED);

	if (err)
		sk->sk_err = err;
=======
static struct l2cap_chan *l2cap_get_chan_by_scid(struct l2cap_conn *conn, u16 cid)
{
	struct l2cap_chan *c;

	mutex_lock(&conn->chan_lock);
	c = __l2cap_get_chan_by_scid(conn, cid);
	mutex_unlock(&conn->chan_lock);

	return c;
}

static struct l2cap_chan *__l2cap_get_chan_by_ident(struct l2cap_conn *conn, u8 ident)
{
	struct l2cap_chan *c;

	list_for_each_entry(c, &conn->chan_l, list) {
		if (c->ident == ident)
			return c;
	}
	return NULL;
}

static inline struct l2cap_chan *l2cap_get_chan_by_ident(struct l2cap_conn *conn, u8 ident)
{
	struct l2cap_chan *c;

	mutex_lock(&conn->chan_lock);
	c = __l2cap_get_chan_by_ident(conn, ident);
	mutex_unlock(&conn->chan_lock);

	return c;
}

static struct l2cap_chan *__l2cap_global_chan_by_addr(__le16 psm, bdaddr_t *src)
{
	struct l2cap_chan *c;

	list_for_each_entry(c, &chan_list, global_l) {
		if (c->sport == psm && !bacmp(&bt_sk(c->sk)->src, src))
			return c;
	}
	return NULL;
}

int l2cap_add_psm(struct l2cap_chan *chan, bdaddr_t *src, __le16 psm)
{
	int err;

	write_lock(&chan_list_lock);

	if (psm && __l2cap_global_chan_by_addr(psm, src)) {
		err = -EADDRINUSE;
		goto done;
	}

	if (psm) {
		chan->psm = psm;
		chan->sport = psm;
		err = 0;
	} else {
		u16 p;

		err = -EINVAL;
		for (p = 0x1001; p < 0x1100; p += 2)
			if (!__l2cap_global_chan_by_addr(cpu_to_le16(p), src)) {
				chan->psm   = cpu_to_le16(p);
				chan->sport = cpu_to_le16(p);
				err = 0;
				break;
			}
	}

done:
	write_unlock(&chan_list_lock);
	return err;
}

int l2cap_add_scid(struct l2cap_chan *chan,  __u16 scid)
{
	write_lock(&chan_list_lock);

	chan->scid = scid;

	write_unlock(&chan_list_lock);

	return 0;
}

static u16 l2cap_alloc_cid(struct l2cap_conn *conn)
{
	u16 cid = L2CAP_CID_DYN_START;

	for (; cid < L2CAP_CID_DYN_END; cid++) {
		if (!__l2cap_get_chan_by_scid(conn, cid))
			return cid;
	}

	return 0;
}

static void __l2cap_state_change(struct l2cap_chan *chan, int state)
{
	BT_DBG("chan %p %s -> %s", chan, state_to_string(chan->state),
						state_to_string(state));

	chan->state = state;
	chan->ops->state_change(chan->data, state);
}

static void l2cap_state_change(struct l2cap_chan *chan, int state)
{
	struct sock *sk = chan->sk;

	lock_sock(sk);
	__l2cap_state_change(chan, state);
	release_sock(sk);
}

static inline void __l2cap_chan_set_err(struct l2cap_chan *chan, int err)
{
	struct sock *sk = chan->sk;

	sk->sk_err = err;
}

static inline void l2cap_chan_set_err(struct l2cap_chan *chan, int err)
{
	struct sock *sk = chan->sk;

	lock_sock(sk);
	__l2cap_chan_set_err(chan, err);
	release_sock(sk);
}

static void l2cap_chan_timeout(struct work_struct *work)
{
	struct l2cap_chan *chan = container_of(work, struct l2cap_chan,
							chan_timer.work);
	struct l2cap_conn *conn = chan->conn;
	int reason;

	BT_DBG("chan %p state %s", chan, state_to_string(chan->state));

	mutex_lock(&conn->chan_lock);
	l2cap_chan_lock(chan);

	if (chan->state == BT_CONNECTED || chan->state == BT_CONFIG)
		reason = ECONNREFUSED;
	else if (chan->state == BT_CONNECT &&
					chan->sec_level != BT_SECURITY_SDP)
		reason = ECONNREFUSED;
	else
		reason = ETIMEDOUT;

	l2cap_chan_close(chan, reason);

	l2cap_chan_unlock(chan);

	chan->ops->close(chan->data);
	mutex_unlock(&conn->chan_lock);

	l2cap_chan_put(chan);
}

struct l2cap_chan *l2cap_chan_create(struct sock *sk)
{
	struct l2cap_chan *chan;

	chan = kzalloc(sizeof(*chan), GFP_ATOMIC);
	if (!chan)
		return NULL;

	mutex_init(&chan->lock);

	chan->sk = sk;

	write_lock(&chan_list_lock);
	list_add(&chan->global_l, &chan_list);
	write_unlock(&chan_list_lock);

	INIT_DELAYED_WORK(&chan->chan_timer, l2cap_chan_timeout);

	chan->state = BT_OPEN;

	atomic_set(&chan->refcnt, 1);

	BT_DBG("sk %p chan %p", sk, chan);

	return chan;
}

void l2cap_chan_destroy(struct l2cap_chan *chan)
{
	write_lock(&chan_list_lock);
	list_del(&chan->global_l);
	write_unlock(&chan_list_lock);

	l2cap_chan_put(chan);
}

void __l2cap_chan_add(struct l2cap_conn *conn, struct l2cap_chan *chan)
{
	BT_DBG("conn %p, psm 0x%2.2x, dcid 0x%4.4x", conn,
			chan->psm, chan->dcid);

	conn->disc_reason = HCI_ERROR_REMOTE_USER_TERM;

	chan->conn = conn;

	switch (chan->chan_type) {
	case L2CAP_CHAN_CONN_ORIENTED:
		if (conn->hcon->type == LE_LINK) {
			/* LE connection */
			chan->omtu = L2CAP_LE_DEFAULT_MTU;
			chan->scid = L2CAP_CID_LE_DATA;
			chan->dcid = L2CAP_CID_LE_DATA;
		} else {
			/* Alloc CID for connection-oriented socket */
			chan->scid = l2cap_alloc_cid(conn);
			chan->omtu = L2CAP_DEFAULT_MTU;
		}
		break;

	case L2CAP_CHAN_CONN_LESS:
		/* Connectionless socket */
		chan->scid = L2CAP_CID_CONN_LESS;
		chan->dcid = L2CAP_CID_CONN_LESS;
		chan->omtu = L2CAP_DEFAULT_MTU;
		break;

	default:
		/* Raw socket can send/recv signalling messages only */
		chan->scid = L2CAP_CID_SIGNALING;
		chan->dcid = L2CAP_CID_SIGNALING;
		chan->omtu = L2CAP_DEFAULT_MTU;
	}

	chan->local_id		= L2CAP_BESTEFFORT_ID;
	chan->local_stype	= L2CAP_SERV_BESTEFFORT;
	chan->local_msdu	= L2CAP_DEFAULT_MAX_SDU_SIZE;
	chan->local_sdu_itime	= L2CAP_DEFAULT_SDU_ITIME;
	chan->local_acc_lat	= L2CAP_DEFAULT_ACC_LAT;
	chan->local_flush_to	= L2CAP_DEFAULT_FLUSH_TO;

	l2cap_chan_hold(chan);

	list_add(&chan->list, &conn->chan_l);
}

void l2cap_chan_add(struct l2cap_conn *conn, struct l2cap_chan *chan)
{
	mutex_lock(&conn->chan_lock);
	__l2cap_chan_add(conn, chan);
	mutex_unlock(&conn->chan_lock);
}

static void l2cap_chan_del(struct l2cap_chan *chan, int err)
{
	struct sock *sk = chan->sk;
	struct l2cap_conn *conn = chan->conn;
	struct sock *parent = bt_sk(sk)->parent;

	__clear_chan_timer(chan);

	BT_DBG("chan %p, conn %p, err %d", chan, conn, err);

	if (conn) {
		/* Delete from channel list */
		list_del(&chan->list);

		l2cap_chan_put(chan);

		chan->conn = NULL;
		hci_conn_put(conn->hcon);
	}

	lock_sock(sk);

	__l2cap_state_change(chan, BT_CLOSED);
	sock_set_flag(sk, SOCK_ZAPPED);

	if (err)
		__l2cap_chan_set_err(chan, err);
>>>>>>> remotes/linux2/linux-3.4.y

	if (parent) {
		bt_accept_unlink(sk);
		parent->sk_data_ready(parent, 0);
	} else
		sk->sk_state_change(sk);

<<<<<<< HEAD
	sk->sk_send_head = NULL;
	skb_queue_purge(TX_QUEUE(sk));

	if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM) {
		if (l2cap_pi(sk)->sdu)
			kfree_skb(l2cap_pi(sk)->sdu);

		skb_queue_purge(SREJ_QUEUE(sk));

		__cancel_delayed_work(&l2cap_pi(sk)->ack_work);
		__cancel_delayed_work(&l2cap_pi(sk)->retrans_work);
		__cancel_delayed_work(&l2cap_pi(sk)->monitor_work);
	}
}

static inline u8 l2cap_get_auth_type(struct sock *sk)
{
	if (sk->sk_type == SOCK_RAW) {
		switch (l2cap_pi(sk)->sec_level) {
=======
	release_sock(sk);

	if (!(test_bit(CONF_OUTPUT_DONE, &chan->conf_state) &&
			test_bit(CONF_INPUT_DONE, &chan->conf_state)))
		return;

	skb_queue_purge(&chan->tx_q);

	if (chan->mode == L2CAP_MODE_ERTM) {
		struct srej_list *l, *tmp;

		__clear_retrans_timer(chan);
		__clear_monitor_timer(chan);
		__clear_ack_timer(chan);

		skb_queue_purge(&chan->srej_q);

		list_for_each_entry_safe(l, tmp, &chan->srej_l, list) {
			list_del(&l->list);
			kfree(l);
		}
	}
}

static void l2cap_chan_cleanup_listen(struct sock *parent)
{
	struct sock *sk;

	BT_DBG("parent %p", parent);

	/* Close not yet accepted channels */
	while ((sk = bt_accept_dequeue(parent, NULL))) {
		struct l2cap_chan *chan = l2cap_pi(sk)->chan;

		l2cap_chan_lock(chan);
		__clear_chan_timer(chan);
		l2cap_chan_close(chan, ECONNRESET);
		l2cap_chan_unlock(chan);

		chan->ops->close(chan->data);
	}
}

void l2cap_chan_close(struct l2cap_chan *chan, int reason)
{
	struct l2cap_conn *conn = chan->conn;
	struct sock *sk = chan->sk;

	BT_DBG("chan %p state %s sk %p", chan,
					state_to_string(chan->state), sk);

	switch (chan->state) {
	case BT_LISTEN:
		lock_sock(sk);
		l2cap_chan_cleanup_listen(sk);

		__l2cap_state_change(chan, BT_CLOSED);
		sock_set_flag(sk, SOCK_ZAPPED);
		release_sock(sk);
		break;

	case BT_CONNECTED:
	case BT_CONFIG:
		if (chan->chan_type == L2CAP_CHAN_CONN_ORIENTED &&
					conn->hcon->type == ACL_LINK) {
			__clear_chan_timer(chan);
			__set_chan_timer(chan, sk->sk_sndtimeo);
			l2cap_send_disconn_req(conn, chan, reason);
		} else
			l2cap_chan_del(chan, reason);
		break;

	case BT_CONNECT2:
		if (chan->chan_type == L2CAP_CHAN_CONN_ORIENTED &&
					conn->hcon->type == ACL_LINK) {
			struct l2cap_conn_rsp rsp;
			__u16 result;

			if (bt_sk(sk)->defer_setup)
				result = L2CAP_CR_SEC_BLOCK;
			else
				result = L2CAP_CR_BAD_PSM;
			l2cap_state_change(chan, BT_DISCONN);

			rsp.scid   = cpu_to_le16(chan->dcid);
			rsp.dcid   = cpu_to_le16(chan->scid);
			rsp.result = cpu_to_le16(result);
			rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
			l2cap_send_cmd(conn, chan->ident, L2CAP_CONN_RSP,
							sizeof(rsp), &rsp);
		}

		l2cap_chan_del(chan, reason);
		break;

	case BT_CONNECT:
	case BT_DISCONN:
		l2cap_chan_del(chan, reason);
		break;

	default:
		lock_sock(sk);
		sock_set_flag(sk, SOCK_ZAPPED);
		release_sock(sk);
		break;
	}
}

static inline u8 l2cap_get_auth_type(struct l2cap_chan *chan)
{
	if (chan->chan_type == L2CAP_CHAN_RAW) {
		switch (chan->sec_level) {
>>>>>>> remotes/linux2/linux-3.4.y
		case BT_SECURITY_HIGH:
			return HCI_AT_DEDICATED_BONDING_MITM;
		case BT_SECURITY_MEDIUM:
			return HCI_AT_DEDICATED_BONDING;
		default:
			return HCI_AT_NO_BONDING;
		}
<<<<<<< HEAD
	} else if (l2cap_pi(sk)->psm == cpu_to_le16(0x0001)) {
		if (l2cap_pi(sk)->sec_level == BT_SECURITY_LOW)
			l2cap_pi(sk)->sec_level = BT_SECURITY_SDP;

		if (l2cap_pi(sk)->sec_level == BT_SECURITY_HIGH)
=======
	} else if (chan->psm == cpu_to_le16(0x0001)) {
		if (chan->sec_level == BT_SECURITY_LOW)
			chan->sec_level = BT_SECURITY_SDP;

		if (chan->sec_level == BT_SECURITY_HIGH)
>>>>>>> remotes/linux2/linux-3.4.y
			return HCI_AT_NO_BONDING_MITM;
		else
			return HCI_AT_NO_BONDING;
	} else {
<<<<<<< HEAD
		switch (l2cap_pi(sk)->sec_level) {
=======
		switch (chan->sec_level) {
>>>>>>> remotes/linux2/linux-3.4.y
		case BT_SECURITY_HIGH:
			return HCI_AT_GENERAL_BONDING_MITM;
		case BT_SECURITY_MEDIUM:
			return HCI_AT_GENERAL_BONDING;
		default:
			return HCI_AT_NO_BONDING;
		}
	}
}

/* Service level security */
<<<<<<< HEAD
static inline int l2cap_check_security(struct sock *sk)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	__u8 auth_type;

	auth_type = l2cap_get_auth_type(sk);

	return hci_conn_security(conn->hcon, l2cap_pi(sk)->sec_level,
								auth_type);
}

u8 l2cap_get_ident(struct l2cap_conn *conn)
=======
int l2cap_chan_check_security(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	__u8 auth_type;

	auth_type = l2cap_get_auth_type(chan);

	return hci_conn_security(conn->hcon, chan->sec_level, auth_type);
}

static u8 l2cap_get_ident(struct l2cap_conn *conn)
>>>>>>> remotes/linux2/linux-3.4.y
{
	u8 id;

	/* Get next available identificator.
	 *    1 - 128 are used by kernel.
	 *  129 - 199 are reserved.
	 *  200 - 254 are used by utilities like l2ping, etc.
	 */

<<<<<<< HEAD
	spin_lock_bh(&conn->lock);
=======
	spin_lock(&conn->lock);
>>>>>>> remotes/linux2/linux-3.4.y

	if (++conn->tx_ident > 128)
		conn->tx_ident = 1;

	id = conn->tx_ident;

<<<<<<< HEAD
	spin_unlock_bh(&conn->lock);
=======
	spin_unlock(&conn->lock);
>>>>>>> remotes/linux2/linux-3.4.y

	return id;
}

<<<<<<< HEAD
static void apply_fcs(struct sk_buff *skb)
{
	size_t len;
	u16 partial_crc;
	struct sk_buff *iter;
	struct sk_buff *final_frag = skb;

	if (skb_has_frag_list(skb))
		len = skb_headlen(skb);
	else
		len = skb->len - L2CAP_FCS_SIZE;

	partial_crc = crc16(0, (u8 *) skb->data, len);

	skb_walk_frags(skb, iter) {
		len = iter->len;
		if (!iter->next)
			len -= L2CAP_FCS_SIZE;

		partial_crc = crc16(partial_crc, iter->data, len);
		final_frag = iter;
	}

	put_unaligned_le16(partial_crc,
		final_frag->data + final_frag->len - L2CAP_FCS_SIZE);
}

void l2cap_send_cmd(struct l2cap_conn *conn, u8 ident, u8 code, u16 len, void *data)
{
	struct sk_buff *skb = l2cap_build_cmd(conn, code, ident, len, data);
	u8 flags;

	BT_DBG("code 0x%2.2x", code);

	if (!skb)
		return;

	if (conn->hcon == NULL || conn->hcon->hdev == NULL)
		return;

	if (lmp_no_flush_capable(conn->hcon->hdev))
		flags = ACL_START_NO_FLUSH;
	else
		flags = ACL_START;

	bt_cb(skb)->force_active = 1;

	hci_send_acl(conn->hcon, NULL, skb, flags);
}

static inline int __l2cap_no_conn_pending(struct sock *sk)
{
	return !(l2cap_pi(sk)->conf_state & L2CAP_CONF_CONNECT_PEND);
}

static void l2cap_send_conn_req(struct sock *sk)
{
	struct l2cap_conn_req req;
	req.scid = cpu_to_le16(l2cap_pi(sk)->scid);
	req.psm  = l2cap_pi(sk)->psm;

	l2cap_pi(sk)->ident = l2cap_get_ident(l2cap_pi(sk)->conn);

	l2cap_send_cmd(l2cap_pi(sk)->conn, l2cap_pi(sk)->ident,
			L2CAP_CONN_REQ, sizeof(req), &req);
}

static void l2cap_send_create_chan_req(struct sock *sk, u8 amp_id)
{
	struct l2cap_create_chan_req req;
	req.scid = cpu_to_le16(l2cap_pi(sk)->scid);
	req.psm  = l2cap_pi(sk)->psm;
	req.amp_id = amp_id;

	l2cap_pi(sk)->conf_state |= L2CAP_CONF_LOCKSTEP;
	l2cap_pi(sk)->ident = l2cap_get_ident(l2cap_pi(sk)->conn);

	l2cap_send_cmd(l2cap_pi(sk)->conn, l2cap_pi(sk)->ident,
			L2CAP_CREATE_CHAN_REQ, sizeof(req), &req);
}

static void l2cap_do_start(struct sock *sk)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
=======
static void l2cap_send_cmd(struct l2cap_conn *conn, u8 ident, u8 code, u16 len, void *data)
{
	struct sk_buff *skb = l2cap_build_cmd(conn, code, ident, len, data);
	u8 flags;

	BT_DBG("code 0x%2.2x", code);

	if (!skb)
		return;

	if (lmp_no_flush_capable(conn->hcon->hdev))
		flags = ACL_START_NO_FLUSH;
	else
		flags = ACL_START;

	bt_cb(skb)->force_active = BT_POWER_FORCE_ACTIVE_ON;
	skb->priority = HCI_PRIO_MAX;

	hci_send_acl(conn->hchan, skb, flags);
}

static void l2cap_do_send(struct l2cap_chan *chan, struct sk_buff *skb)
{
	struct hci_conn *hcon = chan->conn->hcon;
	u16 flags;

	BT_DBG("chan %p, skb %p len %d priority %u", chan, skb, skb->len,
							skb->priority);

	if (!test_bit(FLAG_FLUSHABLE, &chan->flags) &&
					lmp_no_flush_capable(hcon->hdev))
		flags = ACL_START_NO_FLUSH;
	else
		flags = ACL_START;

	bt_cb(skb)->force_active = test_bit(FLAG_FORCE_ACTIVE, &chan->flags);
	hci_send_acl(chan->conn->hchan, skb, flags);
}

static inline void l2cap_send_sframe(struct l2cap_chan *chan, u32 control)
{
	struct sk_buff *skb;
	struct l2cap_hdr *lh;
	struct l2cap_conn *conn = chan->conn;
	int count, hlen;

	if (chan->state != BT_CONNECTED)
		return;

	if (test_bit(FLAG_EXT_CTRL, &chan->flags))
		hlen = L2CAP_EXT_HDR_SIZE;
	else
		hlen = L2CAP_ENH_HDR_SIZE;

	if (chan->fcs == L2CAP_FCS_CRC16)
		hlen += L2CAP_FCS_SIZE;

	BT_DBG("chan %p, control 0x%8.8x", chan, control);

	count = min_t(unsigned int, conn->mtu, hlen);

	control |= __set_sframe(chan);

	if (test_and_clear_bit(CONN_SEND_FBIT, &chan->conn_state))
		control |= __set_ctrl_final(chan);

	if (test_and_clear_bit(CONN_SEND_PBIT, &chan->conn_state))
		control |= __set_ctrl_poll(chan);

	skb = bt_skb_alloc(count, GFP_ATOMIC);
	if (!skb)
		return;

	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(hlen - L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(chan->dcid);

	__put_control(chan, control, skb_put(skb, __ctrl_size(chan)));

	if (chan->fcs == L2CAP_FCS_CRC16) {
		u16 fcs = crc16(0, (u8 *)lh, count - L2CAP_FCS_SIZE);
		put_unaligned_le16(fcs, skb_put(skb, L2CAP_FCS_SIZE));
	}

	skb->priority = HCI_PRIO_MAX;
	l2cap_do_send(chan, skb);
}

static inline void l2cap_send_rr_or_rnr(struct l2cap_chan *chan, u32 control)
{
	if (test_bit(CONN_LOCAL_BUSY, &chan->conn_state)) {
		control |= __set_ctrl_super(chan, L2CAP_SUPER_RNR);
		set_bit(CONN_RNR_SENT, &chan->conn_state);
	} else
		control |= __set_ctrl_super(chan, L2CAP_SUPER_RR);

	control |= __set_reqseq(chan, chan->buffer_seq);

	l2cap_send_sframe(chan, control);
}

static inline int __l2cap_no_conn_pending(struct l2cap_chan *chan)
{
	return !test_bit(CONF_CONNECT_PEND, &chan->conf_state);
}

static void l2cap_send_conn_req(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
	struct l2cap_conn_req req;

	req.scid = cpu_to_le16(chan->scid);
	req.psm  = chan->psm;

	chan->ident = l2cap_get_ident(conn);

	set_bit(CONF_CONNECT_PEND, &chan->conf_state);

	l2cap_send_cmd(conn, chan->ident, L2CAP_CONN_REQ, sizeof(req), &req);
}

static void l2cap_do_start(struct l2cap_chan *chan)
{
	struct l2cap_conn *conn = chan->conn;
>>>>>>> remotes/linux2/linux-3.4.y

	if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT) {
		if (!(conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE))
			return;

<<<<<<< HEAD
		if (l2cap_check_security(sk) && __l2cap_no_conn_pending(sk)) {
			l2cap_pi(sk)->conf_state |= L2CAP_CONF_CONNECT_PEND;

			if (l2cap_pi(sk)->amp_pref ==
					BT_AMP_POLICY_PREFER_AMP &&
					enable_hs &&
					conn->fc_mask & L2CAP_FC_A2MP)
				amp_create_physical(conn, sk);
			else
				l2cap_send_conn_req(sk);
		}
=======
		if (l2cap_chan_check_security(chan) &&
				__l2cap_no_conn_pending(chan))
			l2cap_send_conn_req(chan);
>>>>>>> remotes/linux2/linux-3.4.y
	} else {
		struct l2cap_info_req req;
		req.type = cpu_to_le16(L2CAP_IT_FEAT_MASK);

		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_SENT;
		conn->info_ident = l2cap_get_ident(conn);

<<<<<<< HEAD
		mod_timer(&conn->info_timer, jiffies +
					msecs_to_jiffies(L2CAP_INFO_TIMEOUT));
=======
		schedule_delayed_work(&conn->info_timer, L2CAP_INFO_TIMEOUT);
>>>>>>> remotes/linux2/linux-3.4.y

		l2cap_send_cmd(conn, conn->info_ident,
					L2CAP_INFO_REQ, sizeof(req), &req);
	}
}

static inline int l2cap_mode_supported(__u8 mode, __u32 feat_mask)
{
	u32 local_feat_mask = l2cap_feat_mask;
	if (!disable_ertm)
		local_feat_mask |= L2CAP_FEAT_ERTM | L2CAP_FEAT_STREAMING;

	switch (mode) {
	case L2CAP_MODE_ERTM:
		return L2CAP_FEAT_ERTM & feat_mask & local_feat_mask;
	case L2CAP_MODE_STREAMING:
		return L2CAP_FEAT_STREAMING & feat_mask & local_feat_mask;
	default:
		return 0x00;
	}
}

<<<<<<< HEAD
void l2cap_send_disconn_req(struct l2cap_conn *conn, struct sock *sk, int err)
{
=======
static void l2cap_send_disconn_req(struct l2cap_conn *conn, struct l2cap_chan *chan, int err)
{
	struct sock *sk = chan->sk;
>>>>>>> remotes/linux2/linux-3.4.y
	struct l2cap_disconn_req req;

	if (!conn)
		return;

<<<<<<< HEAD
	sk->sk_send_head = NULL;
	skb_queue_purge(TX_QUEUE(sk));

	if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM) {
		skb_queue_purge(SREJ_QUEUE(sk));

		__cancel_delayed_work(&l2cap_pi(sk)->ack_work);
		__cancel_delayed_work(&l2cap_pi(sk)->retrans_work);
		__cancel_delayed_work(&l2cap_pi(sk)->monitor_work);
	}

	req.dcid = cpu_to_le16(l2cap_pi(sk)->dcid);
	req.scid = cpu_to_le16(l2cap_pi(sk)->scid);
	l2cap_send_cmd(conn, l2cap_get_ident(conn),
			L2CAP_DISCONN_REQ, sizeof(req), &req);

	sk->sk_state = BT_DISCONN;
	sk->sk_err = err;
=======
	if (chan->mode == L2CAP_MODE_ERTM) {
		__clear_retrans_timer(chan);
		__clear_monitor_timer(chan);
		__clear_ack_timer(chan);
	}

	req.dcid = cpu_to_le16(chan->dcid);
	req.scid = cpu_to_le16(chan->scid);
	l2cap_send_cmd(conn, l2cap_get_ident(conn),
			L2CAP_DISCONN_REQ, sizeof(req), &req);

	lock_sock(sk);
	__l2cap_state_change(chan, BT_DISCONN);
	__l2cap_chan_set_err(chan, err);
	release_sock(sk);
>>>>>>> remotes/linux2/linux-3.4.y
}

/* ---- L2CAP connections ---- */
static void l2cap_conn_start(struct l2cap_conn *conn)
{
<<<<<<< HEAD
	struct l2cap_chan_list *l = &conn->chan_list;
	struct sock_del_list del, *tmp1, *tmp2;
	struct sock *sk;

	BT_DBG("conn %p", conn);

	INIT_LIST_HEAD(&del.list);

	read_lock(&l->lock);

	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		bh_lock_sock(sk);

		if (sk->sk_type != SOCK_SEQPACKET &&
				sk->sk_type != SOCK_STREAM) {
			bh_unlock_sock(sk);
			continue;
		}

		if (sk->sk_state == BT_CONNECT) {
			if (!l2cap_check_security(sk) ||
					!__l2cap_no_conn_pending(sk)) {
				bh_unlock_sock(sk);
				continue;
			}

			if (!l2cap_mode_supported(l2cap_pi(sk)->mode,
					conn->feat_mask)
					&& l2cap_pi(sk)->conf_state &
					L2CAP_CONF_STATE2_DEVICE) {
				tmp1 = kzalloc(sizeof(struct sock_del_list),
						GFP_ATOMIC);
				tmp1->sk = sk;
				list_add_tail(&tmp1->list, &del.list);
				bh_unlock_sock(sk);
				continue;
			}

			l2cap_pi(sk)->conf_state |= L2CAP_CONF_CONNECT_PEND;

			if (l2cap_pi(sk)->amp_pref ==
					BT_AMP_POLICY_PREFER_AMP &&
					enable_hs &&
					conn->fc_mask & L2CAP_FC_A2MP)
				amp_create_physical(conn, sk);
			else
				l2cap_send_conn_req(sk);

		} else if (sk->sk_state == BT_CONNECT2) {
			struct l2cap_conn_rsp rsp;
			char buf[128];
			rsp.scid = cpu_to_le16(l2cap_pi(sk)->dcid);
			rsp.dcid = cpu_to_le16(l2cap_pi(sk)->scid);

			if (l2cap_check_security(sk)) {
=======
	struct l2cap_chan *chan, *tmp;

	BT_DBG("conn %p", conn);

	mutex_lock(&conn->chan_lock);

	list_for_each_entry_safe(chan, tmp, &conn->chan_l, list) {
		struct sock *sk = chan->sk;

		l2cap_chan_lock(chan);

		if (chan->chan_type != L2CAP_CHAN_CONN_ORIENTED) {
			l2cap_chan_unlock(chan);
			continue;
		}

		if (chan->state == BT_CONNECT) {
			if (!l2cap_chan_check_security(chan) ||
					!__l2cap_no_conn_pending(chan)) {
				l2cap_chan_unlock(chan);
				continue;
			}

			if (!l2cap_mode_supported(chan->mode, conn->feat_mask)
					&& test_bit(CONF_STATE2_DEVICE,
					&chan->conf_state)) {
				l2cap_chan_close(chan, ECONNRESET);
				l2cap_chan_unlock(chan);
				continue;
			}

			l2cap_send_conn_req(chan);

		} else if (chan->state == BT_CONNECT2) {
			struct l2cap_conn_rsp rsp;
			char buf[128];
			rsp.scid = cpu_to_le16(chan->dcid);
			rsp.dcid = cpu_to_le16(chan->scid);

			if (l2cap_chan_check_security(chan)) {
				lock_sock(sk);
>>>>>>> remotes/linux2/linux-3.4.y
				if (bt_sk(sk)->defer_setup) {
					struct sock *parent = bt_sk(sk)->parent;
					rsp.result = cpu_to_le16(L2CAP_CR_PEND);
					rsp.status = cpu_to_le16(L2CAP_CS_AUTHOR_PEND);
					if (parent)
						parent->sk_data_ready(parent, 0);

				} else {
<<<<<<< HEAD
					sk->sk_state = BT_CONFIG;
					rsp.result = cpu_to_le16(L2CAP_CR_SUCCESS);
					rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
				}
=======
					__l2cap_state_change(chan, BT_CONFIG);
					rsp.result = cpu_to_le16(L2CAP_CR_SUCCESS);
					rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
				}
				release_sock(sk);
>>>>>>> remotes/linux2/linux-3.4.y
			} else {
				rsp.result = cpu_to_le16(L2CAP_CR_PEND);
				rsp.status = cpu_to_le16(L2CAP_CS_AUTHEN_PEND);
			}

<<<<<<< HEAD
			if (rsp.result == cpu_to_le16(L2CAP_CR_SUCCESS) &&
					l2cap_pi(sk)->amp_id) {
				amp_accept_physical(conn,
						l2cap_pi(sk)->amp_id, sk);
				bh_unlock_sock(sk);
				continue;
			}

			l2cap_send_cmd(conn, l2cap_pi(sk)->ident,
					L2CAP_CONN_RSP, sizeof(rsp), &rsp);

			if (l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT ||
					rsp.result != L2CAP_CR_SUCCESS) {
				bh_unlock_sock(sk);
				continue;
			}

			l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;
			l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
						l2cap_build_conf_req(sk, buf), buf);
			l2cap_pi(sk)->num_conf_req++;
		}

		bh_unlock_sock(sk);
	}

	read_unlock(&l->lock);

	list_for_each_entry_safe(tmp1, tmp2, &del.list, list) {
		bh_lock_sock(tmp1->sk);
		__l2cap_sock_close(tmp1->sk, ECONNRESET);
		bh_unlock_sock(tmp1->sk);
		list_del(&tmp1->list);
		kfree(tmp1);
	}
}

/* Find socket with fixed cid with given source and destination bdaddrs.
 * Direction of the req/rsp must match.
 */
struct sock *l2cap_find_sock_by_fixed_cid_and_dir(__le16 cid, bdaddr_t *src,
						bdaddr_t *dst, int incoming)
{
	struct sock *sk = NULL, *sk1 = NULL;
	struct hlist_node *node;

	BT_DBG(" %d", incoming);

	read_lock(&l2cap_sk_list.lock);

	sk_for_each(sk, node, &l2cap_sk_list.head) {

		if (incoming && !l2cap_pi(sk)->incoming)
			continue;

		if (!incoming && l2cap_pi(sk)->incoming)
			continue;

		if (l2cap_pi(sk)->scid == cid && !bacmp(&bt_sk(sk)->dst, dst)) {
			/* Exact match. */
			if (!bacmp(&bt_sk(sk)->src, src))
				break;

			/* Closest match */
			if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY))
				sk1 = sk;
		}
	}

	read_unlock(&l2cap_sk_list.lock);

	return node ? sk : sk1;
}

/* Find socket with cid and source bdaddr.
 * Returns closest match, locked.
 */
static struct sock *l2cap_get_sock_by_scid(int state, __le16 cid, bdaddr_t *src)
{
	struct sock *sk = NULL, *sk1 = NULL;
	struct hlist_node *node;

	read_lock(&l2cap_sk_list.lock);

	sk_for_each(sk, node, &l2cap_sk_list.head) {
		if (state && sk->sk_state != state)
			continue;

		if (l2cap_pi(sk)->scid == cid) {
			/* Exact match. */
			if (!bacmp(&bt_sk(sk)->src, src))
				break;

			/* Closest match */
			if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY))
				sk1 = sk;
		}
	}

	read_unlock(&l2cap_sk_list.lock);

	return node ? sk : sk1;
}

static void l2cap_le_conn_ready(struct l2cap_conn *conn)
{
	struct l2cap_chan_list *list = &conn->chan_list;
	struct sock *parent, *uninitialized_var(sk);

	BT_DBG("");

	/* Check if we have socket listening on cid */
	parent = l2cap_get_sock_by_scid(BT_LISTEN, L2CAP_CID_LE_DATA,
							conn->src);
	if (!parent)
		return;

	bh_lock_sock(parent);
=======
			l2cap_send_cmd(conn, chan->ident, L2CAP_CONN_RSP,
							sizeof(rsp), &rsp);

			if (test_bit(CONF_REQ_SENT, &chan->conf_state) ||
					rsp.result != L2CAP_CR_SUCCESS) {
				l2cap_chan_unlock(chan);
				continue;
			}

			set_bit(CONF_REQ_SENT, &chan->conf_state);
			l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
						l2cap_build_conf_req(chan, buf), buf);
			chan->num_conf_req++;
		}

		l2cap_chan_unlock(chan);
	}

	mutex_unlock(&conn->chan_lock);
}

/* Find socket with cid and source bdaddr.
 * Returns closest match, locked.
 */
static struct l2cap_chan *l2cap_global_chan_by_scid(int state, __le16 cid, bdaddr_t *src)
{
	struct l2cap_chan *c, *c1 = NULL;

	read_lock(&chan_list_lock);

	list_for_each_entry(c, &chan_list, global_l) {
		struct sock *sk = c->sk;

		if (state && c->state != state)
			continue;

		if (c->scid == cid) {
			/* Exact match. */
			if (!bacmp(&bt_sk(sk)->src, src)) {
				read_unlock(&chan_list_lock);
				return c;
			}

			/* Closest match */
			if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY))
				c1 = c;
		}
	}

	read_unlock(&chan_list_lock);

	return c1;
}

static void l2cap_le_conn_ready(struct l2cap_conn *conn)
{
	struct sock *parent, *sk;
	struct l2cap_chan *chan, *pchan;

	BT_DBG("");

	/* Check if we have socket listening on cid */
	pchan = l2cap_global_chan_by_scid(BT_LISTEN, L2CAP_CID_LE_DATA,
							conn->src);
	if (!pchan)
		return;

	parent = pchan->sk;

	lock_sock(parent);
>>>>>>> remotes/linux2/linux-3.4.y

	/* Check for backlog size */
	if (sk_acceptq_is_full(parent)) {
		BT_DBG("backlog full %d", parent->sk_ack_backlog);
		goto clean;
	}

<<<<<<< HEAD
	sk = l2cap_sock_alloc(sock_net(parent), NULL, BTPROTO_L2CAP, GFP_ATOMIC);
	if (!sk)
		goto clean;

	write_lock_bh(&list->lock);

	hci_conn_hold(conn->hcon);

	l2cap_sock_init(sk, parent);
	bacpy(&bt_sk(sk)->src, conn->src);
	bacpy(&bt_sk(sk)->dst, conn->dst);
	l2cap_pi(sk)->incoming = 1;

	bt_accept_enqueue(parent, sk);

	__l2cap_chan_add(conn, sk);

	sk->sk_state = BT_CONNECTED;
	parent->sk_data_ready(parent, 0);

	write_unlock_bh(&list->lock);

clean:
	bh_unlock_sock(parent);
=======
	chan = pchan->ops->new_connection(pchan->data);
	if (!chan)
		goto clean;

	sk = chan->sk;

	hci_conn_hold(conn->hcon);
	conn->hcon->disc_timeout = HCI_DISCONN_TIMEOUT;

	bacpy(&bt_sk(sk)->src, conn->src);
	bacpy(&bt_sk(sk)->dst, conn->dst);

	bt_accept_enqueue(parent, sk);

	l2cap_chan_add(conn, chan);

	__set_chan_timer(chan, sk->sk_sndtimeo);

	__l2cap_state_change(chan, BT_CONNECTED);
	parent->sk_data_ready(parent, 0);

clean:
	release_sock(parent);
}

static void l2cap_chan_ready(struct l2cap_chan *chan)
{
	struct sock *sk = chan->sk;
	struct sock *parent;

	lock_sock(sk);

	parent = bt_sk(sk)->parent;

	BT_DBG("sk %p, parent %p", sk, parent);

	chan->conf_state = 0;
	__clear_chan_timer(chan);

	__l2cap_state_change(chan, BT_CONNECTED);
	sk->sk_state_change(sk);

	if (parent)
		parent->sk_data_ready(parent, 0);

	release_sock(sk);
>>>>>>> remotes/linux2/linux-3.4.y
}

static void l2cap_conn_ready(struct l2cap_conn *conn)
{
<<<<<<< HEAD
	struct l2cap_chan_list *l = &conn->chan_list;
	struct sock *sk;

	BT_DBG("conn %p", conn);

	if (!conn->hcon->out && conn->hcon->type == LE_LINK)
		l2cap_le_conn_ready(conn);

	read_lock(&l->lock);

	if (l->head) {
		for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
			bh_lock_sock(sk);

			if (conn->hcon->type == LE_LINK) {
				u8 sec_level = l2cap_pi(sk)->sec_level;
				u8 pending_sec = conn->hcon->pending_sec_level;

				if (pending_sec > sec_level)
					sec_level = pending_sec;

				if (smp_conn_security(conn, sec_level))
					l2cap_chan_ready(sk);

				hci_conn_put(conn->hcon);

			} else if (sk->sk_type != SOCK_SEQPACKET &&
					sk->sk_type != SOCK_STREAM) {
				l2cap_sock_clear_timer(sk);
				sk->sk_state = BT_CONNECTED;
				sk->sk_state_change(sk);
			} else if (sk->sk_state == BT_CONNECT)
				l2cap_do_start(sk);

			bh_unlock_sock(sk);
		}
	} else if (conn->hcon->type == LE_LINK) {
		smp_conn_security(conn, BT_SECURITY_HIGH);
	}

	read_unlock(&l->lock);

	if (conn->hcon->out && conn->hcon->type == LE_LINK)
		l2cap_le_conn_ready(conn);
=======
	struct l2cap_chan *chan;
	struct hci_conn *hcon = conn->hcon;

	BT_DBG("conn %p", conn);

	if (!hcon->out && hcon->type == LE_LINK)
		l2cap_le_conn_ready(conn);

	if (hcon->out && hcon->type == LE_LINK)
		smp_conn_security(hcon, hcon->pending_sec_level);

	mutex_lock(&conn->chan_lock);

	list_for_each_entry(chan, &conn->chan_l, list) {

		l2cap_chan_lock(chan);

		if (hcon->type == LE_LINK) {
			if (smp_conn_security(hcon, chan->sec_level))
				l2cap_chan_ready(chan);

		} else if (chan->chan_type != L2CAP_CHAN_CONN_ORIENTED) {
			struct sock *sk = chan->sk;
			__clear_chan_timer(chan);
			lock_sock(sk);
			__l2cap_state_change(chan, BT_CONNECTED);
			sk->sk_state_change(sk);
			release_sock(sk);

		} else if (chan->state == BT_CONNECT)
			l2cap_do_start(chan);

		l2cap_chan_unlock(chan);
	}

	mutex_unlock(&conn->chan_lock);
>>>>>>> remotes/linux2/linux-3.4.y
}

/* Notify sockets that we cannot guaranty reliability anymore */
static void l2cap_conn_unreliable(struct l2cap_conn *conn, int err)
{
<<<<<<< HEAD
	struct l2cap_chan_list *l = &conn->chan_list;
	struct sock *sk;

	BT_DBG("conn %p", conn);

	read_lock(&l->lock);

	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		if (l2cap_pi(sk)->force_reliable)
			sk->sk_err = err;
	}

	read_unlock(&l->lock);
}

static void l2cap_info_timeout(unsigned long arg)
{
	struct l2cap_conn *conn = (void *) arg;
=======
	struct l2cap_chan *chan;

	BT_DBG("conn %p", conn);

	mutex_lock(&conn->chan_lock);

	list_for_each_entry(chan, &conn->chan_l, list) {
		if (test_bit(FLAG_FORCE_RELIABLE, &chan->flags))
			__l2cap_chan_set_err(chan, err);
	}

	mutex_unlock(&conn->chan_lock);
}

static void l2cap_info_timeout(struct work_struct *work)
{
	struct l2cap_conn *conn = container_of(work, struct l2cap_conn,
							info_timer.work);
>>>>>>> remotes/linux2/linux-3.4.y

	conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
	conn->info_ident = 0;

	l2cap_conn_start(conn);
}

<<<<<<< HEAD
static struct l2cap_conn *l2cap_conn_add(struct hci_conn *hcon, u8 status)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
=======
static void l2cap_conn_del(struct hci_conn *hcon, int err)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct l2cap_chan *chan, *l;

	if (!conn)
		return;

	BT_DBG("hcon %p conn %p, err %d", hcon, conn, err);

	kfree_skb(conn->rx_skb);

	mutex_lock(&conn->chan_lock);

	/* Kill channels */
	list_for_each_entry_safe(chan, l, &conn->chan_l, list) {
		l2cap_chan_lock(chan);

		l2cap_chan_del(chan, err);

		l2cap_chan_unlock(chan);

		chan->ops->close(chan->data);
	}

	mutex_unlock(&conn->chan_lock);

	hci_chan_del(conn->hchan);

	if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT)
		cancel_delayed_work_sync(&conn->info_timer);

	if (test_and_clear_bit(HCI_CONN_LE_SMP_PEND, &hcon->flags)) {
		cancel_delayed_work_sync(&conn->security_timer);
		smp_chan_destroy(conn);
	}

	hcon->l2cap_data = NULL;
	kfree(conn);
}

static void security_timeout(struct work_struct *work)
{
	struct l2cap_conn *conn = container_of(work, struct l2cap_conn,
						security_timer.work);

	l2cap_conn_del(conn->hcon, ETIMEDOUT);
}

static struct l2cap_conn *l2cap_conn_add(struct hci_conn *hcon, u8 status)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct hci_chan *hchan;
>>>>>>> remotes/linux2/linux-3.4.y

	if (conn || status)
		return conn;

<<<<<<< HEAD
	conn = kzalloc(sizeof(struct l2cap_conn), GFP_ATOMIC);
	if (!conn)
		return NULL;

	hcon->l2cap_data = conn;
	conn->hcon = hcon;

	BT_DBG("hcon %p conn %p", hcon, conn);
=======
	hchan = hci_chan_create(hcon);
	if (!hchan)
		return NULL;

	conn = kzalloc(sizeof(struct l2cap_conn), GFP_ATOMIC);
	if (!conn) {
		hci_chan_del(hchan);
		return NULL;
	}

	hcon->l2cap_data = conn;
	conn->hcon = hcon;
	conn->hchan = hchan;

	BT_DBG("hcon %p conn %p hchan %p", hcon, conn, hchan);
>>>>>>> remotes/linux2/linux-3.4.y

	if (hcon->hdev->le_mtu && hcon->type == LE_LINK)
		conn->mtu = hcon->hdev->le_mtu;
	else
		conn->mtu = hcon->hdev->acl_mtu;

	conn->src = &hcon->hdev->bdaddr;
	conn->dst = &hcon->dst;

	conn->feat_mask = 0;

	spin_lock_init(&conn->lock);
<<<<<<< HEAD
	rwlock_init(&conn->chan_list.lock);

	if (hcon->type == LE_LINK)
		setup_timer(&hcon->smp_timer, smp_timeout,
						(unsigned long) conn);
	else
		setup_timer(&conn->info_timer, l2cap_info_timeout,
						(unsigned long) conn);

	conn->disc_reason = 0x13;
=======
	mutex_init(&conn->chan_lock);

	INIT_LIST_HEAD(&conn->chan_l);

	if (hcon->type == LE_LINK)
		INIT_DELAYED_WORK(&conn->security_timer, security_timeout);
	else
		INIT_DELAYED_WORK(&conn->info_timer, l2cap_info_timeout);

	conn->disc_reason = HCI_ERROR_REMOTE_USER_TERM;
>>>>>>> remotes/linux2/linux-3.4.y

	return conn;
}

<<<<<<< HEAD
static void l2cap_conn_del(struct hci_conn *hcon, int err, u8 is_process)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct sock *sk;
	struct sock *next;

	if (!conn)
		return;

	BT_DBG("hcon %p conn %p, err %d", hcon, conn, err);

	if ((conn->hcon == hcon) && (conn->rx_skb))
		kfree_skb(conn->rx_skb);

	BT_DBG("conn->hcon %p", conn->hcon);

	/* Kill channels */
	for (sk = conn->chan_list.head; sk; ) {
		BT_DBG("ampcon %p", l2cap_pi(sk)->ampcon);
		if ((conn->hcon == hcon) || (l2cap_pi(sk)->ampcon == hcon)) {
			next = l2cap_pi(sk)->next_c;
			if (is_process)
				lock_sock(sk);
			else
				bh_lock_sock(sk);
			l2cap_chan_del(sk, err);
			if (is_process)
				release_sock(sk);
			else
				bh_unlock_sock(sk);
			l2cap_sock_kill(sk);
			sk = next;
		} else
			sk = l2cap_pi(sk)->next_c;
	}

	if (conn->hcon == hcon) {
		if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT)
			del_timer_sync(&conn->info_timer);

		hcon->l2cap_data = NULL;

		kfree(conn);
	}
}

static inline void l2cap_chan_add(struct l2cap_conn *conn, struct sock *sk)
{
	struct l2cap_chan_list *l = &conn->chan_list;
	write_lock_bh(&l->lock);
	__l2cap_chan_add(conn, sk);
	write_unlock_bh(&l->lock);
}

=======
>>>>>>> remotes/linux2/linux-3.4.y
/* ---- Socket interface ---- */

/* Find socket with psm and source bdaddr.
 * Returns closest match.
 */
<<<<<<< HEAD
static struct sock *l2cap_get_sock_by_psm(int state, __le16 psm, bdaddr_t *src)
{
	struct sock *sk = NULL, *sk1 = NULL;
	struct hlist_node *node;

	read_lock(&l2cap_sk_list.lock);

	sk_for_each(sk, node, &l2cap_sk_list.head) {
		if (state && sk->sk_state != state)
			continue;

		if (l2cap_pi(sk)->psm == psm) {
			/* Exact match. */
			if (!bacmp(&bt_sk(sk)->src, src))
				break;

			/* Closest match */
			if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY))
				sk1 = sk;
		}
	}

	read_unlock(&l2cap_sk_list.lock);

	return node ? sk : sk1;
}

int l2cap_do_connect(struct sock *sk)
{
	bdaddr_t *src = &bt_sk(sk)->src;
	bdaddr_t *dst = &bt_sk(sk)->dst;
=======
static struct l2cap_chan *l2cap_global_chan_by_psm(int state, __le16 psm, bdaddr_t *src)
{
	struct l2cap_chan *c, *c1 = NULL;

	read_lock(&chan_list_lock);

	list_for_each_entry(c, &chan_list, global_l) {
		struct sock *sk = c->sk;

		if (state && c->state != state)
			continue;

		if (c->psm == psm) {
			/* Exact match. */
			if (!bacmp(&bt_sk(sk)->src, src)) {
				read_unlock(&chan_list_lock);
				return c;
			}

			/* Closest match */
			if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY))
				c1 = c;
		}
	}

	read_unlock(&chan_list_lock);

	return c1;
}

int l2cap_chan_connect(struct l2cap_chan *chan, __le16 psm, u16 cid, bdaddr_t *dst)
{
	struct sock *sk = chan->sk;
	bdaddr_t *src = &bt_sk(sk)->src;
>>>>>>> remotes/linux2/linux-3.4.y
	struct l2cap_conn *conn;
	struct hci_conn *hcon;
	struct hci_dev *hdev;
	__u8 auth_type;
	int err;

	BT_DBG("%s -> %s psm 0x%2.2x", batostr(src), batostr(dst),
<<<<<<< HEAD
							l2cap_pi(sk)->psm);
=======
							chan->psm);
>>>>>>> remotes/linux2/linux-3.4.y

	hdev = hci_get_route(dst, src);
	if (!hdev)
		return -EHOSTUNREACH;

<<<<<<< HEAD
	hci_dev_lock_bh(hdev);

	auth_type = l2cap_get_auth_type(sk);

	if (l2cap_pi(sk)->fixed_channel) {
		/* Fixed channels piggyback on existing ACL connections */
		hcon = hci_conn_hash_lookup_ba(hdev, ACL_LINK, dst);
		if (!hcon || !hcon->l2cap_data) {
			err = -ENOTCONN;
			goto done;
		}

		conn = hcon->l2cap_data;
	} else {
		if (l2cap_pi(sk)->dcid == L2CAP_CID_LE_DATA)
			hcon = hci_le_connect(hdev, 0, dst,
					l2cap_pi(sk)->sec_level, auth_type,
					&bt_sk(sk)->le_params);
		else
			hcon = hci_connect(hdev, ACL_LINK, 0, dst,
					l2cap_pi(sk)->sec_level, auth_type);

		if (IS_ERR(hcon)) {
			err = PTR_ERR(hcon);
			goto done;
		}

		conn = l2cap_conn_add(hcon, 0);
		if (!conn) {
			hci_conn_put(hcon);
			err = -ENOMEM;
			goto done;
		}
=======
	hci_dev_lock(hdev);

	l2cap_chan_lock(chan);

	/* PSM must be odd and lsb of upper byte must be 0 */
	if ((__le16_to_cpu(psm) & 0x0101) != 0x0001 && !cid &&
					chan->chan_type != L2CAP_CHAN_RAW) {
		err = -EINVAL;
		goto done;
	}

	if (chan->chan_type == L2CAP_CHAN_CONN_ORIENTED && !(psm || cid)) {
		err = -EINVAL;
		goto done;
	}

	switch (chan->mode) {
	case L2CAP_MODE_BASIC:
		break;
	case L2CAP_MODE_ERTM:
	case L2CAP_MODE_STREAMING:
		if (!disable_ertm)
			break;
		/* fall through */
	default:
		err = -ENOTSUPP;
		goto done;
	}

	lock_sock(sk);

	switch (sk->sk_state) {
	case BT_CONNECT:
	case BT_CONNECT2:
	case BT_CONFIG:
		/* Already connecting */
		err = 0;
		release_sock(sk);
		goto done;

	case BT_CONNECTED:
		/* Already connected */
		err = -EISCONN;
		release_sock(sk);
		goto done;

	case BT_OPEN:
	case BT_BOUND:
		/* Can connect */
		break;

	default:
		err = -EBADFD;
		release_sock(sk);
		goto done;
	}

	/* Set destination address and psm */
	bacpy(&bt_sk(sk)->dst, dst);

	release_sock(sk);

	chan->psm = psm;
	chan->dcid = cid;

	auth_type = l2cap_get_auth_type(chan);

	if (chan->dcid == L2CAP_CID_LE_DATA)
		hcon = hci_connect(hdev, LE_LINK, dst,
					chan->sec_level, auth_type);
	else
		hcon = hci_connect(hdev, ACL_LINK, dst,
					chan->sec_level, auth_type);

	if (IS_ERR(hcon)) {
		err = PTR_ERR(hcon);
		goto done;
	}

	conn = l2cap_conn_add(hcon, 0);
	if (!conn) {
		hci_conn_put(hcon);
		err = -ENOMEM;
		goto done;
>>>>>>> remotes/linux2/linux-3.4.y
	}

	/* Update source addr of the socket */
	bacpy(src, conn->src);

<<<<<<< HEAD
	l2cap_chan_add(conn, sk);

	if ((l2cap_pi(sk)->fixed_channel) ||
			(l2cap_pi(sk)->dcid == L2CAP_CID_LE_DATA &&
				hcon->state == BT_CONNECTED)) {
		sk->sk_state = BT_CONNECTED;
		sk->sk_state_change(sk);
	} else {
		sk->sk_state = BT_CONNECT;
		/* If we have valid LE Params, let timeout override default */
		if (l2cap_pi(sk)->dcid == L2CAP_CID_LE_DATA &&
			l2cap_sock_le_params_valid(&bt_sk(sk)->le_params)) {
			u16 timeout = bt_sk(sk)->le_params.conn_timeout;

			if (timeout)
				l2cap_sock_set_timer(sk,
						msecs_to_jiffies(timeout*1000));
		} else
			l2cap_sock_set_timer(sk, sk->sk_sndtimeo);

		sk->sk_state_change(sk);

		if (hcon->state == BT_CONNECTED) {
			if (sk->sk_type != SOCK_SEQPACKET &&
				sk->sk_type != SOCK_STREAM) {
				l2cap_sock_clear_timer(sk);
				if (l2cap_check_security(sk)) {
					sk->sk_state = BT_CONNECTED;
					sk->sk_state_change(sk);
				}
			} else
				l2cap_do_start(sk);
		}
=======
	l2cap_chan_unlock(chan);
	l2cap_chan_add(conn, chan);
	l2cap_chan_lock(chan);

	l2cap_state_change(chan, BT_CONNECT);
	__set_chan_timer(chan, sk->sk_sndtimeo);

	if (hcon->state == BT_CONNECTED) {
		if (chan->chan_type != L2CAP_CHAN_CONN_ORIENTED) {
			__clear_chan_timer(chan);
			if (l2cap_chan_check_security(chan))
				l2cap_state_change(chan, BT_CONNECTED);
		} else
			l2cap_do_start(chan);
>>>>>>> remotes/linux2/linux-3.4.y
	}

	err = 0;

done:
<<<<<<< HEAD
	hci_dev_unlock_bh(hdev);
=======
	l2cap_chan_unlock(chan);
	hci_dev_unlock(hdev);
>>>>>>> remotes/linux2/linux-3.4.y
	hci_dev_put(hdev);
	return err;
}

int __l2cap_wait_ack(struct sock *sk)
{
<<<<<<< HEAD
=======
	struct l2cap_chan *chan = l2cap_pi(sk)->chan;
>>>>>>> remotes/linux2/linux-3.4.y
	DECLARE_WAITQUEUE(wait, current);
	int err = 0;
	int timeo = HZ/5;

	add_wait_queue(sk_sleep(sk), &wait);
<<<<<<< HEAD
	while (l2cap_pi(sk)->unacked_frames > 0 && l2cap_pi(sk)->conn &&
		atomic_read(&l2cap_pi(sk)->ertm_queued)) {
		set_current_state(TASK_INTERRUPTIBLE);

=======
	set_current_state(TASK_INTERRUPTIBLE);
	while (chan->unacked_frames > 0 && chan->conn) {
>>>>>>> remotes/linux2/linux-3.4.y
		if (!timeo)
			timeo = HZ/5;

		if (signal_pending(current)) {
			err = sock_intr_errno(timeo);
			break;
		}

		release_sock(sk);
		timeo = schedule_timeout(timeo);
		lock_sock(sk);
<<<<<<< HEAD
=======
		set_current_state(TASK_INTERRUPTIBLE);
>>>>>>> remotes/linux2/linux-3.4.y

		err = sock_error(sk);
		if (err)
			break;
	}
	set_current_state(TASK_RUNNING);
	remove_wait_queue(sk_sleep(sk), &wait);
	return err;
}

<<<<<<< HEAD
static void l2cap_ertm_tx_worker(struct work_struct *work)
{
	struct l2cap_pinfo *pi =
		container_of(work, struct l2cap_pinfo, tx_work);
	struct sock *sk = (struct sock *)pi;
	BT_DBG("%p", pi);

	lock_sock(sk);
	l2cap_ertm_send(sk);
	release_sock(sk);
	sock_put(sk);
}

static void l2cap_skb_destructor(struct sk_buff *skb)
{
	struct sock *sk = skb->sk;
	int queued;
	int keep_sk = 0;

	queued = atomic_sub_return(1, &l2cap_pi(sk)->ertm_queued);
	if (queued < L2CAP_MIN_ERTM_QUEUED)
		keep_sk = queue_work(_l2cap_wq, &l2cap_pi(sk)->tx_work);

	if (!keep_sk)
		sock_put(sk);
}

void l2cap_do_send(struct sock *sk, struct sk_buff *skb)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);

	BT_DBG("sk %p, skb %p len %d", sk, skb, skb->len);

	if (pi->ampcon && (pi->amp_move_state == L2CAP_AMP_STATE_STABLE ||
			pi->amp_move_state == L2CAP_AMP_STATE_WAIT_PREPARE)) {
		BT_DBG("Sending on AMP connection %p %p",
			pi->ampcon, pi->ampchan);
		if (pi->ampchan)
			hci_send_acl(pi->ampcon, pi->ampchan, skb,
					ACL_COMPLETE);
		else
			kfree_skb(skb);
	} else {
		u16 flags;

		bt_cb(skb)->force_active = pi->force_active;
		BT_DBG("Sending on BR/EDR connection %p", pi->conn->hcon);

		if (lmp_no_flush_capable(pi->conn->hcon->hdev) &&
			!l2cap_pi(sk)->flushable)
			flags = ACL_START_NO_FLUSH;
		else
			flags = ACL_START;

		hci_send_acl(pi->conn->hcon, NULL, skb, flags);
	}
}

int l2cap_ertm_send(struct sock *sk)
{
	struct sk_buff *skb, *tx_skb;
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct bt_l2cap_control *control;
	int sent = 0;

	BT_DBG("sk %p", sk);

	if (sk->sk_state != BT_CONNECTED)
		return -ENOTCONN;

	if (pi->conn_state & L2CAP_CONN_REMOTE_BUSY)
		return 0;

	if (pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
			pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE)
		return 0;

	while (sk->sk_send_head && (pi->unacked_frames < pi->remote_tx_win) &&
		atomic_read(&pi->ertm_queued) < L2CAP_MAX_ERTM_QUEUED &&
		(pi->tx_state == L2CAP_ERTM_TX_STATE_XMIT)) {

		skb = sk->sk_send_head;

		bt_cb(skb)->retries = 1;
		control = &bt_cb(skb)->control;

		if (pi->conn_state & L2CAP_CONN_SEND_FBIT) {
			control->final = 1;
			pi->conn_state &= ~L2CAP_CONN_SEND_FBIT;
		}
		control->reqseq = pi->buffer_seq;
		pi->last_acked_seq = pi->buffer_seq;
		control->txseq = pi->next_tx_seq;

		if (pi->extended_control) {
			put_unaligned_le32(__pack_extended_control(control),
					skb->data + L2CAP_HDR_SIZE);
		} else {
			put_unaligned_le16(__pack_enhanced_control(control),
					skb->data + L2CAP_HDR_SIZE);
		}

		if (pi->fcs == L2CAP_FCS_CRC16)
			apply_fcs(skb);

		/* Clone after data has been modified. Data is assumed to be
		   read-only (for locking purposes) on cloned sk_buffs.
		 */
		tx_skb = skb_clone(skb, GFP_ATOMIC);

		if (!tx_skb)
			break;

		sock_hold(sk);
		tx_skb->sk = sk;
		tx_skb->destructor = l2cap_skb_destructor;
		atomic_inc(&pi->ertm_queued);

		l2cap_ertm_start_retrans_timer(pi);

		pi->next_tx_seq = __next_seq(pi->next_tx_seq, pi);
		pi->unacked_frames += 1;
		pi->frames_sent += 1;
		sent += 1;

		if (skb_queue_is_last(TX_QUEUE(sk), skb))
			sk->sk_send_head = NULL;
		else
			sk->sk_send_head = skb_queue_next(TX_QUEUE(sk), skb);

		l2cap_do_send(sk, tx_skb);
		BT_DBG("Sent txseq %d", (int)control->txseq);
	}

	BT_DBG("Sent %d, %d unacked, %d in ERTM queue, %d in HCI queue", sent,
		(int) pi->unacked_frames, skb_queue_len(TX_QUEUE(sk)),
		atomic_read(&pi->ertm_queued));

	return sent;
}

int l2cap_strm_tx(struct sock *sk, struct sk_buff_head *skbs)
{
	struct sk_buff *skb;
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct bt_l2cap_control *control;
	int sent = 0;

	BT_DBG("sk %p, skbs %p", sk, skbs);

	if (sk->sk_state != BT_CONNECTED)
		return -ENOTCONN;

	if (pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
			pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE)
		return 0;

	skb_queue_splice_tail_init(skbs, TX_QUEUE(sk));

	BT_DBG("skb queue empty 0x%2.2x", skb_queue_empty(TX_QUEUE(sk)));
	while (!skb_queue_empty(TX_QUEUE(sk))) {

		skb = skb_dequeue(TX_QUEUE(sk));

		BT_DBG("skb %p", skb);

		bt_cb(skb)->retries = 1;
		control = &bt_cb(skb)->control;

		BT_DBG("control %p", control);

		control->reqseq = 0;
		control->txseq = pi->next_tx_seq;

		if (pi->extended_control) {
			put_unaligned_le32(__pack_extended_control(control),
					skb->data + L2CAP_HDR_SIZE);
		} else {
			put_unaligned_le16(__pack_enhanced_control(control),
					skb->data + L2CAP_HDR_SIZE);
		}

		if (pi->fcs == L2CAP_FCS_CRC16)
			apply_fcs(skb);

		l2cap_do_send(sk, skb);

		BT_DBG("Sent txseq %d", (int)control->txseq);

		pi->next_tx_seq = __next_seq(pi->next_tx_seq, pi);
		pi->frames_sent += 1;
		sent += 1;
	}

	BT_DBG("Sent %d", sent);

	return 0;
}

static int memcpy_fromkvec(unsigned char *kdata, struct kvec *iv, int len)
{
	while (len > 0) {
		if (iv->iov_len) {
			int copy = min_t(unsigned int, len, iv->iov_len);
			memcpy(kdata, iv->iov_base, copy);
			len -= copy;
			kdata += copy;
			iv->iov_base += copy;
			iv->iov_len -= copy;
		}
		iv++;
	}

	return 0;
}

static inline int l2cap_skbuff_fromiovec(struct sock *sk, struct msghdr *msg,
					int len, int count, struct sk_buff *skb,
					int reseg)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	struct sk_buff **frag;
	struct sk_buff *final;
	int err, sent = 0;

	BT_DBG("sk %p, msg %p, len %d, count %d, skb %p", sk,
		msg, (int)len, (int)count, skb);

	if (!conn)
		return -ENOTCONN;

	/* When resegmenting, data is copied from kernel space */
	if (reseg) {
		err = memcpy_fromkvec(skb_put(skb, count),
				(struct kvec *) msg->msg_iov, count);
	} else {
		err = memcpy_fromiovec(skb_put(skb, count), msg->msg_iov,
					count);
	}

	if (err)
=======
static void l2cap_monitor_timeout(struct work_struct *work)
{
	struct l2cap_chan *chan = container_of(work, struct l2cap_chan,
							monitor_timer.work);

	BT_DBG("chan %p", chan);

	l2cap_chan_lock(chan);

	if (chan->retry_count >= chan->remote_max_tx) {
		l2cap_send_disconn_req(chan->conn, chan, ECONNABORTED);
		l2cap_chan_unlock(chan);
		l2cap_chan_put(chan);
		return;
	}

	chan->retry_count++;
	__set_monitor_timer(chan);

	l2cap_send_rr_or_rnr(chan, L2CAP_CTRL_POLL);
	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);
}

static void l2cap_retrans_timeout(struct work_struct *work)
{
	struct l2cap_chan *chan = container_of(work, struct l2cap_chan,
							retrans_timer.work);

	BT_DBG("chan %p", chan);

	l2cap_chan_lock(chan);

	chan->retry_count = 1;
	__set_monitor_timer(chan);

	set_bit(CONN_WAIT_F, &chan->conn_state);

	l2cap_send_rr_or_rnr(chan, L2CAP_CTRL_POLL);

	l2cap_chan_unlock(chan);
	l2cap_chan_put(chan);
}

static void l2cap_drop_acked_frames(struct l2cap_chan *chan)
{
	struct sk_buff *skb;

	while ((skb = skb_peek(&chan->tx_q)) &&
			chan->unacked_frames) {
		if (bt_cb(skb)->tx_seq == chan->expected_ack_seq)
			break;

		skb = skb_dequeue(&chan->tx_q);
		kfree_skb(skb);

		chan->unacked_frames--;
	}

	if (!chan->unacked_frames)
		__clear_retrans_timer(chan);
}

static void l2cap_streaming_send(struct l2cap_chan *chan)
{
	struct sk_buff *skb;
	u32 control;
	u16 fcs;

	while ((skb = skb_dequeue(&chan->tx_q))) {
		control = __get_control(chan, skb->data + L2CAP_HDR_SIZE);
		control |= __set_txseq(chan, chan->next_tx_seq);
		__put_control(chan, control, skb->data + L2CAP_HDR_SIZE);

		if (chan->fcs == L2CAP_FCS_CRC16) {
			fcs = crc16(0, (u8 *)skb->data,
						skb->len - L2CAP_FCS_SIZE);
			put_unaligned_le16(fcs,
					skb->data + skb->len - L2CAP_FCS_SIZE);
		}

		l2cap_do_send(chan, skb);

		chan->next_tx_seq = __next_seq(chan, chan->next_tx_seq);
	}
}

static void l2cap_retransmit_one_frame(struct l2cap_chan *chan, u16 tx_seq)
{
	struct sk_buff *skb, *tx_skb;
	u16 fcs;
	u32 control;

	skb = skb_peek(&chan->tx_q);
	if (!skb)
		return;

	while (bt_cb(skb)->tx_seq != tx_seq) {
		if (skb_queue_is_last(&chan->tx_q, skb))
			return;

		skb = skb_queue_next(&chan->tx_q, skb);
	}

	if (chan->remote_max_tx &&
			bt_cb(skb)->retries == chan->remote_max_tx) {
		l2cap_send_disconn_req(chan->conn, chan, ECONNABORTED);
		return;
	}

	tx_skb = skb_clone(skb, GFP_ATOMIC);
	bt_cb(skb)->retries++;

	control = __get_control(chan, tx_skb->data + L2CAP_HDR_SIZE);
	control &= __get_sar_mask(chan);

	if (test_and_clear_bit(CONN_SEND_FBIT, &chan->conn_state))
		control |= __set_ctrl_final(chan);

	control |= __set_reqseq(chan, chan->buffer_seq);
	control |= __set_txseq(chan, tx_seq);

	__put_control(chan, control, tx_skb->data + L2CAP_HDR_SIZE);

	if (chan->fcs == L2CAP_FCS_CRC16) {
		fcs = crc16(0, (u8 *)tx_skb->data,
						tx_skb->len - L2CAP_FCS_SIZE);
		put_unaligned_le16(fcs,
				tx_skb->data + tx_skb->len - L2CAP_FCS_SIZE);
	}

	l2cap_do_send(chan, tx_skb);
}

static int l2cap_ertm_send(struct l2cap_chan *chan)
{
	struct sk_buff *skb, *tx_skb;
	u16 fcs;
	u32 control;
	int nsent = 0;

	if (chan->state != BT_CONNECTED)
		return -ENOTCONN;

	while ((skb = chan->tx_send_head) && (!l2cap_tx_window_full(chan))) {

		if (chan->remote_max_tx &&
				bt_cb(skb)->retries == chan->remote_max_tx) {
			l2cap_send_disconn_req(chan->conn, chan, ECONNABORTED);
			break;
		}

		tx_skb = skb_clone(skb, GFP_ATOMIC);

		bt_cb(skb)->retries++;

		control = __get_control(chan, tx_skb->data + L2CAP_HDR_SIZE);
		control &= __get_sar_mask(chan);

		if (test_and_clear_bit(CONN_SEND_FBIT, &chan->conn_state))
			control |= __set_ctrl_final(chan);

		control |= __set_reqseq(chan, chan->buffer_seq);
		control |= __set_txseq(chan, chan->next_tx_seq);

		__put_control(chan, control, tx_skb->data + L2CAP_HDR_SIZE);

		if (chan->fcs == L2CAP_FCS_CRC16) {
			fcs = crc16(0, (u8 *)skb->data,
						tx_skb->len - L2CAP_FCS_SIZE);
			put_unaligned_le16(fcs, skb->data +
						tx_skb->len - L2CAP_FCS_SIZE);
		}

		l2cap_do_send(chan, tx_skb);

		__set_retrans_timer(chan);

		bt_cb(skb)->tx_seq = chan->next_tx_seq;

		chan->next_tx_seq = __next_seq(chan, chan->next_tx_seq);

		if (bt_cb(skb)->retries == 1) {
			chan->unacked_frames++;

			if (!nsent++)
				__clear_ack_timer(chan);
		}

		chan->frames_sent++;

		if (skb_queue_is_last(&chan->tx_q, skb))
			chan->tx_send_head = NULL;
		else
			chan->tx_send_head = skb_queue_next(&chan->tx_q, skb);
	}

	return nsent;
}

static int l2cap_retransmit_frames(struct l2cap_chan *chan)
{
	int ret;

	if (!skb_queue_empty(&chan->tx_q))
		chan->tx_send_head = chan->tx_q.next;

	chan->next_tx_seq = chan->expected_ack_seq;
	ret = l2cap_ertm_send(chan);
	return ret;
}

static void __l2cap_send_ack(struct l2cap_chan *chan)
{
	u32 control = 0;

	control |= __set_reqseq(chan, chan->buffer_seq);

	if (test_bit(CONN_LOCAL_BUSY, &chan->conn_state)) {
		control |= __set_ctrl_super(chan, L2CAP_SUPER_RNR);
		set_bit(CONN_RNR_SENT, &chan->conn_state);
		l2cap_send_sframe(chan, control);
		return;
	}

	if (l2cap_ertm_send(chan) > 0)
		return;

	control |= __set_ctrl_super(chan, L2CAP_SUPER_RR);
	l2cap_send_sframe(chan, control);
}

static void l2cap_send_ack(struct l2cap_chan *chan)
{
	__clear_ack_timer(chan);
	__l2cap_send_ack(chan);
}

static void l2cap_send_srejtail(struct l2cap_chan *chan)
{
	struct srej_list *tail;
	u32 control;

	control = __set_ctrl_super(chan, L2CAP_SUPER_SREJ);
	control |= __set_ctrl_final(chan);

	tail = list_entry((&chan->srej_l)->prev, struct srej_list, list);
	control |= __set_reqseq(chan, tail->tx_seq);

	l2cap_send_sframe(chan, control);
}

static inline int l2cap_skbuff_fromiovec(struct l2cap_chan *chan,
					 struct msghdr *msg, int len,
					 int count, struct sk_buff *skb)
{
	struct l2cap_conn *conn = chan->conn;
	struct sk_buff **frag;
	int err, sent = 0;

	if (memcpy_fromiovec(skb_put(skb, count), msg->msg_iov, count))
>>>>>>> remotes/linux2/linux-3.4.y
		return -EFAULT;

	sent += count;
	len  -= count;
<<<<<<< HEAD
	final = skb;
=======
>>>>>>> remotes/linux2/linux-3.4.y

	/* Continuation fragments (no L2CAP header) */
	frag = &skb_shinfo(skb)->frag_list;
	while (len) {
<<<<<<< HEAD
		int skblen;
		count = min_t(unsigned int, conn->mtu, len);

		/* Add room for the FCS if it fits */
		if (bt_cb(skb)->control.fcs == L2CAP_FCS_CRC16 &&
			len + L2CAP_FCS_SIZE <= conn->mtu)
			skblen = count + L2CAP_FCS_SIZE;
		else
			skblen = count;

		/* Don't use bt_skb_send_alloc() while resegmenting, since
		 * it is not ok to block.
		 */
		if (reseg) {
			*frag = bt_skb_alloc(skblen, GFP_ATOMIC);
			if (*frag)
				skb_set_owner_w(*frag, sk);
		} else {
			*frag = bt_skb_send_alloc(sk, skblen,
					msg->msg_flags & MSG_DONTWAIT, &err);
		}

		if (!*frag)
			return -EFAULT;

		/* When resegmenting, data is copied from kernel space */
		if (reseg) {
			err = memcpy_fromkvec(skb_put(*frag, count),
						(struct kvec *) msg->msg_iov,
						count);
		} else {
			err = memcpy_fromiovec(skb_put(*frag, count),
						msg->msg_iov, count);
		}

		if (err)
			return -EFAULT;
=======
		count = min_t(unsigned int, conn->mtu, len);

		*frag = chan->ops->alloc_skb(chan, count,
					     msg->msg_flags & MSG_DONTWAIT,
					     &err);

		if (!*frag)
			return err;
		if (memcpy_fromiovec(skb_put(*frag, count), msg->msg_iov, count))
			return -EFAULT;

		(*frag)->priority = skb->priority;
>>>>>>> remotes/linux2/linux-3.4.y

		sent += count;
		len  -= count;

<<<<<<< HEAD
		final = *frag;

		frag = &(*frag)->next;
	}

	if (bt_cb(skb)->control.fcs == L2CAP_FCS_CRC16) {
		if (skb_tailroom(final) < L2CAP_FCS_SIZE) {
			if (reseg) {
				*frag = bt_skb_alloc(L2CAP_FCS_SIZE,
						GFP_ATOMIC);
				if (*frag)
					skb_set_owner_w(*frag, sk);
			} else {
				*frag = bt_skb_send_alloc(sk, L2CAP_FCS_SIZE,
						msg->msg_flags & MSG_DONTWAIT,
						&err);
			}

			if (!*frag)
				return -EFAULT;

			final = *frag;
		}

		skb_put(final, L2CAP_FCS_SIZE);
	}

	return sent;
}

struct sk_buff *l2cap_create_connless_pdu(struct sock *sk, struct msghdr *msg, size_t len)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
	struct sk_buff *skb;
	int err, count, hlen = L2CAP_HDR_SIZE + 2;
	struct l2cap_hdr *lh;

	BT_DBG("sk %p len %d", sk, (int)len);

	count = min_t(unsigned int, (conn->mtu - hlen), len);
	skb = bt_skb_send_alloc(sk, count + hlen,
			msg->msg_flags & MSG_DONTWAIT, &err);
	if (!skb)
		return ERR_PTR(err);

	/* Create L2CAP header */
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(l2cap_pi(sk)->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));
	put_unaligned_le16(l2cap_pi(sk)->psm, skb_put(skb, 2));

	err = l2cap_skbuff_fromiovec(sk, msg, len, count, skb, 0);
=======
		frag = &(*frag)->next;
	}

	return sent;
}

static struct sk_buff *l2cap_create_connless_pdu(struct l2cap_chan *chan,
						struct msghdr *msg, size_t len,
						u32 priority)
{
	struct l2cap_conn *conn = chan->conn;
	struct sk_buff *skb;
	int err, count, hlen = L2CAP_HDR_SIZE + L2CAP_PSMLEN_SIZE;
	struct l2cap_hdr *lh;

	BT_DBG("chan %p len %d priority %u", chan, (int)len, priority);

	count = min_t(unsigned int, (conn->mtu - hlen), len);

	skb = chan->ops->alloc_skb(chan, count + hlen,
				   msg->msg_flags & MSG_DONTWAIT, &err);

	if (!skb)
		return ERR_PTR(err);

	skb->priority = priority;

	/* Create L2CAP header */
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(chan->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));
	put_unaligned_le16(chan->psm, skb_put(skb, 2));

	err = l2cap_skbuff_fromiovec(chan, msg, len, count, skb);
>>>>>>> remotes/linux2/linux-3.4.y
	if (unlikely(err < 0)) {
		kfree_skb(skb);
		return ERR_PTR(err);
	}
	return skb;
}

<<<<<<< HEAD
struct sk_buff *l2cap_create_basic_pdu(struct sock *sk, struct msghdr *msg, size_t len)
{
	struct l2cap_conn *conn = l2cap_pi(sk)->conn;
=======
static struct sk_buff *l2cap_create_basic_pdu(struct l2cap_chan *chan,
						struct msghdr *msg, size_t len,
						u32 priority)
{
	struct l2cap_conn *conn = chan->conn;
>>>>>>> remotes/linux2/linux-3.4.y
	struct sk_buff *skb;
	int err, count, hlen = L2CAP_HDR_SIZE;
	struct l2cap_hdr *lh;

<<<<<<< HEAD
	BT_DBG("sk %p len %d", sk, (int)len);

	count = min_t(unsigned int, (conn->mtu - hlen), len);
	skb = bt_skb_send_alloc(sk, count + hlen,
			msg->msg_flags & MSG_DONTWAIT, &err);
	if (!skb)
		return ERR_PTR(err);

	/* Create L2CAP header */
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(l2cap_pi(sk)->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));

	err = l2cap_skbuff_fromiovec(sk, msg, len, count, skb, 0);
=======
	BT_DBG("chan %p len %d", chan, (int)len);

	count = min_t(unsigned int, (conn->mtu - hlen), len);

	skb = chan->ops->alloc_skb(chan, count + hlen,
				   msg->msg_flags & MSG_DONTWAIT, &err);

	if (!skb)
		return ERR_PTR(err);

	skb->priority = priority;

	/* Create L2CAP header */
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(chan->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));

	err = l2cap_skbuff_fromiovec(chan, msg, len, count, skb);
>>>>>>> remotes/linux2/linux-3.4.y
	if (unlikely(err < 0)) {
		kfree_skb(skb);
		return ERR_PTR(err);
	}
	return skb;
}

<<<<<<< HEAD
struct sk_buff *l2cap_create_iframe_pdu(struct sock *sk,
					struct msghdr *msg, size_t len,
					u16 sdulen, int reseg)
{
	struct sk_buff *skb;
	int err, count, hlen;
	int reserve = 0;
	struct l2cap_hdr *lh;
	u8 fcs = l2cap_pi(sk)->fcs;

	if (l2cap_pi(sk)->extended_control)
		hlen = L2CAP_EXTENDED_HDR_SIZE;
	else
		hlen = L2CAP_ENHANCED_HDR_SIZE;
=======
static struct sk_buff *l2cap_create_iframe_pdu(struct l2cap_chan *chan,
						struct msghdr *msg, size_t len,
						u32 control, u16 sdulen)
{
	struct l2cap_conn *conn = chan->conn;
	struct sk_buff *skb;
	int err, count, hlen;
	struct l2cap_hdr *lh;

	BT_DBG("chan %p len %d", chan, (int)len);

	if (!conn)
		return ERR_PTR(-ENOTCONN);

	if (test_bit(FLAG_EXT_CTRL, &chan->flags))
		hlen = L2CAP_EXT_HDR_SIZE;
	else
		hlen = L2CAP_ENH_HDR_SIZE;
>>>>>>> remotes/linux2/linux-3.4.y

	if (sdulen)
		hlen += L2CAP_SDULEN_SIZE;

<<<<<<< HEAD
	if (fcs == L2CAP_FCS_CRC16)
		hlen += L2CAP_FCS_SIZE;

	BT_DBG("sk %p, msg %p, len %d, sdulen %d, hlen %d",
		sk, msg, (int)len, (int)sdulen, hlen);

	count = min_t(unsigned int, (l2cap_pi(sk)->conn->mtu - hlen), len);

	/* Allocate extra headroom for Qualcomm PAL.  This is only
	 * necessary in two places (here and when creating sframes)
	 * because only unfragmented iframes and sframes are sent
	 * using AMP controllers.
	 */
	if (l2cap_pi(sk)->ampcon &&
			l2cap_pi(sk)->ampcon->hdev->manufacturer == 0x001d)
		reserve = BT_SKB_RESERVE_80211;

	/* Don't use bt_skb_send_alloc() while resegmenting, since
	 * it is not ok to block.
	 */
	if (reseg) {
		skb = bt_skb_alloc(count + hlen + reserve, GFP_ATOMIC);
		if (skb)
			skb_set_owner_w(skb, sk);
	} else {
		skb = bt_skb_send_alloc(sk, count + hlen + reserve,
					msg->msg_flags & MSG_DONTWAIT, &err);
	}
	if (!skb)
		return ERR_PTR(err);

	if (reserve)
		skb_reserve(skb, reserve);

	bt_cb(skb)->control.fcs = fcs;

	/* Create L2CAP header */
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(l2cap_pi(sk)->dcid);
	lh->len = cpu_to_le16(len + hlen - L2CAP_HDR_SIZE);

	/* Control header is populated later */
	if (l2cap_pi(sk)->extended_control)
		put_unaligned_le32(0, skb_put(skb, 4));
	else
		put_unaligned_le16(0, skb_put(skb, 2));
=======
	if (chan->fcs == L2CAP_FCS_CRC16)
		hlen += L2CAP_FCS_SIZE;

	count = min_t(unsigned int, (conn->mtu - hlen), len);

	skb = chan->ops->alloc_skb(chan, count + hlen,
					msg->msg_flags & MSG_DONTWAIT, &err);

	if (!skb)
		return ERR_PTR(err);

	/* Create L2CAP header */
	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(chan->dcid);
	lh->len = cpu_to_le16(len + (hlen - L2CAP_HDR_SIZE));

	__put_control(chan, control, skb_put(skb, __ctrl_size(chan)));
>>>>>>> remotes/linux2/linux-3.4.y

	if (sdulen)
		put_unaligned_le16(sdulen, skb_put(skb, L2CAP_SDULEN_SIZE));

<<<<<<< HEAD
	err = l2cap_skbuff_fromiovec(sk, msg, len, count, skb, reseg);
	if (unlikely(err < 0)) {
		BT_DBG("err %d", err);
=======
	err = l2cap_skbuff_fromiovec(chan, msg, len, count, skb);
	if (unlikely(err < 0)) {
>>>>>>> remotes/linux2/linux-3.4.y
		kfree_skb(skb);
		return ERR_PTR(err);
	}

<<<<<<< HEAD
=======
	if (chan->fcs == L2CAP_FCS_CRC16)
		put_unaligned_le16(0, skb_put(skb, L2CAP_FCS_SIZE));

>>>>>>> remotes/linux2/linux-3.4.y
	bt_cb(skb)->retries = 0;
	return skb;
}

<<<<<<< HEAD
static void l2cap_ertm_process_reqseq(struct sock *sk, u16 reqseq)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *acked_skb;
	u16 ackseq;

	BT_DBG("sk %p, reqseq %d", sk, (int) reqseq);

	pi = l2cap_pi(sk);

	if (pi->unacked_frames == 0 || reqseq == pi->expected_ack_seq)
		return;

	BT_DBG("expected_ack_seq %d, unacked_frames %d",
		(int) pi->expected_ack_seq, (int) pi->unacked_frames);

	for (ackseq = pi->expected_ack_seq; ackseq != reqseq;
		ackseq = __next_seq(ackseq, pi)) {

		acked_skb = l2cap_ertm_seq_in_queue(TX_QUEUE(sk), ackseq);
		if (acked_skb) {
			skb_unlink(acked_skb, TX_QUEUE(sk));
			kfree_skb(acked_skb);
			pi->unacked_frames--;
		}
	}

	pi->expected_ack_seq = reqseq;

	if (pi->unacked_frames == 0)
		l2cap_ertm_stop_retrans_timer(pi);

	BT_DBG("unacked_frames %d", (int) pi->unacked_frames);
}

static struct sk_buff *l2cap_create_sframe_pdu(struct sock *sk, u32 control)
{
	struct sk_buff *skb;
	int len;
	int reserve = 0;
	struct l2cap_hdr *lh;

	if (l2cap_pi(sk)->extended_control)
		len = L2CAP_EXTENDED_HDR_SIZE;
	else
		len = L2CAP_ENHANCED_HDR_SIZE;

	if (l2cap_pi(sk)->fcs == L2CAP_FCS_CRC16)
		len += L2CAP_FCS_SIZE;

	/* Allocate extra headroom for Qualcomm PAL */
	if (l2cap_pi(sk)->ampcon &&
			l2cap_pi(sk)->ampcon->hdev->manufacturer == 0x001d)
		reserve = BT_SKB_RESERVE_80211;

	skb = bt_skb_alloc(len + reserve, GFP_ATOMIC);

	if (!skb)
		return ERR_PTR(-ENOMEM);

	if (reserve)
		skb_reserve(skb, reserve);

	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->cid = cpu_to_le16(l2cap_pi(sk)->dcid);
	lh->len = cpu_to_le16(len - L2CAP_HDR_SIZE);

	if (l2cap_pi(sk)->extended_control)
		put_unaligned_le32(control, skb_put(skb, 4));
	else
		put_unaligned_le16(control, skb_put(skb, 2));

	if (l2cap_pi(sk)->fcs == L2CAP_FCS_CRC16) {
		u16 fcs = crc16(0, (u8 *) skb->data, skb->len);
		put_unaligned_le16(fcs, skb_put(skb, L2CAP_FCS_SIZE));
	}

	return skb;
}

static void l2cap_ertm_send_sframe(struct sock *sk,
				struct bt_l2cap_control *control)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;
	u32 control_field;

	BT_DBG("sk %p, control %p", sk, control);

	if (control->frame_type != 's')
		return;

	pi = l2cap_pi(sk);

	if (pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
		pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE &&
		pi->amp_move_state != L2CAP_AMP_STATE_RESEGMENT) {
		BT_DBG("AMP error - attempted S-Frame send during AMP move");
		return;
	}

	if ((pi->conn_state & L2CAP_CONN_SEND_FBIT) && !control->poll) {
		control->final = 1;
		pi->conn_state &= ~L2CAP_CONN_SEND_FBIT;
	}

	if (control->super == L2CAP_SFRAME_RR)
		pi->conn_state &= ~L2CAP_CONN_SENT_RNR;
	else if (control->super == L2CAP_SFRAME_RNR)
		pi->conn_state |= L2CAP_CONN_SENT_RNR;

	if (control->super != L2CAP_SFRAME_SREJ) {
		pi->last_acked_seq = control->reqseq;
		l2cap_ertm_stop_ack_timer(pi);
	}

	BT_DBG("reqseq %d, final %d, poll %d, super %d", (int) control->reqseq,
		(int) control->final, (int) control->poll,
		(int) control->super);

	if (pi->extended_control)
		control_field = __pack_extended_control(control);
	else
		control_field = __pack_enhanced_control(control);

	skb = l2cap_create_sframe_pdu(sk, control_field);
	if (!IS_ERR(skb))
		l2cap_do_send(sk, skb);
}

static void l2cap_ertm_send_ack(struct sock *sk)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct bt_l2cap_control control;
	u16 frames_to_ack = __delta_seq(pi->buffer_seq, pi->last_acked_seq, pi);
	int threshold;

	BT_DBG("sk %p", sk);
	BT_DBG("last_acked_seq %d, buffer_seq %d", (int)pi->last_acked_seq,
		(int)pi->buffer_seq);

	memset(&control, 0, sizeof(control));
	control.frame_type = 's';

	if ((pi->conn_state & L2CAP_CONN_LOCAL_BUSY) &&
		pi->rx_state == L2CAP_ERTM_RX_STATE_RECV) {
		l2cap_ertm_stop_ack_timer(pi);
		control.super = L2CAP_SFRAME_RNR;
		control.reqseq = pi->buffer_seq;
		l2cap_ertm_send_sframe(sk, &control);
	} else {
		if (!(pi->conn_state & L2CAP_CONN_REMOTE_BUSY)) {
			l2cap_ertm_send(sk);
			/* If any i-frames were sent, they included an ack */
			if (pi->buffer_seq == pi->last_acked_seq)
				frames_to_ack = 0;
		}

		/* Ack now if the window is 3/4ths full.
		 * Calculate without mul or div
		 */
		threshold = pi->ack_win;
		threshold += threshold << 1;
		threshold >>= 2;

		BT_DBG("frames_to_ack %d, threshold %d", (int)frames_to_ack,
			threshold);

		if (frames_to_ack >= threshold) {
			l2cap_ertm_stop_ack_timer(pi);
			control.super = L2CAP_SFRAME_RR;
			control.reqseq = pi->buffer_seq;
			l2cap_ertm_send_sframe(sk, &control);
			frames_to_ack = 0;
		}

		if (frames_to_ack)
			l2cap_ertm_start_ack_timer(pi);
	}
}

static void l2cap_ertm_send_rr_or_rnr(struct sock *sk, bool poll)
{
	struct l2cap_pinfo *pi;
	struct bt_l2cap_control control;

	BT_DBG("sk %p, poll %d", sk, (int) poll);

	pi = l2cap_pi(sk);

	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.poll = poll;

	if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY)
		control.super = L2CAP_SFRAME_RNR;
	else
		control.super = L2CAP_SFRAME_RR;

	control.reqseq = pi->buffer_seq;
	l2cap_ertm_send_sframe(sk, &control);
}

static void l2cap_ertm_send_i_or_rr_or_rnr(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	struct bt_l2cap_control control;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.final = 1;
	control.reqseq = pi->buffer_seq;
	pi->conn_state |= L2CAP_CONN_SEND_FBIT;

	if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
		control.super = L2CAP_SFRAME_RNR;
		l2cap_ertm_send_sframe(sk, &control);
	}

	if ((pi->conn_state & L2CAP_CONN_REMOTE_BUSY) &&
		(pi->unacked_frames > 0))
		l2cap_ertm_start_retrans_timer(pi);

	pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

	/* Send pending iframes */
	l2cap_ertm_send(sk);

	if (pi->conn_state & L2CAP_CONN_SEND_FBIT) {
		/* F-bit wasn't sent in an s-frame or i-frame yet, so
		 * send it now.
		 */
		control.super = L2CAP_SFRAME_RR;
		l2cap_ertm_send_sframe(sk, &control);
	}
}

static void l2cap_ertm_send_srej(struct sock *sk, u16 txseq)
{
	struct bt_l2cap_control control;
	struct l2cap_pinfo *pi;
	u16 seq;

	BT_DBG("sk %p, txseq %d", sk, (int)txseq);

	pi = l2cap_pi(sk);
	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.super = L2CAP_SFRAME_SREJ;

	for (seq = pi->expected_tx_seq; seq != txseq;
		seq = __next_seq(seq, pi)) {
		if (!l2cap_ertm_seq_in_queue(SREJ_QUEUE(pi), seq)) {
			control.reqseq = seq;
			l2cap_ertm_send_sframe(sk, &control);
			l2cap_seq_list_append(&pi->srej_list, seq);
		}
	}

	pi->expected_tx_seq = __next_seq(txseq, pi);
}

static void l2cap_ertm_send_srej_tail(struct sock *sk)
{
	struct bt_l2cap_control control;
	struct l2cap_pinfo *pi;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	if (pi->srej_list.tail == L2CAP_SEQ_LIST_CLEAR)
		return;

	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.super = L2CAP_SFRAME_SREJ;
	control.reqseq = pi->srej_list.tail;
	l2cap_ertm_send_sframe(sk, &control);
}

static void l2cap_ertm_send_srej_list(struct sock *sk, u16 txseq)
{
	struct bt_l2cap_control control;
	struct l2cap_pinfo *pi;
	u16 initial_head;
	u16 seq;

	BT_DBG("sk %p, txseq %d", sk, (int) txseq);

	pi = l2cap_pi(sk);
	memset(&control, 0, sizeof(control));
	control.frame_type = 's';
	control.super = L2CAP_SFRAME_SREJ;

	/* Capture initial list head to allow only one pass through the list. */
	initial_head = pi->srej_list.head;

	do {
		seq = l2cap_seq_list_pop(&pi->srej_list);
		if ((seq == txseq) || (seq == L2CAP_SEQ_LIST_CLEAR))
			break;

		control.reqseq = seq;
		l2cap_ertm_send_sframe(sk, &control);
		l2cap_seq_list_append(&pi->srej_list, seq);
	} while (pi->srej_list.head != initial_head);
}

static void l2cap_ertm_abort_rx_srej_sent(struct sock *sk)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	BT_DBG("sk %p", sk);

	pi->expected_tx_seq = pi->buffer_seq;
	l2cap_seq_list_clear(&l2cap_pi(sk)->srej_list);
	skb_queue_purge(SREJ_QUEUE(sk));
	pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
}

static int l2cap_ertm_tx_state_xmit(struct sock *sk,
				struct bt_l2cap_control *control,
				struct sk_buff_head *skbs, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skbs %p, event %d", sk, control, skbs,
		(int)event);
	pi = l2cap_pi(sk);

	switch (event) {
	case L2CAP_ERTM_EVENT_DATA_REQUEST:
		if (sk->sk_send_head == NULL)
			sk->sk_send_head = skb_peek(skbs);

		skb_queue_splice_tail_init(skbs, TX_QUEUE(sk));
		l2cap_ertm_send(sk);
		break;
	case L2CAP_ERTM_EVENT_LOCAL_BUSY_DETECTED:
		BT_DBG("Enter LOCAL_BUSY");
		pi->conn_state |= L2CAP_CONN_LOCAL_BUSY;

		if (pi->rx_state == L2CAP_ERTM_RX_STATE_SREJ_SENT) {
			/* The SREJ_SENT state must be aborted if we are to
			 * enter the LOCAL_BUSY state.
			 */
			l2cap_ertm_abort_rx_srej_sent(sk);
		}

		l2cap_ertm_send_ack(sk);

		break;
	case L2CAP_ERTM_EVENT_LOCAL_BUSY_CLEAR:
		BT_DBG("Exit LOCAL_BUSY");
		pi->conn_state &= ~L2CAP_CONN_LOCAL_BUSY;

		if (pi->amp_move_state == L2CAP_AMP_STATE_WAIT_LOCAL_BUSY) {
			if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP;
				l2cap_send_move_chan_cfm(pi->conn, pi,
						pi->scid,
						L2CAP_MOVE_CHAN_CONFIRMED);
				l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
			} else if (pi->amp_move_role ==
					L2CAP_AMP_MOVE_RESPONDER) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM;
				l2cap_send_move_chan_rsp(pi->conn,
						pi->amp_move_cmd_ident,
						pi->dcid,
						L2CAP_MOVE_CHAN_SUCCESS);
			}
			break;
		}

		if (pi->amp_move_role == L2CAP_AMP_MOVE_NONE &&
			(pi->conn_state & L2CAP_CONN_SENT_RNR)) {
			struct bt_l2cap_control local_control;

			memset(&local_control, 0, sizeof(local_control));
			local_control.frame_type = 's';
			local_control.super = L2CAP_SFRAME_RR;
			local_control.poll = 1;
			local_control.reqseq = pi->buffer_seq;
			l2cap_ertm_send_sframe(sk, &local_control);

			pi->retry_count = 1;
			l2cap_ertm_start_monitor_timer(pi);
			pi->tx_state = L2CAP_ERTM_TX_STATE_WAIT_F;
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_REQSEQ_AND_FBIT:
		l2cap_ertm_process_reqseq(sk, control->reqseq);
		break;
	case L2CAP_ERTM_EVENT_EXPLICIT_POLL:
		l2cap_ertm_send_rr_or_rnr(sk, 1);
		pi->retry_count = 1;
		l2cap_ertm_start_monitor_timer(pi);
		l2cap_ertm_stop_ack_timer(pi);
		pi->tx_state = L2CAP_ERTM_TX_STATE_WAIT_F;
		break;
	case L2CAP_ERTM_EVENT_RETRANS_TIMER_EXPIRES:
		l2cap_ertm_send_rr_or_rnr(sk, 1);
		pi->retry_count = 1;
		l2cap_ertm_start_monitor_timer(pi);
		pi->tx_state = L2CAP_ERTM_TX_STATE_WAIT_F;
		break;
	case L2CAP_ERTM_EVENT_RECV_FBIT:
		/* Nothing to process */
		break;
	default:
		break;
	}

	return err;
}

static int l2cap_ertm_tx_state_wait_f(struct sock *sk,
				struct bt_l2cap_control *control,
				struct sk_buff_head *skbs, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skbs %p, event %d", sk, control, skbs,
		(int)event);
	pi = l2cap_pi(sk);

	switch (event) {
	case L2CAP_ERTM_EVENT_DATA_REQUEST:
		if (sk->sk_send_head == NULL)
			sk->sk_send_head = skb_peek(skbs);
		/* Queue data, but don't send. */
		skb_queue_splice_tail_init(skbs, TX_QUEUE(sk));
		break;
	case L2CAP_ERTM_EVENT_LOCAL_BUSY_DETECTED:
		BT_DBG("Enter LOCAL_BUSY");
		pi->conn_state |= L2CAP_CONN_LOCAL_BUSY;

		if (pi->rx_state == L2CAP_ERTM_RX_STATE_SREJ_SENT) {
			/* The SREJ_SENT state must be aborted if we are to
			 * enter the LOCAL_BUSY state.
			 */
			l2cap_ertm_abort_rx_srej_sent(sk);
		}

		l2cap_ertm_send_ack(sk);

		break;
	case L2CAP_ERTM_EVENT_LOCAL_BUSY_CLEAR:
		BT_DBG("Exit LOCAL_BUSY");
		pi->conn_state &= ~L2CAP_CONN_LOCAL_BUSY;

		if (pi->conn_state & L2CAP_CONN_SENT_RNR) {
			struct bt_l2cap_control local_control;
			memset(&local_control, 0, sizeof(local_control));
			local_control.frame_type = 's';
			local_control.super = L2CAP_SFRAME_RR;
			local_control.poll = 1;
			local_control.reqseq = pi->buffer_seq;
			l2cap_ertm_send_sframe(sk, &local_control);

			pi->retry_count = 1;
			l2cap_ertm_start_monitor_timer(pi);
			pi->tx_state = L2CAP_ERTM_TX_STATE_WAIT_F;
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_REQSEQ_AND_FBIT:
		l2cap_ertm_process_reqseq(sk, control->reqseq);

		/* Fall through */

	case L2CAP_ERTM_EVENT_RECV_FBIT:
		if (control && control->final) {
			l2cap_ertm_stop_monitor_timer(pi);
			if (pi->unacked_frames > 0)
				l2cap_ertm_start_retrans_timer(pi);
			pi->retry_count = 0;
			pi->tx_state = L2CAP_ERTM_TX_STATE_XMIT;
			BT_DBG("recv fbit tx_state 0x2.2%x", pi->tx_state);
		}
		break;
	case L2CAP_ERTM_EVENT_EXPLICIT_POLL:
		/* Ignore */
		break;
	case L2CAP_ERTM_EVENT_MONITOR_TIMER_EXPIRES:
		if ((pi->max_tx == 0) || (pi->retry_count < pi->max_tx)) {
			l2cap_ertm_send_rr_or_rnr(sk, 1);
			l2cap_ertm_start_monitor_timer(pi);
			pi->retry_count += 1;
		} else
			l2cap_send_disconn_req(pi->conn, sk, ECONNABORTED);
		break;
	default:
		break;
	}

	return err;
}

int l2cap_ertm_tx(struct sock *sk, struct bt_l2cap_control *control,
			struct sk_buff_head *skbs, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skbs %p, event %d, state %d",
		sk, control, skbs, (int)event, l2cap_pi(sk)->tx_state);

	pi = l2cap_pi(sk);

	switch (pi->tx_state) {
	case L2CAP_ERTM_TX_STATE_XMIT:
		err = l2cap_ertm_tx_state_xmit(sk, control, skbs, event);
		break;
	case L2CAP_ERTM_TX_STATE_WAIT_F:
		err = l2cap_ertm_tx_state_wait_f(sk, control, skbs, event);
		break;
	default:
		/* Ignore event */
		break;
	}

	return err;
}

int l2cap_segment_sdu(struct sock *sk, struct sk_buff_head* seg_queue,
			struct msghdr *msg, size_t len, int reseg)
{
	struct sk_buff *skb;
	u16 sdu_len;
	size_t pdu_len;
	int err = 0;
	u8 sar;

	BT_DBG("sk %p, msg %p, len %d", sk, msg, (int)len);

	/* It is critical that ERTM PDUs fit in a single HCI fragment,
	 * so fragmented skbs are not used.  The HCI layer's handling
	 * of fragmented skbs is not compatible with ERTM's queueing.
	 */

	/* PDU size is derived from the HCI MTU */
	pdu_len = l2cap_pi(sk)->conn->mtu;

	/* Constrain BR/EDR PDU size to fit within the largest radio packet */
	if (!l2cap_pi(sk)->ampcon)
		pdu_len = min_t(size_t, pdu_len, L2CAP_BREDR_MAX_PAYLOAD);

	/* Adjust for largest possible L2CAP overhead. */
	pdu_len -= L2CAP_EXTENDED_HDR_SIZE + L2CAP_FCS_SIZE;

	/* Remote device may have requested smaller PDUs */
	pdu_len = min_t(size_t, pdu_len, l2cap_pi(sk)->remote_mps);

	if (len <= pdu_len) {
		sar = L2CAP_SAR_UNSEGMENTED;
		sdu_len = 0;
		pdu_len = len;
	} else {
		sar = L2CAP_SAR_START;
		sdu_len = len;
		pdu_len -= L2CAP_SDULEN_SIZE;
	}

	while (len) {
		skb = l2cap_create_iframe_pdu(sk, msg, pdu_len, sdu_len, reseg);

		BT_DBG("iframe skb %p", skb);

		if (IS_ERR(skb)) {
			__skb_queue_purge(seg_queue);
			return PTR_ERR(skb);
		}

		bt_cb(skb)->control.sar = sar;
		__skb_queue_tail(seg_queue, skb);

		len -= pdu_len;
		if (sdu_len) {
			sdu_len = 0;
			pdu_len += L2CAP_SDULEN_SIZE;
		}

		if (len <= pdu_len) {
			sar = L2CAP_SAR_END;
			pdu_len = len;
		} else {
			sar = L2CAP_SAR_CONTINUE;
		}
	}

	return err;
}

static inline int is_initial_frame(u8 sar)
{
	return (sar == L2CAP_SAR_UNSEGMENTED ||
		sar == L2CAP_SAR_START);
}

static inline int l2cap_skbuff_to_kvec(struct sk_buff *skb, struct kvec *iv,
					size_t veclen)
{
	struct sk_buff *frag_iter;

	BT_DBG("skb %p (len %d), iv %p", skb, (int)skb->len, iv);

	if (iv->iov_len + skb->len > veclen)
		return -ENOMEM;

	memcpy(iv->iov_base + iv->iov_len, skb->data, skb->len);
	iv->iov_len += skb->len;

	skb_walk_frags(skb, frag_iter) {
		if (iv->iov_len + skb->len > veclen)
			return -ENOMEM;

		BT_DBG("Copying %d bytes", (int)frag_iter->len);
		memcpy(iv->iov_base + iv->iov_len, frag_iter->data,
			frag_iter->len);
		iv->iov_len += frag_iter->len;
	}

	return 0;
}

int l2cap_resegment_queue(struct sock *sk, struct sk_buff_head *queue)
{
	void *buf;
	int buflen;
	int err = 0;
	struct sk_buff *skb;
	struct msghdr msg;
	struct kvec iv;
	struct sk_buff_head old_frames;
	struct l2cap_pinfo *pi = l2cap_pi(sk);

	BT_DBG("sk %p", sk);

	if (skb_queue_empty(queue))
		return 0;

	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = (struct iovec *) &iv;

	buflen = pi->omtu + L2CAP_FCS_SIZE;
	buf = kzalloc(buflen, GFP_TEMPORARY);

	if (!buf) {
		BT_DBG("Could not allocate resegmentation buffer");
		return -ENOMEM;
	}

	/* Move current frames off the original queue */
	__skb_queue_head_init(&old_frames);
	skb_queue_splice_tail_init(queue, &old_frames);

	while (!skb_queue_empty(&old_frames)) {
		struct sk_buff_head current_sdu;
		u8 original_sar;

		/* Reassemble each SDU from one or more PDUs */

		iv.iov_base = buf;
		iv.iov_len = 0;

		skb = skb_peek(&old_frames);
		original_sar = bt_cb(skb)->control.sar;

		__skb_unlink(skb, &old_frames);

		/* Append data to SDU */
		if (pi->extended_control)
			skb_pull(skb, L2CAP_EXTENDED_HDR_SIZE);
		else
			skb_pull(skb, L2CAP_ENHANCED_HDR_SIZE);

		if (original_sar == L2CAP_SAR_START)
			skb_pull(skb, L2CAP_SDULEN_SIZE);

		err = l2cap_skbuff_to_kvec(skb, &iv, buflen);

		if (bt_cb(skb)->control.fcs == L2CAP_FCS_CRC16)
			iv.iov_len -= L2CAP_FCS_SIZE;

		/* Free skb */
		kfree_skb(skb);

		if (err)
			break;

		while (!skb_queue_empty(&old_frames) && !err) {
			/* Check next frame */
			skb = skb_peek(&old_frames);

			if (is_initial_frame(bt_cb(skb)->control.sar))
				break;

			__skb_unlink(skb, &old_frames);

			/* Append data to SDU */
			if (pi->extended_control)
				skb_pull(skb, L2CAP_EXTENDED_HDR_SIZE);
			else
				skb_pull(skb, L2CAP_ENHANCED_HDR_SIZE);

			if (bt_cb(skb)->control.sar == L2CAP_SAR_START)
				skb_pull(skb, L2CAP_SDULEN_SIZE);

			err = l2cap_skbuff_to_kvec(skb, &iv, buflen);

			if (bt_cb(skb)->control.fcs == L2CAP_FCS_CRC16)
				iv.iov_len -= L2CAP_FCS_SIZE;

			/* Free skb */
			kfree_skb(skb);
		}

		if (err)
			break;

		/* Segment data */

		__skb_queue_head_init(&current_sdu);

		/* skbs for the SDU were just freed, but the
		 * resegmenting process could produce more, smaller
		 * skbs due to smaller PDUs and reduced HCI MTU.  The
		 * overhead from the sk_buff structs could put us over
		 * the sk_sndbuf limit.
		 *
		 * Since this code is running in response to a
		 * received poll/final packet, it cannot block.
		 * Therefore, memory allocation needs to be allowed by
		 * falling back to bt_skb_alloc() (with
		 * skb_set_owner_w() to maintain sk_wmem_alloc
		 * correctly).
		 */
		msg.msg_iovlen = iv.iov_len;
		err = l2cap_segment_sdu(sk, &current_sdu, &msg,
					msg.msg_iovlen, 1);

		if (err || skb_queue_empty(&current_sdu)) {
			BT_DBG("Error %d resegmenting data for socket %p",
				err, sk);
			__skb_queue_purge(&current_sdu);
			break;
		}

		/* Fix up first PDU SAR bits */
		if (!is_initial_frame(original_sar)) {
			BT_DBG("Changing SAR bits, %d PDUs",
				skb_queue_len(&current_sdu));
			skb = skb_peek(&current_sdu);

			if (skb_queue_len(&current_sdu) == 1) {
				/* Change SAR from 'unsegmented' to 'end' */
				bt_cb(skb)->control.sar = L2CAP_SAR_END;
			} else {
				struct l2cap_hdr *lh;
				size_t hdrlen;

				/* Change SAR from 'start' to 'continue' */
				bt_cb(skb)->control.sar = L2CAP_SAR_CONTINUE;

				/* Start frames contain 2 bytes for
				 * sdulen and continue frames don't.
				 * Must rewrite header to eliminate
				 * sdulen and then adjust l2cap frame
				 * length.
				 */
				if (pi->extended_control)
					hdrlen = L2CAP_EXTENDED_HDR_SIZE;
				else
					hdrlen = L2CAP_ENHANCED_HDR_SIZE;

				memmove(skb->data + L2CAP_SDULEN_SIZE,
					skb->data, hdrlen);
				skb_pull(skb, L2CAP_SDULEN_SIZE);
				lh = (struct l2cap_hdr *)skb->data;
				lh->len = cpu_to_le16(le16_to_cpu(lh->len) -
							L2CAP_SDULEN_SIZE);
			}
		}

		/* Add to queue */
		skb_queue_splice_tail(&current_sdu, queue);
	}

	__skb_queue_purge(&old_frames);
	if (err)
		__skb_queue_purge(queue);

	kfree(buf);

	BT_DBG("Queue resegmented, err=%d", err);
	return err;
}

static void l2cap_resegment_worker(struct work_struct *work)
{
	int err = 0;
	struct l2cap_resegment_work *seg_work =
		container_of(work, struct l2cap_resegment_work, work);
	struct sock *sk = seg_work->sk;

	kfree(seg_work);

	BT_DBG("sk %p", sk);
	lock_sock(sk);

	if (l2cap_pi(sk)->amp_move_state != L2CAP_AMP_STATE_RESEGMENT) {
		release_sock(sk);
		sock_put(sk);
		return;
	}

	err = l2cap_resegment_queue(sk, TX_QUEUE(sk));

	l2cap_pi(sk)->amp_move_state = L2CAP_AMP_STATE_STABLE;

	if (skb_queue_empty(TX_QUEUE(sk)))
		sk->sk_send_head = NULL;
	else
		sk->sk_send_head = skb_peek(TX_QUEUE(sk));

	if (err)
		l2cap_send_disconn_req(l2cap_pi(sk)->conn, sk, ECONNRESET);
	else
		l2cap_ertm_send(sk);

	release_sock(sk);
	sock_put(sk);
}

static int l2cap_setup_resegment(struct sock *sk)
{
	struct l2cap_resegment_work *seg_work;

	BT_DBG("sk %p", sk);

	if (skb_queue_empty(TX_QUEUE(sk)))
		return 0;

	seg_work = kzalloc(sizeof(*seg_work), GFP_ATOMIC);
	if (!seg_work)
		return -ENOMEM;

	INIT_WORK(&seg_work->work, l2cap_resegment_worker);
	sock_hold(sk);
	seg_work->sk = sk;

	if (!queue_work(_l2cap_wq, &seg_work->work)) {
		kfree(seg_work);
		sock_put(sk);
		return -ENOMEM;
	}

	l2cap_pi(sk)->amp_move_state = L2CAP_AMP_STATE_RESEGMENT;

	return 0;
}

static inline int l2cap_rmem_available(struct sock *sk)
{
	BT_DBG("sk_rmem_alloc %d, sk_rcvbuf %d",
		atomic_read(&sk->sk_rmem_alloc), sk->sk_rcvbuf);
	return atomic_read(&sk->sk_rmem_alloc) < sk->sk_rcvbuf / 3;
}

static inline int l2cap_rmem_full(struct sock *sk)
{
	BT_DBG("sk_rmem_alloc %d, sk_rcvbuf %d",
		atomic_read(&sk->sk_rmem_alloc), sk->sk_rcvbuf);
	return atomic_read(&sk->sk_rmem_alloc) > (2 * sk->sk_rcvbuf) / 3;
}

void l2cap_amp_move_init(struct sock *sk)
{
	BT_DBG("sk %p", sk);

	if (!l2cap_pi(sk)->conn)
		return;

	if (!(l2cap_pi(sk)->conn->fc_mask & L2CAP_FC_A2MP) || !enable_hs)
		return;

	if (l2cap_pi(sk)->amp_id == 0) {
		if (l2cap_pi(sk)->amp_pref != BT_AMP_POLICY_PREFER_AMP)
			return;
		l2cap_pi(sk)->amp_move_role = L2CAP_AMP_MOVE_INITIATOR;
		l2cap_pi(sk)->amp_move_state = L2CAP_AMP_STATE_WAIT_PREPARE;
		amp_create_physical(l2cap_pi(sk)->conn, sk);
	} else {
		l2cap_pi(sk)->amp_move_role = L2CAP_AMP_MOVE_INITIATOR;
		l2cap_pi(sk)->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS;
		l2cap_pi(sk)->amp_move_id = 0;
		l2cap_amp_move_setup(sk);
		l2cap_send_move_chan_req(l2cap_pi(sk)->conn,
					l2cap_pi(sk), l2cap_pi(sk)->scid, 0);
		l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
	}
}

static void l2cap_chan_ready(struct sock *sk)
{
	struct sock *parent = bt_sk(sk)->parent;

	BT_DBG("sk %p, parent %p", sk, parent);

	l2cap_pi(sk)->conf_state = 0;
	l2cap_sock_clear_timer(sk);

	if (!parent) {
		/* Outgoing channel.
		 * Wake up socket sleeping on connect.
		 */
		sk->sk_state = BT_CONNECTED;
		sk->sk_state_change(sk);
	} else {
		/* Incoming channel.
		 * Wake up socket sleeping on accept.
		 */
		parent->sk_data_ready(parent, 0);
	}
}

/* Copy frame to all raw sockets on that connection */
static void l2cap_raw_recv(struct l2cap_conn *conn, struct sk_buff *skb)
{
	struct l2cap_chan_list *l = &conn->chan_list;
	struct sk_buff *nskb;
	struct sock *sk;

	BT_DBG("conn %p", conn);

	read_lock(&l->lock);
	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		if (sk->sk_type != SOCK_RAW)
			continue;

		/* Don't send frame to the socket it came from */
		if (skb->sk == sk)
			continue;
		nskb = skb_clone(skb, GFP_ATOMIC);
		if (!nskb)
			continue;

		if (sock_queue_rcv_skb(sk, nskb))
			kfree_skb(nskb);
	}
	read_unlock(&l->lock);
}

/* ---- L2CAP signalling commands ---- */
static struct sk_buff *l2cap_build_cmd(struct l2cap_conn *conn,
				u8 code, u8 ident, u16 dlen, void *data)
{
	struct sk_buff *skb, **frag;
	struct l2cap_cmd_hdr *cmd;
	struct l2cap_hdr *lh;
	int len, count;
	unsigned int mtu = conn->hcon->hdev->acl_mtu;

	BT_DBG("conn %p, code 0x%2.2x, ident 0x%2.2x, len %d",
			conn, code, ident, dlen);

	len = L2CAP_HDR_SIZE + L2CAP_CMD_HDR_SIZE + dlen;
	count = min_t(unsigned int, mtu, len);

	skb = bt_skb_alloc(count, GFP_ATOMIC);
	if (!skb)
		return NULL;

	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(L2CAP_CMD_HDR_SIZE + dlen);

	if (conn->hcon->type == LE_LINK)
		lh->cid = cpu_to_le16(L2CAP_CID_LE_SIGNALING);
	else
		lh->cid = cpu_to_le16(L2CAP_CID_SIGNALING);

	cmd = (struct l2cap_cmd_hdr *) skb_put(skb, L2CAP_CMD_HDR_SIZE);
	cmd->code  = code;
	cmd->ident = ident;
	cmd->len   = cpu_to_le16(dlen);

	if (dlen) {
		count -= L2CAP_HDR_SIZE + L2CAP_CMD_HDR_SIZE;
		memcpy(skb_put(skb, count), data, count);
		data += count;
	}

	len -= skb->len;

	/* Continuation fragments (no L2CAP header) */
	frag = &skb_shinfo(skb)->frag_list;
	while (len) {
		count = min_t(unsigned int, mtu, len);

		*frag = bt_skb_alloc(count, GFP_ATOMIC);
		if (!*frag)
			goto fail;

		memcpy(skb_put(*frag, count), data, count);

		len  -= count;
		data += count;

		frag = &(*frag)->next;
	}

	return skb;

fail:
	kfree_skb(skb);
	return NULL;
}

static inline int l2cap_get_conf_opt(void **ptr, int *type, int *olen, unsigned long *val)
{
	struct l2cap_conf_opt *opt = *ptr;
	int len;

	len = L2CAP_CONF_OPT_SIZE + opt->len;
	*ptr += len;

	*type = opt->type;
	*olen = opt->len;

	switch (opt->len) {
	case 1:
		*val = *((u8 *) opt->val);
		break;

	case 2:
		*val = get_unaligned_le16(opt->val);
		break;

	case 4:
		*val = get_unaligned_le32(opt->val);
		break;

	default:
		*val = (unsigned long) opt->val;
		break;
	}

	BT_DBG("type 0x%2.2x len %d val 0x%lx", *type, opt->len, *val);
	return len;
}

static void l2cap_add_conf_opt(void **ptr, u8 type, u8 len, unsigned long val)
{
	struct l2cap_conf_opt *opt = *ptr;

	BT_DBG("type 0x%2.2x len %d val 0x%lx", type, len, val);

	opt->type = type;
	opt->len  = len;

	switch (len) {
	case 1:
		*((u8 *) opt->val)  = val;
		break;

	case 2:
		put_unaligned_le16(val, opt->val);
		break;

	case 4:
		put_unaligned_le32(val, opt->val);
		break;

	default:
		memcpy(opt->val, (void *) val, len);
		break;
	}

	*ptr += L2CAP_CONF_OPT_SIZE + len;
}

static void l2cap_ertm_ack_timeout(struct work_struct *work)
{
	struct delayed_work *delayed =
		container_of(work, struct delayed_work, work);
	struct l2cap_pinfo *pi =
		container_of(delayed, struct l2cap_pinfo, ack_work);
	struct sock *sk = (struct sock *)pi;
	u16 frames_to_ack;

	BT_DBG("sk %p", sk);

	if (!sk)
		return;

	lock_sock(sk);

	if (!l2cap_pi(sk)->conn) {
		release_sock(sk);
		return;
	}

	frames_to_ack = __delta_seq(l2cap_pi(sk)->buffer_seq,
				    l2cap_pi(sk)->last_acked_seq,
				    l2cap_pi(sk));

	if (frames_to_ack)
		l2cap_ertm_send_rr_or_rnr(sk, 0);

	release_sock(sk);
}

static void l2cap_ertm_retrans_timeout(struct work_struct *work)
{
	struct delayed_work *delayed =
		container_of(work, struct delayed_work, work);
	struct l2cap_pinfo *pi =
		container_of(delayed, struct l2cap_pinfo, retrans_work);
	struct sock *sk = (struct sock *)pi;

	BT_DBG("sk %p", sk);

	if (!sk)
		return;

	lock_sock(sk);

	if (!l2cap_pi(sk)->conn) {
		release_sock(sk);
		return;
	}

	l2cap_ertm_tx(sk, 0, 0, L2CAP_ERTM_EVENT_RETRANS_TIMER_EXPIRES);
	release_sock(sk);
}

static void l2cap_ertm_monitor_timeout(struct work_struct *work)
{
	struct delayed_work *delayed =
		container_of(work, struct delayed_work, work);
	struct l2cap_pinfo *pi =
		container_of(delayed, struct l2cap_pinfo, monitor_work);
	struct sock *sk = (struct sock *)pi;

	BT_DBG("sk %p", sk);

	if (!sk)
		return;

	lock_sock(sk);

	if (!l2cap_pi(sk)->conn) {
		release_sock(sk);
		return;
	}

	l2cap_ertm_tx(sk, 0, 0, L2CAP_ERTM_EVENT_MONITOR_TIMER_EXPIRES);

	release_sock(sk);
}

static inline void l2cap_ertm_init(struct sock *sk)
{
	l2cap_pi(sk)->next_tx_seq = 0;
	l2cap_pi(sk)->expected_tx_seq = 0;
	l2cap_pi(sk)->expected_ack_seq = 0;
	l2cap_pi(sk)->unacked_frames = 0;
	l2cap_pi(sk)->buffer_seq = 0;
	l2cap_pi(sk)->frames_sent = 0;
	l2cap_pi(sk)->last_acked_seq = 0;
	l2cap_pi(sk)->sdu = NULL;
	l2cap_pi(sk)->sdu_last_frag = NULL;
	l2cap_pi(sk)->sdu_len = 0;
	atomic_set(&l2cap_pi(sk)->ertm_queued, 0);

	l2cap_pi(sk)->rx_state = L2CAP_ERTM_RX_STATE_RECV;
	l2cap_pi(sk)->tx_state = L2CAP_ERTM_TX_STATE_XMIT;

	BT_DBG("tx_state 0x2.2%x rx_state 0x2.2%x", l2cap_pi(sk)->tx_state,
		l2cap_pi(sk)->rx_state);

	l2cap_pi(sk)->amp_id = 0;
	l2cap_pi(sk)->amp_move_state = L2CAP_AMP_STATE_STABLE;
	l2cap_pi(sk)->amp_move_role = L2CAP_AMP_MOVE_NONE;
	l2cap_pi(sk)->amp_move_reqseq = 0;
	l2cap_pi(sk)->amp_move_event = 0;

	INIT_DELAYED_WORK(&l2cap_pi(sk)->ack_work, l2cap_ertm_ack_timeout);
	INIT_DELAYED_WORK(&l2cap_pi(sk)->retrans_work,
			l2cap_ertm_retrans_timeout);
	INIT_DELAYED_WORK(&l2cap_pi(sk)->monitor_work,
			l2cap_ertm_monitor_timeout);
	INIT_WORK(&l2cap_pi(sk)->tx_work, l2cap_ertm_tx_worker);
	skb_queue_head_init(SREJ_QUEUE(sk));
	skb_queue_head_init(TX_QUEUE(sk));

	l2cap_seq_list_init(&l2cap_pi(sk)->srej_list, l2cap_pi(sk)->tx_win);
	l2cap_seq_list_init(&l2cap_pi(sk)->retrans_list,
			l2cap_pi(sk)->remote_tx_win);
}

void l2cap_ertm_destruct(struct sock *sk)
{
	l2cap_seq_list_free(&l2cap_pi(sk)->srej_list);
	l2cap_seq_list_free(&l2cap_pi(sk)->retrans_list);
}

void l2cap_ertm_shutdown(struct sock *sk)
{
	l2cap_ertm_stop_ack_timer(l2cap_pi(sk));
	l2cap_ertm_stop_retrans_timer(l2cap_pi(sk));
	l2cap_ertm_stop_monitor_timer(l2cap_pi(sk));
}

void l2cap_ertm_recv_done(struct sock *sk)
{
	lock_sock(sk);

	if (l2cap_pi(sk)->mode != L2CAP_MODE_ERTM ||
			sk->sk_state != BT_CONNECTED) {
		release_sock(sk);
		return;
	}

	/* Consume any queued incoming frames and update local busy status */
	if (l2cap_pi(sk)->rx_state == L2CAP_ERTM_RX_STATE_SREJ_SENT &&
			l2cap_ertm_rx_queued_iframes(sk))
		l2cap_send_disconn_req(l2cap_pi(sk)->conn, sk, ECONNRESET);
	else if ((l2cap_pi(sk)->conn_state & L2CAP_CONN_LOCAL_BUSY) &&
			l2cap_rmem_available(sk))
		l2cap_ertm_tx(sk, 0, 0, L2CAP_ERTM_EVENT_LOCAL_BUSY_CLEAR);

	release_sock(sk);
}

static inline __u8 l2cap_select_mode(__u8 mode, __u16 remote_feat_mask)
{
	switch (mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (l2cap_mode_supported(mode, remote_feat_mask))
			return mode;
		/* fall through */
	default:
		return L2CAP_MODE_BASIC;
	}
}

static void l2cap_setup_txwin(struct l2cap_pinfo *pi)
{
	if (pi->tx_win > L2CAP_TX_WIN_MAX_ENHANCED &&
		(pi->conn->feat_mask & L2CAP_FEAT_EXT_WINDOW)) {
		pi->tx_win_max = L2CAP_TX_WIN_MAX_EXTENDED;
		pi->extended_control = 1;
	} else {
		if (pi->tx_win > L2CAP_TX_WIN_MAX_ENHANCED)
			pi->tx_win = L2CAP_TX_WIN_MAX_ENHANCED;

		pi->tx_win_max = L2CAP_TX_WIN_MAX_ENHANCED;
		pi->extended_control = 0;
	}
	pi->ack_win = pi->tx_win;
}

static void l2cap_aggregate_fs(struct hci_ext_fs *cur,
		struct hci_ext_fs *new,
		struct hci_ext_fs *agg)
{
	*agg = *cur;
	if ((cur->max_sdu != 0xFFFF) && (cur->sdu_arr_time != 0xFFFFFFFF)) {
		/* current flow spec has known rate */
		if ((new->max_sdu == 0xFFFF) ||
				(new->sdu_arr_time == 0xFFFFFFFF)) {
			/* new fs has unknown rate, so aggregate is unknown */
			agg->max_sdu = 0xFFFF;
			agg->sdu_arr_time = 0xFFFFFFFF;
		} else {
			/* new fs has known rate, so aggregate is known */
			u64 cur_rate;
			u64 new_rate;
			cur_rate = cur->max_sdu * 1000000ULL;
			if (cur->sdu_arr_time)
				cur_rate = div_u64(cur_rate, cur->sdu_arr_time);
			new_rate = new->max_sdu * 1000000ULL;
			if (new->sdu_arr_time)
				new_rate = div_u64(new_rate, new->sdu_arr_time);
			cur_rate = cur_rate + new_rate;
			if (cur_rate)
				agg->sdu_arr_time = div64_u64(
					agg->max_sdu * 1000000ULL, cur_rate);
		}
	}
}

static int l2cap_aggregate(struct hci_chan *chan, struct l2cap_pinfo *pi)
{
	struct hci_ext_fs tx_fs;
	struct hci_ext_fs rx_fs;

	BT_DBG("chan %p", chan);

	if (((chan->tx_fs.max_sdu == 0xFFFF) ||
			(chan->tx_fs.sdu_arr_time == 0xFFFFFFFF)) &&
			((chan->rx_fs.max_sdu == 0xFFFF) ||
			(chan->rx_fs.sdu_arr_time == 0xFFFFFFFF)))
		return 0;

	l2cap_aggregate_fs(&chan->tx_fs,
				(struct hci_ext_fs *) &pi->local_fs, &tx_fs);
	l2cap_aggregate_fs(&chan->rx_fs,
				(struct hci_ext_fs *) &pi->remote_fs, &rx_fs);
	hci_chan_modify(chan, &tx_fs, &rx_fs);
	return 1;
}

static void l2cap_deaggregate_fs(struct hci_ext_fs *cur,
		struct hci_ext_fs *old,
		struct hci_ext_fs *agg)
{
	*agg = *cur;
	if ((cur->max_sdu != 0xFFFF) && (cur->sdu_arr_time != 0xFFFFFFFF)) {
		u64 cur_rate;
		u64 old_rate;
		cur_rate = cur->max_sdu * 1000000ULL;
		if (cur->sdu_arr_time)
			cur_rate = div_u64(cur_rate, cur->sdu_arr_time);
		old_rate = old->max_sdu * 1000000ULL;
		if (old->sdu_arr_time)
			old_rate = div_u64(old_rate, old->sdu_arr_time);
		cur_rate = cur_rate - old_rate;
		if (cur_rate)
			agg->sdu_arr_time = div64_u64(
				agg->max_sdu * 1000000ULL, cur_rate);
	}
}

static int l2cap_deaggregate(struct hci_chan *chan, struct l2cap_pinfo *pi)
{
	struct hci_ext_fs tx_fs;
	struct hci_ext_fs rx_fs;

	BT_DBG("chan %p", chan);

	if (((chan->tx_fs.max_sdu == 0xFFFF) ||
			(chan->tx_fs.sdu_arr_time == 0xFFFFFFFF)) &&
			((chan->rx_fs.max_sdu == 0xFFFF) ||
			(chan->rx_fs.sdu_arr_time == 0xFFFFFFFF)))
		return 0;

	l2cap_deaggregate_fs(&chan->tx_fs,
				(struct hci_ext_fs *) &pi->local_fs, &tx_fs);
	l2cap_deaggregate_fs(&chan->rx_fs,
				(struct hci_ext_fs *) &pi->remote_fs, &rx_fs);
	hci_chan_modify(chan, &tx_fs, &rx_fs);
	return 1;
}

static struct hci_chan *l2cap_chan_admit(u8 amp_id, struct sock *sk)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct hci_dev *hdev;
	struct hci_conn *hcon;
	struct hci_chan *chan;

	hdev = hci_dev_get(amp_id);
	if (!hdev)
		return NULL;

	BT_DBG("hdev %s", hdev->name);

	hcon = hci_conn_hash_lookup_ba(hdev, ACL_LINK, pi->conn->dst);
	if (!hcon) {
		chan = NULL;
		goto done;
	}

	chan = hci_chan_list_lookup_id(hdev, hcon->handle);
	if (chan) {
		l2cap_aggregate(chan, pi);
		sock_hold(sk);
		chan->l2cap_sk = sk;
		hci_chan_hold(chan);
		pi->ampchan = chan;
		goto done;
	}

	chan = hci_chan_add(hdev);
	if (chan) {
		chan->conn = hcon;
		sock_hold(sk);
		chan->l2cap_sk = sk;
		hci_chan_hold(chan);
		pi->ampchan = chan;
		hci_chan_create(chan,
			(struct hci_ext_fs *) &pi->local_fs,
			(struct hci_ext_fs *) &pi->remote_fs);
	}
done:
	hci_dev_put(hdev);
	return chan;
}

static void l2cap_get_ertm_timeouts(struct l2cap_conf_rfc *rfc,
						struct l2cap_pinfo *pi)
{
	if (pi->amp_id && pi->ampcon) {
		u64 ertm_to = pi->ampcon->hdev->amp_be_flush_to;

		/* Class 1 devices have must have ERTM timeouts
		 * exceeding the Link Supervision Timeout.  The
		 * default Link Supervision Timeout for AMP
		 * controllers is 10 seconds.
		 *
		 * Class 1 devices use 0xffffffff for their
		 * best-effort flush timeout, so the clamping logic
		 * will result in a timeout that meets the above
		 * requirement.  ERTM timeouts are 16-bit values, so
		 * the maximum timeout is 65.535 seconds.
		 */

		/* Convert timeout to milliseconds and round */
		ertm_to = div_u64(ertm_to + 999, 1000);

		/* This is the recommended formula for class 2 devices
		 * that start ERTM timers when packets are sent to the
		 * controller.
		 */
		ertm_to = 3 * ertm_to + 500;

		if (ertm_to > 0xffff)
			ertm_to = 0xffff;

		rfc->retrans_timeout = cpu_to_le16((u16) ertm_to);
		rfc->monitor_timeout = rfc->retrans_timeout;
	} else {
		rfc->retrans_timeout = cpu_to_le16(L2CAP_DEFAULT_RETRANS_TO);
		rfc->monitor_timeout = cpu_to_le16(L2CAP_DEFAULT_MONITOR_TO);
	}
}

int l2cap_build_conf_req(struct sock *sk, void *data)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct l2cap_conf_req *req = data;
	struct l2cap_conf_rfc rfc = { .mode = pi->mode };
	void *ptr = req->data;

	BT_DBG("sk %p mode %d", sk, pi->mode);

	if (pi->num_conf_req || pi->num_conf_rsp)
		goto done;

	switch (pi->mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (pi->conf_state & L2CAP_CONF_STATE2_DEVICE)
			break;

		/* fall through */
	default:
		pi->mode = l2cap_select_mode(rfc.mode, pi->conn->feat_mask);
		break;
	}

done:
	if (pi->imtu != L2CAP_DEFAULT_MTU)
		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, pi->imtu);

	switch (pi->mode) {
	case L2CAP_MODE_BASIC:
		if (!(pi->conn->feat_mask & L2CAP_FEAT_ERTM) &&
				!(pi->conn->feat_mask & L2CAP_FEAT_STREAMING))
			break;
		rfc.txwin_size      = 0;
		rfc.max_transmit    = 0;
		rfc.retrans_timeout = 0;
		rfc.monitor_timeout = 0;
		rfc.max_pdu_size    = 0;

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
							(unsigned long) &rfc);
		break;

	case L2CAP_MODE_ERTM:
		l2cap_setup_txwin(pi);
		if (pi->tx_win > L2CAP_TX_WIN_MAX_ENHANCED)
			rfc.txwin_size = L2CAP_TX_WIN_MAX_ENHANCED;
		else
			rfc.txwin_size = pi->tx_win;
		rfc.max_transmit = pi->max_tx;
		rfc.max_pdu_size = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);
		l2cap_get_ertm_timeouts(&rfc, pi);

		if (L2CAP_DEFAULT_MAX_PDU_SIZE > pi->imtu)
			rfc.max_pdu_size = cpu_to_le16(pi->imtu);

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
							(unsigned long) &rfc);

		if ((pi->conn->feat_mask & L2CAP_FEAT_EXT_WINDOW) &&
			pi->extended_control) {
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_WINDOW, 2,
					pi->tx_win);
		}

		if (pi->amp_id) {
			/* default best effort extended flow spec */
			struct l2cap_conf_ext_fs fs = {1, 1, 0xFFFF,
					0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_FS,
				sizeof(fs), (unsigned long) &fs);
		}

		if (!(pi->conn->feat_mask & L2CAP_FEAT_FCS))
			break;

		if (pi->fcs == L2CAP_FCS_NONE ||
				pi->conf_state & L2CAP_CONF_NO_FCS_RECV) {
			pi->fcs = L2CAP_FCS_NONE;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1, pi->fcs);
		}
		break;

	case L2CAP_MODE_STREAMING:
		l2cap_setup_txwin(pi);
		rfc.txwin_size      = 0;
		rfc.max_transmit    = 0;
		rfc.retrans_timeout = 0;
		rfc.monitor_timeout = 0;
		rfc.max_pdu_size    = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);
		if (L2CAP_DEFAULT_MAX_PDU_SIZE > pi->imtu)
			rfc.max_pdu_size = cpu_to_le16(pi->imtu);

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
							(unsigned long) &rfc);

		if ((pi->conn->feat_mask & L2CAP_FEAT_EXT_WINDOW) &&
			pi->extended_control) {
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_WINDOW, 2, 0);
		}

		if (!(pi->conn->feat_mask & L2CAP_FEAT_FCS))
			break;

		if (pi->fcs == L2CAP_FCS_NONE ||
				pi->conf_state & L2CAP_CONF_NO_FCS_RECV) {
			pi->fcs = L2CAP_FCS_NONE;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1, pi->fcs);
		}
		break;
	}

	req->dcid  = cpu_to_le16(pi->dcid);
	req->flags = cpu_to_le16(0);

	return ptr - data;
}


static int l2cap_build_amp_reconf_req(struct sock *sk, void *data)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct l2cap_conf_req *req = data;
	struct l2cap_conf_rfc rfc = { .mode = pi->mode };
	void *ptr = req->data;

	BT_DBG("sk %p", sk);

	switch (pi->mode) {
	case L2CAP_MODE_ERTM:
		rfc.mode            = L2CAP_MODE_ERTM;
		rfc.txwin_size      = pi->tx_win;
		rfc.max_transmit    = pi->max_tx;
		rfc.max_pdu_size    = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);
		l2cap_get_ertm_timeouts(&rfc, pi);
		if (L2CAP_DEFAULT_MAX_PDU_SIZE > pi->imtu)
			rfc.max_pdu_size = cpu_to_le16(pi->imtu);

		break;

	default:
		return -ECONNREFUSED;
	}

	l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
						(unsigned long) &rfc);

	if (pi->conn->feat_mask & L2CAP_FEAT_FCS) {
		/* TODO assign fcs for br/edr based on socket config option */
		/* FCS is not used with AMP because it is redundant - lower
		 * layers already include a checksum. */
		if (pi->amp_id)
			pi->local_conf.fcs = L2CAP_FCS_NONE;
		else
			pi->local_conf.fcs = L2CAP_FCS_CRC16;

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1, pi->local_conf.fcs);
		pi->fcs = pi->local_conf.fcs | pi->remote_conf.fcs;
	}

	req->dcid  = cpu_to_le16(pi->dcid);
	req->flags = cpu_to_le16(0);

	return ptr - data;
}

static int l2cap_parse_conf_req(struct sock *sk, void *data)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct l2cap_conf_rsp *rsp = data;
	void *ptr = rsp->data;
	void *req = pi->conf_req;
	int len = pi->conf_len;
	int type, hint, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc = { .mode = L2CAP_MODE_BASIC };
	struct l2cap_conf_ext_fs fs;
	u16 mtu = L2CAP_DEFAULT_MTU;
	u16 result = L2CAP_CONF_SUCCESS;

	BT_DBG("sk %p", sk);

	if (pi->omtu > mtu)
		mtu = pi->omtu;

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&req, &type, &olen, &val);

		hint  = type & L2CAP_CONF_HINT;
		type &= L2CAP_CONF_MASK;

		switch (type) {
		case L2CAP_CONF_MTU:
			mtu = val;
			break;

		case L2CAP_CONF_FLUSH_TO:
			pi->flush_to = val;
			if (pi->conf_state & L2CAP_CONF_LOCKSTEP)
				result = L2CAP_CONF_UNACCEPT;
			else
				pi->remote_conf.flush_to = val;
			break;

		case L2CAP_CONF_QOS:
			if (pi->conf_state & L2CAP_CONF_LOCKSTEP)
				result = L2CAP_CONF_UNACCEPT;
			break;

		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *) val, olen);
			break;

		case L2CAP_CONF_FCS:
			if (val == L2CAP_FCS_NONE)
				pi->conf_state |= L2CAP_CONF_NO_FCS_RECV;
			pi->remote_conf.fcs = val;
			break;

		case L2CAP_CONF_EXT_FS:
			if (olen == sizeof(fs)) {
				pi->conf_state |= L2CAP_CONF_EFS_RECV;
				if (!(pi->conf_state & L2CAP_CONF_LOCKSTEP)) {
					result = L2CAP_CONF_UNACCEPT;
					break;
				}
				memcpy(&fs, (void *) val, olen);
				if (fs.type != L2CAP_SERVICE_BEST_EFFORT) {
					result = L2CAP_CONF_FLOW_SPEC_REJECT;
					break;
				}
				pi->remote_conf.flush_to =
						le32_to_cpu(fs.flush_to);
				pi->remote_fs.id = fs.id;
				pi->remote_fs.type = fs.type;
				pi->remote_fs.max_sdu =
						le16_to_cpu(fs.max_sdu);
				pi->remote_fs.sdu_arr_time =
						le32_to_cpu(fs.sdu_arr_time);
				pi->remote_fs.acc_latency =
						le32_to_cpu(fs.acc_latency);
				pi->remote_fs.flush_to =
						le32_to_cpu(fs.flush_to);
			}
			break;

		case L2CAP_CONF_EXT_WINDOW:
			pi->extended_control = 1;
			pi->remote_tx_win = val;
			pi->tx_win_max = L2CAP_TX_WIN_MAX_EXTENDED;
			pi->conf_state |= L2CAP_CONF_EXT_WIN_RECV;
			break;

		default:
			if (hint)
				break;

			result = L2CAP_CONF_UNKNOWN;
			*((u8 *) ptr++) = type;
			break;
		}
	}

	if (pi->num_conf_rsp || pi->num_conf_req > 1)
		goto done;

	switch (pi->mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (!(pi->conf_state & L2CAP_CONF_STATE2_DEVICE)) {
			pi->mode = l2cap_select_mode(rfc.mode,
					pi->conn->feat_mask);
			break;
		}

		if (pi->mode != rfc.mode)
			return -ECONNREFUSED;

		break;
	}

done:
	if (pi->mode != rfc.mode) {
		result = L2CAP_CONF_UNACCEPT;
		rfc.mode = pi->mode;
		if (mtu > L2CAP_DEFAULT_MTU)
			pi->omtu = mtu;

		if (pi->num_conf_rsp == 1)
			return -ECONNREFUSED;

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc);
	}


	if ((pi->conf_state & L2CAP_CONF_LOCKSTEP) &&
			!(pi->conf_state & L2CAP_CONF_EFS_RECV))
		return -ECONNREFUSED;

	if (result == L2CAP_CONF_SUCCESS) {
		/* Configure output options and let the other side know
		 * which ones we don't like. */

		if (mtu < L2CAP_DEFAULT_MIN_MTU) {
			result = L2CAP_CONF_UNACCEPT;
			pi->omtu = L2CAP_DEFAULT_MIN_MTU;
		}
		else {
			pi->omtu = mtu;
			pi->conf_state |= L2CAP_CONF_MTU_DONE;
		}
		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, pi->omtu);

		switch (rfc.mode) {
		case L2CAP_MODE_BASIC:
			pi->fcs = L2CAP_FCS_NONE;
			pi->conf_state |= L2CAP_CONF_MODE_DONE;
			break;

		case L2CAP_MODE_ERTM:
			if (!(pi->conf_state & L2CAP_CONF_EXT_WIN_RECV))
				pi->remote_tx_win = rfc.txwin_size;
			pi->remote_max_tx = rfc.max_transmit;
			pi->remote_mps = le16_to_cpu(rfc.max_pdu_size);
			l2cap_get_ertm_timeouts(&rfc, pi);

			pi->conf_state |= L2CAP_CONF_MODE_DONE;

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc);

			if (pi->conf_state & L2CAP_CONF_LOCKSTEP)
				l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_FS,
					sizeof(fs), (unsigned long) &fs);

			break;

		case L2CAP_MODE_STREAMING:
			pi->remote_mps = le16_to_cpu(rfc.max_pdu_size);

			pi->conf_state |= L2CAP_CONF_MODE_DONE;

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc);

			break;

		default:
			result = L2CAP_CONF_UNACCEPT;

			memset(&rfc, 0, sizeof(rfc));
			rfc.mode = pi->mode;
		}

		if (pi->conf_state & L2CAP_CONF_LOCKSTEP &&
				!(pi->conf_state & L2CAP_CONF_PEND_SENT)) {
			pi->conf_state |= L2CAP_CONF_PEND_SENT;
			result = L2CAP_CONF_PENDING;

			if (pi->conf_state & L2CAP_CONF_LOCKSTEP_PEND &&
					pi->amp_id) {
				struct hci_chan *chan;
				/* Trigger logical link creation only on AMP */

				chan = l2cap_chan_admit(pi->amp_id, sk);
				if (!chan)
					return -ECONNREFUSED;

				if (chan->state == BT_CONNECTED)
					l2cap_create_cfm(chan, 0);
			}
		}

		if (result == L2CAP_CONF_SUCCESS)
			pi->conf_state |= L2CAP_CONF_OUTPUT_DONE;
	}
	rsp->scid   = cpu_to_le16(pi->dcid);
	rsp->result = cpu_to_le16(result);
	rsp->flags  = cpu_to_le16(0x0000);

	return ptr - data;
}

static int l2cap_parse_amp_move_reconf_req(struct sock *sk, void *data)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct l2cap_conf_rsp *rsp = data;
	void *ptr = rsp->data;
	void *req = pi->conf_req;
	int len = pi->conf_len;
	int type, hint, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc = { .mode = L2CAP_MODE_BASIC };
	struct l2cap_conf_ext_fs fs;
	u16 mtu = pi->omtu;
	u16 tx_win = pi->remote_tx_win;
	u16 result = L2CAP_CONF_SUCCESS;

	BT_DBG("sk %p", sk);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&req, &type, &olen, &val);

		hint  = type & L2CAP_CONF_HINT;
		type &= L2CAP_CONF_MASK;

		switch (type) {
		case L2CAP_CONF_MTU:
			mtu = val;
			break;

		case L2CAP_CONF_FLUSH_TO:
			if (pi->amp_move_id)
				result = L2CAP_CONF_UNACCEPT;
			else
				pi->remote_conf.flush_to = val;
			break;

		case L2CAP_CONF_QOS:
			if (pi->amp_move_id)
				result = L2CAP_CONF_UNACCEPT;
			break;

		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *) val, olen);
			break;

		case L2CAP_CONF_FCS:
			pi->remote_conf.fcs = val;
			break;

		case L2CAP_CONF_EXT_FS:
			if (olen == sizeof(fs)) {
				memcpy(&fs, (void *) val, olen);
				if (fs.type != L2CAP_SERVICE_BEST_EFFORT)
					result = L2CAP_CONF_FLOW_SPEC_REJECT;
				else {
					pi->remote_conf.flush_to =
						le32_to_cpu(fs.flush_to);
				}
			}
			break;

		case L2CAP_CONF_EXT_WINDOW:
			tx_win = val;
			break;

		default:
			if (hint)
				break;

			result = L2CAP_CONF_UNKNOWN;
			*((u8 *) ptr++) = type;
			break;
			}
	}

	BT_DBG("result 0x%2.2x cur mode 0x%2.2x req  mode 0x%2.2x",
		result, pi->mode, rfc.mode);

	if (pi->mode != rfc.mode || rfc.mode == L2CAP_MODE_BASIC)
		result = L2CAP_CONF_UNACCEPT;

	if (result == L2CAP_CONF_SUCCESS) {
		/* Configure output options and let the other side know
		 * which ones we don't like. */

		/* Don't allow mtu to decrease. */
		if (mtu < pi->omtu)
			result = L2CAP_CONF_UNACCEPT;

		BT_DBG("mtu %d omtu %d", mtu, pi->omtu);

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, pi->omtu);

		/* Don't allow extended transmit window to change. */
		if (tx_win != pi->remote_tx_win) {
			result = L2CAP_CONF_UNACCEPT;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_WINDOW, 2,
					pi->remote_tx_win);
		}

		pi->remote_mps = rfc.max_pdu_size;

		if (rfc.mode == L2CAP_MODE_ERTM) {
			l2cap_get_ertm_timeouts(&rfc, pi);
		} else {
			rfc.retrans_timeout = 0;
			rfc.monitor_timeout = 0;
		}

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc);
	}

	if (result != L2CAP_CONF_SUCCESS)
		goto done;

	pi->fcs = pi->remote_conf.fcs | pi->local_conf.fcs;

	if (pi->rx_state == L2CAP_ERTM_RX_STATE_WAIT_F_FLAG)
		pi->flush_to = pi->remote_conf.flush_to;

done:
	rsp->scid   = cpu_to_le16(pi->dcid);
	rsp->result = cpu_to_le16(result);
	rsp->flags  = cpu_to_le16(0x0000);

	return ptr - data;
}

static int l2cap_parse_conf_rsp(struct sock *sk, void *rsp, int len, void *data, u16 *result)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	struct l2cap_conf_req *req = data;
	void *ptr = req->data;
	int type, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc;

	BT_DBG("sk %p, rsp %p, len %d, req %p", sk, rsp, len, data);

	/* Initialize rfc in case no rfc option is received */
	rfc.mode = pi->mode;
	rfc.retrans_timeout = cpu_to_le16(L2CAP_DEFAULT_RETRANS_TO);
	rfc.monitor_timeout = cpu_to_le16(L2CAP_DEFAULT_MONITOR_TO);
	rfc.max_pdu_size = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);

		switch (type) {
		case L2CAP_CONF_MTU:
			if (val < L2CAP_DEFAULT_MIN_MTU) {
				*result = L2CAP_CONF_UNACCEPT;
				pi->imtu = L2CAP_DEFAULT_MIN_MTU;
			} else
				pi->imtu = val;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, pi->imtu);
			break;

		case L2CAP_CONF_FLUSH_TO:
			pi->flush_to = val;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FLUSH_TO,
							2, pi->flush_to);
			break;

		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *)val, olen);

			if ((pi->conf_state & L2CAP_CONF_STATE2_DEVICE) &&
							rfc.mode != pi->mode)
				return -ECONNREFUSED;

			pi->fcs = 0;

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc);
			break;

		case L2CAP_CONF_EXT_WINDOW:
			pi->ack_win = min_t(u16, val, pi->ack_win);

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EXT_WINDOW,
					2, pi->tx_win);
			break;

		default:
			break;
		}
	}

	if (pi->mode == L2CAP_MODE_BASIC && pi->mode != rfc.mode)
		return -ECONNREFUSED;

	pi->mode = rfc.mode;

	if (*result == L2CAP_CONF_SUCCESS) {
		switch (rfc.mode) {
		case L2CAP_MODE_ERTM:
			pi->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
			pi->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
			pi->mps    = le16_to_cpu(rfc.max_pdu_size);
			if (!pi->extended_control) {
				pi->ack_win = min_t(u16, pi->ack_win,
						    rfc.txwin_size);
			}
			break;
		case L2CAP_MODE_STREAMING:
			pi->mps    = le16_to_cpu(rfc.max_pdu_size);
		}
	}

	req->dcid   = cpu_to_le16(pi->dcid);
	req->flags  = cpu_to_le16(0x0000);

	return ptr - data;
}

static int l2cap_build_conf_rsp(struct sock *sk, void *data, u16 result, u16 flags)
{
	struct l2cap_conf_rsp *rsp = data;
	void *ptr = rsp->data;

	BT_DBG("sk %p", sk);

	rsp->scid   = cpu_to_le16(l2cap_pi(sk)->dcid);
	rsp->result = cpu_to_le16(result);
	rsp->flags  = cpu_to_le16(flags);

	return ptr - data;
}

static void l2cap_conf_rfc_get(struct sock *sk, void *rsp, int len)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	int type, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc;
	u16 txwin_ext = pi->ack_win;

	BT_DBG("sk %p, rsp %p, len %d", sk, rsp, len);

	/* Initialize rfc in case no rfc option is received */
	rfc.mode = pi->mode;
	rfc.retrans_timeout = cpu_to_le16(L2CAP_DEFAULT_RETRANS_TO);
	rfc.monitor_timeout = cpu_to_le16(L2CAP_DEFAULT_MONITOR_TO);
	rfc.max_pdu_size = cpu_to_le16(L2CAP_DEFAULT_MAX_PDU_SIZE);
	rfc.txwin_size = min_t(u16, pi->ack_win, L2CAP_DEFAULT_TX_WINDOW);

	if ((pi->mode != L2CAP_MODE_ERTM) && (pi->mode != L2CAP_MODE_STREAMING))
		return;

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);

		switch (type) {
		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *)val, olen);
			break;
		case L2CAP_CONF_EXT_WINDOW:
			txwin_ext = val;
			break;
		}
	}

	switch (rfc.mode) {
	case L2CAP_MODE_ERTM:
		pi->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
		pi->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
		pi->mps    = le16_to_cpu(rfc.max_pdu_size);
		if (pi->extended_control)
			pi->ack_win = min_t(u16, pi->ack_win, txwin_ext);
		else
			pi->ack_win = min_t(u16, pi->ack_win, rfc.txwin_size);
		break;
	case L2CAP_MODE_STREAMING:
		pi->mps    = le16_to_cpu(rfc.max_pdu_size);
	}
}

static void l2cap_conf_ext_fs_get(struct sock *sk, void *rsp, int len)
{
	struct l2cap_pinfo *pi = l2cap_pi(sk);
	int type, olen;
	unsigned long val;
	struct l2cap_conf_ext_fs fs;

	BT_DBG("sk %p, rsp %p, len %d", sk, rsp, len);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);
		if ((type == L2CAP_CONF_EXT_FS) &&
				(olen == sizeof(struct l2cap_conf_ext_fs))) {
			memcpy(&fs, (void *)val, olen);
			pi->local_fs.id = fs.id;
			pi->local_fs.type = fs.type;
			pi->local_fs.max_sdu = le16_to_cpu(fs.max_sdu);
			pi->local_fs.sdu_arr_time =
						le32_to_cpu(fs.sdu_arr_time);
			pi->local_fs.acc_latency = le32_to_cpu(fs.acc_latency);
			pi->local_fs.flush_to = le32_to_cpu(fs.flush_to);
			break;
		}
	}

}

static int l2cap_finish_amp_move(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	int err;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
	pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;

	if (pi->ampcon)
		pi->conn->mtu = pi->ampcon->hdev->acl_mtu;
	else
		pi->conn->mtu = pi->conn->hcon->hdev->acl_mtu;

	err = l2cap_setup_resegment(sk);

	return err;
}

static int l2cap_amp_move_reconf_rsp(struct sock *sk, void *rsp, int len,
					u16 result)
{
	int err = 0;
	struct l2cap_conf_rfc rfc = {.mode = L2CAP_MODE_BASIC};
	struct l2cap_pinfo *pi = l2cap_pi(sk);

	BT_DBG("sk %p, rsp %p, len %d, res 0x%2.2x", sk, rsp, len, result);

	if (pi->reconf_state == L2CAP_RECONF_NONE)
		return -ECONNREFUSED;

	if (result == L2CAP_CONF_SUCCESS) {
		while (len >= L2CAP_CONF_OPT_SIZE) {
			int type, olen;
			unsigned long val;

			len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);

			if (type == L2CAP_CONF_RFC) {
				if (olen == sizeof(rfc))
					memcpy(&rfc, (void *)val, olen);

				if (rfc.mode != pi->mode) {
					l2cap_send_disconn_req(pi->conn, sk,
								ECONNRESET);
					return -ECONNRESET;
				}

				goto done;
			}
		}
	}

	BT_ERR("Expected RFC option was missing, using existing values");

	rfc.mode = pi->mode;
	rfc.retrans_timeout = cpu_to_le16(pi->retrans_timeout);
	rfc.monitor_timeout = cpu_to_le16(pi->monitor_timeout);

done:
	l2cap_ertm_stop_ack_timer(pi);
	l2cap_ertm_stop_retrans_timer(pi);
	l2cap_ertm_stop_monitor_timer(pi);

	pi->mps = le16_to_cpu(rfc.max_pdu_size);
	if (pi->mode == L2CAP_MODE_ERTM) {
		pi->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
		pi->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
	}

	if (l2cap_pi(sk)->reconf_state == L2CAP_RECONF_ACC) {
		l2cap_pi(sk)->reconf_state = L2CAP_RECONF_NONE;

		/* Respond to poll */
		err = l2cap_answer_move_poll(sk);
	} else if (l2cap_pi(sk)->reconf_state == L2CAP_RECONF_INT) {
		if (pi->mode == L2CAP_MODE_ERTM) {
			l2cap_ertm_tx(sk, NULL, NULL,
					L2CAP_ERTM_EVENT_EXPLICIT_POLL);
			pi->rx_state = L2CAP_ERTM_RX_STATE_WAIT_F_FLAG;
		}
	}

	return err;
}


static inline int l2cap_command_rej(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_cmd_rej *rej = (struct l2cap_cmd_rej *) data;

	if (rej->reason != 0x0000)
		return 0;

	if ((conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT) &&
					cmd->ident == conn->info_ident) {
		del_timer(&conn->info_timer);

		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
		conn->info_ident = 0;

		l2cap_conn_start(conn);
	}

	return 0;
}

static struct sock *l2cap_create_connect(struct l2cap_conn *conn,
						struct l2cap_cmd_hdr *cmd,
						u8 *data, u8 rsp_code,
						u8 amp_id)
{
	struct l2cap_chan_list *list = &conn->chan_list;
	struct l2cap_conn_req *req = (struct l2cap_conn_req *) data;
	struct l2cap_conn_rsp rsp;
	struct sock *parent, *sk = NULL;
	int result, status = L2CAP_CS_NO_INFO;

	u16 dcid = 0, scid = __le16_to_cpu(req->scid);
	__le16 psm = req->psm;

	BT_DBG("psm 0x%2.2x scid 0x%4.4x", psm, scid);

	/* Check if we have socket listening on psm */
	parent = l2cap_get_sock_by_psm(BT_LISTEN, psm, conn->src);
	if (!parent) {
		result = L2CAP_CR_BAD_PSM;
		goto sendresp;
	}

	bh_lock_sock(parent);

	/* Check if the ACL is secure enough (if not SDP) */
	if (psm != cpu_to_le16(0x0001) &&
				!hci_conn_check_link_mode(conn->hcon)) {
		conn->disc_reason = 0x05;
		result = L2CAP_CR_SEC_BLOCK;
		goto response;
	}

	result = L2CAP_CR_NO_MEM;

	/* Check for backlog size */
	if (sk_acceptq_is_full(parent)) {
		BT_DBG("backlog full %d", parent->sk_ack_backlog);
		goto response;
	}

	sk = l2cap_sock_alloc(sock_net(parent), NULL, BTPROTO_L2CAP, GFP_ATOMIC);
	if (!sk)
		goto response;

	write_lock_bh(&list->lock);

	/* Check if we already have channel with that dcid */
	if (__l2cap_get_chan_by_dcid(list, scid)) {
		write_unlock_bh(&list->lock);
		sock_set_flag(sk, SOCK_ZAPPED);
		l2cap_sock_kill(sk);
		sk = NULL;
		goto response;
	}

	hci_conn_hold(conn->hcon);

	l2cap_sock_init(sk, parent);
	bacpy(&bt_sk(sk)->src, conn->src);
	bacpy(&bt_sk(sk)->dst, conn->dst);
	l2cap_pi(sk)->psm  = psm;
	l2cap_pi(sk)->dcid = scid;

	bt_accept_enqueue(parent, sk);

	__l2cap_chan_add(conn, sk);
	dcid = l2cap_pi(sk)->scid;
	l2cap_pi(sk)->amp_id = amp_id;

	l2cap_sock_set_timer(sk, sk->sk_sndtimeo);

	l2cap_pi(sk)->ident = cmd->ident;

	if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE) {
		if (l2cap_check_security(sk)) {
			if (bt_sk(sk)->defer_setup) {
				sk->sk_state = BT_CONNECT2;
				result = L2CAP_CR_PEND;
				status = L2CAP_CS_AUTHOR_PEND;
				parent->sk_data_ready(parent, 0);
			} else {
				/* Force pending result for AMP controllers.
				 * The connection will succeed after the
				 * physical link is up. */
				if (amp_id) {
					sk->sk_state = BT_CONNECT2;
					result = L2CAP_CR_PEND;
				} else {
					sk->sk_state = BT_CONFIG;
					result = L2CAP_CR_SUCCESS;
				}
				status = L2CAP_CS_NO_INFO;
			}
		} else {
			sk->sk_state = BT_CONNECT2;
			result = L2CAP_CR_PEND;
			status = L2CAP_CS_AUTHEN_PEND;
		}
	} else {
		sk->sk_state = BT_CONNECT2;
		result = L2CAP_CR_PEND;
		status = L2CAP_CS_NO_INFO;
	}

	write_unlock_bh(&list->lock);

response:
	bh_unlock_sock(parent);

sendresp:
	rsp.scid   = cpu_to_le16(scid);
	rsp.dcid   = cpu_to_le16(dcid);
	rsp.result = cpu_to_le16(result);
	rsp.status = cpu_to_le16(status);
	l2cap_send_cmd(conn, cmd->ident, rsp_code, sizeof(rsp), &rsp);

	if (!(conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE)) {
		struct l2cap_info_req info;
		info.type = cpu_to_le16(L2CAP_IT_FEAT_MASK);

		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_SENT;
		conn->info_ident = l2cap_get_ident(conn);

		mod_timer(&conn->info_timer, jiffies +
					msecs_to_jiffies(L2CAP_INFO_TIMEOUT));

		l2cap_send_cmd(conn, conn->info_ident,
					L2CAP_INFO_REQ, sizeof(info), &info);
	}

	if (sk && !(l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT) &&
				result == L2CAP_CR_SUCCESS) {
		u8 buf[128];
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;
		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
					l2cap_build_conf_req(sk, buf), buf);
		l2cap_pi(sk)->num_conf_req++;
	}

	return sk;
}

static inline int l2cap_connect_req(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	l2cap_create_connect(conn, cmd, data, L2CAP_CONN_RSP, 0);
	return 0;
}

static inline int l2cap_connect_rsp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_conn_rsp *rsp = (struct l2cap_conn_rsp *) data;
	u16 scid, dcid, result, status;
	struct sock *sk;
	u8 req[128];

	scid   = __le16_to_cpu(rsp->scid);
	dcid   = __le16_to_cpu(rsp->dcid);
	result = __le16_to_cpu(rsp->result);
	status = __le16_to_cpu(rsp->status);

	BT_DBG("dcid 0x%4.4x scid 0x%4.4x result 0x%2.2x status 0x%2.2x", dcid, scid, result, status);

	if (scid) {
		sk = l2cap_get_chan_by_scid(&conn->chan_list, scid);
		if (!sk)
			return -EFAULT;
	} else {
		sk = l2cap_get_chan_by_ident(&conn->chan_list, cmd->ident);
		if (!sk)
			return -EFAULT;
	}

	switch (result) {
	case L2CAP_CR_SUCCESS:
		sk->sk_state = BT_CONFIG;
		l2cap_pi(sk)->ident = 0;
		l2cap_pi(sk)->dcid = dcid;
		l2cap_pi(sk)->conf_state &= ~L2CAP_CONF_CONNECT_PEND;

		if (l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT)
			break;

		l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;

		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
					l2cap_build_conf_req(sk, req), req);
		l2cap_pi(sk)->num_conf_req++;
		break;

	case L2CAP_CR_PEND:
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_CONNECT_PEND;
		break;

	default:
		/* don't delete l2cap channel if sk is owned by user */
		if (sock_owned_by_user(sk)) {
			sk->sk_state = BT_DISCONN;
			l2cap_sock_clear_timer(sk);
			l2cap_sock_set_timer(sk, HZ / 5);
			break;
		}

		l2cap_chan_del(sk, ECONNREFUSED);
		break;
	}

	bh_unlock_sock(sk);
	return 0;
}

static inline void set_default_fcs(struct l2cap_pinfo *pi)
{
	/* FCS is enabled only in ERTM or streaming mode, if one or both
	 * sides request it.
	 */
	if (pi->mode != L2CAP_MODE_ERTM && pi->mode != L2CAP_MODE_STREAMING)
		pi->fcs = L2CAP_FCS_NONE;
	else if (!(pi->conf_state & L2CAP_CONF_NO_FCS_RECV))
		pi->fcs = L2CAP_FCS_CRC16;
}

static inline int l2cap_config_req(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u16 cmd_len, u8 *data)
{
	struct l2cap_conf_req *req = (struct l2cap_conf_req *) data;
	u16 dcid, flags;
	u8 rspbuf[64];
	struct l2cap_conf_rsp *rsp = (struct l2cap_conf_rsp *) rspbuf;
	struct sock *sk;
	int len;
	u8 amp_move_reconf = 0;

	dcid  = __le16_to_cpu(req->dcid);
	flags = __le16_to_cpu(req->flags);

	BT_DBG("dcid 0x%4.4x flags 0x%2.2x", dcid, flags);

	sk = l2cap_get_chan_by_scid(&conn->chan_list, dcid);
	if (!sk)
		return -ENOENT;

	BT_DBG("sk_state 0x%2.2x rx_state 0x%2.2x "
		"reconf_state 0x%2.2x amp_id 0x%2.2x amp_move_id 0x%2.2x",
		sk->sk_state, l2cap_pi(sk)->rx_state,
		l2cap_pi(sk)->reconf_state, l2cap_pi(sk)->amp_id,
		l2cap_pi(sk)->amp_move_id);

	/* Detect a reconfig request due to channel move between
	 * BR/EDR and AMP
	 */
	if (sk->sk_state == BT_CONNECTED &&
		l2cap_pi(sk)->rx_state ==
			L2CAP_ERTM_RX_STATE_WAIT_P_FLAG_RECONFIGURE)
		l2cap_pi(sk)->reconf_state = L2CAP_RECONF_ACC;

	if (l2cap_pi(sk)->reconf_state != L2CAP_RECONF_NONE)
		amp_move_reconf = 1;

	if (sk->sk_state != BT_CONFIG && !amp_move_reconf) {
		struct l2cap_cmd_rej rej;

		rej.reason = cpu_to_le16(0x0002);
		l2cap_send_cmd(conn, cmd->ident, L2CAP_COMMAND_REJ,
				sizeof(rej), &rej);
		goto unlock;
	}

	/* Reject if config buffer is too small. */
	len = cmd_len - sizeof(*req);
	if (l2cap_pi(sk)->conf_len + len > sizeof(l2cap_pi(sk)->conf_req)) {
		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
				l2cap_build_conf_rsp(sk, rspbuf,
					L2CAP_CONF_REJECT, flags), rspbuf);
		goto unlock;
	}

	/* Store config. */
	memcpy(l2cap_pi(sk)->conf_req + l2cap_pi(sk)->conf_len, req->data, len);
	l2cap_pi(sk)->conf_len += len;

	if (flags & 0x0001) {
		/* Incomplete config. Send empty response. */
		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
				l2cap_build_conf_rsp(sk, rspbuf,
					L2CAP_CONF_SUCCESS, 0x0001), rspbuf);
		goto unlock;
	}

	/* Complete config. */
	if (!amp_move_reconf)
		len = l2cap_parse_conf_req(sk, rspbuf);
	else
		len = l2cap_parse_amp_move_reconf_req(sk, rspbuf);

	if (len < 0) {
		l2cap_send_disconn_req(conn, sk, ECONNRESET);
		goto unlock;
	}

	l2cap_pi(sk)->conf_ident = cmd->ident;
	l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP, len, rspbuf);

	if (l2cap_pi(sk)->conf_state & L2CAP_CONF_LOCKSTEP &&
			rsp->result == cpu_to_le16(L2CAP_CONF_PENDING) &&
			!l2cap_pi(sk)->amp_id) {
		/* Send success response right after pending if using
		 * lockstep config on BR/EDR
		 */
		rsp->result = cpu_to_le16(L2CAP_CONF_SUCCESS);
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_OUTPUT_DONE;
		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP, len, rspbuf);
	}

	/* Reset config buffer. */
	l2cap_pi(sk)->conf_len = 0;

	if (amp_move_reconf)
		goto unlock;

	l2cap_pi(sk)->num_conf_rsp++;

	if (!(l2cap_pi(sk)->conf_state & L2CAP_CONF_OUTPUT_DONE))
		goto unlock;

	if (l2cap_pi(sk)->conf_state & L2CAP_CONF_INPUT_DONE) {
		set_default_fcs(l2cap_pi(sk));

		sk->sk_state = BT_CONNECTED;

		if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM ||
			l2cap_pi(sk)->mode == L2CAP_MODE_STREAMING)
			l2cap_ertm_init(sk);

		l2cap_chan_ready(sk);
		goto unlock;
	}

	if (!(l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT)) {
		u8 buf[64];
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;
		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
					l2cap_build_conf_req(sk, buf), buf);
		l2cap_pi(sk)->num_conf_req++;
	}

unlock:
	bh_unlock_sock(sk);
	return 0;
}

static inline int l2cap_config_rsp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_conf_rsp *rsp = (struct l2cap_conf_rsp *)data;
	u16 scid, flags, result;
	struct sock *sk;
	struct l2cap_pinfo *pi;
	int len = cmd->len - sizeof(*rsp);

	scid   = __le16_to_cpu(rsp->scid);
	flags  = __le16_to_cpu(rsp->flags);
	result = __le16_to_cpu(rsp->result);

	BT_DBG("scid 0x%4.4x flags 0x%2.2x result 0x%2.2x",
			scid, flags, result);

	sk = l2cap_get_chan_by_scid(&conn->chan_list, scid);
	if (!sk)
		return 0;

	pi = l2cap_pi(sk);

	if (pi->reconf_state != L2CAP_RECONF_NONE)  {
		l2cap_amp_move_reconf_rsp(sk, rsp->data, len, result);
		goto done;
	}

	switch (result) {
	case L2CAP_CONF_SUCCESS:
		if (pi->conf_state & L2CAP_CONF_LOCKSTEP &&
				!(pi->conf_state & L2CAP_CONF_LOCKSTEP_PEND)) {
			/* Lockstep procedure requires a pending response
			 * before success.
			 */
			l2cap_send_disconn_req(conn, sk, ECONNRESET);
			goto done;
		}

		l2cap_conf_rfc_get(sk, rsp->data, len);
		break;

	case L2CAP_CONF_PENDING:
		if (!(pi->conf_state & L2CAP_CONF_LOCKSTEP)) {
			l2cap_send_disconn_req(conn, sk, ECONNRESET);
			goto done;
		}

		l2cap_conf_rfc_get(sk, rsp->data, len);

		pi->conf_state |= L2CAP_CONF_LOCKSTEP_PEND;

		l2cap_conf_ext_fs_get(sk, rsp->data, len);

		if (pi->amp_id && pi->conf_state & L2CAP_CONF_PEND_SENT) {
			struct hci_chan *chan;

			/* Already sent a 'pending' response, so set up
			 * the logical link now
			 */
			chan = l2cap_chan_admit(pi->amp_id, sk);
			if (!chan) {
				l2cap_send_disconn_req(pi->conn, sk,
							ECONNRESET);
				goto done;
			}

			if (chan->state == BT_CONNECTED)
				l2cap_create_cfm(chan, 0);
		}

		goto done;

	case L2CAP_CONF_UNACCEPT:
		if (pi->num_conf_rsp <= L2CAP_CONF_MAX_CONF_RSP) {
			char req[64];

			if (len > sizeof(req) - sizeof(struct l2cap_conf_req)) {
				l2cap_send_disconn_req(conn, sk, ECONNRESET);
				goto done;
			}

			/* throw out any old stored conf requests */
			result = L2CAP_CONF_SUCCESS;
			len = l2cap_parse_conf_rsp(sk, rsp->data,
							len, req, &result);
			if (len < 0) {
				l2cap_send_disconn_req(conn, sk, ECONNRESET);
				goto done;
			}

			l2cap_send_cmd(conn, l2cap_get_ident(conn),
						L2CAP_CONF_REQ, len, req);
			pi->num_conf_req++;
			if (result != L2CAP_CONF_SUCCESS)
				goto done;
			break;
		}

	default:
		sk->sk_err = ECONNRESET;
		l2cap_sock_set_timer(sk, HZ * 5);
		l2cap_send_disconn_req(conn, sk, ECONNRESET);
		goto done;
	}

	if (flags & 0x01)
		goto done;

	pi->conf_state |= L2CAP_CONF_INPUT_DONE;

	if (pi->conf_state & L2CAP_CONF_OUTPUT_DONE) {
		set_default_fcs(pi);

		sk->sk_state = BT_CONNECTED;

		if (pi->mode == L2CAP_MODE_ERTM ||
			pi->mode == L2CAP_MODE_STREAMING)
			l2cap_ertm_init(sk);

		l2cap_chan_ready(sk);
	}

done:
	bh_unlock_sock(sk);
	return 0;
}

static inline int l2cap_disconnect_req(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_disconn_req *req = (struct l2cap_disconn_req *) data;
	struct l2cap_disconn_rsp rsp;
	u16 dcid, scid;
	struct sock *sk;

	scid = __le16_to_cpu(req->scid);
	dcid = __le16_to_cpu(req->dcid);

	BT_DBG("scid 0x%4.4x dcid 0x%4.4x", scid, dcid);

	sk = l2cap_get_chan_by_scid(&conn->chan_list, dcid);
	if (!sk)
		return 0;

	rsp.dcid = cpu_to_le16(l2cap_pi(sk)->scid);
	rsp.scid = cpu_to_le16(l2cap_pi(sk)->dcid);
	l2cap_send_cmd(conn, cmd->ident, L2CAP_DISCONN_RSP, sizeof(rsp), &rsp);

	/* Only do cleanup if a disconnect request was not sent already */
	if (sk->sk_state != BT_DISCONN) {
		sk->sk_shutdown = SHUTDOWN_MASK;

		sk->sk_send_head = NULL;
		skb_queue_purge(TX_QUEUE(sk));

		if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM) {
			skb_queue_purge(SREJ_QUEUE(sk));

			__cancel_delayed_work(&l2cap_pi(sk)->ack_work);
			__cancel_delayed_work(&l2cap_pi(sk)->retrans_work);
			__cancel_delayed_work(&l2cap_pi(sk)->monitor_work);
		}
	}

	/* don't delete l2cap channel if sk is owned by user */
	if (sock_owned_by_user(sk)) {
		sk->sk_state = BT_DISCONN;
		l2cap_sock_clear_timer(sk);
		l2cap_sock_set_timer(sk, HZ / 5);
		bh_unlock_sock(sk);
		return 0;
	}

	l2cap_chan_del(sk, ECONNRESET);

	bh_unlock_sock(sk);

	l2cap_sock_kill(sk);
	return 0;
}

static inline int l2cap_disconnect_rsp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_disconn_rsp *rsp = (struct l2cap_disconn_rsp *) data;
	u16 dcid, scid;
	struct sock *sk;

	scid = __le16_to_cpu(rsp->scid);
	dcid = __le16_to_cpu(rsp->dcid);

	BT_DBG("dcid 0x%4.4x scid 0x%4.4x", dcid, scid);

	sk = l2cap_get_chan_by_scid(&conn->chan_list, scid);
	if (!sk)
		return 0;

	/* don't delete l2cap channel if sk is owned by user */
	if (sock_owned_by_user(sk)) {
		sk->sk_state = BT_DISCONN;
		l2cap_sock_clear_timer(sk);
		l2cap_sock_set_timer(sk, HZ / 5);
		bh_unlock_sock(sk);
		return 0;
	}

	l2cap_chan_del(sk, 0);
	bh_unlock_sock(sk);

	l2cap_sock_kill(sk);
	return 0;
}

static inline int l2cap_information_req(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_info_req *req = (struct l2cap_info_req *) data;
	u16 type;

	type = __le16_to_cpu(req->type);

	BT_DBG("type 0x%4.4x", type);

	if (type == L2CAP_IT_FEAT_MASK) {
		u8 buf[8];
		u32 feat_mask = l2cap_feat_mask;
		struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) buf;
		rsp->type   = cpu_to_le16(L2CAP_IT_FEAT_MASK);
		rsp->result = cpu_to_le16(L2CAP_IR_SUCCESS);
		if (!disable_ertm)
			feat_mask |= L2CAP_FEAT_ERTM | L2CAP_FEAT_STREAMING
				| L2CAP_FEAT_FCS | L2CAP_FEAT_EXT_WINDOW;
		put_unaligned_le32(feat_mask, rsp->data);
		l2cap_send_cmd(conn, cmd->ident,
					L2CAP_INFO_RSP, sizeof(buf), buf);
	} else if (type == L2CAP_IT_FIXED_CHAN) {
		u8 buf[12];
		u8 fc_mask = l2cap_fc_mask;
		struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) buf;
		rsp->type   = cpu_to_le16(L2CAP_IT_FIXED_CHAN);
		rsp->result = cpu_to_le16(L2CAP_IR_SUCCESS);
		if (enable_hs)
			fc_mask |= L2CAP_FC_A2MP;
		memset(rsp->data, 0, 8);
		rsp->data[0] = fc_mask;
		l2cap_send_cmd(conn, cmd->ident,
					L2CAP_INFO_RSP, sizeof(buf), buf);
	} else {
		struct l2cap_info_rsp rsp;
		rsp.type   = cpu_to_le16(type);
		rsp.result = cpu_to_le16(L2CAP_IR_NOTSUPP);
		l2cap_send_cmd(conn, cmd->ident,
					L2CAP_INFO_RSP, sizeof(rsp), &rsp);
=======
static int l2cap_sar_segment_sdu(struct l2cap_chan *chan, struct msghdr *msg, size_t len)
{
	struct sk_buff *skb;
	struct sk_buff_head sar_queue;
	u32 control;
	size_t size = 0;

	skb_queue_head_init(&sar_queue);
	control = __set_ctrl_sar(chan, L2CAP_SAR_START);
	skb = l2cap_create_iframe_pdu(chan, msg, chan->remote_mps, control, len);
	if (IS_ERR(skb))
		return PTR_ERR(skb);

	__skb_queue_tail(&sar_queue, skb);
	len -= chan->remote_mps;
	size += chan->remote_mps;

	while (len > 0) {
		size_t buflen;

		if (len > chan->remote_mps) {
			control = __set_ctrl_sar(chan, L2CAP_SAR_CONTINUE);
			buflen = chan->remote_mps;
		} else {
			control = __set_ctrl_sar(chan, L2CAP_SAR_END);
			buflen = len;
		}

		skb = l2cap_create_iframe_pdu(chan, msg, buflen, control, 0);
		if (IS_ERR(skb)) {
			skb_queue_purge(&sar_queue);
			return PTR_ERR(skb);
		}

		__skb_queue_tail(&sar_queue, skb);
		len -= buflen;
		size += buflen;
	}
	skb_queue_splice_tail(&sar_queue, &chan->tx_q);
	if (chan->tx_send_head == NULL)
		chan->tx_send_head = sar_queue.next;

	return size;
}

int l2cap_chan_send(struct l2cap_chan *chan, struct msghdr *msg, size_t len,
								u32 priority)
{
	struct sk_buff *skb;
	u32 control;
	int err;

	/* Connectionless channel */
	if (chan->chan_type == L2CAP_CHAN_CONN_LESS) {
		skb = l2cap_create_connless_pdu(chan, msg, len, priority);
		if (IS_ERR(skb))
			return PTR_ERR(skb);

		l2cap_do_send(chan, skb);
		return len;
	}

	switch (chan->mode) {
	case L2CAP_MODE_BASIC:
		/* Check outgoing MTU */
		if (len > chan->omtu)
			return -EMSGSIZE;

		/* Create a basic PDU */
		skb = l2cap_create_basic_pdu(chan, msg, len, priority);
		if (IS_ERR(skb))
			return PTR_ERR(skb);

		l2cap_do_send(chan, skb);
		err = len;
		break;

	case L2CAP_MODE_ERTM:
	case L2CAP_MODE_STREAMING:
		/* Entire SDU fits into one PDU */
		if (len <= chan->remote_mps) {
			control = __set_ctrl_sar(chan, L2CAP_SAR_UNSEGMENTED);
			skb = l2cap_create_iframe_pdu(chan, msg, len, control,
									0);
			if (IS_ERR(skb))
				return PTR_ERR(skb);

			__skb_queue_tail(&chan->tx_q, skb);

			if (chan->tx_send_head == NULL)
				chan->tx_send_head = skb;

		} else {
			/* Segment SDU into multiples PDUs */
			err = l2cap_sar_segment_sdu(chan, msg, len);
			if (err < 0)
				return err;
		}

		if (chan->mode == L2CAP_MODE_STREAMING) {
			l2cap_streaming_send(chan);
			err = len;
			break;
		}

		if (test_bit(CONN_REMOTE_BUSY, &chan->conn_state) &&
				test_bit(CONN_WAIT_F, &chan->conn_state)) {
			err = len;
			break;
		}

		err = l2cap_ertm_send(chan);
		if (err >= 0)
			err = len;

		break;

	default:
		BT_DBG("bad state %1.1x", chan->mode);
		err = -EBADFD;
	}

	return err;
}

/* Copy frame to all raw sockets on that connection */
static void l2cap_raw_recv(struct l2cap_conn *conn, struct sk_buff *skb)
{
	struct sk_buff *nskb;
	struct l2cap_chan *chan;

	BT_DBG("conn %p", conn);

	mutex_lock(&conn->chan_lock);

	list_for_each_entry(chan, &conn->chan_l, list) {
		struct sock *sk = chan->sk;
		if (chan->chan_type != L2CAP_CHAN_RAW)
			continue;

		/* Don't send frame to the socket it came from */
		if (skb->sk == sk)
			continue;
		nskb = skb_clone(skb, GFP_ATOMIC);
		if (!nskb)
			continue;

		if (chan->ops->recv(chan->data, nskb))
			kfree_skb(nskb);
	}

	mutex_unlock(&conn->chan_lock);
}

/* ---- L2CAP signalling commands ---- */
static struct sk_buff *l2cap_build_cmd(struct l2cap_conn *conn,
				u8 code, u8 ident, u16 dlen, void *data)
{
	struct sk_buff *skb, **frag;
	struct l2cap_cmd_hdr *cmd;
	struct l2cap_hdr *lh;
	int len, count;

	BT_DBG("conn %p, code 0x%2.2x, ident 0x%2.2x, len %d",
			conn, code, ident, dlen);

	len = L2CAP_HDR_SIZE + L2CAP_CMD_HDR_SIZE + dlen;
	count = min_t(unsigned int, conn->mtu, len);

	skb = bt_skb_alloc(count, GFP_ATOMIC);
	if (!skb)
		return NULL;

	lh = (struct l2cap_hdr *) skb_put(skb, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(L2CAP_CMD_HDR_SIZE + dlen);

	if (conn->hcon->type == LE_LINK)
		lh->cid = cpu_to_le16(L2CAP_CID_LE_SIGNALING);
	else
		lh->cid = cpu_to_le16(L2CAP_CID_SIGNALING);

	cmd = (struct l2cap_cmd_hdr *) skb_put(skb, L2CAP_CMD_HDR_SIZE);
	cmd->code  = code;
	cmd->ident = ident;
	cmd->len   = cpu_to_le16(dlen);

	if (dlen) {
		count -= L2CAP_HDR_SIZE + L2CAP_CMD_HDR_SIZE;
		memcpy(skb_put(skb, count), data, count);
		data += count;
	}

	len -= skb->len;

	/* Continuation fragments (no L2CAP header) */
	frag = &skb_shinfo(skb)->frag_list;
	while (len) {
		count = min_t(unsigned int, conn->mtu, len);

		*frag = bt_skb_alloc(count, GFP_ATOMIC);
		if (!*frag)
			goto fail;

		memcpy(skb_put(*frag, count), data, count);

		len  -= count;
		data += count;

		frag = &(*frag)->next;
	}

	return skb;

fail:
	kfree_skb(skb);
	return NULL;
}

static inline int l2cap_get_conf_opt(void **ptr, int *type, int *olen, unsigned long *val)
{
	struct l2cap_conf_opt *opt = *ptr;
	int len;

	len = L2CAP_CONF_OPT_SIZE + opt->len;
	*ptr += len;

	*type = opt->type;
	*olen = opt->len;

	switch (opt->len) {
	case 1:
		*val = *((u8 *) opt->val);
		break;

	case 2:
		*val = get_unaligned_le16(opt->val);
		break;

	case 4:
		*val = get_unaligned_le32(opt->val);
		break;

	default:
		*val = (unsigned long) opt->val;
		break;
	}

	BT_DBG("type 0x%2.2x len %d val 0x%lx", *type, opt->len, *val);
	return len;
}

static void l2cap_add_conf_opt(void **ptr, u8 type, u8 len, unsigned long val)
{
	struct l2cap_conf_opt *opt = *ptr;

	BT_DBG("type 0x%2.2x len %d val 0x%lx", type, len, val);

	opt->type = type;
	opt->len  = len;

	switch (len) {
	case 1:
		*((u8 *) opt->val)  = val;
		break;

	case 2:
		put_unaligned_le16(val, opt->val);
		break;

	case 4:
		put_unaligned_le32(val, opt->val);
		break;

	default:
		memcpy(opt->val, (void *) val, len);
		break;
	}

	*ptr += L2CAP_CONF_OPT_SIZE + len;
}

static void l2cap_add_opt_efs(void **ptr, struct l2cap_chan *chan)
{
	struct l2cap_conf_efs efs;

	switch (chan->mode) {
	case L2CAP_MODE_ERTM:
		efs.id		= chan->local_id;
		efs.stype	= chan->local_stype;
		efs.msdu	= cpu_to_le16(chan->local_msdu);
		efs.sdu_itime	= cpu_to_le32(chan->local_sdu_itime);
		efs.acc_lat	= cpu_to_le32(L2CAP_DEFAULT_ACC_LAT);
		efs.flush_to	= cpu_to_le32(L2CAP_DEFAULT_FLUSH_TO);
		break;

	case L2CAP_MODE_STREAMING:
		efs.id		= 1;
		efs.stype	= L2CAP_SERV_BESTEFFORT;
		efs.msdu	= cpu_to_le16(chan->local_msdu);
		efs.sdu_itime	= cpu_to_le32(chan->local_sdu_itime);
		efs.acc_lat	= 0;
		efs.flush_to	= 0;
		break;

	default:
		return;
	}

	l2cap_add_conf_opt(ptr, L2CAP_CONF_EFS, sizeof(efs),
							(unsigned long) &efs);
}

static void l2cap_ack_timeout(struct work_struct *work)
{
	struct l2cap_chan *chan = container_of(work, struct l2cap_chan,
							ack_timer.work);

	BT_DBG("chan %p", chan);

	l2cap_chan_lock(chan);

	__l2cap_send_ack(chan);

	l2cap_chan_unlock(chan);

	l2cap_chan_put(chan);
}

static inline void l2cap_ertm_init(struct l2cap_chan *chan)
{
	chan->expected_ack_seq = 0;
	chan->unacked_frames = 0;
	chan->buffer_seq = 0;
	chan->num_acked = 0;
	chan->frames_sent = 0;

	INIT_DELAYED_WORK(&chan->retrans_timer, l2cap_retrans_timeout);
	INIT_DELAYED_WORK(&chan->monitor_timer, l2cap_monitor_timeout);
	INIT_DELAYED_WORK(&chan->ack_timer, l2cap_ack_timeout);

	skb_queue_head_init(&chan->srej_q);

	INIT_LIST_HEAD(&chan->srej_l);
}

static inline __u8 l2cap_select_mode(__u8 mode, __u16 remote_feat_mask)
{
	switch (mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (l2cap_mode_supported(mode, remote_feat_mask))
			return mode;
		/* fall through */
	default:
		return L2CAP_MODE_BASIC;
	}
}

static inline bool __l2cap_ews_supported(struct l2cap_chan *chan)
{
	return enable_hs && chan->conn->feat_mask & L2CAP_FEAT_EXT_WINDOW;
}

static inline bool __l2cap_efs_supported(struct l2cap_chan *chan)
{
	return enable_hs && chan->conn->feat_mask & L2CAP_FEAT_EXT_FLOW;
}

static inline void l2cap_txwin_setup(struct l2cap_chan *chan)
{
	if (chan->tx_win > L2CAP_DEFAULT_TX_WINDOW &&
						__l2cap_ews_supported(chan)) {
		/* use extended control field */
		set_bit(FLAG_EXT_CTRL, &chan->flags);
		chan->tx_win_max = L2CAP_DEFAULT_EXT_WINDOW;
	} else {
		chan->tx_win = min_t(u16, chan->tx_win,
						L2CAP_DEFAULT_TX_WINDOW);
		chan->tx_win_max = L2CAP_DEFAULT_TX_WINDOW;
	}
}

static int l2cap_build_conf_req(struct l2cap_chan *chan, void *data)
{
	struct l2cap_conf_req *req = data;
	struct l2cap_conf_rfc rfc = { .mode = chan->mode };
	void *ptr = req->data;
	u16 size;

	BT_DBG("chan %p", chan);

	if (chan->num_conf_req || chan->num_conf_rsp)
		goto done;

	switch (chan->mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (test_bit(CONF_STATE2_DEVICE, &chan->conf_state))
			break;

		if (__l2cap_efs_supported(chan))
			set_bit(FLAG_EFS_ENABLE, &chan->flags);

		/* fall through */
	default:
		chan->mode = l2cap_select_mode(rfc.mode, chan->conn->feat_mask);
		break;
	}

done:
	if (chan->imtu != L2CAP_DEFAULT_MTU)
		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, chan->imtu);

	switch (chan->mode) {
	case L2CAP_MODE_BASIC:
		if (!(chan->conn->feat_mask & L2CAP_FEAT_ERTM) &&
				!(chan->conn->feat_mask & L2CAP_FEAT_STREAMING))
			break;

		rfc.mode            = L2CAP_MODE_BASIC;
		rfc.txwin_size      = 0;
		rfc.max_transmit    = 0;
		rfc.retrans_timeout = 0;
		rfc.monitor_timeout = 0;
		rfc.max_pdu_size    = 0;

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
							(unsigned long) &rfc);
		break;

	case L2CAP_MODE_ERTM:
		rfc.mode            = L2CAP_MODE_ERTM;
		rfc.max_transmit    = chan->max_tx;
		rfc.retrans_timeout = 0;
		rfc.monitor_timeout = 0;

		size = min_t(u16, L2CAP_DEFAULT_MAX_PDU_SIZE, chan->conn->mtu -
						L2CAP_EXT_HDR_SIZE -
						L2CAP_SDULEN_SIZE -
						L2CAP_FCS_SIZE);
		rfc.max_pdu_size = cpu_to_le16(size);

		l2cap_txwin_setup(chan);

		rfc.txwin_size = min_t(u16, chan->tx_win,
						L2CAP_DEFAULT_TX_WINDOW);

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
							(unsigned long) &rfc);

		if (test_bit(FLAG_EFS_ENABLE, &chan->flags))
			l2cap_add_opt_efs(&ptr, chan);

		if (!(chan->conn->feat_mask & L2CAP_FEAT_FCS))
			break;

		if (chan->fcs == L2CAP_FCS_NONE ||
				test_bit(CONF_NO_FCS_RECV, &chan->conf_state)) {
			chan->fcs = L2CAP_FCS_NONE;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1, chan->fcs);
		}

		if (test_bit(FLAG_EXT_CTRL, &chan->flags))
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EWS, 2,
								chan->tx_win);
		break;

	case L2CAP_MODE_STREAMING:
		rfc.mode            = L2CAP_MODE_STREAMING;
		rfc.txwin_size      = 0;
		rfc.max_transmit    = 0;
		rfc.retrans_timeout = 0;
		rfc.monitor_timeout = 0;

		size = min_t(u16, L2CAP_DEFAULT_MAX_PDU_SIZE, chan->conn->mtu -
						L2CAP_EXT_HDR_SIZE -
						L2CAP_SDULEN_SIZE -
						L2CAP_FCS_SIZE);
		rfc.max_pdu_size = cpu_to_le16(size);

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC, sizeof(rfc),
							(unsigned long) &rfc);

		if (test_bit(FLAG_EFS_ENABLE, &chan->flags))
			l2cap_add_opt_efs(&ptr, chan);

		if (!(chan->conn->feat_mask & L2CAP_FEAT_FCS))
			break;

		if (chan->fcs == L2CAP_FCS_NONE ||
				test_bit(CONF_NO_FCS_RECV, &chan->conf_state)) {
			chan->fcs = L2CAP_FCS_NONE;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FCS, 1, chan->fcs);
		}
		break;
	}

	req->dcid  = cpu_to_le16(chan->dcid);
	req->flags = cpu_to_le16(0);

	return ptr - data;
}

static int l2cap_parse_conf_req(struct l2cap_chan *chan, void *data)
{
	struct l2cap_conf_rsp *rsp = data;
	void *ptr = rsp->data;
	void *req = chan->conf_req;
	int len = chan->conf_len;
	int type, hint, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc = { .mode = L2CAP_MODE_BASIC };
	struct l2cap_conf_efs efs;
	u8 remote_efs = 0;
	u16 mtu = L2CAP_DEFAULT_MTU;
	u16 result = L2CAP_CONF_SUCCESS;
	u16 size;

	BT_DBG("chan %p", chan);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&req, &type, &olen, &val);

		hint  = type & L2CAP_CONF_HINT;
		type &= L2CAP_CONF_MASK;

		switch (type) {
		case L2CAP_CONF_MTU:
			mtu = val;
			break;

		case L2CAP_CONF_FLUSH_TO:
			chan->flush_to = val;
			break;

		case L2CAP_CONF_QOS:
			break;

		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *) val, olen);
			break;

		case L2CAP_CONF_FCS:
			if (val == L2CAP_FCS_NONE)
				set_bit(CONF_NO_FCS_RECV, &chan->conf_state);
			break;

		case L2CAP_CONF_EFS:
			remote_efs = 1;
			if (olen == sizeof(efs))
				memcpy(&efs, (void *) val, olen);
			break;

		case L2CAP_CONF_EWS:
			if (!enable_hs)
				return -ECONNREFUSED;

			set_bit(FLAG_EXT_CTRL, &chan->flags);
			set_bit(CONF_EWS_RECV, &chan->conf_state);
			chan->tx_win_max = L2CAP_DEFAULT_EXT_WINDOW;
			chan->remote_tx_win = val;
			break;

		default:
			if (hint)
				break;

			result = L2CAP_CONF_UNKNOWN;
			*((u8 *) ptr++) = type;
			break;
		}
	}

	if (chan->num_conf_rsp || chan->num_conf_req > 1)
		goto done;

	switch (chan->mode) {
	case L2CAP_MODE_STREAMING:
	case L2CAP_MODE_ERTM:
		if (!test_bit(CONF_STATE2_DEVICE, &chan->conf_state)) {
			chan->mode = l2cap_select_mode(rfc.mode,
					chan->conn->feat_mask);
			break;
		}

		if (remote_efs) {
			if (__l2cap_efs_supported(chan))
				set_bit(FLAG_EFS_ENABLE, &chan->flags);
			else
				return -ECONNREFUSED;
		}

		if (chan->mode != rfc.mode)
			return -ECONNREFUSED;

		break;
	}

done:
	if (chan->mode != rfc.mode) {
		result = L2CAP_CONF_UNACCEPT;
		rfc.mode = chan->mode;

		if (chan->num_conf_rsp == 1)
			return -ECONNREFUSED;

		l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc);
	}

	if (result == L2CAP_CONF_SUCCESS) {
		/* Configure output options and let the other side know
		 * which ones we don't like. */

		if (mtu < L2CAP_DEFAULT_MIN_MTU)
			result = L2CAP_CONF_UNACCEPT;
		else {
			chan->omtu = mtu;
			set_bit(CONF_MTU_DONE, &chan->conf_state);
		}
		l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, chan->omtu);

		if (remote_efs) {
			if (chan->local_stype != L2CAP_SERV_NOTRAFIC &&
					efs.stype != L2CAP_SERV_NOTRAFIC &&
					efs.stype != chan->local_stype) {

				result = L2CAP_CONF_UNACCEPT;

				if (chan->num_conf_req >= 1)
					return -ECONNREFUSED;

				l2cap_add_conf_opt(&ptr, L2CAP_CONF_EFS,
							sizeof(efs),
							(unsigned long) &efs);
			} else {
				/* Send PENDING Conf Rsp */
				result = L2CAP_CONF_PENDING;
				set_bit(CONF_LOC_CONF_PEND, &chan->conf_state);
			}
		}

		switch (rfc.mode) {
		case L2CAP_MODE_BASIC:
			chan->fcs = L2CAP_FCS_NONE;
			set_bit(CONF_MODE_DONE, &chan->conf_state);
			break;

		case L2CAP_MODE_ERTM:
			if (!test_bit(CONF_EWS_RECV, &chan->conf_state))
				chan->remote_tx_win = rfc.txwin_size;
			else
				rfc.txwin_size = L2CAP_DEFAULT_TX_WINDOW;

			chan->remote_max_tx = rfc.max_transmit;

			size = min_t(u16, le16_to_cpu(rfc.max_pdu_size),
						chan->conn->mtu -
						L2CAP_EXT_HDR_SIZE -
						L2CAP_SDULEN_SIZE -
						L2CAP_FCS_SIZE);
			rfc.max_pdu_size = cpu_to_le16(size);
			chan->remote_mps = size;

			rfc.retrans_timeout =
				le16_to_cpu(L2CAP_DEFAULT_RETRANS_TO);
			rfc.monitor_timeout =
				le16_to_cpu(L2CAP_DEFAULT_MONITOR_TO);

			set_bit(CONF_MODE_DONE, &chan->conf_state);

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc);

			if (test_bit(FLAG_EFS_ENABLE, &chan->flags)) {
				chan->remote_id = efs.id;
				chan->remote_stype = efs.stype;
				chan->remote_msdu = le16_to_cpu(efs.msdu);
				chan->remote_flush_to =
						le32_to_cpu(efs.flush_to);
				chan->remote_acc_lat =
						le32_to_cpu(efs.acc_lat);
				chan->remote_sdu_itime =
					le32_to_cpu(efs.sdu_itime);
				l2cap_add_conf_opt(&ptr, L2CAP_CONF_EFS,
					sizeof(efs), (unsigned long) &efs);
			}
			break;

		case L2CAP_MODE_STREAMING:
			size = min_t(u16, le16_to_cpu(rfc.max_pdu_size),
						chan->conn->mtu -
						L2CAP_EXT_HDR_SIZE -
						L2CAP_SDULEN_SIZE -
						L2CAP_FCS_SIZE);
			rfc.max_pdu_size = cpu_to_le16(size);
			chan->remote_mps = size;

			set_bit(CONF_MODE_DONE, &chan->conf_state);

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc);

			break;

		default:
			result = L2CAP_CONF_UNACCEPT;

			memset(&rfc, 0, sizeof(rfc));
			rfc.mode = chan->mode;
		}

		if (result == L2CAP_CONF_SUCCESS)
			set_bit(CONF_OUTPUT_DONE, &chan->conf_state);
	}
	rsp->scid   = cpu_to_le16(chan->dcid);
	rsp->result = cpu_to_le16(result);
	rsp->flags  = cpu_to_le16(0x0000);

	return ptr - data;
}

static int l2cap_parse_conf_rsp(struct l2cap_chan *chan, void *rsp, int len, void *data, u16 *result)
{
	struct l2cap_conf_req *req = data;
	void *ptr = req->data;
	int type, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc = { .mode = L2CAP_MODE_BASIC };
	struct l2cap_conf_efs efs;

	BT_DBG("chan %p, rsp %p, len %d, req %p", chan, rsp, len, data);

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);

		switch (type) {
		case L2CAP_CONF_MTU:
			if (val < L2CAP_DEFAULT_MIN_MTU) {
				*result = L2CAP_CONF_UNACCEPT;
				chan->imtu = L2CAP_DEFAULT_MIN_MTU;
			} else
				chan->imtu = val;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_MTU, 2, chan->imtu);
			break;

		case L2CAP_CONF_FLUSH_TO:
			chan->flush_to = val;
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_FLUSH_TO,
							2, chan->flush_to);
			break;

		case L2CAP_CONF_RFC:
			if (olen == sizeof(rfc))
				memcpy(&rfc, (void *)val, olen);

			if (test_bit(CONF_STATE2_DEVICE, &chan->conf_state) &&
							rfc.mode != chan->mode)
				return -ECONNREFUSED;

			chan->fcs = 0;

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_RFC,
					sizeof(rfc), (unsigned long) &rfc);
			break;

		case L2CAP_CONF_EWS:
			chan->tx_win = min_t(u16, val,
						L2CAP_DEFAULT_EXT_WINDOW);
			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EWS, 2,
							chan->tx_win);
			break;

		case L2CAP_CONF_EFS:
			if (olen == sizeof(efs))
				memcpy(&efs, (void *)val, olen);

			if (chan->local_stype != L2CAP_SERV_NOTRAFIC &&
					efs.stype != L2CAP_SERV_NOTRAFIC &&
					efs.stype != chan->local_stype)
				return -ECONNREFUSED;

			l2cap_add_conf_opt(&ptr, L2CAP_CONF_EFS,
					sizeof(efs), (unsigned long) &efs);
			break;
		}
	}

	if (chan->mode == L2CAP_MODE_BASIC && chan->mode != rfc.mode)
		return -ECONNREFUSED;

	chan->mode = rfc.mode;

	if (*result == L2CAP_CONF_SUCCESS || *result == L2CAP_CONF_PENDING) {
		switch (rfc.mode) {
		case L2CAP_MODE_ERTM:
			chan->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
			chan->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
			chan->mps    = le16_to_cpu(rfc.max_pdu_size);

			if (test_bit(FLAG_EFS_ENABLE, &chan->flags)) {
				chan->local_msdu = le16_to_cpu(efs.msdu);
				chan->local_sdu_itime =
						le32_to_cpu(efs.sdu_itime);
				chan->local_acc_lat = le32_to_cpu(efs.acc_lat);
				chan->local_flush_to =
						le32_to_cpu(efs.flush_to);
			}
			break;

		case L2CAP_MODE_STREAMING:
			chan->mps    = le16_to_cpu(rfc.max_pdu_size);
		}
	}

	req->dcid   = cpu_to_le16(chan->dcid);
	req->flags  = cpu_to_le16(0x0000);

	return ptr - data;
}

static int l2cap_build_conf_rsp(struct l2cap_chan *chan, void *data, u16 result, u16 flags)
{
	struct l2cap_conf_rsp *rsp = data;
	void *ptr = rsp->data;

	BT_DBG("chan %p", chan);

	rsp->scid   = cpu_to_le16(chan->dcid);
	rsp->result = cpu_to_le16(result);
	rsp->flags  = cpu_to_le16(flags);

	return ptr - data;
}

void __l2cap_connect_rsp_defer(struct l2cap_chan *chan)
{
	struct l2cap_conn_rsp rsp;
	struct l2cap_conn *conn = chan->conn;
	u8 buf[128];

	rsp.scid   = cpu_to_le16(chan->dcid);
	rsp.dcid   = cpu_to_le16(chan->scid);
	rsp.result = cpu_to_le16(L2CAP_CR_SUCCESS);
	rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
	l2cap_send_cmd(conn, chan->ident,
				L2CAP_CONN_RSP, sizeof(rsp), &rsp);

	if (test_and_set_bit(CONF_REQ_SENT, &chan->conf_state))
		return;

	l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
			l2cap_build_conf_req(chan, buf), buf);
	chan->num_conf_req++;
}

static void l2cap_conf_rfc_get(struct l2cap_chan *chan, void *rsp, int len)
{
	int type, olen;
	unsigned long val;
	struct l2cap_conf_rfc rfc;

	BT_DBG("chan %p, rsp %p, len %d", chan, rsp, len);

	if ((chan->mode != L2CAP_MODE_ERTM) && (chan->mode != L2CAP_MODE_STREAMING))
		return;

	while (len >= L2CAP_CONF_OPT_SIZE) {
		len -= l2cap_get_conf_opt(&rsp, &type, &olen, &val);

		if (type != L2CAP_CONF_RFC)
			continue;

		if (olen != sizeof(rfc))
			break;

		memcpy(&rfc, (void *)val, olen);
		goto done;
	}

	/* Use sane default values in case a misbehaving remote device
	 * did not send an RFC option.
	 */
	rfc.mode = chan->mode;
	rfc.retrans_timeout = cpu_to_le16(L2CAP_DEFAULT_RETRANS_TO);
	rfc.monitor_timeout = cpu_to_le16(L2CAP_DEFAULT_MONITOR_TO);
	rfc.max_pdu_size = cpu_to_le16(chan->imtu);

	BT_ERR("Expected RFC option was not found, using defaults");

done:
	switch (rfc.mode) {
	case L2CAP_MODE_ERTM:
		chan->retrans_timeout = le16_to_cpu(rfc.retrans_timeout);
		chan->monitor_timeout = le16_to_cpu(rfc.monitor_timeout);
		chan->mps    = le16_to_cpu(rfc.max_pdu_size);
		break;
	case L2CAP_MODE_STREAMING:
		chan->mps    = le16_to_cpu(rfc.max_pdu_size);
	}
}

static inline int l2cap_command_rej(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_cmd_rej_unk *rej = (struct l2cap_cmd_rej_unk *) data;

	if (rej->reason != L2CAP_REJ_NOT_UNDERSTOOD)
		return 0;

	if ((conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_SENT) &&
					cmd->ident == conn->info_ident) {
		cancel_delayed_work(&conn->info_timer);

		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
		conn->info_ident = 0;

		l2cap_conn_start(conn);
>>>>>>> remotes/linux2/linux-3.4.y
	}

	return 0;
}

<<<<<<< HEAD
static inline int l2cap_information_rsp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) data;
	u16 type, result;

	type   = __le16_to_cpu(rsp->type);
	result = __le16_to_cpu(rsp->result);

	BT_DBG("type 0x%4.4x result 0x%2.2x", type, result);

	/* L2CAP Info req/rsp are unbound to channels, add extra checks */
	if (cmd->ident != conn->info_ident ||
			conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE)
		return 0;

	del_timer(&conn->info_timer);

	if (result != L2CAP_IR_SUCCESS) {
		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
		conn->info_ident = 0;

		l2cap_conn_start(conn);

		return 0;
	}

	if (type == L2CAP_IT_FEAT_MASK) {
		conn->feat_mask = get_unaligned_le32(rsp->data);

		if (conn->feat_mask & L2CAP_FEAT_FIXED_CHAN) {
			struct l2cap_info_req req;
			req.type = cpu_to_le16(L2CAP_IT_FIXED_CHAN);

			conn->info_ident = l2cap_get_ident(conn);

			l2cap_send_cmd(conn, conn->info_ident,
					L2CAP_INFO_REQ, sizeof(req), &req);
		} else {
			conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
			conn->info_ident = 0;

			l2cap_conn_start(conn);
		}
	} else if (type == L2CAP_IT_FIXED_CHAN) {
		conn->fc_mask = rsp->data[0];
		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
		conn->info_ident = 0;

		l2cap_conn_start(conn);
	}

	return 0;
}

static void l2cap_send_move_chan_req(struct l2cap_conn *conn,
			struct l2cap_pinfo *pi, u16 icid, u8 dest_amp_id)
{
	struct l2cap_move_chan_req req;
	u8 ident;

	BT_DBG("pi %p, icid %d, dest_amp_id %d", pi, (int) icid,
		(int) dest_amp_id);

	ident = l2cap_get_ident(conn);
	if (pi)
		pi->ident = ident;

	req.icid = cpu_to_le16(icid);
	req.dest_amp_id = dest_amp_id;

	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_REQ, sizeof(req), &req);
}

static void l2cap_send_move_chan_rsp(struct l2cap_conn *conn, u8 ident,
				u16 icid, u16 result)
{
	struct l2cap_move_chan_rsp rsp;

	BT_DBG("icid %d, result %d", (int) icid, (int) result);

	rsp.icid = cpu_to_le16(icid);
	rsp.result = cpu_to_le16(result);

	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_RSP, sizeof(rsp), &rsp);
}

static void l2cap_send_move_chan_cfm(struct l2cap_conn *conn,
				struct l2cap_pinfo *pi, u16 icid, u16 result)
{
	struct l2cap_move_chan_cfm cfm;
	u8 ident;

	BT_DBG("icid %d, result %d", (int) icid, (int) result);

	ident = l2cap_get_ident(conn);
	if (pi)
		pi->ident = ident;

	cfm.icid = cpu_to_le16(icid);
	cfm.result = cpu_to_le16(result);

	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_CFM, sizeof(cfm), &cfm);
}

static void l2cap_send_move_chan_cfm_rsp(struct l2cap_conn *conn, u8 ident,
					u16 icid)
{
	struct l2cap_move_chan_cfm_rsp rsp;

	BT_DBG("icid %d", (int) icid);

	rsp.icid = cpu_to_le16(icid);
	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_CFM_RSP, sizeof(rsp), &rsp);
}

static inline int l2cap_create_channel_req(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_create_chan_req *req =
		(struct l2cap_create_chan_req *) data;
	struct sock *sk;
	u16 psm, scid;

	psm = le16_to_cpu(req->psm);
	scid = le16_to_cpu(req->scid);

	BT_DBG("psm %d, scid %d, amp_id %d", (int) psm, (int) scid,
		(int) req->amp_id);

	if (req->amp_id) {
		struct hci_dev *hdev;

		/* Validate AMP controller id */
		hdev = hci_dev_get(req->amp_id);
		if (!hdev || !test_bit(HCI_UP, &hdev->flags)) {
			struct l2cap_create_chan_rsp rsp;

			rsp.dcid = 0;
			rsp.scid = cpu_to_le16(scid);
			rsp.result = L2CAP_CREATE_CHAN_REFUSED_CONTROLLER;
			rsp.status = L2CAP_CREATE_CHAN_STATUS_NONE;

			l2cap_send_cmd(conn, cmd->ident, L2CAP_CREATE_CHAN_RSP,
				       sizeof(rsp), &rsp);

			if (hdev)
				hci_dev_put(hdev);

			return 0;
		}

		hci_dev_put(hdev);
	}

	sk = l2cap_create_connect(conn, cmd, data, L2CAP_CREATE_CHAN_RSP,
					req->amp_id);

	if (sk)
		l2cap_pi(sk)->conf_state |= L2CAP_CONF_LOCKSTEP;

	if (sk && req->amp_id &&
			(conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE))
		amp_accept_physical(conn, req->amp_id, sk);

	return 0;
}

static inline int l2cap_create_channel_rsp(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	BT_DBG("conn %p", conn);

	return l2cap_connect_rsp(conn, cmd, data);
}

static inline int l2cap_move_channel_req(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_move_chan_req *req = (struct l2cap_move_chan_req *) data;
	struct sock *sk;
	struct l2cap_pinfo *pi;
	u16 icid = 0;
	u16 result = L2CAP_MOVE_CHAN_REFUSED_NOT_ALLOWED;

	icid = le16_to_cpu(req->icid);

	BT_DBG("icid %d, dest_amp_id %d", (int) icid, (int) req->dest_amp_id);

	read_lock(&conn->chan_list.lock);
	sk = __l2cap_get_chan_by_dcid(&conn->chan_list, icid);
	read_unlock(&conn->chan_list.lock);

	if (!sk)
		goto send_move_response;

	lock_sock(sk);
	pi = l2cap_pi(sk);

	if (pi->scid < L2CAP_CID_DYN_START ||
		(pi->mode != L2CAP_MODE_ERTM &&
		 pi->mode != L2CAP_MODE_STREAMING)) {
		goto send_move_response;
	}

	if (pi->amp_id == req->dest_amp_id) {
		result = L2CAP_MOVE_CHAN_REFUSED_SAME_ID;
		goto send_move_response;
	}

	if (req->dest_amp_id) {
		struct hci_dev *hdev;
		hdev = hci_dev_get(req->dest_amp_id);
		if (!hdev || !test_bit(HCI_UP, &hdev->flags)) {
			if (hdev)
				hci_dev_put(hdev);

			result = L2CAP_MOVE_CHAN_REFUSED_CONTROLLER;
			goto send_move_response;
		}
		hci_dev_put(hdev);
	}

	if (((pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
		pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE) ||
		pi->amp_move_role != L2CAP_AMP_MOVE_NONE) &&
		bacmp(conn->src, conn->dst) > 0) {
		result = L2CAP_MOVE_CHAN_REFUSED_COLLISION;
		goto send_move_response;
	}

	if (pi->amp_pref == BT_AMP_POLICY_REQUIRE_BR_EDR) {
		result = L2CAP_MOVE_CHAN_REFUSED_NOT_ALLOWED;
		goto send_move_response;
	}

	pi->amp_move_cmd_ident = cmd->ident;
	pi->amp_move_role = L2CAP_AMP_MOVE_RESPONDER;
	l2cap_amp_move_setup(sk);
	pi->amp_move_id = req->dest_amp_id;
	icid = pi->dcid;

	if (req->dest_amp_id == 0) {
		/* Moving to BR/EDR */
		if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
			pi->amp_move_state = L2CAP_AMP_STATE_WAIT_LOCAL_BUSY;
			result = L2CAP_MOVE_CHAN_PENDING;
		} else {
			pi->amp_move_state = L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM;
			result = L2CAP_MOVE_CHAN_SUCCESS;
		}
	} else {
		pi->amp_move_state = L2CAP_AMP_STATE_WAIT_PREPARE;
		amp_accept_physical(pi->conn, req->dest_amp_id, sk);
		result = L2CAP_MOVE_CHAN_PENDING;
	}

send_move_response:
	l2cap_send_move_chan_rsp(conn, cmd->ident, icid, result);

	if (sk)
		release_sock(sk);

	return 0;
}

static inline int l2cap_move_channel_rsp(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_move_chan_rsp *rsp = (struct l2cap_move_chan_rsp *) data;
	struct sock *sk;
	struct l2cap_pinfo *pi;
	u16 icid, result;

	icid = le16_to_cpu(rsp->icid);
	result = le16_to_cpu(rsp->result);

	BT_DBG("icid %d, result %d", (int) icid, (int) result);

	switch (result) {
	case L2CAP_MOVE_CHAN_SUCCESS:
	case L2CAP_MOVE_CHAN_PENDING:
		read_lock(&conn->chan_list.lock);
		sk = __l2cap_get_chan_by_scid(&conn->chan_list, icid);
		read_unlock(&conn->chan_list.lock);

		if (!sk) {
			l2cap_send_move_chan_cfm(conn, NULL, icid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
			break;
		}

		lock_sock(sk);
		pi = l2cap_pi(sk);

		l2cap_sock_clear_timer(sk);
		if (result == L2CAP_MOVE_CHAN_PENDING)
			l2cap_sock_set_timer(sk, L2CAP_MOVE_ERTX_TIMEOUT);

		if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_LOGICAL_COMPLETE) {
			/* Move confirm will be sent when logical link
			 * is complete.
			 */
			pi->amp_move_state =
				L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM;
		} else if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS) {
			if (result == L2CAP_MOVE_CHAN_PENDING) {
				break;
			} else if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOCAL_BUSY;
			} else {
				/* Logical link is up or moving to BR/EDR,
				 * proceed with move */
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP;
				l2cap_send_move_chan_cfm(conn, pi, pi->scid,
						L2CAP_MOVE_CHAN_CONFIRMED);
				l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
			}
		} else if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_MOVE_RSP) {
			struct l2cap_conf_ext_fs default_fs = {1, 1, 0xFFFF,
					0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
			struct hci_chan *chan;
			/* Moving to AMP */
			if (result == L2CAP_MOVE_CHAN_SUCCESS) {
				/* Remote is ready, send confirm immediately
				 * after logical link is ready
				 */
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM;
			} else {
				/* Both logical link and move success
				 * are required to confirm
				 */
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOGICAL_COMPLETE;
			}
			pi->remote_fs = default_fs;
			pi->local_fs = default_fs;
			chan = l2cap_chan_admit(pi->amp_move_id, sk);
			if (!chan) {
				/* Logical link not available */
				l2cap_send_move_chan_cfm(conn, pi, pi->scid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
				break;
			}

			if (chan->state == BT_CONNECTED) {
				/* Logical link is already ready to go */
				pi->ampcon = chan->conn;
				pi->ampcon->l2cap_data = pi->conn;
				if (result == L2CAP_MOVE_CHAN_SUCCESS) {
					/* Can confirm now */
					l2cap_send_move_chan_cfm(conn, pi,
						pi->scid,
						L2CAP_MOVE_CHAN_CONFIRMED);
				} else {
					/* Now only need move success
					 * required to confirm
					 */
					pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS;
				}

				l2cap_create_cfm(chan, 0);
			}
		} else {
			/* Any other amp move state means the move failed. */
			pi->amp_move_id = pi->amp_id;
			pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
			l2cap_amp_move_revert(sk);
			pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
			l2cap_send_move_chan_cfm(conn, pi, pi->scid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
			l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
		}
		break;
	default:
		/* Failed (including collision case) */
		read_lock(&conn->chan_list.lock);
		sk = __l2cap_get_chan_by_ident(&conn->chan_list, cmd->ident);
		read_unlock(&conn->chan_list.lock);

		if (!sk) {
			/* Could not locate channel, icid is best guess */
			l2cap_send_move_chan_cfm(conn, NULL, icid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
			break;
		}

		lock_sock(sk);
		pi = l2cap_pi(sk);

		l2cap_sock_clear_timer(sk);

		if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
			if (result == L2CAP_MOVE_CHAN_REFUSED_COLLISION)
				pi->amp_move_role = L2CAP_AMP_MOVE_RESPONDER;
			else {
				/* Cleanup - cancel move */
				pi->amp_move_id = pi->amp_id;
				pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
				l2cap_amp_move_revert(sk);
				pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
			}
		}

		l2cap_send_move_chan_cfm(conn, pi, pi->scid,
					L2CAP_MOVE_CHAN_UNCONFIRMED);
		l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
		break;
	}

	if (sk)
		release_sock(sk);

	return 0;
}

static inline int l2cap_move_channel_confirm(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_move_chan_cfm *cfm = (struct l2cap_move_chan_cfm *) data;
	struct sock *sk;
	struct l2cap_pinfo *pi;
	u16 icid, result;

	icid = le16_to_cpu(cfm->icid);
	result = le16_to_cpu(cfm->result);

	BT_DBG("icid %d, result %d", (int) icid, (int) result);

	read_lock(&conn->chan_list.lock);
	sk = __l2cap_get_chan_by_dcid(&conn->chan_list, icid);
	read_unlock(&conn->chan_list.lock);

	if (!sk) {
		BT_DBG("Bad channel (%d)", (int) icid);
		goto send_move_confirm_response;
	}

	lock_sock(sk);
	pi = l2cap_pi(sk);

	if (pi->amp_move_state == L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM) {
		pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
		if (result == L2CAP_MOVE_CHAN_CONFIRMED) {
			pi->amp_id = pi->amp_move_id;
			if (!pi->amp_id && pi->ampchan) {
				struct hci_chan *ampchan = pi->ampchan;
				struct hci_conn *ampcon = pi->ampcon;
				/* Have moved off of AMP, free the channel */
				pi->ampchan = NULL;
				pi->ampcon = NULL;
				if (hci_chan_put(ampchan))
					ampcon->l2cap_data = NULL;
				else
					l2cap_deaggregate(ampchan, pi);
			}
			l2cap_amp_move_success(sk);
		} else {
			pi->amp_move_id = pi->amp_id;
			l2cap_amp_move_revert(sk);
		}
		pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
	} else if (pi->amp_move_state ==
			L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM) {
		BT_DBG("Bad AMP_MOVE_STATE (%d)", pi->amp_move_state);
	}

send_move_confirm_response:
	l2cap_send_move_chan_cfm_rsp(conn, cmd->ident, icid);

	if (sk)
		release_sock(sk);
=======
static inline int l2cap_connect_req(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_conn_req *req = (struct l2cap_conn_req *) data;
	struct l2cap_conn_rsp rsp;
	struct l2cap_chan *chan = NULL, *pchan;
	struct sock *parent, *sk = NULL;
	int result, status = L2CAP_CS_NO_INFO;

	u16 dcid = 0, scid = __le16_to_cpu(req->scid);
	__le16 psm = req->psm;

	BT_DBG("psm 0x%2.2x scid 0x%4.4x", psm, scid);

	/* Check if we have socket listening on psm */
	pchan = l2cap_global_chan_by_psm(BT_LISTEN, psm, conn->src);
	if (!pchan) {
		result = L2CAP_CR_BAD_PSM;
		goto sendresp;
	}

	parent = pchan->sk;

	mutex_lock(&conn->chan_lock);
	lock_sock(parent);

	/* Check if the ACL is secure enough (if not SDP) */
	if (psm != cpu_to_le16(0x0001) &&
				!hci_conn_check_link_mode(conn->hcon)) {
		conn->disc_reason = HCI_ERROR_AUTH_FAILURE;
		result = L2CAP_CR_SEC_BLOCK;
		goto response;
	}

	result = L2CAP_CR_NO_MEM;

	/* Check for backlog size */
	if (sk_acceptq_is_full(parent)) {
		BT_DBG("backlog full %d", parent->sk_ack_backlog);
		goto response;
	}

	chan = pchan->ops->new_connection(pchan->data);
	if (!chan)
		goto response;

	sk = chan->sk;

	/* Check if we already have channel with that dcid */
	if (__l2cap_get_chan_by_dcid(conn, scid)) {
		sock_set_flag(sk, SOCK_ZAPPED);
		chan->ops->close(chan->data);
		goto response;
	}

	hci_conn_hold(conn->hcon);

	bacpy(&bt_sk(sk)->src, conn->src);
	bacpy(&bt_sk(sk)->dst, conn->dst);
	chan->psm  = psm;
	chan->dcid = scid;

	bt_accept_enqueue(parent, sk);

	__l2cap_chan_add(conn, chan);

	dcid = chan->scid;

	__set_chan_timer(chan, sk->sk_sndtimeo);

	chan->ident = cmd->ident;

	if (conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE) {
		if (l2cap_chan_check_security(chan)) {
			if (bt_sk(sk)->defer_setup) {
				__l2cap_state_change(chan, BT_CONNECT2);
				result = L2CAP_CR_PEND;
				status = L2CAP_CS_AUTHOR_PEND;
				parent->sk_data_ready(parent, 0);
			} else {
				__l2cap_state_change(chan, BT_CONFIG);
				result = L2CAP_CR_SUCCESS;
				status = L2CAP_CS_NO_INFO;
			}
		} else {
			__l2cap_state_change(chan, BT_CONNECT2);
			result = L2CAP_CR_PEND;
			status = L2CAP_CS_AUTHEN_PEND;
		}
	} else {
		__l2cap_state_change(chan, BT_CONNECT2);
		result = L2CAP_CR_PEND;
		status = L2CAP_CS_NO_INFO;
	}

response:
	release_sock(parent);
	mutex_unlock(&conn->chan_lock);

sendresp:
	rsp.scid   = cpu_to_le16(scid);
	rsp.dcid   = cpu_to_le16(dcid);
	rsp.result = cpu_to_le16(result);
	rsp.status = cpu_to_le16(status);
	l2cap_send_cmd(conn, cmd->ident, L2CAP_CONN_RSP, sizeof(rsp), &rsp);

	if (result == L2CAP_CR_PEND && status == L2CAP_CS_NO_INFO) {
		struct l2cap_info_req info;
		info.type = cpu_to_le16(L2CAP_IT_FEAT_MASK);

		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_SENT;
		conn->info_ident = l2cap_get_ident(conn);

		schedule_delayed_work(&conn->info_timer, L2CAP_INFO_TIMEOUT);

		l2cap_send_cmd(conn, conn->info_ident,
					L2CAP_INFO_REQ, sizeof(info), &info);
	}

	if (chan && !test_bit(CONF_REQ_SENT, &chan->conf_state) &&
				result == L2CAP_CR_SUCCESS) {
		u8 buf[128];
		set_bit(CONF_REQ_SENT, &chan->conf_state);
		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
					l2cap_build_conf_req(chan, buf), buf);
		chan->num_conf_req++;
	}

	return 0;
}

static inline int l2cap_connect_rsp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_conn_rsp *rsp = (struct l2cap_conn_rsp *) data;
	u16 scid, dcid, result, status;
	struct l2cap_chan *chan;
	u8 req[128];
	int err;

	scid   = __le16_to_cpu(rsp->scid);
	dcid   = __le16_to_cpu(rsp->dcid);
	result = __le16_to_cpu(rsp->result);
	status = __le16_to_cpu(rsp->status);

	BT_DBG("dcid 0x%4.4x scid 0x%4.4x result 0x%2.2x status 0x%2.2x",
						dcid, scid, result, status);

	mutex_lock(&conn->chan_lock);

	if (scid) {
		chan = __l2cap_get_chan_by_scid(conn, scid);
		if (!chan) {
			err = -EFAULT;
			goto unlock;
		}
	} else {
		chan = __l2cap_get_chan_by_ident(conn, cmd->ident);
		if (!chan) {
			err = -EFAULT;
			goto unlock;
		}
	}

	err = 0;

	l2cap_chan_lock(chan);

	switch (result) {
	case L2CAP_CR_SUCCESS:
		l2cap_state_change(chan, BT_CONFIG);
		chan->ident = 0;
		chan->dcid = dcid;
		clear_bit(CONF_CONNECT_PEND, &chan->conf_state);

		if (test_and_set_bit(CONF_REQ_SENT, &chan->conf_state))
			break;

		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
					l2cap_build_conf_req(chan, req), req);
		chan->num_conf_req++;
		break;

	case L2CAP_CR_PEND:
		set_bit(CONF_CONNECT_PEND, &chan->conf_state);
		break;

	default:
		l2cap_chan_del(chan, ECONNREFUSED);
		break;
	}

	l2cap_chan_unlock(chan);

unlock:
	mutex_unlock(&conn->chan_lock);

	return err;
}

static inline void set_default_fcs(struct l2cap_chan *chan)
{
	/* FCS is enabled only in ERTM or streaming mode, if one or both
	 * sides request it.
	 */
	if (chan->mode != L2CAP_MODE_ERTM && chan->mode != L2CAP_MODE_STREAMING)
		chan->fcs = L2CAP_FCS_NONE;
	else if (!test_bit(CONF_NO_FCS_RECV, &chan->conf_state))
		chan->fcs = L2CAP_FCS_CRC16;
}

static inline int l2cap_config_req(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u16 cmd_len, u8 *data)
{
	struct l2cap_conf_req *req = (struct l2cap_conf_req *) data;
	u16 dcid, flags;
	u8 rsp[64];
	struct l2cap_chan *chan;
	int len;

	dcid  = __le16_to_cpu(req->dcid);
	flags = __le16_to_cpu(req->flags);

	BT_DBG("dcid 0x%4.4x flags 0x%2.2x", dcid, flags);

	chan = l2cap_get_chan_by_scid(conn, dcid);
	if (!chan)
		return -ENOENT;

	l2cap_chan_lock(chan);

	if (chan->state != BT_CONFIG && chan->state != BT_CONNECT2) {
		struct l2cap_cmd_rej_cid rej;

		rej.reason = cpu_to_le16(L2CAP_REJ_INVALID_CID);
		rej.scid = cpu_to_le16(chan->scid);
		rej.dcid = cpu_to_le16(chan->dcid);

		l2cap_send_cmd(conn, cmd->ident, L2CAP_COMMAND_REJ,
				sizeof(rej), &rej);
		goto unlock;
	}

	/* Reject if config buffer is too small. */
	len = cmd_len - sizeof(*req);
	if (len < 0 || chan->conf_len + len > sizeof(chan->conf_req)) {
		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
				l2cap_build_conf_rsp(chan, rsp,
					L2CAP_CONF_REJECT, flags), rsp);
		goto unlock;
	}

	/* Store config. */
	memcpy(chan->conf_req + chan->conf_len, req->data, len);
	chan->conf_len += len;

	if (flags & 0x0001) {
		/* Incomplete config. Send empty response. */
		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
				l2cap_build_conf_rsp(chan, rsp,
					L2CAP_CONF_SUCCESS, 0x0001), rsp);
		goto unlock;
	}

	/* Complete config. */
	len = l2cap_parse_conf_req(chan, rsp);
	if (len < 0) {
		l2cap_send_disconn_req(conn, chan, ECONNRESET);
		goto unlock;
	}

	l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP, len, rsp);
	chan->num_conf_rsp++;

	/* Reset config buffer. */
	chan->conf_len = 0;

	if (!test_bit(CONF_OUTPUT_DONE, &chan->conf_state))
		goto unlock;

	if (test_bit(CONF_INPUT_DONE, &chan->conf_state)) {
		set_default_fcs(chan);

		l2cap_state_change(chan, BT_CONNECTED);

		chan->next_tx_seq = 0;
		chan->expected_tx_seq = 0;
		skb_queue_head_init(&chan->tx_q);
		if (chan->mode == L2CAP_MODE_ERTM)
			l2cap_ertm_init(chan);

		l2cap_chan_ready(chan);
		goto unlock;
	}

	if (!test_and_set_bit(CONF_REQ_SENT, &chan->conf_state)) {
		u8 buf[64];
		l2cap_send_cmd(conn, l2cap_get_ident(conn), L2CAP_CONF_REQ,
					l2cap_build_conf_req(chan, buf), buf);
		chan->num_conf_req++;
	}

	/* Got Conf Rsp PENDING from remote side and asume we sent
	   Conf Rsp PENDING in the code above */
	if (test_bit(CONF_REM_CONF_PEND, &chan->conf_state) &&
			test_bit(CONF_LOC_CONF_PEND, &chan->conf_state)) {

		/* check compatibility */

		clear_bit(CONF_LOC_CONF_PEND, &chan->conf_state);
		set_bit(CONF_OUTPUT_DONE, &chan->conf_state);

		l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
					l2cap_build_conf_rsp(chan, rsp,
					L2CAP_CONF_SUCCESS, 0x0000), rsp);
	}

unlock:
	l2cap_chan_unlock(chan);
	return 0;
}

static inline int l2cap_config_rsp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_conf_rsp *rsp = (struct l2cap_conf_rsp *)data;
	u16 scid, flags, result;
	struct l2cap_chan *chan;
	int len = cmd->len - sizeof(*rsp);

	scid   = __le16_to_cpu(rsp->scid);
	flags  = __le16_to_cpu(rsp->flags);
	result = __le16_to_cpu(rsp->result);

	BT_DBG("scid 0x%4.4x flags 0x%2.2x result 0x%2.2x",
			scid, flags, result);

	chan = l2cap_get_chan_by_scid(conn, scid);
	if (!chan)
		return 0;

	l2cap_chan_lock(chan);

	switch (result) {
	case L2CAP_CONF_SUCCESS:
		l2cap_conf_rfc_get(chan, rsp->data, len);
		clear_bit(CONF_REM_CONF_PEND, &chan->conf_state);
		break;

	case L2CAP_CONF_PENDING:
		set_bit(CONF_REM_CONF_PEND, &chan->conf_state);

		if (test_bit(CONF_LOC_CONF_PEND, &chan->conf_state)) {
			char buf[64];

			len = l2cap_parse_conf_rsp(chan, rsp->data, len,
								buf, &result);
			if (len < 0) {
				l2cap_send_disconn_req(conn, chan, ECONNRESET);
				goto done;
			}

			/* check compatibility */

			clear_bit(CONF_LOC_CONF_PEND, &chan->conf_state);
			set_bit(CONF_OUTPUT_DONE, &chan->conf_state);

			l2cap_send_cmd(conn, cmd->ident, L2CAP_CONF_RSP,
						l2cap_build_conf_rsp(chan, buf,
						L2CAP_CONF_SUCCESS, 0x0000), buf);
		}
		goto done;

	case L2CAP_CONF_UNACCEPT:
		if (chan->num_conf_rsp <= L2CAP_CONF_MAX_CONF_RSP) {
			char req[64];

			if (len > sizeof(req) - sizeof(struct l2cap_conf_req)) {
				l2cap_send_disconn_req(conn, chan, ECONNRESET);
				goto done;
			}

			/* throw out any old stored conf requests */
			result = L2CAP_CONF_SUCCESS;
			len = l2cap_parse_conf_rsp(chan, rsp->data, len,
								req, &result);
			if (len < 0) {
				l2cap_send_disconn_req(conn, chan, ECONNRESET);
				goto done;
			}

			l2cap_send_cmd(conn, l2cap_get_ident(conn),
						L2CAP_CONF_REQ, len, req);
			chan->num_conf_req++;
			if (result != L2CAP_CONF_SUCCESS)
				goto done;
			break;
		}

	default:
		l2cap_chan_set_err(chan, ECONNRESET);

		__set_chan_timer(chan, L2CAP_DISC_REJ_TIMEOUT);
		l2cap_send_disconn_req(conn, chan, ECONNRESET);
		goto done;
	}

	if (flags & 0x01)
		goto done;

	set_bit(CONF_INPUT_DONE, &chan->conf_state);

	if (test_bit(CONF_OUTPUT_DONE, &chan->conf_state)) {
		set_default_fcs(chan);

		l2cap_state_change(chan, BT_CONNECTED);
		chan->next_tx_seq = 0;
		chan->expected_tx_seq = 0;
		skb_queue_head_init(&chan->tx_q);
		if (chan->mode ==  L2CAP_MODE_ERTM)
			l2cap_ertm_init(chan);

		l2cap_chan_ready(chan);
	}

done:
	l2cap_chan_unlock(chan);
	return 0;
}

static inline int l2cap_disconnect_req(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_disconn_req *req = (struct l2cap_disconn_req *) data;
	struct l2cap_disconn_rsp rsp;
	u16 dcid, scid;
	struct l2cap_chan *chan;
	struct sock *sk;

	scid = __le16_to_cpu(req->scid);
	dcid = __le16_to_cpu(req->dcid);

	BT_DBG("scid 0x%4.4x dcid 0x%4.4x", scid, dcid);

	mutex_lock(&conn->chan_lock);

	chan = __l2cap_get_chan_by_scid(conn, dcid);
	if (!chan) {
		mutex_unlock(&conn->chan_lock);
		return 0;
	}

	l2cap_chan_lock(chan);

	sk = chan->sk;

	rsp.dcid = cpu_to_le16(chan->scid);
	rsp.scid = cpu_to_le16(chan->dcid);
	l2cap_send_cmd(conn, cmd->ident, L2CAP_DISCONN_RSP, sizeof(rsp), &rsp);

	lock_sock(sk);
	sk->sk_shutdown = SHUTDOWN_MASK;
	release_sock(sk);

	l2cap_chan_del(chan, ECONNRESET);

	l2cap_chan_unlock(chan);

	chan->ops->close(chan->data);

	mutex_unlock(&conn->chan_lock);
>>>>>>> remotes/linux2/linux-3.4.y

	return 0;
}

<<<<<<< HEAD
static inline int l2cap_move_channel_confirm_rsp(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_move_chan_cfm_rsp *rsp =
		(struct l2cap_move_chan_cfm_rsp *) data;
	struct sock *sk;
	struct l2cap_pinfo *pi;

	u16 icid;

	icid = le16_to_cpu(rsp->icid);

	BT_DBG("icid %d", (int) icid);

	read_lock(&conn->chan_list.lock);
	sk = __l2cap_get_chan_by_scid(&conn->chan_list, icid);
	read_unlock(&conn->chan_list.lock);

	if (!sk)
		return 0;

	lock_sock(sk);
	pi = l2cap_pi(sk);

	l2cap_sock_clear_timer(sk);

	if (pi->amp_move_state ==
			L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP) {
		pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
		pi->amp_id = pi->amp_move_id;

		if (!pi->amp_id && pi->ampchan) {
			struct hci_chan *ampchan = pi->ampchan;
			struct hci_conn *ampcon = pi->ampcon;
			/* Have moved off of AMP, free the channel */
			pi->ampchan = NULL;
			pi->ampcon = NULL;
			if (hci_chan_put(ampchan))
				ampcon->l2cap_data = NULL;
			else
				l2cap_deaggregate(ampchan, pi);
		}

		l2cap_amp_move_success(sk);

		pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
	}

	release_sock(sk);
=======
static inline int l2cap_disconnect_rsp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_disconn_rsp *rsp = (struct l2cap_disconn_rsp *) data;
	u16 dcid, scid;
	struct l2cap_chan *chan;

	scid = __le16_to_cpu(rsp->scid);
	dcid = __le16_to_cpu(rsp->dcid);

	BT_DBG("dcid 0x%4.4x scid 0x%4.4x", dcid, scid);

	mutex_lock(&conn->chan_lock);

	chan = __l2cap_get_chan_by_scid(conn, scid);
	if (!chan) {
		mutex_unlock(&conn->chan_lock);
		return 0;
	}

	l2cap_chan_lock(chan);

	l2cap_chan_del(chan, 0);

	l2cap_chan_unlock(chan);

	chan->ops->close(chan->data);

	mutex_unlock(&conn->chan_lock);
>>>>>>> remotes/linux2/linux-3.4.y

	return 0;
}

<<<<<<< HEAD
static void l2cap_amp_signal_worker(struct work_struct *work)
{
	int err = 0;
	struct l2cap_amp_signal_work *ampwork =
		container_of(work, struct l2cap_amp_signal_work, work);

	switch (ampwork->cmd.code) {
	case L2CAP_MOVE_CHAN_REQ:
		err = l2cap_move_channel_req(ampwork->conn, &ampwork->cmd,
						ampwork->data);
		break;

	case L2CAP_MOVE_CHAN_RSP:
		err = l2cap_move_channel_rsp(ampwork->conn, &ampwork->cmd,
						ampwork->data);
		break;

	case L2CAP_MOVE_CHAN_CFM:
		err = l2cap_move_channel_confirm(ampwork->conn, &ampwork->cmd,
						ampwork->data);
		break;

	case L2CAP_MOVE_CHAN_CFM_RSP:
		err = l2cap_move_channel_confirm_rsp(ampwork->conn,
						&ampwork->cmd, ampwork->data);
		break;

	default:
		BT_ERR("Unknown signaling command 0x%2.2x", ampwork->cmd.code);
		err = -EINVAL;
		break;
	}

	if (err) {
		struct l2cap_cmd_rej rej;
		BT_DBG("error %d", err);

		/* In this context, commands are only rejected with
		 * "command not understood", code 0.
		 */
		rej.reason = cpu_to_le16(0);
		l2cap_send_cmd(ampwork->conn, ampwork->cmd.ident,
				L2CAP_COMMAND_REJ, sizeof(rej), &rej);
	}

	kfree_skb(ampwork->skb);
	kfree(ampwork);
}

void l2cap_amp_physical_complete(int result, u8 local_id, u8 remote_id,
				struct sock *sk)
{
	struct l2cap_pinfo *pi;

	BT_DBG("result %d, local_id %d, remote_id %d, sk %p", result,
		(int) local_id, (int) remote_id, sk);

	lock_sock(sk);

	if (sk->sk_state == BT_DISCONN || sk->sk_state == BT_CLOSED) {
		release_sock(sk);
		return;
	}

	pi = l2cap_pi(sk);

	if (sk->sk_state != BT_CONNECTED) {
		if (bt_sk(sk)->parent) {
			struct l2cap_conn_rsp rsp;
			char buf[128];
			rsp.scid = cpu_to_le16(l2cap_pi(sk)->dcid);
			rsp.dcid = cpu_to_le16(l2cap_pi(sk)->scid);

			/* Incoming channel on AMP */
			if (result == L2CAP_CREATE_CHAN_SUCCESS) {
				/* Send successful response */
				rsp.result = cpu_to_le16(L2CAP_CR_SUCCESS);
				rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
			} else {
				/* Send negative response */
				rsp.result = cpu_to_le16(L2CAP_CR_NO_MEM);
				rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
			}

			l2cap_send_cmd(pi->conn, pi->ident,
					L2CAP_CREATE_CHAN_RSP,
					sizeof(rsp), &rsp);

			if (result == L2CAP_CREATE_CHAN_SUCCESS) {
				sk->sk_state = BT_CONFIG;
				pi->conf_state |= L2CAP_CONF_REQ_SENT;
				l2cap_send_cmd(pi->conn,
					l2cap_get_ident(pi->conn),
					L2CAP_CONF_REQ,
					l2cap_build_conf_req(sk, buf), buf);
				l2cap_pi(sk)->num_conf_req++;
			}
		} else {
			/* Outgoing channel on AMP */
			if (result != L2CAP_CREATE_CHAN_SUCCESS) {
				/* Revert to BR/EDR connect */
				l2cap_send_conn_req(sk);
			} else {
				pi->amp_id = local_id;
				l2cap_send_create_chan_req(sk, remote_id);
			}
		}
	} else if (result == L2CAP_MOVE_CHAN_SUCCESS &&
		pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
		l2cap_amp_move_setup(sk);
		pi->amp_move_id = local_id;
		pi->amp_move_state = L2CAP_AMP_STATE_WAIT_MOVE_RSP;

		l2cap_send_move_chan_req(pi->conn, pi, pi->scid, remote_id);
		l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
	} else if (result == L2CAP_MOVE_CHAN_SUCCESS &&
		pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER) {
		struct hci_chan *chan;
		struct l2cap_conf_ext_fs default_fs = {1, 1, 0xFFFF,
				0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
		pi->remote_fs = default_fs;
		pi->local_fs = default_fs;
		chan = l2cap_chan_admit(local_id, sk);
		if (chan) {
			if (chan->state == BT_CONNECTED) {
				/* Logical link is ready to go */
				pi->ampcon = chan->conn;
				pi->ampcon->l2cap_data = pi->conn;
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM;
				l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_SUCCESS);

				l2cap_create_cfm(chan, 0);
			} else {
				/* Wait for logical link to be ready */
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM;
			}
		} else {
			/* Logical link not available */
			l2cap_send_move_chan_rsp(pi->conn,
				pi->amp_move_cmd_ident, pi->dcid,
				L2CAP_MOVE_CHAN_REFUSED_NOT_ALLOWED);
		}
	} else {
		BT_DBG("result %d, role %d, local_busy %d", result,
			(int) pi->amp_move_role,
			(int) ((pi->conn_state & L2CAP_CONN_LOCAL_BUSY) != 0));

		if (pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER) {
			if (result == -EINVAL)
				l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_REFUSED_CONTROLLER);
			else
				l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_REFUSED_NOT_ALLOWED);
		}

		pi->amp_move_role = L2CAP_AMP_MOVE_NONE;
		pi->amp_move_state = L2CAP_AMP_STATE_STABLE;

		if ((l2cap_pi(sk)->conn_state & L2CAP_CONN_LOCAL_BUSY) &&
			l2cap_rmem_available(sk))
			l2cap_ertm_tx(sk, 0, 0,
					L2CAP_ERTM_EVENT_LOCAL_BUSY_CLEAR);

		/* Restart data transmission */
		l2cap_ertm_send(sk);
	}

	release_sock(sk);
}

static void l2cap_logical_link_complete(struct hci_chan *chan, u8 status)
{
	struct l2cap_pinfo *pi;
	struct sock *sk;
	struct hci_chan *ampchan;
	struct hci_conn *ampcon;

	BT_DBG("status %d, chan %p, conn %p", (int) status, chan, chan->conn);

	sk = chan->l2cap_sk;
	chan->l2cap_sk = NULL;

	BT_DBG("sk %p", sk);

	lock_sock(sk);

	if (sk->sk_state != BT_CONNECTED && !l2cap_pi(sk)->amp_id) {
		release_sock(sk);
		return;
	}

	pi = l2cap_pi(sk);

	if ((!status) && (chan != NULL)) {
		pi->ampcon = chan->conn;
		pi->ampcon->l2cap_data = pi->conn;

		BT_DBG("amp_move_state %d", pi->amp_move_state);

		if (sk->sk_state != BT_CONNECTED) {
			struct l2cap_conf_rsp rsp;

			/* Must use spinlock to prevent concurrent
			 * execution of l2cap_config_rsp()
			 */
			bh_lock_sock(sk);
			l2cap_send_cmd(pi->conn, pi->conf_ident, L2CAP_CONF_RSP,
					l2cap_build_conf_rsp(sk, &rsp,
						L2CAP_CONF_SUCCESS, 0), &rsp);
			pi->conf_state |= L2CAP_CONF_OUTPUT_DONE;

			if (l2cap_pi(sk)->conf_state & L2CAP_CONF_INPUT_DONE) {
				set_default_fcs(l2cap_pi(sk));

				sk->sk_state = BT_CONNECTED;

				if (l2cap_pi(sk)->mode == L2CAP_MODE_ERTM ||
				    l2cap_pi(sk)->mode == L2CAP_MODE_STREAMING)
					l2cap_ertm_init(sk);

				l2cap_chan_ready(sk);
			}
			bh_unlock_sock(sk);
		} else if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_LOGICAL_COMPLETE) {
			/* Move confirm will be sent after a success
			 * response is received
			 */
			pi->amp_move_state =
				L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS;
		} else if (pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM) {
			if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY)
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_LOCAL_BUSY;
			else if (pi->amp_move_role ==
					L2CAP_AMP_MOVE_INITIATOR) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP;
				l2cap_send_move_chan_cfm(pi->conn, pi, pi->scid,
					L2CAP_MOVE_CHAN_SUCCESS);
				l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
			} else if (pi->amp_move_role ==
					L2CAP_AMP_MOVE_RESPONDER) {
				pi->amp_move_state =
					L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM;
				l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_SUCCESS);
			}
		} else if ((pi->amp_move_state !=
				L2CAP_AMP_STATE_WAIT_MOVE_RSP_SUCCESS) &&
			(pi->amp_move_state !=
				L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM) &&
			(pi->amp_move_state !=
				L2CAP_AMP_STATE_WAIT_MOVE_CONFIRM_RSP)) {
			/* Move was not in expected state, free the channel */
			ampchan = pi->ampchan;
			ampcon = pi->ampcon;
			pi->ampchan = NULL;
			pi->ampcon = NULL;
			if (ampchan) {
				if (hci_chan_put(ampchan))
					ampcon->l2cap_data = NULL;
				else
					l2cap_deaggregate(ampchan, pi);
			}
			pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
		}
	} else {
		/* Logical link setup failed. */

		if (sk->sk_state != BT_CONNECTED)
			l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		else if (pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER) {
			l2cap_amp_move_revert(sk);
			l2cap_pi(sk)->amp_move_role = L2CAP_AMP_MOVE_NONE;
			pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
			l2cap_send_move_chan_rsp(pi->conn,
					pi->amp_move_cmd_ident, pi->dcid,
					L2CAP_MOVE_CHAN_REFUSED_CONFIG);
		} else if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
			if ((pi->amp_move_state ==
				L2CAP_AMP_STATE_WAIT_LOGICAL_COMPLETE) ||
				(pi->amp_move_state ==
				    L2CAP_AMP_STATE_WAIT_LOGICAL_CONFIRM)) {
				/* Remote has only sent pending or
				 * success responses, clean up
				 */
				l2cap_amp_move_revert(sk);
				l2cap_pi(sk)->amp_move_role =
					L2CAP_AMP_MOVE_NONE;
				pi->amp_move_state = L2CAP_AMP_STATE_STABLE;
			}

			/* Other amp move states imply that the move
			 * has already aborted
			 */
			l2cap_send_move_chan_cfm(pi->conn, pi, pi->scid,
						L2CAP_MOVE_CHAN_UNCONFIRMED);
			l2cap_sock_set_timer(sk, L2CAP_MOVE_TIMEOUT);
		}
		ampchan = pi->ampchan;
		ampcon = pi->ampcon;
		pi->ampchan = NULL;
		pi->ampcon = NULL;
		if (ampchan) {
			if (hci_chan_put(ampchan))
				ampcon->l2cap_data = NULL;
			else
				l2cap_deaggregate(ampchan, pi);
		}
	}

	release_sock(sk);
}

static void l2cap_logical_link_worker(struct work_struct *work)
{
	struct l2cap_logical_link_work *log_link_work =
		container_of(work, struct l2cap_logical_link_work, work);
	struct sock *sk = log_link_work->chan->l2cap_sk;

	if (sk) {
		l2cap_logical_link_complete(log_link_work->chan,
							log_link_work->status);
		sock_put(sk);
	}
	hci_chan_put(log_link_work->chan);
	kfree(log_link_work);
}

static int l2cap_create_cfm(struct hci_chan *chan, u8 status)
{
	struct l2cap_logical_link_work *amp_work;

	if (!chan->l2cap_sk) {
		BT_ERR("Expected l2cap_sk to point to connecting socket");
		return -EFAULT;
	}

	amp_work = kzalloc(sizeof(*amp_work), GFP_ATOMIC);
	if (!amp_work) {
		sock_put(chan->l2cap_sk);
		return -ENOMEM;
	}

	INIT_WORK(&amp_work->work, l2cap_logical_link_worker);
	amp_work->chan = chan;
	amp_work->status = status;

	hci_chan_hold(chan);

	if (!queue_work(_l2cap_wq, &amp_work->work)) {
		kfree(amp_work);
		sock_put(chan->l2cap_sk);
		hci_chan_put(chan);
		return -ENOMEM;
	}
=======
static inline int l2cap_information_req(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_info_req *req = (struct l2cap_info_req *) data;
	u16 type;

	type = __le16_to_cpu(req->type);

	BT_DBG("type 0x%4.4x", type);

	if (type == L2CAP_IT_FEAT_MASK) {
		u8 buf[8];
		u32 feat_mask = l2cap_feat_mask;
		struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) buf;
		rsp->type   = cpu_to_le16(L2CAP_IT_FEAT_MASK);
		rsp->result = cpu_to_le16(L2CAP_IR_SUCCESS);
		if (!disable_ertm)
			feat_mask |= L2CAP_FEAT_ERTM | L2CAP_FEAT_STREAMING
							 | L2CAP_FEAT_FCS;
		if (enable_hs)
			feat_mask |= L2CAP_FEAT_EXT_FLOW
						| L2CAP_FEAT_EXT_WINDOW;

		put_unaligned_le32(feat_mask, rsp->data);
		l2cap_send_cmd(conn, cmd->ident,
					L2CAP_INFO_RSP, sizeof(buf), buf);
	} else if (type == L2CAP_IT_FIXED_CHAN) {
		u8 buf[12];
		struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) buf;

		if (enable_hs)
			l2cap_fixed_chan[0] |= L2CAP_FC_A2MP;
		else
			l2cap_fixed_chan[0] &= ~L2CAP_FC_A2MP;

		rsp->type   = cpu_to_le16(L2CAP_IT_FIXED_CHAN);
		rsp->result = cpu_to_le16(L2CAP_IR_SUCCESS);
		memcpy(rsp->data, l2cap_fixed_chan, sizeof(l2cap_fixed_chan));
		l2cap_send_cmd(conn, cmd->ident,
					L2CAP_INFO_RSP, sizeof(buf), buf);
	} else {
		struct l2cap_info_rsp rsp;
		rsp.type   = cpu_to_le16(type);
		rsp.result = cpu_to_le16(L2CAP_IR_NOTSUPP);
		l2cap_send_cmd(conn, cmd->ident,
					L2CAP_INFO_RSP, sizeof(rsp), &rsp);
	}

	return 0;
}

static inline int l2cap_information_rsp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct l2cap_info_rsp *rsp = (struct l2cap_info_rsp *) data;
	u16 type, result;

	type   = __le16_to_cpu(rsp->type);
	result = __le16_to_cpu(rsp->result);

	BT_DBG("type 0x%4.4x result 0x%2.2x", type, result);

	/* L2CAP Info req/rsp are unbound to channels, add extra checks */
	if (cmd->ident != conn->info_ident ||
			conn->info_state & L2CAP_INFO_FEAT_MASK_REQ_DONE)
		return 0;

	cancel_delayed_work(&conn->info_timer);

	if (result != L2CAP_IR_SUCCESS) {
		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
		conn->info_ident = 0;

		l2cap_conn_start(conn);

		return 0;
	}

	switch (type) {
	case L2CAP_IT_FEAT_MASK:
		conn->feat_mask = get_unaligned_le32(rsp->data);

		if (conn->feat_mask & L2CAP_FEAT_FIXED_CHAN) {
			struct l2cap_info_req req;
			req.type = cpu_to_le16(L2CAP_IT_FIXED_CHAN);

			conn->info_ident = l2cap_get_ident(conn);

			l2cap_send_cmd(conn, conn->info_ident,
					L2CAP_INFO_REQ, sizeof(req), &req);
		} else {
			conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
			conn->info_ident = 0;

			l2cap_conn_start(conn);
		}
		break;

	case L2CAP_IT_FIXED_CHAN:
		conn->fixed_chan_mask = rsp->data[0];
		conn->info_state |= L2CAP_INFO_FEAT_MASK_REQ_DONE;
		conn->info_ident = 0;

		l2cap_conn_start(conn);
		break;
	}

	return 0;
}

static inline int l2cap_create_channel_req(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u16 cmd_len,
					void *data)
{
	struct l2cap_create_chan_req *req = data;
	struct l2cap_create_chan_rsp rsp;
	u16 psm, scid;

	if (cmd_len != sizeof(*req))
		return -EPROTO;

	if (!enable_hs)
		return -EINVAL;

	psm = le16_to_cpu(req->psm);
	scid = le16_to_cpu(req->scid);

	BT_DBG("psm %d, scid %d, amp_id %d", psm, scid, req->amp_id);

	/* Placeholder: Always reject */
	rsp.dcid = 0;
	rsp.scid = cpu_to_le16(scid);
	rsp.result = L2CAP_CR_NO_MEM;
	rsp.status = L2CAP_CS_NO_INFO;

	l2cap_send_cmd(conn, cmd->ident, L2CAP_CREATE_CHAN_RSP,
		       sizeof(rsp), &rsp);

	return 0;
}

static inline int l2cap_create_channel_rsp(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, void *data)
{
	BT_DBG("conn %p", conn);

	return l2cap_connect_rsp(conn, cmd, data);
}

static void l2cap_send_move_chan_rsp(struct l2cap_conn *conn, u8 ident,
							u16 icid, u16 result)
{
	struct l2cap_move_chan_rsp rsp;

	BT_DBG("icid %d, result %d", icid, result);

	rsp.icid = cpu_to_le16(icid);
	rsp.result = cpu_to_le16(result);

	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_RSP, sizeof(rsp), &rsp);
}

static void l2cap_send_move_chan_cfm(struct l2cap_conn *conn,
				struct l2cap_chan *chan, u16 icid, u16 result)
{
	struct l2cap_move_chan_cfm cfm;
	u8 ident;

	BT_DBG("icid %d, result %d", icid, result);

	ident = l2cap_get_ident(conn);
	if (chan)
		chan->ident = ident;

	cfm.icid = cpu_to_le16(icid);
	cfm.result = cpu_to_le16(result);

	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_CFM, sizeof(cfm), &cfm);
}

static void l2cap_send_move_chan_cfm_rsp(struct l2cap_conn *conn, u8 ident,
								u16 icid)
{
	struct l2cap_move_chan_cfm_rsp rsp;

	BT_DBG("icid %d", icid);

	rsp.icid = cpu_to_le16(icid);
	l2cap_send_cmd(conn, ident, L2CAP_MOVE_CHAN_CFM_RSP, sizeof(rsp), &rsp);
}

static inline int l2cap_move_channel_req(struct l2cap_conn *conn,
			struct l2cap_cmd_hdr *cmd, u16 cmd_len, void *data)
{
	struct l2cap_move_chan_req *req = data;
	u16 icid = 0;
	u16 result = L2CAP_MR_NOT_ALLOWED;

	if (cmd_len != sizeof(*req))
		return -EPROTO;

	icid = le16_to_cpu(req->icid);

	BT_DBG("icid %d, dest_amp_id %d", icid, req->dest_amp_id);

	if (!enable_hs)
		return -EINVAL;

	/* Placeholder: Always refuse */
	l2cap_send_move_chan_rsp(conn, cmd->ident, icid, result);
>>>>>>> remotes/linux2/linux-3.4.y

	return 0;
}

<<<<<<< HEAD
int l2cap_modify_cfm(struct hci_chan *chan, u8 status)
{
	struct l2cap_conn *conn = chan->conn->l2cap_data;

	BT_DBG("chan %p conn %p status %d", chan, conn, status);

	/* TODO: if failed status restore previous fs */
	return 0;
}

int l2cap_destroy_cfm(struct hci_chan *chan, u8 reason)
{
	struct l2cap_chan_list *l;
	struct l2cap_conn *conn = chan->conn->l2cap_data;
	struct sock *sk;

	BT_DBG("chan %p conn %p", chan, conn);

	if (!conn)
		return 0;

	l = &conn->chan_list;

	read_lock(&l->lock);

	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		bh_lock_sock(sk);
		/* TODO MM/PK - What to do if connection is LOCAL_BUSY?  */
		if (l2cap_pi(sk)->ampchan == chan) {
			struct hci_conn *ampcon = l2cap_pi(sk)->ampcon;
			l2cap_pi(sk)->ampchan = NULL;
			l2cap_pi(sk)->ampcon = NULL;
			if (hci_chan_put(chan))
				ampcon->l2cap_data = NULL;
			else
				l2cap_deaggregate(chan, l2cap_pi(sk));

			l2cap_amp_move_init(sk);
		}
		bh_unlock_sock(sk);
	}

	read_unlock(&l->lock);

	return 0;


}

static int l2cap_sig_amp(struct l2cap_conn *conn, struct l2cap_cmd_hdr *cmd,
			u8 *data, struct sk_buff *skb)
{
	struct l2cap_amp_signal_work *amp_work;

	amp_work = kzalloc(sizeof(*amp_work), GFP_ATOMIC);
	if (!amp_work)
		return -ENOMEM;

	INIT_WORK(&amp_work->work, l2cap_amp_signal_worker);
	amp_work->conn = conn;
	amp_work->cmd = *cmd;
	amp_work->data = data;
	amp_work->skb = skb_clone(skb, GFP_ATOMIC);
	if (!amp_work->skb) {
		kfree(amp_work);
		return -ENOMEM;
	}

	if (!queue_work(_l2cap_wq, &amp_work->work)) {
		kfree_skb(amp_work->skb);
		kfree(amp_work);
		return -ENOMEM;
	}
=======
static inline int l2cap_move_channel_rsp(struct l2cap_conn *conn,
			struct l2cap_cmd_hdr *cmd, u16 cmd_len, void *data)
{
	struct l2cap_move_chan_rsp *rsp = data;
	u16 icid, result;

	if (cmd_len != sizeof(*rsp))
		return -EPROTO;

	icid = le16_to_cpu(rsp->icid);
	result = le16_to_cpu(rsp->result);

	BT_DBG("icid %d, result %d", icid, result);

	/* Placeholder: Always unconfirmed */
	l2cap_send_move_chan_cfm(conn, NULL, icid, L2CAP_MC_UNCONFIRMED);

	return 0;
}

static inline int l2cap_move_channel_confirm(struct l2cap_conn *conn,
			struct l2cap_cmd_hdr *cmd, u16 cmd_len, void *data)
{
	struct l2cap_move_chan_cfm *cfm = data;
	u16 icid, result;

	if (cmd_len != sizeof(*cfm))
		return -EPROTO;

	icid = le16_to_cpu(cfm->icid);
	result = le16_to_cpu(cfm->result);

	BT_DBG("icid %d, result %d", icid, result);

	l2cap_send_move_chan_cfm_rsp(conn, cmd->ident, icid);

	return 0;
}

static inline int l2cap_move_channel_confirm_rsp(struct l2cap_conn *conn,
			struct l2cap_cmd_hdr *cmd, u16 cmd_len, void *data)
{
	struct l2cap_move_chan_cfm_rsp *rsp = data;
	u16 icid;

	if (cmd_len != sizeof(*rsp))
		return -EPROTO;

	icid = le16_to_cpu(rsp->icid);

	BT_DBG("icid %d", icid);
>>>>>>> remotes/linux2/linux-3.4.y

	return 0;
}

static inline int l2cap_check_conn_param(u16 min, u16 max, u16 latency,
							u16 to_multiplier)
{
	u16 max_latency;

	if (min > max || min < 6 || max > 3200)
		return -EINVAL;

	if (to_multiplier < 10 || to_multiplier > 3200)
		return -EINVAL;

	if (max >= to_multiplier * 8)
		return -EINVAL;

	max_latency = (to_multiplier * 8 / max) - 1;
	if (latency > 499 || latency > max_latency)
		return -EINVAL;

	return 0;
}

static inline int l2cap_conn_param_update_req(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	struct hci_conn *hcon = conn->hcon;
	struct l2cap_conn_param_update_req *req;
	struct l2cap_conn_param_update_rsp rsp;
<<<<<<< HEAD
	struct sock *sk;
	u16 min, max, latency, timeout, cmd_len;
=======
	u16 min, max, latency, to_multiplier, cmd_len;
>>>>>>> remotes/linux2/linux-3.4.y
	int err;

	if (!(hcon->link_mode & HCI_LM_MASTER))
		return -EINVAL;

	cmd_len = __le16_to_cpu(cmd->len);
	if (cmd_len != sizeof(struct l2cap_conn_param_update_req))
		return -EPROTO;

<<<<<<< HEAD
	memset(&rsp, 0, sizeof(rsp));
	rsp.result = cpu_to_le16(L2CAP_CONN_PARAM_REJECTED);

	sk = l2cap_find_sock_by_fixed_cid_and_dir(4, conn->src, conn->dst, 0);

	if (sk && !bt_sk(sk)->le_params.prohibit_remote_chg) {
		req = (struct l2cap_conn_param_update_req *) data;
		min = __le16_to_cpu(req->min);
		max = __le16_to_cpu(req->max);
		latency = __le16_to_cpu(req->latency);
		timeout = __le16_to_cpu(req->to_multiplier);

		err = l2cap_check_conn_param(min, max, latency, timeout);
		if (!err) {
			rsp.result = cpu_to_le16(L2CAP_CONN_PARAM_ACCEPTED);
			hci_le_conn_update(hcon, min, max, latency, timeout);
			bt_sk(sk)->le_params.interval_min = min;
			bt_sk(sk)->le_params.interval_max = max;
			bt_sk(sk)->le_params.latency = latency;
			bt_sk(sk)->le_params.supervision_timeout = timeout;
		}
	}
=======
	req = (struct l2cap_conn_param_update_req *) data;
	min		= __le16_to_cpu(req->min);
	max		= __le16_to_cpu(req->max);
	latency		= __le16_to_cpu(req->latency);
	to_multiplier	= __le16_to_cpu(req->to_multiplier);

	BT_DBG("min 0x%4.4x max 0x%4.4x latency: 0x%4.4x Timeout: 0x%4.4x",
						min, max, latency, to_multiplier);

	memset(&rsp, 0, sizeof(rsp));

	err = l2cap_check_conn_param(min, max, latency, to_multiplier);
	if (err)
		rsp.result = cpu_to_le16(L2CAP_CONN_PARAM_REJECTED);
	else
		rsp.result = cpu_to_le16(L2CAP_CONN_PARAM_ACCEPTED);
>>>>>>> remotes/linux2/linux-3.4.y

	l2cap_send_cmd(conn, cmd->ident, L2CAP_CONN_PARAM_UPDATE_RSP,
							sizeof(rsp), &rsp);

<<<<<<< HEAD
=======
	if (!err)
		hci_le_conn_update(hcon, min, max, latency, to_multiplier);
>>>>>>> remotes/linux2/linux-3.4.y

	return 0;
}

static inline int l2cap_bredr_sig_cmd(struct l2cap_conn *conn,
<<<<<<< HEAD
			struct l2cap_cmd_hdr *cmd, u16 cmd_len, u8 *data,
			struct sk_buff *skb)
=======
			struct l2cap_cmd_hdr *cmd, u16 cmd_len, u8 *data)
>>>>>>> remotes/linux2/linux-3.4.y
{
	int err = 0;

	switch (cmd->code) {
	case L2CAP_COMMAND_REJ:
		l2cap_command_rej(conn, cmd, data);
		break;

	case L2CAP_CONN_REQ:
		err = l2cap_connect_req(conn, cmd, data);
		break;

	case L2CAP_CONN_RSP:
		err = l2cap_connect_rsp(conn, cmd, data);
		break;

	case L2CAP_CONF_REQ:
		err = l2cap_config_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_CONF_RSP:
		err = l2cap_config_rsp(conn, cmd, data);
		break;

	case L2CAP_DISCONN_REQ:
		err = l2cap_disconnect_req(conn, cmd, data);
		break;

	case L2CAP_DISCONN_RSP:
		err = l2cap_disconnect_rsp(conn, cmd, data);
		break;

	case L2CAP_ECHO_REQ:
		l2cap_send_cmd(conn, cmd->ident, L2CAP_ECHO_RSP, cmd_len, data);
		break;

	case L2CAP_ECHO_RSP:
		break;

	case L2CAP_INFO_REQ:
		err = l2cap_information_req(conn, cmd, data);
		break;

	case L2CAP_INFO_RSP:
		err = l2cap_information_rsp(conn, cmd, data);
		break;

	case L2CAP_CREATE_CHAN_REQ:
<<<<<<< HEAD
		err = l2cap_create_channel_req(conn, cmd, data);
=======
		err = l2cap_create_channel_req(conn, cmd, cmd_len, data);
>>>>>>> remotes/linux2/linux-3.4.y
		break;

	case L2CAP_CREATE_CHAN_RSP:
		err = l2cap_create_channel_rsp(conn, cmd, data);
		break;

	case L2CAP_MOVE_CHAN_REQ:
<<<<<<< HEAD
	case L2CAP_MOVE_CHAN_RSP:
	case L2CAP_MOVE_CHAN_CFM:
	case L2CAP_MOVE_CHAN_CFM_RSP:
		err = l2cap_sig_amp(conn, cmd, data, skb);
		break;
=======
		err = l2cap_move_channel_req(conn, cmd, cmd_len, data);
		break;

	case L2CAP_MOVE_CHAN_RSP:
		err = l2cap_move_channel_rsp(conn, cmd, cmd_len, data);
		break;

	case L2CAP_MOVE_CHAN_CFM:
		err = l2cap_move_channel_confirm(conn, cmd, cmd_len, data);
		break;

	case L2CAP_MOVE_CHAN_CFM_RSP:
		err = l2cap_move_channel_confirm_rsp(conn, cmd, cmd_len, data);
		break;

>>>>>>> remotes/linux2/linux-3.4.y
	default:
		BT_ERR("Unknown BR/EDR signaling command 0x%2.2x", cmd->code);
		err = -EINVAL;
		break;
	}

	return err;
}

static inline int l2cap_le_sig_cmd(struct l2cap_conn *conn,
					struct l2cap_cmd_hdr *cmd, u8 *data)
{
	switch (cmd->code) {
	case L2CAP_COMMAND_REJ:
		return 0;

	case L2CAP_CONN_PARAM_UPDATE_REQ:
		return l2cap_conn_param_update_req(conn, cmd, data);

	case L2CAP_CONN_PARAM_UPDATE_RSP:
		return 0;

	default:
		BT_ERR("Unknown LE signaling command 0x%2.2x", cmd->code);
		return -EINVAL;
	}
}

static inline void l2cap_sig_channel(struct l2cap_conn *conn,
							struct sk_buff *skb)
{
	u8 *data = skb->data;
	int len = skb->len;
	struct l2cap_cmd_hdr cmd;
	int err;

	l2cap_raw_recv(conn, skb);

	while (len >= L2CAP_CMD_HDR_SIZE) {
		u16 cmd_len;
		memcpy(&cmd, data, L2CAP_CMD_HDR_SIZE);
		data += L2CAP_CMD_HDR_SIZE;
		len  -= L2CAP_CMD_HDR_SIZE;

		cmd_len = le16_to_cpu(cmd.len);

		BT_DBG("code 0x%2.2x len %d id 0x%2.2x", cmd.code, cmd_len, cmd.ident);

		if (cmd_len > len || !cmd.ident) {
			BT_DBG("corrupted command");
			break;
		}

		if (conn->hcon->type == LE_LINK)
			err = l2cap_le_sig_cmd(conn, &cmd, data);
		else
<<<<<<< HEAD
			err = l2cap_bredr_sig_cmd(conn, &cmd, cmd_len,
							data, skb);

		if (err) {
			struct l2cap_cmd_rej rej;
=======
			err = l2cap_bredr_sig_cmd(conn, &cmd, cmd_len, data);

		if (err) {
			struct l2cap_cmd_rej_unk rej;
>>>>>>> remotes/linux2/linux-3.4.y

			BT_ERR("Wrong link type (%d)", err);

			/* FIXME: Map err to a valid reason */
<<<<<<< HEAD
			rej.reason = cpu_to_le16(0);
=======
			rej.reason = cpu_to_le16(L2CAP_REJ_NOT_UNDERSTOOD);
>>>>>>> remotes/linux2/linux-3.4.y
			l2cap_send_cmd(conn, cmd.ident, L2CAP_COMMAND_REJ, sizeof(rej), &rej);
		}

		data += cmd_len;
		len  -= cmd_len;
	}

	kfree_skb(skb);
}

<<<<<<< HEAD
static int l2cap_check_fcs(struct l2cap_pinfo *pi,  struct sk_buff *skb)
=======
static int l2cap_check_fcs(struct l2cap_chan *chan,  struct sk_buff *skb)
>>>>>>> remotes/linux2/linux-3.4.y
{
	u16 our_fcs, rcv_fcs;
	int hdr_size;

<<<<<<< HEAD
	if (pi->extended_control)
		hdr_size = L2CAP_EXTENDED_HDR_SIZE;
	else
		hdr_size = L2CAP_ENHANCED_HDR_SIZE;

	if (pi->fcs == L2CAP_FCS_CRC16) {
=======
	if (test_bit(FLAG_EXT_CTRL, &chan->flags))
		hdr_size = L2CAP_EXT_HDR_SIZE;
	else
		hdr_size = L2CAP_ENH_HDR_SIZE;

	if (chan->fcs == L2CAP_FCS_CRC16) {
>>>>>>> remotes/linux2/linux-3.4.y
		skb_trim(skb, skb->len - L2CAP_FCS_SIZE);
		rcv_fcs = get_unaligned_le16(skb->data + skb->len);
		our_fcs = crc16(0, skb->data - hdr_size, skb->len + hdr_size);

<<<<<<< HEAD
		if (our_fcs != rcv_fcs) {
			BT_DBG("Bad FCS");
			return -EBADMSG;
		}
=======
		if (our_fcs != rcv_fcs)
			return -EBADMSG;
>>>>>>> remotes/linux2/linux-3.4.y
	}
	return 0;
}

<<<<<<< HEAD
static void l2cap_ertm_pass_to_tx(struct sock *sk,
				struct bt_l2cap_control *control)
{
	BT_DBG("sk %p, control %p", sk, control);
	l2cap_ertm_tx(sk, control, 0, L2CAP_ERTM_EVENT_RECV_REQSEQ_AND_FBIT);
}

static void l2cap_ertm_pass_to_tx_fbit(struct sock *sk,
				struct bt_l2cap_control *control)
{
	BT_DBG("sk %p, control %p", sk, control);
	l2cap_ertm_tx(sk, control, 0, L2CAP_ERTM_EVENT_RECV_FBIT);
}

static void l2cap_ertm_resend(struct sock *sk)
{
	struct bt_l2cap_control control;
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;
	struct sk_buff *tx_skb;
	u16 seq;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	if (pi->conn_state & L2CAP_CONN_REMOTE_BUSY)
		return;

	if (pi->amp_move_state != L2CAP_AMP_STATE_STABLE &&
			pi->amp_move_state != L2CAP_AMP_STATE_WAIT_PREPARE)
		return;

	while (pi->retrans_list.head != L2CAP_SEQ_LIST_CLEAR) {
		seq = l2cap_seq_list_pop(&pi->retrans_list);

		skb = l2cap_ertm_seq_in_queue(TX_QUEUE(sk), seq);
		if (!skb) {
			BT_DBG("Error: Can't retransmit seq %d, frame missing",
				(int) seq);
			continue;
		}

		bt_cb(skb)->retries += 1;
		control = bt_cb(skb)->control;

		if ((pi->max_tx != 0) && (bt_cb(skb)->retries > pi->max_tx)) {
			BT_DBG("Retry limit exceeded (%d)", (int) pi->max_tx);
			l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
			l2cap_seq_list_clear(&pi->retrans_list);
			break;
		}

		control.reqseq = pi->buffer_seq;
		if (pi->conn_state & L2CAP_CONN_SEND_FBIT) {
			control.final = 1;
			pi->conn_state &= ~L2CAP_CONN_SEND_FBIT;
		} else {
			control.final = 0;
		}

		if (skb_cloned(skb)) {
			/* Cloned sk_buffs are read-only, so we need a
			 * writeable copy
			 */
			tx_skb = skb_copy(skb, GFP_ATOMIC);
		} else {
			tx_skb = skb_clone(skb, GFP_ATOMIC);
		}

		if (!tx_skb) {
			l2cap_seq_list_clear(&pi->retrans_list);
			break;
		}

		/* Update skb contents */
		if (pi->extended_control) {
			put_unaligned_le32(__pack_extended_control(&control),
					tx_skb->data + L2CAP_HDR_SIZE);
		} else {
			put_unaligned_le16(__pack_enhanced_control(&control),
					tx_skb->data + L2CAP_HDR_SIZE);
		}

		if (pi->fcs == L2CAP_FCS_CRC16)
			apply_fcs(tx_skb);

		sock_hold(sk);
		tx_skb->sk = sk;
		tx_skb->destructor = l2cap_skb_destructor;
		atomic_inc(&pi->ertm_queued);

		l2cap_do_send(sk, tx_skb);

		BT_DBG("Resent txseq %d", (int)control.txseq);

		pi->last_acked_seq = pi->buffer_seq;
	}
}

static inline void l2cap_ertm_retransmit(struct sock *sk,
					struct bt_l2cap_control *control)
{
	BT_DBG("sk %p, control %p", sk, control);

	l2cap_seq_list_append(&l2cap_pi(sk)->retrans_list, control->reqseq);
	l2cap_ertm_resend(sk);
}

static void l2cap_ertm_retransmit_all(struct sock *sk,
				struct bt_l2cap_control *control)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;

	BT_DBG("sk %p, control %p", sk, control);

	pi = l2cap_pi(sk);

	if (control->poll)
		pi->conn_state |= L2CAP_CONN_SEND_FBIT;

	l2cap_seq_list_clear(&pi->retrans_list);

	if (pi->conn_state & L2CAP_CONN_REMOTE_BUSY)
		return;

	if (pi->unacked_frames) {
		skb_queue_walk(TX_QUEUE(sk), skb) {
			if ((bt_cb(skb)->control.txseq == control->reqseq) ||
				skb == sk->sk_send_head)
				break;
		}

		skb_queue_walk_from(TX_QUEUE(sk), skb) {
			if (skb == sk->sk_send_head)
				break;

			l2cap_seq_list_append(&pi->retrans_list,
					bt_cb(skb)->control.txseq);
		}

		l2cap_ertm_resend(sk);
	}
}

static inline void append_skb_frag(struct sk_buff *skb,
			struct sk_buff *new_frag, struct sk_buff **last_frag)
{
	/* skb->len reflects data in skb as well as all fragments
	   skb->data_len reflects only data in fragments
	 */
	BT_DBG("skb %p, new_frag %p, *last_frag %p", skb, new_frag, *last_frag);

=======
static inline void l2cap_send_i_or_rr_or_rnr(struct l2cap_chan *chan)
{
	u32 control = 0;

	chan->frames_sent = 0;

	control |= __set_reqseq(chan, chan->buffer_seq);

	if (test_bit(CONN_LOCAL_BUSY, &chan->conn_state)) {
		control |= __set_ctrl_super(chan, L2CAP_SUPER_RNR);
		l2cap_send_sframe(chan, control);
		set_bit(CONN_RNR_SENT, &chan->conn_state);
	}

	if (test_bit(CONN_REMOTE_BUSY, &chan->conn_state))
		l2cap_retransmit_frames(chan);

	l2cap_ertm_send(chan);

	if (!test_bit(CONN_LOCAL_BUSY, &chan->conn_state) &&
			chan->frames_sent == 0) {
		control |= __set_ctrl_super(chan, L2CAP_SUPER_RR);
		l2cap_send_sframe(chan, control);
	}
}

static int l2cap_add_to_srej_queue(struct l2cap_chan *chan, struct sk_buff *skb, u16 tx_seq, u8 sar)
{
	struct sk_buff *next_skb;
	int tx_seq_offset, next_tx_seq_offset;

	bt_cb(skb)->tx_seq = tx_seq;
	bt_cb(skb)->sar = sar;

	next_skb = skb_peek(&chan->srej_q);

	tx_seq_offset = __seq_offset(chan, tx_seq, chan->buffer_seq);

	while (next_skb) {
		if (bt_cb(next_skb)->tx_seq == tx_seq)
			return -EINVAL;

		next_tx_seq_offset = __seq_offset(chan,
				bt_cb(next_skb)->tx_seq, chan->buffer_seq);

		if (next_tx_seq_offset > tx_seq_offset) {
			__skb_queue_before(&chan->srej_q, next_skb, skb);
			return 0;
		}

		if (skb_queue_is_last(&chan->srej_q, next_skb))
			next_skb = NULL;
		else
			next_skb = skb_queue_next(&chan->srej_q, next_skb);
	}

	__skb_queue_tail(&chan->srej_q, skb);

	return 0;
}

static void append_skb_frag(struct sk_buff *skb,
			struct sk_buff *new_frag, struct sk_buff **last_frag)
{
	/* skb->len reflects data in skb as well as all fragments
	 * skb->data_len reflects only data in fragments
	 */
>>>>>>> remotes/linux2/linux-3.4.y
	if (!skb_has_frag_list(skb))
		skb_shinfo(skb)->frag_list = new_frag;

	new_frag->next = NULL;

	(*last_frag)->next = new_frag;
	*last_frag = new_frag;

	skb->len += new_frag->len;
	skb->data_len += new_frag->len;
	skb->truesize += new_frag->truesize;
}

<<<<<<< HEAD
static int l2cap_ertm_rx_expected_iframe(struct sock *sk,
			struct bt_l2cap_control *control, struct sk_buff *skb)
{
	struct l2cap_pinfo *pi;
	int err = -EINVAL;

	BT_DBG("sk %p, control %p, skb %p len %d truesize %d", sk, control,
		skb, skb->len, skb->truesize);

	if (!control)
		return err;

	pi = l2cap_pi(sk);

	BT_DBG("type %c, sar %d, txseq %d, reqseq %d, final %d",
		control->frame_type, control->sar, control->txseq,
		control->reqseq, control->final);

	switch (control->sar) {
	case L2CAP_SAR_UNSEGMENTED:
		if (pi->sdu) {
			BT_DBG("Unexpected unsegmented PDU during reassembly");
			kfree_skb(pi->sdu);
			pi->sdu = NULL;
			pi->sdu_last_frag = NULL;
			pi->sdu_len = 0;
		}

		BT_DBG("Unsegmented");
		err = sock_queue_rcv_skb(sk, skb);
		break;

	case L2CAP_SAR_START:
		if (pi->sdu) {
			BT_DBG("Unexpected start PDU during reassembly");
			kfree_skb(pi->sdu);
		}

		pi->sdu_len = get_unaligned_le16(skb->data);
		skb_pull(skb, 2);

		if (pi->sdu_len > pi->imtu) {
=======
static int l2cap_reassemble_sdu(struct l2cap_chan *chan, struct sk_buff *skb, u32 control)
{
	int err = -EINVAL;

	switch (__get_ctrl_sar(chan, control)) {
	case L2CAP_SAR_UNSEGMENTED:
		if (chan->sdu)
			break;

		err = chan->ops->recv(chan->data, skb);
		break;

	case L2CAP_SAR_START:
		if (chan->sdu)
			break;

		chan->sdu_len = get_unaligned_le16(skb->data);
		skb_pull(skb, L2CAP_SDULEN_SIZE);

		if (chan->sdu_len > chan->imtu) {
>>>>>>> remotes/linux2/linux-3.4.y
			err = -EMSGSIZE;
			break;
		}

<<<<<<< HEAD
		if (skb->len >= pi->sdu_len)
			break;

		pi->sdu = skb;
		pi->sdu_last_frag = skb;

		BT_DBG("Start");
=======
		if (skb->len >= chan->sdu_len)
			break;

		chan->sdu = skb;
		chan->sdu_last_frag = skb;
>>>>>>> remotes/linux2/linux-3.4.y

		skb = NULL;
		err = 0;
		break;

	case L2CAP_SAR_CONTINUE:
<<<<<<< HEAD
		if (!pi->sdu)
			break;

		append_skb_frag(pi->sdu, skb,
				&pi->sdu_last_frag);
		skb = NULL;

		if (pi->sdu->len >= pi->sdu_len)
			break;

		BT_DBG("Continue, reassembled %d", pi->sdu->len);

=======
		if (!chan->sdu)
			break;

		append_skb_frag(chan->sdu, skb,
				&chan->sdu_last_frag);
		skb = NULL;

		if (chan->sdu->len >= chan->sdu_len)
			break;

>>>>>>> remotes/linux2/linux-3.4.y
		err = 0;
		break;

	case L2CAP_SAR_END:
<<<<<<< HEAD
		if (!pi->sdu)
			break;

		append_skb_frag(pi->sdu, skb,
				&pi->sdu_last_frag);
		skb = NULL;

		if (pi->sdu->len != pi->sdu_len)
			break;

		BT_DBG("End, reassembled %d", pi->sdu->len);
		/* If the sender used tiny PDUs, the rcv queuing could fail.
		 * Applications that have issues here should use a larger
		 * sk_rcvbuf.
		 */
		err = sock_queue_rcv_skb(sk, pi->sdu);

		if (!err) {
			/* Reassembly complete */
			pi->sdu = NULL;
			pi->sdu_last_frag = NULL;
			pi->sdu_len = 0;
		}
		break;

	default:
		BT_DBG("Bad SAR value");
		break;
	}

	if (err) {
		BT_DBG("Reassembly error %d, sk_rcvbuf %d, sk_rmem_alloc %d",
			err, sk->sk_rcvbuf, atomic_read(&sk->sk_rmem_alloc));
		if (pi->sdu) {
			kfree_skb(pi->sdu);
			pi->sdu = NULL;
		}
		pi->sdu_last_frag = NULL;
		pi->sdu_len = 0;
		if (skb)
			kfree_skb(skb);
	}

	/* Update local busy state */
	if (!(pi->conn_state & L2CAP_CONN_LOCAL_BUSY) && l2cap_rmem_full(sk))
		l2cap_ertm_tx(sk, 0, 0, L2CAP_ERTM_EVENT_LOCAL_BUSY_DETECTED);

	return err;
}

static int l2cap_ertm_rx_queued_iframes(struct sock *sk)
{
	int err = 0;
	/* Pass sequential frames to l2cap_ertm_rx_expected_iframe()
	 * until a gap is encountered.
	 */

	struct l2cap_pinfo *pi;

	BT_DBG("sk %p", sk);
	pi = l2cap_pi(sk);

	while (l2cap_rmem_available(sk)) {
		struct sk_buff *skb;
		BT_DBG("Searching for skb with txseq %d (queue len %d)",
			(int) pi->buffer_seq, skb_queue_len(SREJ_QUEUE(sk)));

		skb = l2cap_ertm_seq_in_queue(SREJ_QUEUE(sk), pi->buffer_seq);

		if (!skb)
			break;

		skb_unlink(skb, SREJ_QUEUE(sk));
		pi->buffer_seq = __next_seq(pi->buffer_seq, pi);
		err = l2cap_ertm_rx_expected_iframe(sk,
						&bt_cb(skb)->control, skb);
		if (err)
			break;
	}

	if (skb_queue_empty(SREJ_QUEUE(sk))) {
		pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
		l2cap_ertm_send_ack(sk);
	}

	return err;
}

static void l2cap_ertm_handle_srej(struct sock *sk,
				struct bt_l2cap_control *control)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;

	BT_DBG("sk %p, control %p", sk, control);

	pi = l2cap_pi(sk);

	if (control->reqseq == pi->next_tx_seq) {
		BT_DBG("Invalid reqseq %d, disconnecting",
			(int) control->reqseq);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		return;
	}

	skb = l2cap_ertm_seq_in_queue(TX_QUEUE(sk), control->reqseq);

	if (skb == NULL) {
		BT_DBG("Seq %d not available for retransmission",
			(int) control->reqseq);
		return;
	}

	if ((pi->max_tx != 0) && (bt_cb(skb)->retries >= pi->max_tx)) {
		BT_DBG("Retry limit exceeded (%d)", (int) pi->max_tx);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		return;
	}

	pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

	if (control->poll) {
		l2cap_ertm_pass_to_tx(sk, control);

		pi->conn_state |= L2CAP_CONN_SEND_FBIT;
		l2cap_ertm_retransmit(sk, control);
		l2cap_ertm_send(sk);

		if (pi->tx_state == L2CAP_ERTM_TX_STATE_WAIT_F) {
			pi->conn_state |= L2CAP_CONN_SREJ_ACT;
			pi->srej_save_reqseq = control->reqseq;
		}
	} else {
		l2cap_ertm_pass_to_tx_fbit(sk, control);

		if (control->final) {
			if ((pi->conn_state & L2CAP_CONN_SREJ_ACT) &&
				(pi->srej_save_reqseq == control->reqseq)) {
				pi->conn_state &= ~L2CAP_CONN_SREJ_ACT;
			} else {
				l2cap_ertm_retransmit(sk, control);
			}
		} else {
			l2cap_ertm_retransmit(sk, control);
			if (pi->tx_state == L2CAP_ERTM_TX_STATE_WAIT_F) {
				pi->conn_state |= L2CAP_CONN_SREJ_ACT;
				pi->srej_save_reqseq = control->reqseq;
			}
		}
	}
}

static void l2cap_ertm_handle_rej(struct sock *sk,
				struct bt_l2cap_control *control)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;

	BT_DBG("sk %p, control %p", sk, control);

	pi = l2cap_pi(sk);

	if (control->reqseq == pi->next_tx_seq) {
		BT_DBG("Invalid reqseq %d, disconnecting",
			(int) control->reqseq);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		return;
	}

	skb = l2cap_ertm_seq_in_queue(TX_QUEUE(sk), control->reqseq);

	if (pi->max_tx && skb && bt_cb(skb)->retries >= pi->max_tx) {
		BT_DBG("Retry limit exceeded (%d)", (int) pi->max_tx);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
		return;
	}

	pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

	l2cap_ertm_pass_to_tx(sk, control);

	if (control->final) {
		if (pi->conn_state & L2CAP_CONN_REJ_ACT)
			pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
		else
			l2cap_ertm_retransmit_all(sk, control);
	} else {
		l2cap_ertm_retransmit_all(sk, control);
		l2cap_ertm_send(sk);
		if (pi->tx_state == L2CAP_ERTM_TX_STATE_WAIT_F)
			pi->conn_state |= L2CAP_CONN_REJ_ACT;
	}
}

static u8 l2cap_ertm_classify_txseq(struct sock *sk, u16 txseq)
{
	struct l2cap_pinfo *pi;

	BT_DBG("sk %p, txseq %d", sk, (int)txseq);
	pi = l2cap_pi(sk);

	BT_DBG("last_acked_seq %d, expected_tx_seq %d", (int)pi->last_acked_seq,
		(int)pi->expected_tx_seq);

	if (pi->rx_state == L2CAP_ERTM_RX_STATE_SREJ_SENT) {
		if (__delta_seq(txseq, pi->last_acked_seq, pi) >= pi->tx_win) {
			/* See notes below regarding "double poll" and
			 * invalid packets.
			 */
			if (pi->tx_win <= ((pi->tx_win_max + 1) >> 1)) {
				BT_DBG("Invalid/Ignore - txseq outside "
					"tx window after SREJ sent");
				return L2CAP_ERTM_TXSEQ_INVALID_IGNORE;
			} else {
				BT_DBG("Invalid - bad txseq within tx "
					"window after SREJ sent");
				return L2CAP_ERTM_TXSEQ_INVALID;
			}
		}

		if (pi->srej_list.head == txseq) {
			BT_DBG("Expected SREJ");
			return L2CAP_ERTM_TXSEQ_EXPECTED_SREJ;
		}

		if (l2cap_ertm_seq_in_queue(SREJ_QUEUE(sk), txseq)) {
			BT_DBG("Duplicate SREJ - txseq already stored");
			return L2CAP_ERTM_TXSEQ_DUPLICATE_SREJ;
		}

		if (l2cap_seq_list_contains(&pi->srej_list, txseq)) {
			BT_DBG("Unexpected SREJ - txseq not requested "
				"with SREJ");
			return L2CAP_ERTM_TXSEQ_UNEXPECTED_SREJ;
		}
	}

	if (pi->expected_tx_seq == txseq) {
		if (__delta_seq(txseq, pi->last_acked_seq, pi) >= pi->tx_win) {
			BT_DBG("Invalid - txseq outside tx window");
			return L2CAP_ERTM_TXSEQ_INVALID;
		} else {
			BT_DBG("Expected");
			return L2CAP_ERTM_TXSEQ_EXPECTED;
		}
	}

	if (__delta_seq(txseq, pi->last_acked_seq, pi) <
		__delta_seq(pi->expected_tx_seq, pi->last_acked_seq, pi)) {
		BT_DBG("Duplicate - expected_tx_seq later than txseq");
		return L2CAP_ERTM_TXSEQ_DUPLICATE;
	}

	if (__delta_seq(txseq, pi->last_acked_seq, pi) >= pi->tx_win) {
		/* A source of invalid packets is a "double poll" condition,
		 * where delays cause us to send multiple poll packets.  If
		 * the remote stack receives and processes both polls,
		 * sequence numbers can wrap around in such a way that a
		 * resent frame has a sequence number that looks like new data
		 * with a sequence gap.  This would trigger an erroneous SREJ
		 * request.
		 *
		 * Fortunately, this is impossible with a tx window that's
		 * less than half of the maximum sequence number, which allows
		 * invalid frames to be safely ignored.
		 *
		 * With tx window sizes greater than half of the tx window
		 * maximum, the frame is invalid and cannot be ignored.  This
		 * causes a disconnect.
		 */

		if (pi->tx_win <= ((pi->tx_win_max + 1) >> 1)) {
			BT_DBG("Invalid/Ignore - txseq outside tx window");
			return L2CAP_ERTM_TXSEQ_INVALID_IGNORE;
		} else {
			BT_DBG("Invalid - txseq outside tx window");
			return L2CAP_ERTM_TXSEQ_INVALID;
		}
	} else {
		BT_DBG("Unexpected - txseq indicates missing frames");
		return L2CAP_ERTM_TXSEQ_UNEXPECTED;
	}
}

static int l2cap_ertm_rx_state_recv(struct sock *sk,
				struct bt_l2cap_control *control,
				struct sk_buff *skb, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;
	bool skb_in_use = 0;

	BT_DBG("sk %p, control %p, skb %p, event %d", sk, control, skb,
		(int)event);
	pi = l2cap_pi(sk);

	switch (event) {
	case L2CAP_ERTM_EVENT_RECV_IFRAME:
		switch (l2cap_ertm_classify_txseq(sk, control->txseq)) {
		case L2CAP_ERTM_TXSEQ_EXPECTED:
			l2cap_ertm_pass_to_tx(sk, control);

			if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
				BT_DBG("Busy, discarding expected seq %d",
					control->txseq);
				break;
			}

			pi->expected_tx_seq = __next_seq(control->txseq, pi);
			pi->buffer_seq = pi->expected_tx_seq;
			skb_in_use = 1;

			err = l2cap_ertm_rx_expected_iframe(sk, control, skb);
			if (err)
				break;

			if (control->final) {
				if (pi->conn_state & L2CAP_CONN_REJ_ACT)
					pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
				else {
					control->final = 0;
					l2cap_ertm_retransmit_all(sk, control);
					l2cap_ertm_send(sk);
				}
			}

			if (!(pi->conn_state & L2CAP_CONN_LOCAL_BUSY))
				l2cap_ertm_send_ack(sk);
			break;
		case L2CAP_ERTM_TXSEQ_UNEXPECTED:
			l2cap_ertm_pass_to_tx(sk, control);

			/* Can't issue SREJ frames in the local busy state.
			 * Drop this frame, it will be seen as missing
			 * when local busy is exited.
			 */
			if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
				BT_DBG("Busy, discarding unexpected seq %d",
					control->txseq);
				break;
			}

			/* There was a gap in the sequence, so an SREJ
			 * must be sent for each missing frame.  The
			 * current frame is stored for later use.
			 */
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			pi->conn_state &= ~L2CAP_CONN_SREJ_ACT;
			l2cap_seq_list_clear(&pi->srej_list);
			l2cap_ertm_send_srej(sk, control->txseq);

			pi->rx_state = L2CAP_ERTM_RX_STATE_SREJ_SENT;
			break;
		case L2CAP_ERTM_TXSEQ_DUPLICATE:
			l2cap_ertm_pass_to_tx(sk, control);
			break;
		case L2CAP_ERTM_TXSEQ_INVALID_IGNORE:
			break;
		case L2CAP_ERTM_TXSEQ_INVALID:
		default:
			l2cap_send_disconn_req(l2cap_pi(sk)->conn, sk,
					ECONNRESET);
			break;
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RR:
		l2cap_ertm_pass_to_tx(sk, control);
		if (control->final) {
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

			if (pi->conn_state & L2CAP_CONN_REJ_ACT)
				pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
			else if (pi->amp_move_state == L2CAP_AMP_STATE_STABLE ||
				pi->amp_move_state ==
						L2CAP_AMP_STATE_WAIT_PREPARE) {
				control->final = 0;
				l2cap_ertm_retransmit_all(sk, control);
			}

			l2cap_ertm_send(sk);
		} else if (control->poll) {
			l2cap_ertm_send_i_or_rr_or_rnr(sk);
		} else {
			if ((pi->conn_state & L2CAP_CONN_REMOTE_BUSY) &&
				pi->unacked_frames)
				l2cap_ertm_start_retrans_timer(pi);
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;
			l2cap_ertm_send(sk);
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RNR:
		pi->conn_state |= L2CAP_CONN_REMOTE_BUSY;
		l2cap_ertm_pass_to_tx(sk, control);
		if (control && control->poll) {
			pi->conn_state |= L2CAP_CONN_SEND_FBIT;
			l2cap_ertm_send_rr_or_rnr(sk, 0);
		}
		l2cap_ertm_stop_retrans_timer(pi);
		l2cap_seq_list_clear(&pi->retrans_list);
		break;
	case L2CAP_ERTM_EVENT_RECV_REJ:
		l2cap_ertm_handle_rej(sk, control);
		break;
	case L2CAP_ERTM_EVENT_RECV_SREJ:
		l2cap_ertm_handle_srej(sk, control);
		break;
	default:
		break;
	}

	if (skb && !skb_in_use) {
		BT_DBG("Freeing %p", skb);
		kfree_skb(skb);
	}

	return err;
}

static int l2cap_ertm_rx_state_srej_sent(struct sock *sk,
					struct bt_l2cap_control *control,
					struct sk_buff *skb, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;
	u16 txseq = control->txseq;
	bool skb_in_use = 0;

	BT_DBG("sk %p, control %p, skb %p, event %d", sk, control, skb,
		(int)event);
	pi = l2cap_pi(sk);

	switch (event) {
	case L2CAP_ERTM_EVENT_RECV_IFRAME:
		switch (l2cap_ertm_classify_txseq(sk, txseq)) {
		case L2CAP_ERTM_TXSEQ_EXPECTED:
			/* Keep frame for reassembly later */
			l2cap_ertm_pass_to_tx(sk, control);
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			pi->expected_tx_seq = __next_seq(txseq, pi);
			break;
		case L2CAP_ERTM_TXSEQ_EXPECTED_SREJ:
			l2cap_seq_list_pop(&pi->srej_list);

			l2cap_ertm_pass_to_tx(sk, control);
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			err = l2cap_ertm_rx_queued_iframes(sk);
			if (err)
				break;

			break;
		case L2CAP_ERTM_TXSEQ_UNEXPECTED:
			/* Got a frame that can't be reassembled yet.
			 * Save it for later, and send SREJs to cover
			 * the missing frames.
			 */
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			l2cap_ertm_pass_to_tx(sk, control);
			l2cap_ertm_send_srej(sk, control->txseq);
			break;
		case L2CAP_ERTM_TXSEQ_UNEXPECTED_SREJ:
			/* This frame was requested with an SREJ, but
			 * some expected retransmitted frames are
			 * missing.  Request retransmission of missing
			 * SREJ'd frames.
			 */
			skb_queue_tail(SREJ_QUEUE(sk), skb);
			skb_in_use = 1;
			BT_DBG("Queued %p (queue len %d)", skb,
			       skb_queue_len(SREJ_QUEUE(sk)));

			l2cap_ertm_pass_to_tx(sk, control);
			l2cap_ertm_send_srej_list(sk, control->txseq);
			break;
		case L2CAP_ERTM_TXSEQ_DUPLICATE_SREJ:
			/* We've already queued this frame.  Drop this copy. */
			l2cap_ertm_pass_to_tx(sk, control);
			break;
		case L2CAP_ERTM_TXSEQ_DUPLICATE:
			/* Expecting a later sequence number, so this frame
			 * was already received.  Ignore it completely.
			 */
			break;
		case L2CAP_ERTM_TXSEQ_INVALID_IGNORE:
			break;
		case L2CAP_ERTM_TXSEQ_INVALID:
		default:
			l2cap_send_disconn_req(l2cap_pi(sk)->conn, sk,
					ECONNRESET);
			break;
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RR:
		l2cap_ertm_pass_to_tx(sk, control);
		if (control->final) {
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;

			if (pi->conn_state & L2CAP_CONN_REJ_ACT)
				pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
			else {
				control->final = 0;
				l2cap_ertm_retransmit_all(sk, control);
			}

			l2cap_ertm_send(sk);
		} else if (control->poll) {
			if ((pi->conn_state & L2CAP_CONN_REMOTE_BUSY) &&
				pi->unacked_frames) {
				l2cap_ertm_start_retrans_timer(pi);
			}
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;
			pi->conn_state |= L2CAP_CONN_SEND_FBIT;
			l2cap_ertm_send_srej_tail(sk);
		} else {
			if ((pi->conn_state & L2CAP_CONN_REMOTE_BUSY) &&
				pi->unacked_frames) {
				l2cap_ertm_start_retrans_timer(pi);
			}
			pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;
			l2cap_ertm_send_ack(sk);
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RNR:
		pi->conn_state |= L2CAP_CONN_REMOTE_BUSY;
		l2cap_ertm_pass_to_tx(sk, control);
		if (control->poll)
			l2cap_ertm_send_srej_tail(sk);
		else {
			struct bt_l2cap_control rr_control;
			memset(&rr_control, 0, sizeof(rr_control));
			rr_control.frame_type = 's';
			rr_control.super = L2CAP_SFRAME_RR;
			rr_control.reqseq = pi->buffer_seq;
			l2cap_ertm_send_sframe(sk, &rr_control);
		}

		break;
	case L2CAP_ERTM_EVENT_RECV_REJ:
		l2cap_ertm_handle_rej(sk, control);
		break;
	case L2CAP_ERTM_EVENT_RECV_SREJ:
		l2cap_ertm_handle_srej(sk, control);
		break;
	}

	if (skb && !skb_in_use) {
		BT_DBG("Freeing %p", skb);
		kfree_skb(skb);
	}

	return err;
}

static int l2cap_ertm_rx_state_amp_move(struct sock *sk,
					struct bt_l2cap_control *control,
					struct sk_buff *skb, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;
	bool skb_in_use = 0;

	BT_DBG("sk %p, control %p, skb %p, event %d", sk, control, skb,
		(int)event);
	pi = l2cap_pi(sk);

	/* Only handle expected frames, to avoid state changes. */

	switch (event) {
	case L2CAP_ERTM_EVENT_RECV_IFRAME:
		if (l2cap_ertm_classify_txseq(sk, control->txseq) ==
				L2CAP_ERTM_TXSEQ_EXPECTED) {
			l2cap_ertm_pass_to_tx(sk, control);

			if (pi->conn_state & L2CAP_CONN_LOCAL_BUSY) {
				BT_DBG("Busy, discarding expected seq %d",
					control->txseq);
				break;
			}

			pi->expected_tx_seq = __next_seq(control->txseq, pi);
			pi->buffer_seq = pi->expected_tx_seq;
			skb_in_use = 1;

			err = l2cap_ertm_rx_expected_iframe(sk, control, skb);
			if (err)
				break;

			if (control->final) {
				if (pi->conn_state & L2CAP_CONN_REJ_ACT)
					pi->conn_state &= ~L2CAP_CONN_REJ_ACT;
				else
					control->final = 0;
			}
		}
		break;
	case L2CAP_ERTM_EVENT_RECV_RR:
	case L2CAP_ERTM_EVENT_RECV_RNR:
	case L2CAP_ERTM_EVENT_RECV_REJ:
		l2cap_ertm_process_reqseq(sk, control->reqseq);
		break;
	case L2CAP_ERTM_EVENT_RECV_SREJ:
		/* Ignore */
		break;
	default:
		break;
	}

	if (skb && !skb_in_use) {
		BT_DBG("Freeing %p", skb);
		kfree_skb(skb);
	}

	return err;
}

static int l2cap_answer_move_poll(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	struct bt_l2cap_control control;
	int err = 0;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	l2cap_ertm_process_reqseq(sk, pi->amp_move_reqseq);

	if (!skb_queue_empty(TX_QUEUE(sk)))
		sk->sk_send_head = skb_peek(TX_QUEUE(sk));
	else
		sk->sk_send_head = NULL;

	/* Rewind next_tx_seq to the point expected
	 * by the receiver.
	 */
	pi->next_tx_seq = pi->amp_move_reqseq;
	pi->unacked_frames = 0;

	err = l2cap_finish_amp_move(sk);

	if (err)
		return err;

	pi->conn_state |= L2CAP_CONN_SEND_FBIT;
	l2cap_ertm_send_i_or_rr_or_rnr(sk);

	memset(&control, 0, sizeof(control));
	control.reqseq = pi->amp_move_reqseq;

	if (pi->amp_move_event == L2CAP_ERTM_EVENT_RECV_IFRAME)
		err = -EPROTO;
	else
		err = l2cap_ertm_rx_state_recv(sk, &control, NULL,
					pi->amp_move_event);

	return err;
}

static void l2cap_amp_move_setup(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	struct sk_buff *skb;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	l2cap_ertm_stop_ack_timer(pi);
	l2cap_ertm_stop_retrans_timer(pi);
	l2cap_ertm_stop_monitor_timer(pi);

	pi->retry_count = 0;
	skb_queue_walk(TX_QUEUE(sk), skb) {
		if (bt_cb(skb)->retries)
			bt_cb(skb)->retries = 1;
		else
			break;
	}

	pi->expected_tx_seq = pi->buffer_seq;

	pi->conn_state &= ~(L2CAP_CONN_REJ_ACT | L2CAP_CONN_SREJ_ACT);
	l2cap_seq_list_clear(&pi->retrans_list);
	l2cap_seq_list_clear(&l2cap_pi(sk)->srej_list);
	skb_queue_purge(SREJ_QUEUE(sk));

	pi->tx_state = L2CAP_ERTM_TX_STATE_XMIT;
	pi->rx_state = L2CAP_ERTM_RX_STATE_AMP_MOVE;

	BT_DBG("tx_state 0x2.2%x rx_state  0x2.2%x", pi->tx_state,
		pi->rx_state);

	pi->conn_state |= L2CAP_CONN_REMOTE_BUSY;
}

static void l2cap_amp_move_revert(struct sock *sk)
{
	struct l2cap_pinfo *pi;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
		l2cap_ertm_tx(sk, NULL, NULL, L2CAP_ERTM_EVENT_EXPLICIT_POLL);
		pi->rx_state = L2CAP_ERTM_RX_STATE_WAIT_F_FLAG;
	} else if (pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER)
		pi->rx_state = L2CAP_ERTM_RX_STATE_WAIT_P_FLAG;
}

static int l2cap_amp_move_reconf(struct sock *sk)
{
	struct l2cap_pinfo *pi;
	u8 buf[64];
	int err = 0;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	l2cap_send_cmd(pi->conn, l2cap_get_ident(pi->conn), L2CAP_CONF_REQ,
				l2cap_build_amp_reconf_req(sk, buf), buf);
	return err;
}

static void l2cap_amp_move_success(struct sock *sk)
{
	struct l2cap_pinfo *pi;

	BT_DBG("sk %p", sk);

	pi = l2cap_pi(sk);

	if (pi->amp_move_role == L2CAP_AMP_MOVE_INITIATOR) {
		int err = 0;
		/* Send reconfigure request */
		if (pi->mode == L2CAP_MODE_ERTM) {
			pi->reconf_state = L2CAP_RECONF_INT;
			if (enable_reconfig)
				err = l2cap_amp_move_reconf(sk);

			if (err || !enable_reconfig) {
				pi->reconf_state = L2CAP_RECONF_NONE;
				l2cap_ertm_tx(sk, NULL, NULL,
						L2CAP_ERTM_EVENT_EXPLICIT_POLL);
				pi->rx_state = L2CAP_ERTM_RX_STATE_WAIT_F_FLAG;
			}
		} else
			pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
	} else if (pi->amp_move_role == L2CAP_AMP_MOVE_RESPONDER) {
		if (pi->mode == L2CAP_MODE_ERTM)
			pi->rx_state =
				L2CAP_ERTM_RX_STATE_WAIT_P_FLAG_RECONFIGURE;
		else
			pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
	}
}

static inline bool __valid_reqseq(struct l2cap_pinfo *pi, u16 reqseq)
{
	/* Make sure reqseq is for a packet that has been sent but not acked */
	u16 unacked = __delta_seq(pi->next_tx_seq, pi->expected_ack_seq, pi);
	return __delta_seq(pi->next_tx_seq, reqseq, pi) <= unacked;
}

static int l2cap_strm_rx(struct sock *sk, struct bt_l2cap_control *control,
			struct sk_buff *skb)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skb %p, state %d",
		sk, control, skb, l2cap_pi(sk)->rx_state);

	pi = l2cap_pi(sk);

	if (l2cap_ertm_classify_txseq(sk, control->txseq) ==
		L2CAP_ERTM_TXSEQ_EXPECTED) {
		l2cap_ertm_pass_to_tx(sk, control);

		BT_DBG("buffer_seq %d->%d", pi->buffer_seq,
			   __next_seq(pi->buffer_seq, pi));

		pi->buffer_seq = __next_seq(pi->buffer_seq, pi);

		l2cap_ertm_rx_expected_iframe(sk, control, skb);
	} else {
		if (pi->sdu) {
			kfree_skb(pi->sdu);
			pi->sdu = NULL;
		}
		pi->sdu_last_frag = NULL;
		pi->sdu_len = 0;

		if (skb) {
			BT_DBG("Freeing %p", skb);
			kfree_skb(skb);
		}
	}

	pi->last_acked_seq = control->txseq;
	pi->expected_tx_seq = __next_seq(control->txseq, pi);

	return err;
}

static int l2cap_ertm_rx(struct sock *sk, struct bt_l2cap_control *control,
			struct sk_buff *skb, u8 event)
{
	struct l2cap_pinfo *pi;
	int err = 0;

	BT_DBG("sk %p, control %p, skb %p, event %d, state %d",
		sk, control, skb, (int)event, l2cap_pi(sk)->rx_state);

	pi = l2cap_pi(sk);

	if (__valid_reqseq(pi, control->reqseq)) {
		switch (pi->rx_state) {
		case L2CAP_ERTM_RX_STATE_RECV:
			err = l2cap_ertm_rx_state_recv(sk, control, skb, event);
			break;
		case L2CAP_ERTM_RX_STATE_SREJ_SENT:
			err = l2cap_ertm_rx_state_srej_sent(sk, control, skb,
							event);
			break;
		case L2CAP_ERTM_RX_STATE_AMP_MOVE:
			err = l2cap_ertm_rx_state_amp_move(sk, control, skb,
							event);
			break;
		case L2CAP_ERTM_RX_STATE_WAIT_F_FLAG:
			if (control->final) {
				pi->conn_state &= ~L2CAP_CONN_REMOTE_BUSY;
				pi->amp_move_role = L2CAP_AMP_MOVE_NONE;

				pi->rx_state = L2CAP_ERTM_RX_STATE_RECV;
				l2cap_ertm_process_reqseq(sk, control->reqseq);

				if (!skb_queue_empty(TX_QUEUE(sk)))
					sk->sk_send_head =
						skb_peek(TX_QUEUE(sk));
				else
					sk->sk_send_head = NULL;

				/* Rewind next_tx_seq to the point expected
				 * by the receiver.
				 */
				pi->next_tx_seq = control->reqseq;
				pi->unacked_frames = 0;

				if (pi->ampcon)
					pi->conn->mtu =
						pi->ampcon->hdev->acl_mtu;
				else
					pi->conn->mtu =
						pi->conn->hcon->hdev->acl_mtu;

				err = l2cap_setup_resegment(sk);

				if (err)
					break;

				err = l2cap_ertm_rx_state_recv(sk, control, skb,
							event);
			}
			break;
		case L2CAP_ERTM_RX_STATE_WAIT_P_FLAG:
			if (control->poll) {
				pi->amp_move_reqseq = control->reqseq;
				pi->amp_move_event = event;
				err = l2cap_answer_move_poll(sk);
			}
			break;
		case L2CAP_ERTM_RX_STATE_WAIT_P_FLAG_RECONFIGURE:
			if (control->poll) {
				pi->amp_move_reqseq = control->reqseq;
				pi->amp_move_event = event;

				BT_DBG("amp_move_role 0x%2.2x, "
					"reconf_state 0x%2.2x",
					pi->amp_move_role, pi->reconf_state);

				if (pi->reconf_state == L2CAP_RECONF_ACC)
					err = l2cap_amp_move_reconf(sk);
				else
					err = l2cap_answer_move_poll(sk);
			}
			break;
		default:
			/* shut it down */
			break;
		}
	} else {
		BT_DBG("Invalid reqseq %d (next_tx_seq %d, expected_ack_seq %d",
			control->reqseq, pi->next_tx_seq, pi->expected_ack_seq);
		l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
	}

	return err;
}

void l2cap_fixed_channel_config(struct sock *sk, struct l2cap_options *opt)
{
	lock_sock(sk);

	l2cap_pi(sk)->fixed_channel = 1;

	l2cap_pi(sk)->imtu = opt->imtu;
	l2cap_pi(sk)->omtu = opt->omtu;
	l2cap_pi(sk)->remote_mps = opt->omtu;
	l2cap_pi(sk)->mps = opt->omtu;
	l2cap_pi(sk)->flush_to = opt->flush_to;
	l2cap_pi(sk)->mode = opt->mode;
	l2cap_pi(sk)->fcs = opt->fcs;
	l2cap_pi(sk)->max_tx = opt->max_tx;
	l2cap_pi(sk)->remote_max_tx = opt->max_tx;
	l2cap_pi(sk)->tx_win = opt->txwin_size;
	l2cap_pi(sk)->remote_tx_win = opt->txwin_size;
	l2cap_pi(sk)->retrans_timeout = L2CAP_DEFAULT_RETRANS_TO;
	l2cap_pi(sk)->monitor_timeout = L2CAP_DEFAULT_MONITOR_TO;

	if (opt->mode == L2CAP_MODE_ERTM ||
		l2cap_pi(sk)->mode == L2CAP_MODE_STREAMING)
		l2cap_ertm_init(sk);

	release_sock(sk);

	return;
}

static const u8 l2cap_ertm_rx_func_to_event[4] = {
	L2CAP_ERTM_EVENT_RECV_RR, L2CAP_ERTM_EVENT_RECV_REJ,
	L2CAP_ERTM_EVENT_RECV_RNR, L2CAP_ERTM_EVENT_RECV_SREJ
};

int l2cap_data_channel(struct sock *sk, struct sk_buff *skb)
{
	struct l2cap_pinfo *pi;
	struct bt_l2cap_control *control;
	u16 len;
	u8 event;
	pi = l2cap_pi(sk);

	BT_DBG("sk %p, len %d, mode %d", sk, skb->len, pi->mode);

	if (sk->sk_state != BT_CONNECTED)
		goto drop;

	switch (pi->mode) {
=======
		if (!chan->sdu)
			break;

		append_skb_frag(chan->sdu, skb,
				&chan->sdu_last_frag);
		skb = NULL;

		if (chan->sdu->len != chan->sdu_len)
			break;

		err = chan->ops->recv(chan->data, chan->sdu);

		if (!err) {
			/* Reassembly complete */
			chan->sdu = NULL;
			chan->sdu_last_frag = NULL;
			chan->sdu_len = 0;
		}
		break;
	}

	if (err) {
		kfree_skb(skb);
		kfree_skb(chan->sdu);
		chan->sdu = NULL;
		chan->sdu_last_frag = NULL;
		chan->sdu_len = 0;
	}

	return err;
}

static void l2cap_ertm_enter_local_busy(struct l2cap_chan *chan)
{
	BT_DBG("chan %p, Enter local busy", chan);

	set_bit(CONN_LOCAL_BUSY, &chan->conn_state);

	__set_ack_timer(chan);
}

static void l2cap_ertm_exit_local_busy(struct l2cap_chan *chan)
{
	u32 control;

	if (!test_bit(CONN_RNR_SENT, &chan->conn_state))
		goto done;

	control = __set_reqseq(chan, chan->buffer_seq);
	control |= __set_ctrl_poll(chan);
	control |= __set_ctrl_super(chan, L2CAP_SUPER_RR);
	l2cap_send_sframe(chan, control);
	chan->retry_count = 1;

	__clear_retrans_timer(chan);
	__set_monitor_timer(chan);

	set_bit(CONN_WAIT_F, &chan->conn_state);

done:
	clear_bit(CONN_LOCAL_BUSY, &chan->conn_state);
	clear_bit(CONN_RNR_SENT, &chan->conn_state);

	BT_DBG("chan %p, Exit local busy", chan);
}

void l2cap_chan_busy(struct l2cap_chan *chan, int busy)
{
	if (chan->mode == L2CAP_MODE_ERTM) {
		if (busy)
			l2cap_ertm_enter_local_busy(chan);
		else
			l2cap_ertm_exit_local_busy(chan);
	}
}

static void l2cap_check_srej_gap(struct l2cap_chan *chan, u16 tx_seq)
{
	struct sk_buff *skb;
	u32 control;

	while ((skb = skb_peek(&chan->srej_q)) &&
			!test_bit(CONN_LOCAL_BUSY, &chan->conn_state)) {
		int err;

		if (bt_cb(skb)->tx_seq != tx_seq)
			break;

		skb = skb_dequeue(&chan->srej_q);
		control = __set_ctrl_sar(chan, bt_cb(skb)->sar);
		err = l2cap_reassemble_sdu(chan, skb, control);

		if (err < 0) {
			l2cap_send_disconn_req(chan->conn, chan, ECONNRESET);
			break;
		}

		chan->buffer_seq_srej = __next_seq(chan, chan->buffer_seq_srej);
		tx_seq = __next_seq(chan, tx_seq);
	}
}

static void l2cap_resend_srejframe(struct l2cap_chan *chan, u16 tx_seq)
{
	struct srej_list *l, *tmp;
	u32 control;

	list_for_each_entry_safe(l, tmp, &chan->srej_l, list) {
		if (l->tx_seq == tx_seq) {
			list_del(&l->list);
			kfree(l);
			return;
		}
		control = __set_ctrl_super(chan, L2CAP_SUPER_SREJ);
		control |= __set_reqseq(chan, l->tx_seq);
		l2cap_send_sframe(chan, control);
		list_del(&l->list);
		list_add_tail(&l->list, &chan->srej_l);
	}
}

static int l2cap_send_srejframe(struct l2cap_chan *chan, u16 tx_seq)
{
	struct srej_list *new;
	u32 control;

	while (tx_seq != chan->expected_tx_seq) {
		control = __set_ctrl_super(chan, L2CAP_SUPER_SREJ);
		control |= __set_reqseq(chan, chan->expected_tx_seq);
		l2cap_send_sframe(chan, control);

		new = kzalloc(sizeof(struct srej_list), GFP_ATOMIC);
		if (!new)
			return -ENOMEM;

		new->tx_seq = chan->expected_tx_seq;

		chan->expected_tx_seq = __next_seq(chan, chan->expected_tx_seq);

		list_add_tail(&new->list, &chan->srej_l);
	}

	chan->expected_tx_seq = __next_seq(chan, chan->expected_tx_seq);

	return 0;
}

static inline int l2cap_data_channel_iframe(struct l2cap_chan *chan, u32 rx_control, struct sk_buff *skb)
{
	u16 tx_seq = __get_txseq(chan, rx_control);
	u16 req_seq = __get_reqseq(chan, rx_control);
	u8 sar = __get_ctrl_sar(chan, rx_control);
	int tx_seq_offset, expected_tx_seq_offset;
	int num_to_ack = (chan->tx_win/6) + 1;
	int err = 0;

	BT_DBG("chan %p len %d tx_seq %d rx_control 0x%8.8x", chan, skb->len,
							tx_seq, rx_control);

	if (__is_ctrl_final(chan, rx_control) &&
			test_bit(CONN_WAIT_F, &chan->conn_state)) {
		__clear_monitor_timer(chan);
		if (chan->unacked_frames > 0)
			__set_retrans_timer(chan);
		clear_bit(CONN_WAIT_F, &chan->conn_state);
	}

	chan->expected_ack_seq = req_seq;
	l2cap_drop_acked_frames(chan);

	tx_seq_offset = __seq_offset(chan, tx_seq, chan->buffer_seq);

	/* invalid tx_seq */
	if (tx_seq_offset >= chan->tx_win) {
		l2cap_send_disconn_req(chan->conn, chan, ECONNRESET);
		goto drop;
	}

	if (test_bit(CONN_LOCAL_BUSY, &chan->conn_state)) {
		if (!test_bit(CONN_RNR_SENT, &chan->conn_state))
			l2cap_send_ack(chan);
		goto drop;
	}

	if (tx_seq == chan->expected_tx_seq)
		goto expected;

	if (test_bit(CONN_SREJ_SENT, &chan->conn_state)) {
		struct srej_list *first;

		first = list_first_entry(&chan->srej_l,
				struct srej_list, list);
		if (tx_seq == first->tx_seq) {
			l2cap_add_to_srej_queue(chan, skb, tx_seq, sar);
			l2cap_check_srej_gap(chan, tx_seq);

			list_del(&first->list);
			kfree(first);

			if (list_empty(&chan->srej_l)) {
				chan->buffer_seq = chan->buffer_seq_srej;
				clear_bit(CONN_SREJ_SENT, &chan->conn_state);
				l2cap_send_ack(chan);
				BT_DBG("chan %p, Exit SREJ_SENT", chan);
			}
		} else {
			struct srej_list *l;

			/* duplicated tx_seq */
			if (l2cap_add_to_srej_queue(chan, skb, tx_seq, sar) < 0)
				goto drop;

			list_for_each_entry(l, &chan->srej_l, list) {
				if (l->tx_seq == tx_seq) {
					l2cap_resend_srejframe(chan, tx_seq);
					return 0;
				}
			}

			err = l2cap_send_srejframe(chan, tx_seq);
			if (err < 0) {
				l2cap_send_disconn_req(chan->conn, chan, -err);
				return err;
			}
		}
	} else {
		expected_tx_seq_offset = __seq_offset(chan,
				chan->expected_tx_seq, chan->buffer_seq);

		/* duplicated tx_seq */
		if (tx_seq_offset < expected_tx_seq_offset)
			goto drop;

		set_bit(CONN_SREJ_SENT, &chan->conn_state);

		BT_DBG("chan %p, Enter SREJ", chan);

		INIT_LIST_HEAD(&chan->srej_l);
		chan->buffer_seq_srej = chan->buffer_seq;

		__skb_queue_head_init(&chan->srej_q);
		l2cap_add_to_srej_queue(chan, skb, tx_seq, sar);

		/* Set P-bit only if there are some I-frames to ack. */
		if (__clear_ack_timer(chan))
			set_bit(CONN_SEND_PBIT, &chan->conn_state);

		err = l2cap_send_srejframe(chan, tx_seq);
		if (err < 0) {
			l2cap_send_disconn_req(chan->conn, chan, -err);
			return err;
		}
	}
	return 0;

expected:
	chan->expected_tx_seq = __next_seq(chan, chan->expected_tx_seq);

	if (test_bit(CONN_SREJ_SENT, &chan->conn_state)) {
		bt_cb(skb)->tx_seq = tx_seq;
		bt_cb(skb)->sar = sar;
		__skb_queue_tail(&chan->srej_q, skb);
		return 0;
	}

	err = l2cap_reassemble_sdu(chan, skb, rx_control);
	chan->buffer_seq = __next_seq(chan, chan->buffer_seq);

	if (err < 0) {
		l2cap_send_disconn_req(chan->conn, chan, ECONNRESET);
		return err;
	}

	if (__is_ctrl_final(chan, rx_control)) {
		if (!test_and_clear_bit(CONN_REJ_ACT, &chan->conn_state))
			l2cap_retransmit_frames(chan);
	}


	chan->num_acked = (chan->num_acked + 1) % num_to_ack;
	if (chan->num_acked == num_to_ack - 1)
		l2cap_send_ack(chan);
	else
		__set_ack_timer(chan);

	return 0;

drop:
	kfree_skb(skb);
	return 0;
}

static inline void l2cap_data_channel_rrframe(struct l2cap_chan *chan, u32 rx_control)
{
	BT_DBG("chan %p, req_seq %d ctrl 0x%8.8x", chan,
				__get_reqseq(chan, rx_control), rx_control);

	chan->expected_ack_seq = __get_reqseq(chan, rx_control);
	l2cap_drop_acked_frames(chan);

	if (__is_ctrl_poll(chan, rx_control)) {
		set_bit(CONN_SEND_FBIT, &chan->conn_state);
		if (test_bit(CONN_SREJ_SENT, &chan->conn_state)) {
			if (test_bit(CONN_REMOTE_BUSY, &chan->conn_state) &&
					(chan->unacked_frames > 0))
				__set_retrans_timer(chan);

			clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);
			l2cap_send_srejtail(chan);
		} else {
			l2cap_send_i_or_rr_or_rnr(chan);
		}

	} else if (__is_ctrl_final(chan, rx_control)) {
		clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);

		if (!test_and_clear_bit(CONN_REJ_ACT, &chan->conn_state))
			l2cap_retransmit_frames(chan);

	} else {
		if (test_bit(CONN_REMOTE_BUSY, &chan->conn_state) &&
				(chan->unacked_frames > 0))
			__set_retrans_timer(chan);

		clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);
		if (test_bit(CONN_SREJ_SENT, &chan->conn_state))
			l2cap_send_ack(chan);
		else
			l2cap_ertm_send(chan);
	}
}

static inline void l2cap_data_channel_rejframe(struct l2cap_chan *chan, u32 rx_control)
{
	u16 tx_seq = __get_reqseq(chan, rx_control);

	BT_DBG("chan %p, req_seq %d ctrl 0x%8.8x", chan, tx_seq, rx_control);

	clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);

	chan->expected_ack_seq = tx_seq;
	l2cap_drop_acked_frames(chan);

	if (__is_ctrl_final(chan, rx_control)) {
		if (!test_and_clear_bit(CONN_REJ_ACT, &chan->conn_state))
			l2cap_retransmit_frames(chan);
	} else {
		l2cap_retransmit_frames(chan);

		if (test_bit(CONN_WAIT_F, &chan->conn_state))
			set_bit(CONN_REJ_ACT, &chan->conn_state);
	}
}
static inline void l2cap_data_channel_srejframe(struct l2cap_chan *chan, u32 rx_control)
{
	u16 tx_seq = __get_reqseq(chan, rx_control);

	BT_DBG("chan %p, req_seq %d ctrl 0x%8.8x", chan, tx_seq, rx_control);

	clear_bit(CONN_REMOTE_BUSY, &chan->conn_state);

	if (__is_ctrl_poll(chan, rx_control)) {
		chan->expected_ack_seq = tx_seq;
		l2cap_drop_acked_frames(chan);

		set_bit(CONN_SEND_FBIT, &chan->conn_state);
		l2cap_retransmit_one_frame(chan, tx_seq);

		l2cap_ertm_send(chan);

		if (test_bit(CONN_WAIT_F, &chan->conn_state)) {
			chan->srej_save_reqseq = tx_seq;
			set_bit(CONN_SREJ_ACT, &chan->conn_state);
		}
	} else if (__is_ctrl_final(chan, rx_control)) {
		if (test_bit(CONN_SREJ_ACT, &chan->conn_state) &&
				chan->srej_save_reqseq == tx_seq)
			clear_bit(CONN_SREJ_ACT, &chan->conn_state);
		else
			l2cap_retransmit_one_frame(chan, tx_seq);
	} else {
		l2cap_retransmit_one_frame(chan, tx_seq);
		if (test_bit(CONN_WAIT_F, &chan->conn_state)) {
			chan->srej_save_reqseq = tx_seq;
			set_bit(CONN_SREJ_ACT, &chan->conn_state);
		}
	}
}

static inline void l2cap_data_channel_rnrframe(struct l2cap_chan *chan, u32 rx_control)
{
	u16 tx_seq = __get_reqseq(chan, rx_control);

	BT_DBG("chan %p, req_seq %d ctrl 0x%8.8x", chan, tx_seq, rx_control);

	set_bit(CONN_REMOTE_BUSY, &chan->conn_state);
	chan->expected_ack_seq = tx_seq;
	l2cap_drop_acked_frames(chan);

	if (__is_ctrl_poll(chan, rx_control))
		set_bit(CONN_SEND_FBIT, &chan->conn_state);

	if (!test_bit(CONN_SREJ_SENT, &chan->conn_state)) {
		__clear_retrans_timer(chan);
		if (__is_ctrl_poll(chan, rx_control))
			l2cap_send_rr_or_rnr(chan, L2CAP_CTRL_FINAL);
		return;
	}

	if (__is_ctrl_poll(chan, rx_control)) {
		l2cap_send_srejtail(chan);
	} else {
		rx_control = __set_ctrl_super(chan, L2CAP_SUPER_RR);
		l2cap_send_sframe(chan, rx_control);
	}
}

static inline int l2cap_data_channel_sframe(struct l2cap_chan *chan, u32 rx_control, struct sk_buff *skb)
{
	BT_DBG("chan %p rx_control 0x%8.8x len %d", chan, rx_control, skb->len);

	if (__is_ctrl_final(chan, rx_control) &&
			test_bit(CONN_WAIT_F, &chan->conn_state)) {
		__clear_monitor_timer(chan);
		if (chan->unacked_frames > 0)
			__set_retrans_timer(chan);
		clear_bit(CONN_WAIT_F, &chan->conn_state);
	}

	switch (__get_ctrl_super(chan, rx_control)) {
	case L2CAP_SUPER_RR:
		l2cap_data_channel_rrframe(chan, rx_control);
		break;

	case L2CAP_SUPER_REJ:
		l2cap_data_channel_rejframe(chan, rx_control);
		break;

	case L2CAP_SUPER_SREJ:
		l2cap_data_channel_srejframe(chan, rx_control);
		break;

	case L2CAP_SUPER_RNR:
		l2cap_data_channel_rnrframe(chan, rx_control);
		break;
	}

	kfree_skb(skb);
	return 0;
}

static int l2cap_ertm_data_rcv(struct l2cap_chan *chan, struct sk_buff *skb)
{
	u32 control;
	u16 req_seq;
	int len, next_tx_seq_offset, req_seq_offset;

	control = __get_control(chan, skb->data);
	skb_pull(skb, __ctrl_size(chan));
	len = skb->len;

	/*
	 * We can just drop the corrupted I-frame here.
	 * Receiver will miss it and start proper recovery
	 * procedures and ask retransmission.
	 */
	if (l2cap_check_fcs(chan, skb))
		goto drop;

	if (__is_sar_start(chan, control) && !__is_sframe(chan, control))
		len -= L2CAP_SDULEN_SIZE;

	if (chan->fcs == L2CAP_FCS_CRC16)
		len -= L2CAP_FCS_SIZE;

	if (len > chan->mps) {
		l2cap_send_disconn_req(chan->conn, chan, ECONNRESET);
		goto drop;
	}

	req_seq = __get_reqseq(chan, control);

	req_seq_offset = __seq_offset(chan, req_seq, chan->expected_ack_seq);

	next_tx_seq_offset = __seq_offset(chan, chan->next_tx_seq,
						chan->expected_ack_seq);

	/* check for invalid req-seq */
	if (req_seq_offset > next_tx_seq_offset) {
		l2cap_send_disconn_req(chan->conn, chan, ECONNRESET);
		goto drop;
	}

	if (!__is_sframe(chan, control)) {
		if (len < 0) {
			l2cap_send_disconn_req(chan->conn, chan, ECONNRESET);
			goto drop;
		}

		l2cap_data_channel_iframe(chan, control, skb);
	} else {
		if (len != 0) {
			BT_ERR("%d", len);
			l2cap_send_disconn_req(chan->conn, chan, ECONNRESET);
			goto drop;
		}

		l2cap_data_channel_sframe(chan, control, skb);
	}

	return 0;

drop:
	kfree_skb(skb);
	return 0;
}

static inline int l2cap_data_channel(struct l2cap_conn *conn, u16 cid, struct sk_buff *skb)
{
	struct l2cap_chan *chan;
	u32 control;
	u16 tx_seq;
	int len;

	chan = l2cap_get_chan_by_scid(conn, cid);
	if (!chan) {
		BT_DBG("unknown cid 0x%4.4x", cid);
		/* Drop packet and return */
		kfree_skb(skb);
		return 0;
	}

	l2cap_chan_lock(chan);

	BT_DBG("chan %p, len %d", chan, skb->len);

	if (chan->state != BT_CONNECTED)
		goto drop;

	switch (chan->mode) {
>>>>>>> remotes/linux2/linux-3.4.y
	case L2CAP_MODE_BASIC:
		/* If socket recv buffers overflows we drop data here
		 * which is *bad* because L2CAP has to be reliable.
		 * But we don't have any other choice. L2CAP doesn't
		 * provide flow control mechanism. */

<<<<<<< HEAD
		if (pi->imtu < skb->len)
			goto drop;

		if (!sock_queue_rcv_skb(sk, skb))
=======
		if (chan->imtu < skb->len)
			goto drop;

		if (!chan->ops->recv(chan->data, skb))
>>>>>>> remotes/linux2/linux-3.4.y
			goto done;
		break;

	case L2CAP_MODE_ERTM:
<<<<<<< HEAD
	case L2CAP_MODE_STREAMING:
		control = &bt_cb(skb)->control;
		if (pi->extended_control) {
			__get_extended_control(get_unaligned_le32(skb->data),
						control);
			skb_pull(skb, 4);
		} else {
			__get_enhanced_control(get_unaligned_le16(skb->data),
						control);
			skb_pull(skb, 2);
		}

		len = skb->len;

		if (l2cap_check_fcs(pi, skb))
			goto drop;

		if ((control->frame_type == 'i') &&
			(control->sar == L2CAP_SAR_START))
			len -= 2;

		if (pi->fcs == L2CAP_FCS_CRC16)
			len -= 2;

		/*
		 * We can just drop the corrupted I-frame here.
		 * Receiver will miss it and start proper recovery
		 * procedures and ask for retransmission.
		 */
		if (len > pi->mps) {
			l2cap_send_disconn_req(pi->conn, sk, ECONNRESET);
			goto drop;
		}

		if (control->frame_type == 'i') {

			int err;

			BT_DBG("iframe sar %d, reqseq %d, final %d, txseq %d",
				control->sar, control->reqseq, control->final,
				control->txseq);

			/* Validate F-bit - F=0 always valid, F=1 only
			 * valid in TX WAIT_F
			 */
			if (control->final && (pi->tx_state !=
					L2CAP_ERTM_TX_STATE_WAIT_F))
				goto drop;

			if (pi->mode != L2CAP_MODE_STREAMING) {
				event = L2CAP_ERTM_EVENT_RECV_IFRAME;
				err = l2cap_ertm_rx(sk, control, skb, event);
			} else
				err = l2cap_strm_rx(sk, control, skb);
			if (err)
				l2cap_send_disconn_req(pi->conn, sk,
						ECONNRESET);
		} else {
			/* Only I-frames are expected in streaming mode */
			if (pi->mode == L2CAP_MODE_STREAMING)
				goto drop;

			BT_DBG("sframe reqseq %d, final %d, poll %d, super %d",
				control->reqseq, control->final, control->poll,
				control->super);

			if (len != 0) {
				l2cap_send_disconn_req(pi->conn, sk,
						ECONNRESET);
				goto drop;
			}

			/* Validate F and P bits */
			if (control->final &&
				((pi->tx_state != L2CAP_ERTM_TX_STATE_WAIT_F)
					|| control->poll))
				goto drop;

			event = l2cap_ertm_rx_func_to_event[control->super];
			if (l2cap_ertm_rx(sk, control, skb, event))
				l2cap_send_disconn_req(pi->conn, sk,
						ECONNRESET);
		}
=======
		l2cap_ertm_data_rcv(chan, skb);

		goto done;

	case L2CAP_MODE_STREAMING:
		control = __get_control(chan, skb->data);
		skb_pull(skb, __ctrl_size(chan));
		len = skb->len;

		if (l2cap_check_fcs(chan, skb))
			goto drop;

		if (__is_sar_start(chan, control))
			len -= L2CAP_SDULEN_SIZE;

		if (chan->fcs == L2CAP_FCS_CRC16)
			len -= L2CAP_FCS_SIZE;

		if (len > chan->mps || len < 0 || __is_sframe(chan, control))
			goto drop;

		tx_seq = __get_txseq(chan, control);

		if (chan->expected_tx_seq != tx_seq) {
			/* Frame(s) missing - must discard partial SDU */
			kfree_skb(chan->sdu);
			chan->sdu = NULL;
			chan->sdu_last_frag = NULL;
			chan->sdu_len = 0;

			/* TODO: Notify userland of missing data */
		}

		chan->expected_tx_seq = __next_seq(chan, tx_seq);

		if (l2cap_reassemble_sdu(chan, skb, control) == -EMSGSIZE)
			l2cap_send_disconn_req(chan->conn, chan, ECONNRESET);
>>>>>>> remotes/linux2/linux-3.4.y

		goto done;

	default:
<<<<<<< HEAD
		BT_DBG("sk %p: bad mode 0x%2.2x", sk, pi->mode);
=======
		BT_DBG("chan %p: bad mode 0x%2.2x", chan, chan->mode);
>>>>>>> remotes/linux2/linux-3.4.y
		break;
	}

drop:
	kfree_skb(skb);

done:
<<<<<<< HEAD
	return 0;
}

void l2cap_recv_deferred_frame(struct sock *sk, struct sk_buff *skb)
{
	lock_sock(sk);
	l2cap_data_channel(sk, skb);
	release_sock(sk);
=======
	l2cap_chan_unlock(chan);

	return 0;
>>>>>>> remotes/linux2/linux-3.4.y
}

static inline int l2cap_conless_channel(struct l2cap_conn *conn, __le16 psm, struct sk_buff *skb)
{
<<<<<<< HEAD
	struct sock *sk;

	sk = l2cap_get_sock_by_psm(0, psm, conn->src);
	if (!sk)
		goto drop;

	bh_lock_sock(sk);

	BT_DBG("sk %p, len %d", sk, skb->len);

	if (sk->sk_state != BT_BOUND && sk->sk_state != BT_CONNECTED)
		goto drop;

	if (l2cap_pi(sk)->imtu < skb->len)
		goto drop;

	if (!sock_queue_rcv_skb(sk, skb))
		goto done;
=======
	struct l2cap_chan *chan;

	chan = l2cap_global_chan_by_psm(0, psm, conn->src);
	if (!chan)
		goto drop;

	BT_DBG("chan %p, len %d", chan, skb->len);

	if (chan->state != BT_BOUND && chan->state != BT_CONNECTED)
		goto drop;

	if (chan->imtu < skb->len)
		goto drop;

	if (!chan->ops->recv(chan->data, skb))
		return 0;
>>>>>>> remotes/linux2/linux-3.4.y

drop:
	kfree_skb(skb);

<<<<<<< HEAD
done:
	if (sk)
		bh_unlock_sock(sk);
	return 0;
}

static inline int l2cap_att_channel(struct l2cap_conn *conn, __le16 cid,
							struct sk_buff *skb)
{
	struct sock *sk = NULL;
	struct sk_buff *skb_rsp;
	struct l2cap_hdr *lh;
	int dir;
	u8 err_rsp[] = {L2CAP_ATT_ERROR, 0x00, 0x00, 0x00,
						L2CAP_ATT_NOT_SUPPORTED};

	if (skb->data[0] == L2CAP_ATT_MTU_REQ) {
		u8 mtu_rsp[] = {L2CAP_ATT_MTU_RSP, 23, 0};

		skb_rsp = bt_skb_alloc(sizeof(mtu_rsp) + L2CAP_HDR_SIZE,
								GFP_ATOMIC);
		if (!skb_rsp)
			goto drop;

		lh = (struct l2cap_hdr *) skb_put(skb_rsp, L2CAP_HDR_SIZE);
		lh->len = cpu_to_le16(sizeof(mtu_rsp));
		lh->cid = cpu_to_le16(L2CAP_CID_LE_DATA);
		memcpy(skb_put(skb_rsp, sizeof(mtu_rsp)), mtu_rsp,
							sizeof(mtu_rsp));
		hci_send_acl(conn->hcon, NULL, skb_rsp, 0);

		goto free_skb;
	}

	dir = (skb->data[0] & L2CAP_ATT_RESPONSE_BIT) ? 0 : 1;

	sk = l2cap_find_sock_by_fixed_cid_and_dir(cid, conn->src,
							conn->dst, dir);

	BT_DBG("sk %p, dir:%d", sk, dir);

	if (!sk)
		goto drop;

	bh_lock_sock(sk);

	BT_DBG("sk %p, len %d", sk, skb->len);

	if (sk->sk_state != BT_BOUND && sk->sk_state != BT_CONNECTED)
		goto drop;

	if (l2cap_pi(sk)->imtu < skb->len)
		goto drop;

	if (!sock_queue_rcv_skb(sk, skb))
		goto done;

drop:
	if (skb->data[0] != L2CAP_ATT_INDICATE)
		goto not_indicate;

	/* If this is an incoming Indication, we are required to confirm */

	skb_rsp = bt_skb_alloc(sizeof(u8) + L2CAP_HDR_SIZE, GFP_ATOMIC);
	if (!skb_rsp)
		goto free_skb;

	lh = (struct l2cap_hdr *) skb_put(skb_rsp, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(sizeof(u8));
	lh->cid = cpu_to_le16(L2CAP_CID_LE_DATA);
	err_rsp[0] = L2CAP_ATT_CONFIRM;
	memcpy(skb_put(skb_rsp, sizeof(u8)), err_rsp, sizeof(u8));
	hci_send_acl(conn->hcon, NULL, skb_rsp, 0);
	goto free_skb;

not_indicate:
	if (skb->data[0] & L2CAP_ATT_RESPONSE_BIT ||
			skb->data[0] == L2CAP_ATT_CONFIRM)
		goto free_skb;

	/* If this is an incoming PDU that requires a response, respond with
	 * a generic error so remote device doesn't hang */

	skb_rsp = bt_skb_alloc(sizeof(err_rsp) + L2CAP_HDR_SIZE, GFP_ATOMIC);
	if (!skb_rsp)
		goto free_skb;

	lh = (struct l2cap_hdr *) skb_put(skb_rsp, L2CAP_HDR_SIZE);
	lh->len = cpu_to_le16(sizeof(err_rsp));
	lh->cid = cpu_to_le16(L2CAP_CID_LE_DATA);
	err_rsp[1] = skb->data[0];
	memcpy(skb_put(skb_rsp, sizeof(err_rsp)), err_rsp, sizeof(err_rsp));
	hci_send_acl(conn->hcon, NULL, skb_rsp, 0);

free_skb:
	kfree_skb(skb);

done:
	if (sk)
		bh_unlock_sock(sk);
=======
	return 0;
}

static inline int l2cap_att_channel(struct l2cap_conn *conn, __le16 cid, struct sk_buff *skb)
{
	struct l2cap_chan *chan;

	chan = l2cap_global_chan_by_scid(0, cid, conn->src);
	if (!chan)
		goto drop;

	BT_DBG("chan %p, len %d", chan, skb->len);

	if (chan->state != BT_BOUND && chan->state != BT_CONNECTED)
		goto drop;

	if (chan->imtu < skb->len)
		goto drop;

	if (!chan->ops->recv(chan->data, skb))
		return 0;

drop:
	kfree_skb(skb);

>>>>>>> remotes/linux2/linux-3.4.y
	return 0;
}

static void l2cap_recv_frame(struct l2cap_conn *conn, struct sk_buff *skb)
{
	struct l2cap_hdr *lh = (void *) skb->data;
<<<<<<< HEAD
	struct sock *sk;
=======
>>>>>>> remotes/linux2/linux-3.4.y
	u16 cid, len;
	__le16 psm;

	skb_pull(skb, L2CAP_HDR_SIZE);
	cid = __le16_to_cpu(lh->cid);
	len = __le16_to_cpu(lh->len);

	if (len != skb->len) {
		kfree_skb(skb);
		return;
	}

	BT_DBG("len %d, cid 0x%4.4x", len, cid);

	switch (cid) {
	case L2CAP_CID_LE_SIGNALING:
	case L2CAP_CID_SIGNALING:
		l2cap_sig_channel(conn, skb);
		break;

	case L2CAP_CID_CONN_LESS:
		psm = get_unaligned_le16(skb->data);
		skb_pull(skb, 2);
		l2cap_conless_channel(conn, psm, skb);
		break;

	case L2CAP_CID_LE_DATA:
		l2cap_att_channel(conn, cid, skb);
		break;

	case L2CAP_CID_SMP:
		if (smp_sig_channel(conn, skb))
<<<<<<< HEAD
			l2cap_conn_del(conn->hcon, EACCES, 0);
		break;

	default:
		sk = l2cap_get_chan_by_scid(&conn->chan_list, cid);
		if (sk) {
			if (sock_owned_by_user(sk)) {
				BT_DBG("backlog sk %p", sk);
				if (sk_add_backlog(sk, skb))
					kfree_skb(skb);
			} else
				l2cap_data_channel(sk, skb);

			bh_unlock_sock(sk);
		} else if ((cid == L2CAP_CID_A2MP) && enable_hs) {
			BT_DBG("A2MP");
			amp_conn_ind(conn->hcon, skb);
		} else {
			BT_DBG("unknown cid 0x%4.4x", cid);
			kfree_skb(skb);
		}

=======
			l2cap_conn_del(conn->hcon, EACCES);
		break;

	default:
		l2cap_data_channel(conn, cid, skb);
>>>>>>> remotes/linux2/linux-3.4.y
		break;
	}
}

/* ---- L2CAP interface with lower layer (HCI) ---- */

<<<<<<< HEAD
static int l2cap_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr, u8 type)
{
	int exact = 0, lm1 = 0, lm2 = 0;
	register struct sock *sk;
	struct hlist_node *node;

	if (type != ACL_LINK)
		return 0;
=======
int l2cap_connect_ind(struct hci_dev *hdev, bdaddr_t *bdaddr)
{
	int exact = 0, lm1 = 0, lm2 = 0;
	struct l2cap_chan *c;
>>>>>>> remotes/linux2/linux-3.4.y

	BT_DBG("hdev %s, bdaddr %s", hdev->name, batostr(bdaddr));

	/* Find listening sockets and check their link_mode */
<<<<<<< HEAD
	read_lock(&l2cap_sk_list.lock);
	sk_for_each(sk, node, &l2cap_sk_list.head) {
		if (sk->sk_state != BT_LISTEN)
=======
	read_lock(&chan_list_lock);
	list_for_each_entry(c, &chan_list, global_l) {
		struct sock *sk = c->sk;

		if (c->state != BT_LISTEN)
>>>>>>> remotes/linux2/linux-3.4.y
			continue;

		if (!bacmp(&bt_sk(sk)->src, &hdev->bdaddr)) {
			lm1 |= HCI_LM_ACCEPT;
<<<<<<< HEAD
			if (l2cap_pi(sk)->role_switch)
=======
			if (test_bit(FLAG_ROLE_SWITCH, &c->flags))
>>>>>>> remotes/linux2/linux-3.4.y
				lm1 |= HCI_LM_MASTER;
			exact++;
		} else if (!bacmp(&bt_sk(sk)->src, BDADDR_ANY)) {
			lm2 |= HCI_LM_ACCEPT;
<<<<<<< HEAD
			if (l2cap_pi(sk)->role_switch)
				lm2 |= HCI_LM_MASTER;
		}
	}
	read_unlock(&l2cap_sk_list.lock);
=======
			if (test_bit(FLAG_ROLE_SWITCH, &c->flags))
				lm2 |= HCI_LM_MASTER;
		}
	}
	read_unlock(&chan_list_lock);
>>>>>>> remotes/linux2/linux-3.4.y

	return exact ? lm1 : lm2;
}

<<<<<<< HEAD
static int l2cap_connect_cfm(struct hci_conn *hcon, u8 status)
=======
int l2cap_connect_cfm(struct hci_conn *hcon, u8 status)
>>>>>>> remotes/linux2/linux-3.4.y
{
	struct l2cap_conn *conn;

	BT_DBG("hcon %p bdaddr %s status %d", hcon, batostr(&hcon->dst), status);

<<<<<<< HEAD
	if (!(hcon->type == ACL_LINK || hcon->type == LE_LINK))
		return -EINVAL;

=======
>>>>>>> remotes/linux2/linux-3.4.y
	if (!status) {
		conn = l2cap_conn_add(hcon, status);
		if (conn)
			l2cap_conn_ready(conn);
	} else
<<<<<<< HEAD
		l2cap_conn_del(hcon, bt_err(status), 0);
=======
		l2cap_conn_del(hcon, bt_to_errno(status));
>>>>>>> remotes/linux2/linux-3.4.y

	return 0;
}

<<<<<<< HEAD
static int l2cap_disconn_ind(struct hci_conn *hcon)
=======
int l2cap_disconn_ind(struct hci_conn *hcon)
>>>>>>> remotes/linux2/linux-3.4.y
{
	struct l2cap_conn *conn = hcon->l2cap_data;

	BT_DBG("hcon %p", hcon);

<<<<<<< HEAD
	if (hcon->type != ACL_LINK || !conn)
		return 0x13;

	return conn->disc_reason;
}

static int l2cap_disconn_cfm(struct hci_conn *hcon, u8 reason, u8 is_process)
{
	BT_DBG("hcon %p reason %d", hcon, reason);

	if (!(hcon->type == ACL_LINK || hcon->type == LE_LINK))
		return -EINVAL;

	l2cap_conn_del(hcon, bt_err(reason), is_process);

	return 0;
}

static inline void l2cap_check_encryption(struct sock *sk, u8 encrypt)
{
	if (sk->sk_type != SOCK_SEQPACKET && sk->sk_type != SOCK_STREAM)
		return;

	if (encrypt == 0x00) {
		if (l2cap_pi(sk)->sec_level == BT_SECURITY_MEDIUM) {
			l2cap_sock_clear_timer(sk);
			l2cap_sock_set_timer(sk, HZ * 5);
		} else if (l2cap_pi(sk)->sec_level == BT_SECURITY_HIGH)
			__l2cap_sock_close(sk, ECONNREFUSED);
	} else {
		if (l2cap_pi(sk)->sec_level == BT_SECURITY_MEDIUM)
			l2cap_sock_clear_timer(sk);
	}
}

static int l2cap_security_cfm(struct hci_conn *hcon, u8 status, u8 encrypt)
{
	struct l2cap_chan_list *l;
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct sock *sk;
	int smp = 0;
=======
	if (!conn)
		return HCI_ERROR_REMOTE_USER_TERM;
	return conn->disc_reason;
}

int l2cap_disconn_cfm(struct hci_conn *hcon, u8 reason)
{
	BT_DBG("hcon %p reason %d", hcon, reason);

	l2cap_conn_del(hcon, bt_to_errno(reason));
	return 0;
}

static inline void l2cap_check_encryption(struct l2cap_chan *chan, u8 encrypt)
{
	if (chan->chan_type != L2CAP_CHAN_CONN_ORIENTED)
		return;

	if (encrypt == 0x00) {
		if (chan->sec_level == BT_SECURITY_MEDIUM) {
			__clear_chan_timer(chan);
			__set_chan_timer(chan, L2CAP_ENC_TIMEOUT);
		} else if (chan->sec_level == BT_SECURITY_HIGH)
			l2cap_chan_close(chan, ECONNREFUSED);
	} else {
		if (chan->sec_level == BT_SECURITY_MEDIUM)
			__clear_chan_timer(chan);
	}
}

int l2cap_security_cfm(struct hci_conn *hcon, u8 status, u8 encrypt)
{
	struct l2cap_conn *conn = hcon->l2cap_data;
	struct l2cap_chan *chan;
>>>>>>> remotes/linux2/linux-3.4.y

	if (!conn)
		return 0;

<<<<<<< HEAD
	l = &conn->chan_list;

	BT_DBG("conn %p", conn);

	read_lock(&l->lock);

	for (sk = l->head; sk; sk = l2cap_pi(sk)->next_c) {
		bh_lock_sock(sk);

		BT_DBG("sk->scid %d", l2cap_pi(sk)->scid);

		if (l2cap_pi(sk)->scid == L2CAP_CID_LE_DATA) {
			if (!status && encrypt) {
				l2cap_pi(sk)->sec_level = hcon->sec_level;
				l2cap_chan_ready(sk);
			}

			smp = 1;
			bh_unlock_sock(sk);
			continue;
		}

		if (l2cap_pi(sk)->conf_state & L2CAP_CONF_CONNECT_PEND) {
			bh_unlock_sock(sk);
			continue;
		}

		if (!status && (sk->sk_state == BT_CONNECTED ||
						sk->sk_state == BT_CONFIG)) {
			l2cap_check_encryption(sk, encrypt);
			bh_unlock_sock(sk);
			continue;
		}

		if (sk->sk_state == BT_CONNECT) {
			if (!status) {
				l2cap_pi(sk)->conf_state |=
						L2CAP_CONF_CONNECT_PEND;
				if ((l2cap_pi(sk)->amp_pref ==
						BT_AMP_POLICY_PREFER_AMP) &&
						enable_hs) {
					amp_create_physical(l2cap_pi(sk)->conn,
								sk);
				} else
					l2cap_send_conn_req(sk);
			} else {
				l2cap_sock_clear_timer(sk);
				l2cap_sock_set_timer(sk, HZ / 10);
			}
		} else if (sk->sk_state == BT_CONNECT2) {
			struct l2cap_conn_rsp rsp;
			__u16 result;

			if (!status) {
				if (l2cap_pi(sk)->amp_id) {
					amp_accept_physical(conn,
						l2cap_pi(sk)->amp_id, sk);
					bh_unlock_sock(sk);
					continue;
				}

				sk->sk_state = BT_CONFIG;
				result = L2CAP_CR_SUCCESS;
			} else {
				sk->sk_state = BT_DISCONN;
				l2cap_sock_set_timer(sk, HZ / 10);
				result = L2CAP_CR_SEC_BLOCK;
			}

			rsp.scid   = cpu_to_le16(l2cap_pi(sk)->dcid);
			rsp.dcid   = cpu_to_le16(l2cap_pi(sk)->scid);
			rsp.result = cpu_to_le16(result);
			rsp.status = cpu_to_le16(L2CAP_CS_NO_INFO);
			l2cap_send_cmd(conn, l2cap_pi(sk)->ident,
					L2CAP_CONN_RSP, sizeof(rsp), &rsp);

			if (!(l2cap_pi(sk)->conf_state & L2CAP_CONF_REQ_SENT) &&
				result == L2CAP_CR_SUCCESS) {
				char buf[128];
				l2cap_pi(sk)->conf_state |= L2CAP_CONF_REQ_SENT;
				l2cap_send_cmd(conn, l2cap_get_ident(conn),
					       L2CAP_CONF_REQ,
					       l2cap_build_conf_req(sk, buf),
					       buf);
				l2cap_pi(sk)->num_conf_req++;
			}
		}

		bh_unlock_sock(sk);
	}

	read_unlock(&l->lock);

	if (smp) {
		del_timer(&hcon->smp_timer);
		smp_link_encrypt_cmplt(conn, status, encrypt);
	}
=======
	BT_DBG("conn %p", conn);

	if (hcon->type == LE_LINK) {
		smp_distribute_keys(conn, 0);
		cancel_delayed_work(&conn->security_timer);
	}

	mutex_lock(&conn->chan_lock);

	list_for_each_entry(chan, &conn->chan_l, list) {
		l2cap_chan_lock(chan);

		BT_DBG("chan->scid %d", chan->scid);

		if (chan->scid == L2CAP_CID_LE_DATA) {
			if (!status && encrypt) {
				chan->sec_level = hcon->sec_level;
				l2cap_chan_ready(chan);
			}

			l2cap_chan_unlock(chan);
			continue;
		}

		if (test_bit(CONF_CONNECT_PEND, &chan->conf_state)) {
			l2cap_chan_unlock(chan);
			continue;
		}

		if (!status && (chan->state == BT_CONNECTED ||
						chan->state == BT_CONFIG)) {
			struct sock *sk = chan->sk;

			bt_sk(sk)->suspended = false;
			sk->sk_state_change(sk);

			l2cap_check_encryption(chan, encrypt);
			l2cap_chan_unlock(chan);
			continue;
		}

		if (chan->state == BT_CONNECT) {
			if (!status) {
				l2cap_send_conn_req(chan);
			} else {
				__clear_chan_timer(chan);
				__set_chan_timer(chan, L2CAP_DISC_TIMEOUT);
			}
		} else if (chan->state == BT_CONNECT2) {
			struct sock *sk = chan->sk;
			struct l2cap_conn_rsp rsp;
			__u16 res, stat;

			lock_sock(sk);

			if (!status) {
				if (bt_sk(sk)->defer_setup) {
					struct sock *parent = bt_sk(sk)->parent;
					res = L2CAP_CR_PEND;
					stat = L2CAP_CS_AUTHOR_PEND;
					if (parent)
						parent->sk_data_ready(parent, 0);
				} else {
					__l2cap_state_change(chan, BT_CONFIG);
					res = L2CAP_CR_SUCCESS;
					stat = L2CAP_CS_NO_INFO;
				}
			} else {
				__l2cap_state_change(chan, BT_DISCONN);
				__set_chan_timer(chan, L2CAP_DISC_TIMEOUT);
				res = L2CAP_CR_SEC_BLOCK;
				stat = L2CAP_CS_NO_INFO;
			}

			release_sock(sk);

			rsp.scid   = cpu_to_le16(chan->dcid);
			rsp.dcid   = cpu_to_le16(chan->scid);
			rsp.result = cpu_to_le16(res);
			rsp.status = cpu_to_le16(stat);
			l2cap_send_cmd(conn, chan->ident, L2CAP_CONN_RSP,
							sizeof(rsp), &rsp);
		}

		l2cap_chan_unlock(chan);
	}

	mutex_unlock(&conn->chan_lock);
>>>>>>> remotes/linux2/linux-3.4.y

	return 0;
}

<<<<<<< HEAD
static int l2cap_recv_acldata(struct hci_conn *hcon, struct sk_buff *skb, u16 flags)
{
	struct l2cap_conn *conn = hcon->l2cap_data;

	if (!conn && hcon->hdev->dev_type != HCI_BREDR)
		goto drop;

=======
int l2cap_recv_acldata(struct hci_conn *hcon, struct sk_buff *skb, u16 flags)
{
	struct l2cap_conn *conn = hcon->l2cap_data;

>>>>>>> remotes/linux2/linux-3.4.y
	if (!conn)
		conn = l2cap_conn_add(hcon, 0);

	if (!conn)
		goto drop;

	BT_DBG("conn %p len %d flags 0x%x", conn, skb->len, flags);

<<<<<<< HEAD
	if (flags & ACL_START) {
		struct l2cap_hdr *hdr;
=======
	if (!(flags & ACL_CONT)) {
		struct l2cap_hdr *hdr;
		struct l2cap_chan *chan;
		u16 cid;
>>>>>>> remotes/linux2/linux-3.4.y
		int len;

		if (conn->rx_len) {
			BT_ERR("Unexpected start frame (len %d)", skb->len);
			kfree_skb(conn->rx_skb);
			conn->rx_skb = NULL;
			conn->rx_len = 0;
			l2cap_conn_unreliable(conn, ECOMM);
		}

		/* Start fragment always begin with Basic L2CAP header */
		if (skb->len < L2CAP_HDR_SIZE) {
			BT_ERR("Frame is too short (len %d)", skb->len);
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

		hdr = (struct l2cap_hdr *) skb->data;
		len = __le16_to_cpu(hdr->len) + L2CAP_HDR_SIZE;
<<<<<<< HEAD
=======
		cid = __le16_to_cpu(hdr->cid);
>>>>>>> remotes/linux2/linux-3.4.y

		if (len == skb->len) {
			/* Complete frame received */
			l2cap_recv_frame(conn, skb);
			return 0;
		}

<<<<<<< HEAD
		if (flags & ACL_CONT) {
			BT_ERR("Complete frame is incomplete "
				"(len %d, expected len %d)",
				skb->len, len);
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

=======
>>>>>>> remotes/linux2/linux-3.4.y
		BT_DBG("Start: total len %d, frag len %d", len, skb->len);

		if (skb->len > len) {
			BT_ERR("Frame is too long (len %d, expected len %d)",
				skb->len, len);
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

<<<<<<< HEAD
=======
		chan = l2cap_get_chan_by_scid(conn, cid);

		if (chan && chan->sk) {
			struct sock *sk = chan->sk;
			lock_sock(sk);

			if (chan->imtu < len - L2CAP_HDR_SIZE) {
				BT_ERR("Frame exceeding recv MTU (len %d, "
							"MTU %d)", len,
							chan->imtu);
				release_sock(sk);
				l2cap_conn_unreliable(conn, ECOMM);
				goto drop;
			}
			release_sock(sk);
		}

>>>>>>> remotes/linux2/linux-3.4.y
		/* Allocate skb for the complete frame (with header) */
		conn->rx_skb = bt_skb_alloc(len, GFP_ATOMIC);
		if (!conn->rx_skb)
			goto drop;

		skb_copy_from_linear_data(skb, skb_put(conn->rx_skb, skb->len),
								skb->len);
		conn->rx_len = len - skb->len;
	} else {
		BT_DBG("Cont: frag len %d (expecting %d)", skb->len, conn->rx_len);

		if (!conn->rx_len) {
			BT_ERR("Unexpected continuation frame (len %d)", skb->len);
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

		if (skb->len > conn->rx_len) {
			BT_ERR("Fragment is too long (len %d, expected %d)",
					skb->len, conn->rx_len);
			kfree_skb(conn->rx_skb);
			conn->rx_skb = NULL;
			conn->rx_len = 0;
			l2cap_conn_unreliable(conn, ECOMM);
			goto drop;
		}

		skb_copy_from_linear_data(skb, skb_put(conn->rx_skb, skb->len),
								skb->len);
		conn->rx_len -= skb->len;

		if (!conn->rx_len) {
			/* Complete frame received */
			l2cap_recv_frame(conn, conn->rx_skb);
			conn->rx_skb = NULL;
		}
	}

drop:
	kfree_skb(skb);
	return 0;
}

<<<<<<< HEAD
static void l2cap_set_acl_flushto(struct hci_conn *hcon, u16 flush_to)
{
	struct hci_cp_write_automatic_flush_timeout flush_tm;
	if (hcon && hcon->hdev) {
		flush_tm.handle = hcon->handle;
		if (flush_to == L2CAP_DEFAULT_FLUSH_TO)
			flush_to = 0;
		flush_tm.timeout = (flush_to < L2CAP_MAX_FLUSH_TO) ?
				flush_to : L2CAP_MAX_FLUSH_TO;
		hci_send_cmd(hcon->hdev,
			HCI_OP_WRITE_AUTOMATIC_FLUSH_TIMEOUT,
			4, &(flush_tm));
	}
}

static u16 l2cap_get_smallest_flushto(struct l2cap_chan_list *l)
{
	int ret_flush_to = L2CAP_DEFAULT_FLUSH_TO;
	struct sock *s;
	for (s = l->head; s; s = l2cap_pi(s)->next_c) {
		if (l2cap_pi(s)->flush_to > 0 &&
				l2cap_pi(s)->flush_to < ret_flush_to)
			ret_flush_to = l2cap_pi(s)->flush_to;
	}
	return ret_flush_to;
}

static int l2cap_debugfs_show(struct seq_file *f, void *p)
{
	struct sock *sk;
	struct hlist_node *node;

	read_lock_bh(&l2cap_sk_list.lock);

	sk_for_each(sk, node, &l2cap_sk_list.head) {
		struct l2cap_pinfo *pi = l2cap_pi(sk);
=======
static int l2cap_debugfs_show(struct seq_file *f, void *p)
{
	struct l2cap_chan *c;

	read_lock(&chan_list_lock);

	list_for_each_entry(c, &chan_list, global_l) {
		struct sock *sk = c->sk;
>>>>>>> remotes/linux2/linux-3.4.y

		seq_printf(f, "%s %s %d %d 0x%4.4x 0x%4.4x %d %d %d %d\n",
					batostr(&bt_sk(sk)->src),
					batostr(&bt_sk(sk)->dst),
<<<<<<< HEAD
					sk->sk_state, __le16_to_cpu(pi->psm),
					pi->scid, pi->dcid,
					pi->imtu, pi->omtu, pi->sec_level,
					pi->mode);
	}

	read_unlock_bh(&l2cap_sk_list.lock);
=======
					c->state, __le16_to_cpu(c->psm),
					c->scid, c->dcid, c->imtu, c->omtu,
					c->sec_level, c->mode);
	}

	read_unlock(&chan_list_lock);
>>>>>>> remotes/linux2/linux-3.4.y

	return 0;
}

static int l2cap_debugfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, l2cap_debugfs_show, inode->i_private);
}

static const struct file_operations l2cap_debugfs_fops = {
	.open		= l2cap_debugfs_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct dentry *l2cap_debugfs;

<<<<<<< HEAD
static struct hci_proto l2cap_hci_proto = {
	.name		= "L2CAP",
	.id		= HCI_PROTO_L2CAP,
	.connect_ind	= l2cap_connect_ind,
	.connect_cfm	= l2cap_connect_cfm,
	.disconn_ind	= l2cap_disconn_ind,
	.disconn_cfm	= l2cap_disconn_cfm,
	.security_cfm	= l2cap_security_cfm,
	.recv_acldata	= l2cap_recv_acldata,
	.create_cfm	= l2cap_create_cfm,
	.modify_cfm	= l2cap_modify_cfm,
	.destroy_cfm	= l2cap_destroy_cfm,
};

=======
>>>>>>> remotes/linux2/linux-3.4.y
int __init l2cap_init(void)
{
	int err;

	err = l2cap_init_sockets();
	if (err < 0)
		return err;

<<<<<<< HEAD
	_l2cap_wq = create_singlethread_workqueue("l2cap");
	if (!_l2cap_wq) {
		err = -ENOMEM;
		goto error;
	}

	err = hci_register_proto(&l2cap_hci_proto);
	if (err < 0) {
		BT_ERR("L2CAP protocol registration failed");
		bt_sock_unregister(BTPROTO_L2CAP);
		goto error;
	}

=======
>>>>>>> remotes/linux2/linux-3.4.y
	if (bt_debugfs) {
		l2cap_debugfs = debugfs_create_file("l2cap", 0444,
					bt_debugfs, NULL, &l2cap_debugfs_fops);
		if (!l2cap_debugfs)
			BT_ERR("Failed to create L2CAP debug file");
	}

<<<<<<< HEAD
	if (amp_init() < 0) {
		BT_ERR("AMP Manager initialization failed");
		goto error;
	}

	return 0;

error:
	destroy_workqueue(_l2cap_wq);
	l2cap_cleanup_sockets();
	return err;
=======
	return 0;
>>>>>>> remotes/linux2/linux-3.4.y
}

void l2cap_exit(void)
{
<<<<<<< HEAD
	amp_exit();

	debugfs_remove(l2cap_debugfs);

	flush_workqueue(_l2cap_wq);
	destroy_workqueue(_l2cap_wq);

	if (hci_unregister_proto(&l2cap_hci_proto) < 0)
		BT_ERR("L2CAP protocol unregistration failed");

=======
	debugfs_remove(l2cap_debugfs);
>>>>>>> remotes/linux2/linux-3.4.y
	l2cap_cleanup_sockets();
}

module_param(disable_ertm, bool, 0644);
MODULE_PARM_DESC(disable_ertm, "Disable enhanced retransmission mode");
<<<<<<< HEAD

module_param(enable_hs, bool, 0644);
MODULE_PARM_DESC(enable_hs, "Enable A2MP protocol");

module_param(enable_reconfig, bool, 0644);
MODULE_PARM_DESC(enable_reconfig, "Enable reconfig after initiating AMP move");
=======
>>>>>>> remotes/linux2/linux-3.4.y
