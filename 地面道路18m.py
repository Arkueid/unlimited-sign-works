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
    cube(1500, 1500, 1500), # 基
    cube(500, 400, 20), # 底板
    cube(200, 8, 9000), # 立柱-横板
    cube(8, 200, 9000), # 立柱-纵板
    cylinder(95, 18000 + 200), # 横梁
    stiffener(20, 103, 12, 50, 200, 0, 0), # 横梁-加劲板1
    cube(200, 12, 41), # 横梁-加劲板2
    cylinder(300, 20), # 法兰盘

]
parts = [
    part(True, 0, "基-左", [-9000, -750, 0], [0, 0, 0], [False for i in range(6)], [4 for i in range(6)]),
    part(True, 0, "基-右", [9000, -750, 0], [0, 0, 0], [False for i in range(6)], [4 for i in range(6)]),
    part(False, 1, "底板-左-前", [-9000, 10, 250], [0, 0, 0], [False for i in range(6)], [8 for i in range(6)]),
    part(False, 1, "底板-左-后", [-9000, 10, -250], [0, 0, 0], [False for i in range(6)], [8 for i in range(6)]),
    part(False, 1, "底板-右-前", [9000, 10, 250], [0, 0, 0], [False for i in range(6)], [8 for i in range(6)]),
    part(False, 1, "底板-右-后", [9000, 10, -250], [0, 0, 0], [False for i in range(6)], [8 for i in range(6)]),
    part(False, 2, "立柱-左-前-前", [-9000, 20 + 4500, 250 + 100], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),
    part(False, 3, "立柱-左-前-中", [-9000, 20 + 4500, 250], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),
    part(False, 2, "立柱-左-前-前", [-9000, 20 + 4500, 250 - 100], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),

    part(False, 2, "立柱-左-后-前", [-9000, 20 + 4500, -250 + 100], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),
    part(False, 3, "立柱-左-后-中", [-9000, 20 + 4500, -250], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),
    part(False, 2, "立柱-左-后-前", [-9000, 20 + 4500, -250 - 100], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),

    part(False, 2, "立柱-右-前-前", [9000, 20 + 4500, 250 + 100], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),
    part(False, 3, "立柱-右-前-中", [9000, 20 + 4500, 250], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),
    part(False, 2, "立柱-右-前-前", [9000, 20 + 4500, 250 - 100], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),

    part(False, 2, "立柱-右-后-前", [9000, 20 + 4500, -250 + 100], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),
    part(False, 3, "立柱-右-后-中", [9000, 20 + 4500, -250], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),
    part(False, 2, "立柱-右-后-前", [9000, 20 + 4500, -250 - 100], [0, 0, 0], [False for i in range(6)], [32 for i in range(6)]),

    part(False, 4, "横梁-前-上", [9000 + 100, 8520, 250], [0, 0, 90], [False for i in range(3)], [7 for i in range(3)]),
    part(False, 4, "横梁-前-下", [9000 + 100, 6920, 250], [0, 0, 90], [False for i in range(3)], [7 for i in range(3)]),
    part(False, 4, "横梁-后-上", [9000 + 100, 8520, -250], [0, 0, 90], [False for i in range(3)], [7 for i in range(3)]),
    part(False, 4, "横梁-后-下", [9000 + 100, 6920, -250], [0, 0, 90], [False for i in range(3)], [7 for i in range(3)]),
]

