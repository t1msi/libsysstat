/*
 * saf_misc.c: Functions used by saf to display special records
 * (C) 2011-2025 by Sebastien GODARD (sysstat <at> orange.fr)
 *
 ***************************************************************************
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published  by  the *
 * Free Software Foundation; either version 2 of the License, or (at  your *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it  will  be  useful,  but *
 * WITHOUT ANY WARRANTY; without the implied warranty  of  MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License *
 * for more details.                                                       *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335 USA              *
 ***************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "saf.h"

#ifdef USE_NLS
#include <locale.h>
#include <libintl.h>
#define _(string) gettext(string)
#else
#define _(string) (string)
#endif

extern uint64_t flags;
extern char *seps[];

/*
 ***************************************************************************
 * Display restart messages (JSON format).
 *
 * IN:
 * @tab		Number of tabulations.
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone (unused here).
 * @file_hdr	System activity file standard header.
 * @record_hdr	Current record header (unused here).
 *
 * OUT:
 * @tab		Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_json_restart(int *tab, int action, char *cur_date, char *cur_time,
				    char *my_tz, struct file_header *file_hdr,
				    struct record_header *record_hdr)
{
	static int sep = FALSE;

	if (action & F_BEGIN) {
		printf(",\n");
		xprintf((*tab)++, "\"restarts\": [");
	}
	if (action & F_MAIN) {
		if (sep) {
			printf(",\n");
		}
		xprintf((*tab)++, "{");
		xprintf(*tab, "\"boot\": {\"date\": \"%s\", \"time\": \"%s\", \"tz\": \"%s\", \"cpu_count\": %d}",
			cur_date, cur_time,
			PRINT_LOCAL_TIME(flags) ? my_tz
						: (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									  : "UTC"),
			file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);
		xprintf0(--(*tab), "}");
		sep = TRUE;
	}
	if (action & F_END) {
		if (sep) {
			printf("\n");
			sep = FALSE;
		}
		xprintf0(--(*tab), "]");
	}
}

__printf_funct_t print_json_comment(int *tab, int action, char *cur_date, char *cur_time,
				    char *my_tz, char *comment, struct file_header *file_hdr,
				    struct record_header *record_hdr)
{
	static int sep = FALSE;

	if (action & F_BEGIN) {
		printf(",\n");
		xprintf((*tab)++, "\"comments\": [");
	}
	if (action & F_MAIN) {
		if (sep) {
			printf(",\n");
		}
		xprintf((*tab)++, "{");
		xprintf(*tab,
			"\"comment\": {\"date\": \"%s\", \"time\": \"%s\", "
			"\"tz\": \"%s\", \"com\": \"%s\"}",
			cur_date, cur_time,
			PRINT_LOCAL_TIME(flags) ? my_tz
						: (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									  : "UTC"),
			comment);
		xprintf0(--(*tab), "}");
		sep = TRUE;
	}
	if (action & F_END) {
		if (sep) {
			printf("\n");
			sep = FALSE;
		}
		xprintf0(--(*tab), "]");
	}
}

/*
 ***************************************************************************
 * Display the "statistics" part of the report (JSON format).
 *
 * IN:
 * @tab		Number of tabulations.
 * @action	Action expected from current function.
 * @act		Array of activities (unused here).
 * @id_seq	Activity sequence (unused here).
 *
 * OUT:
 * @tab		Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_json_statistics(int *tab, int action, struct activity *act[],
				       unsigned int id_seq[])
{
	static int sep = FALSE;

	if (action & F_BEGIN) {
		printf(",\n");
		xprintf((*tab)++, "\"statistics\": [");
	}
	if (action & F_MAIN) {
		if (sep) {
			xprintf(--(*tab), "},");
		}
		xprintf((*tab)++, "{");
		sep = TRUE;
	}
	if (action & F_END) {
		if (sep) {
			xprintf(--(*tab), "}");
			sep = FALSE;
		}
		xprintf0(--(*tab), "]");
	}
}

/*
 ***************************************************************************
 * Display the "timestamp" part of the report (JSON format).
 *
 * IN:
 * @parm	Specific parameter. Here: number of tabulations.
 * @action	Action expected from current function.
 * @cur_date	Date string of current comment.
 * @cur_time	Time string of current comment.
 * @my_tz	Current timezone.
 * @itv		Interval of time with preceding record.
 * @record_hdr	Record header for current sample (unused here).
 * @file_hdr	System activity file standard header (unused here).
 * @flags	Flags for common options.
 ***************************************************************************
 */
__tm_funct_t print_json_timestamp(void *parm, int action, char *cur_date,
				  char *cur_time, char *my_tz, unsigned long long itv,
				  struct record_header *record_hdr,
				  struct file_header *file_hdr, unsigned int flags)
{
	int *tab = (int *) parm;

	if (action & F_BEGIN) {
		xprintf0(*tab,
			 "\"timestamp\": {\"date\": \"%s\", \"time\": \"%s\", "
			 "\"tz\": \"%s\", \"interval\": %llu}",
			 cur_date, cur_time,
			 PRINT_LOCAL_TIME(flags) ? my_tz
						 : (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									   : "UTC"),
			itv);
	}
	if (action & F_MAIN) {
		printf(",\n");
	}
	if (action & F_END) {
		printf("\n");
	}

