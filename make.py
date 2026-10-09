"""
Unified make script for project
"""
import os
import argparse
import shutil

def build_project(build_type):
    os.mkdir('_build') if not os.path.exists('_build') else None

    os.chdir('_build')
    print(os.getcwd())
    cmake_cmd = ['cmake','-G', '"MinGW Makefiles"', '-DCMAKE_BUILD_TYPE=' + build_type, '..']
    os.system(' '.join(cmake_cmd))
    
    if os.name == 'nt':
        os.system('mingw32-make -j')
    else:
        os.system('make -j')

def clean_project():
    TARGETS_TO_CLEAN = {'dirs': ['_deploy', '_build'], 'files': []}
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