from setuptools import setup
# from setuptools.extension import Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "pyfqmr.core",
        ["pyfqmr/Simplify_pyapi.cpp"],
        include_dirs = ['pyfqmr'],
        language='c++',
        # extra_compile_args = ['-O3', '-w'],
        extra_compile_args = ['-fopenmp', '-O3', '-w'],
        extra_link_args = ['-fopenmp'],
    ),                  
]

setup(
    name="pyfqmr",
    ext_modules=ext_modules,
    packages=['pyfqmr'],
    package_dir={'pyfqmr': 'pyfqmr'},
    cmdclass={"build_ext": build_ext},
)