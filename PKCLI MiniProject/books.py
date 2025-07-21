import json
from misc import confirmation, load_data, save_data, status_dic

def book_entry():
    dic = {}
    try:
        with open("books.json", "r") as bookfile:
            books = json.load(bookfile)
    except (FileNotFoundError, json.JSONDecodeError):
        books = []
        
    while True:
        book_title = input("Book title: ")
        book_author = input("Author: ")
        print("Please confirm book title and Author:")
        print(f"{book_title}: {book_author}")
        confirm = confirmation("y/n >> ")
        if confirm is True:
            break
        else:
            continue
    status = input("Status of book (n:not started, f: finished, c:in progress): ")
    
    dic = {
        "note_title": book_title,
        "note_content": book_author,
        "status": status_dic(status)
    }
    
    books.append(dic)
    with open("books.json", "w") as bookfile:
        json.dump(books, bookfile, indent=4)
        
    print("Book entered successfully.")
    print()