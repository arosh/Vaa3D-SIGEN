APPNAME = 'vaa3d_sigen'
top = '.'
subdirs = 'src test'


def options(opt):
    opt.load('compiler_cxx')
    opt.recurse(subdirs)


def configure(conf):
    conf.load('compiler_cxx')
    conf.env.append_unique('CXXFLAGS', ['-std=c++11', '-Wall', '-Wextra', '-Wshadow',
                                        '-Woverloaded-virtual', '-Wnon-virtual-dtor', '-g'])
    conf.env.append_unique('LINKFLAGS', ['-g'])
    for f in conf.env.CXXFLAGS:
        conf.check_cxx(cxxflags=f)
    for f in conf.env.LINKFLAGS:
        conf.check_cxx(linkflags=f)
    conf.check_cxx(lib='boost_system')
    conf.check_cxx(lib='boost_filesystem')
    conf.check_cxx(lib='glog')
    conf.check_cfg(package='opencv', args='--cflags --libs')
    conf.recurse(subdirs)


def build(bld):
    bld.recurse(subdirs)

# vim: filetype=python
