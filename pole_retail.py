import json

with open('地面道路14m.json', 'r', encoding='utf-8') as f:
    data = json.load(f)

for part in data['parts']:
    if '立' in part['name'] or '底' in part['name'] or '基' in part['name'] or '底' in part['name'] or '基' in part['name']:
        continue
    
    part['position'][1] = part['position'][1] - 500

with open('地面道路14m改.json', 'w', encoding='utf-8') as f:
    json.dump(data, f, ensure_ascii=False, indent=4)