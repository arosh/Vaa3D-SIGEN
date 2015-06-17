top = '.'
subdirs = 'src test'

def options(opt):
    opt.recurse(subdirs)

def configure(conf):
    conf.recurse(subdirs)

def build(bld):
    bld.recurse(subdirs)

# vim: filetype=python
