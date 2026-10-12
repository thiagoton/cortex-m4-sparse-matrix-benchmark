"""
Unified make script for project
"""
import os
import argparse
import shutil
import subprocess

def build_project(build_type):
    build_dir = os.path.join('_build', build_type)
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)
        cmake_cmd = ['cmake', '--preset', build_type]
        os.system(' '.join(cmake_cmd))

    cmake_cmd = ['cmake','--build', '--preset', build_type]
    os.system(' '.join(cmake_cmd))

def build_dsp(build_type):
    project_root = os.path.dirname(os.path.abspath(__file__))
    build_dir = os.path.join(project_root, '_build', 'cmsis-dsp', build_type)
    cache_file = os.path.join(build_dir, 'CMakeCache.txt')

    if not os.path.exists(cache_file):
        os.makedirs(build_dir, exist_ok=True)
        subprocess.run([
            'cmake',
            '-S', os.path.join(project_root, 'cmake', 'cmsis-dsp'),
            '-B', build_dir,
            '-G', 'Ninja',
            f'-DCMAKE_TOOLCHAIN_FILE={os.path.join(project_root, "cmake", "armgcc.cmake")}',
            f'-DCMAKE_BUILD_TYPE={build_type}',
        ], check=True)

    subprocess.run(['cmake', '--build', build_dir], check=True)

def clean_project():
    TARGETS_TO_CLEAN = {'dirs': ['_deploy', '_build'], 'files': []}
    for dir in TARGETS_TO_CLEAN['dirs']:
        shutil.rmtree(dir, ignore_errors=True)
    for file in TARGETS_TO_CLEAN['files']:
        os.remove(file) if os.path.exists(file) else None

def main():
    BUILD_TYPES = ["debug", "release"]
    parser = argparse.ArgumentParser(description="Make script for the project.")
    parser.add_argument("action", choices=["build", "build_dsp", "build_all", "clean"], help="Action to perform")
    parser.add_argument("--build-type", choices=BUILD_TYPES, default="release", help=f"Build type (default: %(default)s)")

    args = parser.parse_args()

    if args.action == "build":
        build_project(args.build_type)
    elif args.action == "build_dsp":
        build_dsp(args.build_type)
    elif args.action == "build_all":
        for build_type in BUILD_TYPES:
            build_project(build_type)
    elif args.action == "clean":
        clean_project()

if __name__ == "__main__":
    main()