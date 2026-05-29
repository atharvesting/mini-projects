from misc import confirmation, load_data, save_data, entry_date, status_dic
from dateutil import parser
from datetime import datetime

def task_entry():
    dic = {}
    tasks = load_data("tasks.json")
    
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
    
    save_data("tasks.json", tasks, dic, "Task")

def task_query():
    tasks = load_data("tasks.json")
    if not tasks:
        print("No tasks in the base.")
        return
    
    print("Action (t:today's task list, f: finished tasks, o:overdue):")    

    search_type = input(">> ").lower()
    
    if search_type == "t":
        for item in tasks:
            if item["do_date"] == datetime.today().strftime("%d.%m.%Y"):
                print(f"{item['task']}: {item['status']}")
            
    elif search_type == "f":
        for item in tasks:
            if item["status"] == status_dic(search_type):
                print(item["task"])
                
    elif search_type == "o":
        for item in tasks:
            if datetime.strptime(item["do_date"], "%d.%m.%Y") < datetime.today() and item["status"] in ["not started", "in progress"]:
                print(f"{item['do_date']} {item['task']}: {item['status']}")

    else:
        print("Choose a valid input.")
        
        
def task_summary():
    tasks = load_data("tasks.json")
    
    
    