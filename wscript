#! /usr/bin/env python
# encoding: utf-8

# the following two variables are used by the target "waf dist"
VERSION='4.0.8'
APPNAME='tk'

# these variables are mandatory ('/' are converted automatically)
srcdir = '.'
blddir = '_build_'

def set_options(opt):
	opt.tool_options('compiler_cxx')

def configure(conf):
	# Check for working C++ compiler
	conf.check_tool('compiler_cxx')

	# Check for SDL and GTK libraries
	if not conf.check_pkg2('sdl', version='1.2', uselib='SDL', mandatory=0):
		conf.check_cfg2('sdl', uselib='SDL')
	conf.check_pkg2('gtk+-2.0', version='2.0', uselib='GTK')

	# Check for used additional libraries
	conf.check_library2('SDL_image')
	conf.check_library2('SDL_mixer')
	conf.check_library2('SDL_net')
	conf.env['LIB'] = ['SDL_image', 'SDL_mixer', 'SDL_net']

	# Define that we want to use the linux header
	conf.env['CXXDEFINES'] = '__LINUX__'

	# Remove -Wall flag added by waf because it causes too much warnings
	conf.env['CXXFLAGS'].remove('-Wall')

	# Make default install directory the current directory
	conf.env['PREFIX'] = '.'

def build(bld):
	# Add wscript files in subdirectories
	bld.add_subdirs('src')

