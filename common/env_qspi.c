/*
 * LowLevel function for QSPI Flash environment support
 * Author : Dmitry Konyshev <probables@emcraft.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <command.h>
#include <environment.h>
#include <linux/stddef.h>
#include <search.h>
#include <errno.h>

DECLARE_GLOBAL_DATA_PTR;

int qspi_flash_probe(int swap);
int qspi_flash_erase(unsigned long offset, unsigned int len);
int qspi_flash_write(u32 offset, size_t len, void *buf);
int qspi_flash_read(u32 offset, size_t len, void *buf);

env_t *env_ptr;

char *env_name_spec = "qspi";

static void read_qspi(unsigned long off, unsigned int count, char *buf)
{
	qspi_flash_read(off, count, buf);
}

void env_relocate_spec(void)
{
	char buf[CONFIG_ENV_SIZE];

	qspi_flash_probe(1);
	read_qspi(CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE, buf);

	env_import(buf, 1);
}

#ifdef CONFIG_ENV_OFFSET_REDUND
#error No support for redundant environment on qspi yet!
#endif

int saveenv(void)
{
	env_t	env_new;
	ssize_t	len;
	char	*res;

	res = (char *)&env_new.data;
	len = hexport_r(&env_htab, '\0', &res, ENV_SIZE, 0, NULL);
	if (len < 0) {
		error("Cannot export environment: errno = %d\n", errno);
		return 1;
	}
	env_new.crc = crc32(0, env_new.data, ENV_SIZE);

	qspi_flash_erase(CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE);
	return qspi_flash_write(CONFIG_ENV_OFFSET,
			CONFIG_ENV_SIZE,
			&env_new);
}

int env_init(void)
{
	/* QSPI flash isn't usable before relocation */
	gd->env_addr = (ulong)&default_environment[0];
	gd->env_valid = 1;

	return 0;
}

