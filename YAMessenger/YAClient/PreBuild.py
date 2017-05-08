import os, shutil, sys

def copytree(src, dst, symlinks=0):
  print ("copy tree " + src)
  names = os.listdir(src)
  if not os.path.exists(dst):
    os.mkdir(dst)
  for name in names:
    srcname = os.path.join(src, name)
    dstname = os.path.join(dst, name)
    try:
      if symlinks and os.path.islink(srcname):
        linkto = os.readlink(srcname)
        os.symlink(linkto, dstname)
      elif os.path.isdir(srcname):
        copytree(srcname, dstname, symlinks)
      else:
        shutil.copy2(srcname, dstname)
    except (IOError, os.error):
      print ("Can't copy %s to %s: %s", srcname, dstname, str(why))

if __name__ == "__main__":
	if len (sys.argv) > 1:
		currDir = sys.argv[1]
		bin = os.path.join(currDir, "bin")
		release = os.path.join(bin, "release")
		debug = os.path.join(bin, "debug")
		if not os.path.exists(bin):
			os.makedirs(bin)
			os.makedirs(release)
			os.makedirs(debug)
		if len (sys.argv) < 4:
			externals = sys.argv[2]
			print("Copy files")
			src = os.path.join(externals, "Windows", "YAClient", "release")
			copytree(src, release)
			src = os.path.join(externals, "OpenSSL")
			copytree(src, release)
		elif sys.argv[3] == "debug":
			externals = sys.argv[2]
			print("Copy files")
			src = os.path.join(externals, "Windows", "YAClient", "debug")
			copytree(src, debug)
			src = os.path.join(externals, "OpenSSL")
			copytree(src, debug)