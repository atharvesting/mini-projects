import json
from misc_new import confirmation, load_data, save_data, entry_date, status_dic

class Task:
    
    def __init__(self, title:str, details:str, status:str):
        self.title = title
        self.details = details
        self.status = status_dic(status)
        
    def remove_task(self, task_name):
        tasks = load_data("task.json")
        for item in tasks:
            if item["task"] == task_name:
                print(item)
                
    def __str__(self):
        return str(load_data("tasks.json"))
                
tasks_obj = Task("PKCLI V2", "Finish the second version of TM using classes.", "n")

print(tasks_obj)

#tasks_obj.remove_task("PKCLI V2")
    