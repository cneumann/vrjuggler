# ************** <auto-copyright.pl BEGIN do not edit this line> **************
# Doozer++
#
# Original Author:
#   Patrick Hartling
# -----------------------------------------------------------------------------
# VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          dpp.info.mk,v
# Date modified: 2002/02/23 18:43:05
# Version:       1.1
# -----------------------------------------------------------------
# *************** <auto-copyright.pl END do not edit this line> ***************

# =============================================================================
# dpp.info.mk,v 1.1 2002/02/23 18:43:05 patrickh Exp
#
# This file <dpp.info.mk> is intended to be used by build systems that want to
# give users an easy way to find out about all the possible targets.  It
# defines two targets, 'help' and 'info', that print out the targets of the
# using makefile.  The information printed is denoted using special comment
# syntax.
# -----------------------------------------------------------------------------
# The makefile including this file must define the following variables:
#
# DPP_SCRIPTDIR - The directory containing Doozer++ scripts.  In particular,
#                 genmkinfo.pl is needed.
# SRC_MKFILES   - The file(s) that will be processed by genmkinfo.pl.
# PERL          - The full path to a Perl binary, version 5.004 or newer.
# -----------------------------------------------------------------------------
# The targets defined here are as follows:
#
# help - Print the help information as collected by genmkinfo.pl
# info - Same as 'help'
# =============================================================================

help info:
	@$(PERL) $(DPP_SCRIPTDIR)/genmkinfo.pl $(SRC_MKFILES)
