This is jsoncpp (http://sourceforge.net/projects/jsoncpp).  I had a hard time building this since I don't
know Python, had never heard of SCons, and the build apparently has some errors in it.  I mean, I hate Makefiles
just as much as the next guy, but come on!  Anyway, here's what I did to build and test it.  

1) Downloaded the HEAD (rev 69) from SVN:

      $ cd ~/terkos/trunk/src
      $ svn export http://jsoncpp.svn.sourceforge.net/svnroot/jsoncpp/trunk/jsoncpp jsoncpp

2) Downloaded and untarred the latest stable version of scons-local (version 1.2.0)

      $ cd ~/terkos/trunk/src/jsoncpp
      $ tar zxvf scons-local-1.2.0.tar.gz

3) Edited the ~/terkos/trunk/src/jsoncpp/Sconstruct file to add a "arm-oe-linux-uclibcgnueabi" target for the vexpro
   and to fix a problem with the runJSONTests_action() function.  I saved the original as SConstruct.old.

4) I then had to hack up the ~/terkos/trunk/src/jsoncpp/test/runjsontests.py file because I kept getting a weird error
   about the usage of glob.glob().  I was tired of mucking around with this crap, so I just hardcoded the list of tests.
   I saved the original version as runjsontests.py.old.  WARNING: If you're going to try to reproduce this on your
   machine, you'll need to fix the absolute paths to match your directory structure.

5) Ran the build by doing the following:

      $ cd ~/terkos/trunk/src/jsoncpp
      $ python scons.py platform=arm-oe-linux-uclibcgnueabi check

   It tries to run all the tests which fail because it's cross compiled.

6) I tested the shared library by copying it, the jsontestrunner binary and all the test/*.json and test/*.expected
   files to the vex and manually ran the tests.  It produced a bunch of *.actual, *.rewrite, and *.actual-rewrite files.
   I compared them and found that they only differed in blank lines so I called it good.

7) I removed all the non-essential generated/uncompressed stuff before SVN checkin, so this directory now only contains
   the jsoncpp stuff, the scons-local .tgz, and the built shared library which I've put in the dist directory.  I
   also renamed the shared library libjsoncpp.so (I felt the previous libjson_arm-oe-linux-uclibcgnueabi-4.2.4_libmt.so
   name was rather unwieldy).