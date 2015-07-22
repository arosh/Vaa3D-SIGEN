top = '.'
subdirs = 'src test'
cxxflags = ['-std=c++11', '-Wall', '-Wextra', '-Wshadow', '-Woverloaded-virtual', '-Wnon-virtual-dtor', '-g']
linkflags = ['-g']

def options(opt):
    opt.load('compiler_cxx')
    opt.recurse(subdirs)

def configure(conf):
    conf.load('compiler_cxx')
    conf.env.CXXFLAGS += cxxflags
    conf.env.LINKFLAGS += linkflags
    conf.check_cxx(cxxflags=cxxflags)
    conf.check_cxx(linkflags=linkflags)
    conf.check_cxx(lib='boost_system')
    conf.check_cxx(lib='boost_filesystem')
    conf.check_cxx(lib='glog')
    conf.check_cfg(
            package='opencv',
            args='--cflags --libs')
    conf.recurse(subdirs)

def build(bld):
    bld.recurse(subdirs)

# vim: filetype=python
