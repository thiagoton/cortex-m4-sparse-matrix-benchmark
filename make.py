"""
Unified make script for project
"""
import os
import argparse
import shutil

def build_project(build_type):
    cmake_cmd = ['cmake', '-DCMAKE_TOOLCHAIN_FILE=armgcc.cmake','-G', '"MinGW Makefiles"', '-DCMAKE_BUILD_TYPE=' + build_type, '.']
    os.system(' '.join(cmake_cmd))
    
    if os.name == 'nt':
        os.system('mingw32-make -j')
    else:
        os.system('make -j')

def clean_project():
    TARGETS_TO_CLEAN = {'dirs': ['_deploy', 'CMakeFiles'], 'files': ['CMakeCache.txt', 'Makefile', 'cmake_install.cmake']}
    for dir in TARGETS_TO_CLEAN['dirs']:
        shutil.rmtree(dir, ignore_errors=True)
    for file in TARGETS_TO_CLEAN['files']:
        os.remove(file) if os.path.exists(file) else None

def main():
    BUILD_TYPES = ["debug", "release"]
    parser = argparse.ArgumentParser(description="Make script for the project.")
    parser.add_argument("action", choices=["build", "build_all", "clean"], help="Action to perform: build or clean")
    parser.add_argument("--build-type", choices=BUILD_TYPES, default="release", help=f"Build type (default: %(default)s)")

    args = parser.parse_args()

    if args.action == "build":
        build_project(args.build_type)
    elif args.action == "build_all":
        for build_type in BUILD_TYPES:
            build_project(build_type)
    elif args.action == "clean":
        clean_project()

if __name__ == "__main__":
    main()