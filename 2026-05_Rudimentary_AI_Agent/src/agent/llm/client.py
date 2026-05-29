import requests
from pydantic import BaseModel
from typing import Any

class ChatResponse(BaseModel):
    response: str
    done: bool
    eval_count: int

class Client:
    
    def __init__(self, model: str, base_url:str):
        self.model: str = model
        self.base_url: str = base_url
        self.session: requests.Session = requests.Session()
        
    def parse_response_json(self, r: dict[str, Any]) -> ChatResponse:
        return ChatResponse(**r)
        
    def payload(self, prompt:str, ):
        return {
            "model": self.model,
            "prompt": prompt,
            "stream": False,
            "options": {
                "num_ctx": 2048,
                "num_gpu": 99,
                "low_vram": True,
                "temperature": 0.1
            }
        }
    
    def generate(self, prompt:str, retries:int = 3) -> ChatResponse | None:
        payload = self.payload(prompt)
        for _ in range(retries):
            
            try:
                r = self.session.post(self.base_url, json=payload, timeout=300)
                r.raise_for_status()
                return self.parse_response_json(r.json())
            
            except requests.exceptions.InvalidJSONError as e:
                print(type(e), e)
                continue
            except requests.exceptions.HTTPError as e:
                print(type(e), e)
                continue
            except requests.exceptions.ConnectTimeout as e:
                print(type(e), e)
                continue
            except requests.exceptions.InvalidSchema as e:
                print(type(e), e)
                continue
            
        return None
            
    