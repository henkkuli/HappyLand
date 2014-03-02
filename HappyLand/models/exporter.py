import bpy
from struct import pack

def triangulateMesh(mesh):
    import bmesh
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(mesh)
    bm.free()
    
def debug(m):
#    print(m)
    pass

context = bpy.context
scene = context.scene
objects = context.selected_objects

hasMesh = False

# Data to be saved
datVertices = []
datVetexGroups = None
datFaces = []
datBones = []
def getSimilar(datVertex):
    if datVertex not in datVertices:
        datVertices.append(datVertex)
    return datVertices.index(datVertex)

# Go through every selected object
for object in objects:
    if object.type == 'MESH':
        if hasMesh:
            print("Exported object can contain only one mesh")
            break
            # TODO Break
        debug("MESH")
        debug('\tName:' + object.name)
        
        # Get some variables
        mesh = object.to_mesh(scene, True, 'PREVIEW', calc_tessface=False)
        triangulateMesh(mesh)
        faces = mesh.polygons[:]
        vertices = mesh.vertices[:]
        uvLayer = mesh.uv_layers.active
        datVertexGroups = object.vertex_groups
        
        debug('\tVertex groups:')
        for g in object.vertex_groups:
            debug('\t\t' + g.name + ' (' + str(g.index) + ')')
            
        debug('\tVertices:')
        debug('\t\tCount: ' + str(len(vertices)))
        for v in vertices:
            debug('\t\t\t' + str(v.co))
        
        debug('\tUvs:')
        debug('\t\tLength: ' + str(len(mesh.uv_layers.active.data)))
        for f in mesh.uv_layers.active.data:
            debug('\t\t\t' + str(f.uv))
        
        debug('\tFaces:')
        debug('\t\tLength: ' + str(len(faces)))
        for face in faces:
            debug('\t\tFace:')
            newIndices = []
            for i in face.loop_indices:
                l = mesh.loops[i]
                v = vertices[l.vertex_index]
                pos = v.co
                norm = v.normal
                uv = uvLayer.data[l.index].uv
                debug('\t\t\tVertex:')
                debug('\t\t\t\tPosition: ' + str(pos))
                debug('\t\t\t\tNormal:   ' + str(norm))
                debug('\t\t\t\tUv:       ' + str(uv))
                
                debug('\t\t\tGroups:')
                groups = [(g.group, g.weight) for g in v.groups]
                debug('\t\t\t\t' + str(groups))
                if len(groups) > 2:
                    raise Exception('One vertex should have maximum two groups attached')
                while len(groups) < 2:
                    groups.append([0, 0])
                
                newIndices.append(getSimilar((pos, norm, uv, groups)))
            
            # Flip the order of vertices
            newIndices[0], newIndices[1] = newIndices[1], newIndices[0]
            
            datFaces.append(newIndices)
            
        hasMesh = True
    elif object.type == 'ARMATURE':
        print("ARMATURE")
        root = object.data.bones['ROOT']
        
        def goThrough(bone, parentIndex):
            datBones.append((parentIndex, bone))
            parentIndex = len(datBones) - 1         # Index of just added bone
            for child in bone.children:
                goThrough(child, parentIndex)
        
        goThrough(root, 2**16-1)
    else:
        print("Unknown type: " + object.type)
        

# Save the file
datFile = open('C:/Users/Henrik/Documents/visual studio 2013/Projects/Games/HappyLand/HappyLand/models/out.dat', 'wb')

datGroupsToBones = {}
for index, (parentI, bone) in enumerate(datBones):
    if bone.name in datVertexGroups:
        print(bone.name, datVertexGroups[bone.name])
        datGroupsToBones[datVertexGroups[bone.name].index] = index

datFile.write(pack('HHH', len(datVertices), len(datFaces), len(datBones)))
for datVertex in datVertices:
    pos = datVertex[0]
    uv = datVertex[2]
    norm = datVertex[1]
    groups = datVertex[3]
    datFile.write(pack('=fff', pos.x, pos.z, pos.y))
    datFile.write(pack('=ff', uv.x, uv.y))
    datFile.write(pack('=fff', norm.x, norm.z, norm.y))
    
    datFile.write(pack('=BB', datGroupsToBones[groups[0][0]], datGroupsToBones[groups[1][0]]))
    datFile.write(pack('=ff', groups[0][1], groups[1][1]))
    
for datFace in datFaces:
    datFile.write(pack('HHH', datFace[0], datFace[1], datFace[2]))

for datBone in datBones:
    parentIndex = datBone[0]
    bone = datBone[1]
    name = bone.name.encode('ascii')
    head = bone.head_local
    tail = bone.tail - bone.head
    
    datFile.write(pack('=H', len(name)) + name)
    datFile.write(pack('=H', parentIndex))
    datFile.write(pack('=fff', head.x, head.z, head.y))
    datFile.write(pack('=fff', tail.x, tail.z, tail.y))

datFile.close()