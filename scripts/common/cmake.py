import os
import shutil
from typing import *
from pathlib import Path
import platform

from common.paths import PATHS
from common.utils import shell


class CMake:
    class Paths:
        def __init__(self):
            self.source: Path = Path()
            self.buildRelease: Path = Path()
            self.buildDebug: Path = Path()
            self.install: Path = Path()

    @property
    def release(self) -> bool:
        return self._release

    @release.setter
    def release(self, value: bool):
        self._release = bool(value)

    @property
    def debug(self) -> bool:
        return self._debug

    @debug.setter
    def debug(self, value: bool):
        self._debug = bool(value)

    @property
    def configs(self) -> List[str]:
        configs = []
        if self.debug:
            configs.append('Debug')
        if self.release:
            configs.append('Release')
        return configs

    def __init__(self, debug=True, release=True):
        self.paths: CMake.Paths = CMake.Paths()
        self.options: dict[str, Any] = {}
        self._debug = debug
        self._release = release

    def generate(self):
        for config in self.configs:
            S = f'-S {self.paths.source.resolve().as_posix()}'
            build_dir = self.paths.buildRelease
            if config == 'Debug':
                build_dir = self.paths.buildDebug
            B = f'-B {build_dir.resolve().as_posix()}'
            A = ''
            if platform.system().lower() == 'windows':
                G = '-G "Visual Studio 16 2019"'
                A = '-A x64'
            else:
                G = '-G "Unix Makefiles"'

            local_options = self.options.copy()
            local_options['CMAKE_BUILD_TYPE'] = config
            local_options['CMAKE_INSTALL_PREFIX'] = self.paths.install.resolve().as_posix()

            strings = []
            for name, value in local_options.items():
                strings.append(f'-D{name}={value}')

            _D = '\n'.join(strings)
            _cmd = f'cmake\n{G}\n{S}\n{B}\n{A}\n{_D}'
            print(_cmd)

            D = ' '.join(strings)
            cmd = f'cmake {S} {B} {G} {A} {D}'
            shell(cmd)

    def build(self, target='all'):
        for config in self.configs:
            build_dir = self.paths.buildRelease.resolve()
            if config == 'Debug':
                build_dir = self.paths.buildDebug.resolve()
            if not build_dir.exists():
                os.makedirs(build_dir)
            if target == 'all' and platform.system().lower() == 'windows':
                target = 'ALL_BUILD'
            if target == 'install' and platform.system().lower() == 'windows':
                target = 'INSTALL'
            _cmd = f'cmake\n--build {build_dir.as_posix()}\n--target {target}\n--config {config}'
            cmd = f'cmake --build {build_dir.as_posix()} --target {target} --config {config}'
            shell(cmd)

    def install(self):
        if not self.paths.install.exists():
            os.makedirs(self.paths.install)
        self.build('install')

    def clean(self):
        self.build('clean')

    def clear(self):
        if self.paths.buildRelease.exists():
            shutil.rmtree(self.paths.buildRelease)
        if self.paths.buildRelease.exists():
            shutil.rmtree(self.paths.buildRelease)

    def setup(self):
        self.generate()
        self.build('all')
        self.install()


def create(name: str, debug=True, release=True) -> CMake:
    result = CMake(debug, release)
    result.paths.buildRelease = PATHS.build / '3rdparty/Release' / name
    result.paths.buildDebug = PATHS.build / '3rdparty/Debug' / name
    result.paths.source = PATHS.third_party / name
    result.paths.install = PATHS.dependencies
    return result
