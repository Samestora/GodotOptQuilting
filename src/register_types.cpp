#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

static bool editor_plugin_registered = false;

static void register_classes() {}

static void register_singletons() {}

static void register_editor_plugins() {}

static void unregister_singletons() {}

static void unregister_editor_plugins() {}

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		register_classes();
		register_singletons();
	} else if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
        register_editor_plugins();
    }
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
        unregister_editor_plugins();
    } else if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        unregister_singletons();
    }
}

extern "C" {

GDExtensionBool GDE_EXPORT gdoptquilt_library_init(
		GDExtensionInterfaceGetProcAddress p_get_proc_address,
		GDExtensionClassLibraryPtr p_library,
		GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
	return init_obj.init();
}

} // extern "C"
