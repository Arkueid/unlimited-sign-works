import json

MESH_TYPE_CUBE = 0
MESH_TYPE_CYLINDER = 1
MESH_TYPE_STIFFENER = 2
MESH_TYPE_ROUNDED_CUBE = 3
MESH_TYPE_ROUNDED_TRIPRISM = 4

POLE_TYPE_SINGLE = 0
POLE_TYPE_DOUBLE = 1
POLE_TYPE_GATE = 2
POLE_TYPE_SINGLE_ARM = 3
POLE_TYPE_DOUBLE_ARM = 4

def cube(a, b, c) -> dict:
    return {
        "type": MESH_TYPE_CUBE,
        "a" : a,
        "b" : b,
        "c" : c,
    }

def cylinder(d, h) -> dict:
    return {
        "type": MESH_TYPE_CYLINDER,
        "d": d,
        "h": h,
    }
    
def stiffener(a, b, c, h1, h2, offsetH, offsetV) -> dict:
    return {
        "type": MESH_TYPE_STIFFENER,
        "a" : a,
        "b" : b,
        "c" : c,
        "h1" : h1,
        "h2" : h2,
        "offsetH": offsetH,
        "offsetV": offsetV,
    }
    
def rounded_cube(a, b, c, r) -> dict:
    return {
        "type": MESH_TYPE_ROUNDED_CUBE,
        "a" : a,
        "b" : b,
        "c" : c,
        "radius" : r,
    }
    
def rounded_triprism(a, h, r) -> dict:
    return {
        "type": MESH_TYPE_ROUNDED_TRIPRISM,
        "a" : a,
        "h" : h,
        "radius" : r,
    }


def part(isPoleBase, meshRef, name, position, rotation, textureEdits, textureRefs) -> dict:
    return {
        "isPoleBase": isPoleBase,
        "meshRef": meshRef,
        "name": name,
        "position": position,
        "rotation": rotation,
        "textureEdits": textureEdits,
        "textureRefs": textureRefs
    }

meshes = [
    cube(1500, 1500, 1500), # 基底
    cube(750, 750, 20), # 底板
    cylinder(377, 9000), # 立柱
    cylinder(203, 14000), # 横梁
    cylinder(60, 797), # 直腹杆
    cylinder(60, 1254), # 斜腹杆
    stiffener(50, 300, 12, 50, 140, 20, 0), # 底加劲板
    stiffener(40, 90, 12, 242, 390, 100, 0), # 横梁加劲板
    cylinder(390, 20), # 法兰盘
]
parts = [
    part(True, 0, "基底-左", [-7000, -750, 0], [0, 0, 0], [False for i in range(6)], [4 for i in range(6)]),
    part(True, 0, "基底-右", [7000, -750, 0], [0, 0, 0], [False for i in range(6)], [4 for i in range(6)]),
    part(False, 1, "底板-左", [-7000, 10, 0], [0, 0, 0], [False for i in range(6)], [8 for i in range(6)]),
    part(False, 1, "底板-右", [7000, 10, 0], [0, 0, 0], [False for i in range(6)], [8 for i in range(6)]),
    part(False, 2, "立柱-左", [-7000, 20, 0], [0, 0, 0], [False for i in range(3)], [7 for i in range(3)]),
    part(False, 2, "立柱-右", [7000, 20, 0], [0, 0, 0], [False for i in range(3)], [7 for i in range(3)]),
    part(False, 3, "横梁-上", [7000, 8620, 0], [0, 0, 90], [False for i in range(3)], [7 for i in range(3)]),
    part(False, 3, "横梁-中", [7000, 7620, 0], [0, 0, 90], [False for i in range(3)], [7 for i in range(3)]),
    part(False, 3, "横梁-下", [7000, 6620, 0], [0, 0, 90], [False for i in range(3)], [7 for i in range(3)]),
]

# 直腹杆
for i in range(13):
    parts.append(
        part(False, 4, "直腹杆-上-" + str(i + 1), [-7000 + 700 + i * 1050, 7620 + 101.5, 0], [0, 0, 0], [False for i in range(3)], [7 for i in range(3)])
    )
    parts.append(
        part(False, 4, "直腹杆-下-" + str(i + 1), [-7000 + 700 + i * 1050, 6620 + 101.5, 0], [0, 0, 0], [False for i in range(3)], [7 for i in range(3)])
    )

