from pathlib import Path


class Paths:
    @property
    def root(self) -> Path:
        return Path(__file__).parent.parent.parent

    @property
    def third_party(self) -> Path:
        return self.root / '3rdparty'

    @property
    def dependencies(self) -> Path:
        return self.root / 'dependencies'

    @property
    def resources(self) -> Path:
        return self.root / 'resources'

    @property
    def build(self) -> Path:
        return self.root / 'build'

    @property
    def develop(self) -> Path:
        return self.root / 'develop'

    @property
    def tools(self) -> Path:
        return self.root / 'tools'


PATHS = Paths()
