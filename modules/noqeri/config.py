# Copyright (c) 2026 Noethric.
# SPDX-License-Identifier: LicenseRef-Noethric-Proprietary

def can_build(env, platform):
    # The initial native Noqeri integration uses the existing GDScript VM
    # for a deliberately restricted subset of Noqeri's source syntax.
    if not env["module_gdscript_enabled"]:
        return False
    env.module_add_dependencies("noqeri", ["gdscript"])
    return True

def configure(env):
    pass

def get_doc_classes():
    return ["NoqeriScript"]

def get_doc_path():
    return "doc_classes"
