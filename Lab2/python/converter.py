import xml.etree.ElementTree as ET
import os

from ordered_set import T

class Note:
    freq = 0
    dur = 0
    leds = 0
    # def __init__(self, freq, dur):
    #     self.freq = freq
    #     self.duration = dur

pitches = {
    "AB": 415/(2**4),
    "A" : 440/(2**4),
    "AS": 466/(2**4),
    "BB": 466/(2**4),
    "B" : 494/(2**4),
    "BS": 262/(2**4),
    "CB": 262/(2**4),
    "C" : 262/(2**4),
    "CS": 277/(2**4),
    "DB": 277/(2**4),
    "D" : 294/(2**4),
    "DS": 311/(2**4),
    "EB": 311/(2**4),
    "E" : 330/(2**4),
    "ES": 350/(2**4),
    "FB": 350/(2**4),
    "F" : 350/(2**4),
    "FS": 370/(2**4),
    "FS": 370/(2**4),
    "GB": 370/(2**4),
    "G" : 392/(2**4),
    "GS": 415/(2**4)
}

durations = {
    "whole"   : 1,
    "half"    : 2,
    "quarter" : 4,
    "eighth"  : 8,
    "16th"    : 16,
    "32nd"    : 32
}

with open("cfiles/songs.h", "w") as cf:
    cf.write("#ifndef SONGS_H\n")
    cf.write("#define SONGS_H\n\n")
    cf.write("#include \"musicplayer.h\"\n\n")

    songs = []

    for f in os.listdir("./xmls"):

        fileName = f.split(".")[0]

        root_node = ET.parse("xmls/" + fileName + '.xml').getroot()

        p = root_node.find('part')

        try:
            bpm = int(p.find('measure').find('direction').find('sound').get('tempo'))
        except:
            bpm = 60

        print(f"Tempo: {bpm}bpm")

        # Preprocess Music
        for measure in p.findall('measure'):
            for note in measure.findall('note'):
                if note.find('staff') is not None:
                    if note.find('staff').text != '1':
                        measure.remove(note)
                        continue
                if note.find('chord') is not None:
                    measure.remove(note)
                    continue

        ET.ElementTree(root_node).write(f"test_xmls/test_{fileName}.xml")
        
        music = []

        # Parse
        for measure in p.findall('measure'):
            for note in measure.findall('note'):

                n = Note()

                p = note.find('pitch')
                
                # frequency parse
                if p is not None:
                    key = p.find('step').text
                    if p.find('alter') is not None:
                        if int(p.find('alter').text) < 0:
                            key += 'B'
                        if int(p.find('alter').text) > 0:
                            key += 'S'

                    n.freq = int(pitches[key] * 2**int(p.find('octave').text))

                    if key[0] == 'A':
                        n.leds |= 0b0001
                    if key[0] == 'B' or key[0] == 'C':
                        n.leds |= 0b0010
                    if key[0] == 'D' or key[0] == 'E':
                        n.leds |= 0b0100
                    if key[0] == 'F' or key[0] == 'G':
                        n.leds |= 0b1000
                        
                else:
                    n.freq = 0

                # duration parse
                t = note.find('type')
                if t is not None:
                    n.dur |= int(durations[t.text])
                else:
                    continue

                music.append(n)

        cf.write(f"const static Note {fileName}Notes[] = {'{'}\n")

        for n in music:
            cf.write(f"    {'{'}.freq = {n.freq}, .duration = {n.dur}, .leds = {n.leds}{'}'},\n")

        cf.write("};\n\n")

        cf.write(f"const static Song {fileName}Song = {'{'}\n")
        cf.write(f"    .name = \"{fileName}\",\n")
        cf.write(f"    .notes = {fileName}Notes,\n")
        cf.write(f"    .length = {len(music)},\n")
        cf.write(f"    .bpm = {bpm}\n")
        cf.write("};\n\n")

        songs.append(fileName + "Song")

    cf.write(f"const static Song* songList[] = {'{'}\n")
    for name in songs:
        cf.write(f"    &{name},\n")
    cf.write('};\n\n')

    cf.write("#endif")