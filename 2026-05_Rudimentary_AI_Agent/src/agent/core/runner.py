from agent import config
from agent import prompts
from agent.llm.client import Client

class Agent:
    
    def __init__(self, system_prompt: str):
        
        self.model = config.OLLAMA_MODEL
        self.client = Client(self.model, config.OLLAMA_API_URL)
        self.messages = [{"role": "system", "content": system_prompt}]
        
    def ask_agent(self, prompt:str) -> str | None:
        
        self.messages.append({"role": "user", "content": prompt})
        response = self.client.generate(self.messages)
        
        if response is not None:
            self.messages.append(response.message)
            return response.message["content"]
        
        return None