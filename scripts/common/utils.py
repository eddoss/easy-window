import subprocess
import sys
from pathlib import Path
from typing import Optional


def shell(cmd: str, pwd: Optional[Path] = None) -> int:
    with subprocess.Popen(
            args=cmd,
            stdout=subprocess.PIPE,
            universal_newlines=True,
            cwd=pwd,
            shell=True
    ) as proc:
        while proc.poll() is None:
            if proc.stdout:
                line = proc.stdout.readline()
                if line != "":
                    print(line.rstrip())
            if proc.stderr:
                line = proc.stderr.readline()
                if line != "":
                    print(line.rstrip(), file=sys.stderr)
        return proc.returncode
