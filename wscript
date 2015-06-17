top = '.'

def options(opt):
    opt.load('compiler_cxx unittest_gtest')

def configure(conf):
    conf.load('compiler_cxx unittest_gtest')

def build(bld):
    pass

# vim: filetype=python
