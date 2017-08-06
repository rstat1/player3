#!/usr/bin/python
import os
import sys
import glob
import shlex
import platform
import subprocess
import urllib, zipfile

from shutil import make_archive, rmtree
from distutils.dir_util import copy_tree

global arch

path = os.path.abspath(os.path.split(__file__)[0])
currentOS = ""
outputDir = ""

script_dir = os.path.dirname(__file__)
# Directory within which we can find the gyp source.
gyp_source_dir = os.path.abspath(os.path.join(script_dir, 'gyp')) #os.path.join(script_dir, 'gyp')

sys.path.insert(0, os.path.join(gyp_source_dir, 'pylib'))
import gyp

if (os.path.isdir(os.getcwd() + "/external") == False):
        print "Please execute this script from the root of your source tree."
        exit(1)

def InitBuildEnv():
        global buildDir, arch, currentOS#, outputDir
        # GNCmd = "build/gn gen "
        buildDir = os.getcwd()
        arch = "x86_64"
        outputDir = "Debug_x64"
        if platform.system() == "Linux":
                if ("STEAMLINK_SDK_PATH" in os.environ):
                        arch = "armv7l"
                        outputDir = "Debug-armv7l"
                        SetSteamLinkEnvVars()
                currentOS = "linux"
        elif platform.system() == "Windows":
                currentOS = "win"

        if (os.path.exists("external/libs/" + currentOS + "/" + arch ) == False):
                print("Fetching " + arch + " " + currentOS + " deps")
                urllib.urlretrieve ("http://files.apps.rdro.us/libs-" + currentOS + "-" + arch + ".zip", "external/libs.zip")
                with zipfile.ZipFile('external/libs.zip') as zip:
                        zip.extractall("external")
                os.remove("external/libs.zip")

        # GNCmd += "Debug-" + arch
        RunGyp()
        # subprocess.check_call(GNCmd, shell=True)
def RunGyp():
        gypDefs = 'OS=' + currentOS + ' using_ninja=1 skylight_arch=' + arch
        if arch == "armv7l":
                gypDefs += ' SDLIncludeDir=' + os.environ["MARVELL_ROOTFS"] + 'usr/include/SDL2 SLIncludesDir=' + os.environ["MARVELL_ROOTFS"] + 'usr/include'
        else:
                gypDefs += ' SDLIncludeDir=' + os.getcwd() + '/external/sdl/include SLIncludesDir=/usr/include'

        gypDefs += ' buildPath=' + os.getcwd() + ' renderer_impl=Null with_profiler=0'
        print("Generating build files for " + arch + " on " + currentOS)
        print(outputDir)
        os.environ['GYP_DEFINES'] = gypDefs
        os.environ['GYP_GENERATORS'] = 'ninja'
        args = sys.argv[1:]
        args.append('-I%s/../target_defaults.gypi' % gyp_source_dir)
        args.append('build/streamlink.gyp')
        args.extend(['--depth', '.'])
        args.extend(['--generator-output', os.path.abspath('.')])
        args.extend(['-Goutput_dir=.'])
        gyp.main(args)

def SetSteamLinkEnvVars():
        os.environ["CROSS"] = "armv7a-cros-linux-gnueabi-"
        tcPath = os.environ["STEAMLINK_SDK_PATH"] + "/toolchain/bin"
        sdkBinPath = os.environ["STEAMLINK_SDK_PATH"] + "/bin"
        sdkLibPath = os.environ["STEAMLINK_SDK_PATH"] + "/toolchain/usr/lib/gcc/armv7a-cros-linux-gnueabi/4.9.x-google/"
        currPath = os.environ["PATH"]
        os.environ["PATH"] = sdkBinPath + ":" + tcPath + ":" + currPath
        os.environ["MARVELL_ROOTFS"] = os.environ["STEAMLINK_SDK_PATH"] + "/rootfs"
        os.environ["CROSS_COMPILE"] = os.environ["CROSS"]
        os.environ["AS"] = os.environ["CROSS"] + "as"
        os.environ["CC"] = os.environ["CROSS"] + "gcc --sysroot=" + os.environ["MARVELL_ROOTFS"] + " -marm -mfloat-abi=hard" #-L" + sdkLibPath #+ "/libstdc++.so.6"
        os.environ["CPP"] = os.environ["CROSS"] + "cpp --sysroot=" + os.environ["MARVELL_ROOTFS"] + "-marm -mfloat-abi=hard" #-L" + sdkLibPath
        os.environ["CXX"] = os.environ["CROSS"] + "g++ --sysroot=" + os.environ["MARVELL_ROOTFS"] + " -marm -mfloat-abi=hard" #-L" + sdkLibPath
        os.environ["STRIP"] = os.environ["CROSS"] + "strip"
        os.environ["LC_ALL"] = "C"
        os.environ["LDFLAGS"] = "-L" + sdkLibPath + " -static-libgcc -static-libstdc++"

