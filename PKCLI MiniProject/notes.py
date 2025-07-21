from misc import confirmation, load_data, save_data, entry_date
import json

def note_entry():
    dic = {}
    try:
        with open("notes.json", "r") as notefile:
            notes = json.load(notefile)
    except (FileNotFoundError, json.JSONDecodeError):
        notes = []
        
    while True:
        note_title = input("Note title: ")
        note_contents = input("Note content: ")
        print("Please confirm Note title and Note:")
        print(f"{note_title}: {note_contents}")
        confirm = confirmation("y/n >> ")
        if confirm is True:
            break
        else:
            continue
         
    today_date = entry_date()
    
    dic = {
        "note_title": note_title,
        "note_content": note_contents,
        "date": today_date
    }
    notes.append(dic)
    with open("notes.json", "w") as notefile:
        json.dump(notes, notefile, indent=4)
        
    print("Note entered successfully.")
    print()
    
def note_query():
    with open("notes.json") as notefile:
        notes = json.loads(notefile)
        for item in notes:
            print(item)