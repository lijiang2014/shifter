/** @file shifter_core.h
 *  @brief Library for setting up and tearing down user-defined images
 *
 *  @author Douglas M. Jacobsen <dmjacobsen@lbl.gov>
 */

/* Shifter, Copyright (c) 2016, The Regents of the University of California,
 * through Lawrence Berkeley National Laboratory (subject to receipt of any
 * required approvals from the U.S. Dept. of Energy).  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of the University of California, Lawrence Berkeley
 *     National Laboratory, U.S. Dept. of Energy nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * See LICENSE for full text.
 */

#ifndef __SHFTR_CORE_INCLUDE
#define __SHFTR_CORE_INCLUDE

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mount.h>


#include "ImageData.h"
#include "UdiRootConfig.h"
#include "VolumeMap.h"
#include "MountList.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_USER UINT_MAX
#define INVALID_GROUP UINT_MAX
#define FILE_SIZE_LIMIT 5242880

int setupUserMounts(VolumeMap *map, UdiRootConfig *udiConfig);
int setupVolumeMapMounts(MountList *mountCache, VolumeMap *map, const char *fromPrefix, const char *toPrefix, dev_t createTo, UdiRootConfig *udiConfig);
int userMountFilter(char *udiRoot, char *filtered_from, char *filtered_to, char *flags);
int isKernelModuleLoaded(const char *name);
int loadKernelModule(const char *name, const char *path, UdiRootConfig *udiConfig);
int mountImageVFS(ImageData *imageData, const char *username, const char *minNodeSpec, UdiRootConfig *udiConfig);
int mountImageLoop(ImageData *imageData, UdiRootConfig *udiConfig);
int loopMount(const char *imagePath, const char *loopMountPath, ImageFormat format, UdiRootConfig *udiConfig, int readonly);
int destructUDI(UdiRootConfig *udiConfig, int killSshd);
int bindImageIntoUDI(const char *relpath, ImageData *imageData, UdiRootConfig *udiConfig, int copyFlag);
int prepareSiteModifications(const char *username, const char *minNodeSpec, UdiRootConfig *udiConfig);
int setupImageSsh(char *sshPubKey, char *username, uid_t uid, gid_t gid, UdiRootConfig *udiConfig);
int startSshd(const char *user, UdiRootConfig *udiConfig);
int filterEtcGroup(const char *dest, const char *from, const char *username, size_t maxGroups);
int remountUdiRootReadonly(UdiRootConfig *udiConfig);
int forkAndExecv(char *const *argvs);
int forkAndExecvSilent(char *const *argvs);
pid_t findSshd(void);
int killSshd(void);
char **parseMounts(size_t *n_mounts);
char *generateShifterConfigString(const char *, ImageData *, VolumeMap *);
int saveShifterConfig(const char *, ImageData *, VolumeMap *, UdiRootConfig *);
int compareShifterConfig(const char *, ImageData*, VolumeMap *, UdiRootConfig *);
int unmountTree(MountList *mounts, const char *base);
int validateUnmounted(const char *path, int subtree);
int isSharedMount(const char *);
int writeHostFile(const char *minNodeSpec, UdiRootConfig *udiConfig);

/** shifter_copyenv
  * copy current process environ into a newly allocated array with newly
  * allocated strings
  *
  * @return copy of the environment, caller is responsible to deal with memory
  */
char **shifter_copyenv(void);
int shifter_putenv(char ***env, char *var);
int shifter_appendenv(char ***env, char *var);
int shifter_prependenv(char ***env, char *var);
int shifter_unsetenv(char ***env, char *var);
int shifter_setupenv(char ***env, ImageData *image, UdiRootConfig *udiConfig);
struct passwd *shifter_getpwuid(uid_t tgtuid, UdiRootConfig *config);
struct passwd *shifter_getpwnam(const char *tgtnam, UdiRootConfig *config);

int setupPerNodeCacheFilename(UdiRootConfig *udiConfig, VolMapPerNodeCacheConfig *, char *, size_t);
int setupPerNodeCacheBackingStore(VolMapPerNodeCacheConfig *cache, const char *from_buffer, UdiRootConfig *udiConfig);
int makeUdiMountPrivate(UdiRootConfig *udiConfig);
char **getSupportedFilesystems();
int supportsFilesystem(char *const * fsTypes, const char *fsType);

#ifdef __cplusplus
}
#endif

#endif
