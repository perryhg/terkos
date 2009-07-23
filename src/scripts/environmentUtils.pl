#!/usr/bin/perl

#===================================================================================================
sub setUpLdLibraryPath()
   {
   # We need to set the LD_LIBRARY_PATH environment variable when we're executing as a CGI and we
   # want to call binaries which depend on shared libraries.
   $ENV{'LD_LIBRARY_PATH'} = "/opt/lib:.";
   }
#===================================================================================================
return 1;
