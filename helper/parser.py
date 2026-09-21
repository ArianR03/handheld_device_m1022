"""
Helper file that converts .json into .txt files
for parsing in C++.
"""

import json
import os
import glob

file_pattern = "*.json"
json_files = glob.glob(file_pattern)

for file_path in json_files:

    book = os.path.splitext(os.path.basename(file_path))[0]

    with open(file_path, "r", encoding="utf-8") as f:
        data = json.load(f)

    with open(f'{book}.txt', 'w', encoding='utf-8') as txt_file:
        txt_file.write('chapter|verse|text\n')

        for ch in data["chapters"]:
            curr_chapter = ch["chapter"]

            for verse in ch["verses"]:
                verse_num = verse["verse"]
                verse_txt = verse["text"]

                txt_file.write(f'{curr_chapter}|{verse_num}|{verse_txt}\n')

    os.remove(file_path)