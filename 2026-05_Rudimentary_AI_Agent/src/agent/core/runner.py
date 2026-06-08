from agent import config
from agent import prompts
from agent.core.memory import MemoryModule
from agent.llm.client import Client

class Agent:
    
    def __init__(self, system_prompt: str):
        
        self.model = config.OLLAMA_MODEL
        self.client = Client(self.model, config.OLLAMA_API_URL)
        self.mm = MemoryModule(system_prompt)
        
    def get_messages(self):
        return self.mm.get_messages()

    def ask_agent(self, prompt:str) -> str | None:
        
        self.mm.messages.append({"role": "user", "content": prompt})
        response = self.client.generate(self.get_messages())
        thinking = response[1]
        actual = response[0] 
        
        if response is not None:
            self.mm.messages.append({"role": "assistant", "content": actual.response})
            return actual.response, thinking
        
        return None
    
