import requests
from pydantic import BaseModel, ValidationError
from typing import Any
from time import sleep

class ChatResponse(BaseModel):
    message: dict[str, str]
    done: bool

class Client:
    
    def __init__(self, model: str, base_url:str):
        self.model: str = model
        self.base_url: str = base_url
        self.session: requests.Session = requests.Session()
    
    def payload(self, messages: list[dict[str, str]]):
        return {
            "model": self.model,
            "stream": False,
            "options": {
                "num_ctx": 2048,
                "num_gpu": 99,
                "low_vram": True,
                "temperature": 0.1
            },
            "messages": messages,
            "keep_alive": "10m"
        }
    
    def generate(self, 
                 messages: list[dict[str, str]] = [], 
                 retries: int = 3
        ) -> ChatResponse | None:
        
        payload = self.payload(messages)
        for _ in range(retries):
            
            try:
                r = self.session.post(self.base_url, json=payload, timeout=300)
                r.raise_for_status()
                return ChatResponse(**(r.json()))
            
            except ValidationError as e:
                print(e)
                print(r.text)
                sleep(1)
                continue
            except requests.exceptions.JSONDecodeError as e:
                print(type(e), e)
                sleep(1)
                continue
            except requests.exceptions.HTTPError as e:
                print(type(e), e)
                sleep(1)
                continue
            except requests.exceptions.ConnectTimeout as e:
                print(type(e), e)
                sleep(1)
                continue
            except requests.exceptions.InvalidSchema as e:
                print(type(e), e)
                sleep(1)
                continue
            
        return None
            
    