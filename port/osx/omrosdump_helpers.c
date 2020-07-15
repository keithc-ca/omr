/*******************************************************************************
 * Copyright (c) 2016, 2020 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

/**
 * @file
 * @ingroup Port
 * @brief Dump formatting
 */

#include <sys/mman.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#include "portnls.h"
#include "omrosdump_helpers.h"

static intptr_t waitCore(char *path);

/*
 * @internal
 *
 * Figures out what the system core file name that was generated by the child process will be
 *  according to man core{5}, then renames it to @ref filename.
 *
 *  If @ref filename is an empty string, renameDump writes in the name of the OS-generated core file.
 *
 * @param[in]	portLibrary	The port library
 * @param[in]	filename	User-allocated buffer of size EsMaxPath containing the desired core filename
 * @param[out]	filename	On success, the name of the generated core file, otherwise, the error message
 * @param[in]	pid			pid of process that generated the core file.
 *
 * @return 0 on success, otherwise non-zero and @ref filename contains error message.
 */
uintptr_t
renameDump(struct OMRPortLibrary *portLibrary, char *filename, pid_t pid, int signalNumber)
{
	char tempPath[PATH_MAX] = {'\0'};
	char derivedAbsoluteCorePath[PATH_MAX] = {'\0'};
	intptr_t waitCoreRC = -1;
	char *lastSep = NULL;
	intptr_t renameRC = -1;
	struct stat attrBuf;

	portLibrary->str_printf(portLibrary, derivedAbsoluteCorePath, PATH_MAX, "/cores/core.%i", pid);

	/* Wait until the core file is created or timeout */
	waitCoreRC = waitCore(derivedAbsoluteCorePath);
	if (0 != waitCoreRC) {
		/* The core file does not exist, bail */
		portLibrary->str_printf(portLibrary,
								filename,
								EsMaxPath,
								"The core file created by child process with pid = %i was not found. Expected to find core file with name \"%s\"",
								pid,
								derivedAbsoluteCorePath);
		return 1;
	}

	/* Check that the path we found was to a regular file (not to an existing directory, pipe, symlink etc) */
	if (0 == stat(derivedAbsoluteCorePath, &attrBuf)) {
		if (!S_ISREG(attrBuf.st_mode)) {
			portLibrary->nls_printf(portLibrary, J9NLS_ERROR | J9NLS_STDERR, J9NLS_PORT_DUMP_PATH_EXISTS, derivedAbsoluteCorePath);
			return 1;
		}
	} else {
		portLibrary->str_printf(portLibrary, filename, EsMaxPath, "Unable to read file status for core file path \"%s\"", derivedAbsoluteCorePath);
		return 1;
	}

	/* Rename the file as required by the specified (or default) -Xdump agent file option */
	if ('\0' != filename[0]) {
		renameRC = rename(derivedAbsoluteCorePath, filename);

		if ((0 != renameRC) && (EXDEV == errno)) { /* Failed with 'cross device rename error' */
			memset(tempPath, 0, PATH_MAX);
			/* Retry, leaving the dump directory location unchanged */
			lastSep = strrchr(derivedAbsoluteCorePath, DIR_SEPARATOR);
			if (NULL != lastSep) {
				size_t charsToCopy = lastSep - derivedAbsoluteCorePath + 1;
				strncpy(tempPath, derivedAbsoluteCorePath, charsToCopy);
			}
			/* Using the dump agent requested file name */
			lastSep = strrchr(filename, DIR_SEPARATOR);
			if (NULL != lastSep) {
				strcat(tempPath, lastSep + 1);
			} else {
				strcat(tempPath, filename);
			}
			/* Message warning that the -Xdump option was not fully honoured */
			portLibrary->tty_printf(portLibrary, "Warning: unable to move dump to \"%s\" across file systems (check kernel core_pattern). Using alternate file location \"%s\"\n",
									filename, tempPath);
			/* Copy the new file destination back into the supplied filename for the RAS messages */
			strncpy(filename, tempPath, EsMaxPath);
			renameRC = rename(derivedAbsoluteCorePath, filename);
		}

		if (0 != renameRC) {
			portLibrary->tty_printf(portLibrary, "Attempt to rename \"%s\" to \"%s\" failed with error: %s\n", derivedAbsoluteCorePath, filename, strerror(errno));
			return 1;
		}
	} else {
		/* Filename was empty */
		strncpy(filename, derivedAbsoluteCorePath, EsMaxPath);
	}

	return 0;
}

/**
 * Wait for the core dump file to be written.
 *
 * @param[in] path	Absolute path of the expected core file, null terminated. Maximum length including null is PATH_MAX.
 *
 * @return intptr_t	0 if the file was found, otherwise 1 indicating timeout while looking for the file
 *
 */
static intptr_t
waitCore(char *path)
{
	int rc = 1;
	time_t starttime = time(NULL);

	while (0 != rc) {
		FILE *fd = fopen(path, "r");
		if (NULL == fd) {
			/* Could not open the file. */
			if (5 < difftime(time(NULL), starttime)) {
				/* We waited long enough. */
				break;
			}
			usleep(100000);
		} else {
			/* Opened the file successfully, so we know it exists. Now close it. */
			fclose(fd);
			rc = 0;
		}
	}
	return rc;
}
