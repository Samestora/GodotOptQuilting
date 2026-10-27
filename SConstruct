#!/usr/bin/env python
import os
import sys

from methods import print_error


libname = "gdoptquilt"
projectdir = "project"

localEnv = Environment(tools=["default"], PLATFORM="")

# Build profiles can be used to decrease compile times.
# You can either specify "disabled_classes", OR
# explicitly specify "enabled_classes" which disables all other classes.
# Modify the example file as needed and uncomment the line below or
# manually specify the build_profile parameter when running SCons.

# localEnv["build_profile"] = "build_profile.json"

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()

if not (os.path.isdir("godot-cpp") and os.listdir("godot-cpp")):
    print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download godot-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

env = SConscript("godot-cpp/SConstruct", {"api_version": 4.7, "env": env, "customs": customs})

env.Append(CPPPATH=["src/"])

def glob_recursive(root, pattern="*.cpp"):
    files = []
    for dirpath, dirnames, _ in os.walk(root):
        dirnames[:] = [d for d in dirnames if d != "gen"]   # template adds gen/doc_data itself
        files += Glob(os.path.join(dirpath, pattern))
    return files

sources = glob_recursive("src")

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# .dev doesn't inhibit compatibility, so we don't need to key it.
# .universal just means "compatible with all relevant arches" so we don't need to key it.
# --- Standard Godot-CPP Naming Generation ---
# We let SCons run normally so it doesn't crash on internal builders.
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")
lib_filename = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))

# Standard compilation path
scons_target_path = "bin/{}/{}".format(env['platform'], lib_filename)
library = env.SharedLibrary(
    scons_target_path,
    source=sources,
)

# --- Determine Custom Target Name ---
is_double = env.get("precision", "single") == "double"
is_debug = env["target"] in ["editor", "template_debug"]

if is_double and is_debug:
    prefix = "dd_"
elif is_double and not is_debug:
    prefix = "rr_"
elif not is_double and is_debug:
    prefix = "d_"
else:
    prefix = ""

# 2. Determine Architecture Specific Suffix
arch_str = str(env.get("arch", ""))

# Explicit architecture mapping to differentiate arm64, x86_64, rv64, etc.
arch_mapping = {
    "x86_64": "64",
    "x86_32": "32",
    "x86": "32",
    "arm64": "arm64",
    "arm32": "arm32",
    "rv64": "rv64",
    "wasm32": "32"
}

# Fallback pattern matching if an exact match isn't found in the map
if arch_str in arch_mapping:
    arch_suffix = arch_mapping[arch_str]
elif "64" in arch_str:
    arch_suffix = "64"
elif "32" in arch_str:
    arch_suffix = "32"
else:
    arch_suffix = ""

if env["platform"] == "web":
    ext = ".nothreads.wasm" if "nothreads" in env.get("suffix", "") else ".wasm"
else:
    ext = env.subst('$SHLIBSUFFIX')

clean_filename = "{}{}{}{}".format(prefix, libname, arch_suffix, ext)

# --- Post-Build Copy and Renaming Action ---
# Instead of basic installing, we copy and explicitly rename it to the custom filename.
destination_file = "{}/bin/{}/{}".format(projectdir, env["platform"], clean_filename)

# SCons built-in InstallAs handles copying and renaming across all platforms seamlessly
copy = env.InstallAs(destination_file, library)

default_args = [library, copy]
Default(*default_args)