# 斜腹杆
for i in range(6):
    parts.append(
        part(False, 5, "斜腹杆-上-" + str(i + 1) + "-1", [-7000 + 700 + 1050 + i * 2 * 1050 - 20 - 40, 7620 + 101.5 - 20, 0], [0, 0, 48], [False for i in range(3)], [7 for i in range(3)])
    )
    parts.append(
        part(False, 5, "斜腹杆-上-" + str(i + 1) + "-2", [-7000 + 700 + 1050 + i * 2 * 1050 + 20 + 40, 7620 + 101.5 - 20, 0], [0, 0, -48], [False for i in range(3)], [7 for i in range(3)])
    )

    parts.append(
        part(False, 5, "斜腹杆-下-" + str(i + 1) + "-1", [-7000 + 700 + i * 2 * 1050 + 20 + 40, 6620 + 101.5 - 20, 0], [0, 0, -48], [False for i in range(3)], [7 for i in range(3)])
    )
    parts.append(
        part(False, 5, "斜腹杆-下-" + str(i + 1) + "-2", [-7000 + 700 + 1050 * 2 + i * 2 * 1050 - 20 - 40, 6620 + 101.5 - 20, 0], [0, 0, 48], [False for i in range(3)], [7 for i in range(3)])
    )

import math
# 底加劲板
for i in range(8):
    parts.append(
        part(False, 6, "底加劲板-左-" + str(i + 1), [-7000 + math.cos(i * math.pi / 4) * 377 / 2, 20, -math.sin(i * math.pi / 4) * 377 / 2], [0, i * 45, 0], [False for i in range(7)], [8 for i in range(7)])
    )
    parts.append(
        part(False, 6, "底加劲板-右-" + str(i + 1), [7000 + math.cos(i * math.pi / 4) * 377 / 2, 20, -math.sin(i * math.pi / 4) * 377 / 2], [0, i * 45, 0], [False for i in range(7)], [8 for i in range(7)])
    )

# 横梁加劲板
r = 203 / 2
mr = 377 / 2
for i in range(1, 5):
    parts.append(
        part(False, 7, "横梁加劲板-左-上-" + str(i + 1), [-7000 + mr, 8620 + math.sin(i * math.pi / 2) * r, - math.cos(i * math.pi / 2) * r], [(i - 1) * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 7, "横梁加劲板-左-中-" + str(i + 1), [-7000 + mr, 7620 + math.sin(i * math.pi / 2) * r, - math.cos(i * math.pi / 2) * r], [(i - 1) * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 7, "横梁加劲板-左-下-" + str(i + 1), [-7000 + mr, 6620 + math.sin(i * math.pi / 2) * r, - math.cos(i * math.pi / 2) * r], [(i - 1) * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 7, "横梁加劲板-右-上-" + str(i + 1), [7000 - mr, 8620 + math.sin(i * math.pi / 2) * r, - math.cos(i * math.pi / 2) * r], [(i - 1) * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 7, "横梁加劲板-右-中-" + str(i + 1), [7000 - mr, 7620 + math.sin(i * math.pi / 2) * r, - math.cos(i * math.pi / 2) * r], [(i - 1) * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 7, "横梁加劲板-右-下-" + str(i + 1), [7000 - mr, 6620 + math.sin(i * math.pi / 2) * r, - math.cos(i * math.pi / 2) * r], [(i - 1) * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )

# 法兰盘
parts.append(
    part(False, 8, "法兰盘-左-" + str(i + 1), [-7000 + mr + 159, 8620, 0], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)])
)
parts.append(
    part(False, 8, "法兰盘-左-" + str(i + 1), [-7000 + mr + 159, 7620, 0], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)])
)
parts.append(
    part(False, 8, "法兰盘-左-" + str(i + 1), [-7000 + mr + 159, 6620, 0], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)])
)
parts.append(
    part(False, 8, "法兰盘-右-" + str(i + 1), [7000 - mr - 159, 8620, 0], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)])
)
parts.append(
    part(False, 8, "法兰盘-右-" + str(i + 1), [7000 - mr - 159, 7620, 0], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)])
)
parts.append(
    part(False, 8, "法兰盘-右-" + str(i + 1), [7000 - mr - 159, 6620, 0], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)])
)


pole_name = "地面道路14m"
pole_type = POLE_TYPE_GATE

with open(pole_name + ".json", "w", encoding="utf-8") as f:
    json.dump({
        "meshes": meshes,
        "parts": parts,
        "name": pole_name,
        "type": pole_type
    }, f, ensure_ascii=False, indent=4)

