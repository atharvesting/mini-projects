import json
from misc import confirmation, load_data, save_data, status_dic, entry_date

def book_entry():
    dic = {}
    books = load_data("books.json")
    while True:
        book_title = input("Book title: ")
        book_author = input("Author: ")
        print("Please confirm book title and Author:")
        print(f"{book_title}: {book_author}")
        confirm = confirmation("y/n >> ")
        if confirm is True:
            break
        else:
            print()
            continue
    status = input("Status of book (n:not started, f: finished, c:in progress): ")
    today_date = entry_date()
    
    dic = {
        "book_title": book_title,
        "book_author": book_author,
        "status": status_dic(status),
        "entry_date": today_date
    }
    save_data("books.json", books, dic, "Book")
    
def book_query():
    try:
        books = load_data("books.json")
        for item in books:
            print(f"{item['book_title']}: {item['book_author']}")
    except (FileNotFoundError, json.JSONDecodeError):
        print("Error occured. Try adding a book entry first.")
        book_entry()