# this is for emacs file handling -*- mode: cmake; indent-tabs-mode: nil -*-

# -- BEGIN LICENSE BLOCK ----------------------------------------------
# Copyright (c) 2018, FZI Forschungszentrum Informatik
#
# Redistribution and use in source and binary forms, with or without modification, are permitted
# provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this list of conditions
#    and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of
#    conditions and the following disclaimer in the documentation and/or other materials provided
#    with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors may be used to
#    endorse or promote products derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
# WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# -- END LICENSE BLOCK ------------------------------------------------

#----------------------------------------------------------------------
# \file
#
# \author  Jan Oberlaender <oberlaender@fzi.de>
# \date    2014-08-13
#
# Try to find Eigen3.  Once done, this will define:
#  Eigen3_FOUND:          System has Eigen3
#  Eigen3_INCLUDE_DIRS:   The '-I' preprocessor flags (w/o the '-I')
#  Eigen3_LIBRARY_DIRS:   The paths of the libraries (w/o the '-L')
# Variables defined if pkg-config was employed:
#  Eigen3_DEFINITIONS:    Preprocessor definitions.
#  Eigen3_LIBRARIES:      only the libraries (w/o the '-l')
#  Eigen3_LDFLAGS:        all required linker flags
#  Eigen3_LDFLAGS_OTHER:  all other linker flags
#  Eigen3_CFLAGS:         all required cflags
#  Eigen3_CFLAGS_OTHER:   the other compiler flags
#  Eigen3_VERSION:        version of the module
#  Eigen3_PREFIX:         prefix-directory of the module
#  Eigen3_INCLUDEDIR:     include-dir of the module
#  Eigen3_LIBDIR:         lib-dir of the module
#----------------------------------------------------------------------

include(PrintLibraryStatus)
include(LibFindMacros)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "&{CMAKE_CURRENT_SOURCE_DIR}/../../eigen")
libfind_lib_with_pkg_config(Eigen3 eigen3
  HEADERS Eigen/Core
  HEADER_PATHS "${CMAKE_PREFIX_PATH}" "/usr/include/eigen3" "${CMAKE_INSTALL_PREFIX}/include/eigen3"
  DEFINE _IC_BUILDER_EIGEN_
  )