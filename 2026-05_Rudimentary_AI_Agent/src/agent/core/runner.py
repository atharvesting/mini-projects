from agent import config
from agent.core.memory import MemoryModule
from agent.llm.client import Client, ChatResponse
from agent.tools.calculator import Calculator

class Brain:
    def __init__(self, thinker_prompt: str):
        self.model = config.OLLAMA_MODEL
        self.client = Client(self.model, config.OLLAMA_API_URL)
        self.thinker_prompt = thinker_prompt

    def think(self, messages: str) -> str:
        return self.client.generate(messages,
                             system=self.thinker_prompt,
                             format=None,
                             think=False)

class Agent:

    def __init__(self, brain_prompt, system_prompt: str):
        self.model = config.OLLAMA_MODEL
        self.client = Client(self.model, config.OLLAMA_API_URL)
        self.brain = Brain(brain_prompt)
        self.mm = MemoryModule(system_prompt)
        self.tools = {
            "calculator": Calculator()
        }
        
    def get_messages(self) -> str:
        return self.mm.get_messages()

    def ask_agent(self, prompt:str) -> str | None:
        
        self.mm.messages.append({"role": "user", "content": prompt})
        thoughts = self.brain.think(self.get_messages())
        print("THOUGHTS::", thoughts)
        response = self.client.generate(self.get_messages() + "\nAssistant Thoughts: " + thoughts,
                                        system=None,
                                        format=ChatResponse.model_json_schema(),
                                        think=False)

        while not response.concluded:
            if response.tool_use == True:
                tool_result = self.tools[response.action](**response.action_input)
                self.mm.messages.append({"role": f"tool_{response.action}", "content": tool_result})
            
            thoughts = self.brain.think(self.get_messages())
            response = self.client.generate(self.get_messages() + "\n" + thoughts,
                                            system=None,
                                            format=ChatResponse.model_json_schema(),
                                            think=False)
        
        if response is not None:
            self.mm.messages.append({"role": "assistant", "content": response.response})
            return response.response
        
        return None
    
