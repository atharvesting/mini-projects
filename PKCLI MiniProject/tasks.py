from misc import confirmation, load_data, save_data, entry_date, status_dic
import json
from dateutil import parser
from datetime import datetime

def task_entry():
    dic = {}
    try:
        with open("tasks.json", "r") as taskfile:
            tasks = json.load(taskfile)
            print("loading sucess")
    except (FileNotFoundError, json.JSONDecodeError):
        tasks = []
        print("new list created")
    
    while True:
        try:
            task = input("Task: ")
            do_date = parser.parse(input("Do date (Please use a recognisable format): ")).strftime("%d.%m.%Y")
            status = input("Status of task (n:not started, f: finished, c:in progress): ")
            break
        except:
            print("Please use valid inputs.")
            
    today_date = entry_date()
    
    dic = {
        "task": task,
        "do_date": do_date,
        "entry_date": today_date,
        "status": status_dic(status)
    }
    
    tasks.append(dic)
    with open("tasks.json", "w") as taskfile:
        json.dump(tasks, taskfile, indent=4)
        
    print("Task entered successfully.")
    print()
    
    return None

def task_query():
    query_mode = data_type()
    if query_mode == "t":
        try:
            with open("tasks.json", "r") as taskfile:
                tasks = json.load(taskfile)
        except (FileNotFoundError, json.JSONDecodeError):
            print("No tasks in the base.")
            task_entry()
        
        print("Action (t:today's task list, f: finished tasks, o:overdue):")    

        search_type = input(">> ").lower()
        
        if search_type == "t":
            for item in tasks:
                if item["do_date"] == datetime.today().strftime("%d.%m.%Y"):
                    print(f"{item["task"]}: {item["status"]}")
                
        elif search_type == "f":
            for item in tasks:
                if item["status"] == "f":
                    print(item["task"])
                    
        elif search_type == "o":
            for item in tasks:
                if datetime.strptime(item["do_date"], "%d.%m.%Y") < datetime.today() and item["status"] == "not started" and item["status"] == "in progress":
                    print(f"{item["do_date"]} {item["task"]}: {item["status"]}")

    else:
        print("Choose a valid input.")
        
        
def task_summary():
    pass