import math
# 加劲板
r = 95 / 2
for i in range(4):
    parts.append(
        part(False, 5, "横梁-加劲板1-左-前-上-" + str(i), [-9000 + 70 + 200, 8520 + math.cos(i * math.pi / 2) * r, 250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 5, "横梁-加劲板1-左-前-下-" + str(i), [-9000 + 70 + 200, 6920 + math.cos(i * math.pi / 2) * r, 250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 5, "横梁-加劲板1-左-后-上-" + str(i), [-9000 + 70 + 200, 8520 + math.cos(i * math.pi / 2) * r, -250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 5, "横梁-加劲板1-左-后-下-" + str(i), [-9000 + 70 + 200, 6920 + math.cos(i * math.pi / 2) * r, -250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 6, "横梁-加劲板2-左-前-上-" + str(i), [-9000 + 70 + 100, 8520 + math.cos(i * math.pi / 2) * (r + 20.5), 250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 6, "横梁-加劲板2-左-前-下-" + str(i), [-9000 + 70 + 100, 6920 + math.cos(i * math.pi / 2) * (r + 20.5), 250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 6, "横梁-加劲板2-左-后-上-" + str(i), [-9000 + 70 + 100, 8520 + math.cos(i * math.pi / 2) * (r + 20.5), -250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 6, "横梁-加劲板2-左-后-下-" + str(i), [-9000 + 70 + 100, 6920 + math.cos(i * math.pi / 2) * (r + 20.5), -250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    # 右

    parts.append(
        part(False, 5, "横梁-加劲板-右-前-上-" + str(i), [9000 - 70 - 200, 8520 + math.cos(i * math.pi / 2) * r, 250 + math.sin(i * math.pi / 2) * r], [i * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 5, "横梁-加劲板-右-前-下-" + str(i), [9000 - 70 - 200, 6920 + math.cos(i * math.pi / 2) * r, 250 + math.sin(i * math.pi / 2) * r], [i * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 5, "横梁-加劲板-右-后-上-" + str(i), [9000 - 70 - 200, 8520 + math.cos(i * math.pi / 2) * r, -250 + math.sin(i * math.pi / 2) * r], [i * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 5, "横梁-加劲板-右-后-下-" + str(i), [9000 - 70 - 200, 6920 + math.cos(i * math.pi / 2) * r, -250 + math.sin(i * math.pi / 2) * r], [i * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 6, "横梁-加劲板2-右-前-上-" + str(i), [9000 - 70 - 100, 8520 + math.cos(i * math.pi / 2) * (r + 20.5), 250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 6, "横梁-加劲板2-右-前-下-" + str(i), [9000 - 70 - 100, 6920 + math.cos(i * math.pi / 2) * (r + 20.5), 250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 6, "横梁-加劲板2-右-后-上-" + str(i), [9000 - 70 - 100, 8520 + math.cos(i * math.pi / 2) * (r + 20.5), -250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 6, "横梁-加劲板2-右-后-下-" + str(i), [9000 - 70 - 100, 6920 + math.cos(i * math.pi / 2) * (r + 20.5), -250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    # 中心
    parts.append(
        part(False, 5, "横梁-中心加劲板-左-前-上-" + str(i), [-10, 8520 + math.cos(i * math.pi / 2) * r, 250 + math.sin(i * math.pi / 2) * r], [i * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )
    parts.append(
        part(False, 5, "横梁-中心加劲板-右-前-上-" + str(i), [10, 8520 + math.cos(i * math.pi / 2) * r, 250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )
    parts.append(
        part(False, 5, "横梁-中心加劲板-左-前-下-" + str(i), [-10, 6920 + math.cos(i * math.pi / 2) * r, 250 + math.sin(i * math.pi / 2) * r], [i * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )
    parts.append(
        part(False, 5, "横梁-中心加劲板-右-前-下-" + str(i), [10, 6920 + math.cos(i * math.pi / 2) * r, 250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

    parts.append(
        part(False, 5, "横梁-中心加劲板-左-后-上-" + str(i), [-10, 8520 + math.cos(i * math.pi / 2) * r, -250 + math.sin(i * math.pi / 2) * r], [i * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )
    parts.append(
        part(False, 5, "横梁-中心加劲板-右-后-上-" + str(i), [10, 8520 + math.cos(i * math.pi / 2) * r, -250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )
    parts.append(
        part(False, 5, "横梁-中心加劲板-左-后-下-" + str(i), [-10, 6920 + math.cos(i * math.pi / 2) * r, -250 + math.sin(i * math.pi / 2) * r], [i * 90, 180, 0], [False for i in range(7)], [8 for i in range(7)])
    )
    parts.append(
        part(False, 5, "横梁-中心加劲板-右-后-下-" + str(i), [10, 6920 + math.cos(i * math.pi / 2) * r, -250 + math.sin(i * math.pi / 2) * r], [i * 90, 0, 0], [False for i in range(7)], [8 for i in range(7)])
    )

# 法兰盘
parts.append(
    part(False, 7, "法兰盘-左-前-上", [-9000 + 70 + 200, 8520, 250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)
parts.append(
    part(False, 7, "法兰盘-左-前-下", [-9000 + 70 + 200, 6920, 250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)
parts.append(
    part(False, 7, "法兰盘-左-后-上", [-9000 + 70 + 200, 8520, -250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)
parts.append(
    part(False, 7, "法兰盘-左-后-下", [-9000 + 70 + 200, 6920, -250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)

parts.append(
    part(False, 7, "法兰盘-右-前-上", [9000 - 70 - 200, 8520, 250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)
parts.append(
    part(False, 7, "法兰盘-右-前-下", [9000 - 70 - 200, 6920, 250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)
parts.append(
    part(False, 7, "法兰盘-右-后-上", [9000 - 70 - 200, 8520, -250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)
parts.append(
    part(False, 7, "法兰盘-右-后-下", [9000 - 70 - 200, 6920, -250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)

# 中心法兰
parts.append(
    part(False, 7, "中心法兰盘-前-上", [10, 8520, 250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)

parts.append(
    part(False, 7, "中心法兰盘-前-下", [10, 6920, 250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)

parts.append(
    part(False, 7, "中心法兰盘-后-上", [10, 8520, -250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)

parts.append(
    part(False, 7, "中心法兰盘-后-下", [10, 6920, -250], [0, 0, 90], [False for i in range(3)], [8 for i in range(3)]),
)

pole_name = "地面道路18m"
pole_type = POLE_TYPE_GATE

with open(pole_name + ".json", "w", encoding="utf-8") as f:
    json.dump({
        "meshes": meshes,
        "parts": parts,
        "name": pole_name,
        "type": pole_type
    }, f, ensure_ascii=False, indent=4)

