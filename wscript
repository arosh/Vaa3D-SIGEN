from waflib import Options

APPNAME = 'vaa3d_sigen'
top = '.'
subdirs = 'sigen_core sigen_main test'


def options(opt):
    opt.load('compiler_cxx')
    # If you specify `./waf configure --debug`, debug build will be enabled.
    opt.add_option('--debug', action='store_true')
    opt.recurse(subdirs)


def configure(conf):
    conf.load('compiler_cxx')

    # compiler options
    conf.env.append_unique('CXXFLAGS', ['-std=c++03', '-Wall', '-Wextra', '-Wshadow',
                                        '-Woverloaded-virtual', '-Wnon-virtual-dtor',
                                        '-Wno-c++11-extensions', '-g'])
    conf.env.append_unique('LINKFLAGS', ['-g'])

    # debug options
    if Options.options.debug:
        conf.define('_GLIBCXX_DEBUG', 1)
        for sanitize in ['-fsanitize=address', '-fsanitize=undefined']:
            if conf.check_cxx(cxxflags=sanitize, linkflags=sanitize, mandatory=False):
                conf.env.append_unique('CXXFLAGS', sanitize)
                conf.env.append_unique('LINKFLAGS', sanitize)
    else:
        conf.define('NDEBUG', 1)
        conf.env.append_unique('CXXFLAGS', '-O2')

    # libraries
    conf.check_cxx(lib='boost_system')
    conf.check_cxx(lib='boost_filesystem')
    conf.check_cxx(lib='glog')
    conf.check_cfg(package='opencv', args='--cflags --libs')

    conf.recurse(subdirs)


def build(bld):
    bld.recurse(subdirs)

# vim: filetype=python
