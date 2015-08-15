/**
 * @file config.h
 * @brief Configuration file code
 * @note Copyright (C) 2011 Richard Cochran <richardcochran@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H

#include <sys/queue.h>

#include "ds.h"
#include "dm.h"
#include "filter.h"
#include "transport.h"
#include "servo.h"
#include "sk.h"

#define MAX_IFNAME_SIZE 108 /* = UNIX_PATH_MAX */

/** Defines a network interface, with PTP options. */
struct interface {
	STAILQ_ENTRY(interface) list;
	char name[MAX_IFNAME_SIZE + 1];
	enum delay_mechanism dm;
	enum transport_type transport;
	struct sk_ts_info ts_info;
	enum tsproc_mode tsproc_mode;
	enum filter_type delay_filter;
};

#define CFG_IGNORE_DM           (1 << 0)
#define CFG_IGNORE_TRANSPORT    (1 << 1)
#define CFG_IGNORE_TIMESTAMPING (1 << 2)
#define CFG_IGNORE_SLAVEONLY    (1 << 3)

struct config {
	/* configuration override */
	int cfg_ignore;

	/* configured interfaces */
	STAILQ_HEAD(interfaces_head, interface) interfaces;

	/* hash of all non-legacy items */
	struct hash *htab;

	/* the rest are legacy fields */
	enum timestamp_type timestamping;
	enum transport_type transport;
	enum delay_mechanism dm;

	struct default_ds dds;
	enum servo_type clock_servo;

	unsigned char *ptp_dst_mac;
	unsigned char *p2p_dst_mac;
	unsigned char *udp6_scope;
	char *uds_address;
};

int config_read(char *name, struct config *cfg);
struct interface *config_create_interface(char *name, struct config *cfg);
void config_init_interface(struct interface *iface, struct config *cfg);
void config_destroy(struct config *cfg);

/* New, hash table based methods: */

int config_init(struct config *cfg);

double config_get_double(struct config *cfg, const char *section,
			 const char *option);

int config_get_int(struct config *cfg, const char *section,
		   const char *option);

int config_set_double(struct config *cfg, const char *option, double val);

int config_set_section_int(struct config *cfg, const char *section,
			   const char *option, int val);

static inline int config_set_int(struct config *cfg,
				 const char *option, int val)
{
	return config_set_section_int(cfg, NULL, option, val);
}

#endif
