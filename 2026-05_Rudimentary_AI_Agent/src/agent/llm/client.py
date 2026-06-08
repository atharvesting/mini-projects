import requests
from pydantic import BaseModel, ValidationError
from typing import Any
from time import sleep

class ChatResponse(BaseModel):
    response: str
    tool_use: bool
    action: str | None
    action_input: dict | None

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
                "temperature": 0.0
            },
            "prompt": str(messages),
            "think": True,
            "format": ChatResponse.model_json_schema(),
            "keep_alive": "10m"
        }
    
    def generate(self, 
                 messages: list[dict[str, str]] = [], 
                 retries: int = 3
        ) -> tuple[ChatResponse, str] | None:
        
        payload = self.payload(messages)
        for _ in range(retries):
            
            try:
                r = self.session.post(self.base_url, json=payload, timeout=300)
                r.raise_for_status()

                response_data = r.json()
                llm_string = response_data.get("response", "")
                thinking_string = response_data.get("thinking", "")

                validated_llm_string = ChatResponse.model_validate_json(llm_string)
                return validated_llm_string, thinking_string
            
            except ValidationError as e:
                print(e)
                print(r.text)
                sleep(1)
                continue
            except (requests.exceptions.JSONDecodeError,
                    requests.exceptions.HTTPError,
                    requests.exceptions.ConnectTimeout,
                    requests.exceptions.InvalidSchema) as e:
                print(type(e), e)
                sleep(1)
                continue
        return None
            
    