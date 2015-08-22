import os
import sys
joinp = os.path.join
sys.path.insert(0, 'whitgl')
sys.path.insert(0, joinp('whitgl', 'input'))
import build
import platform
import ninja_syntax

def main():
  target = 'doomish'
  srcdir = 'src'
  inputdir = joinp('whitgl', 'input')
  builddir = 'build'
  targetdir = joinp(builddir, 'out')
  if build.plat == 'Darwin':
    packagedir = joinp(targetdir, 'Doomish.app', 'Contents')
    executabledir = joinp(packagedir, 'MacOS')
    data_out = joinp(packagedir, 'Resources', 'data')
  else:
    executabledir = targetdir
    data_out = joinp(targetdir, 'data')
  objdir = joinp(builddir, 'obj')
  libdir = joinp(builddir, 'lib')
  data_in =  'data'
  buildfile = open('build.ninja', 'w')
  n = ninja_syntax.Writer(buildfile)
  cflags = build.cflags + ' -Iwhitgl/inc -Isrc -O2'
  build.rules(n, cflags, build.ldflags)
  if build.plat == 'Windows':
    n.rule('windres', command='windres $in -O coff -o $out', description='windres $out')
  obj = build.walk_src(n, srcdir, objdir)
  if build.plat == 'Windows':
    obj += n.build(joinp(objdir, 'icon.res'), 'windres', joinp(data_in, 'win', 'Doomish.rc'))
  whitgl = [joinp('whitgl','build','lib','whitgl.a')]
  targets = []
  targets += n.build(joinp(executabledir, target), 'link', obj+whitgl)
  n.newline()

  data = build.walk_data(n, data_in, data_out, ['png','ogg','lvl','wav'])

  targets += n.build('data', 'phony', data)
  n.newline()

  targets += build.copy_libs(n, inputdir, executabledir)

  if build.plat == 'Darwin':
    targets += n.build(joinp(packagedir, 'Info.plist'), 'cp', joinp(data_in, 'osx', 'Info.plist'))

  n.build('all', 'phony', targets)
  n.default('all')

if __name__ == '__main__':
  main()
