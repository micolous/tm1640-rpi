
env = Environment()

lib = env.SharedLibrary(
	target='tm1640',
	source=[
		'src/libtm1640/tm1640.c',
	],
	LIBS=['wiringPi']
)


bin = env.Program(
	target='tm1640',
	source=[
		'src/main.c',
	],
	LIBS=['tm1640'],
	LIBPATH='.'
)

env.Install('/usr/bin', bin)
env.Install('/usr/lib', lib)
env.Install('/usr/include', 'src/libtm1640/tm1640.h')

env.Alias('install', ['/usr/include', '/usr/bin', '/usr/lib'])
