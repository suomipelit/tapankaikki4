#! /usr/bin/env python
# encoding: utf-8

import sys

# the following two variables are used by the target "waf dist"
VERSION='4.0.8'
APPNAME='tk'

# these variables are mandatory ('/' are converted automatically)
srcdir = '.'
blddir = '_build_'

def set_options(opt):
	opt.tool_options('compiler_cxx')

# the check_library2 in waf doesn't let us define path unfortunately...
def check_library(conf, name, mandatory=1, uselib='', path=''):
	ck_lib = conf.create_library_configurator()
	if uselib: ck_lib.uselib = uselib
	ck_lib.mandatory = mandatory
	ck_lib.name = name
	ck_lib.path = path
	return ck_lib.run()

def configure(conf):
	# Check for working C++ compiler
	conf.check_tool('compiler_cxx')

	# Check for SDL and GTK libraries
	if not conf.check_pkg2('sdl', version='1.2.9', uselib='SDL', mandatory=0):
		conf.check_cfg2('sdl', uselib='SDL')

	# Check for used additional libraries
	check_library(conf, 'SDL_image', uselib='SDL_image', path=conf.env['LIBPATH_SDL'])
	check_library(conf, 'SDL_mixer', uselib='SDL_mixer', path=conf.env['LIBPATH_SDL'])
	check_library(conf, 'SDL_net', uselib='SDL_net', path=conf.env['LIBPATH_SDL'])
	
	# On Windows native win32 api is used and therefore GTK+ is not needed
	if sys.platform != "win32":
		conf.check_pkg2('gtk+-2.0', version='2.0', uselib='GTK')

	# Define that we want to use the linux header (remove this hack later)
	conf.env['CXXDEFINES'] = '__LINUX__'

	# Remove -Wall flag added by waf because it causes too much warnings
	conf.env['CXXFLAGS'].remove('-Wall')

	# Make default install directory the current directory
	conf.env['PREFIX'] = '.'

def build(bld):
	# Add wscript files in subdirectories
	bld.add_subdirs('src')

