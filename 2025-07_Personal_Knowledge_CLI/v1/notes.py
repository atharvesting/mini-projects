from misc import confirmation, load_data, save_data, entry_date

def note_entry():
    dic = {}
    notes = load_data("notes.json")     
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
    save_data("notes.json", notes, dic, "Book")
    
def note_query():
    notes = load_data("notes.json")
    for item in notes:
        print(f"{item['note_title']}: {item['note_content']}")