def MakeACake():
        currentPath = os.getcwd()
        nugetCommand = []
        if platform.system() == "Linux":
                nugetCommand.append("mono")
        os.chdir(buildDir + "/build/tools")
        nugetCommand.extend(["nuget.exe", "install", "-ExcludeVersion"])
        print(nugetCommand)
        subprocess.check_call(nugetCommand)
        os.chdir(currentPath)

def Build():
        global outputDir
        command = ""
        outputDir = "Debug_x64"
        if platform.system() == "Windows":
                command = ['build/ninja', '-C']
                os.chdir(buildDir)
        elif platform.system() == "Linux":
                command = ['build/ninja-linux64', '-C']
                if ("STEAMLINK_SDK_PATH" in os.environ):
                        outputDir = "Debug-armv7l"
        command.append(outputDir)
        print(command)
        subprocess.check_call(command)

def BuildManaged():
        if (os.path.exists("build/tools/Cake/Cake.exe") == False):
                MakeACake()

        skcorelibSLNName = "src/app/managed/streamlink.sln"
        cakeCommand = []
        if platform.system() == "Linux":
                cakeCommand.append("mono")
        cakeCommand.extend(["build/tools/Cake/Cake.exe", "src/app/managed/build.cake", "--root=" + os.getcwd(),
                "--output=" + outputDir])
        subprocess.check_call(cakeCommand)
        if (os.path.exists(outputDir + "/app/libs") == False):
                GetMonoBCL()

def GetMonoBCL():
        print("Get Mono libs")
        urllib.urlretrieve ("http://files.apps.rdro.us/monoBCL-streamlink.zip", outputDir + "/monoBCL.zip")
        with zipfile.ZipFile(outputDir + "/monoBCL.zip") as zip:
                zip.extractall(outputDir + "/")
        os.remove(outputDir + "/monoBCL.zip")

def BuildZipMaker():
        libsCmd = ""
        libsPath = ""
        resourcesCmd = ""
        compiledBitsCmd = ""
        zipDir = buildDir + "/out/"
        resDir = buildDir + "/build/resources/"
        if platform.system() == "Windows":
                roboArgs = " /ndl /njh /np /ns /nc /nfl /njs "
                libsPath = "external/libs/windows/" + arch
                libsCmd = "robocopy" + roboArgs + libsPath + " " + zipDir + " *.dll"
                resourcesCmd = "robocopy" + roboArgs + resDir + " " + zipDir +  " AppManifest.json sl-shell-icon.png"
                compiledBitsCmd = "robocopy" + roboArgs + outputDir + " " + zipDir + " *.exe *.dll"

        elif platform.system() == "Linux":
                libsPath = "external/libs/linux/" + arch + "/"
                libsCmd = "cp "
                resourcesCmd = "cp " + resDir + "AppManifest.json " + resDir + "sl-shell-icon.png " + resDir + "NotoSans-Regular.ttf "
                if (arch == "armv7l"):
                        #libsCmd += libsPath + "libmonoboehm-2.0.so.1 "# + libsPath + "libMonoPosixHelper.so"
                        resourcesCmd += resDir + "streamlink.sh " + resDir + "toc.txt "
                resourcesCmd += resDir + "streamlink_client.config " + zipDir
                compiledBitsCmd = "cp -R " + outputDir + "/streamlink_client " + outputDir + "/lib " + zipDir
                libsCmd += zipDir
        print("Copy managed bits")
        copy_tree(outputDir + "/app/", zipDir + "/app")
        print("Copy resources")
        subprocess.call(resourcesCmd, shell=True)
        print("Copy native bits")
        subprocess.call(compiledBitsCmd, shell=True)
        # if (arch == "armv7l"):
        #         print("Copy native libs")
        #         subprocess.call(libsCmd, shell=True)
        print("Making build zip")
        make_archive("streamlink-dev", "zip", root_dir=zipDir)
        rmtree(zipDir)

def GetSymbols():
        clientSymbolizerCmd = "mono build/tools/Symbolizer.exe " + outputDir + "/player3 " + buildDir + "/syms/"
        subprocess.call(clientSymbolizerCmd, shell=True)
        clientSymbolizerCmd = "mono build/tools/Symbolizer.exe " + outputDir + "/lib/libexternal.so " + buildDir + "/syms/"
        subprocess.call(clientSymbolizerCmd, shell=True)

def GenerateBuildInfo():
        branchName = subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"])
        with open('build/BuildInfo.h.in', 'r') as template:
                data = template.read().replace("##BRANCHNAME##", branchName.strip())
        with open("src/BuildInfo.h", 'w+') as buildInfo:
                buildInfo.write(data)

InitBuildEnv()
GenerateBuildInfo()
os.chdir(buildDir)
Build()
GetSymbols()