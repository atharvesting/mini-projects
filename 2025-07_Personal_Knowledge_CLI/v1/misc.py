import json
from datetime import datetime

def confirmation(input_string: str):
    while True:
        confirm = input(input_string).strip().lower()
        if confirm == "y":
            return True
        elif confirm == "n":
            return False
        else:
            print("Please input a valid value.") 
            
def load_data(filename):
    try:
        with open(filename, "r") as f:
            return json.load(f)
    except (FileNotFoundError, json.JSONDecodeError):
        return []

def save_data(filename: str, data: list, dic: dict, object: str):
    data.append(dic)
    with open(filename, "w") as f:
        json.dump(data, f, indent=4)
    print(f"{object} entered successfully")
    print()
        
def entry_date():
    today_date = datetime.today().strftime("%d.%m.%Y")  
    return today_date

def status_dic(status: str):
    status_options = {"n": "not started", "f": "finished", "c": "in progress"}
    result = status_options[status]
    return result

def mode_dic(mode: str):
    mode_options = {"q": "query", "e": 'entry', "s": "summary", "c": "close"}
    result = mode_options[mode]
    return result