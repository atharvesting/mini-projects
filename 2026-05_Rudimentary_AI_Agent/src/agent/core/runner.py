from agent import config
from agent import prompts
from agent.llm.client import Client

class Agent:
    
    def __init__(self):
        
        self.model = config.OLLAMA_MODEL
        self.client = Client(self.model, config.OLLAMA_API_URL)
        self.system_message = prompts.SYS_PROMPT1
        self.messages = []
        
    def ask_agent(self, prompt:str) -> str | None:
        response = self.client.generate(prompt)
        if response is not None: 
            return response.response
        return None