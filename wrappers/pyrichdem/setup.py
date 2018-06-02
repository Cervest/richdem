from collections import defaultdict
import setuptools
from setuptools.command.build_ext import build_ext as _build_ext
import glob
import datetime
import subprocess
import re

RICHDEM_COMPILE_TIME = None
RICHDEM_GIT_HASH     = None

#Compiler specific arguments
BUILD_ARGS = {
  'msvc': ['-std=c++11','-g','-fvisibility=hidden','-O3'],
  'gcc':  ['-std=c++11','-g','-fvisibility=hidden','-O3','-Wno-unknown-pragmas'],
  'unix': ['-std=c++11','-g','-fvisibility=hidden','-O3','-Wno-unknown-pragmas']
}

#Magic that hooks compiler specific arguments up with the compiler
class build_ext_compiler_check(_build_ext):
  def build_extensions(self):
    compiler = self.compiler.compiler_type
    print('COMPILER',compiler)
    args     = BUILD_ARGS[compiler]
    for ext in self.extensions:
        ext.extra_compile_args = args
    print('COMPILER ARGUMENTS',ext.extra_compile_args)
    _build_ext.build_extensions(self)

if RICHDEM_GIT_HASH is None:
  try:
    shash = subprocess.Popen(["git log --pretty=format:'%h' -n 1"], shell=True, stderr=subprocess.STDOUT, stdout=subprocess.PIPE).stdout.readlines()[0].decode('utf8').strip()
    sdate = subprocess.Popen(["git log -1 --pretty='%ci'"], shell=True, stderr=subprocess.STDOUT, stdout=subprocess.PIPE).stdout.readlines()[0].decode('utf8').strip()
    if re.match(r'^[0-9a-z]+$', shash) and re.match(r'^[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}.*$', sdate):
      RICHDEM_COMPILE_TIME = sdate
      RICHDEM_GIT_HASH     = shash
  except:
    print("Warning! Could not find RichDEM version. Software will still work, but reproducibility will be compromised.")
    pass

if RICHDEM_GIT_HASH is None:
  RICHDEM_COMPILE_TIME = 'Unknown'
  RICHDEM_GIT_HASH     = 'Unknown'

print("Using RichDEM hash={0}, time={1}".format(RICHDEM_GIT_HASH, RICHDEM_COMPILE_TIME))

ext_modules = [
  setuptools.Extension(
    "_richdem",
    glob.glob('src/*.cpp') + ['lib/richdem/common/random.cpp', 'lib/richdem/richdem.cpp'],
    include_dirs  = ['lib/'],
    language      = 'c++',
    define_macros = [
      ('DOCTEST_CONFIG_DISABLE', None                ),
      ('RICHDEM_COMPILE_TIME',   '"\\"'+RICHDEM_COMPILE_TIME+'\\""'),
      ('RICHDEM_GIT_HASH',       '"\\"'+RICHDEM_GIT_HASH+'\\""'    ),
      ('RICHDEM_LOGGING',        None                )
    ]
  )
]

long_description = """RichDEM is a set of digital elevation model (DEM) hydrologic analysis tools.

RichDEM uses parallel processing and state of the art algorithms to quickly process even very large DEMs.

RichDEM offers a variety of flow metrics, such as D8 and D-infinity.

It can flood or breach depressions, as well as calculate flow accumulation, slopes, curvatures, &c."""


#TODO: https://packaging.python.org/tutorials/distributing-packages/#configuring-your-project
setuptools.setup(
  name              = 'richdem',
  version           = '0.3.2',
  description       = 'High-Performance Terrain Analysis',
  long_description  = long_description,
  url               = 'https://github.com/r-barnes/richdem',
  author            = 'Richard Barnes',
  author_email      = 'rbarnes@umn.edu',
  license           = 'GPLv3',
  packages          = setuptools.find_packages(),
  #scripts           = glob.glob('bin/*'),
  entry_points = {'console_scripts': [
    'rd_depression_filling=richdem.cli:DepressionFilling',
    'rd_breach_depressions=richdem.cli:BreachDepressions',
    'rd_flow_accumulation=richdem.cli:FlowAccumulation',
    'rd_terrain_attribute=richdem.cli:TerrainAttribute',
    'rd_info=richdem.cli:RdInfo',
    'rd_compare=richdem.cli:RdCompare'
  ]},
  ext_modules      = ext_modules,
  cmdclass         = {'build_ext': build_ext_compiler_check},
  keywords         = 'GIS terrain hydrology geomorphology raster',
  #packages        = find_packages(exclude=['contrib', 'docs', 'tests*']),
  install_requires = [
    "numpy>=1.7,<2; python_version > '3.4' or python_version < '3.0'",
    "numpy>=1.7,<1.12; python_version < '3.4' and python_version > '3.0'"
  ],
  # extras_require    = {
  #   ':python_version > "3.4"': [
  #     'numpy>=1.7,<2'
  #   ],
  #   ':python_version < "3.4"': [
  #     'numpy>=1.7,<1.12'
  #   ]
  # },  
  #python_requires  = ' >= 2.6, !=3.0.*, !=3.1.*, !=3.2.*, <4',

  #TODO: https://pypi.python.org/pypi?%3Aaction=list_classifiers
  classifiers      = [
      'Development Status :: 4 - Beta',

      'Environment :: Console',

      'Intended Audience :: Developers',
      'Intended Audience :: End Users/Desktop',
      'Intended Audience :: Education',
      'Intended Audience :: Science/Research',
      'Intended Audience :: Other Audience',

      'License :: OSI Approved :: GNU General Public License v3 (GPLv3)',

      'Natural Language :: English',

      'Topic :: Scientific/Engineering :: GIS',
      'Topic :: Scientific/Engineering :: Information Analysis',
      'Topic :: Scientific/Engineering :: Visualization',
      'Topic :: Software Development :: Libraries',

      # Specify the Python versions you support here. In particular, ensure
      # that you indicate whether you support Python 2, Python 3 or both.
      'Programming Language :: Python :: 2',
      'Programming Language :: Python :: 2.6',
      'Programming Language :: Python :: 2.7',
      'Programming Language :: Python :: 3',
      'Programming Language :: Python :: 3.2',
      'Programming Language :: Python :: 3.3',
      'Programming Language :: Python :: 3.4',
  ]
)