	return NULL;
}

/*
 ***************************************************************************
 * Display the header of the report (JSON format).
 *
 * IN:
 * @parm	Specific parameter. Here: number of tabulations.
 * @action	Action expected from current function.
 * @dfile	Unused here (PCP archive file).
 * @my_tz	Current timezone (unused here).
 * @file_magic	System activity file magic header.
 * @file_hdr	System activity file standard header.
 * @act		Array of activities (unused here).
 * @id_seq	Activity sequence (unused here).
 * @file_actlst	List of (known or unknown) activities in file (unused here).
 *
 * OUT:
 * @parm	Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_json_header(void *parm, int action, char *dfile, char *my_tz,
				   struct file_magic *file_magic,
				   struct file_header *file_hdr,
				   struct activity *act[], unsigned int id_seq[],
				   struct file_activity *file_actlst)
{
	struct tm rectime, loc_t;
	time_t t = file_hdr->sa_ust_time;
	int *tab = (int *) parm;

	if (action & F_BEGIN) {
		char cur_time[TIMESTAMP_LEN];

		xprintf(*tab, "{\"sysstat\": {");

		xprintf(++(*tab), "\"hosts\": [");
		xprintf(++(*tab), "{");
		xprintf(++(*tab), "\"nodename\": \"%s\",", file_hdr->sa_nodename);
		xprintf(*tab, "\"sysname\": \"%s\",", file_hdr->sa_sysname);
		xprintf(*tab, "\"release\": \"%s\",", file_hdr->sa_release);

		xprintf(*tab, "\"machine\": \"%s\",", file_hdr->sa_machine);
		xprintf(*tab, "\"number-of-cpus\": %d,",
			file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);

		/* Fill file timestmap structure (rectime) */
		get_file_timestamp_struct(flags, &rectime, file_hdr);
		strftime(cur_time, sizeof(cur_time), "%Y-%m-%d", &rectime);
		xprintf(*tab, "\"file-date\": \"%s\",", cur_time);

		if (gmtime_r(&t, &loc_t) != NULL) {
			strftime(cur_time, sizeof(cur_time), "%T", &loc_t);
			xprintf(*tab, "\"file-utc-time\": \"%s\",", cur_time);
		}

		xprintf0(*tab, "\"timezone\": \"%s\"", file_hdr->sa_tzname);
	}
	if (action & F_END) {
		printf("\n");
		xprintf(--(*tab), "}");
		xprintf(--(*tab), "]");
		xprintf(--(*tab), "}}");
	}
}

/*
 ***************************************************************************
 * Display data file header.
 *
 * IN:
 * @parm	Specific parameter (unused here).
 * @action	Action expected from current function.
 * @dfile	Name of system activity data file (unused here).
 * @my_tz	Current timezone (unused here).
 * @file_magic	System activity file magic header.
 * @file_hdr	System activity file standard header.
 * @act		Array of activities.
 * @id_seq	Activity sequence.
 * @file_actlst	List of (known or unknown) activities in file.
 ***************************************************************************
 */
