from tasks import task_entry, task_query, task_summary
from notes import note_entry, note_query
from books import book_entry
 
def input_action():
    while True:
        print("What would you like to do?")
        print("q:query, e:entry, s:summary, c:close")
        action = input(">> ").strip().lower()

        if action == "q":
            mode = data_type()
            handle_query(mode)
        elif action == "e":
            mode = data_type()
            handle_entry(mode)
        elif action == "s":
            mode = data_type()
            handle_summary(mode)
        elif action == "c":
            print("PKCLI Shutting down...")
            break
        else:
            print(f"Invalid value '{action}'. Please choose one of the listed options.")

def data_type():
    print("What would you like to work with?")
    print("t:tasks, b:books, n:notes, h:habits")
    data_mode = input(">> ").strip().lower()
    return data_mode

def handle_entry(mode):
    if mode == "t":
        task_entry()
    elif mode == "b":
        book_entry()
    elif mode == "n":
        note_entry()
    elif mode == "h":
        habit_entry()
    else:
        print(f"Invalid entry type '{mode}'. Try again.")

def handle_query(mode):
    # Similarly dispatch query by mode
    if mode == "t":
        task_query()
    elif mode == "n":
        note_query()
    else:
        print(f"No querying supported for '{mode}' yet.")

def handle_summary(mode):
    if mode == "t":
        task_summary()
    elif mode == "h":
        habit_summary()
    else:
        print(f"No summary available for '{mode}'.")
    
print("Welcome to PKCLI!")
print()

input_action()


