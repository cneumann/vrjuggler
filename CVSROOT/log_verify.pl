#!/usr/bin/perl
#
# $Id$

require 5.003;	# This script needs perl5

use File::Find ();
use strict qw(vars);
use vars qw($cwd);

BEGIN {
    require Cwd;
    $cwd = Cwd::cwd();
}

# for the convenience of &wanted calls, including -eval statements:
use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;

sub validUser();

#my $CVSROOT = $ENV{'CVSROOT'} || "/cvsroot/vrjuggler";
my $CVSROOT = "/cvsroot/vrjuggler";

sub print_exit_message {
    print "** NOTE: Add entry to ChangeLog for major changes **\n";
}

sub cleanup_lockfiles (;$) {
    my $dir = shift || $CVSROOT;
    print "Cleaing up garbage CVS locks in $dir...\n";
    File::Find::find({wanted => \&wanted}, "$dir");
}

sub wanted {
    my ($dev,$ino,$mode,$nlink,$uid,$gid);

    /^#cvs.*\z/s &&
    (
        (($dev,$ino,$mode,$nlink,$uid,$gid) = lstat($_)) &&
        -f _
        ||
        -d _
    ) &&
    handle();
}

sub handle {
    if ( -d "$name" ) {
        doexec(0, 'rmdir', '{}');
    }
    else {
        unlink("$name") || warn "$name: $!\n";
    }
}

sub doexec {
    my $ok = shift;
    my(@cmd) = @_;
    for my $word (@cmd)
        { $word =~ s#{}#$name#g }
    if ($ok) {
        my $old = select(STDOUT);
        $| = 1;
        print "@cmd";
        select($old);
        return 0 unless <STDIN> =~ /^y/;
    }
    chdir $cwd; #sigh
    print "@cmd\n";
    system @cmd;
    chdir $File::Find::dir;
    return !$?;
}

my $MAILCMD = "/usr/lib/sendmail -odb -oem -t";

open(CMDFILE, "$ARGV[0]") or die "Could not read $ARGV[0]: $!\n";

my $junk = <CMDFILE>;

chomp $junk;

if ( $junk eq "COMMAND" && validUser() )
{
   print "We have a command in log_verify.pl\n";

   my $addrs = <CMDFILE>;

   open(MAIL, "| $MAILCMD") or die "Please check $MAILCMD.\n";
   print MAIL "To: $addrs\n";
   print MAIL "Subject: COMMANDS\n";

   my(@cmds) = <CMDFILE>;

   print MAIL "-" x 15, " Commands ", "-" x 15, "\n";

   foreach ( @cmds )
   {
      print MAIL "$_\n";
   }

   print MAIL "-" x 15, " Commands ", "-" x 15, "\n";

   my $cmd;
   foreach $cmd ( @cmds )
   {
      print MAIL `$cmd`;
   }

   close(MAIL);

   cleanup_lockfiles();

   exit 1;
}
elsif ( $junk eq "CLEAN" && validUser() )
{
   print "Cleaning up stale lock files\n";

   my(@dirs) = <CMDFILE>;

   foreach $dir ( @dirs )
   {
      chop($dir);
      cleanup_lockfiles("$dir");
   }

   exit 1;
}
else
{
   exit 0;
}

sub validUser ()
{
   my @users = ('patrickh', 'allenb', 'aronb');

   foreach ( @users )
   {
      if ( $_ eq "$ENV{'USER'}" )
      {
         return 1;
      }
   }

   return 0;
}