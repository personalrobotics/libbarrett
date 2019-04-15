#ifndef __LIBBARRETT_CONFIG_H
#define __LIBBARRETT_CONFIG_H

#include <pwd.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace barrett {
static const std::string EtcPathRelative(const std::string &relpath) {

  std::string path = "/.barrett/";
  char *home;
  struct stat statbuf;

  home = getenv("HOME");
  if (home == NULL)
	home = getpwuid(getuid())->pw_dir;
  path = home + path;
  if (stat(path.c_str(), &statbuf) != -1) {
	if (S_ISDIR(statbuf.st_mode)) {
	  return (path + relpath);
	}
  }
  return std::string("/etc/barrett/") + relpath;
}
}

#endif // ifndef __LIBBARRETT_CONFIG_H
