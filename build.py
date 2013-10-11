import os
import sys
import subprocess

cc = 'clang' # 'gcc'

buildDir = 'build'
srcDir = 'src'
incDir = 'include'
cFlags = []
testDir = 'tests'

runTarget = 'run'

getFlags = subprocess.Popen(['sdl-config --cflags --libs sdl'], shell=True, stdout=subprocess.PIPE)
out, err = getFlags.communicate()
cFlags.append(out.replace('\n', ' '))

tests = {
	'types/testHashMap': ['types/HashMap.c'],
	'types/testList': ['types/List.c'],
	'types/testPQueue': ['types/PQueue.c'],
}

def get_src(baseDir, ext):
	srcs = []
	for root, subFolders, files in os.walk(baseDir):
		for file in files:
			if file.endswith(ext):
				srcs.append(os.path.join(root, file))
	return srcs

def build(target, srcs):
	buildPath = [
		cc,
		'-Wall -Werror -O2',
		'-o ' + os.path.join(buildDir, target),
		'-I./' + incDir,
		' '.join(cFlags),
		' '.join(srcs)
	]

	buildPath = ' '.join(buildPath)
	print buildPath

	return subprocess.call(buildPath, shell=True)

def make_src():
	ext = '.c'
	srcs = get_src('./' + srcDir, ext)
	build(runTarget, srcs)

def make_tests():
	ext = '.c'
	for test in tests:
		binaryPath = os.path.join(buildDir, test)
		subpath = os.path.dirname(binaryPath)
		if not os.path.exists(subpath):
			os.makedirs(subpath)
		
		srcs = [os.path.join(srcDir, path) for path in tests[test]] + [os.path.join(testDir, test) + ext]
		if build(test, srcs) != 0 or subprocess.call(binaryPath, shell=True) != 0:
			return False
		else:
			print '\n'

	return True

if len(sys.argv) > 1 and sys.argv[1] == 'tests':
	make_tests()
else:
	if make_tests():
		make_src()
	else:
		print 'Tests Failed'