__printf_funct_t print_hdr_header(void *parm, int action, char *dfile, char *my_tz,
				  struct file_magic *file_magic,
				  struct file_header *file_hdr,
				  struct activity *act[], unsigned int id_seq[],
				  struct file_activity *file_actlst)
{
	/* Actions F_MAIN and F_END ignored */
	if (action & F_BEGIN) {
		struct tm rectime, loc_t;
		time_t t = file_hdr->sa_ust_time;
		int i, p;
		char cur_time[TIMESTAMP_LEN];
		struct file_activity *fal;

		printf(_("System activity data file: %s (%#x)\n"),
		       dfile, file_magic->format_magic);

		display_sa_file_version(stdout, file_magic);

		if (file_magic->format_magic != FORMAT_MAGIC) {
			return;
		}

		printf(_("Genuine sa datafile: %s (%x)\n"),
		       file_magic->upgraded ? _("no") : _("yes"),
		       file_magic->upgraded);

		printf(_("Host: "));
		print_gal_header(localtime_r(&t, &rectime),
				 file_hdr->sa_sysname, file_hdr->sa_release,
				 file_hdr->sa_nodename, file_hdr->sa_machine,
				 file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1,
				 PLAIN_OUTPUT);

		/* Fill file timestamp structure (rectime) */
		get_file_timestamp_struct(flags, &rectime, file_hdr);
		strftime(cur_time, sizeof(cur_time), "%Y-%m-%d", &rectime);
		printf(_("File date: %s\n"), cur_time);

		if (gmtime_r(&t, &loc_t) != NULL) {
			printf(_("File time: "));
			strftime(cur_time, sizeof(cur_time), "%T", &loc_t);
			printf("%s UTC (%llu)\n", cur_time, file_hdr->sa_ust_time);
		}

		printf(_("Timezone: %s\n"), file_hdr->sa_tzname);

		/* File composition: file_header, file_activity, record_header */
		printf(_("File composition: (%u,%u,%u),(%u,%u,%u),(%u,%u,%u)\n"),
		       file_magic->hdr_types_nr[0], file_magic->hdr_types_nr[1], file_magic->hdr_types_nr[2],
		       file_hdr->act_types_nr[0], file_hdr->act_types_nr[1], file_hdr->act_types_nr[2],
		       file_hdr->rec_types_nr[0], file_hdr->rec_types_nr[1], file_hdr->rec_types_nr[2]);

		printf(_("Size of a long int: %d\n"), file_hdr->sa_sizeof_long);
		printf("HZ = %lu\n", file_hdr->sa_hz);
		printf(_("Number of activities in file: %u\n"),
		       file_hdr->sa_act_nr);
		printf(_("Extra structures available: %c\n"),
		       file_hdr->extra_next ? 'Y' : 'N');

		printf(_("List of activities:\n"));
		fal = file_actlst;
		for (i = 0; i < file_hdr->sa_act_nr; i++, fal++) {

			p = get_activity_position(act, fal->id, RESUME_IF_NOT_FOUND);

			printf("%02u: [%02x] ", fal->id, fal->magic);
			if (p >= 0) {
				printf("%-20s", act[p]->name);
			}
			else {
				printf("%-20s", _("Unknown activity"));
			}
			printf(" %c:%4d", fal->has_nr ? 'Y' : 'N', fal->nr);
			if (fal->nr2 > 1) {
				printf("x%d", fal->nr2);
			}
			printf("\t(%u,%u,%u)", fal->types_nr[0], fal->types_nr[1], fal->types_nr[2]);
			if ((p >= 0) && (act[p]->magic != fal->magic)) {
				printf(_(" \t[Unknown format]"));
			}
			printf("\n");
		}
	}
}

/*
 ***************************************************************************
 * Count the number of new network interfaces in current sample. If a new
 * interface is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new interfaces identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_net_dev(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_net_dev *sndc;

	for (i = 0; i < a->nr[curr]; i++) {
		sndc = (struct stats_net_dev *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list), sndc->interface, MAX_IFACE_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of new network interfaces in current sample. If a new
 * interface is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new interfaces identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_net_edev(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_net_edev *snedc;

	for (i = 0; i < a->nr[curr]; i++) {
		snedc = (struct stats_net_edev *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list), snedc->interface, MAX_IFACE_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of new filesystems in current sample. If a new
 * filesystem is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new filesystems identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_filesystem(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_filesystem *sfc;

	for (i = 0; i < a->nr[curr]; i++) {
		sfc = (struct stats_filesystem *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list),
				    get_fs_name_to_display(a, flags, sfc),
				    MAX_FS_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of new fchosts in current sample. If a new
 * fchost is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new fchosts identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_fchost(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_fchost *sfcc;

	for (i = 0; i < a->nr[curr]; i++) {
		sfcc = (struct stats_fchost *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list), sfcc->fchost_name, MAX_FCH_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of new block devices in current sample. If a new
 * block device is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new block devices identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_disk(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_disk *sdc;

	for (i = 0; i < a->nr[curr]; i++) {
		sdc = (struct stats_disk *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list),
				    get_device_name(sdc->major, sdc->minor, sdc->wwn, sdc->part_nr,
						    DISPLAY_PRETTY(flags), DISPLAY_PERSIST_NAME_S(flags),
						    USE_STABLE_ID(flags), NULL),
				    MAX_DEV_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of interrupts in current sample. Add each interrupt name
 * to the linked list starting at @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of interrupts added to the list.
 ***************************************************************************
 */
__nr_t count_new_int(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_irq *stc_cpuall_irq;

	if (a->item_list)
		/*
		 * If a list already exists, do nothing. This means that a list has been
		 * explicitly entered on the command line using option "--int=", or that
		 * the list has already been created here (remember that the number of
		 * interrupts cannot change in file: @nr2, the second matrix dimension,
		 * is a constant).
		 */
		return 0;

	for (i = 0; i < a->nr2; i++) {
		stc_cpuall_irq = (struct stats_irq *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list), stc_cpuall_irq->irq_name,
				    MAX_SA_IRQ_LEN, NULL);
	}

	return nr;
}

/*
 * **************************************************************************
 * Count the number of new batteries in current sample. If a new
 * battery is found then add it to the linked list starting at
 * @a->item_list.
 * Mainly useful to create a list of battery names (BATx) that will be used
 * as instance names for sadf PCP output format.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new batteries identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_bat(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_pwr_bat *spbc;
	char bat_name[16];

	for (i = 0; i < a->nr[curr]; i++) {
		spbc = (struct stats_pwr_bat *) ((char *) a->buf[curr] + i * a->msize);

		snprintf(bat_name, sizeof(bat_name), "BAT%d", (int) spbc->bat_id);
		nr += add_list_item(&(a->item_list), bat_name, sizeof(bat_name), NULL);
	}

	return nr;
}
