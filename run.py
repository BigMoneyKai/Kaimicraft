from pathlib import Path
import subprocess

# dir to store build contents
build_dir = Path("build")
exec_file = Path("build/bin/minecraft")

# functions
def clean():
    if build_dir.is_dir():
        subprocess.run(["rm", "-rf", "build"], check=True)

def config():
    subprocess.run(["cmake", "-S", ".", "-B", str(build_dir)], check=True)

def build():
    if not exec_file.is_file():
        subprocess.run(["cmake", "--build", str(build_dir)], check=True)

def run():
    if exec_file.is_file():
        subprocess.run([str(exec_file)], True);


if __name__ == "__main__":
    clean()
    config()
    build()
    run()
