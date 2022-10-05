import xml.etree.ElementTree as ET

fileName = 'tetris'

root_node = ET.parse(fileName + '.xml').getroot()

p = root_node.find('part')

for measure in p.findall('measure'):
    for note in measure.findall('note'):
        if note.find('staff') is not None:
            if note.find('staff').text != '1':
                measure.remove(note)
                continue
        if note.find('chord') is not None:
            measure.remove(note)
            continue

