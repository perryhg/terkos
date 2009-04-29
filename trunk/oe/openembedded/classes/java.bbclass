# Defines the commonly used target directories and provides a convenience
# function to install jar files.
#
# All the default directory locations herein resemble locations chosen in
# the Debian distribution.

# Jar location on target
datadir_java ?= ${datadir}/java

# JNI library location on target
libdir_jni ?= ${libdir}/jni

# JVM bundle location on target
libdir_jvm ?= ${libdir}/jvm

STAGING_DATADIR_JAVA ?= ${STAGING_DATADIR}/java
STAGING_LIBDIR_JNI ?= ${STAGING_LIBDIR}/jni
STAGING_LIBDIR_JVM ?= ${STAGING_LIBDIR}/jvm

STAGING_DATADIR_JAVA_NATIVE ?= ${STAGING_DATADIR_NATIVE}/java
STAGING_LIBDIR_JNI_NATIVE ?= ${STAGING_LIBDIR_NATIVE}/jni
STAGING_LIBDIR_JVM_NATIVE ?= ${STAGING_LIBDIR_NATIVE}/jvm

oe_jarinstall() {
  # Purpose: Install a jar file and create all the given symlinks to it.
  # Example:
  # oe_jarinstall foo-1.3.jar foo.jar
  # Installs foo-1.3.jar and creates symlink foo.jar.
  #
  # oe_jarinstall -s foo-1.3.jar foo.jar
  # Installs foo-1.3.jar to staging and creates symlink foo.jar.
  #
  # oe_jarinstall -r foo-1.3.jar foo_1_3.jar foo.jar
  # Installs foo_1_3.jar as foo-1.3.jar and creates a symlink to this.
  #
  dir=${D}${datadir_java}
  destname=""
  while [ "$#" -gt 0 ]; do
    case "$1" in
    -s)
			# put jar files to native staging if this is a -native recipe
			if [ ${PACKAGE_ARCH} = ${BUILD_ARCH} ]; then
	      dir=${STAGING_DATADIR_JAVA_NATIVE}
			else
	      dir=${STAGING_DATADIR_JAVA}
			fi
      ;;
    -r)
      shift
      destname=$1
      ;;
    -*)
      oefatal "oe_jarinstall: unknown option: $1"
      ;;
    *)
      break;
      ;;
    esac
    shift
  done

  jarname=$1
  destname=${destname:-`basename $jarname`}
  shift

  install -d $dir
  install -m 0644 $jarname $dir/$destname

  # Creates symlinks out of the remaining arguments.
  while [ "$#" -gt 0 ]; do
    if [ -e $dir/$1 ]; then
      oewarn "file was in the way. removing:" $dir/$1
      rm $dir/$1
    fi
    ln -s $destname $dir/$1
    shift
  done
}

oe_makeclasspath() {
  # Purpose: Generate a classpath variable from the given Jar file names
  # where the ".jar" has been omitted. The string is stored in the script
  # variable whose name is given in the first argument to this function.
  #
  # oe_makeclasspath cp foo baz bar
  # Stores ${datadir_java}/foo.jar:${datadir_java}/baz.jar:${datadir_java}/bar.jar
  # in variable "cp".
  #
  # oe_makeclasspath bootcp -s foo baz bar
  # Stores ${STAGING_DATADIR_JAVA}/foo.jar:${STAGING_DATADIR_JAVA}/baz.jar:${STAGING_DATADIR_JAVA}/bar.jar
  # in variable "bootcp".
  # 
  # Provide the -s at the beginning otherwise strange things happen.
  # If -s is given the function checks whether the requested jar file exists
  # and exits with an error message if it cannot be found.
  #
  # Note: In order to encourage usage of the DEPENDS variable, the function
  # can accept recipe names. If a recipe has no corresponding Jar file it
  # is ignored. Be careful with recipes where the recipe name is different
  # from the the Jar file name!
  dir=${datadir_java}
  classpath=
  delimiter=
  retval=$1

  shift

  while [ "$#" -gt 0 ]; do
      case "$1" in
          -s)
              # take jar files from native staging if this is a -native recipe
              if [ ${PACKAGE_ARCH} = ${BUILD_ARCH} ]; then
                  dir=${STAGING_DATADIR_JAVA_NATIVE}
              else
                  dir=${STAGING_DATADIR_JAVA}
              fi
              ;;
          -*)
              oefatal "oe_makeclasspath: unknown option: $1"
              ;;
          *)
              file=$dir/$1.jar

              if [ -e $file ]; then
                  classpath=$classpath$delimiter$file
                  delimiter=":"
              fi

          ;;
      esac
      shift
  done

  eval $retval="$classpath"
}

# Creates a simple wrapper script for your Java program.
# The script is written to ${PN} by default. 
#
# Parameters are as follows:
# [options] <output file> <main class> [jar files ...]
#
# Options are
# -o <name> where name is the output file name
#
# It can only take jar files from ${datadir_java}!
oe_java_simple_wrapper() {
  delimiter=
  mainclass=
  classpath=
  output=${PN}

  while [ "$#" -gt 0 ]; do
    case "$1" in
    -o)
      shift
      output=$1
      ;;
    -*)
      oefatal "oe_java_simple_wrapper: unknown option: $1"
      ;;
    *)
      if [ $mainclass ]
      then
        classpath=$classpath$delimiter${datadir_java}/$1
        delimiter=":"
      else
        mainclass=$1
      fi
      ;;
    esac
    shift
  done

  oenote "Creating simple Java wrapper script"
  oenote "Output File: $output"
  oenote "Main Class: $mainclass"
  oenote "Classpath: $classpath"

  echo "#!/bin/sh" > $output
  echo "# This file is autogenerated by the oe_java_simple_wrapper function of OpenEmbedded" >> $output
  echo >> $output
  echo "# You can provide additional VM arguments by setting the VMARGS environment variable." >> $output
  echo "CLASSPATH_ARG=\"-cp $classpath\"" >> $output
  echo >> $output
  echo "MAIN_CLASS=$mainclass" >> $output
  echo >> $output
  echo "# Allows overriding the VM by setting the JAVA environment variable." >> $output
  echo "if [ x\${JAVA} = x ]" >> $output
  echo "then" >> $output
  echo "  JAVA=java" >> $output
  echo "fi" >> $output
  echo >> $output
  echo "exec \${JAVA} \${VMARGS} \${CLASSPATH_ARG} \${MAIN_CLASS} \${@}" >> $output
}
