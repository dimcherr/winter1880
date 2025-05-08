import bpy
import sys

try:
    mesh_names = {}
    for obj in bpy.context.scene.objects:
        if obj.data.name not in mesh_names:
            mesh_names[obj.data.name] = 1
        else:
            mesh_names[obj.data.name] += 1

    for mesh_name in mesh_names:
        print("Mesh {} Count {}".format(mesh_name, mesh_names[mesh_name]))

    for obj in bpy.context.scene.objects:
        mesh_count = mesh_names[obj.data.name]
        if mesh_count < 2:
            print("making copy of {}".format(obj.name))
            copyobj = obj.copy()
            copyobj.location.x += 500
            bpy.context.scene.collection.objects.link(copyobj)
    argv = sys.argv
    args = {
        # Settings from "Remember Export Settings"
        **dict(bpy.context.scene.get('glTF2ExportSettings', {})),
        'export_format': 'GLB',
        'filepath': argv[5],
    }
    bpy.context.preferences.addons['io_scene_gltf2'].preferences.KHR_materials_variants_ui = True
    bpy.ops.export_scene.gltf(**args)
    sys.exit(0)
except Exception as err:
    print(err, file=sys.stderr)
    sys.exit(1)