import requests
from pydantic import BaseModel, ValidationError
from time import sleep

class ChatResponse(BaseModel):
    response: str
    tool_use: bool
    action: str | None
    action_input: dict | None
    concluded: bool

class Client:
    
    def __init__(self, model: str, base_url:str):
        self.model: str = model
        self.base_url: str = base_url
        self.session: requests.Session = requests.Session()

    def payload(self, 
                messages: list[dict[str, str]],
                system: str | None = None,
                format: dict | None = None,
                think: bool = False
    ):
        if not messages:
            return None
        
        return {
            "model": self.model,
            "stream": False,
            "prompt": str(messages),
            # format clashes with thinking because it enforces strict per-token control to adhere to requried structure.
            # This led to the use of a brain class that is used to generate thought processes, while maintaining strict
            #   formatting in the final answer.
            "format": format, 
            "think": think,
            "keep_alive": "10m",
            "options": {
                "num_ctx": 2048,
                "num_gpu": 99,
                # "low_vram": True,
                "temperature": 0.0
            },
        }
    
    def generate(self, 
                 messages: list[dict[str, str]] = [], 
                 system: str | None = None,
                 format: dict | None = None,
                 think: bool = False,
                 retries: int = 3
        ) -> ChatResponse | None:

        payload = self.payload(messages=messages,
                          system=system,
                          format=format,
                          think=think)

        for _ in range(retries):
            try:
                r = self.session.post(self.base_url, json=payload, timeout=300)
                r.raise_for_status()
                full_response_json = r.json()
                response = full_response_json.get("response", "")
                if format is None:
                    return response
                validated_response = ChatResponse.model_validate_json(response)
                return validated_response
            
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
    

# "response":"action = calculator\naction_input = {\"expression\": \"56 * 372 - 532\"}\nconcluded = True\nresponse = \"19840\"","done":true,